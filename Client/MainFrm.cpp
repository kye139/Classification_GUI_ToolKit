// MainFrm.cpp : CMainFrame Ŭ������ ����
//
#include <vector>
#include <string>
#include "stdafx.h"
#include "ImageTool.h"
#include "MainFrm.h"
#include "ImageToolDoc.h"
#include "PropertyCNNDlg.h"
#include "ServerIPDlg.h"
#include "IdSelectDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif
#pragma warning(disable : 4996)
#pragma once

// CMainFrame

IMPLEMENT_DYNAMIC(CMainFrame, CMDIFrameWndEx)

BEGIN_MESSAGE_MAP(CMainFrame, CMDIFrameWndEx)
	ON_WM_CREATE()
	ON_COMMAND(ID_WINDOW_MANAGER, &CMainFrame::OnWindowManager)
	ON_COMMAND(ID_VIEW_CUSTOMIZE, &CMainFrame::OnViewCustomize)
	ON_REGISTERED_MESSAGE(AFX_WM_CREATETOOLBAR, &CMainFrame::OnToolbarCreateNew)
	ON_WM_SETTINGCHANGE()
	ON_COMMAND(ID_DIR_LOAD, &CMainFrame::OnDirLoad)
	ON_UPDATE_COMMAND_UI(ID_EDIT_CUT, &CMainFrame::OnUpdateEditCut)
	ON_COMMAND(ID_EDIT_CUT, &CMainFrame::OnEditCut)
	ON_COMMAND(ID_EDIT_COPY, &CMainFrame::OnEditCopy)
	ON_UPDATE_COMMAND_UI(ID_EDIT_COPY, &CMainFrame::OnUpdateEditCopy)
	ON_COMMAND(ID_EDIT_PASTE, &CMainFrame::OnEditPaste)
	ON_UPDATE_COMMAND_UI(ID_EDIT_PASTE, &CMainFrame::OnUpdateEditPaste)
	ON_COMMAND(ID_CNN_PROPERTY, &CMainFrame::OnCnnProperty)
	ON_UPDATE_COMMAND_UI(ID_NEW_GROUP, &CMainFrame::OnUpdateNewGroup)
END_MESSAGE_MAP()

static UINT indicators[] =
{
	ID_SEPARATOR,           // ���� �� ǥ�ñ�
	ID_INDICATOR_CAPS,
	ID_INDICATOR_NUM,
	ID_INDICATOR_SCRL,
};

// CMainFrame ����/�Ҹ�

CMainFrame::CMainFrame()
{
	// TODO: ���⿡ ��� �ʱ�ȭ �ڵ带 �߰��մϴ�.
}

