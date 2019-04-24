
// ImageToolDoc.h : CImageToolDoc Ŭ������ �������̽�
//


#pragma once


class CImageToolDoc : public CDocument
{
protected: // serialization������ ��������ϴ�.
	CImageToolDoc();
	DECLARE_DYNCREATE(CImageToolDoc)

// Ư���Դϴ�.
public:

// �۾��Դϴ�.
public:

// �������Դϴ�.
public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
#ifdef SHARED_HANDLERS
	virtual void InitializeSearchContent();
	virtual void OnDrawThumbnail(CDC& dc, LPRECT lprcBounds);
#endif // SHARED_HANDLERS

// �����Դϴ�.
public:
	virtual ~CImageToolDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// ������ �޽��� �� �Լ�
protected:
	DECLARE_MESSAGE_MAP()

#ifdef SHARED_HANDLERS
	// �˻� ó���⿡ ���� �˻� �������� �����ϴ� ����� �Լ�
	void SetSearchContent(const CString& value);
#endif // SHARED_HANDLERS
public:
	virtual BOOL OnOpenDocument(LPCTSTR lpszPathName);
	virtual BOOL OnSaveDocument(LPCTSTR lpszPathName);
	IppImage* m_Image;
	afx_msg void OnDirOpen();
	afx_msg void OnImageInverse();
	afx_msg void OnBrightnessContrast();
	afx_msg void OnColorGrayscale();
	afx_msg void OnUpdateColorGrayscale(CCmdUI *pCmdUI);
	afx_msg void OnViewHistogram();
	afx_msg void OnHistoEqualization();
	afx_msg void OnUpdateHistoEqualization(CCmdUI *pCmdUI);
	afx_msg void OnFilterMean();
	afx_msg void OnFilterMedian();
	afx_msg void OnFilterGaussian();
};

typedef struct imageInfo {
	LPCTSTR pathName;
	int width;
	int height;
	int channels;
}ImageInfo;