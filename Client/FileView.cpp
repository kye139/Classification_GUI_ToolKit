
#include "stdafx.h"
#include "mainfrm.h"
#include "FileView.h"
#include "Resource.h"
#include "ImageTool.h"
#include "NewGroupDlg.h"
#include "zip_utils\zip.h"

#pragma warning(disable : 4996)


#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

/////////////////////////////////////////////////////////////////////////////
// CFileView

CFileView::CFileView()
	: tempCut(NULL), ckbox(false)
{
	wcscpy_s(cpBoard.name, _T("\0"));
	cpBoard.uniqueId = -1;
	cpBoard.image = NULL;
}

CFileView::~CFileView()
{
}

BEGIN_MESSAGE_MAP(CFileView, CDockablePane)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_WM_CONTEXTMENU()
	ON_COMMAND(ID_PROPERTIES, OnProperties)
	ON_COMMAND(ID_OPEN, OnFileOpen)
	ON_COMMAND(ID_OPEN_WITH, OnFileOpenWith)
	ON_COMMAND(ID_DUMMY_COMPILE, OnDummyCompile)
	ON_COMMAND(ID_EDIT_CUT, OnEditCut)
	ON_COMMAND(ID_EDIT_COPY, OnEditCopy)
	ON_COMMAND(ID_EDIT_CLEAR, OnEditClear)
	ON_WM_PAINT()
	ON_WM_SETFOCUS()
	ON_UPDATE_COMMAND_UI(ID_EDIT_COPY, &CFileView::OnUpdateEditCopy)
	ON_COMMAND(ID_EDIT_PASTE, &CFileView::OnEditPaste)
	ON_UPDATE_COMMAND_UI(ID_EDIT_PASTE, &CFileView::OnUpdateEditPaste)
	ON_UPDATE_COMMAND_UI(ID_EDIT_CUT, &CFileView::OnUpdateEditCut)
	ON_UPDATE_COMMAND_UI(ID_EDIT_CLEAR, &CFileView::OnUpdateEditClear)
	ON_COMMAND(ID_NEW_GROUP, &CFileView::OnNewGroup)
	ON_COMMAND(ID_MULTI_SELECT, &CFileView::OnMultiSelect)
	ON_COMMAND(ID_IMAGE_TRANSPORT, &CFileView::OnImageTransport)
	ON_UPDATE_COMMAND_UI(ID_IMAGE_TRANSPORT, &CFileView::OnUpdateImageTransport)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CWorkspaceBar �޽��� ó����

int CFileView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDockablePane::OnCreate(lpCreateStruct) == -1)
		return -1;

	CRect rectDummy;
	rectDummy.SetRectEmpty();

	// �並 ����ϴ�.
	const DWORD dwViewStyle = WS_CHILD | WS_VISIBLE | TVS_HASLINES | TVS_LINESATROOT | TVS_HASBUTTONS | TVS_EDITLABELS | TVS_FULLROWSELECT;

	if (!m_wndFileView.Create(dwViewStyle, rectDummy, this, 4))
	{
		TRACE0("���� �並 ������ ���߽��ϴ�.\n");
		return -1;      // ������ ���߽��ϴ�.
	}

	// �� �̹����� �ε��մϴ�.
	m_FileViewImages.Create(IDB_FILE_VIEW, 16, 0, RGB(255, 0, 255));
	m_wndFileView.SetImageList(&m_FileViewImages, TVSIL_NORMAL);

	m_wndToolBar.Create(this, AFX_DEFAULT_TOOLBAR_STYLE, IDR_EXPLORER);
	m_wndToolBar.LoadToolBar(IDR_EXPLORER, 0, 0, TRUE /* ��� */);

	OnChangeVisualStyle();

	m_wndToolBar.SetPaneStyle(m_wndToolBar.GetPaneStyle() | CBRS_TOOLTIPS | CBRS_FLYBY);

	m_wndToolBar.SetPaneStyle(m_wndToolBar.GetPaneStyle() & ~(CBRS_GRIPPER | CBRS_SIZE_DYNAMIC | CBRS_BORDER_TOP | CBRS_BORDER_BOTTOM | CBRS_BORDER_LEFT | CBRS_BORDER_RIGHT));

	m_wndToolBar.SetOwner(this);

	// ��� ����� �θ� �������� �ƴ� �� ��Ʈ���� ���� ����õ˴ϴ�.
	m_wndToolBar.SetRouteCommandsViaFrame(FALSE);

	// ���� Ʈ�� �� �����͸� ���� �ڵ�� ä��ϴ�.
	FillFileView();
	AdjustLayout();

	return 0;
}

