// OutputPropertyDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "ImageTool.h"
#include "OutputPropertyDlg.h"
#include "afxdialogex.h"


// COutputPropertyDlg 대화 상자입니다.

IMPLEMENT_DYNAMIC(COutputPropertyDlg, CDialogEx)

COutputPropertyDlg::COutputPropertyDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_OUTPUT_PROPERTY_DIALOG, pParent)
	, m_nClassNum(0)
	, m_nOneHotOnOff(0)
{

}

COutputPropertyDlg::COutputPropertyDlg(OutputLayerProperty outputProp, CWnd* pParent)
	: CDialogEx(IDD_OUTPUT_PROPERTY_DIALOG, pParent)
	, m_nClassNum(outputProp.numOfClassification)
	, m_nOneHotOnOff(outputProp.oneHotOnOff)
{
}

COutputPropertyDlg::~COutputPropertyDlg()
{
}

void COutputPropertyDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_CLASSIFICATION_NUMBER_EDIT, m_nClassNum);
	DDV_MinMaxInt(pDX, m_nClassNum, 1, 100);
	DDX_Check(pDX, IDC_ONE_HOT_CHECK, m_nOneHotOnOff);
	DDV_MinMaxInt(pDX, m_nOneHotOnOff, 0, 1);
}


BEGIN_MESSAGE_MAP(COutputPropertyDlg, CDialogEx)
END_MESSAGE_MAP()


// COutputPropertyDlg 메시지 처리기입니다.


BOOL COutputPropertyDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.
	UpdateData(FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}


BOOL COutputPropertyDlg::OnCommand(WPARAM wParam, LPARAM lParam)
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	if (wParam == IDOK)
	{
		UpdateData(TRUE);
	}

	return CDialogEx::OnCommand(wParam, lParam);
}
