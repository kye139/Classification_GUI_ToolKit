// NetNameDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "ImageTool.h"
#include "NetNameDlg.h"
#include "afxdialogex.h"
#include "MainFrm.h"


// CNetNameDlg 대화 상자입니다.

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


// CNetNameDlg 메시지 처리기입니다.


BOOL CNetNameDlg::OnCommand(WPARAM wParam, LPARAM lParam)
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
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

				// 같은 이름의 신경망이 존재하는 경우
				if (!m_strNetName.Compare(CA2T(name)))
				{
					AfxMessageBox(_T("같은 이름의 신경망이 존재합니다."));
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
