// IdSelectDlg.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "ImageTool.h"
#include "IdSelectDlg.h"
#include "afxdialogex.h"


// CIdSelectDlg ��ȭ �����Դϴ�.

IMPLEMENT_DYNAMIC(CIdSelectDlg, CDialogEx)

CIdSelectDlg::CIdSelectDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_ID_SELECT, pParent), m_bCreateOrLogin(false)
	, m_sCreateId(_T(""))
{
	
}

CIdSelectDlg::~CIdSelectDlg()
{
}

void CIdSelectDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_ID_LIST, m_idList);
	DDX_Control(pDX, IDC_ID_LIST, m_idList);
	DDX_Text(pDX, IDC_CREATE_ID_EDIT, m_sCreateId);
}


BEGIN_MESSAGE_MAP(CIdSelectDlg, CDialogEx)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_ID_LIST, &CIdSelectDlg::OnLvnItemchangedIdList)
	ON_BN_CLICKED(IDC_CREATE_ID_BUTTON, &CIdSelectDlg::OnBnClickedCreateIdButton)
END_MESSAGE_MAP()


// CIdSelectDlg �޽��� ó�����Դϴ�.


BOOL CIdSelectDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  ���⿡ �߰� �ʱ�ȭ �۾��� �߰��մϴ�.

	// �� �� ���� ��� �߰�
	m_idList.SetExtendedStyle(m_idList.GetExtendedStyle() | LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);

	// �̸� ������ id���� ����Ʈ�� ���
	for (int i = 0; i < idListVec.size(); i++)
	{
		// m_idList.AddItem(CString(idListVec[i]), NULL, i);
		m_idList.InsertItem(i, CString(idListVec[i]));
	}

	return TRUE;  // return TRUE unless you set the focus to a control
				  // ����: OCX �Ӽ� �������� FALSE�� ��ȯ�ؾ� �մϴ�.
}


void CIdSelectDlg::OnLvnItemchangedIdList(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	int item = pNMLV->iItem; // idx of item that changed

	m_idStr = m_idList.GetItemText(item, 0);

	*pResult = 0;
}

void CIdSelectDlg::OnBnClickedCreateIdButton()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	UpdateData(TRUE);
	if (!m_sCreateId.Compare(_T("")))
	{
		MessageBox(_T("������ ������ �Է����ּ���"));
		return;
	}
	m_bCreateOrLogin = true;
	SendMessage(WM_COMMAND, IDOK, 0);
}