void CFileView::OnSize(UINT nType, int cx, int cy)
{
	CDockablePane::OnSize(nType, cx, cy);
	AdjustLayout();
}

void CFileView::FillFileView()
{
	HTREEITEM hRoot = m_wndFileView.InsertItem(_T("�̹��� ����"), 0, 0);
	m_wndFileView.SetItemState(hRoot, TVIS_BOLD, TVIS_BOLD);

	HTREEITEM hSrc = m_wndFileView.InsertItem(_T("Images"), 0, 0, hRoot);
	int* uniqueGroupId = (int*)malloc(sizeof(int));
	*uniqueGroupId = theApp._id++;
	m_wndFileView.SetItemData(hSrc, (DWORD_PTR)uniqueGroupId);

	m_wndFileView.Expand(hRoot, TVE_EXPAND);
	m_wndFileView.Expand(hSrc, TVE_EXPAND);
}

void CFileView::OnContextMenu(CWnd* pWnd, CPoint point)
{
	CTreeCtrl* pWndTree = (CTreeCtrl*) &m_wndFileView;
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
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EXPLORER, point.x, point.y, this, TRUE);
}

void CFileView::AdjustLayout()
{
	if (GetSafeHwnd() == NULL)
	{
		return;
	}

	CRect rectClient;
	GetClientRect(rectClient);

	int cyTlb = m_wndToolBar.CalcFixedLayout(FALSE, TRUE).cy;

	m_wndToolBar.SetWindowPos(NULL, rectClient.left, rectClient.top, rectClient.Width(), cyTlb, SWP_NOACTIVATE | SWP_NOZORDER);
	m_wndFileView.SetWindowPos(NULL, rectClient.left + 1, rectClient.top + cyTlb + 1, rectClient.Width() - 2, rectClient.Height() - cyTlb - 2, SWP_NOACTIVATE | SWP_NOZORDER);
}

void CFileView::OnProperties()
{
	AfxMessageBox(_T("�Ӽ�...."));

}

void CFileView::OnFileOpen()
{
	// TODO: ���⿡ ��� ó���� �ڵ带 �߰��մϴ�.
}

void CFileView::OnFileOpenWith()
{
	// TODO: ���⿡ ��� ó���� �ڵ带 �߰��մϴ�.
}

void CFileView::OnDummyCompile()
{
	// TODO: ���⿡ ��� ó���� �ڵ带 �߰��մϴ�.
}

void CFileView::OnEditCut()
{
	// TODO: ���⿡ ��� ó���� �ڵ带 �߰��մϴ�.
	// ���� �׸� ����
	HTREEITEM selItem = m_wndFileView.GetSelectedItem();

	// �߶󳻱��� ��� �����׸��� �ּҸ� ����.
	tempCut = selItem;

	// ��� �帮�� ���帯
	//m_wndFileView.SetTextColor(RGB(238, 238, 238));

	AfxMessageBox(_T("�߶󳻱� �Ϸ�"));
}

void CFileView::OnEditCopy()
{
	// TODO: ���⿡ ��� ó���� �ڵ带 �߰��մϴ�.
	// ���� �׸� ����
	HTREEITEM selItem = m_wndFileView.GetSelectedItem();
	ImageInform* selItemInfo = (ImageInform *)m_wndFileView.GetItemData(selItem);

	// ������ ��� uniqueId ������ �ʿ䰡 ����.
	cpBoard.uniqueId = -1;

	// ������ �̹��� �������� Ŭ������� ����.
	wcscpy_s(cpBoard.name, selItemInfo->name);
	if (tempCut)
		tempCut = NULL;
	if (cpBoard.image)
		cpBoard.image = NULL;
	cpBoard.image = selItemInfo->image;
	AfxMessageBox(_T("���� �Ϸ�"));
}

