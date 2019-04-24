// IdSelectDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "ImageTool.h"
#include "IdSelectDlg.h"
#include "afxdialogex.h"


// CIdSelectDlg 대화 상자입니다.

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


// CIdSelectDlg 메시지 처리기입니다.


BOOL CIdSelectDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.

	// 한 줄 선택 기능 추가
	m_idList.SetExtendedStyle(m_idList.GetExtendedStyle() | LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);

	// 이름 벡터의 id들을 리스트에 출력
	for (int i = 0; i < idListVec.size(); i++)
	{
		// m_idList.AddItem(CString(idListVec[i]), NULL, i);
		m_idList.InsertItem(i, CString(idListVec[i]));
	}

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}


void CIdSelectDlg::OnLvnItemchangedIdList(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	int item = pNMLV->iItem; // idx of item that changed

	m_idStr = m_idList.GetItemText(item, 0);

	*pResult = 0;
}

void CIdSelectDlg::OnBnClickedCreateIdButton()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);
	if (!m_sCreateId.Compare(_T("")))
	{
		MessageBox(_T("생성할 계정을 입력해주세요"));
		return;
	}
	m_bCreateOrLogin = true;
	SendMessage(WM_COMMAND, IDOK, 0);
}
