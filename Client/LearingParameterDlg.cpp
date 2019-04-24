// LearingParameterDlg.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "ImageTool.h"
#include "LearingParameterDlg.h"
#include "afxdialogex.h"
#include "MainFrm.h"


// CLearingParameterDlg ��ȭ �����Դϴ�.

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


// CLearingParameterDlg �޽��� ó�����Դϴ�.


BOOL CLearingParameterDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  ���⿡ �߰� �ʱ�ȭ �۾��� �߰��մϴ�.
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
				  // ����: OCX �Ӽ� �������� FALSE�� ��ȯ�ؾ� �մϴ�.
}


BOOL CLearingParameterDlg::OnCommand(WPARAM wParam, LPARAM lParam)
{
	// TODO: ���⿡ Ư��ȭ�� �ڵ带 �߰� ��/�Ǵ� �⺻ Ŭ������ ȣ���մϴ�.
	if (wParam == IDOK)
	{
		UpdateData(TRUE);
		m_comboNetNames.GetWindowTextW(m_strNetName);
	}

	return CDialogEx::OnCommand(wParam, lParam);
}
