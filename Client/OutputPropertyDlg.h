#pragma once

typedef struct {
	int oneHotOnOff;
	int numOfClassification;
} OutputLayerProperty;

// COutputPropertyDlg 대화 상자입니다.

class COutputPropertyDlg : public CDialogEx
{
	DECLARE_DYNAMIC(COutputPropertyDlg)

public:
	COutputPropertyDlg(CWnd* pParent = NULL);   // 표준 생성자입니다.
	COutputPropertyDlg(OutputLayerProperty outputProp, CWnd* pParent = NULL);
	virtual ~COutputPropertyDlg();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_OUTPUT_PROPERTY_DIALOG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	int m_nClassNum;
	int m_nOneHotOnOff;
	virtual BOOL OnInitDialog();
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
};
