#pragma once


// CHistorgramDlg ��ȭ �����Դϴ�.

class CHistorgramDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CHistorgramDlg)

public:
	CHistorgramDlg(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CHistorgramDlg();

// ��ȭ ���� �������Դϴ�.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_HISTOGRAM };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()
public:
	cv::Mat m_Histogram;
	afx_msg void OnPaint();
	void SetImage(IppImage* img);
};
