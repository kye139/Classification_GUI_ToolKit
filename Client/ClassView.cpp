
#include "stdafx.h"
#include "MainFrm.h"
#include "ClassView.h"
#include "Resource.h"
#include "ImageTool.h"
#include "LearingParameterDlg.h"
#include <memory>

class CClassViewMenuButton : public CMFCToolBarMenuButton
{
	friend class CClassView;

	DECLARE_SERIAL(CClassViewMenuButton)

public:
	CClassViewMenuButton(HMENU hMenu = NULL) : CMFCToolBarMenuButton((UINT)-1, hMenu, -1)
	{
	}

	virtual void OnDraw(CDC* pDC, const CRect& rect, CMFCToolBarImages* pImages, BOOL bHorz = TRUE,
		BOOL bCustomizeMode = FALSE, BOOL bHighlight = FALSE, BOOL bDrawBorder = TRUE, BOOL bGrayDisabledButtons = TRUE)
	{
		pImages = CMFCToolBar::GetImages();

		CAfxDrawState ds;
		pImages->PrepareDrawImage(ds);

		CMFCToolBarMenuButton::OnDraw(pDC, rect, pImages, bHorz, bCustomizeMode, bHighlight, bDrawBorder, bGrayDisabledButtons);

		pImages->EndDrawImage(ds);
	}
};

IMPLEMENT_SERIAL(CClassViewMenuButton, CMFCToolBarMenuButton, 1)

//////////////////////////////////////////////////////////////////////
// ����/�Ҹ�
//////////////////////////////////////////////////////////////////////

CClassView::CClassView()
	: ckbox(false)
{
	m_nCurrSort = ID_SORTING_GROUPBYTYPE;
}

CClassView::~CClassView()
{
}

BEGIN_MESSAGE_MAP(CClassView, CDockablePane)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_WM_CONTEXTMENU()
	ON_COMMAND(ID_CLASS_ADD_MEMBER_FUNCTION, OnClassAddMemberFunction)
	ON_COMMAND(ID_CLASS_ADD_MEMBER_VARIABLE, OnClassAddMemberVariable)
	ON_COMMAND(ID_CLASS_DEFINITION, OnClassDefinition)
	ON_COMMAND(ID_CLASS_PROPERTIES, OnClassProperties)
	ON_COMMAND(ID_NEW_FOLDER, OnNewFolder)
	ON_WM_PAINT()
	ON_WM_SETFOCUS()
	ON_COMMAND_RANGE(ID_SORTING_GROUPBYTYPE, ID_SORTING_SORTBYACCESS, OnSort)
	ON_UPDATE_COMMAND_UI_RANGE(ID_SORTING_GROUPBYTYPE, ID_SORTING_SORTBYACCESS, OnUpdateSort)
//	ON_WM_LBUTTONDOWN()
ON_UPDATE_COMMAND_UI(ID_DATA_CHECKBOX, &CClassView::OnUpdateDataCheckbox)
ON_COMMAND(ID_DATA_CHECKBOX, &CClassView::OnDataCheckbox)
ON_UPDATE_COMMAND_UI(ID_DATA_LOAD, &CClassView::OnUpdateDataLoad)
ON_COMMAND(ID_DATA_LOAD, &CClassView::OnDataLoad)
ON_COMMAND(ID_DATA_DOWNLOAD, &CClassView::OnDataDownload)
ON_COMMAND(ID_DATA_LEARNING, &CClassView::OnDataLearning)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CClassView �޽��� ó����

