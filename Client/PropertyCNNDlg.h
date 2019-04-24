#pragma once
#include "afxwin.h"
#include <vector>
#include "afxbutton.h"
#include "HiddenLayerPropertyStruct.h"
#include "InputPropertyDlg.h"
#include "OutputPropertyDlg.h"
#include "OptimizationPropertyDlg.h"


// CPropertyCNNDlg 대화 상자입니다.

class CPropertyCNNDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CPropertyCNNDlg)

public:
	CPropertyCNNDlg(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CPropertyCNNDlg();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_PROPERTY_CNN_DLG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedAddButton();

	std::vector<CButton *> m_VButtons; // 히든 레이어 버튼을 저장할 벡터
	std::vector<HiddenLayerProperty *> m_VProperty; // 각 레이어 버튼에 대한 속성값 저장 벡터
	
	InputLayerProperty inputProp;
	OutputLayerProperty outputProp;
	OptimizationLayerProperty optimProp;

	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
	CStatic m_InputText;
	CStatic m_OutputText;
	CStatic m_OptimizeText;
	CMFCButton m_InputButton;
	CMFCButton m_OutputButton;
	CMFCButton m_OptimizationButton;
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedDelButton();
	afx_msg void OnBnClickedInputLayout();
	afx_msg void OnBnClickedOutputLayer();
	afx_msg void OnBnClickedOptimization();
};
