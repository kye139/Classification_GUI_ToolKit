#pragma once

typedef struct {
	int width;
	int height;
} InputLayerProperty;


// CInputPropertyDlg 대화 상자입니다.

class CInputPropertyDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CInputPropertyDlg)

public:
	CInputPropertyDlg(CWnd* pParent = NULL);   // 표준 생성자입니다.
	CInputPropertyDlg(InputLayerProperty inputProp, CWnd* pParent = NULL);
	virtual ~CInputPropertyDlg();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_INPUT_PROPERTY_DIALOG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
	int m_nWidth;
	int m_nHeight;
};