void CFileView::OnEditClear()
{
	// TODO: ���⿡ ��� ó���� �ڵ带 �߰��մϴ�.
	// ���� �׸� ����
	HTREEITEM selItem = m_wndFileView.GetSelectedItem();
	ImageInform* selItemInfo = (ImageInform *)m_wndFileView.GetItemData(selItem);

	// �����׸��� �θ�
	HTREEITEM parentItem = m_wndFileView.GetParentItem(selItem);

	// �׸��� ������ ���
	if (selItemInfo)
	{
		wchar_t id[10] = { 0, };
		wsprintf(id, _T("%d"), selItemInfo->uniqueId);
		HWND mdi_parent = ::FindWindowEx(AfxGetMainWnd()->m_hWnd, NULL, _T("MDIClient"), NULL);
		HWND mdi_child = ::FindWindowEx(mdi_parent, NULL, id, NULL);
		// �����찡 �����ִ� ���
		if (mdi_child != NULL)
			::DestroyWindow(mdi_child);

		// �����׸��� IppImage �Ҹ�
		if (selItemInfo->image)
			delete selItemInfo->image;

		// ���� �׸� ImageInform ��ü �Ҹ�
		delete selItemInfo;

		// ���� �信�� �׸� ����
		m_wndFileView.DeleteItem(selItem);
	}
	else
	// �׸��� �׷��� ���
	{
		// �׷� �ȿ� ������ �����ϴ� ���
		if (m_wndFileView.ItemHasChildren(selItem))
		{
			AfxMessageBox(_T("�׷� �ȿ� ������ �����ϴ� ��� �׷� ������ �� �� �����ϴ�."));
			return;
		}
		// �׷� �ȿ� ������ ���� ���
		else
		{
			m_wndFileView.DeleteItem(selItem);
		}
	}

	// ���ΰ�ħ
	m_wndFileView.Expand(parentItem, TVE_EXPAND);
}

void CFileView::OnPaint()
{
	CPaintDC dc(this); // �׸��⸦ ���� ����̽� ���ؽ�Ʈ�Դϴ�.

	CRect rectTree;
	m_wndFileView.GetWindowRect(rectTree);
	ScreenToClient(rectTree);

	rectTree.InflateRect(1, 1);
	dc.Draw3dRect(rectTree, ::GetSysColor(COLOR_3DSHADOW), ::GetSysColor(COLOR_3DSHADOW));
}

void CFileView::OnSetFocus(CWnd* pOldWnd)
{
	CDockablePane::OnSetFocus(pOldWnd);

	m_wndFileView.SetFocus();
}

void CFileView::OnChangeVisualStyle()
{
	m_wndToolBar.CleanUpLockedImages();
	m_wndToolBar.LoadBitmap(theApp.m_bHiColorIcons ? IDB_EXPLORER_24 : IDR_EXPLORER, 0, 0, TRUE /* ��� */);

	m_FileViewImages.DeleteImageList();

	UINT uiBmpId = theApp.m_bHiColorIcons ? IDB_FILE_VIEW_24 : IDB_FILE_VIEW;

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

	m_FileViewImages.Create(16, bmpObj.bmHeight, nFlags, 0, 0);
	m_FileViewImages.Add(&bmp, RGB(255, 0, 255));

	m_wndFileView.SetImageList(&m_FileViewImages, TVSIL_NORMAL);
}

void CFileView::OnUpdateEditCopy(CCmdUI *pCmdUI)
{
	// TODO: ���⿡ ��� ������Ʈ UI ó���� �ڵ带 �߰��մϴ�.
	CViewTree* fileView = &m_wndFileView;
	HTREEITEM selItem = fileView->GetSelectedItem();
	if (!fileView->GetItemData(selItem))
		pCmdUI->Enable(FALSE);
	else
		pCmdUI->Enable(TRUE);
}

