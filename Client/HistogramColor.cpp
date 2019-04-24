// HistogramColor.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "ImageTool.h"
#include "HistogramColor.h"
#include "afxdialogex.h"
#include "IppImage\IppEnhance.h"


// CHistogramColor ��ȭ �����Դϴ�.

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


// CHistogramColor �޽��� ó�����Դϴ�.


void CHistogramColor::SetImage(IppImage* img)
{
	cv::Mat temp_bgr[3];
	IppHistogram(img, temp_bgr);
	
	// b ����ȭ
	normalize(temp_bgr[0], BGR[0], 0, 100, NORM_MINMAX, CV_32S);
	// g ����ȭ
	normalize(temp_bgr[1], BGR[1], 0, 100, NORM_MINMAX, CV_32S);
	// r ����ȭ
	normalize(temp_bgr[2], BGR[2], 0, 100, NORM_MINMAX, CV_32S);
}


void CHistogramColor::OnPaint()
{
	CPaintDC dc(this); // device context for painting
					   // TODO: ���⿡ �޽��� ó���� �ڵ带 �߰��մϴ�.
					   // �׸��� �޽����� ���ؼ��� CDialogEx::OnPaint()��(��) ȣ������ ���ʽÿ�.
	
	CGdiObject* pOldPen = dc.SelectStockObject(DC_PEN);

	// R ������׷� �ڽ�
	dc.SetDCPenColor(RGB(128, 128, 128));
	dc.MoveTo(20, 50);
	dc.LineTo(20, 150);
	dc.LineTo(275, 150);
	dc.LineTo(275, 50);

	// R�� �ش��ϴ� ������׷� ���
	dc.SetDCPenColor(RGB(255, 0, 0));
	for (int i = 0; i < 256; i++)
	{
		dc.MoveTo(20 + i, 150);
		dc.LineTo(20 + i, 150 - BGR[2].at<int>(Point(0, i)));
	}
	// R ���� ���
	for (int i = 0; i < 256; i++)
	{
		dc.SetDCPenColor(RGB(255, i, i));
		dc.MoveTo(20 + i, 160);
		dc.LineTo(20 + i, 175);
	}

	// G ������׷� �ڽ�
	dc.SetDCPenColor(RGB(128, 128, 128));
	dc.MoveTo(20, 235);
	dc.LineTo(20, 335);
	dc.LineTo(275, 335);
	dc.LineTo(275, 235);

	// G�� �ش��ϴ� ������׷� ���
	dc.SetDCPenColor(RGB(0, 255, 0));
	for (int i = 0; i < 256; i++)
	{
		dc.MoveTo(20 + i, 335);
		dc.LineTo(20 + i, 335 - BGR[1].at<int>(Point(0, i)));
	}
	// G ���� ���
	for (int i = 0; i < 256; i++)
	{
		dc.SetDCPenColor(RGB(i, 255, i));
		dc.MoveTo(20 + i, 345);
		dc.LineTo(20 + i, 360);
	}

	// B ������׷� �ڽ�
	dc.SetDCPenColor(RGB(128, 128, 128));
	dc.MoveTo(20, 420);
	dc.LineTo(20, 520);
	dc.LineTo(275, 520);
	dc.LineTo(275, 420);

	// B�� �ش��ϴ� ������׷� ���
	dc.SetDCPenColor(RGB(0, 0, 255));
	for (int i = 0; i < 256; i++)
	{
		dc.MoveTo(20 + i, 520);
		dc.LineTo(20 + i, 520 - BGR[1].at<int>(Point(0, i)));
	}
	// B ���� ���
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

	// TODO:  ���⿡ �߰� �ʱ�ȭ �۾��� �߰��մϴ�.
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
				  // ����: OCX �Ӽ� �������� FALSE�� ��ȯ�ؾ� �մϴ�.
}


void CHistogramColor::OnStnClickedRStatic()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
}
