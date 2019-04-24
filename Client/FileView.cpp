
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
// CWorkspaceBar 메시지 처리기

int CFileView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDockablePane::OnCreate(lpCreateStruct) == -1)
		return -1;

	CRect rectDummy;
	rectDummy.SetRectEmpty();

	// 뷰를 만듭니다.
	const DWORD dwViewStyle = WS_CHILD | WS_VISIBLE | TVS_HASLINES | TVS_LINESATROOT | TVS_HASBUTTONS | TVS_EDITLABELS | TVS_FULLROWSELECT;

	if (!m_wndFileView.Create(dwViewStyle, rectDummy, this, 4))
	{
		TRACE0("파일 뷰를 만들지 못했습니다.\n");
		return -1;      // 만들지 못했습니다.
	}

	// 뷰 이미지를 로드합니다.
	m_FileViewImages.Create(IDB_FILE_VIEW, 16, 0, RGB(255, 0, 255));
	m_wndFileView.SetImageList(&m_FileViewImages, TVSIL_NORMAL);

	m_wndToolBar.Create(this, AFX_DEFAULT_TOOLBAR_STYLE, IDR_EXPLORER);
	m_wndToolBar.LoadToolBar(IDR_EXPLORER, 0, 0, TRUE /* 잠금 */);

	OnChangeVisualStyle();

	m_wndToolBar.SetPaneStyle(m_wndToolBar.GetPaneStyle() | CBRS_TOOLTIPS | CBRS_FLYBY);

	m_wndToolBar.SetPaneStyle(m_wndToolBar.GetPaneStyle() & ~(CBRS_GRIPPER | CBRS_SIZE_DYNAMIC | CBRS_BORDER_TOP | CBRS_BORDER_BOTTOM | CBRS_BORDER_LEFT | CBRS_BORDER_RIGHT));

	m_wndToolBar.SetOwner(this);

	// 모든 명령은 부모 프레임이 아닌 이 컨트롤을 통해 라우팅됩니다.
	m_wndToolBar.SetRouteCommandsViaFrame(FALSE);

	// 정적 트리 뷰 데이터를 더미 코드로 채웁니다.
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
	HTREEITEM hRoot = m_wndFileView.InsertItem(_T("이미지 파일"), 0, 0);
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
		// 클릭한 항목을 선택합니다.
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
	AfxMessageBox(_T("속성...."));

}

void CFileView::OnFileOpen()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
}

void CFileView::OnFileOpenWith()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
}

void CFileView::OnDummyCompile()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
}

void CFileView::OnEditCut()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	// 선택 항목 추출
	HTREEITEM selItem = m_wndFileView.GetSelectedItem();

	// 잘라내기의 경우 선택항목의 주소만 복사.
	tempCut = selItem;

	// 대상 흐리게 만드릭
	//m_wndFileView.SetTextColor(RGB(238, 238, 238));

	AfxMessageBox(_T("잘라내기 완료"));
}

void CFileView::OnEditCopy()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	// 선택 항목 추출
	HTREEITEM selItem = m_wndFileView.GetSelectedItem();
	ImageInform* selItemInfo = (ImageInform *)m_wndFileView.GetItemData(selItem);

	// 복사의 경우 uniqueId 복사할 필요가 없음.
	cpBoard.uniqueId = -1;

	// 나머지 이미지 정보들을 클립보드로 복사.
	wcscpy_s(cpBoard.name, selItemInfo->name);
	if (tempCut)
		tempCut = NULL;
	if (cpBoard.image)
		cpBoard.image = NULL;
	cpBoard.image = selItemInfo->image;
	AfxMessageBox(_T("복사 완료"));
}

void CFileView::OnEditClear()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	// 선택 항목 추출
	HTREEITEM selItem = m_wndFileView.GetSelectedItem();
	ImageInform* selItemInfo = (ImageInform *)m_wndFileView.GetItemData(selItem);

	// 선택항목의 부모
	HTREEITEM parentItem = m_wndFileView.GetParentItem(selItem);

	// 항목이 파일인 경우
	if (selItemInfo)
	{
		wchar_t id[10] = { 0, };
		wsprintf(id, _T("%d"), selItemInfo->uniqueId);
		HWND mdi_parent = ::FindWindowEx(AfxGetMainWnd()->m_hWnd, NULL, _T("MDIClient"), NULL);
		HWND mdi_child = ::FindWindowEx(mdi_parent, NULL, id, NULL);
		// 윈도우가 열려있는 경우
		if (mdi_child != NULL)
			::DestroyWindow(mdi_child);

		// 선택항목의 IppImage 소멸
		if (selItemInfo->image)
			delete selItemInfo->image;

		// 선택 항목 ImageInform 객체 소멸
		delete selItemInfo;

		// 파일 뷰에서 항목 삭제
		m_wndFileView.DeleteItem(selItem);
	}
	else
	// 항목이 그룹인 경우
	{
		// 그룹 안에 파일이 존재하는 경우
		if (m_wndFileView.ItemHasChildren(selItem))
		{
			AfxMessageBox(_T("그룹 안에 파일이 존재하는 경우 그룹 삭제를 할 수 없습니다."));
			return;
		}
		// 그룹 안에 파일이 없는 경우
		else
		{
			m_wndFileView.DeleteItem(selItem);
		}
	}

	// 새로고침
	m_wndFileView.Expand(parentItem, TVE_EXPAND);
}

