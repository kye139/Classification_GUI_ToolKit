#pragma once
#include "afxvslistbox.h"
#include <vector>
#include "afxcmn.h"


// CIdSelectDlg 대화 상자입니다.

class CIdSelectDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CIdSelectDlg)

public:
	CIdSelectDlg(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CIdSelectDlg();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ID_SELECT };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	std::vector<char*> idListVec;
	virtual BOOL OnInitDialog();
	CListCtrl m_idList;
	afx_msg void OnLvnItemchangedIdList(NMHDR *pNMHDR, LRESULT *pResult);
	CString m_idStr = _T("");
	bool m_bCreateOrLogin; // false는 로그인, true는 계정 생성
	afx_msg void OnBnClickedCreateIdButton();
	CString m_sCreateId;
};
