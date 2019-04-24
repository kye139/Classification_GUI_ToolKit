#pragma once
#include "afxwin.h"
#include <vector>
#include "afxbutton.h"
#include "HiddenLayerPropertyStruct.h"
#include "InputPropertyDlg.h"
#include "OutputPropertyDlg.h"
#include "OptimizationPropertyDlg.h"


// CPropertyCNNDlg ��ȭ �����Դϴ�.

class CPropertyCNNDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CPropertyCNNDlg)

public:
	CPropertyCNNDlg(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CPropertyCNNDlg();

// ��ȭ ���� �������Դϴ�.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_PROPERTY_CNN_DLG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedAddButton();

	std::vector<CButton *> m_VButtons; // ���� ���̾� ��ư�� ������ ����
	std::vector<HiddenLayerProperty *> m_VProperty; // �� ���̾� ��ư�� ���� �Ӽ��� ���� ����
	
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
