// HiddenPropertyDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "ImageTool.h"
#include "HiddenPropertyDlg.h"
#include "afxdialogex.h"


// CHiddenPropertyDlg 대화 상자입니다.

IMPLEMENT_DYNAMIC(CHiddenPropertyDlg, CDialogEx)

CHiddenPropertyDlg::CHiddenPropertyDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_HIDDEN_PROPERTY_DIALOG, pParent)
	, m_InitMethod(0)
	, m_fAverage(0.0)
	, m_fSigma(0.1)
	, m_nFilterSize(0)
	, m_nStride(1)
	, m_nPadding(0)
	, m_nOutput(16)
	, m_fBiasis(0.1)
	, m_fFullyAverage(0.0)
	, m_fFullySigma(0.1)
	, m_nFullyOutput(256)
	, m_fFullyBias(0.1)
	, m_nActiveMethod(0)
	, m_nFullyInitMethod(0)
	, m_nPoolFilterSize(2)
	, m_nPoolStride(2)
	, m_nPoolPadding(0)
	, m_nPoolInitMethod(0)
	, m_nConvOrFully(0)
	, m_bBatchOnOff(FALSE)
	, m_bDropOnOff(FALSE)
	, m_fDropProportion(0)
{
}

CHiddenPropertyDlg::CHiddenPropertyDlg(HiddenLayerProperty* prop, CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_HIDDEN_PROPERTY_DIALOG, pParent)
	, m_InitMethod(prop->conv.initMethod)
	, m_fAverage(prop->conv.average)
	, m_fSigma(prop->conv.sigma)
	, m_nFilterSize(prop->conv.filterSize)
	, m_nStride(prop->conv.stride)
	, m_nPadding(prop->conv.padding)
	, m_nOutput(prop->conv.output)
	, m_fBiasis(prop->conv.bias)
	, m_fFullyAverage(prop->fulCon.average)
	, m_fFullySigma(prop->fulCon.sigma)
	, m_nFullyOutput(prop->fulCon.output)
	, m_fFullyBias(prop->fulCon.bias)
	, m_nActiveMethod(prop->activ.activationMethod)
	, m_nFullyInitMethod(prop->fulCon.initMethod)
	, m_nPoolFilterSize(prop->pool.filterSize)
	, m_nPoolStride(prop->pool.stride)
	, m_nPoolPadding(prop->pool.padding)
	, m_nPoolInitMethod(prop->pool.poolingMethod)
	, m_bPoolingOnOff(prop->PoolingOnOff)
	, m_nConvOrFully(prop->convOrFully)
	, m_fDropProportion(prop->drop.proportion)
	, m_bBatchOnOff(prop->BatchOnOff)
	, m_bDropOnOff(prop->DropOnOff)
	, propert(prop)
{
}

CHiddenPropertyDlg::~CHiddenPropertyDlg()
{
}

void CHiddenPropertyDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_CBIndex(pDX, IDC_INIT_METHOD, m_InitMethod);
	DDX_Text(pDX, IDC_AVERAGE_EDIT, m_fAverage);
	DDV_MinMaxFloat(pDX, m_fAverage, -0.3f, 0.3f);
	DDX_Text(pDX, IDC_SIGMA_EDIT, m_fSigma);
	DDV_MinMaxFloat(pDX, m_fSigma, 0.01f, 1.0f);
	DDX_CBIndex(pDX, IDC_FILTER_SIZE, m_nFilterSize);
	DDX_Text(pDX, IDC_STRIDE_EDIT, m_nStride);
	DDV_MinMaxInt(pDX, m_nStride, 1, 5);
	DDX_CBIndex(pDX, IDC_PADDING, m_nPadding);
	DDX_Text(pDX, IDC_OUTPUT_EDIT, m_nOutput);
	DDV_MinMaxInt(pDX, m_nOutput, 1, 100);
	DDX_Text(pDX, IDC_BIASIS_EDIT, m_fBiasis);
	DDV_MinMaxFloat(pDX, m_fBiasis, 0.0f, 0.3f);
	DDX_Control(pDX, IDC_AVERAGE_SLIDER, m_sliderAverage);
	DDX_Control(pDX, IDC_SIGMA_SLIDER, m_sliderSigma);
	DDX_Control(pDX, IDC_STRIDE_SLIDER, m_sliderStride);
	DDX_Control(pDX, IDC_OUTPUT_SLIDER, m_sliderOutput);
	DDX_Control(pDX, IDC_BIAS_SLIDER, m_sliderBias);
	DDX_Text(pDX, IDC_FULLY_AVERAGE_EDIT, m_fFullyAverage);
	DDV_MinMaxFloat(pDX, m_fFullyAverage, -0.3, 0.3);
	DDX_Text(pDX, IDC_FULLY_SIGMA_EDIT, m_fFullySigma);
	DDV_MinMaxFloat(pDX, m_fFullySigma, 0.01, 1.0);
	DDX_Text(pDX, IDC_FULLY_OUTPUT_EDIT, m_nFullyOutput);
	DDV_MinMaxInt(pDX, m_nFullyOutput, 1, 1024);
	DDX_Text(pDX, IDC_FULLY_BIAS_EDIT, m_fFullyBias);
	DDV_MinMaxFloat(pDX, m_fFullyBias, 0.0, 0.3);
	DDX_CBIndex(pDX, IDC_ACTIVE_INIT_METHOD, m_nActiveMethod);
	DDX_Control(pDX, IDC_FULLY_AVERAGE_SLIDE, m_sliderFullyAverage);
	DDX_Control(pDX, IDC_FULLY_SIGMA_SLIDE, m_sliderFullySigma);
	DDX_Control(pDX, IDC_FULLY_OUTPUT_SLIDE, m_sliderFullyOutput);
	DDX_Control(pDX, IDC_FULLY_BIAS_SLIDE, m_sliderFullyBias);
	DDX_CBIndex(pDX, IDC_FULLY_INIT_METHOD, m_nFullyInitMethod);
	DDX_Control(pDX, IDC_POOL_FILTER_SIZE_SLIDE, m_sliderPoolFilterSize);
	DDX_Text(pDX, IDC_POOL_FILTER_SIZE_EDIT, m_nPoolFilterSize);
	DDV_MinMaxInt(pDX, m_nPoolFilterSize, 0, 100);
	DDX_Control(pDX, IDC_POOL_STRIDE_SLIDE, m_sliderPoolStride);
	DDX_Text(pDX, IDC_POOL_STRIDE_EDIT, m_nPoolStride);
	DDV_MinMaxInt(pDX, m_nPoolStride, 1, 100);
	DDX_CBIndex(pDX, IDC_POOL_PADDING, m_nPoolPadding);
	DDX_CBIndex(pDX, IDC_POOL_INIT_METHOD, m_nPoolInitMethod);
	DDX_Control(pDX, IDC_POOL_ONOFF, m_checkPoolOnOff);
	DDX_Radio(pDX, IDC_RADIO1, m_nConvOrFully);
	DDX_Check(pDX, IDC_BATCH_ONOFF, m_bBatchOnOff);
	DDX_Check(pDX, IDC_DROPOUT_ONOFF, m_bDropOnOff);
	DDX_Control(pDX, IDC_DROP_PROPORTION_SLIDER, m_sliderDropProportion);
	DDX_Text(pDX, IDC_DROP_PROPORTION_EDIT, m_fDropProportion);
	DDV_MinMaxFloat(pDX, m_fDropProportion, 0.0, 0.9);
}


