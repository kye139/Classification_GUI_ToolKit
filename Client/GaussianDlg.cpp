// GaussianDlg.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "ImageTool.h"
#include "GaussianDlg.h"
#include "afxdialogex.h"


// CGaussianDlg ��ȭ �����Դϴ�.

IMPLEMENT_DYNAMIC(CGaussianDlg, CDialogEx)

CGaussianDlg::CGaussianDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_GAUSSIAN, pParent)
	, m_fSigma(1.40f)
{

}

CGaussianDlg::~CGaussianDlg()
{
}

void CGaussianDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_SIGMA_SLIDER, m_sliderSigma);
	DDX_Text(pDX, IDC_SIGMA_EDIT, m_fSigma);
	DDV_MinMaxFloat(pDX, m_fSigma, 0.20, 5.00);
}


BEGIN_MESSAGE_MAP(CGaussianDlg, CDialogEx)
	ON_EN_CHANGE(IDC_SIGMA_EDIT, &CGaussianDlg::OnEnChangeSigmaEdit)
	ON_WM_HSCROLL()
END_MESSAGE_MAP()


// CGaussianDlg �޽��� ó�����Դϴ�.


void CGaussianDlg::OnEnChangeSigmaEdit()
{
	// TODO:  RICHEDIT ��Ʈ���� ���, �� ��Ʈ����
	// CDialogEx::OnInitDialog() �Լ��� ������ 
	//�ϰ� ����ũ�� OR �����Ͽ� ������ ENM_CHANGE �÷��׸� �����Ͽ� CRichEditCtrl().SetEventMask()�� ȣ������ ������
	// �� �˸� �޽����� ������ �ʽ��ϴ�.

	// TODO:  ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	UpdateData(TRUE);
	m_sliderSigma.SetPos(static_cast<int>(m_fSigma * 50));
}


void CGaussianDlg::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰� ��/�Ǵ� �⺻���� ȣ���մϴ�.
	// �����̵� �ٿ��� �߻��� WM_HSCROLL �޽����� ��� ó��
	if (m_sliderSigma.GetSafeHwnd() == pScrollBar->GetSafeHwnd())
	{
		int pos = m_sliderSigma.GetPos();
		m_fSigma = (pos / 50.f);
		UpdateData(FALSE);
	}

	CDialogEx::OnHScroll(nSBCode, nPos, pScrollBar);
}


BOOL CGaussianDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  ���⿡ �߰� �ʱ�ȭ �۾��� �߰��մϴ�.
	m_sliderSigma.SetRange(10, 250);
	m_sliderSigma.SetTicFreq(20);
	m_sliderSigma.SetPageSize(20);
	m_sliderSigma.SetPos(static_cast<int>(m_fSigma * 50));

	return TRUE;  // return TRUE unless you set the focus to a control
				  // ����: OCX �Ӽ� �������� FALSE�� ��ȯ�ؾ� �մϴ�.
}
