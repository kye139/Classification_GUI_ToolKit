// ServerIPDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "ImageTool.h"
#include "ServerIPDlg.h"
#include "afxdialogex.h"
#pragma warning(disable : 4996)


// CServerIPDlg 대화 상자입니다.

IMPLEMENT_DYNAMIC(CServerIPDlg, CDialogEx)

CServerIPDlg::CServerIPDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DIALOG3, pParent)
{

}

CServerIPDlg::~CServerIPDlg()
{
}

void CServerIPDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_IPADDRESS1, m_IpAddress);
}


BEGIN_MESSAGE_MAP(CServerIPDlg, CDialogEx)
	ON_BN_CLICKED(IDC_CONNECT_CHECK, &CServerIPDlg::OnBnClickedConnectCheck)
END_MESSAGE_MAP()


// CServerIPDlg 메시지 처리기입니다.


void CServerIPDlg::OnBnClickedConnectCheck()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	m_IpAddress.GetAddress(ipFirst, ipSecond, ipThird, ipForth);
	
	char addr[30];
	sprintf_s(addr, "%d.%d.%d.%d", ipFirst, ipSecond, ipThird, ipForth);

	hSocket = createSocket(addr, 10000);

	int requestCode = 10;

	packet pack{ requestCode, 0, NULL };
	sendToServ(hSocket, &pack);
	recvFromServ(hSocket, &pack);

	if (pack.n_opcode == 10)
	{
		int numOfDir = *(int *)pack.cptr_payload;
		char* idListPtr = pack.cptr_payload + sizeof(int);
		for (int i = 0; i < numOfDir; i++)
		{
			int len = *(int *)idListPtr;
			char* dirName = (char *)malloc(len + 1);
			memcpy(dirName, idListPtr + sizeof(int), len);
			dirName[len] = '\0';
			dirList.push_back(dirName);
			idListPtr = idListPtr + sizeof(int) + len;
		}
		
	}

	SendMessage(WM_COMMAND, IDOK, 0);
}