CMainFrame::~CMainFrame()
{
}

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CMDIFrameWndEx::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	// CServerIPDlg dlg;
	// if (dlg.DoModal() == IDOK)
	// {
	// 	CIdSelectDlg idDlg;
	// 	hSocket = dlg.hSocket;
	// 	idDlg.idListVec.assign(dlg.dirList.begin(), dlg.dirList.end());
	// 	if (idDlg.DoModal() == IDOK)
	// 	{
	// 		// ���� ���� �α���
	// 		if (idDlg.m_bCreateOrLogin == false)
	// 		{
	// 			packet pack;
	// 			int len = idDlg.m_idStr.GetLength();
	// 
	// 			pack.n_opcode = 11;
	// 			pack.n_size = sizeof(int) + len;
	// 			pack.cptr_payload = (char*)malloc(pack.n_size);
	// 			memcpy(pack.cptr_payload, &len, sizeof(len));
	// 			// memcpy(pack.cptr_payload + sizeof(int), LPSTR(LPCTSTR(idDlg.m_idStr)), len);
	// 			memcpy(pack.cptr_payload + sizeof(int), CT2A(idDlg.m_idStr), len);
	// 
	// 			sendToServ(hSocket, &pack);
	// 		}
	// 		else
	// 		{
	// 			// ���� ����
	// 			packet pack;
	// 			int len = idDlg.m_sCreateId.GetLength();
	// 
	// 			pack.n_opcode = 12;
	// 			pack.n_size = sizeof(int) + len;
	// 			pack.cptr_payload = (char*)malloc(pack.n_size);
	// 			memcpy(pack.cptr_payload, &len, sizeof(len));
	// 			// memcpy(pack.cptr_payload + sizeof(int), LPSTR(LPCTSTR(idDlg.m_sCreateId)), len);
	// 			memcpy(pack.cptr_payload + sizeof(int), CT2A(idDlg.m_sCreateId), len);
	// 
	// 			sendToServ(hSocket, &pack);
	// 		}
	// 	}
	// 	else
	// 		exit(-1);
	// }
	// else
	// 	exit(-1);

	// ����ٰ� dirList ����� free ������� ��!!!!!
		
	BOOL bNameValid;

	if (!m_wndMenuBar.Create(this))
	{
		TRACE0("�޴� ������ ������ ���߽��ϴ�.\n");
		return -1;      // ������ ���߽��ϴ�.
	}

	m_wndMenuBar.SetPaneStyle(m_wndMenuBar.GetPaneStyle() | CBRS_SIZE_DYNAMIC | CBRS_TOOLTIPS | CBRS_FLYBY);

	// �޴� ������ Ȱ��ȭ�ص� ��Ŀ���� �̵����� �ʰ� �մϴ�.
	CMFCPopupMenu::SetForceMenuFocus(FALSE);

	if (!m_wndToolBar.CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP | CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC) ||
		!m_wndToolBar.LoadToolBar(theApp.m_bHiColorIcons ? IDR_MAINFRAME_256 : IDR_MAINFRAME))
	{
		TRACE0("���� ������ ������ ���߽��ϴ�.\n");
		return -1;      // ������ ���߽��ϴ�.
	}

	CString strToolBarName;
	bNameValid = strToolBarName.LoadString(IDS_TOOLBAR_STANDARD);
	ASSERT(bNameValid);
	m_wndToolBar.SetWindowText(strToolBarName);

	CString strCustomize;
	bNameValid = strCustomize.LoadString(IDS_TOOLBAR_CUSTOMIZE);
	ASSERT(bNameValid);
	m_wndToolBar.EnableCustomizeButton(TRUE, ID_VIEW_CUSTOMIZE, strCustomize);

	if (!m_wndStatusBar.Create(this))
	{
		TRACE0("���� ǥ������ ������ ���߽��ϴ�.\n");
		return -1;      // ������ ���߽��ϴ�.
	}
	m_wndStatusBar.SetIndicators(indicators, sizeof(indicators)/sizeof(UINT));

	// TODO: ���� ���� �� �޴� ������ ��ŷ�� �� ���� �Ϸ��� �� �ټ� ���� �����Ͻʽÿ�.
	m_wndMenuBar.EnableDocking(CBRS_ALIGN_ANY);
	m_wndToolBar.EnableDocking(CBRS_ALIGN_ANY);
	EnableDocking(CBRS_ALIGN_ANY);
	DockPane(&m_wndMenuBar);
	DockPane(&m_wndToolBar);


	// Visual Studio 2005 ��Ÿ�� ��ŷ â ������ Ȱ��ȭ�մϴ�.
	CDockingManager::SetDockingMode(DT_SMART);
	// Visual Studio 2005 ��Ÿ�� ��ŷ â �ڵ� ���� ������ Ȱ��ȭ�մϴ�.
	EnableAutoHidePanes(CBRS_ALIGN_ANY);

	// �޴� �׸� �̹����� �ε��մϴ�(ǥ�� ���� ������ ����).
	CMFCToolBar::AddToolBarForImageCollection(IDR_MENU_IMAGES, theApp.m_bHiColorIcons ? IDB_MENU_IMAGES_24 : 0);

	// ��ŷ â�� ����ϴ�.
	if (!CreateDockingWindows())
	{
		TRACE0("��ŷ â�� ������ ���߽��ϴ�.\n");
		return -1;
	}

	m_wndFileView.EnableDocking(CBRS_ALIGN_ANY);
	m_wndClassView.EnableDocking(CBRS_ALIGN_ANY);
	m_wndNetView.EnableDocking(CBRS_ALIGN_ANY);
	m_wndOutputView.EnableDocking(CBRS_ALIGN_ANY);
	DockPane(&m_wndFileView);
	CDockablePane* pTabbedBar = NULL;
	m_wndClassView.AttachToTabWnd(&m_wndFileView, DM_SHOW, TRUE, &pTabbedBar);
	m_wndNetView.AttachToTabWnd(&m_wndFileView, DM_SHOW, TRUE, &pTabbedBar);
	m_wndOutputView.AttachToTabWnd(&m_wndFileView, DM_SHOW, TRUE, &pTabbedBar);
	m_wndOutput.EnableDocking(CBRS_ALIGN_ANY);
	DockPane(&m_wndOutput);
	m_wndProperties.EnableDocking(CBRS_ALIGN_ANY);
	DockPane(&m_wndProperties);


	// ��� ����� �������̽� ��Ҹ� �׸��� �� ����ϴ� ���־� �����ڸ� �����մϴ�.
	CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerWindows));

	// ���� â ���� ��ȭ ���ڸ� Ȱ��ȭ�մϴ�.
	EnableWindowsDialog(ID_WINDOW_MANAGER, ID_WINDOW_MANAGER, TRUE);

	// ���� ���� �� ��ŷ â �޴� �ٲٱ⸦ Ȱ��ȭ�մϴ�.
	EnablePaneMenu(TRUE, ID_VIEW_CUSTOMIZE, strCustomize, ID_VIEW_TOOLBAR);

	// ����(<Alt> Ű�� ���� ä ����) ���� ���� ����� ������ Ȱ��ȭ�մϴ�.
	CMFCToolBar::EnableQuickCustomization();

	return 0;
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CMDIFrameWndEx::PreCreateWindow(cs) )
		return FALSE;
	// TODO: CREATESTRUCT cs�� �����Ͽ� ���⿡��
	//  Window Ŭ���� �Ǵ� ��Ÿ���� �����մϴ�.

	return TRUE;
}

