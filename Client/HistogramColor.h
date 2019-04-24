#pragma once
#include "C:\opencv\build\include\opencv2\core\mat.hpp"
#include "afxwin.h"


// CHistogramColor 대화 상자입니다.

class CHistogramColor : public CDialogEx
{
	DECLARE_DYNAMIC(CHistogramColor)

public:
	CHistogramColor(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CHistogramColor();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_HISTOGRAM_COLOR };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	cv::Mat BGR[3];
	void SetImage(IppImage* img);
	afx_msg void OnPaint();
	CStatic m_rStatic;
	virtual BOOL OnInitDialog();
	afx_msg void OnStnClickedRStatic();
};