BOOL CFileView::CheckName(HTREEITEM targetItem, TCHAR* name)
{
	// �̸� �ߺ� ���� �˻�
	// targetItem�� ������ �̹��� �����̾�� ��.
	ImageInform* targetItemInfo = (ImageInform *)m_wndFileView.GetItemData(targetItem);

	// targetitem�� ���͸��� ���
	if (!targetItemInfo)
		return false;

	// target ������� �̸� �ߺ� ���� �˻�
	if (!_tcscmp(targetItemInfo->name, name))
	{
		MessageBox(_T("������ �̸��� ������ �����մϴ�."));
		return false;
	}
	
	HTREEITEM pointItem = targetItem;
	ImageInform* imgInfo;

	while (pointItem = m_wndFileView.GetNextSiblingItem(pointItem))
	{
		imgInfo = (ImageInform *)m_wndFileView.GetItemData(pointItem);

		// ������ �׸��� �׷��� ���
		if (!imgInfo)
			continue;
		// �̸� ���� ������ �����ϴ� ���
		if (!_tcscmp(imgInfo->name, name))
		{
			MessageBox(_T("������ �̸��� ������ �����մϴ�."));
			return false;
		}
	}
	pointItem = targetItem;
	while (pointItem = m_wndFileView.GetPrevSiblingItem(pointItem))
	{
		imgInfo = (ImageInform *)m_wndFileView.GetItemData(pointItem);

		// ������ �׸��� �׷��� ���
		if (!imgInfo)
			continue;
		// �̸� ���� ������ �����ϴ� ���
		if (!_tcscmp(imgInfo->name, name))
		{
			MessageBox(_T("������ �̸��� ������ �����մϴ�."));
			return false;
		}
	}

	return true;
}

void CFileView::OnEditPaste()
{
	// TODO: ���⿡ ��� ó���� �ڵ带 �߰��մϴ�.
	HTREEITEM selItem = m_wndFileView.GetSelectedItem();
	ImageInform* selItemInfo = (ImageInform *)m_wndFileView.GetItemData(selItem);

	// �׷��� ��������� �̺�Ʈ
	if (!selItemInfo)
	{
		// �߶󳻱��� ���
		if (tempCut)
		{
			// �߶󳻱��� �׸� ������ ����
			ImageInform * cutInfo = (ImageInform *)m_wndFileView.GetItemData(tempCut);

			// �ٿ��ֱ� Ÿ�� �̸� �ߺ� ���� �˻�
			if (m_wndFileView.ItemHasChildren(selItem) && !CheckName(m_wndFileView.GetChildItem(selItem), cutInfo->name))
				return;

			// Dest �׸� �߰�
			// HTREEITEM pasteItem = m_wndFileView.InsertItem(cutInfo->name, 2, 2, m_wndFileView.GetParentItem(selItem));
			HTREEITEM pasteItem = m_wndFileView.InsertItem(cutInfo->name, 2, 2, selItem);
			m_wndFileView.SetItemData(pasteItem, (DWORD_PTR)cutInfo);

			// src �׸� ����
			m_wndFileView.DeleteItem(tempCut);
			tempCut = NULL;
		}
		else
		// ������ ���
		{
			// �ٿ��ֱ� Ÿ�� �̸� �ߺ� ���� �˻�
			if (m_wndFileView.ItemHasChildren(selItem) && !CheckName(m_wndFileView.GetChildItem(selItem), cpBoard.name))
				return;

			// �����ϴ� �̹��� ���� ����
			ImageInform* pasteImageInfo = new ImageInform;
			wcscat_s(cpBoard.name, _T(".cpy"));
			wcscpy_s(pasteImageInfo->name, cpBoard.name);
			pasteImageInfo->uniqueId = theApp._id++;
			pasteImageInfo->image = new IppImage(cpBoard.image);

			// ���Ϻ信 �׸� �߰�
			HTREEITEM pasteItem = m_wndFileView.InsertItem(CString(pasteImageInfo->name), 2, 2, selItem);
			m_wndFileView.SetItemData(pasteItem, (DWORD_PTR)pasteImageInfo);

			// Ŭ������ �̹��� ���� �ʱ�ȭ
			cpBoard.uniqueId = -1;
			cpBoard.image = NULL;
		}

		// ���� �� ���ΰ�ħ
		m_wndFileView.Expand(selItem, TVE_EXPAND);
	}
	else
	// ������ ������� �� �̺�Ʈ
	{
		// �߶󳻱��� ���
		if (tempCut)
		{
			// �߶󳻱��� �׸� ������ ����
			ImageInform * cutInfo = (ImageInform *)m_wndFileView.GetItemData(tempCut);

			// �ٿ��ֱ� Ÿ�� �̸� �ߺ� ���� �˻�
			if (CheckName(selItem, cutInfo->name))
			{
				// Dest �׸� �߰�
				HTREEITEM pasteItem = m_wndFileView.InsertItem(cutInfo->name, 2, 2, m_wndFileView.GetParentItem(selItem));
				m_wndFileView.SetItemData(pasteItem, (DWORD_PTR)cutInfo);
			}

			// src �׸� ����
			m_wndFileView.DeleteItem(tempCut);
			tempCut = NULL;
		}
		else
		// ������ ���
		{
			// �ٿ��ֱ� Ÿ�� �̸� �ߺ� ���� �˻�
			if (CheckName(selItem, cpBoard.name))
			{
				// �����ϴ� �̹��� ���� ����
				ImageInform* pasteImageInfo = new ImageInform;
				wcscpy_s(pasteImageInfo->name, cpBoard.name);
				pasteImageInfo->uniqueId = theApp._id++;
				pasteImageInfo->image = new IppImage(cpBoard.image);

				// ���Ϻ信 �׸� �߰�
				HTREEITEM pasteItem = m_wndFileView.InsertItem(CString(pasteImageInfo->name) + CString(_T(".cpy")), 2, 2, m_wndFileView.GetParentItem(selItem));
				m_wndFileView.SetItemData(pasteItem, (DWORD_PTR)pasteImageInfo);
			}

			// Ŭ������ �̹��� ���� �ʱ�ȭ
			cpBoard.uniqueId = -1;
			cpBoard.image = NULL;
		}

		// ���� �� ���ΰ�ħ
		m_wndFileView.Expand(m_wndFileView.GetParentItem(selItem), TVE_EXPAND);
	}
}


