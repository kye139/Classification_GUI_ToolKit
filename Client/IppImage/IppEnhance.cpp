#include "stdafx.h"
#include "IppEnhance.h"

void IppInverse(IppImage* src, IppImage* dst)
{
	int ch = src->GetImage().channels();
	cv::Mat result;

	// 원본 이미지가 그레이스케일 이미지인 경우
	if (ch == 1)
	{
		result = 255 - src->GetImage();
	}
	// 원본 이미지가 컬러 이미지인 경우
	else if (ch == 3)
	{
		cv::Mat bgr[3];
		cv::split(src->GetImage(), bgr);

		for (int i = 0; i < 3; i++)
			bgr[i] = 255 - bgr[i];

		cv::merge(bgr, 3, result);
	}

	dst->SetImage(result);
}

void IppBrightness(IppImage* src, IppImage* dst, int n)
{
	int ch = src->GetImage().channels();
	cv::Mat result;

	// 원본 이미지가 그레이스케일 이미지인 경우
	if (ch == 1)
	{
		result = src->GetImage() + n;
	}
	// 원본 이미지가 컬러 이미지인 경우
	// HSV 컬러 이미지로 변경 후 Intensity를 변경 -> 이후 다시 RGB로 변경
	else if (ch == 3)
	{
		cv::Mat hsv, HSV[3];
		cvtColor(src->GetImage(), hsv, CV_BGR2HSV);
		split(hsv, HSV);
		HSV[2] += n;
		merge(HSV, 3, hsv);
		cvtColor(hsv, result, CV_HSV2BGR);
	}

	dst->SetImage(result);
}

void IppContrast(IppImage* src, IppImage* dst, int n)
{
	int ch = src->GetImage().channels();
	cv::Mat img, result;

	img = src->GetImage();

	// 원본 이미지가 그레이스케일 이미지인 경우
	if (ch == 1)
	{
		result = img + (img - 128) * n / 100;
	}
	// 원본 이미지가 컬러 이미지인 경우
	// HSV 컬러 이미지로 변경 후 Intensity를 변경 -> 이후 다시 RGB로 변경
	else if (ch == 3)
	{
		cv::Mat hsv, HSV[3];
		cvtColor(img, hsv, CV_BGR2HSV);
		split(hsv, HSV);
		
		HSV[2] = HSV[2] + (HSV[2] - 128) * n / 100;
		merge(HSV, 3, hsv);
		cvtColor(hsv, result, CV_HSV2BGR);
	}

	dst->SetImage(result);
}

void IppHistogram(IppImage* src, cv::Mat* histo)
{
	int ch = src->GetImage().channels();
	cv::Mat img = src->GetImage();
	int number_bins[1] = { 256 };
	float channel_range[] = { 0.0, 255.0 };
	const float* channel_ranges[] = { channel_range };

	// 원본 이미지가 그레이스케일 이미지인 경우
	if (ch == 1)
	{
		int channels[1] = { 0 };

		calcHist(&img, 1, channels, Mat(), *histo, 1, number_bins, channel_ranges);
	}
	// 원본 이미지가 컬러 이미지인 경우
	else if (ch == 3)
	{
		int channelsB[1] = { 0 };
		int channelsG[1] = { 1 };
		int channelsR[1] = { 2 };

		calcHist(&img, 1, channelsB, Mat(), histo[0], 1, number_bins, channel_ranges);
		calcHist(&img, 1, channelsG, Mat(), histo[1], 1, number_bins, channel_ranges);
		calcHist(&img, 1, channelsR, Mat(), histo[2], 1, number_bins, channel_ranges);
	}
}

void IppHistogramEqualization(IppImage* src, IppImage* dst)
{
	cv::Mat result;
	equalizeHist(src->GetImage(), result);

	dst->SetImage(result);
}