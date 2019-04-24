// MainFrm.cpp : CMainFrame 클래스의 구현
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
	ID_SEPARATOR,           // 상태 줄 표시기
	ID_INDICATOR_CAPS,
	ID_INDICATOR_NUM,
	ID_INDICATOR_SCRL,
};

// CMainFrame 생성/소멸

CMainFrame::CMainFrame()
{
	// TODO: 여기에 멤버 초기화 코드를 추가합니다.
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
	// 		// 기존 계정 로그인
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
	// 			// 계정 생성
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

	// 여기다가 dirList 내용들 free 시켜줘야 함!!!!!
		
	BOOL bNameValid;

	if (!m_wndMenuBar.Create(this))
	{
		TRACE0("메뉴 모음을 만들지 못했습니다.\n");
		return -1;      // 만들지 못했습니다.
	}

	m_wndMenuBar.SetPaneStyle(m_wndMenuBar.GetPaneStyle() | CBRS_SIZE_DYNAMIC | CBRS_TOOLTIPS | CBRS_FLYBY);

	// 메뉴 모음을 활성화해도 포커스가 이동하지 않게 합니다.
	CMFCPopupMenu::SetForceMenuFocus(FALSE);

	if (!m_wndToolBar.CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP | CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC) ||
		!m_wndToolBar.LoadToolBar(theApp.m_bHiColorIcons ? IDR_MAINFRAME_256 : IDR_MAINFRAME))
	{
		TRACE0("도구 모음을 만들지 못했습니다.\n");
		return -1;      // 만들지 못했습니다.
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
		TRACE0("상태 표시줄을 만들지 못했습니다.\n");
		return -1;      // 만들지 못했습니다.
	}
	m_wndStatusBar.SetIndicators(indicators, sizeof(indicators)/sizeof(UINT));

	// TODO: 도구 모음 및 메뉴 모음을 도킹할 수 없게 하려면 이 다섯 줄을 삭제하십시오.
	m_wndMenuBar.EnableDocking(CBRS_ALIGN_ANY);
	m_wndToolBar.EnableDocking(CBRS_ALIGN_ANY);
	EnableDocking(CBRS_ALIGN_ANY);
	DockPane(&m_wndMenuBar);
	DockPane(&m_wndToolBar);


	// Visual Studio 2005 스타일 도킹 창 동작을 활성화합니다.
	CDockingManager::SetDockingMode(DT_SMART);
	// Visual Studio 2005 스타일 도킹 창 자동 숨김 동작을 활성화합니다.
	EnableAutoHidePanes(CBRS_ALIGN_ANY);

	// 메뉴 항목 이미지를 로드합니다(표준 도구 모음에 없음).
	CMFCToolBar::AddToolBarForImageCollection(IDR_MENU_IMAGES, theApp.m_bHiColorIcons ? IDB_MENU_IMAGES_24 : 0);

	// 도킹 창을 만듭니다.
	if (!CreateDockingWindows())
	{
		TRACE0("도킹 창을 만들지 못했습니다.\n");
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


	// 모든 사용자 인터페이스 요소를 그리는 데 사용하는 비주얼 관리자를 설정합니다.
	CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerWindows));

	// 향상된 창 관리 대화 상자를 활성화합니다.
	EnableWindowsDialog(ID_WINDOW_MANAGER, ID_WINDOW_MANAGER, TRUE);

	// 도구 모음 및 도킹 창 메뉴 바꾸기를 활성화합니다.
	EnablePaneMenu(TRUE, ID_VIEW_CUSTOMIZE, strCustomize, ID_VIEW_TOOLBAR);

	// 빠른(<Alt> 키를 누른 채 끌기) 도구 모음 사용자 지정을 활성화합니다.
	CMFCToolBar::EnableQuickCustomization();

	return 0;
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CMDIFrameWndEx::PreCreateWindow(cs) )
		return FALSE;
	// TODO: CREATESTRUCT cs를 수정하여 여기에서
	//  Window 클래스 또는 스타일을 수정합니다.

	return TRUE;
}

