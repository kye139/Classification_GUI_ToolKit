
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
// CViewTree �޽��� ó����

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

// ���õ� �̹��� �׸� �Ӽ� â�� ���.
void CViewTree::OnTvnSelchanged(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	HTREEITEM selectItem = pNMTreeView->itemNew.hItem;
	if ((ImageInform *)GetItemData(selectItem) && GetParentItem(selectItem) != GetRootItem())
	{
		wchar_t value[100];
		CMainFrame* pFrame = (CMainFrame *)AfxGetMainWnd();
		// ���Ϻ信�� ����� �̹��� ���� ��������
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
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
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

			// �����찡 ���� �ִ� ���
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
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.s
	*pResult = 0;
}


void CViewTree::OnTvnEndlabeledit(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMTVDISPINFO pTVDispInfo = reinterpret_cast<LPNMTVDISPINFO>(pNMHDR);
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	CEdit* edit = GetEditControl();
	TCHAR str[255];
	edit->GetWindowTextW(str, 255);

	// ���� ���õ� Ʈ�� ������ ����
	HTREEITEM cursel = GetSelectedItem();

	// ���õ� �׸��� �̹��� ������ ��� - �̸� �ߺ� ���� �˻�
	if (GetItemData(cursel))
	{
		HTREEITEM pointItem = cursel;
		ImageInform* imgInfo;

		while (pointItem = GetNextSiblingItem(pointItem))
		{
			imgInfo = (ImageInform *)GetItemData(pointItem);

			// ������ �׸��� �׷��� ���
			if (!imgInfo)
				continue;
			// �̸� ���� ������ �����ϴ� ���
			if (!_tcscmp(imgInfo->name, str))
			{
				MessageBox(_T("������ �̸��� ������ �����մϴ�."));
				return;
			}
		}
		pointItem = cursel;
		while (pointItem = GetPrevSiblingItem(pointItem))
		{
			imgInfo = (ImageInform *)GetItemData(pointItem);

			// ������ �׸��� �׷��� ���
			if (!imgInfo)
				continue;
			// �̸� ���� ������ �����ϴ� ���
			if (!_tcscmp(imgInfo->name, str))
			{
				MessageBox(_T("������ �̸��� ������ �����մϴ�."));
				return;
			}
		}

		// ���� ���� �̸��� ����
		imgInfo = (ImageInform *)GetItemData(cursel);
		wcscpy_s(imgInfo->name, str);
	}

	SetItemText(pTVDispInfo->item.hItem, str);
	*pResult = 0;
}


BOOL CViewTree::PreTranslateMessage(MSG* pMsg)
{
	// TODO: ���⿡ Ư��ȭ�� �ڵ带 �߰� ��/�Ǵ� �⺻ Ŭ������ ȣ���մϴ�.
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
// 	// �Է� ���� Ʈ�� �������� �ڽ� �����۵��� ������ üũ�Ѵ�.
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
// 	// �Է� ���� Ʈ�� �������� �ڽ� �����۵��� ������ üũ �����Ѵ�.
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
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.

	CPoint point;
	UINT nFlags = 0;

	GetCursorPos(&point);
	::ScreenToClient(m_hWnd, &point);

	HTREEITEM hItem = HitTest(point, &nFlags);

	// �������� �����ϰ� üũ�ڽ��� �̺�Ʈ�� �߻��ߴٸ�
	if (hItem != NULL && (nFlags & TVHT_ONITEMSTATEICON) != 0)
	{
		// �ش� �������� üũ�Ǿ� �ִٸ�
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
		// �ش� �������� üũ�Ǿ� ���� �ʴٸ�
		else
		{
			// CheckChildItems(GetChildItem(hItem));
			int* id = (int*)GetItemData(hItem);
			m_vSelectList.push_back(*id);
		}
	}

	*pResult = 0;
}
