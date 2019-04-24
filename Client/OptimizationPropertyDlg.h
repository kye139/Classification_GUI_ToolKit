#pragma once
#include "afxwin.h"
#include "afxcmn.h"

typedef struct {
	int optimizer;
	float momentum;
	float learningRate;
} OptimizationLayerProperty;


// COptimizationPropertyDlg 대화 상자입니다.

class COptimizationPropertyDlg : public CDialogEx
{
	DECLARE_DYNAMIC(COptimizationPropertyDlg)

public:
	COptimizationPropertyDlg(CWnd* pParent = NULL);   // 표준 생성자입니다.
	COptimizationPropertyDlg(OptimizationLayerProperty optimProp, CWnd* pParent = NULL);
	virtual ~COptimizationPropertyDlg();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_OPTIM_PROPERTY_DIALOG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	int m_nOptimizer;
	int m_nMomentun;
	CButton m_Radio5;
	CSliderCtrl m_sliderLearningRate;
	float m_fLearningRate;
	afx_msg void OnCbnSelchangeOptimizerCombo();
	virtual BOOL OnInitDialog();
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);

	OptimizationLayerProperty optimProp;
	CButton m_Radio9;
	CButton m_Radio99;
	CButton m_Radio999;
	afx_msg void OnEnChangeLearningRateEdit();
};