BOOL CMainFrame::CreateDockingWindows()
{
	BOOL bNameValid;

	// 클래스 뷰를 만듭니다.
	CString strClassView;
	bNameValid = strClassView.LoadString(IDS_CLASS_VIEW);
	ASSERT(bNameValid);
	if (!m_wndClassView.Create(strClassView, this, CRect(0, 0, 200, 200), TRUE, ID_VIEW_CLASSVIEW, WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | CBRS_LEFT | CBRS_FLOAT_MULTI))
	{
		TRACE0("클래스 뷰 창을 만들지 못했습니다.\n");
		return FALSE; // 만들지 못했습니다.
	}

	// 파일 뷰를 만듭니다.
	CString strFileView;
	bNameValid = strFileView.LoadString(IDS_FILE_VIEW);
	ASSERT(bNameValid);
	if (!m_wndFileView.Create(strFileView, this, CRect(0, 0, 200, 200), TRUE, ID_VIEW_FILEVIEW, WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | CBRS_LEFT| CBRS_FLOAT_MULTI))
	{
		TRACE0("파일 뷰 창을 만들지 못했습니다.\n");
		return FALSE; // 만들지 못했습니다.
	}

	// Net 뷰를 만듭니다.
	CString strNewView;
	bNameValid = strNewView.LoadString(IDS_NET_VIEW);
	ASSERT(bNameValid);
	if (!m_wndNetView.Create(strNewView, this, CRect(0, 0, 200, 200), TRUE, ID_VIEW_NETVIEW, WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | CBRS_LEFT | CBRS_FLOAT_MULTI))
	{
		TRACE0("Net 뷰 창을 만들지 못했습니다.\n");
		return FALSE; // 만들지 못했습니다.
	}

	// Output 뷰를 만듭니다.
	CString strOutputView;
	bNameValid = strOutputView.LoadString(IDS_OUTPUT_VIEW);
	ASSERT(bNameValid);
	if (!m_wndOutputView.Create(strOutputView, this, CRect(0, 0, 200, 200), TRUE, ID_VIEW_OUTPUTVIEW, WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | CBRS_LEFT | CBRS_FLOAT_MULTI))
	{
		TRACE0("산출물 뷰 창을 만들지 못했습니다.\n");
		return FALSE; // 만들지 못했습니다.
	}

	// 출력 창을 만듭니다.
	CString strOutputWnd;
	bNameValid = strOutputWnd.LoadString(IDS_OUTPUT_WND);
	ASSERT(bNameValid);
	if (!m_wndOutput.Create(strOutputWnd, this, CRect(0, 0, 100, 100), TRUE, ID_VIEW_OUTPUTWND, WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | CBRS_BOTTOM | CBRS_FLOAT_MULTI))
	{
		TRACE0("출력 창을 만들지 못했습니다.\n");
		return FALSE; // 만들지 못했습니다.
	}

	// 속성 창을 만듭니다.
	CString strPropertiesWnd;
	bNameValid = strPropertiesWnd.LoadString(IDS_PROPERTIES_WND);
	ASSERT(bNameValid);
	if (!m_wndProperties.Create(strPropertiesWnd, this, CRect(0, 0, 200, 200), TRUE, ID_VIEW_PROPERTIESWND, WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | CBRS_RIGHT | CBRS_FLOAT_MULTI))
	{
		TRACE0("속성 창을 만들지 못했습니다.\n");
		return FALSE; // 만들지 못했습니다.
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

// CMainFrame 진단

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


// CMainFrame 메시지 처리기

void CMainFrame::OnWindowManager()
{
	ShowWindowsDialog();
}

void CMainFrame::OnViewCustomize()
{
	CMFCToolBarsCustomizeDialog* pDlgCust = new CMFCToolBarsCustomizeDialog(this, TRUE /* 메뉴를 검색합니다. */);
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
	// 디렉토리명 뽑아내기
	CString str((LPCTSTR)pathName);
	CString dirName, nextName;
	int curPos = 0;

	nextName = str.Tokenize(_T("\\"), curPos);

	while (nextName != "")
	{
		dirName = nextName;
		nextName = str.Tokenize(_T("\\"), curPos);
	};

	// 파일 뷰 그룹 생성
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

			// 이미지 정보 구조체 저장
			ImageInform* newImgInfo = new ImageInform;
			newImgInfo->uniqueId = theApp._id++;
			wcscpy_s(newImgInfo->name, (LPCTSTR)fileName);
			newImgInfo->image = openImage;

			// TreeView 클래스 지정.
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
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	BROWSEINFO BrInfo;
	TCHAR szBuffer[512];                                      // 경로저장 버퍼 
	std::vector<std::string> fileList;

	::ZeroMemory(&BrInfo, sizeof(BROWSEINFO));
	::ZeroMemory(szBuffer, 512);

	BrInfo.hwndOwner = GetSafeHwnd();
	BrInfo.lpszTitle = _T("파일이 저장될 폴더를 선택하세요");
	BrInfo.ulFlags = BIF_NEWDIALOGSTYLE | BIF_EDITBOX | BIF_RETURNONLYFSDIRS;
	LPITEMIDLIST pItemIdList = ::SHBrowseForFolder(&BrInfo);
	::SHGetPathFromIDList(pItemIdList, szBuffer);               // 파일경로 읽어오기
	if (*szBuffer == NULL)
		return;

	getFilesInFolder(CString(szBuffer));
}

void CMainFrame::OnUpdateEditCut(CCmdUI *pCmdUI)
{
	// TODO: 여기에 명령 업데이트 UI 처리기 코드를 추가합니다.
	HTREEITEM selItem = m_wndFileView.m_wndFileView.GetSelectedItem();
	if (!m_wndFileView.m_wndFileView.GetItemData(selItem))
		pCmdUI->Enable(FALSE);
	else
		pCmdUI->Enable(TRUE);
}


void CMainFrame::OnEditCut()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	m_wndFileView.OnEditCut();
}


void CMainFrame::OnEditCopy()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	m_wndFileView.OnEditCopy();
}