void CFileView::OnPaint()
{
	CPaintDC dc(this); // 그리기를 위한 디바이스 컨텍스트입니다.

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
	m_wndToolBar.LoadBitmap(theApp.m_bHiColorIcons ? IDB_EXPLORER_24 : IDR_EXPLORER, 0, 0, TRUE /* 잠금 */);

	m_FileViewImages.DeleteImageList();

	UINT uiBmpId = theApp.m_bHiColorIcons ? IDB_FILE_VIEW_24 : IDB_FILE_VIEW;

	CBitmap bmp;
	if (!bmp.LoadBitmap(uiBmpId))
	{
		TRACE(_T("비트맵을 로드할 수 없습니다. %x\n"), uiBmpId);
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
	// TODO: 여기에 명령 업데이트 UI 처리기 코드를 추가합니다.
	CViewTree* fileView = &m_wndFileView;
	HTREEITEM selItem = fileView->GetSelectedItem();
	if (!fileView->GetItemData(selItem))
		pCmdUI->Enable(FALSE);
	else
		pCmdUI->Enable(TRUE);
}

BOOL CFileView::CheckName(HTREEITEM targetItem, TCHAR* name)
{
	// 이름 중복 여부 검사
	// targetItem은 무조건 이미지 파일이어야 함.
	ImageInform* targetItemInfo = (ImageInform *)m_wndFileView.GetItemData(targetItem);

	// targetitem이 디렉터리인 경우
	if (!targetItemInfo)
		return false;

	// target 대상으로 이름 중복 여부 검사
	if (!_tcscmp(targetItemInfo->name, name))
	{
		MessageBox(_T("동일한 이름의 파일이 존재합니다."));
		return false;
	}
	
	HTREEITEM pointItem = targetItem;
	ImageInform* imgInfo;

	while (pointItem = m_wndFileView.GetNextSiblingItem(pointItem))
	{
		imgInfo = (ImageInform *)m_wndFileView.GetItemData(pointItem);

		// 포인팅 항목이 그룹인 경우
		if (!imgInfo)
			continue;
		// 이름 같은 파일이 존재하는 경우
		if (!_tcscmp(imgInfo->name, name))
		{
			MessageBox(_T("동일한 이름의 파일이 존재합니다."));
			return false;
		}
	}
	pointItem = targetItem;
	while (pointItem = m_wndFileView.GetPrevSiblingItem(pointItem))
	{
		imgInfo = (ImageInform *)m_wndFileView.GetItemData(pointItem);

		// 포인팅 항목이 그룹인 경우
		if (!imgInfo)
			continue;
		// 이름 같은 파일이 존재하는 경우
		if (!_tcscmp(imgInfo->name, name))
		{
			MessageBox(_T("동일한 이름의 파일이 존재합니다."));
			return false;
		}
	}

	return true;
}

void CFileView::OnEditPaste()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	HTREEITEM selItem = m_wndFileView.GetSelectedItem();
	ImageInform* selItemInfo = (ImageInform *)m_wndFileView.GetItemData(selItem);

	// 그룹을 대상으로한 이벤트
	if (!selItemInfo)
	{
		// 잘라내기인 경우
		if (tempCut)
		{
			// 잘라내기한 항목 데이터 추출
			ImageInform * cutInfo = (ImageInform *)m_wndFileView.GetItemData(tempCut);

			// 붙여넣기 타겟 이름 중복 여부 검사
			if (m_wndFileView.ItemHasChildren(selItem) && !CheckName(m_wndFileView.GetChildItem(selItem), cutInfo->name))
				return;

			// Dest 항목 추가
			// HTREEITEM pasteItem = m_wndFileView.InsertItem(cutInfo->name, 2, 2, m_wndFileView.GetParentItem(selItem));
			HTREEITEM pasteItem = m_wndFileView.InsertItem(cutInfo->name, 2, 2, selItem);
			m_wndFileView.SetItemData(pasteItem, (DWORD_PTR)cutInfo);

			// src 항목 삭제
			m_wndFileView.DeleteItem(tempCut);
			tempCut = NULL;
		}
		else
		// 복사인 경우
		{
			// 붙여넣기 타겟 이름 중복 여부 검사
			if (m_wndFileView.ItemHasChildren(selItem) && !CheckName(m_wndFileView.GetChildItem(selItem), cpBoard.name))
				return;

			// 복사하는 이미지 정보 복사
			ImageInform* pasteImageInfo = new ImageInform;
			wcscat_s(cpBoard.name, _T(".cpy"));
			wcscpy_s(pasteImageInfo->name, cpBoard.name);
			pasteImageInfo->uniqueId = theApp._id++;
			pasteImageInfo->image = new IppImage(cpBoard.image);

			// 파일뷰에 항목 추가
			HTREEITEM pasteItem = m_wndFileView.InsertItem(CString(pasteImageInfo->name), 2, 2, selItem);
			m_wndFileView.SetItemData(pasteItem, (DWORD_PTR)pasteImageInfo);

			// 클립보드 이미지 정보 초기화
			cpBoard.uniqueId = -1;
			cpBoard.image = NULL;
		}

		// 파일 뷰 새로고침
		m_wndFileView.Expand(selItem, TVE_EXPAND);
	}
	else
	// 파일을 대상으로 한 이벤트
	{
		// 잘라내기인 경우
		if (tempCut)
		{
			// 잘라내기한 항목 데이터 추출
			ImageInform * cutInfo = (ImageInform *)m_wndFileView.GetItemData(tempCut);

			// 붙여넣기 타겟 이름 중복 여부 검사
			if (CheckName(selItem, cutInfo->name))
			{
				// Dest 항목 추가
				HTREEITEM pasteItem = m_wndFileView.InsertItem(cutInfo->name, 2, 2, m_wndFileView.GetParentItem(selItem));
				m_wndFileView.SetItemData(pasteItem, (DWORD_PTR)cutInfo);
			}

			// src 항목 삭제
			m_wndFileView.DeleteItem(tempCut);
			tempCut = NULL;
		}
		else
		// 복사인 경우
		{
			// 붙여넣기 타겟 이름 중복 여부 검사
			if (CheckName(selItem, cpBoard.name))
			{
				// 복사하는 이미지 정보 복사
				ImageInform* pasteImageInfo = new ImageInform;
				wcscpy_s(pasteImageInfo->name, cpBoard.name);
				pasteImageInfo->uniqueId = theApp._id++;
				pasteImageInfo->image = new IppImage(cpBoard.image);

				// 파일뷰에 항목 추가
				HTREEITEM pasteItem = m_wndFileView.InsertItem(CString(pasteImageInfo->name) + CString(_T(".cpy")), 2, 2, m_wndFileView.GetParentItem(selItem));
				m_wndFileView.SetItemData(pasteItem, (DWORD_PTR)pasteImageInfo);
			}

			// 클립보드 이미지 정보 초기화
			cpBoard.uniqueId = -1;
			cpBoard.image = NULL;
		}

		// 파일 뷰 새로고침
		m_wndFileView.Expand(m_wndFileView.GetParentItem(selItem), TVE_EXPAND);
	}
}


