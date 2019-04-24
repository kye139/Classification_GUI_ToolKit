// HistorgramDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "ImageTool.h"
#include "HistorgramDlg.h"
#include "afxdialogex.h"
#include "IppImage\IppEnhance.h"


// CHistorgramDlg 대화 상자입니다.

IMPLEMENT_DYNAMIC(CHistorgramDlg, CDialogEx)

CHistorgramDlg::CHistorgramDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_HISTOGRAM, pParent)
{
}

CHistorgramDlg::~CHistorgramDlg()
{
}

void CHistorgramDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CHistorgramDlg, CDialogEx)
	ON_WM_PAINT()
END_MESSAGE_MAP()


// CHistorgramDlg 메시지 처리기입니다.


void CHistorgramDlg::OnPaint()
{
	CPaintDC dc(this); // device context for painting
					   // TODO: 여기에 메시지 처리기 코드를 추가합니다.
					   // 그리기 메시지에 대해서는 CDialogEx::OnPaint()을(를) 호출하지 마십시오.

	CGdiObject* pOldPen = dc.SelectStockObject(DC_PEN);

	// 히스토그램 박스
	dc.SetDCPenColor(RGB(128, 128, 128));
	dc.MoveTo(20, 20);
	dc.LineTo(20, 120);
	dc.LineTo(275, 120);
	dc.LineTo(275, 20);

	// 각 그레이스케일에 해당하는 히스토그램 출력
	dc.SetDCPenColor(RGB(0, 0, 0));
	for (int i = 0; i < 256; i++)
	{
		dc.MoveTo(20 + i, 120);
		dc.LineTo(20 + i, 120 - m_Histogram.at<int>(Point(0, i)));
	}
	// 그레이 스케일 레벨 출력
	for (int i = 0; i < 256; i++)
	{
		dc.SetDCPenColor(RGB(i, i, i));
		dc.MoveTo(20 + i, 130);
		dc.LineTo(20 + i, 145);
	}

	dc.SelectObject(pOldPen);
}

void CHistorgramDlg::SetImage(IppImage* img)
{
	cv::Mat histo;
	IppHistogram(img, &histo);
	normalize(histo, m_Histogram, 0, 100, NORM_MINMAX, CV_32S);
}
