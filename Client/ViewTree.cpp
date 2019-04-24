
#include "stdafx.h"
#include "ImageTool.h"
#include "ViewTree.h"
#include "MainFrm.h"
#include "IppImage\IppImage.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CViewTree

CViewTree::CViewTree()
{
}

CViewTree::~CViewTree()
{
}

BEGIN_MESSAGE_MAP(CViewTree, CTreeCtrl)
	ON_NOTIFY_REFLECT(TVN_SELCHANGED, &CViewTree::OnTvnSelchanged)
	ON_NOTIFY_REFLECT(NM_DBLCLK, &CViewTree::OnNMDblclk)
	ON_NOTIFY_REFLECT(TVN_BEGINLABELEDIT, &CViewTree::OnTvnBeginlabeledit)
	ON_NOTIFY_REFLECT(TVN_ENDLABELEDIT, &CViewTree::OnTvnEndlabeledit)
	ON_NOTIFY_REFLECT(NM_CLICK, &CViewTree::OnNMClick)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CViewTree 메시지 처리기

BOOL CViewTree::OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult)
{
	BOOL bRes = CTreeCtrl::OnNotify(wParam, lParam, pResult);

	NMHDR* pNMHDR = (NMHDR*)lParam;
	ASSERT(pNMHDR != NULL);

	if (pNMHDR && pNMHDR->code == TTN_SHOW && GetToolTips() != NULL)
	{
		GetToolTips()->SetWindowPos(&wndTop, -1, -1, -1, -1, SWP_NOMOVE | SWP_NOACTIVATE | SWP_NOSIZE);
	}

	return bRes;
}

// 선택된 이미지 항목 속성 창에 출력.
void CViewTree::OnTvnSelchanged(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	HTREEITEM selectItem = pNMTreeView->itemNew.hItem;
	if ((ImageInform *)GetItemData(selectItem) && GetParentItem(selectItem) != GetRootItem())
	{
		wchar_t value[100];
		CMainFrame* pFrame = (CMainFrame *)AfxGetMainWnd();
		// 파일뷰에서 저장된 이미지 정보 가져오기
		ImageInform* imgInfo = (ImageInform *)GetItemData(selectItem);
		IppImage* selImage = imgInfo->image;

		CString imgName = GetItemText(selectItem);
		int channel = selImage->GetImage().channels();
		int width = selImage->GetWidth();
		int height = selImage->GetHeight();

		CMFCPropertyGridCtrl * propPtr = &pFrame->m_wndProperties.m_wndPropList;
		CMFCPropertyGridProperty * nameWindow = propPtr->GetProperty(5)->GetSubItem(0);
		CMFCPropertyGridProperty * channelWindow = propPtr->GetProperty(5)->GetSubItem(1);
		CMFCPropertyGridProperty * widthWindow = propPtr->GetProperty(5)->GetSubItem(2)->GetSubItem(0);
		CMFCPropertyGridProperty * heightWindow = propPtr->GetProperty(5)->GetSubItem(2)->GetSubItem(1);

		nameWindow->SetValue(imgName);
		wsprintf(value, _T("%d"), channel);
		channelWindow->SetValue(value);
		wsprintf(value, _T("%d"), width);
		widthWindow->SetValue(value);
		wsprintf(value, _T("%d"), height);
		heightWindow->SetValue(value);
	}
	*pResult = 0;
}


void CViewTree::OnNMDblclk(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);

	if (pNMItemActivate->iItem != -1)
	{
		if (GetItemData(GetSelectedItem()))
		{
			ImageInform * selectImageinfo = (ImageInform*)GetItemData(GetSelectedItem());
			wchar_t id[10] = { 0, };
			wsprintf(id, _T("%d"), selectImageinfo->uniqueId);
			HWND mdi_parent = ::FindWindowEx(AfxGetMainWnd()->m_hWnd, NULL, _T("MDIClient"), NULL);

			HWND mdi_child = ::FindWindowEx(mdi_parent, NULL, id, NULL);

			// 윈도우가 열려 있는 경우
			if (mdi_child != NULL)
			{
				::BringWindowToTop(mdi_child);
				::SetActiveWindow(mdi_child);
				::SetForegroundWindow(mdi_child);
				::ShowWindow(mdi_child, SW_SHOWNORMAL);
				::UpdateWindow(mdi_child);
			}
			else 
			{
				theApp._openId = selectImageinfo->uniqueId;
				theApp._openName = CString(selectImageinfo->name);
				AfxNewImage(selectImageinfo->image);
			}
			
		}
	}

	*pResult = 0;
}