BEGIN_MESSAGE_MAP(CHiddenPropertyDlg, CDialogEx)
	ON_WM_HSCROLL()
	ON_EN_CHANGE(IDC_AVERAGE_EDIT, &CHiddenPropertyDlg::OnEnChangeAverageEdit)
	ON_EN_CHANGE(IDC_SIGMA_EDIT, &CHiddenPropertyDlg::OnEnChangeSigmaEdit)
	ON_EN_CHANGE(IDC_STRIDE_EDIT, &CHiddenPropertyDlg::OnEnChangeStrideEdit)
	ON_EN_CHANGE(IDC_OUTPUT_EDIT, &CHiddenPropertyDlg::OnEnChangeOutputEdit)
	ON_EN_CHANGE(IDC_BIASIS_EDIT, &CHiddenPropertyDlg::OnEnChangeBiasisEdit)
	ON_EN_CHANGE(IDC_FULLY_AVERAGE_EDIT, &CHiddenPropertyDlg::OnEnChangeFullyAverageEdit)
	ON_EN_CHANGE(IDC_FULLY_SIGMA_EDIT, &CHiddenPropertyDlg::OnEnChangeFullySigmaEdit)
	ON_EN_CHANGE(IDC_FULLY_OUTPUT_EDIT, &CHiddenPropertyDlg::OnEnChangeFullyOutputEdit)
	ON_EN_CHANGE(IDC_FULLY_BIAS_EDIT, &CHiddenPropertyDlg::OnEnChangeFullyBiasEdit)
	ON_EN_CHANGE(IDC_POOL_FILTER_SIZE_EDIT, &CHiddenPropertyDlg::OnEnChangePoolFilterSizeEdit)
	ON_EN_CHANGE(IDC_POOL_STRIDE_EDIT, &CHiddenPropertyDlg::OnEnChangePoolStrideEdit)
	ON_BN_CLICKED(IDC_POOL_ONOFF, &CHiddenPropertyDlg::OnBnClickedPoolOnoff)
	ON_CONTROL_RANGE(BN_CLICKED, IDC_RADIO1, IDC_RADIO2, CHiddenPropertyDlg::OnBnClickedLayer)
	ON_BN_CLICKED(IDC_DROPOUT_ONOFF, &CHiddenPropertyDlg::OnBnClickedDropoutOnoff)
	ON_EN_CHANGE(IDC_DROP_PROPORTION_EDIT, &CHiddenPropertyDlg::OnEnChangeDropProportionEdit)
END_MESSAGE_MAP()


// CHiddenPropertyDlg 메시지 처리기입니다.


