
// ImageToolDoc.cpp : CImageToolDoc Ŭ������ ����
//

#include "stdafx.h"
// SHARED_HANDLERS�� �̸� ����, ����� �׸� �� �˻� ���� ó���⸦ �����ϴ� ATL ������Ʈ���� ������ �� ������
// �ش� ������Ʈ�� ���� �ڵ带 �����ϵ��� �� �ݴϴ�.
#ifndef SHARED_HANDLERS
#include "ImageTool.h"
#endif
#include "ImageToolDoc.h"
#include "MainFrm.h"

#include <propkey.h>

#include "IppImage\IppImage.h"
#include "IppImage\IppEnhance.h"
#include "IppImage\IppFilter.h"
#include "BrightnessContrastDlg.h"
#include "HistorgramDlg.h"
#include "HistogramColor.h"
#include "GaussianDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CImageToolDoc

IMPLEMENT_DYNCREATE(CImageToolDoc, CDocument)

BEGIN_MESSAGE_MAP(CImageToolDoc, CDocument)
	ON_COMMAND(ID_DIR_OPEN, &CImageToolDoc::OnDirOpen)
	ON_COMMAND(ID_DIR_OPEN, &CImageToolDoc::OnDirOpen)
	ON_COMMAND(ID_IMAGE_INVERSE, &CImageToolDoc::OnImageInverse)
	ON_COMMAND(ID_BRIGHTNESS_CONTRAST, &CImageToolDoc::OnBrightnessContrast)
	ON_COMMAND(ID_COLOR_GRAYSCALE, &CImageToolDoc::OnColorGrayscale)
	ON_UPDATE_COMMAND_UI(ID_COLOR_GRAYSCALE, &CImageToolDoc::OnUpdateColorGrayscale)
	ON_COMMAND(ID_VIEW_HISTOGRAM, &CImageToolDoc::OnViewHistogram)
	ON_COMMAND(ID_HISTO_EQUALIZATION, &CImageToolDoc::OnHistoEqualization)
	ON_UPDATE_COMMAND_UI(ID_HISTO_EQUALIZATION, &CImageToolDoc::OnUpdateHistoEqualization)
	ON_COMMAND(ID_FILTER_MEAN, &CImageToolDoc::OnFilterMean)
	ON_COMMAND(ID_FILTER_MEDIAN, &CImageToolDoc::OnFilterMedian)
	ON_COMMAND(ID_FILTER_GAUSSIAN, &CImageToolDoc::OnFilterGaussian)
END_MESSAGE_MAP()


// CImageToolDoc ����/�Ҹ�

CImageToolDoc::CImageToolDoc()
{
	// TODO: ���⿡ ��ȸ�� ���� �ڵ带 �߰��մϴ�.

}

CImageToolDoc::~CImageToolDoc()
{
}

BOOL CImageToolDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: ���⿡ ���ʱ�ȭ �ڵ带 �߰��մϴ�.
	// SDI ������ �� ������ �ٽ� ����մϴ�.
	if (theApp.m_pIppImage != NULL)
	{
		m_Image = theApp.m_pIppImage;
		theApp.m_pIppImage = NULL;


		return TRUE;
	}

	return FALSE;
}




// CImageToolDoc serialization

void CImageToolDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: ���⿡ ���� �ڵ带 �߰��մϴ�.
	}
	else
	{
		// TODO: ���⿡ �ε� �ڵ带 �߰��մϴ�.
	}
}

#ifdef SHARED_HANDLERS

// ����� �׸��� �����մϴ�.
void CImageToolDoc::OnDrawThumbnail(CDC& dc, LPRECT lprcBounds)
{
	// ������ �����͸� �׸����� �� �ڵ带 �����Ͻʽÿ�.
	dc.FillSolidRect(lprcBounds, RGB(255, 255, 255));

	CString strText = _T("TODO: implement thumbnail drawing here");
	LOGFONT lf;

	CFont* pDefaultGUIFont = CFont::FromHandle((HFONT) GetStockObject(DEFAULT_GUI_FONT));
	pDefaultGUIFont->GetLogFont(&lf);
	lf.lfHeight = 36;

	CFont fontDraw;
	fontDraw.CreateFontIndirect(&lf);

	CFont* pOldFont = dc.SelectObject(&fontDraw);
	dc.DrawText(strText, lprcBounds, DT_CENTER | DT_WORDBREAK);
	dc.SelectObject(pOldFont);
}