int CClassView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDockablePane::OnCreate(lpCreateStruct) == -1)
		return -1;

	CRect rectDummy;
	rectDummy.SetRectEmpty();

	// �並 ����ϴ�.
	const DWORD dwViewStyle = WS_CHILD | WS_VISIBLE | TVS_HASLINES | TVS_LINESATROOT | TVS_HASBUTTONS | WS_CLIPSIBLINGS | WS_CLIPCHILDREN;

	if (!m_wndClassView.Create(dwViewStyle, rectDummy, this, 2))
	{
		TRACE0("Ŭ���� �並 ������ ���߽��ϴ�.\n");
		return -1;      // ������ ���߽��ϴ�.
	}

	// �̹����� �ε��մϴ�.
	m_wndToolBar.Create(this, AFX_DEFAULT_TOOLBAR_STYLE, IDR_DATA_VIEW_TOOLBAR);
	m_wndToolBar.LoadToolBar(IDR_DATA_VIEW_TOOLBAR, 0, 0, TRUE /* ��� */);

	OnChangeVisualStyle();

	m_wndToolBar.SetPaneStyle(m_wndToolBar.GetPaneStyle() | CBRS_TOOLTIPS | CBRS_FLYBY);
	m_wndToolBar.SetPaneStyle(m_wndToolBar.GetPaneStyle() & ~(CBRS_GRIPPER | CBRS_SIZE_DYNAMIC | CBRS_BORDER_TOP | CBRS_BORDER_BOTTOM | CBRS_BORDER_LEFT | CBRS_BORDER_RIGHT));

	m_wndToolBar.SetOwner(this);

	// ��� ����� �θ� �������� �ƴ� �� ��Ʈ���� ���� ����õ˴ϴ�.
	m_wndToolBar.SetRouteCommandsViaFrame(FALSE);

	CMenu menuSort;
	menuSort.LoadMenu(IDR_POPUP_SORT);

	m_wndToolBar.ReplaceButton(ID_SORT_MENU, CClassViewMenuButton(menuSort.GetSubMenu(0)->GetSafeHmenu()));

	CClassViewMenuButton* pButton =  DYNAMIC_DOWNCAST(CClassViewMenuButton, m_wndToolBar.GetButton(0));

	if (pButton != NULL)
	{
		pButton->m_bText = FALSE;
		pButton->m_bImage = TRUE;
		pButton->SetImage(GetCmdMgr()->GetCmdImage(m_nCurrSort));
		pButton->SetMessageWnd(this);
	}

	// ���� Ʈ�� �� �����͸� ���� �ڵ�� ä��ϴ�.
	FillClassView();

	return 0;
}

void CClassView::OnSize(UINT nType, int cx, int cy)
{
	CDockablePane::OnSize(nType, cx, cy);
	AdjustLayout();
}

void CClassView::FillClassView()
{
	HTREEITEM hRoot = m_wndClassView.InsertItem(_T("�̹��� ������"), 0, 0);
	m_wndClassView.SetItemState(hRoot, TVIS_BOLD, TVIS_BOLD);

	// HTREEITEM hClass = m_wndClassView.InsertItem(_T("CFakeAboutDlg"), 1, 1, hRoot);
	// m_wndClassView.InsertItem(_T("CFakeAboutDlg()"), 3, 3, hClass);
	// 
	// m_wndClassView.Expand(hRoot, TVE_EXPAND);
	// 
	// hClass = m_wndClassView.InsertItem(_T("CFakeApp"), 1, 1, hRoot);
	// m_wndClassView.InsertItem(_T("CFakeApp()"), 3, 3, hClass);
	// m_wndClassView.InsertItem(_T("InitInstance()"), 3, 3, hClass);
	// m_wndClassView.InsertItem(_T("OnAppAbout()"), 3, 3, hClass);
	// 
	// hClass = m_wndClassView.InsertItem(_T("CFakeAppDoc"), 1, 1, hRoot);
	// m_wndClassView.InsertItem(_T("CFakeAppDoc()"), 4, 4, hClass);
	// m_wndClassView.InsertItem(_T("~CFakeAppDoc()"), 3, 3, hClass);
	// m_wndClassView.InsertItem(_T("OnNewDocument()"), 3, 3, hClass);
	// 
	// hClass = m_wndClassView.InsertItem(_T("CFakeAppView"), 1, 1, hRoot);
	// m_wndClassView.InsertItem(_T("CFakeAppView()"), 4, 4, hClass);
	// m_wndClassView.InsertItem(_T("~CFakeAppView()"), 3, 3, hClass);
	// m_wndClassView.InsertItem(_T("GetDocument()"), 3, 3, hClass);
	// m_wndClassView.Expand(hClass, TVE_EXPAND);
	// 
	// hClass = m_wndClassView.InsertItem(_T("CFakeAppFrame"), 1, 1, hRoot);
	// m_wndClassView.InsertItem(_T("CFakeAppFrame()"), 3, 3, hClass);
	// m_wndClassView.InsertItem(_T("~CFakeAppFrame()"), 3, 3, hClass);
	// m_wndClassView.InsertItem(_T("m_wndMenuBar"), 6, 6, hClass);
	// m_wndClassView.InsertItem(_T("m_wndToolBar"), 6, 6, hClass);
	// m_wndClassView.InsertItem(_T("m_wndStatusBar"), 6, 6, hClass);
	// 
	// hClass = m_wndClassView.InsertItem(_T("Globals"), 2, 2, hRoot);
	// m_wndClassView.InsertItem(_T("theFakeApp"), 5, 5, hClass);
	// m_wndClassView.Expand(hClass, TVE_EXPAND);
}