void CFileView::OnUpdateEditPaste(CCmdUI *pCmdUI)
{
	// TODO: ���⿡ ��� ������Ʈ UI ó���� �ڵ带 �߰��մϴ�.
	// ī�ǵ� �׸��� �ִ� ���
	if (cpBoard.image || tempCut)
	{
		// ��Ʈ �׷��� ������ ���
		if (m_wndFileView.GetFirstVisibleItem() == m_wndFileView.GetSelectedItem())
			pCmdUI->Enable(FALSE);
		pCmdUI->Enable(TRUE);
	}
	else
		// ī�ǵ� �׸��� ���� ���
		pCmdUI->Enable(FALSE);

}


void CFileView::OnUpdateEditCut(CCmdUI *pCmdUI)
{
	// TODO: ���⿡ ��� ������Ʈ UI ó���� �ڵ带 �߰��մϴ�.
	HTREEITEM selItem = m_wndFileView.GetSelectedItem();
	if (!m_wndFileView.GetItemData(selItem))
		pCmdUI->Enable(FALSE);
	else
		pCmdUI->Enable(TRUE);
}


void CFileView::OnUpdateEditClear(CCmdUI *pCmdUI)
{
	// TODO: ���⿡ ��� ������Ʈ UI ó���� �ڵ带 �߰��մϴ�.
	HTREEITEM root = m_wndFileView.GetRootItem();
	HTREEITEM sel = m_wndFileView.GetSelectedItem();
	// ��Ʈ �׷��� ������ ��� ��Ȱ��.
	if (root == sel || m_wndFileView.GetChildItem(root) == sel)
		pCmdUI->Enable(FALSE);
	else
		pCmdUI->Enable(TRUE);
}


void CFileView::OnNewGroup()
{
	// TODO: ���⿡ ��� ó���� �ڵ带 �߰��մϴ�.
	CNewGroupDlg dlg;
	if (dlg.DoModal() == IDOK)
	{
		// HTREEITEM selItem = m_wndFileView.GetSelectedItem();
		// ImageInform* selImageInfo = (ImageInform *)m_wndFileView.GetItemData(selItem);
		// // ���õ� �׸��� �׷���ΰ��
		// if (!selImageInfo)
		// 	m_wndFileView.InsertItem(dlg.m_nGroupName, 0, 0, selItem, TVI_FIRST);
		// // ���õ� �׸��� �̹����ΰ��
		// else
		// 	m_wndFileView.InsertItem(dlg.m_nGroupName, 0, 0, m_wndFileView.GetParentItem(selItem), TVI_FIRST);
		// ��� �׷� ���� child�� ����
		HTREEITEM newGroup = m_wndFileView.InsertItem(dlg.m_nGroupName, 0, 0, m_wndFileView.GetRootItem(), TVI_LAST);
		int * uniqueGroupId = (int*)malloc(sizeof(int));
		*uniqueGroupId = theApp._id++;
		m_wndFileView.SetItemData(newGroup, (DWORD_PTR)uniqueGroupId);
	}
}