BOOL CMainFrame::CreateDockingWindows()
{
	BOOL bNameValid;

	// Ŭ���� �並 ����ϴ�.
	CString strClassView;
	bNameValid = strClassView.LoadString(IDS_CLASS_VIEW);
	ASSERT(bNameValid);
	if (!m_wndClassView.Create(strClassView, this, CRect(0, 0, 200, 200), TRUE, ID_VIEW_CLASSVIEW, WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | CBRS_LEFT | CBRS_FLOAT_MULTI))
	{
		TRACE0("Ŭ���� �� â�� ������ ���߽��ϴ�.\n");
		return FALSE; // ������ ���߽��ϴ�.
	}

	// ���� �並 ����ϴ�.
	CString strFileView;
	bNameValid = strFileView.LoadString(IDS_FILE_VIEW);
	ASSERT(bNameValid);
	if (!m_wndFileView.Create(strFileView, this, CRect(0, 0, 200, 200), TRUE, ID_VIEW_FILEVIEW, WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | CBRS_LEFT| CBRS_FLOAT_MULTI))
	{
		TRACE0("���� �� â�� ������ ���߽��ϴ�.\n");
		return FALSE; // ������ ���߽��ϴ�.
	}

	// Net �並 ����ϴ�.
	CString strNewView;
	bNameValid = strNewView.LoadString(IDS_NET_VIEW);
	ASSERT(bNameValid);
	if (!m_wndNetView.Create(strNewView, this, CRect(0, 0, 200, 200), TRUE, ID_VIEW_NETVIEW, WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | CBRS_LEFT | CBRS_FLOAT_MULTI))
	{
		TRACE0("Net �� â�� ������ ���߽��ϴ�.\n");
		return FALSE; // ������ ���߽��ϴ�.
	}

	// Output �並 ����ϴ�.
	CString strOutputView;
	bNameValid = strOutputView.LoadString(IDS_OUTPUT_VIEW);
	ASSERT(bNameValid);
	if (!m_wndOutputView.Create(strOutputView, this, CRect(0, 0, 200, 200), TRUE, ID_VIEW_OUTPUTVIEW, WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | CBRS_LEFT | CBRS_FLOAT_MULTI))
	{
		TRACE0("���⹰ �� â�� ������ ���߽��ϴ�.\n");
		return FALSE; // ������ ���߽��ϴ�.
	}

	// ��� â�� ����ϴ�.
	CString strOutputWnd;
	bNameValid = strOutputWnd.LoadString(IDS_OUTPUT_WND);
	ASSERT(bNameValid);
	if (!m_wndOutput.Create(strOutputWnd, this, CRect(0, 0, 100, 100), TRUE, ID_VIEW_OUTPUTWND, WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | CBRS_BOTTOM | CBRS_FLOAT_MULTI))
	{
		TRACE0("��� â�� ������ ���߽��ϴ�.\n");
		return FALSE; // ������ ���߽��ϴ�.
	}

	// �Ӽ� â�� ����ϴ�.
	CString strPropertiesWnd;
	bNameValid = strPropertiesWnd.LoadString(IDS_PROPERTIES_WND);
	ASSERT(bNameValid);
	if (!m_wndProperties.Create(strPropertiesWnd, this, CRect(0, 0, 200, 200), TRUE, ID_VIEW_PROPERTIESWND, WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | CBRS_RIGHT | CBRS_FLOAT_MULTI))
	{
		TRACE0("�Ӽ� â�� ������ ���߽��ϴ�.\n");
		return FALSE; // ������ ���߽��ϴ�.
	}

	SetDockingWindowIcons(theApp.m_bHiColorIcons);
	return TRUE;
}