void CFileView::OnUpdateEditPaste(CCmdUI *pCmdUI)
{
	// TODO: 여기에 명령 업데이트 UI 처리기 코드를 추가합니다.
	// 카피된 항목이 있는 경우
	if (cpBoard.image || tempCut)
	{
		// 루트 그룹을 선택한 경우
		if (m_wndFileView.GetFirstVisibleItem() == m_wndFileView.GetSelectedItem())
			pCmdUI->Enable(FALSE);
		pCmdUI->Enable(TRUE);
	}
	else
		// 카피된 항목이 없는 경우
		pCmdUI->Enable(FALSE);

}


void CFileView::OnUpdateEditCut(CCmdUI *pCmdUI)
{
	// TODO: 여기에 명령 업데이트 UI 처리기 코드를 추가합니다.
	HTREEITEM selItem = m_wndFileView.GetSelectedItem();
	if (!m_wndFileView.GetItemData(selItem))
		pCmdUI->Enable(FALSE);
	else
		pCmdUI->Enable(TRUE);
}


void CFileView::OnUpdateEditClear(CCmdUI *pCmdUI)
{
	// TODO: 여기에 명령 업데이트 UI 처리기 코드를 추가합니다.
	HTREEITEM root = m_wndFileView.GetRootItem();
	HTREEITEM sel = m_wndFileView.GetSelectedItem();
	// 루트 그룹을 선택한 경우 비활성.
	if (root == sel || m_wndFileView.GetChildItem(root) == sel)
		pCmdUI->Enable(FALSE);
	else
		pCmdUI->Enable(TRUE);
}


