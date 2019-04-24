#pragma once
#include "afxwin.h"
#include "afxcmn.h"


// CLearingParameterDlg ��ȭ �����Դϴ�.

class CLearingParameterDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CLearingParameterDlg)

public:
	CLearingParameterDlg(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CLearingParameterDlg();

	CString m_strNetName;

// ��ȭ ���� �������Դϴ�.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_LEARNING_PROPERTY_DIALOG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

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