BOOL CHiddenPropertyDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.
	UpdateData(FALSE);

	m_sliderAverage.SetRange(0, 60);
	m_sliderAverage.SetTicFreq(5);
	m_sliderAverage.SetPageSize(5);
	m_sliderAverage.SetPos(static_cast<int>(m_fAverage) * 100 + 30);

	m_sliderSigma.SetRange(1, 100);
	m_sliderSigma.SetTicFreq(10);
	m_sliderSigma.SetPageSize(5);
	m_sliderSigma.SetPos(static_cast<int>(m_fSigma * 100));

	m_sliderStride.SetRange(1, 5);
	m_sliderStride.SetTicFreq(1);
	m_sliderStride.SetPageSize(1);
	m_sliderStride.SetPos(m_nStride);

	m_sliderOutput.SetRange(1, 100);
	m_sliderOutput.SetTicFreq(10);
	m_sliderOutput.SetPageSize(5);
	m_sliderOutput.SetPos(m_nOutput);
	
	m_sliderBias.SetRange(0, 30);
	m_sliderBias.SetTicFreq(5);
	m_sliderBias.SetPageSize(5);
	m_sliderBias.SetPos(static_cast<int>(m_fBiasis * 100));

	m_sliderFullyAverage.SetRange(0, 60);
	m_sliderFullyAverage.SetTicFreq(5);
	m_sliderFullyAverage.SetPageSize(5);
	m_sliderFullyAverage.SetPos(static_cast<int>(m_fFullyAverage) * 100 + 30);

	m_sliderFullySigma.SetRange(1, 100);
	m_sliderFullySigma.SetTicFreq(10);
	m_sliderFullySigma.SetPageSize(5);
	m_sliderFullySigma.SetPos(static_cast<int>(m_fFullySigma * 100));

	m_sliderFullyOutput.SetRange(1, 1024);
	m_sliderFullyOutput.SetTicFreq(100);
	m_sliderFullyOutput.SetPageSize(10);
	m_sliderFullyOutput.SetPos(m_nFullyOutput);

	m_sliderFullyBias.SetRange(0, 30);
	m_sliderFullyBias.SetTicFreq(5);
	m_sliderFullyBias.SetPageSize(5);
	m_sliderFullyBias.SetPos(static_cast<int>(m_fFullyBias * 100));

	m_sliderPoolFilterSize.SetRange(1, 100);
	m_sliderPoolFilterSize.SetTicFreq(10);
	m_sliderPoolFilterSize.SetPageSize(10);
	m_sliderPoolFilterSize.SetPos(m_nPoolFilterSize);

	m_sliderPoolStride.SetRange(1, 100);
	m_sliderPoolStride.SetTicFreq(10);
	m_sliderPoolStride.SetPageSize(10);
	m_sliderPoolStride.SetPos(m_nPoolStride);

	m_sliderDropProportion.SetRange(0, 9);
	m_sliderDropProportion.SetTicFreq(1);
	m_sliderDropProportion.SetPageSize(1);
	m_sliderDropProportion.SetPos(static_cast<int>(m_fDropProportion * 10));

	if (m_bPoolingOnOff == true)
		m_checkPoolOnOff.SetCheck(BST_CHECKED);
	else if (m_bPoolingOnOff == false)
		m_checkPoolOnOff.SetCheck(BST_UNCHECKED);

	OnBnClickedPoolOnoff(); // 풀링 선택 여부 적용

	OnBnClickedDropoutOnoff(); // 드롭아웃 선택 여부 적용

	OnBnClickedLayer(0); // 레이어 종류 선택 적용

	// // 초기 풀링 비활성화
	// m_checkPoolOnOff.SetCheck(BST_UNCHECKED);
	// m_sliderPoolFilterSize.EnableWindow(FALSE);
	// GetDlgItem(IDC_POOL_FILTER_SIZE_EDIT)->EnableWindow(FALSE);
	// m_sliderPoolStride.EnableWindow(FALSE);
	// GetDlgItem(IDC_POOL_STRIDE_EDIT)->EnableWindow(FALSE);
	// GetDlgItem(IDC_POOL_PADDING)->EnableWindow(FALSE);
	// GetDlgItem(IDC_POOL_INIT_METHOD)->EnableWindow(FALSE);
	// 
	// // 합성곱 레이어 활성화
	// GetDlgItem(IDC_INIT_METHOD)->EnableWindow(TRUE);
	// m_sliderAverage.EnableWindow(TRUE);
	// GetDlgItem(IDC_AVERAGE_EDIT)->EnableWindow(TRUE);
	// m_sliderSigma.EnableWindow(TRUE);
	// GetDlgItem(IDC_SIGMA_EDIT)->EnableWindow(TRUE);
	// GetDlgItem(IDC_FILTER_SIZE)->EnableWindow(TRUE);
	// m_sliderStride.EnableWindow(TRUE);
	// GetDlgItem(IDC_STRIDE_EDIT)->EnableWindow(TRUE);
	// GetDlgItem(IDC_PADDING)->EnableWindow(TRUE);
	// m_sliderOutput.EnableWindow(TRUE);
	// GetDlgItem(IDC_OUTPUT_EDIT)->EnableWindow(TRUE);
	// m_sliderBias.EnableWindow(TRUE);
	// GetDlgItem(IDC_BIASIS_EDIT)->EnableWindow(TRUE);
	// 
	// // 완전 연결 레이어 비활성화
	// GetDlgItem(IDC_FULLY_INIT_METHOD)->EnableWindow(FALSE);
	// m_sliderFullyAverage.EnableWindow(FALSE);
	// GetDlgItem(IDC_FULLY_AVERAGE_EDIT)->EnableWindow(FALSE);
	// m_sliderFullySigma.EnableWindow(FALSE);
	// GetDlgItem(IDC_FULLY_SIGMA_EDIT)->EnableWindow(FALSE);
	// m_sliderFullyOutput.EnableWindow(FALSE);
	// GetDlgItem(IDC_FULLY_OUTPUT_EDIT)->EnableWindow(FALSE);
	// m_sliderFullyBias.EnableWindow(FALSE);
	// GetDlgItem(IDC_FULLY_BIAS_EDIT)->EnableWindow(FALSE);


	return TRUE;  // return TRUE unless you set the focus to a control
				  // 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}


