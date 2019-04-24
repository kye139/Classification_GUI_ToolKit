#pragma once


// CNewGroupDlg 대화 상자입니다.

class CNewGroupDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CNewGroupDlg)

public:
	CNewGroupDlg(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CNewGroupDlg();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_NEW_GROUP_DLG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	CString m_nGroupName;
};
