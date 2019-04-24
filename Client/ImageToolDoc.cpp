
// ImageToolDoc.cpp : CImageToolDoc 클래스의 구현
//

#include "stdafx.h"
// SHARED_HANDLERS는 미리 보기, 축소판 그림 및 검색 필터 처리기를 구현하는 ATL 프로젝트에서 정의할 수 있으며
// 해당 프로젝트와 문서 코드를 공유하도록 해 줍니다.
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


// CImageToolDoc 생성/소멸

CImageToolDoc::CImageToolDoc()
{
	// TODO: 여기에 일회성 생성 코드를 추가합니다.

}

CImageToolDoc::~CImageToolDoc()
{
}

BOOL CImageToolDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: 여기에 재초기화 코드를 추가합니다.
	// SDI 문서는 이 문서를 다시 사용합니다.
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
		// TODO: 여기에 저장 코드를 추가합니다.
	}
	else
	{
		// TODO: 여기에 로딩 코드를 추가합니다.
	}
}

#ifdef SHARED_HANDLERS

// 축소판 그림을 지원합니다.
void CImageToolDoc::OnDrawThumbnail(CDC& dc, LPRECT lprcBounds)
{
	// 문서의 데이터를 그리려면 이 코드를 수정하십시오.
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

// 검색 처리기를 지원합니다.
void CImageToolDoc::InitializeSearchContent()
{
	CString strSearchContent;
	// 문서의 데이터에서 검색 콘텐츠를 설정합니다.
	// 콘텐츠 부분은 ";"로 구분되어야 합니다.

	// 예: strSearchContent = _T("point;rectangle;circle;ole object;");
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

// CImageToolDoc 진단

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


// CImageToolDoc 명령


BOOL CImageToolDoc::OnOpenDocument(LPCTSTR lpszPathName)
{
	if (!CDocument::OnOpenDocument(lpszPathName))
		return FALSE;

	// TODO:  여기에 특수화된 작성 코드를 추가합니다.
	IppImage* newImage = new IppImage();

	if (!newImage->Load(CString(lpszPathName)))
	{
		AfxMessageBox(CString("이미지 파일을 로드해주세요."));
		delete newImage;
		return FALSE;
	}

	// 이름 뽑아내기
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

	wcscpy_s(newImgInfo->name, (LPCTSTR)fileName); // 이름 저장
	newImgInfo->uniqueId = theApp._id++; // 식별자 저장
	newImgInfo->image = newImage;
	theApp._openName = fileName;

	// TreeView 클래스 지정.
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

	AfxPrintInfo(_T("[파일 열기] 파일 경로: %s, 가로 크기: %d픽셀, 세로 크기: %d픽셀"), lpszPathName, newImage->GetWidth(), newImage->GetHeight());

	m_Image = newImage;
	return TRUE;

}

BOOL CImageToolDoc::OnSaveDocument(LPCTSTR lpszPathName)
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.

	return m_Image->Save(CString(lpszPathName));
}


void CImageToolDoc::OnDirOpen()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	CMainFrame* mainFrm = (CMainFrame *)AfxGetMainWnd();
	mainFrm->OnDirLoad();
}


void CImageToolDoc::OnImageInverse()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	IppImage* newImage = new IppImage();

	IppInverse(m_Image, newImage);

	//AfxPrintInfo(_T("[반전] 입력 영상: %s", m_Image->);
	AfxNewImage(newImage);
}


void CImageToolDoc::OnBrightnessContrast()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
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
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	cv::Mat temp, src = m_Image->GetImage();

	cvtColor(src, temp, CV_BGR2GRAY);
	IppImage* newImage = new IppImage();
	newImage->SetImage(temp);

	AfxNewImage(newImage);
}


void CImageToolDoc::OnUpdateColorGrayscale(CCmdUI *pCmdUI)
{
	// TODO: 여기에 명령 업데이트 UI 처리기 코드를 추가합니다.
	pCmdUI->Enable(m_Image->GetImage().channels() == 3);
}


void CImageToolDoc::OnViewHistogram()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
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
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	IppImage* newImage = new IppImage();

	IppHistogramEqualization(m_Image, newImage);

	AfxNewImage(newImage);
}


void CImageToolDoc::OnUpdateHistoEqualization(CCmdUI *pCmdUI)
{
	// TODO: 여기에 명령 업데이트 UI 처리기 코드를 추가합니다.
	pCmdUI->Enable(m_Image->GetImage().channels() == 1);
}


void CImageToolDoc::OnFilterMean()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	IppImage* newImage = new IppImage();

	IppFilterMean(m_Image, newImage);

	AfxNewImage(newImage);
}


void CImageToolDoc::OnFilterMedian()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	IppImage* newImage = new IppImage();

	IppFilterMedian(m_Image, newImage);

	AfxNewImage(newImage);
}


void CImageToolDoc::OnFilterGaussian()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	CGaussianDlg dlg;
	if (dlg.DoModal() == IDOK)
	{
		IppImage* newImage = new IppImage();

		ippFilterGausian(m_Image, newImage, dlg.m_fSigma);

		AfxNewImage(newImage);
	}
}