void CHiddenPropertyDlg::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	if (m_sliderAverage.GetSafeHwnd() == pScrollBar->GetSafeHwnd())
	{
		int pos = m_sliderAverage.GetPos();
		m_fAverage = ((pos - 30) / 100.f);
		UpdateData(FALSE);
	} 
	else if (m_sliderSigma.GetSafeHwnd() == pScrollBar->GetSafeHwnd())
	{
		int pos = m_sliderSigma.GetPos();
		m_fSigma = (pos / 100.f);
		UpdateData(FALSE);
	}
	else if (m_sliderStride.GetSafeHwnd() == pScrollBar->GetSafeHwnd())
	{
		int pos = m_sliderStride.GetPos();
		m_nStride = pos;
		UpdateData(FALSE);
	}
	else if (m_sliderOutput.GetSafeHwnd() == pScrollBar->GetSafeHwnd())
	{
		int pos = m_sliderOutput.GetPos();
		m_nOutput = pos;
		UpdateData(FALSE);
	}
	else if (m_sliderBias.GetSafeHwnd() == pScrollBar->GetSafeHwnd())
	{
		int pos = m_sliderBias.GetPos();
		m_fBiasis = pos / 100.f;
		UpdateData(FALSE);
	}
	else if (m_sliderFullyAverage.GetSafeHwnd() == pScrollBar->GetSafeHwnd())
	{
		int pos = m_sliderFullyAverage.GetPos();
		m_fFullyAverage = ((pos - 30) / 100.f);
		UpdateData(FALSE);
	}
	else if (m_sliderFullySigma.GetSafeHwnd() == pScrollBar->GetSafeHwnd())
	{
		int pos = m_sliderFullySigma.GetPos();
		m_fFullySigma = (pos / 100.f);
		UpdateData(FALSE);
	}
	else if (m_sliderFullyOutput.GetSafeHwnd() == pScrollBar->GetSafeHwnd())
	{
		int pos = m_sliderFullyOutput.GetPos();
		m_nFullyOutput = pos;
		UpdateData(FALSE);
	}
	else if (m_sliderFullyBias.GetSafeHwnd() == pScrollBar->GetSafeHwnd())
	{
		int pos = m_sliderFullyBias.GetPos();
		m_fFullyBias = pos / 100.f;
		UpdateData(FALSE);
	}
	else if (m_sliderPoolFilterSize.GetSafeHwnd() == pScrollBar->GetSafeHwnd())
	{
		int pos = m_sliderPoolFilterSize.GetPos();
		m_nPoolFilterSize = pos;
		UpdateData(FALSE);
	}
	else if (m_sliderPoolStride.GetSafeHwnd() == pScrollBar->GetSafeHwnd())
	{
		int pos = m_sliderPoolStride.GetPos();
		m_nPoolStride = pos;
		UpdateData(FALSE);
	}
	else if (m_sliderDropProportion.GetSafeHwnd() == pScrollBar->GetSafeHwnd())
	{
		int pos = m_sliderDropProportion.GetPos();
		m_fDropProportion = pos / 10.f;
		UpdateData(FALSE);
	}

	CDialogEx::OnHScroll(nSBCode, nPos, pScrollBar);
}


void CHiddenPropertyDlg::OnEnChangeAverageEdit()
{
	// TODO:  RICHEDIT 컨트롤인 경우, 이 컨트롤은
	// CDialogEx::OnInitDialog() 함수를 재지정 
	//하고 마스크에 OR 연산하여 설정된 ENM_CHANGE 플래그를 지정하여 CRichEditCtrl().SetEventMask()를 호출하지 않으면
	// 이 알림 메시지를 보내지 않습니다.

	// TODO:  여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);
	m_sliderAverage.SetPos(static_cast<int>(m_fAverage) * 100 + 30);
}


void CHiddenPropertyDlg::OnEnChangeSigmaEdit()
{
	// TODO:  RICHEDIT 컨트롤인 경우, 이 컨트롤은
	// CDialogEx::OnInitDialog() 함수를 재지정 
	//하고 마스크에 OR 연산하여 설정된 ENM_CHANGE 플래그를 지정하여 CRichEditCtrl().SetEventMask()를 호출하지 않으면
	// 이 알림 메시지를 보내지 않습니다.

	// TODO:  여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);
	m_sliderSigma.SetPos(static_cast<int>(m_fSigma * 100));
}


void CHiddenPropertyDlg::OnEnChangeStrideEdit()
{
	// TODO:  RICHEDIT 컨트롤인 경우, 이 컨트롤은
	// CDialogEx::OnInitDialog() 함수를 재지정 
	//하고 마스크에 OR 연산하여 설정된 ENM_CHANGE 플래그를 지정하여 CRichEditCtrl().SetEventMask()를 호출하지 않으면
	// 이 알림 메시지를 보내지 않습니다.

	// TODO:  여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);
	m_sliderStride.SetPos(m_nStride);
}


