// OptimizationPropertyDlg.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "ImageTool.h"
#include "OptimizationPropertyDlg.h"
#include "afxdialogex.h"


// COptimizationPropertyDlg ��ȭ �����Դϴ�.

IMPLEMENT_DYNAMIC(COptimizationPropertyDlg, CDialogEx)

COptimizationPropertyDlg::COptimizationPropertyDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_OPTIM_PROPERTY_DIALOG, pParent)
	, m_nOptimizer(0)
	, m_nMomentun(0)
	, m_fLearningRate(0.1)
{

}
COptimizationPropertyDlg::COptimizationPropertyDlg(OptimizationLayerProperty optimProp, CWnd* pParent)
	: CDialogEx(IDD_OPTIM_PROPERTY_DIALOG, pParent)
	, m_nOptimizer(optimProp.optimizer)
	, m_nMomentun(optimProp.momentum)
	, m_fLearningRate(optimProp.learningRate)
{

}

COptimizationPropertyDlg::~COptimizationPropertyDlg()
{
}

void COptimizationPropertyDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_CBIndex(pDX, IDC_OPTIMIZER_COMBO, m_nOptimizer);
	DDV_MinMaxInt(pDX, m_nOptimizer, 0, 4);
	DDX_Radio(pDX, IDC_MOMENTUM_RADIO1, m_nMomentun);
	DDV_MinMaxInt(pDX, m_nMomentun, 0, 3);
	DDX_Control(pDX, IDC_MOMENTUM_RADIO1, m_Radio5);
	DDX_Control(pDX, IDC_LEARNING_RATE_SLIDE, m_sliderLearningRate);
	DDX_Text(pDX, IDC_LEARNING_RATE_EDIT, m_fLearningRate);
	DDV_MinMaxFloat(pDX, m_fLearningRate, 0.001, 0.1);
	DDX_Control(pDX, IDC_MOMENTUM_RADIO2, m_Radio9);
	DDX_Control(pDX, IDC_MOMENTUM_RADIO3, m_Radio99);
	DDX_Control(pDX, IDC_MOMENTUM_RADIO4, m_Radio999);
}


BEGIN_MESSAGE_MAP(COptimizationPropertyDlg, CDialogEx)
	ON_CBN_SELCHANGE(IDC_OPTIMIZER_COMBO, &COptimizationPropertyDlg::OnCbnSelchangeOptimizerCombo)
	ON_WM_HSCROLL()
	ON_EN_CHANGE(IDC_LEARNING_RATE_EDIT, &COptimizationPropertyDlg::OnEnChangeLearningRateEdit)
END_MESSAGE_MAP()


// COptimizationPropertyDlg �޽��� ó�����Դϴ�.


void COptimizationPropertyDlg::OnCbnSelchangeOptimizerCombo()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	UpdateData(TRUE);
	if (m_nOptimizer >= 1 && m_nOptimizer <= 3)
	{
		m_Radio5.EnableWindow(TRUE);
		m_Radio9.EnableWindow(TRUE);
		m_Radio99.EnableWindow(TRUE);
		m_Radio999.EnableWindow(TRUE);
	}
	else
	{
		m_Radio5.EnableWindow(FALSE);
		m_Radio9.EnableWindow(FALSE);
		m_Radio99.EnableWindow(FALSE);
		m_Radio999.EnableWindow(FALSE);
	}
}


BOOL COptimizationPropertyDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  ���⿡ �߰� �ʱ�ȭ �۾��� �߰��մϴ�.
	if (m_nOptimizer == 0 || m_nOptimizer == 4)
	{
		m_Radio5.EnableWindow(FALSE);
		m_Radio9.EnableWindow(FALSE);
		m_Radio99.EnableWindow(FALSE);
		m_Radio999.EnableWindow(FALSE);
	}

	m_sliderLearningRate.SetRange(1, 100);
	m_sliderLearningRate.SetTicFreq(10);
	m_sliderLearningRate.SetPageSize(10);
	m_sliderLearningRate.SetPos(static_cast<int>(m_fLearningRate * 1000));

	return TRUE;  // return TRUE unless you set the focus to a control
				  // ����: OCX �Ӽ� �������� FALSE�� ��ȯ�ؾ� �մϴ�.
}


void COptimizationPropertyDlg::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰� ��/�Ǵ� �⺻���� ȣ���մϴ�.
	if (m_sliderLearningRate.GetSafeHwnd() == pScrollBar->GetSafeHwnd())
	{
		int pos = m_sliderLearningRate.GetPos();
		m_fLearningRate = pos / 1000.f;
		UpdateData(FALSE);
	}

	CDialogEx::OnHScroll(nSBCode, nPos, pScrollBar);
}


BOOL COptimizationPropertyDlg::OnCommand(WPARAM wParam, LPARAM lParam)
{
	// TODO: ���⿡ Ư��ȭ�� �ڵ带 �߰� ��/�Ǵ� �⺻ Ŭ������ ȣ���մϴ�.
	if (wParam == IDOK)
	{
		UpdateData(TRUE);
		if (m_nMomentun == 0)
			optimProp.momentum = 0.5f;
		else if (m_nMomentun == 1)
			optimProp.momentum = 0.9f;
		else if (m_nMomentun == 2)
			optimProp.momentum = 0.99f;
		else
			optimProp.momentum = 0.999f;
	}

	return CDialogEx::OnCommand(wParam, lParam);
}


void COptimizationPropertyDlg::OnEnChangeLearningRateEdit()
{
	// TODO:  RICHEDIT ��Ʈ���� ���, �� ��Ʈ����
	// CDialogEx::OnInitDialog() �Լ��� ������ 
	//�ϰ� ����ũ�� OR �����Ͽ� ������ ENM_CHANGE �÷��׸� �����Ͽ� CRichEditCtrl().SetEventMask()�� ȣ������ ������
	// �� �˸� �޽����� ������ �ʽ��ϴ�.

	// TODO:  ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	UpdateData(TRUE);
	m_sliderLearningRate.SetPos(static_cast<int>(m_fLearningRate * 1000));
}