// �˻� ó���⸦ �����մϴ�.
void CImageToolDoc::InitializeSearchContent()
{
	CString strSearchContent;
	// ������ �����Ϳ��� �˻� �������� �����մϴ�.
	// ������ �κ��� ";"�� ���еǾ�� �մϴ�.

	// ��: strSearchContent = _T("point;rectangle;circle;ole object;");
	SetSearchContent(strSearchContent);
}

void CImageToolDoc::SetSearchContent(const CString& value)
{
	if (value.IsEmpty())
	{
		RemoveChunk(PKEY_Search_Contents.fmtid, PKEY_Search_Contents.pid);
	}
	else
	{
		CMFCFilterChunkValueImpl *pChunk = NULL;
		ATLTRY(pChunk = new CMFCFilterChunkValueImpl);
		if (pChunk != NULL)
		{
			pChunk->SetTextValue(PKEY_Search_Contents, value, CHUNK_TEXT);
			SetChunkValue(pChunk);
		}
	}
}

#endif // SHARED_HANDLERS

// CImageToolDoc ����

#ifdef _DEBUG
void CImageToolDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CImageToolDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// CImageToolDoc ���


BOOL CImageToolDoc::OnOpenDocument(LPCTSTR lpszPathName)
{
	if (!CDocument::OnOpenDocument(lpszPathName))
		return FALSE;

	// TODO:  ���⿡ Ư��ȭ�� �ۼ� �ڵ带 �߰��մϴ�.
	IppImage* newImage = new IppImage();

	if (!newImage->Load(CString(lpszPathName)))
	{
		AfxMessageBox(CString("�̹��� ������ �ε����ּ���."));
		delete newImage;
		return FALSE;
	}

	// �̸� �̾Ƴ���
	CString str(lpszPathName);
	CString fileName, nextName;
	int curPos = 0;

	nextName = str.Tokenize(_T("\\"), curPos);

	while (nextName != "")
	{
		fileName = nextName;
		nextName = str.Tokenize(_T("\\"), curPos);
	};
	ImageInform* newImgInfo = new ImageInform;

	wcscpy_s(newImgInfo->name, (LPCTSTR)fileName); // �̸� ����
	newImgInfo->uniqueId = theApp._id++; // �ĺ��� ����
	newImgInfo->image = newImage;
	theApp._openName = fileName;

	// TreeView Ŭ���� ����.
	CMainFrame* pFrame = (CMainFrame *)AfxGetMainWnd();
	CViewTree* fileView = &(pFrame->m_wndFileView.m_wndFileView);
	HTREEITEM hRoot = fileView->GetFirstVisibleItem();

	if (fileView->ItemHasChildren(hRoot))
	{
		HTREEITEM hImageItem = fileView->GetChildItem(hRoot);
		HTREEITEM newImgItem = fileView->InsertItem(fileName, 2, 2, hImageItem);
		fileView->SetItemData(newImgItem, (DWORD_PTR)newImgInfo);
		fileView->Expand(hImageItem, TVE_EXPAND);
	}

	AfxPrintInfo(_T("[���� ����] ���� ���: %s, ���� ũ��: %d�ȼ�, ���� ũ��: %d�ȼ�"), lpszPathName, newImage->GetWidth(), newImage->GetHeight());

	m_Image = newImage;
	return TRUE;

}

BOOL CImageToolDoc::OnSaveDocument(LPCTSTR lpszPathName)
{
	// TODO: ���⿡ Ư��ȭ�� �ڵ带 �߰� ��/�Ǵ� �⺻ Ŭ������ ȣ���մϴ�.

	return m_Image->Save(CString(lpszPathName));
}


