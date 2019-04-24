#pragma once

#include <opencv2/opencv.hpp>
using namespace cv;

class IppImage
{
public:
	// �����ڿ� �Ҹ���
	IppImage();
	IppImage(const Mat& img);
	IppImage(const IppImage& img);
	IppImage(IppImage* img);
	~IppImage();

	// ���� �����
	BOOL Load(CString filename);
	BOOL Save(CString filename);

	// ȭ�� ���
	void Draw(HDC hdc, int dx = 0, int dy = 0);
	void Draw(HDC hdc, int dx, int dy, int dw, int dh, DWORD dwRop = SRCCOPY);
	void Draw(HDC hdc, int dx, int dy, int dw, int dh, int sx, int sy, int sw, int sh, DWORD dwRop = SRCCOPY);

	// Windows ȯ�濡�� Ŭ������� �����ϱ� �� �ٿ��ֱ�
	BOOL CopyToClipboard();
	BOOL PasteFromClipboard();

	// ���� �Լ�.
	Mat GetImage() const { return img; }
	bool IsValid() const { return(!img.empty()); }
	int GetWidth() const {	return img.cols; }
	int GetHeight() const { return img.rows; }

	// ���� �Լ�.
	void SetImage(cv::Mat inputImg);
private:
	Mat img; // ���� ó�� ������ ���� �̹��� Mat ��ü.
};

typedef struct ImageInform {
	wchar_t name[100];
	int uniqueId;
	IppImage* image;
}ImageInform;