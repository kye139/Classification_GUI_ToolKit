
// ImageTool.h : ImageTool ���� ���α׷��� ���� �� ��� ����
//
#pragma once

#ifndef __AFXWIN_H__
	#error "PCH�� ���� �� ������ �����ϱ� ���� 'stdafx.h'�� �����մϴ�."
#endif

#include "resource.h"       // �� ��ȣ�Դϴ�.
#include "IppImage\IppImage.h"
#include "payload.h"


// CImageToolApp:
// �� Ŭ������ ������ ���ؼ��� ImageTool.cpp�� �����Ͻʽÿ�.
//

class CImageToolApp : public CWinAppEx
{
public:
	CImageToolApp();


// �������Դϴ�.
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

	IppImage* m_pIppImage; // Child WIndow ������ �ѱ� IppImage Ŭ����
	int _id;
	int _openId;
	CString _openName;

// �����Դϴ�.
	BOOL  m_bHiColorIcons;

	virtual void PreLoadState();
	virtual void LoadCustomState();
	virtual void SaveCustomState();

	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern CImageToolApp theApp;

// ���� �Լ� ����
void AfxNewImage(IppImage* img); // img�� ���� �����츦 ����
void AfxPrintInfo(CString message); // CStrng ���ڿ��� ��� �����쿡 ���
void AfxPrintInfo(LPCTSTR lpszFormat, ...); // ���� ���� ���ڿ� ������ ��� �����쿡 ���