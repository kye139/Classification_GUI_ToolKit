// OutputPropertyDlg.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "ImageTool.h"
#include "OutputPropertyDlg.h"
#include "afxdialogex.h"


// COutputPropertyDlg ��ȭ �����Դϴ�.

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


// COutputPropertyDlg �޽��� ó�����Դϴ�.


BOOL COutputPropertyDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  ���⿡ �߰� �ʱ�ȭ �۾��� �߰��մϴ�.
	UpdateData(FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
				  // ����: OCX �Ӽ� �������� FALSE�� ��ȯ�ؾ� �մϴ�.
}


BOOL COutputPropertyDlg::OnCommand(WPARAM wParam, LPARAM lParam)
{
	// TODO: ���⿡ Ư��ȭ�� �ڵ带 �߰� ��/�Ǵ� �⺻ Ŭ������ ȣ���մϴ�.
	if (wParam == IDOK)
	{
		UpdateData(TRUE);
	}

	return CDialogEx::OnCommand(wParam, lParam);
}
