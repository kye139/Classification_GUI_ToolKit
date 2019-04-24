
#include "stdafx.h"
#include "ImageTool.h"
#include "DataTree.h"
#include "MainFrm.h"
#include "IppImage\IppImage.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDataTree

CDataTree::CDataTree()
{
}

CDataTree::~CDataTree()
{
}

BEGIN_MESSAGE_MAP(CDataTree, CTreeCtrl)
	// ON_NOTIFY_REFLECT(TVN_SELCHANGED, &CDataTree::OnTvnSelchanged)
	ON_NOTIFY_REFLECT(NM_CLICK, &CDataTree::OnNMClick)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDataTree �޽��� ó����

// BOOL CDataTree::OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult)
// {
// 	BOOL bRes = CDataTree::OnNotify(wParam, lParam, pResult);
// 
// 	NMHDR* pNMHDR = (NMHDR*)lParam;
// 	ASSERT(pNMHDR != NULL);
// 
// 	if (pNMHDR && pNMHDR->code == TTN_SHOW && GetToolTips() != NULL)
// 	{
// 		GetToolTips()->SetWindowPos(&wndTop, -1, -1, -1, -1, SWP_NOMOVE | SWP_NOACTIVATE | SWP_NOSIZE);
// 	}
// 
// 	return bRes;
// }

// ���õ� �̹��� �׸� �Ӽ� â�� ���.
// void CDataTree::OnTvnSelchanged(NMHDR *pNMHDR, LRESULT *pResult)
// {
// 	LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);
// 	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
// 	HTREEITEM selectItem = pNMTreeView->itemNew.hItem;
// 	if ((ImageInform *)GetItemData(selectItem) && GetParentItem(selectItem) != GetRootItem())
// 	{
// 		wchar_t value[100];
// 		CMainFrame* pFrame = (CMainFrame *)AfxGetMainWnd();
// 		// ���Ϻ信�� ����� �̹��� ���� ��������
// 		ImageInform* imgInfo = (ImageInform *)GetItemData(selectItem);
// 		IppImage* selImage = imgInfo->image;
// 
// 		CString imgName = GetItemText(selectItem);
// 		int channel = selImage->GetImage().channels();
// 		int width = selImage->GetWidth();
// 		int height = selImage->GetHeight();
// 
// 		CMFCPropertyGridCtrl * propPtr = &pFrame->m_wndProperties.m_wndPropList;
// 		CMFCPropertyGridProperty * nameWindow = propPtr->GetProperty(5)->GetSubItem(0);
// 		CMFCPropertyGridProperty * channelWindow = propPtr->GetProperty(5)->GetSubItem(1);
// 		CMFCPropertyGridProperty * widthWindow = propPtr->GetProperty(5)->GetSubItem(2)->GetSubItem(0);
// 		CMFCPropertyGridProperty * heightWindow = propPtr->GetProperty(5)->GetSubItem(2)->GetSubItem(1);
// 
// 		nameWindow->SetValue(imgName);
// 		wsprintf(value, _T("%d"), channel);
// 		channelWindow->SetValue(value);
// 		wsprintf(value, _T("%d"), width);
// 		widthWindow->SetValue(value);
// 		wsprintf(value, _T("%d"), height);
// 		heightWindow->SetValue(value);
// 	}
// 	*pResult = 0;
// }

BOOL CDataTree::PreTranslateMessage(MSG* pMsg)
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

void CDataTree::OnNMClick(NMHDR *pNMHDR, LRESULT *pResult)
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
