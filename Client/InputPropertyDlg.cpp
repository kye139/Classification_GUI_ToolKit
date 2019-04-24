// InputPropertyDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "ImageTool.h"
#include "InputPropertyDlg.h"
#include "afxdialogex.h"


// CInputPropertyDlg 대화 상자입니다.

IMPLEMENT_DYNAMIC(CInputPropertyDlg, CDialogEx)

CInputPropertyDlg::CInputPropertyDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_INPUT_PROPERTY_DIALOG, pParent)
	, m_nWidth(0)
	, m_nHeight(0)
{

}

CInputPropertyDlg::CInputPropertyDlg(InputLayerProperty inputProp, CWnd* pParent)
	: CDialogEx(IDD_INPUT_PROPERTY_DIALOG, pParent)
	, m_nWidth(inputProp.width)
	, m_nHeight(inputProp.height)
{
}

CInputPropertyDlg::~CInputPropertyDlg()
{
}

void CInputPropertyDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_WIDTH_EDIT, m_nWidth);
	DDV_MinMaxInt(pDX, m_nWidth, 1, 10000);
	DDX_Text(pDX, IDC_HEIGHT_EDIT, m_nHeight);
	DDV_MinMaxInt(pDX, m_nHeight, 1, 10000);
}


BEGIN_MESSAGE_MAP(CInputPropertyDlg, CDialogEx)
END_MESSAGE_MAP()


// CInputPropertyDlg 메시지 처리기입니다.


BOOL CInputPropertyDlg::OnCommand(WPARAM wParam, LPARAM lParam)
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	if (wParam == IDOK)
	{
		UpdateData(TRUE); // 입력의 크기
	}

	return CDialogEx::OnCommand(wParam, lParam);
}