void CMainFrame::SetDockingWindowIcons(BOOL bHiColorIcons)
{
	HICON hFileViewIcon = (HICON) ::LoadImage(::AfxGetResourceHandle(), MAKEINTRESOURCE(bHiColorIcons ? IDI_FILE_VIEW_HC : IDI_FILE_VIEW), IMAGE_ICON, ::GetSystemMetrics(SM_CXSMICON), ::GetSystemMetrics(SM_CYSMICON), 0);
	m_wndFileView.SetIcon(hFileViewIcon, FALSE);

	HICON hClassViewIcon = (HICON) ::LoadImage(::AfxGetResourceHandle(), MAKEINTRESOURCE(bHiColorIcons ? IDI_CLASS_VIEW_HC : IDI_CLASS_VIEW), IMAGE_ICON, ::GetSystemMetrics(SM_CXSMICON), ::GetSystemMetrics(SM_CYSMICON), 0);
	m_wndClassView.SetIcon(hClassViewIcon, FALSE);

	HICON hNetViewIcon = (HICON) ::LoadImage(::AfxGetResourceHandle(), MAKEINTRESOURCE(bHiColorIcons ? IDI_CLASS_VIEW_HC : IDI_CLASS_VIEW), IMAGE_ICON, ::GetSystemMetrics(SM_CXSMICON), ::GetSystemMetrics(SM_CYSMICON), 0);
	m_wndNetView.SetIcon(hNetViewIcon, FALSE);

	HICON hOutputViewIcon = (HICON) ::LoadImage(::AfxGetResourceHandle(), MAKEINTRESOURCE(bHiColorIcons ? IDI_CLASS_VIEW_HC : IDI_CLASS_VIEW), IMAGE_ICON, ::GetSystemMetrics(SM_CXSMICON), ::GetSystemMetrics(SM_CYSMICON), 0);
	m_wndOutputView.SetIcon(hOutputViewIcon, FALSE);

	HICON hOutputBarIcon = (HICON) ::LoadImage(::AfxGetResourceHandle(), MAKEINTRESOURCE(bHiColorIcons ? IDI_OUTPUT_WND_HC : IDI_OUTPUT_WND), IMAGE_ICON, ::GetSystemMetrics(SM_CXSMICON), ::GetSystemMetrics(SM_CYSMICON), 0);
	m_wndOutput.SetIcon(hOutputBarIcon, FALSE);

	HICON hPropertiesBarIcon = (HICON) ::LoadImage(::AfxGetResourceHandle(), MAKEINTRESOURCE(bHiColorIcons ? IDI_PROPERTIES_WND_HC : IDI_PROPERTIES_WND), IMAGE_ICON, ::GetSystemMetrics(SM_CXSMICON), ::GetSystemMetrics(SM_CYSMICON), 0);
	m_wndProperties.SetIcon(hPropertiesBarIcon, FALSE);

}