void CHiddenPropertyDlg::OnEnChangeOutputEdit()
{
	// TODO:  RICHEDIT 컨트롤인 경우, 이 컨트롤은
	// CDialogEx::OnInitDialog() 함수를 재지정 
	//하고 마스크에 OR 연산하여 설정된 ENM_CHANGE 플래그를 지정하여 CRichEditCtrl().SetEventMask()를 호출하지 않으면
	// 이 알림 메시지를 보내지 않습니다.

	// TODO:  여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);
	m_sliderOutput.SetPos(m_nOutput);
}


void CHiddenPropertyDlg::OnEnChangeBiasisEdit()
{
	// TODO:  RICHEDIT 컨트롤인 경우, 이 컨트롤은
	// CDialogEx::OnInitDialog() 함수를 재지정 
	//하고 마스크에 OR 연산하여 설정된 ENM_CHANGE 플래그를 지정하여 CRichEditCtrl().SetEventMask()를 호출하지 않으면
	// 이 알림 메시지를 보내지 않습니다.

	// TODO:  여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);
	m_sliderBias.SetPos(m_fBiasis * 100);
}


void CHiddenPropertyDlg::OnEnChangeFullyAverageEdit()
{
	// TODO:  RICHEDIT 컨트롤인 경우, 이 컨트롤은
	// CDialogEx::OnInitDialog() 함수를 재지정 
	//하고 마스크에 OR 연산하여 설정된 ENM_CHANGE 플래그를 지정하여 CRichEditCtrl().SetEventMask()를 호출하지 않으면
	// 이 알림 메시지를 보내지 않습니다.

	// TODO:  여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);
	m_sliderFullyAverage.SetPos(static_cast<int>(m_fFullyAverage) * 100 + 30);
}


void CHiddenPropertyDlg::OnEnChangeFullySigmaEdit()
{
	// TODO:  RICHEDIT 컨트롤인 경우, 이 컨트롤은
	// CDialogEx::OnInitDialog() 함수를 재지정 
	//하고 마스크에 OR 연산하여 설정된 ENM_CHANGE 플래그를 지정하여 CRichEditCtrl().SetEventMask()를 호출하지 않으면
	// 이 알림 메시지를 보내지 않습니다.

	// TODO:  여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);
	m_sliderFullySigma.SetPos(static_cast<int>(m_fFullySigma * 100));
}


void CHiddenPropertyDlg::OnEnChangeFullyOutputEdit()
{
	// TODO:  RICHEDIT 컨트롤인 경우, 이 컨트롤은
	// CDialogEx::OnInitDialog() 함수를 재지정 
	//하고 마스크에 OR 연산하여 설정된 ENM_CHANGE 플래그를 지정하여 CRichEditCtrl().SetEventMask()를 호출하지 않으면
	// 이 알림 메시지를 보내지 않습니다.

	// TODO:  여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);
	m_sliderFullyOutput.SetPos(m_nFullyOutput);
}


void CHiddenPropertyDlg::OnEnChangeFullyBiasEdit()
{
	// TODO:  RICHEDIT 컨트롤인 경우, 이 컨트롤은
	// CDialogEx::OnInitDialog() 함수를 재지정 
	//하고 마스크에 OR 연산하여 설정된 ENM_CHANGE 플래그를 지정하여 CRichEditCtrl().SetEventMask()를 호출하지 않으면
	// 이 알림 메시지를 보내지 않습니다.

	// TODO:  여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);
	m_sliderFullyBias.SetPos(m_fFullyBias * 100);
}


void CHiddenPropertyDlg::OnEnChangePoolFilterSizeEdit()
{
	// TODO:  RICHEDIT 컨트롤인 경우, 이 컨트롤은
	// CDialogEx::OnInitDialog() 함수를 재지정 
	//하고 마스크에 OR 연산하여 설정된 ENM_CHANGE 플래그를 지정하여 CRichEditCtrl().SetEventMask()를 호출하지 않으면
	// 이 알림 메시지를 보내지 않습니다.

	// TODO:  여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);
	m_sliderPoolFilterSize.SetPos(m_nPoolFilterSize);
}


void CHiddenPropertyDlg::OnEnChangePoolStrideEdit()
{
	// TODO:  RICHEDIT 컨트롤인 경우, 이 컨트롤은
	// CDialogEx::OnInitDialog() 함수를 재지정 
	//하고 마스크에 OR 연산하여 설정된 ENM_CHANGE 플래그를 지정하여 CRichEditCtrl().SetEventMask()를 호출하지 않으면
	// 이 알림 메시지를 보내지 않습니다.

	// TODO:  여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);
	m_sliderPoolStride.SetPos(m_nPoolStride);
}