void CClassView::OnContextMenu(CWnd* pWnd, CPoint point)
{
	CTreeCtrl* pWndTree = (CTreeCtrl*)&m_wndClassView;
	ASSERT_VALID(pWndTree);

	if (pWnd != pWndTree)
	{
		CDockablePane::OnContextMenu(pWnd, point);
		return;
	}

	if (point != CPoint(-1, -1))
	{
		// Ŭ���� �׸��� �����մϴ�.
		CPoint ptTree = point;
		pWndTree->ScreenToClient(&ptTree);

		UINT flags = 0;
		HTREEITEM hTreeItem = pWndTree->HitTest(ptTree, &flags);
		if (hTreeItem != NULL)
		{
			pWndTree->SelectItem(hTreeItem);
		}
	}

	pWndTree->SetFocus();
	CMenu menu;
	menu.LoadMenu(IDR_POPUP_SORT);

	CMenu* pSumMenu = menu.GetSubMenu(0);

	if (AfxGetMainWnd()->IsKindOf(RUNTIME_CLASS(CMDIFrameWndEx)))
	{
		CMFCPopupMenu* pPopupMenu = new CMFCPopupMenu;

		if (!pPopupMenu->Create(this, point.x, point.y, (HMENU)pSumMenu->m_hMenu, FALSE, TRUE))
			return;

		((CMDIFrameWndEx*)AfxGetMainWnd())->OnShowPopupMenu(pPopupMenu);
		UpdateDialogControls(this, FALSE);
	}
}

void CClassView::AdjustLayout()
{
	if (GetSafeHwnd() == NULL)
	{
		return;
	}

	CRect rectClient;
	GetClientRect(rectClient);

	int cyTlb = m_wndToolBar.CalcFixedLayout(FALSE, TRUE).cy;

	m_wndToolBar.SetWindowPos(NULL, rectClient.left, rectClient.top, rectClient.Width(), cyTlb, SWP_NOACTIVATE | SWP_NOZORDER);
	m_wndClassView.SetWindowPos(NULL, rectClient.left + 1, rectClient.top + cyTlb + 1, rectClient.Width() - 2, rectClient.Height() - cyTlb - 2, SWP_NOACTIVATE | SWP_NOZORDER);
}

BOOL CClassView::PreTranslateMessage(MSG* pMsg)
{
	return CDockablePane::PreTranslateMessage(pMsg);
}

void CClassView::OnSort(UINT id)
{
	if (m_nCurrSort == id)
	{
		return;
	}

	m_nCurrSort = id;

	CClassViewMenuButton* pButton =  DYNAMIC_DOWNCAST(CClassViewMenuButton, m_wndToolBar.GetButton(0));

	if (pButton != NULL)
	{
		pButton->SetImage(GetCmdMgr()->GetCmdImage(id));
		m_wndToolBar.Invalidate();
		m_wndToolBar.UpdateWindow();
	}
}

void CClassView::OnUpdateSort(CCmdUI* pCmdUI)
{
	pCmdUI->SetCheck(pCmdUI->m_nID == m_nCurrSort);
}

void CClassView::OnClassAddMemberFunction()
{
	AfxMessageBox(_T("��� �Լ� �߰�..."));
}

