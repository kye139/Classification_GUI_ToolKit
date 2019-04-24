
// ChildFrm.cpp : CChildFrame 클래스의 구현
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

// CChildFrame 생성/소멸

CChildFrame::CChildFrame()
{
	// TODO: 여기에 멤버 초기화 코드를 추가합니다.
}

CChildFrame::~CChildFrame()
{
}


BOOL CChildFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: CREATESTRUCT cs를 수정하여 여기에서 Window 클래스 또는 스타일을 수정합니다.
	if( !CMDIChildWndEx::PreCreateWindow(cs) )
		return FALSE;

	// Just clear the styles we don't want.

	cs.dwExStyle &= ~WS_EX_CLIENTEDGE;

	// 요기부터가 핵심
	// Check if our class is already defined

	// OnOpenDocument, OnNewDocument로 윈도우가 생성되는 경우.
	CString tempStr; // 해당 Child의 _id를 임시로 담을 CString 객체
	LPCTSTR pszClassName; // 실제 사용을 위해 tempStr을 LPCTSTR형으로 변환
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
	//SetTitle(theApp._openName); //::SetWindowText(m_hWnd, "AutoFileUpdate");도 됨
	cs.lpszClass = pszClassName;
	//cs.style &= ~FWS_ADDTOTITLE; // window title set enabled
	//SetTitle(theApp._openName);
	return TRUE;
}

// CChildFrame 진단

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

// CChildFrame 메시지 처리기

void CChildFrame::OnDirLoad()
{
}


void CChildFrame::OnDirOpen()
{

}
