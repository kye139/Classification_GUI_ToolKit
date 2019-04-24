// OptimizationPropertyDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "ImageTool.h"
#include "OptimizationPropertyDlg.h"
#include "afxdialogex.h"


// COptimizationPropertyDlg 대화 상자입니다.

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


// COptimizationPropertyDlg 메시지 처리기입니다.


void COptimizationPropertyDlg::OnCbnSelchangeOptimizerCombo()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
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

	// TODO:  여기에 추가 초기화 작업을 추가합니다.
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
				  // 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}


void COptimizationPropertyDlg::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
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
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
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
	// TODO:  RICHEDIT 컨트롤인 경우, 이 컨트롤은
	// CDialogEx::OnInitDialog() 함수를 재지정 
	//하고 마스크에 OR 연산하여 설정된 ENM_CHANGE 플래그를 지정하여 CRichEditCtrl().SetEventMask()를 호출하지 않으면
	// 이 알림 메시지를 보내지 않습니다.

	// TODO:  여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);
	m_sliderLearningRate.SetPos(static_cast<int>(m_fLearningRate * 1000));
}