void CClassView::OnClassAddMemberVariable()
{
	// TODO: ���⿡ ��� ó���� �ڵ带 �߰��մϴ�.
}

void CClassView::OnClassDefinition()
{
	// TODO: ���⿡ ��� ó���� �ڵ带 �߰��մϴ�.
}

void CClassView::OnClassProperties()
{
	// TODO: ���⿡ ��� ó���� �ڵ带 �߰��մϴ�.
}

void CClassView::SelToCString(void)
{
	m_vSelString.clear();

	HTREEITEM cursor = m_wndClassView.GetChildItem(m_wndClassView.GetRootItem());
	int labelNum = m_wndClassView.m_vSelectList.size();
	int targetIndex = 0;

	// ���õ� �׸� �̸� ���ڿ� ã��
	while (cursor)
	{
		// ����� name�� �� ã�� ���� ���� �����Ѱ�� �ٽ� ó������
		HTREEITEM next = m_wndClassView.GetNextItem(cursor, TVGN_NEXT);
		if (!next)
			cursor = m_wndClassView.GetChildItem(m_wndClassView.GetRootItem());

		int cursorId = *(int*)m_wndClassView.GetItemData(cursor);
		if (m_wndClassView.m_vSelectList[targetIndex] == cursorId)
		{
			m_vSelString.push_back(m_wndClassView.GetItemText(cursor));
			targetIndex++;

			// �� ã�� ���
			if (targetIndex >= labelNum)
				break;
		}

		cursor = next;
	}
}

void CClassView::hideCheckBox(HTREEITEM hItem)
{
	if (hItem == NULL)
		return;

	m_wndClassView.SetItemState(hItem, INDEXTOSTATEIMAGEMASK(0), TVIS_STATEIMAGEMASK);

	hideCheckBox(m_wndClassView.GetChildItem(hItem));
	hideCheckBox(m_wndClassView.GetNextItem(hItem, TVGN_NEXT));
}
/*
void CClassView::ToggleFirstLayerCheckBox(HTREEITEM root)
{
	HTREEITEM firstLevelPointer = m_wndClassView.GetChildItem(root);
	HTREEITEM childPointer = m_wndClassView.GetChildItem(firstLevelPointer);

	while (firstLevelPointer)
	{
		while (childPointer)
		{
			hideCheckBox(childPointer);
			childPointer = m_wndClassView.GetNextItem(childPointer, TVGN_NEXT);
		}
		firstLevelPointer = m_wndClassView.GetNextItem(firstLevelPointer, TVGN_NEXT);
		childPointer = m_wndClassView.GetChildItem(firstLevelPointer);
	}
}
*/
void CClassView::OnNewFolder()
{
	// üũ�ڽ��� Ȱ��ȭ�Ǿ� �ִ� ���
	if (ckbox)
	{
		//m_wndFileView.SetExtendedStyle(nowStyle, nowStyle &= ~TVS_CHECKBOXES);
		hideCheckBox(m_wndClassView.GetRootItem());
		m_wndClassView.ModifyStyle(TVS_CHECKBOXES, 0);
		ckbox = false;
	}
	// üũ�ڽ��� ��Ȱ��ȭ �Ǿ� �ִ� ���
	else
	{
		//m_wndFileView.SetExtendedStyle(nowStyle, TVS_CHECKBOXES);
		// First remove the checkbox style
		m_wndClassView.ModifyStyle(TVS_CHECKBOXES, 0);
		// Then explicitly set it
		m_wndClassView.ModifyStyle(0, TVS_CHECKBOXES);
		//ToggleFirstLayerCheckBox(m_wndClassView.GetRootItem());
		ckbox = true;
	}
}

void CClassView::OnPaint()
{
	CPaintDC dc(this); // �׸��⸦ ���� ����̽� ���ؽ�Ʈ�Դϴ�.

	CRect rectTree;
	m_wndClassView.GetWindowRect(rectTree);
	ScreenToClient(rectTree);

	rectTree.InflateRect(1, 1);
	dc.Draw3dRect(rectTree, ::GetSysColor(COLOR_3DSHADOW), ::GetSysColor(COLOR_3DSHADOW));
}

