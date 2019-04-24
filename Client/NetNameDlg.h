#pragma once


// CNetNameDlg 대화 상자입니다.

class CNetNameDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CNetNameDlg)

public:
	CNetNameDlg(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CNetNameDlg();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_NET_NAME };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	CString m_strNetName;
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
};