void CFileView::hideCheckBox(HTREEITEM hItem)
{
	if (hItem == NULL)
		return;

	m_wndFileView.SetItemState(hItem, INDEXTOSTATEIMAGEMASK(0), TVIS_STATEIMAGEMASK);

	hideCheckBox(m_wndFileView.GetChildItem(hItem));
	hideCheckBox(m_wndFileView.GetNextItem(hItem, TVGN_NEXT));

}


void CFileView::OnMultiSelect()
{
	// TODO: ���⿡ ��� ó���� �ڵ带 �߰��մϴ�.
	// DWORD nowStyle = m_wndFileView.GetExtendedStyle();

	// üũ�ڽ��� Ȱ��ȭ�Ǿ� �ִ� ���
	if (ckbox)
	{
		//m_wndFileView.SetExtendedStyle(nowStyle, nowStyle &= ~TVS_CHECKBOXES);
		hideCheckBox(m_wndFileView.GetRootItem());
		m_wndFileView.ModifyStyle(TVS_CHECKBOXES, 0);
		m_wndFileView.m_vSelectList.clear();
		ckbox = false;
	}
	// üũ�ڽ��� ��Ȱ��ȭ �Ǿ� �ִ� ���
	else
	{
		HTREEITEM child = m_wndFileView.GetChildItem(m_wndFileView.GetRootItem());
		m_wndFileView.Expand(child, TVE_COLLAPSE);
		while (child = m_wndFileView.GetNextItem(child, TVGN_NEXT))
			m_wndFileView.Expand(child, TVE_COLLAPSE);
		//m_wndFileView.SetExtendedStyle(nowStyle, TVS_CHECKBOXES);
		// First remove the checkbox style
		m_wndFileView.ModifyStyle(TVS_CHECKBOXES, 0);
		// Then explicitly set it
		m_wndFileView.ModifyStyle(0, TVS_CHECKBOXES);
		child = m_wndFileView.GetChildItem(m_wndFileView.GetRootItem());
		m_wndFileView.SetItemState(m_wndFileView.GetRootItem(), INDEXTOSTATEIMAGEMASK(0), TVIS_STATEIMAGEMASK);
		HTREEITEM hideChild = m_wndFileView.GetChildItem(child);
		m_wndFileView.SetItemState(hideChild, INDEXTOSTATEIMAGEMASK(0), TVIS_STATEIMAGEMASK);
		while (hideChild = m_wndFileView.GetNextItem(hideChild, TVGN_NEXT))
			m_wndFileView.SetItemState(hideChild, INDEXTOSTATEIMAGEMASK(0), TVIS_STATEIMAGEMASK);
		while (child = m_wndFileView.GetNextItem(child, TVGN_NEXT))
		{
			hideChild = m_wndFileView.GetChildItem(child);
			m_wndFileView.SetItemState(hideChild, INDEXTOSTATEIMAGEMASK(0), TVIS_STATEIMAGEMASK);
			while(hideChild = m_wndFileView.GetNextItem(hideChild, TVGN_NEXT))
				m_wndFileView.SetItemState(hideChild, INDEXTOSTATEIMAGEMASK(0), TVIS_STATEIMAGEMASK);
		}
		ckbox = true;
	}
}