void CClassView::OnSetFocus(CWnd* pOldWnd)
{
	CDockablePane::OnSetFocus(pOldWnd);

	m_wndClassView.SetFocus();
}

void CClassView::OnChangeVisualStyle()
{
	m_ClassViewImages.DeleteImageList();

	UINT uiBmpId = theApp.m_bHiColorIcons ? IDB_CLASS_VIEW_24 : IDB_CLASS_VIEW;

	CBitmap bmp;
	if (!bmp.LoadBitmap(uiBmpId))
	{
		TRACE(_T("��Ʈ���� �ε��� �� �����ϴ�. %x\n"), uiBmpId);
		ASSERT(FALSE);
		return;
	}

	BITMAP bmpObj;
	bmp.GetBitmap(&bmpObj);

	UINT nFlags = ILC_MASK;

	nFlags |= (theApp.m_bHiColorIcons) ? ILC_COLOR24 : ILC_COLOR4;

	m_ClassViewImages.Create(16, bmpObj.bmHeight, nFlags, 0, 0);
	m_ClassViewImages.Add(&bmp, RGB(255, 0, 0));

	m_wndClassView.SetImageList(&m_ClassViewImages, TVSIL_NORMAL);

	m_wndToolBar.CleanUpLockedImages();
	m_wndToolBar.LoadBitmap(theApp.m_bHiColorIcons ? IDB_DATA_VIEW_TOOLBAR : IDR_SORT, 0, 0, TRUE /* ��� */);
}

void CClassView::OnUpdateDataCheckbox(CCmdUI *pCmdUI)
{
	// TODO: ���⿡ ��� ������Ʈ UI ó���� �ڵ带 �߰��մϴ�.
	pCmdUI->Enable(TRUE);
}


void CClassView::OnDataCheckbox()
{
	// TODO: ���⿡ ��� ó���� �ڵ带 �߰��մϴ�.
	// DWORD nowStyle = m_wndFileView.GetExtendedStyle();

	// üũ�ڽ��� Ȱ��ȭ�Ǿ� �ִ� ���
	if (ckbox)
	{
		//m_wndFileView.SetExtendedStyle(nowStyle, nowStyle &= ~TVS_CHECKBOXES);
		hideCheckBox(m_wndClassView.GetRootItem());
		m_wndClassView.ModifyStyle(TVS_CHECKBOXES, 0);
		m_wndClassView.m_vSelectList.clear();
		ckbox = false;
	}
	// üũ�ڽ��� ��Ȱ��ȭ �Ǿ� �ִ� ���
	else
	{
		HTREEITEM child = m_wndClassView.GetChildItem(m_wndClassView.GetRootItem());
		m_wndClassView.Expand(child, TVE_COLLAPSE);
		while (child = m_wndClassView.GetNextItem(child, TVGN_NEXT))
			m_wndClassView.Expand(child, TVE_COLLAPSE);
		//m_wndFileView.SetExtendedStyle(nowStyle, TVS_CHECKBOXES);
		// First remove the checkbox style
		m_wndClassView.ModifyStyle(TVS_CHECKBOXES, 0);
		// Then explicitly set it
		m_wndClassView.ModifyStyle(0, TVS_CHECKBOXES);
		child = m_wndClassView.GetChildItem(m_wndClassView.GetRootItem());
		m_wndClassView.SetItemState(m_wndClassView.GetRootItem(), INDEXTOSTATEIMAGEMASK(0), TVIS_STATEIMAGEMASK);
		HTREEITEM hideChild = m_wndClassView.GetChildItem(child);
		m_wndClassView.SetItemState(hideChild, INDEXTOSTATEIMAGEMASK(0), TVIS_STATEIMAGEMASK);
		while (hideChild = m_wndClassView.GetNextItem(hideChild, TVGN_NEXT))
			m_wndClassView.SetItemState(hideChild, INDEXTOSTATEIMAGEMASK(0), TVIS_STATEIMAGEMASK);
		while (child = m_wndClassView.GetNextItem(child, TVGN_NEXT))
		{
			hideChild = m_wndClassView.GetChildItem(child);
			m_wndClassView.SetItemState(hideChild, INDEXTOSTATEIMAGEMASK(0), TVIS_STATEIMAGEMASK);
			while (hideChild = m_wndClassView.GetNextItem(hideChild, TVGN_NEXT))
				m_wndClassView.SetItemState(hideChild, INDEXTOSTATEIMAGEMASK(0), TVIS_STATEIMAGEMASK);
		}
		ckbox = true;
	}
}