// CMainFrame ����

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CMDIFrameWndEx::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CMDIFrameWndEx::Dump(dc);
}
#endif //_DEBUG


// CMainFrame �޽��� ó����

void CMainFrame::OnWindowManager()
{
	ShowWindowsDialog();
}

void CMainFrame::OnViewCustomize()
{
	CMFCToolBarsCustomizeDialog* pDlgCust = new CMFCToolBarsCustomizeDialog(this, TRUE /* �޴��� �˻��մϴ�. */);
	pDlgCust->Create();
}

LRESULT CMainFrame::OnToolbarCreateNew(WPARAM wp,LPARAM lp)
{
	LRESULT lres = CMDIFrameWndEx::OnToolbarCreateNew(wp,lp);
	if (lres == 0)
	{
		return 0;
	}

	CMFCToolBar* pUserToolbar = (CMFCToolBar*)lres;
	ASSERT_VALID(pUserToolbar);

	BOOL bNameValid;
	CString strCustomize;
	bNameValid = strCustomize.LoadString(IDS_TOOLBAR_CUSTOMIZE);
	ASSERT(bNameValid);

	pUserToolbar->EnableCustomizeButton(TRUE, ID_VIEW_CUSTOMIZE, strCustomize);
	return lres;
}


void CMainFrame::OnSettingChange(UINT uFlags, LPCTSTR lpszSection)
{
	CMDIFrameWndEx::OnSettingChange(uFlags, lpszSection);
	m_wndOutput.UpdateFonts();
}

void onOpenDirImage(CString pathName, WIN32_FIND_DATA fd, HANDLE hFind)
{
	// ���丮�� �̾Ƴ���
	CString str((LPCTSTR)pathName);
	CString dirName, nextName;
	int curPos = 0;

	nextName = str.Tokenize(_T("\\"), curPos);

	while (nextName != "")
	{
		dirName = nextName;
		nextName = str.Tokenize(_T("\\"), curPos);
	};

	// ���� �� �׷� ����
	CMainFrame* pFrame = (CMainFrame *)AfxGetMainWnd();
	CViewTree* fileView = &(pFrame->m_wndFileView.m_wndFileView);
	HTREEITEM hRoot = fileView->GetFirstVisibleItem();
	HTREEITEM newItemGroup = fileView->InsertItem(dirName, 0, 0, hRoot);
	int * uniqueGroupId = (int*)malloc(sizeof(int));
	*uniqueGroupId = theApp._id++;
	fileView->SetItemData(newItemGroup, (DWORD_PTR)uniqueGroupId);

	do {
		// read all (real) files in current folder
		// , delete '!' read other 2 default folder . and ..
		if (!(fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)) {
			IppImage* openImage = new IppImage();
			CString fileName(fd.cFileName);

			if (!openImage->Load(pathName + "\\" + fileName))
			{
				delete openImage;
				continue;
			}

			// �̹��� ���� ����ü ����
			ImageInform* newImgInfo = new ImageInform;
			newImgInfo->uniqueId = theApp._id++;
			wcscpy_s(newImgInfo->name, (LPCTSTR)fileName);
			newImgInfo->image = openImage;

			// TreeView Ŭ���� ����.
			HTREEITEM newImgItem = fileView->InsertItem(fileName, 2, 2, newItemGroup);
			fileView->SetItemData(newImgItem, (DWORD_PTR)newImgInfo);
		}
	} while (::FindNextFile(hFind, &fd));

	fileView->Expand(newItemGroup, TVE_EXPAND);
}


void CMainFrame::getFilesInFolder(CString folder)
{
	wchar_t search_path[200];
	wsprintf(search_path, _T("%s/*.*"), (LPCTSTR)folder);
	WIN32_FIND_DATA fd;
	HANDLE hFind = ::FindFirstFile(search_path, &fd);
	if (hFind != INVALID_HANDLE_VALUE) {
		onOpenDirImage(folder, fd, hFind);
		::FindClose(hFind);
	}
}