void CFileView::OnImageTransport()
{
	// TODO: ���⿡ ��� ó���� �ڵ带 �߰��մϴ�.
	CString savePath;

	GetDirPathDialog(&savePath, GetSafeHwnd(), _T("���� ��� ����"));
	if (savePath.IsEmpty())
		return;

	// üũ�� �׸�� ���� �� ����
	for (std::vector<int>::iterator iter = m_wndFileView.m_vSelectList.begin(); iter != m_wndFileView.m_vSelectList.end(); ++iter)
	{
		HTREEITEM childIndex = m_wndFileView.GetChildItem(m_wndFileView.GetRootItem());
		int * id = (int *)m_wndFileView.GetItemData(childIndex);
		if (*id != *iter)
		{
			while (childIndex = m_wndFileView.GetNextItem(childIndex, TVGN_NEXT))
			{
				id = (int *)m_wndFileView.GetItemData(childIndex);
				if (*id == *iter)
					break;
			}
		}
		CString dirName = m_wndFileView.GetItemText(childIndex); // ������ ���丮 �̸�
		CString fullName = savePath + CString(_T("\\")) + dirName; // ���͸� ���� ���

		if (GetFileAttributes((LPCTSTR)fullName) == 0xFFFFFFFF)
			CreateDirectory((LPCTSTR)fullName, NULL);

		CString zipName = fullName + CString(_T(".zip"));

		// ���� �̸��� ���������� �����ϴ� ��� ���� �� �ٽ� �������� ���� ����
		if (!GetFileAttributes((LPCTSTR)zipName) == 0xFFFFFFFF)
			DeleteFile((LPCTSTR)zipName);

		HZIP hz = CreateZip(zipName, 0); // ���� ���� ����

		HTREEITEM imageIndex = m_wndFileView.GetChildItem(childIndex);
		ImageInform * inform = (ImageInform *)m_wndFileView.GetItemData(imageIndex);

		CString imageName = m_wndFileView.GetItemText(imageIndex); // �̹��� ���� �̸�

		// �̹��� ���� Ȯ���� �˻�
		CString ext = imageName.Right(4);
		if (ext.Compare(_T(".jpg")) && ext.Compare(_T(".bmp")) && ext.Compare(_T(".png")))
			imageName += CString(_T(".bmp"));

		CString resultPath = fullName + CString(_T("\\")) + imageName; // �̹��� ���� ���� ���
		inform->image->Save(resultPath);
		ZipAdd(hz, imageName, resultPath);
		while (imageIndex = m_wndFileView.GetNextItem(imageIndex, TVGN_NEXT))
		{
			inform = (ImageInform *)m_wndFileView.GetItemData(imageIndex);
			imageName = m_wndFileView.GetItemText(imageIndex);
			resultPath = fullName + CString(_T("\\")) + imageName;
			inform->image->Save(resultPath);
			ZipAdd(hz, imageName, resultPath);
		}
		CloseZip(hz);

		// ������� ���� ���� ����
		packet pack;

		int opcode = 2;
		int namelen = dirName.GetLength(); // �������� �̸��� ����
		char* name = CT2A(dirName); // �������� �̸�
		struct _stat statbuf;
		int statResult = _stat(CT2A(zipName), &statbuf); // statbuf.st_size => �ش� ������ ũ��
		if (statResult != 0)
		{
			AfxMessageBox(_T("�������� Ž�� ����"));
			return;
		}
		else
		{
			int payloadLen = sizeof(int) + namelen + statbuf.st_size; // ��ü ���̷ε��� ����
			char* payload = (char*)malloc(payloadLen); // ���̷ε� ���� ����

			memcpy(payload, &namelen, sizeof(int));
			memcpy(payload + sizeof(int), name, namelen);

			FILE* fp = _wfopen(zipName, _T("rb"));
			if (fp == NULL)
			{
				AfxMessageBox(_T("������ ���� ����"));
				return;
			}
			fread(payload + sizeof(int) + namelen, statbuf.st_size, 1, fp);
			fclose(fp);

			pack.n_opcode = opcode;
			pack.n_size = payloadLen;
			pack.cptr_payload = payload;

			CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
			sendToServ(pFrame->hSocket, &pack);

		}
	}
	OnMultiSelect();
}


void CFileView::OnUpdateImageTransport(CCmdUI *pCmdUI)
{
	// TODO: ���⿡ ��� ������Ʈ UI ó���� �ڵ带 �߰��մϴ�.
	if (m_wndFileView.m_vSelectList.size() > 0)
		pCmdUI->Enable(TRUE);
	else
		pCmdUI->Enable(FALSE);
}
