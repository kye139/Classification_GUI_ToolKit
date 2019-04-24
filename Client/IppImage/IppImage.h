#pragma once

#include <opencv2/opencv.hpp>
using namespace cv;

class IppImage
{
public:
	// 생성자와 소멸자
	IppImage();
	IppImage(const Mat& img);
	IppImage(const IppImage& img);
	IppImage(IppImage* img);
	~IppImage();

	// 파일 입출력
	BOOL Load(CString filename);
	BOOL Save(CString filename);

	// 화면 출력
	void Draw(HDC hdc, int dx = 0, int dy = 0);
	void Draw(HDC hdc, int dx, int dy, int dw, int dh, DWORD dwRop = SRCCOPY);
	void Draw(HDC hdc, int dx, int dy, int dw, int dh, int sx, int sy, int sw, int sh, DWORD dwRop = SRCCOPY);

	// Windows 환경에서 클립보드로 복사하기 및 붙여넣기
	BOOL CopyToClipboard();
	BOOL PasteFromClipboard();

	// 게터 함수.
	Mat GetImage() const { return img; }
	bool IsValid() const { return(!img.empty()); }
	int GetWidth() const {	return img.cols; }
	int GetHeight() const { return img.rows; }

	// 세터 함수.
	void SetImage(cv::Mat inputImg);
private:
	Mat img; // 영상 처리 연산을 위한 이미지 Mat 객체.
};

typedef struct ImageInform {
	wchar_t name[100];
	int uniqueId;
	IppImage* image;
}ImageInform;