// LearingParameterDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "ImageTool.h"
#include "LearingParameterDlg.h"
#include "afxdialogex.h"
#include "MainFrm.h"


// CLearingParameterDlg 대화 상자입니다.

IMPLEMENT_DYNAMIC(CLearingParameterDlg, CDialogEx)

CLearingParameterDlg::CLearingParameterDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_LEARNING_PROPERTY_DIALOG, pParent)
	, m_nLoopCount(1)
	, m_nTestPeriod(1)
{

}

CLearingParameterDlg::~CLearingParameterDlg()
{
}

void CLearingParameterDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO2, m_comboNetNames);
	DDX_Text(pDX, IDC_EDIT1, m_nLoopCount);
	DDV_MinMaxInt(pDX, m_nLoopCount, 100, 100000);
	DDX_Text(pDX, IDC_EDIT15, m_nTestPeriod);
	DDV_MinMaxInt(pDX, m_nTestPeriod, 10, 100);
	DDX_Control(pDX, IDC_SLIDER1, m_sliderLoopCount);
	DDX_Control(pDX, IDC_SLIDER2, m_sliderTestPeriod);
}


BEGIN_MESSAGE_MAP(CLearingParameterDlg, CDialogEx)
END_MESSAGE_MAP()


// CLearingParameterDlg 메시지 처리기입니다.


BOOL CLearingParameterDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.
	CMainFrame* frame = (CMainFrame *)AfxGetMainWnd();
	HTREEITEM netPointer = frame->m_wndNetView.m_wndNetView.GetChildItem(frame->m_wndNetView.m_wndNetView.GetRootItem());

	while (netPointer)
	{
		CString netName = frame->m_wndNetView.m_wndNetView.GetItemText(netPointer);
		m_comboNetNames.AddString(netName);
		frame->m_wndNetView.m_wndNetView.GetNextItem(netPointer, TVGN_NEXT);
	}

	m_sliderLoopCount.SetRange(100, 100000);
	m_sliderLoopCount.SetTicFreq(11000);
	m_sliderLoopCount.SetPageSize(11000);
	m_sliderLoopCount.SetPos(m_nLoopCount);

	m_sliderTestPeriod.SetRange(10, 100);
	m_sliderTestPeriod.SetTicFreq(10);
	m_sliderTestPeriod.SetPageSize(10);
	m_sliderTestPeriod.SetPos(m_nTestPeriod);

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}


BOOL CLearingParameterDlg::OnCommand(WPARAM wParam, LPARAM lParam)
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	if (wParam == IDOK)
	{
		UpdateData(TRUE);
		m_comboNetNames.GetWindowTextW(m_strNetName);
	}

	return CDialogEx::OnCommand(wParam, lParam);
}
