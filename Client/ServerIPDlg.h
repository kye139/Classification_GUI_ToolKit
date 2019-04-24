#pragma once
#include "afxcmn.h"
#include <vector>


// CServerIPDlg ��ȭ �����Դϴ�.

class CServerIPDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CServerIPDlg)

public:
	CServerIPDlg(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CServerIPDlg();

// ��ȭ ���� �������Դϴ�.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG3 };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()
public:
	CIPAddressCtrl m_IpAddress;
	afx_msg void OnBnClickedConnectCheck();

	BYTE ipFirst, ipSecond, ipThird, ipForth;

	std::vector<char *> dirList;
	SOCKET hSocket;
};
