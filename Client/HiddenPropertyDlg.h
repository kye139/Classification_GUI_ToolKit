#pragma once
#include "afxcmn.h"
#include "HiddenLayerPropertyStruct.h"
#include "afxwin.h"


// CHiddenPropertyDlg 대화 상자입니다.

class CHiddenPropertyDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CHiddenPropertyDlg)

public:
	CHiddenPropertyDlg(CWnd* pParent = NULL);
	CHiddenPropertyDlg(HiddenLayerProperty* prop, CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CHiddenPropertyDlg();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_HIDDEN_PROPERTY_DIALOG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	int m_InitMethod;
	float m_fAverage;
	float m_fSigma;
	int m_nFilterSize;
	int m_nStride;
	int m_nPadding;
	int m_nOutput;
	float m_fBiasis;
	CSliderCtrl m_sliderAverage;
	virtual BOOL OnInitDialog();
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnEnChangeAverageEdit();
	CSliderCtrl m_sliderSigma;
	afx_msg void OnEnChangeSigmaEdit();
	CSliderCtrl m_sliderStride;
	afx_msg void OnEnChangeStrideEdit();
	CSliderCtrl m_sliderOutput;
	afx_msg void OnEnChangeOutputEdit();
	CSliderCtrl m_sliderBias;
	afx_msg void OnEnChangeBiasisEdit();
	float m_fFullyAverage;
	float m_fFullySigma;
	int m_nFullyOutput;
	float m_fFullyBias;
	int m_nActiveMethod;
	CSliderCtrl m_sliderFullyAverage;
	afx_msg void OnEnChangeFullyAverageEdit();
	CSliderCtrl m_sliderFullySigma;
	afx_msg void OnEnChangeFullySigmaEdit();
	CSliderCtrl m_sliderFullyOutput;
	afx_msg void OnEnChangeFullyOutputEdit();
	CSliderCtrl m_sliderFullyBias;
	afx_msg void OnEnChangeFullyBiasEdit();
	int m_nFullyInitMethod;
	CSliderCtrl m_sliderPoolFilterSize;
	int m_nPoolFilterSize;
	CSliderCtrl m_sliderPoolStride;
	int m_nPoolStride;
	int m_nPoolPadding;
	int m_nPoolInitMethod;
	afx_msg void OnEnChangePoolFilterSizeEdit();
	afx_msg void OnEnChangePoolStrideEdit();
	afx_msg void OnBnClickedPoolOnoff();
	CButton m_checkPoolOnOff;
	int m_nConvOrFully;
	afx_msg void OnBnClickedLayer(UINT value);
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);

	bool m_bPoolingOnOff;

	HiddenLayerProperty* propert;
	BOOL m_bBatchOnOff;
	BOOL m_bDropOnOff;
	CSliderCtrl m_sliderDropProportion;
	float m_fDropProportion;
	afx_msg void OnBnClickedDropoutOnoff();
	afx_msg void OnEnChangeDropProportionEdit();
};