void CClassView::OnUpdateDataLoad(CCmdUI *pCmdUI)
{
	// TODO: ���⿡ ��� ������Ʈ UI ó���� �ڵ带 �߰��մϴ�.
	pCmdUI->Enable(TRUE);
}


void CClassView::OnDataLoad()
{
	// TODO: ���⿡ ��� ó���� �ڵ带 �߰��մϴ�.
	int requestCode = 13;
	CMainFrame* frame = (CMainFrame *)AfxGetMainWnd();

	packet pack{ requestCode, 0, NULL };
	sendToServ(frame->hSocket, &pack);
	recvFromServ(frame->hSocket, &pack);

	if (pack.n_opcode == 13)
	{
		int dirNum = *(int *)pack.cptr_payload;
		CDataTree* dataTree = &frame->m_wndClassView.m_wndClassView;
		HTREEITEM hRoot = dataTree->GetRootItem();
		char* startPointer = pack.cptr_payload + sizeof(int);

		for (int i = 0; i < dirNum; i++)
		{
			int nameLen = *(int*)startPointer;
			char * name = (char*)malloc(nameLen + 1);
			memcpy(name, startPointer + sizeof(int), nameLen);
			name[nameLen] = 0;
			CString strName = CA2T(name);
			CString countFile;

			int count = *(int*)(startPointer + sizeof(int) + nameLen);
			memcpy(name, &count, sizeof(int));
			countFile.Format(_T("%d"), count);

			// Ʈ���� ������ ����
			HTREEITEM hItem = dataTree->InsertItem(strName + CString(_T("(")) + countFile + CString(_T(")")), hRoot, TVI_LAST);

			// �����ۿ� ���̵� ����
			int* _id = new int(theApp._id++);
			dataTree->SetItemData(hItem, (DWORD_PTR)(_id));

			startPointer = startPointer + sizeof(int) + nameLen + sizeof(int);

			free(name);
		}
	}
	else
	{
		AfxMessageBox(_T("�������� ��ſ� �����߽��ϴ�."));
	}

}


void CClassView::OnDataDownload()
{
	// TODO: ���⿡ ��� ó���� �ڵ带 �߰��մϴ�.

	if (m_wndClassView.m_vSelectList.size() == 0)
	{
		AfxMessageBox(_T("�ٿ�ε��� ���̺��� �������ּ���."));
		return;
	}

	// ������ ���� ��� ��������
	CString savePath;
	GetDirPathDialog(&savePath, GetSafeHwnd(), _T("���� ��� ����"));
	if (savePath.IsEmpty())
		return;

	// ��û ���̺��� ������ ������ ����
	int labelNum = m_wndClassView.m_vSelectList.size();
	SelToCString();

	// ���̷ε� ����
	int sizePayload = 0;
	for (int i = 0; i < m_vSelString.size(); i++)
		sizePayload = sizePayload + sizeof(int) + m_vSelString[i].GetLength();
	sizePayload += sizeof(int);

	char * payload = (char *)malloc(sizeof(sizePayload));
	char* startPointer = payload;

	memcpy(startPointer, &labelNum, sizeof(int));
	startPointer += sizeof(int);
	for (int i = 0; i < m_vSelString.size(); i++)
	{
		int nameLen = m_vSelString[i].GetLength();
		memcpy(startPointer, &nameLen, sizeof(int));
		startPointer += sizeof(int);
		memcpy(startPointer, CT2A(m_vSelString[i]), nameLen);
		startPointer += nameLen;
	}
	
	// ������ ��Ŷ ����
	int requestCode = 17;
	CMainFrame* frame = (CMainFrame *)AfxGetMainWnd();
	packet pack = { requestCode, sizePayload, payload };
	
	sendToServ(frame->hSocket, &pack);
	recvFromServ(frame->hSocket, &pack);

	if (pack.n_opcode == 17)
	{
		// ���� �޴� ������ ����
		startPointer = pack.cptr_payload;
		int zipNum = *(int *)startPointer;
		startPointer += sizeof(int);
		for (int i = 0; i < zipNum; ++i)
		{
			int len = *(int *)startPointer;
			startPointer += sizeof(int);
			char * zipName = (char *)malloc(len);
			memcpy(zipName, startPointer, len);
			zipName[len] = 0;
			startPointer += len;
			int zipLen = *(int *)startPointer;
			startPointer += sizeof(int);

			// ������ ���� ���� ��Ʈ�� ����
			FILE * fp = NULL;
			errno_t err = fopen_s(&fp, zipName, "wb");
			if (err != 0)
			{
				AfxMessageBox(_T("���� ���� ����"));
				return;
			}
			
			fwrite(startPointer, zipLen, 1, fp);
			fclose(fp);
		}

		AfxMessageBox(_T("���� �ٿ�ε尡 �Ϸ�Ǿ����ϴ�."));
		
	}
}


