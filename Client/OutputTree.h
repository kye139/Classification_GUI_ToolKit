#pragma once
#pragma once

/////////////////////////////////////////////////////////////////////////////
// CViewTree 창입니다.
#include <vector>

class COutputTree : public CTreeCtrl
{
	// 생성입니다.
public:
	COutputTree();

	// 재정의입니다.
protected:
	//virtual BOOL OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult);

	// 구현입니다.
public:
	virtual ~COutputTree();

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnTvnSelchanged(NMHDR *pNMHDR, LRESULT *pResult);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnNMClick(NMHDR *pNMHDR, LRESULT *pResult);

	std::vector<int> m_vSelectList;
};
