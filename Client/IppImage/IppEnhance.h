#pragma once
#include "IppImage.h"

// 이미지 색상 반전
void IppInverse(IppImage* src, IppImage* dst);
// 이미지 밝기 변환
void IppBrightness(IppImage* src, IppImage* dst, int n);
// 이미지 명암비 변환
void IppContrast(IppImage* src, IppImage* dst, int n);
// 히스토그램 구하기
void IppHistogram(IppImage* src, cv::Mat* histo);
// 히스토그램 평활화
void IppHistogramEqualization(IppImage* src, IppImage* dst);