void CImageToolDoc::OnDirOpen()
{
	// TODO: ���⿡ ��� ó���� �ڵ带 �߰��մϴ�.
	CMainFrame* mainFrm = (CMainFrame *)AfxGetMainWnd();
	mainFrm->OnDirLoad();
}


void CImageToolDoc::OnImageInverse()
{
	// TODO: ���⿡ ��� ó���� �ڵ带 �߰��մϴ�.
	IppImage* newImage = new IppImage();

	IppInverse(m_Image, newImage);

	//AfxPrintInfo(_T("[����] �Է� ����: %s", m_Image->);
	AfxNewImage(newImage);
}


void CImageToolDoc::OnBrightnessContrast()
{
	// TODO: ���⿡ ��� ó���� �ڵ带 �߰��մϴ�.
	CBrightnessContrastDlg dlg;
	if (dlg.DoModal() == IDOK)
	{
		IppImage* newImage = new IppImage();

		IppBrightness(m_Image, newImage, dlg.m_nBrightness);
		IppContrast(newImage, newImage, dlg.m_nContrast);

		AfxNewImage(newImage);
	}
}


void CImageToolDoc::OnColorGrayscale()
{
	// TODO: ���⿡ ��� ó���� �ڵ带 �߰��մϴ�.
	cv::Mat temp, src = m_Image->GetImage();

	cvtColor(src, temp, CV_BGR2GRAY);
	IppImage* newImage = new IppImage();
	newImage->SetImage(temp);

	AfxNewImage(newImage);
}


void CImageToolDoc::OnUpdateColorGrayscale(CCmdUI *pCmdUI)
{
	// TODO: ���⿡ ��� ������Ʈ UI ó���� �ڵ带 �߰��մϴ�.
	pCmdUI->Enable(m_Image->GetImage().channels() == 3);
}


void CImageToolDoc::OnViewHistogram()
{
	// TODO: ���⿡ ��� ó���� �ڵ带 �߰��մϴ�.
	if (m_Image->GetImage().channels() == 1)
	{
		CHistorgramDlg dlg;
		dlg.SetImage(m_Image);
		dlg.DoModal();
	}
	else
	{
		CHistogramColor dlg;
		dlg.SetImage(m_Image);
		dlg.DoModal();
	}
}


void CImageToolDoc::OnHistoEqualization()
{
	// TODO: ���⿡ ��� ó���� �ڵ带 �߰��մϴ�.
	IppImage* newImage = new IppImage();

	IppHistogramEqualization(m_Image, newImage);

	AfxNewImage(newImage);
}


void CImageToolDoc::OnUpdateHistoEqualization(CCmdUI *pCmdUI)
{
	// TODO: ���⿡ ��� ������Ʈ UI ó���� �ڵ带 �߰��մϴ�.
	pCmdUI->Enable(m_Image->GetImage().channels() == 1);
}


void CImageToolDoc::OnFilterMean()
{
	// TODO: ���⿡ ��� ó���� �ڵ带 �߰��մϴ�.
	IppImage* newImage = new IppImage();

	IppFilterMean(m_Image, newImage);

	AfxNewImage(newImage);
}


void CImageToolDoc::OnFilterMedian()
{
	// TODO: ���⿡ ��� ó���� �ڵ带 �߰��մϴ�.
	IppImage* newImage = new IppImage();

	IppFilterMedian(m_Image, newImage);

	AfxNewImage(newImage);
}


void CImageToolDoc::OnFilterGaussian()
{
	// TODO: ���⿡ ��� ó���� �ڵ带 �߰��մϴ�.
	CGaussianDlg dlg;
	if (dlg.DoModal() == IDOK)
	{
		IppImage* newImage = new IppImage();

		ippFilterGausian(m_Image, newImage, dlg.m_fSigma);

		AfxNewImage(newImage);
	}
}
