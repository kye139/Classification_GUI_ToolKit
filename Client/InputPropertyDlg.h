#pragma once

typedef struct {
	int width;
	int height;
} InputLayerProperty;


// CInputPropertyDlg ��ȭ �����Դϴ�.

class CInputPropertyDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CInputPropertyDlg)

public:
	CInputPropertyDlg(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	CInputPropertyDlg(InputLayerProperty inputProp, CWnd* pParent = NULL);
	virtual ~CInputPropertyDlg();

// ��ȭ ���� �������Դϴ�.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_INPUT_PROPERTY_DIALOG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
	int m_nWidth;
	int m_nHeight;
};
