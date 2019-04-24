#pragma once
#include "afxwin.h"
#include "afxcmn.h"


// CLearingParameterDlg 대화 상자입니다.

class CLearingParameterDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CLearingParameterDlg)

public:
	CLearingParameterDlg(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CLearingParameterDlg();

	CString m_strNetName;

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_LEARNING_PROPERTY_DIALOG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	CComboBox m_comboNetNames;
	int m_nLoopCount;
	int m_nTestPeriod;
	CSliderCtrl m_sliderLoopCount;
	CSliderCtrl m_sliderTestPeriod;
	virtual BOOL OnInitDialog();
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
};
