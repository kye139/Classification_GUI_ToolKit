// NewGroupDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "ImageTool.h"
#include "NewGroupDlg.h"
#include "afxdialogex.h"


// CNewGroupDlg 대화 상자입니다.

IMPLEMENT_DYNAMIC(CNewGroupDlg, CDialogEx)

CNewGroupDlg::CNewGroupDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_NEW_GROUP_DLG, pParent)
	, m_nGroupName(_T("새 그룹"))
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


// CNewGroupDlg 메시지 처리기입니다.
