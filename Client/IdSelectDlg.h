#pragma once
#include "afxvslistbox.h"
#include <vector>
#include "afxcmn.h"


// CIdSelectDlg ��ȭ �����Դϴ�.

class CIdSelectDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CIdSelectDlg)

public:
	CIdSelectDlg(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CIdSelectDlg();

// ��ȭ ���� �������Դϴ�.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ID_SELECT };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()
public:
	std::vector<char*> idListVec;
	virtual BOOL OnInitDialog();
	CListCtrl m_idList;
	afx_msg void OnLvnItemchangedIdList(NMHDR *pNMHDR, LRESULT *pResult);
	CString m_idStr = _T("");
	bool m_bCreateOrLogin; // false�� �α���, true�� ���� ����
	afx_msg void OnBnClickedCreateIdButton();
	CString m_sCreateId;
};
