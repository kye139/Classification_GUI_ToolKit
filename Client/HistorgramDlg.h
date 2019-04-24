#pragma once


// CHistorgramDlg 대화 상자입니다.

class CHistorgramDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CHistorgramDlg)

public:
	CHistorgramDlg(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CHistorgramDlg();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_HISTOGRAM };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	cv::Mat m_Histogram;
	afx_msg void OnPaint();
	void SetImage(IppImage* img);
};
