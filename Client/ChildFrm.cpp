
// ChildFrm.cpp : CChildFrame Ŭ������ ����
//
#include <vector>
#include <string>
#include "stdafx.h"
#include "ImageTool.h"
#include "MainFrm.h"
#include "ChildFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CChildFrame

IMPLEMENT_DYNCREATE(CChildFrame, CMDIChildWndEx)

BEGIN_MESSAGE_MAP(CChildFrame, CMDIChildWndEx)
	ON_COMMAND(ID_DIR_LOAD, &CChildFrame::OnDirLoad)
	ON_COMMAND(ID_DIR_OPEN, &CChildFrame::OnDirOpen)
END_MESSAGE_MAP()

// CChildFrame ����/�Ҹ�

CChildFrame::CChildFrame()
{
	// TODO: ���⿡ ��� �ʱ�ȭ �ڵ带 �߰��մϴ�.
}

CChildFrame::~CChildFrame()
{
}


BOOL CChildFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: CREATESTRUCT cs�� �����Ͽ� ���⿡�� Window Ŭ���� �Ǵ� ��Ÿ���� �����մϴ�.
	if( !CMDIChildWndEx::PreCreateWindow(cs) )
		return FALSE;

	// Just clear the styles we don't want.

	cs.dwExStyle &= ~WS_EX_CLIENTEDGE;

	// �����Ͱ� �ٽ�
	// Check if our class is already defined

	// OnOpenDocument, OnNewDocument�� �����찡 �����Ǵ� ���.
	CString tempStr; // �ش� Child�� _id�� �ӽ÷� ���� CString ��ü
	LPCTSTR pszClassName; // ���� ����� ���� tempStr�� LPCTSTR������ ��ȯ
	int temp = theApp._openId;
	if (temp == -1)
	{
		tempStr.Format(_T("%d"), theApp._id);
		pszClassName = (LPCTSTR)tempStr;
	}
	else
	{
		tempStr.Format(_T("%d"), theApp._openId);
		pszClassName = (LPCTSTR)tempStr;
	}
	// LPCTSTR pszClassName = _T("aaa");
	WNDCLASS wndcls;

	if (!::GetClassInfo(AfxGetInstanceHandle(), pszClassName, &wndcls))
	{
		// Get the current requested window class
		VERIFY(GetClassInfo(AfxGetInstanceHandle(), cs.lpszClass, &wndcls));
		// We want to register this infos with our name
		wndcls.lpszClassName = pszClassName;

		// Need to preset the icon otherwise the function GetIconWndClass
		// calling us will overwrite our class.
		LPCTSTR pszIcon = MAKEINTRESOURCE(IDR_MAINFRAME);
		HINSTANCE hInst = AfxFindResourceHandle(pszIcon, ATL_RT_GROUP_ICON);
		_ASSERTE(hInst != NULL);
		wndcls.hIcon = ::LoadIcon(hInst, pszIcon);

		// Register our class now and check the outcome
		if (!::RegisterClass(&wndcls))
		{
			_ASSERTE(!__FUNCTION__ "RegisterClass failed");
			return FALSE;
		}
	}
	// Now use our class 
	//SetTitle(theApp._openName); //::SetWindowText(m_hWnd, "AutoFileUpdate");�� ��
	cs.lpszClass = pszClassName;
	//cs.style &= ~FWS_ADDTOTITLE; // window title set enabled
	//SetTitle(theApp._openName);
	return TRUE;
}

// CChildFrame ����

#ifdef _DEBUG
void CChildFrame::AssertValid() const
{
	CMDIChildWndEx::AssertValid();
}

void CChildFrame::Dump(CDumpContext& dc) const
{
	CMDIChildWndEx::Dump(dc);
}
#endif //_DEBUG

// CChildFrame �޽��� ó����

void CChildFrame::OnDirLoad()
{
}


void CChildFrame::OnDirOpen()
{

}
