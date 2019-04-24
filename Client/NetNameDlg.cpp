// NetNameDlg.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "ImageTool.h"
#include "NetNameDlg.h"
#include "afxdialogex.h"
#include "MainFrm.h"


// CNetNameDlg ��ȭ �����Դϴ�.

IMPLEMENT_DYNAMIC(CNetNameDlg, CDialogEx)

CNetNameDlg::CNetNameDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_NET_NAME, pParent)
	, m_strNetName(_T(""))
{

}

CNetNameDlg::~CNetNameDlg()
{
}

void CNetNameDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_NET_NAME_EDIT, m_strNetName);
}


BEGIN_MESSAGE_MAP(CNetNameDlg, CDialogEx)
END_MESSAGE_MAP()


// CNetNameDlg �޽��� ó�����Դϴ�.


BOOL CNetNameDlg::OnCommand(WPARAM wParam, LPARAM lParam)
{
	// TODO: ���⿡ Ư��ȭ�� �ڵ带 �߰� ��/�Ǵ� �⺻ Ŭ������ ȣ���մϴ�.
	if (wParam == IDOK)
	{
		UpdateData(TRUE);

		int requestCode = 16;
		CMainFrame* frame = (CMainFrame *)AfxGetMainWnd();

		packet pack{ requestCode, 0, NULL };
		sendToServ(frame->hSocket, &pack);
		recvFromServ(frame->hSocket, &pack);

		if (pack.n_opcode == 16)
		{
			int netNum = *(int *)pack.cptr_payload;
			char* startPointer = pack.cptr_payload + sizeof(int);

			for (int i = 0; i < netNum; i++)
			{
				int netNameLen = *(int *)startPointer;
				char* name = (char *)malloc(netNameLen + 1);
				memcpy(name, startPointer + sizeof(int), netNameLen);
				name[netNameLen] = 0;

				// ���� �̸��� �Ű���� �����ϴ� ���
				if (!m_strNetName.Compare(CA2T(name)))
				{
					AfxMessageBox(_T("���� �̸��� �Ű���� �����մϴ�."));
					free(name);
					return false;
				}

				free(name);
				startPointer = startPointer + sizeof(int) + netNameLen;
			}
		}
	}

	return CDialogEx::OnCommand(wParam, lParam);
}
