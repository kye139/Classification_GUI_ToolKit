#pragma once

typedef struct {
	int oneHotOnOff;
	int numOfClassification;
} OutputLayerProperty;

// COutputPropertyDlg ��ȭ �����Դϴ�.

class COutputPropertyDlg : public CDialogEx
{
	DECLARE_DYNAMIC(COutputPropertyDlg)

public:
	COutputPropertyDlg(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	COutputPropertyDlg(OutputLayerProperty outputProp, CWnd* pParent = NULL);
	virtual ~COutputPropertyDlg();

// ��ȭ ���� �������Դϴ�.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_OUTPUT_PROPERTY_DIALOG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()
public:
	int m_nClassNum;
	int m_nOneHotOnOff;
	virtual BOOL OnInitDialog();
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
};