void CClassView::OnDataLearning()
{
	// TODO: ���⿡ ��� ó���� �ڵ带 �߰��մϴ�.
	MessageBox(_T("�н��� �ϱ� ���� �Ű�� �並 ���� �ε����ּ���!"), _T("���!"), MB_ICONERROR | MB_OK);

	CLearingParameterDlg dlg;
	if (dlg.DoModal() == IDOK)
	{
		// ���õ� ���̺� ��� ��������
		SelToCString();

		int payloadSize = 0;
		payloadSize += sizeof(int); // �Ű�� �̸� ����
		payloadSize += dlg.m_strNetName.GetLength(); // �Ű�� �̸�
		payloadSize += sizeof(int); // �н��� ���̺� ����
		
		for (int i = 0; i < m_vSelString.size(); ++i)
		{
			payloadSize += sizeof(int); // ���̺�[i]�� ���ڿ� ����
			payloadSize += m_vSelString[i].GetLength(); // ���̺�[i]�� ���ڿ�
		}

		char* payload = (char *)malloc(payloadSize);
		char* startPointer = payload;

		// �Ű�� �̸� ���� ����
		int netNameSize = dlg.m_strNetName.GetLength();
		memcpy(startPointer, &netNameSize, sizeof(int));
		startPointer += sizeof(int);

		// �Ű�� �̸� ����
		memcpy(startPointer, CT2A(dlg.m_strNetName), netNameSize);
		startPointer += netNameSize;

		// �н��� ���̺� ���� ����
		int labelNum = m_vSelString.size();
		memcpy(startPointer, &labelNum, sizeof(int));
		startPointer += sizeof(int);

		// ���̺� ����
		for (int i = 0; i < labelNum; ++i)
		{
			// ���̺� �̸� ���� ����
			int labelNameSize = m_vSelString[i].GetLength();
			memcpy(startPointer, &labelNameSize, sizeof(int));
			startPointer += sizeof(int);

			// ���̺� �̸� ����
			memcpy(startPointer, CT2A(m_vSelString[i]), labelNameSize);
			startPointer += labelNameSize;
		}

		// opcode 3�� �۽�
		CMainFrame* frame = (CMainFrame *)AfxGetMainWnd();
		int requestCode = 3;
		packet pack = { requestCode, payloadSize, payload };

		sendToServ(frame->hSocket, &pack);

		// opcode 4�� �۽�
		payloadSize = sizeof(int) + sizeof(int); // �Ķ���� 2���� ���� ũ��
		payload = (char *)malloc(payloadSize);
		startPointer = payload;

		memcpy(startPointer, &(dlg.m_nLoopCount), sizeof(int));
		startPointer += sizeof(int);
		memcpy(startPointer, &(dlg.m_nTestPeriod), sizeof(int));

		requestCode = 4;
		pack = { requestCode, payloadSize, payload };

		sendToServ(frame->hSocket, &pack);
	}
}
