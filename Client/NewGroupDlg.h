#pragma once


// CNewGroupDlg ��ȭ �����Դϴ�.

class CNewGroupDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CNewGroupDlg)

public:
	CNewGroupDlg(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CNewGroupDlg();

// ��ȭ ���� �������Դϴ�.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_NEW_GROUP_DLG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()
public:
	CString m_nGroupName;
};