void CHiddenPropertyDlg::OnEnChangeDropProportionEdit()
{
	// TODO:  RICHEDIT 컨트롤인 경우, 이 컨트롤은
	// CDialogEx::OnInitDialog() 함수를 재지정 
	//하고 마스크에 OR 연산하여 설정된 ENM_CHANGE 플래그를 지정하여 CRichEditCtrl().SetEventMask()를 호출하지 않으면
	// 이 알림 메시지를 보내지 않습니다.

	// TODO:  여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);
	m_sliderDropProportion.SetPos(static_cast<int>(m_fDropProportion * 10));
}



void CHiddenPropertyDlg::OnBnClickedPoolOnoff()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	
	// 체크가 된 상태라면
	if (m_checkPoolOnOff.GetCheck() == BST_CHECKED)
	{
		m_sliderPoolFilterSize.EnableWindow(TRUE);
		GetDlgItem(IDC_POOL_FILTER_SIZE_EDIT)->EnableWindow(TRUE);
		m_sliderPoolStride.EnableWindow(TRUE);
		GetDlgItem(IDC_POOL_STRIDE_EDIT)->EnableWindow(TRUE);
		GetDlgItem(IDC_POOL_PADDING)->EnableWindow(TRUE);
		GetDlgItem(IDC_POOL_INIT_METHOD)->EnableWindow(TRUE);
	}
	// 체크가 해제된 상태라면
	else if (m_checkPoolOnOff.GetCheck() == BST_UNCHECKED)
	{
		m_sliderPoolFilterSize.EnableWindow(FALSE);
		GetDlgItem(IDC_POOL_FILTER_SIZE_EDIT)->EnableWindow(FALSE);
		m_sliderPoolStride.EnableWindow(FALSE);
		GetDlgItem(IDC_POOL_STRIDE_EDIT)->EnableWindow(FALSE);
		GetDlgItem(IDC_POOL_PADDING)->EnableWindow(FALSE);
		GetDlgItem(IDC_POOL_INIT_METHOD)->EnableWindow(FALSE);
	}
}

void CHiddenPropertyDlg::OnBnClickedLayer(UINT value)
{
	UpdateData(TRUE);

	switch (m_nConvOrFully)
	{
	case 0:
		// 합성곱 레이어 활성화
		GetDlgItem(IDC_INIT_METHOD)->EnableWindow(TRUE);
		m_sliderAverage.EnableWindow(TRUE);
		GetDlgItem(IDC_AVERAGE_EDIT)->EnableWindow(TRUE);
		m_sliderSigma.EnableWindow(TRUE);
		GetDlgItem(IDC_SIGMA_EDIT)->EnableWindow(TRUE);
		GetDlgItem(IDC_FILTER_SIZE)->EnableWindow(TRUE);
		m_sliderStride.EnableWindow(TRUE);
		GetDlgItem(IDC_STRIDE_EDIT)->EnableWindow(TRUE);
		GetDlgItem(IDC_PADDING)->EnableWindow(TRUE);
		m_sliderOutput.EnableWindow(TRUE);
		GetDlgItem(IDC_OUTPUT_EDIT)->EnableWindow(TRUE);
		m_sliderBias.EnableWindow(TRUE);
		GetDlgItem(IDC_BIASIS_EDIT)->EnableWindow(TRUE);
		
		// 완전 연결 레이어 비활성화
		GetDlgItem(IDC_FULLY_INIT_METHOD)->EnableWindow(FALSE);
		m_sliderFullyAverage.EnableWindow(FALSE);
		GetDlgItem(IDC_FULLY_AVERAGE_EDIT)->EnableWindow(FALSE);
		m_sliderFullySigma.EnableWindow(FALSE);
		GetDlgItem(IDC_FULLY_SIGMA_EDIT)->EnableWindow(FALSE);
		m_sliderFullyOutput.EnableWindow(FALSE);
		GetDlgItem(IDC_FULLY_OUTPUT_EDIT)->EnableWindow(FALSE);
		m_sliderFullyBias.EnableWindow(FALSE);
		GetDlgItem(IDC_FULLY_BIAS_EDIT)->EnableWindow(FALSE);
		break;
	case 1:
		// 합성곱 레이어 비활성화
		GetDlgItem(IDC_INIT_METHOD)->EnableWindow(FALSE);
		m_sliderAverage.EnableWindow(FALSE);
		GetDlgItem(IDC_AVERAGE_EDIT)->EnableWindow(FALSE);
		m_sliderSigma.EnableWindow(FALSE);
		GetDlgItem(IDC_SIGMA_EDIT)->EnableWindow(FALSE);
		GetDlgItem(IDC_FILTER_SIZE)->EnableWindow(FALSE);
		m_sliderStride.EnableWindow(FALSE);
		GetDlgItem(IDC_STRIDE_EDIT)->EnableWindow(FALSE);
		GetDlgItem(IDC_PADDING)->EnableWindow(FALSE);
		m_sliderOutput.EnableWindow(FALSE);
		GetDlgItem(IDC_OUTPUT_EDIT)->EnableWindow(FALSE);
		m_sliderBias.EnableWindow(FALSE);
		GetDlgItem(IDC_BIASIS_EDIT)->EnableWindow(FALSE);

		// 완전 연결 레이어 활성화
		GetDlgItem(IDC_FULLY_INIT_METHOD)->EnableWindow(TRUE);
		m_sliderFullyAverage.EnableWindow(TRUE);
		GetDlgItem(IDC_FULLY_AVERAGE_EDIT)->EnableWindow(TRUE);
		m_sliderFullySigma.EnableWindow(TRUE);
		GetDlgItem(IDC_FULLY_SIGMA_EDIT)->EnableWindow(TRUE);
		m_sliderFullyOutput.EnableWindow(TRUE);
		GetDlgItem(IDC_FULLY_OUTPUT_EDIT)->EnableWindow(TRUE);
		m_sliderFullyBias.EnableWindow(TRUE);
		GetDlgItem(IDC_FULLY_BIAS_EDIT)->EnableWindow(TRUE);
		break;
	}
}


