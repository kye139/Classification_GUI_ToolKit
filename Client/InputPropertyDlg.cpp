// InputPropertyDlg.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "ImageTool.h"
#include "InputPropertyDlg.h"
#include "afxdialogex.h"


// CInputPropertyDlg ��ȭ �����Դϴ�.

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


// CInputPropertyDlg �޽��� ó�����Դϴ�.


BOOL CInputPropertyDlg::OnCommand(WPARAM wParam, LPARAM lParam)
{
	// TODO: ���⿡ Ư��ȭ�� �ڵ带 �߰� ��/�Ǵ� �⺻ Ŭ������ ȣ���մϴ�.
	if (wParam == IDOK)
	{
		UpdateData(TRUE); // �Է��� ũ��
	}

	return CDialogEx::OnCommand(wParam, lParam);
}
