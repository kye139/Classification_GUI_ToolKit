#pragma once


// CPropertyDetailLayerDlg ��ȭ �����Դϴ�.

class CPropertyDetailLayerDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CPropertyDetailLayerDlg)

public:
	CPropertyDetailLayerDlg(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CPropertyDetailLayerDlg();

// ��ȭ ���� �������Դϴ�.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG2 };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()
};
