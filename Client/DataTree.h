#pragma once

/////////////////////////////////////////////////////////////////////////////
// CViewTree â�Դϴ�.
#include <vector>

class CDataTree : public CTreeCtrl
{
	// �����Դϴ�.
public:
	CDataTree();

	// �������Դϴ�.
protected:
	//virtual BOOL OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult);

	// �����Դϴ�.
public:
	virtual ~CDataTree();

protected:
	DECLARE_MESSAGE_MAP()
public:
	// afx_msg void OnTvnSelchanged(NMHDR *pNMHDR, LRESULT *pResult);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnNMClick(NMHDR *pNMHDR, LRESULT *pResult);

	std::vector<int> m_vSelectList;
};