void CFileView::OnNewGroup()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	CNewGroupDlg dlg;
	if (dlg.DoModal() == IDOK)
	{
		// HTREEITEM selItem = m_wndFileView.GetSelectedItem();
		// ImageInform* selImageInfo = (ImageInform *)m_wndFileView.GetItemData(selItem);
		// // 선택된 항목이 그룹명인경우
		// if (!selImageInfo)
		// 	m_wndFileView.InsertItem(dlg.m_nGroupName, 0, 0, selItem, TVI_FIRST);
		// // 선택된 항목이 이미지인경우
		// else
		// 	m_wndFileView.InsertItem(dlg.m_nGroupName, 0, 0, m_wndFileView.GetParentItem(selItem), TVI_FIRST);
		// 모든 그룹 명은 child에 삽입
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
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	// DWORD nowStyle = m_wndFileView.GetExtendedStyle();

	// 체크박스가 활성화되어 있는 경우
	if (ckbox)
	{
		//m_wndFileView.SetExtendedStyle(nowStyle, nowStyle &= ~TVS_CHECKBOXES);
		hideCheckBox(m_wndFileView.GetRootItem());
		m_wndFileView.ModifyStyle(TVS_CHECKBOXES, 0);
		m_wndFileView.m_vSelectList.clear();
		ckbox = false;
	}
	// 체크박스가 비활성화 되어 있는 경우
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
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	CString savePath;

	GetDirPathDialog(&savePath, GetSafeHwnd(), _T("파일 경로 설정"));
	if (savePath.IsEmpty())
		return;

	// 체크된 항목들 압축 및 전송
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
		CString dirName = m_wndFileView.GetItemText(childIndex); // 저장할 디렉토리 이름
		CString fullName = savePath + CString(_T("\\")) + dirName; // 디렉터리 절대 경로

		if (GetFileAttributes((LPCTSTR)fullName) == 0xFFFFFFFF)
			CreateDirectory((LPCTSTR)fullName, NULL);

		CString zipName = fullName + CString(_T(".zip"));

		// 같은 이름의 압축파일이 존재하는 경우 삭제 후 다시 압축파일 생성 저장
		if (!GetFileAttributes((LPCTSTR)zipName) == 0xFFFFFFFF)
			DeleteFile((LPCTSTR)zipName);

		HZIP hz = CreateZip(zipName, 0); // 압축 파일 생성

		HTREEITEM imageIndex = m_wndFileView.GetChildItem(childIndex);
		ImageInform * inform = (ImageInform *)m_wndFileView.GetItemData(imageIndex);

		CString imageName = m_wndFileView.GetItemText(imageIndex); // 이미지 파일 이름

		// 이미지 파일 확장자 검색
		CString ext = imageName.Right(4);
		if (ext.Compare(_T(".jpg")) && ext.Compare(_T(".bmp")) && ext.Compare(_T(".png")))
			imageName += CString(_T(".bmp"));

		CString resultPath = fullName + CString(_T("\\")) + imageName; // 이미지 파일 절대 경로
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

		// 여기부터 압축 파일 전송
		packet pack;

		int opcode = 2;
		int namelen = dirName.GetLength(); // 압축파일 이름의 길이
		char* name = CT2A(dirName); // 압축파일 이름
		struct _stat statbuf;
		int statResult = _stat(CT2A(zipName), &statbuf); // statbuf.st_size => 해당 파일의 크기
		if (statResult != 0)
		{
			AfxMessageBox(_T("압축파일 탐색 실패"));
			return;
		}
		else
		{
			int payloadLen = sizeof(int) + namelen + statbuf.st_size; // 전체 페이로드의 길이
			char* payload = (char*)malloc(payloadLen); // 페이로드 공간 생성

			memcpy(payload, &namelen, sizeof(int));
			memcpy(payload + sizeof(int), name, namelen);

			FILE* fp = _wfopen(zipName, _T("rb"));
			if (fp == NULL)
			{
				AfxMessageBox(_T("데이터 전송 실패"));
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
	// TODO: 여기에 명령 업데이트 UI 처리기 코드를 추가합니다.
	if (m_wndFileView.m_vSelectList.size() > 0)
		pCmdUI->Enable(TRUE);
	else
		pCmdUI->Enable(FALSE);
}
