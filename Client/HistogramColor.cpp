// HistogramColor.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "ImageTool.h"
#include "HistogramColor.h"
#include "afxdialogex.h"
#include "IppImage\IppEnhance.h"


// CHistogramColor 대화 상자입니다.

IMPLEMENT_DYNAMIC(CHistogramColor, CDialogEx)

CHistogramColor::CHistogramColor(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_HISTOGRAM_COLOR, pParent)
{
}

CHistogramColor::~CHistogramColor()
{
}

void CHistogramColor::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_R_STATIC, m_rStatic);
}


BEGIN_MESSAGE_MAP(CHistogramColor, CDialogEx)
	ON_WM_PAINT()
	ON_STN_CLICKED(IDC_R_STATIC, &CHistogramColor::OnStnClickedRStatic)
END_MESSAGE_MAP()


// CHistogramColor 메시지 처리기입니다.


void CHistogramColor::SetImage(IppImage* img)
{
	cv::Mat temp_bgr[3];
	IppHistogram(img, temp_bgr);
	
	// b 정규화
	normalize(temp_bgr[0], BGR[0], 0, 100, NORM_MINMAX, CV_32S);
	// g 정규화
	normalize(temp_bgr[1], BGR[1], 0, 100, NORM_MINMAX, CV_32S);
	// r 정규화
	normalize(temp_bgr[2], BGR[2], 0, 100, NORM_MINMAX, CV_32S);
}


void CHistogramColor::OnPaint()
{
	CPaintDC dc(this); // device context for painting
					   // TODO: 여기에 메시지 처리기 코드를 추가합니다.
					   // 그리기 메시지에 대해서는 CDialogEx::OnPaint()을(를) 호출하지 마십시오.
	
	CGdiObject* pOldPen = dc.SelectStockObject(DC_PEN);

	// R 히스토그램 박스
	dc.SetDCPenColor(RGB(128, 128, 128));
	dc.MoveTo(20, 50);
	dc.LineTo(20, 150);
	dc.LineTo(275, 150);
	dc.LineTo(275, 50);

	// R에 해당하는 히스토그램 출력
	dc.SetDCPenColor(RGB(255, 0, 0));
	for (int i = 0; i < 256; i++)
	{
		dc.MoveTo(20 + i, 150);
		dc.LineTo(20 + i, 150 - BGR[2].at<int>(Point(0, i)));
	}
	// R 레벨 출력
	for (int i = 0; i < 256; i++)
	{
		dc.SetDCPenColor(RGB(255, i, i));
		dc.MoveTo(20 + i, 160);
		dc.LineTo(20 + i, 175);
	}

	// G 히스토그램 박스
	dc.SetDCPenColor(RGB(128, 128, 128));
	dc.MoveTo(20, 235);
	dc.LineTo(20, 335);
	dc.LineTo(275, 335);
	dc.LineTo(275, 235);

	// G에 해당하는 히스토그램 출력
	dc.SetDCPenColor(RGB(0, 255, 0));
	for (int i = 0; i < 256; i++)
	{
		dc.MoveTo(20 + i, 335);
		dc.LineTo(20 + i, 335 - BGR[1].at<int>(Point(0, i)));
	}
	// G 레벨 출력
	for (int i = 0; i < 256; i++)
	{
		dc.SetDCPenColor(RGB(i, 255, i));
		dc.MoveTo(20 + i, 345);
		dc.LineTo(20 + i, 360);
	}

	// B 히스토그램 박스
	dc.SetDCPenColor(RGB(128, 128, 128));
	dc.MoveTo(20, 420);
	dc.LineTo(20, 520);
	dc.LineTo(275, 520);
	dc.LineTo(275, 420);

	// B에 해당하는 히스토그램 출력
	dc.SetDCPenColor(RGB(0, 0, 255));
	for (int i = 0; i < 256; i++)
	{
		dc.MoveTo(20 + i, 520);
		dc.LineTo(20 + i, 520 - BGR[1].at<int>(Point(0, i)));
	}
	// B 레벨 출력
	for (int i = 0; i < 256; i++)
	{
		dc.SetDCPenColor(RGB(i, i, 255));
		dc.MoveTo(20 + i, 530);
		dc.LineTo(20 + i, 545);
	}
}


BOOL CHistogramColor::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.
	CFont fnt;
	LOGFONT lf;
	::ZeroMemory(&lf, sizeof(lf));
	lf.lfHeight = 18;
	lf.lfWeight = FW_BOLD;
	::lstrcpy(lf.lfFaceName, (LPCWSTR)"Tahoma");
	fnt.CreateFontIndirect(&lf);
	GetDlgItem(IDC_R_STATIC)->SetFont(&fnt);

	::ZeroMemory(&lf, sizeof(lf));
	lf.lfHeight = 18;
	lf.lfWeight = FW_BOLD;
	::lstrcpy(lf.lfFaceName, (LPCWSTR)"Tahoma");
	fnt.CreateFontIndirect(&lf);
	GetDlgItem(IDC_G_STATIC)->SetFont(&fnt);

	::ZeroMemory(&lf, sizeof(lf));
	lf.lfHeight = 18;
	lf.lfWeight = FW_BOLD;
	::lstrcpy(lf.lfFaceName, (LPCWSTR)"Tahoma");
	fnt.CreateFontIndirect(&lf);
	GetDlgItem(IDC_B_STATIC)->SetFont(&fnt);

	fnt.Detach();
	return TRUE;  // return TRUE unless you set the focus to a control
				  // 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}


void CHistogramColor::OnStnClickedRStatic()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}
