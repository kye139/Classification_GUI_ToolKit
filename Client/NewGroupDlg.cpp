// NewGroupDlg.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "ImageTool.h"
#include "NewGroupDlg.h"
#include "afxdialogex.h"


// CNewGroupDlg ��ȭ �����Դϴ�.

IMPLEMENT_DYNAMIC(CNewGroupDlg, CDialogEx)

CNewGroupDlg::CNewGroupDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_NEW_GROUP_DLG, pParent)
	, m_nGroupName(_T("�� �׷�"))
{

}

CNewGroupDlg::~CNewGroupDlg()
{
}

void CNewGroupDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_NEW_GROUP_NAME, m_nGroupName);
}


BEGIN_MESSAGE_MAP(CNewGroupDlg, CDialogEx)
END_MESSAGE_MAP()


// CNewGroupDlg �޽��� ó�����Դϴ�.
