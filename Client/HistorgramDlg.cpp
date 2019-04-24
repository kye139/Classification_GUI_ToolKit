// HistorgramDlg.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "ImageTool.h"
#include "HistorgramDlg.h"
#include "afxdialogex.h"
#include "IppImage\IppEnhance.h"


// CHistorgramDlg ��ȭ �����Դϴ�.

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


// CHistorgramDlg �޽��� ó�����Դϴ�.


void CHistorgramDlg::OnPaint()
{
	CPaintDC dc(this); // device context for painting
					   // TODO: ���⿡ �޽��� ó���� �ڵ带 �߰��մϴ�.
					   // �׸��� �޽����� ���ؼ��� CDialogEx::OnPaint()��(��) ȣ������ ���ʽÿ�.

	CGdiObject* pOldPen = dc.SelectStockObject(DC_PEN);

	// ������׷� �ڽ�
	dc.SetDCPenColor(RGB(128, 128, 128));
	dc.MoveTo(20, 20);
	dc.LineTo(20, 120);
	dc.LineTo(275, 120);
	dc.LineTo(275, 20);

	// �� �׷��̽����Ͽ� �ش��ϴ� ������׷� ���
	dc.SetDCPenColor(RGB(0, 0, 0));
	for (int i = 0; i < 256; i++)
	{
		dc.MoveTo(20 + i, 120);
		dc.LineTo(20 + i, 120 - m_Histogram.at<int>(Point(0, i)));
	}
	// �׷��� ������ ���� ���
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
