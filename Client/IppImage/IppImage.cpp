#include "stdafx.h"
#include "./IppImage.h"
#pragma warning(disable : 4996)

using namespace cv;

// 생성자
IppImage::IppImage()
//	: img(Mat(300, 500, CV_8UC3, Scalar(255, 255, 255)))
{
}

IppImage::IppImage(const Mat& img)
{
	img.copyTo(this->img);
}

IppImage::IppImage(IppImage* image)
//	: img(image->img)
{
	image->img.copyTo(img);
}

IppImage::IppImage(const IppImage& image)
	: img(image.img)
{
}

IppImage::~IppImage()
{
	img.release();
}

BOOL IppImage::Load(CString filename)
{
	const char* pathName;
	pathName = (CStringA)filename;

	Mat image = imread(pathName, IMREAD_COLOR);

	if (image.empty())
	{
		return false;
	}

	image.copyTo(img);
	return true;
}

BOOL IppImage::Save(CString filename)
{
	// const char* pathName = (LPSTR)(LPCTSTR)filename;
	// char* pathName = new char[filename.GetLength() + 1];
	// strcpy(pathName, CT2A(filename));
	CT2CA pszConvertedAnsiString(filename);
	std::string s(pszConvertedAnsiString);

	if (imwrite(s, img))
	{
		return true;
	}
	else 
	{
		return false;
	}
}

void IppImage::Draw(HDC hdc, int dx, int dy)
{
	if (img.empty())
		return;

	int bpp = 8 * img.elemSize();
	// eleSize 즉 채널 수에 8을 곱해줌 : 8bit를 채널에 곱해줌 
	// 즉 bpp, bits per pixel, 1 픽셀당 비트 수, 여기선 총 24개가 될 것
	assert((bpp == 8 || bpp == 24 || bpp == 32));
	// 식의 참 거짓 여부를 판단해주는 assert, 만일 거짓이라면 에러 메세지박스를 띄운다.
	int padding = 0;
	//32 bit image is always DWORD aligned because each pixel requires 4 bytes
	// 아래는 32bit냐 아니냐에 따라 padding을 어떻게 결정할지 정해주는 듯
	// 아무래도 padding은 여백의 유무인 듯...?   
	if (bpp < 32)
		padding = 4 - (img.cols % 4);

	if (padding == 4)
		padding = 0;

	int border = 0;
	//32 bit image is always DWORD aligned because each pixel requires 4 bytes
	if (bpp < 32)
	{
		border = 4 - (img.cols % 4);
	}

	BITMAPINFO bitmapinfo;
	memset(&bitmapinfo, 0, sizeof(bitmapinfo));
	bitmapinfo.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	bitmapinfo.bmiHeader.biPlanes = 1;
	bitmapinfo.bmiHeader.biCompression = BI_RGB;
	bitmapinfo.bmiHeader.biWidth = img.cols;
	bitmapinfo.bmiHeader.biHeight = -img.rows;

	bitmapinfo.bmiHeader.biBitCount = bpp;

	SetDIBitsToDevice(hdc, dx, dy, img.cols, img.rows, 0, 0, 0, img.rows, img.data, &bitmapinfo, DIB_RGB_COLORS);
}

void IppImage::Draw(HDC hdc, int dx, int dy, int dw, int dh, DWORD dwRop)
{
	Draw(hdc, dx, dy, dw, dh, 0, 0, img.cols, img.rows, dwRop);
}

void IppImage::Draw(HDC hdc, int dx, int dy, int dw, int dh, int sx, int sy, int sw, int sh, DWORD dwRop)
{
	if (img.empty())
		return;

	int bpp = 8 * img.elemSize();
	// eleSize 즉 채널 수에 8을 곱해줌 : 8bit를 채널에 곱해줌 
	// 즉 bpp, bits per pixel, 1 픽셀당 비트 수, 여기선 총 24개가 될 것
	assert((bpp == 8 || bpp == 24 || bpp == 32));
	// 식의 참 거짓 여부를 판단해주는 assert, 만일 거짓이라면 에러 메세지박스를 띄운다.
	int padding = 0;
	//32 bit image is always DWORD aligned because each pixel requires 4 bytes
	// 아래는 32bit냐 아니냐에 따라 padding을 어떻게 결정할지 정해주는 듯
	// 아무래도 padding은 여백의 유무인 듯...?   
	if (bpp < 32)
		padding = 4 - (img.cols % 4);

	if (padding == 4)
		padding = 0;

	int border = 0;
	//32 bit image is always DWORD aligned because each pixel requires 4 bytes
	if (bpp < 32)
	{
		border = 4 - (img.cols % 4);
	}
	/*
	BITMAPINFO bitmapinfo;
	memset(&bitmapinfo, 0, sizeof(bitmapinfo));
	bitmapinfo.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	bitmapinfo.bmiHeader.biPlanes = 1;
	bitmapinfo.bmiHeader.biCompression = BI_RGB;
	bitmapinfo.bmiHeader.biWidth = img.cols;
	bitmapinfo.bmiHeader.biHeight = -img.rows;

	bitmapinfo.bmiHeader.biBitCount = bpp;
	*/
	BITMAPINFO* pbmi;

	if (img.channels() == 3)
	{
		pbmi = (BITMAPINFO*)malloc(sizeof(BITMAPINFOHEADER));
		pbmi->bmiHeader.biSize = sizeof(pbmi->bmiHeader);
		pbmi->bmiHeader.biPlanes = 1;
		pbmi->bmiHeader.biCompression = BI_RGB;
		pbmi->bmiHeader.biWidth = img.cols;
		pbmi->bmiHeader.biHeight = -img.rows;
		pbmi->bmiHeader.biBitCount = bpp;
	}
	else
	{
		pbmi = (BITMAPINFO*)malloc(sizeof(BITMAPINFOHEADER) + sizeof(RGBQUAD) * 256);
		pbmi->bmiHeader.biSize = sizeof(pbmi->bmiHeader);
		pbmi->bmiHeader.biPlanes = 1;
		pbmi->bmiHeader.biCompression = BI_RGB;
		pbmi->bmiHeader.biWidth = img.cols;
		pbmi->bmiHeader.biHeight = -img.rows;
		pbmi->bmiHeader.biBitCount = bpp;
		pbmi->bmiHeader.biClrUsed = 0;
		pbmi->bmiHeader.biClrImportant = 0;

		for (int i = 0; i<256; i++)
		{
			pbmi->bmiColors[i].rgbRed = i;
			pbmi->bmiColors[i].rgbGreen = i;
			pbmi->bmiColors[i].rgbBlue = i;
			pbmi->bmiColors[i].rgbReserved = 0;
		}
	}
	SetStretchBltMode(hdc, COLORONCOLOR);
	::StretchDIBits(hdc, dx, dy, dw, dh, sx, sy, sw, sh, img.data, pbmi, DIB_RGB_COLORS, SRCCOPY);
	free(pbmi);
}

void IppImage::SetImage(cv::Mat inputImg)
{
	img.release();
	inputImg.copyTo(img);
}