void CMainFrame::OnDirLoad()
{
	// TODO: ���⿡ ��� ó���� �ڵ带 �߰��մϴ�.
	BROWSEINFO BrInfo;
	TCHAR szBuffer[512];                                      // ������� ���� 
	std::vector<std::string> fileList;

	::ZeroMemory(&BrInfo, sizeof(BROWSEINFO));
	::ZeroMemory(szBuffer, 512);

	BrInfo.hwndOwner = GetSafeHwnd();
	BrInfo.lpszTitle = _T("������ ����� ������ �����ϼ���");
	BrInfo.ulFlags = BIF_NEWDIALOGSTYLE | BIF_EDITBOX | BIF_RETURNONLYFSDIRS;
	LPITEMIDLIST pItemIdList = ::SHBrowseForFolder(&BrInfo);
	::SHGetPathFromIDList(pItemIdList, szBuffer);               // ���ϰ�� �о����
	if (*szBuffer == NULL)
		return;

	getFilesInFolder(CString(szBuffer));
}

void CMainFrame::OnUpdateEditCut(CCmdUI *pCmdUI)
{
	// TODO: ���⿡ ��� ������Ʈ UI ó���� �ڵ带 �߰��մϴ�.
	HTREEITEM selItem = m_wndFileView.m_wndFileView.GetSelectedItem();
	if (!m_wndFileView.m_wndFileView.GetItemData(selItem))
		pCmdUI->Enable(FALSE);
	else
		pCmdUI->Enable(TRUE);
}


void CMainFrame::OnEditCut()
{
	// TODO: ���⿡ ��� ó���� �ڵ带 �߰��մϴ�.
	m_wndFileView.OnEditCut();
}


void CMainFrame::OnEditCopy()
{
	// TODO: ���⿡ ��� ó���� �ڵ带 �߰��մϴ�.
	m_wndFileView.OnEditCopy();
}


void CMainFrame::OnUpdateEditCopy(CCmdUI *pCmdUI)
{
	// TODO: ���⿡ ��� ������Ʈ UI ó���� �ڵ带 �߰��մϴ�.
	// TODO: ���⿡ ��� ������Ʈ UI ó���� �ڵ带 �߰��մϴ�.
	HTREEITEM selItem = m_wndFileView.m_wndFileView.GetSelectedItem();
	if (!m_wndFileView.m_wndFileView.GetItemData(selItem))
		pCmdUI->Enable(FALSE);
	else
		pCmdUI->Enable(TRUE);
}


void CMainFrame::OnEditPaste()
{
	// TODO: ���⿡ ��� ó���� �ڵ带 �߰��մϴ�.
	m_wndFileView.OnEditPaste();
}


void CMainFrame::OnUpdateEditPaste(CCmdUI *pCmdUI)
{
	// TODO: ���⿡ ��� ������Ʈ UI ó���� �ڵ带 �߰��մϴ�.
	// ī�ǵ� �׸��� �ִ� ���
	if (m_wndFileView.cpBoard.image || m_wndFileView.tempCut)
	{
		// ��Ʈ �׷��� ������ ���
		if (m_wndFileView.m_wndFileView.GetFirstVisibleItem() == m_wndFileView.m_wndFileView.GetSelectedItem())
			pCmdUI->Enable(FALSE);
		pCmdUI->Enable(TRUE);
	}
	else
		// ī�ǵ� �׸��� ���� ���
		pCmdUI->Enable(FALSE);
}


void CMainFrame::OnCnnProperty()
{
	// TODO: ���⿡ ��� ó���� �ڵ带 �߰��մϴ�.
	CPropertyCNNDlg dlg;
	if (dlg.DoModal() == IDOK)
	{

	}
	// ���� ���� ���� ��� �Ҹ�
	if (dlg.m_VButtons.size() > 0)
	{
		std::vector<CButton *>::iterator iter;
		for (iter = dlg.m_VButtons.begin(); iter != dlg.m_VButtons.end(); ++iter)
			delete *iter;
	}
		
	if (dlg.m_VProperty.size() > 0)
	{
		std::vector<HiddenLayerProperty *>::iterator iterProp;
		for (iterProp = dlg.m_VProperty.begin(); iterProp != dlg.m_VProperty.end(); ++iterProp)
			delete *iterProp;
	}
}


void CMainFrame::OnUpdateNewGroup(CCmdUI *pCmdUI)
{
	// TODO: ���⿡ ��� ������Ʈ UI ó���� �ڵ带 �߰��մϴ�.

}