void CViewTree::OnTvnBeginlabeledit(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMTVDISPINFO pTVDispInfo = reinterpret_cast<LPNMTVDISPINFO>(pNMHDR);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.s
	*pResult = 0;
}


void CViewTree::OnTvnEndlabeledit(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMTVDISPINFO pTVDispInfo = reinterpret_cast<LPNMTVDISPINFO>(pNMHDR);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CEdit* edit = GetEditControl();
	TCHAR str[255];
	edit->GetWindowTextW(str, 255);

	// 현재 선택된 트리 아이템 추출
	HTREEITEM cursel = GetSelectedItem();

	// 선택된 항목이 이미지 파일인 경우 - 이름 중복 여부 검사
	if (GetItemData(cursel))
	{
		HTREEITEM pointItem = cursel;
		ImageInform* imgInfo;

		while (pointItem = GetNextSiblingItem(pointItem))
		{
			imgInfo = (ImageInform *)GetItemData(pointItem);

			// 포인팅 항목이 그룹인 경우
			if (!imgInfo)
				continue;
			// 이름 같은 파일이 존재하는 경우
			if (!_tcscmp(imgInfo->name, str))
			{
				MessageBox(_T("동일한 이름의 파일이 존재합니다."));
				return;
			}
		}
		pointItem = cursel;
		while (pointItem = GetPrevSiblingItem(pointItem))
		{
			imgInfo = (ImageInform *)GetItemData(pointItem);

			// 포인팅 항목이 그룹인 경우
			if (!imgInfo)
				continue;
			// 이름 같은 파일이 존재하는 경우
			if (!_tcscmp(imgInfo->name, str))
			{
				MessageBox(_T("동일한 이름의 파일이 존재합니다."));
				return;
			}
		}

		// 내부 파일 이름도 변경
		imgInfo = (ImageInform *)GetItemData(cursel);
		wcscpy_s(imgInfo->name, str);
	}

	SetItemText(pTVDispInfo->item.hItem, str);
	*pResult = 0;
}


BOOL CViewTree::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	if (pMsg->message == WM_KEYDOWN)
	{
		if (GetEditControl() && (pMsg->wParam == VK_RETURN))
		{
			::TranslateMessage(pMsg);
			::DispatchMessage(pMsg);
			return TRUE;
		}
	}

	return CTreeCtrl::PreTranslateMessage(pMsg);
}

// void CViewTree::CheckChildItems(HTREEITEM hItem)
// {
// 	// 입력 받은 트리 아이템의 자식 아이템들을 모조리 체크한다.
// 	if (hItem == NULL)
// 		return;
// 
// 	if (!SetCheck(hItem, TRUE))
// 		exit(-1);
// 
// 	CheckChildItems(GetChildItem(hItem));
// 	CheckChildItems(GetNextItem(hItem, TVGN_NEXT));
// }
// 
// void CViewTree::UnCheckChildItems(HTREEITEM hItem)
// {
// 	// 입력 받은 트리 아이템의 자식 아이템들을 모조리 체크 해제한다.
// 	if (hItem == NULL)
// 		return;
// 
// 	if (!SetCheck(hItem, FALSE))
// 		exit(-1);
// 
// 	UnCheckChildItems(GetChildItem(hItem));
// 	UnCheckChildItems(GetNextItem(hItem, TVGN_NEXT));
// }

void CViewTree::OnNMClick(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	CPoint point;
	UINT nFlags = 0;

	GetCursorPos(&point);
	::ScreenToClient(m_hWnd, &point);

	HTREEITEM hItem = HitTest(point, &nFlags);

	// 아이템이 존재하고 체크박스에 이벤트가 발생했다면
	if (hItem != NULL && (nFlags & TVHT_ONITEMSTATEICON) != 0)
	{
		// 해당 아이템이 체크되어 있다면
		if (GetCheck(hItem))
		{
			// UnCheckChildItems(GetChildItem(hItem));
			int * id = (int *)GetItemData(hItem);
			std::vector<int>::iterator iter;
			for (iter = m_vSelectList.begin(); iter != m_vSelectList.end(); iter++)
			{
				if (*iter == *id)
				{
					m_vSelectList.erase(iter);
					return;
				}
			}
		}
		// 해당 아이템이 체크되어 있지 않다면
		else
		{
			// CheckChildItems(GetChildItem(hItem));
			int* id = (int*)GetItemData(hItem);
			m_vSelectList.push_back(*id);
		}
	}

	*pResult = 0;
}
