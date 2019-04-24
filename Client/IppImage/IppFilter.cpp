#include "stdafx.h"
#include "IppFilter.h"

// Convolution ÇÔ¼ö

void IppFilterMean(IppImage* src, IppImage* dst)
{
	cv::Mat result;
	blur(src->GetImage(), result, Size(5, 5));
	dst->SetImage(result);
}

void IppFilterMedian(IppImage* src, IppImage* dst)
{
	cv::Mat result;
	medianBlur(src->GetImage(), result, 5);
	dst->SetImage(result);
}

void ippFilterGausian(IppImage* src, IppImage* dst, float sigma)
{
	cv::Mat result;
	GaussianBlur(src->GetImage(), result, Size(9, 9), sigma, sigma);
	dst->SetImage(result);
}