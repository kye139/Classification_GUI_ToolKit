#pragma once


// CNetNameDlg ��ȭ �����Դϴ�.

class CNetNameDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CNetNameDlg)

public:
	CNetNameDlg(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CNetNameDlg();

// ��ȭ ���� �������Դϴ�.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_NET_NAME };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()
public:
	CString m_strNetName;
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
};
