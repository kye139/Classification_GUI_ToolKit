#pragma once
#include "afxcmn.h"
#include <vector>


// CServerIPDlg 대화 상자입니다.

class CServerIPDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CServerIPDlg)

public:
	CServerIPDlg(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CServerIPDlg();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG3 };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	CIPAddressCtrl m_IpAddress;
	afx_msg void OnBnClickedConnectCheck();

	BYTE ipFirst, ipSecond, ipThird, ipForth;

	std::vector<char *> dirList;
	SOCKET hSocket;
};
