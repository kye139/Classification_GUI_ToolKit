
// ImageTool.h : ImageTool 응용 프로그램에 대한 주 헤더 파일
//
#pragma once

#ifndef __AFXWIN_H__
	#error "PCH에 대해 이 파일을 포함하기 전에 'stdafx.h'를 포함합니다."
#endif

#include "resource.h"       // 주 기호입니다.
#include "IppImage\IppImage.h"
#include "payload.h"


// CImageToolApp:
// 이 클래스의 구현에 대해서는 ImageTool.cpp을 참조하십시오.
//

class CImageToolApp : public CWinAppEx
{
public:
	CImageToolApp();


// 재정의입니다.
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

	IppImage* m_pIppImage; // Child WIndow 생성에 넘길 IppImage 클래스
	int _id;
	int _openId;
	CString _openName;

// 구현입니다.
	BOOL  m_bHiColorIcons;

	virtual void PreLoadState();
	virtual void LoadCustomState();
	virtual void SaveCustomState();

	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern CImageToolApp theApp;

// 전역 함수 선언
void AfxNewImage(IppImage* img); // img에 대한 윈도우를 생성
void AfxPrintInfo(CString message); // CStrng 문자열을 출력 윈도우에 출력
void AfxPrintInfo(LPCTSTR lpszFormat, ...); // 서식 문자 문자열 포맷을 출력 윈도우에 출력