BOOL CHiddenPropertyDlg::OnCommand(WPARAM wParam, LPARAM lParam)
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.

	if (wParam == IDOK)
	{
		UpdateData(TRUE);
		propert->conv.initMethod = m_InitMethod;
		propert->conv.average = m_fAverage;
		propert->conv.sigma = m_fSigma;
		propert->conv.stride = m_nStride;
		propert->conv.padding = m_nPadding;
		propert->conv.output = m_nOutput;
		// propert->conv.filterSize = m_nFilterSize;
		if (m_nFilterSize == 0)
			propert->conv.filterSize = 3;
		else if (m_nFilterSize == 1)
			propert->conv.filterSize = 5;
		else if (m_nFilterSize == 2)
			propert->conv.filterSize = 7;

		propert->conv.bias = m_fBiasis;

		propert->fulCon.initMethod = m_nFullyInitMethod;
		propert->fulCon.average = m_fFullyAverage;
		propert->fulCon.bias = m_fFullyBias;
		propert->fulCon.output = m_nFullyOutput;
		propert->fulCon.sigma = m_fFullySigma;

		propert->activ.activationMethod = m_nActiveMethod;

		propert->pool.filterSize = m_nPoolFilterSize;
		propert->pool.padding = m_nPoolPadding;
		propert->pool.poolingMethod = m_nPoolInitMethod;
		propert->pool.stride = m_nPoolStride;

		propert->drop.proportion = m_fDropProportion;

		propert->convOrFully = m_nConvOrFully;

		if (m_checkPoolOnOff.GetCheck() == BST_CHECKED)
			propert->PoolingOnOff = true;
		else if (m_checkPoolOnOff.GetCheck() == BST_UNCHECKED)
			propert->PoolingOnOff = false;		

		propert->BatchOnOff = m_bBatchOnOff;
		propert->DropOnOff = m_bDropOnOff;
	}
	return CDialogEx::OnCommand(wParam, lParam);
}


void CHiddenPropertyDlg::OnBnClickedDropoutOnoff()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);
	if (m_bDropOnOff == true)
	{
		m_sliderDropProportion.EnableWindow(TRUE);
		GetDlgItem(IDC_DROP_PROPORTION_EDIT)->EnableWindow(TRUE);
	}
	else
	{
		m_sliderDropProportion.EnableWindow(FALSE);
		GetDlgItem(IDC_DROP_PROPORTION_EDIT)->EnableWindow(FALSE);
	}
}