void CMainFrame::OnUpdateEditCopy(CCmdUI *pCmdUI)
{
	// TODO: 여기에 명령 업데이트 UI 처리기 코드를 추가합니다.
	// TODO: 여기에 명령 업데이트 UI 처리기 코드를 추가합니다.
	HTREEITEM selItem = m_wndFileView.m_wndFileView.GetSelectedItem();
	if (!m_wndFileView.m_wndFileView.GetItemData(selItem))
		pCmdUI->Enable(FALSE);
	else
		pCmdUI->Enable(TRUE);
}


void CMainFrame::OnEditPaste()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	m_wndFileView.OnEditPaste();
}


void CMainFrame::OnUpdateEditPaste(CCmdUI *pCmdUI)
{
	// TODO: 여기에 명령 업데이트 UI 처리기 코드를 추가합니다.
	// 카피된 항목이 있는 경우
	if (m_wndFileView.cpBoard.image || m_wndFileView.tempCut)
	{
		// 루트 그룹을 선택한 경우
		if (m_wndFileView.m_wndFileView.GetFirstVisibleItem() == m_wndFileView.m_wndFileView.GetSelectedItem())
			pCmdUI->Enable(FALSE);
		pCmdUI->Enable(TRUE);
	}
	else
		// 카피된 항목이 없는 경우
		pCmdUI->Enable(FALSE);
}


void CMainFrame::OnCnnProperty()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	CPropertyCNNDlg dlg;
	if (dlg.DoModal() == IDOK)
	{

	}
	// 벡터 안의 내용 모두 소멸
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
	// TODO: 여기에 명령 업데이트 UI 처리기 코드를 추가합니다.

}
