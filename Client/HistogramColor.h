#pragma once
#include "C:\opencv\build\include\opencv2\core\mat.hpp"
#include "afxwin.h"


// CHistogramColor ��ȭ �����Դϴ�.

class CHistogramColor : public CDialogEx
{
	DECLARE_DYNAMIC(CHistogramColor)

public:
	CHistogramColor(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CHistogramColor();

// ��ȭ ���� �������Դϴ�.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_HISTOGRAM_COLOR };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()
public:
	cv::Mat BGR[3];
	void SetImage(IppImage* img);
	afx_msg void OnPaint();
	CStatic m_rStatic;
	virtual BOOL OnInitDialog();
	afx_msg void OnStnClickedRStatic();
};
