#pragma once
#include "IppImage.h"

// 평균 값 필터링 (블러링)
void IppFilterMean(IppImage* src, IppImage* dst);
// 가중 평균 값 필터링
void IppFilterMedian(IppImage* src, IppImage* dst);
// 가우시안 필터링
void ippFilterGausian(IppImage* src, IppImage* dst, float sigma);
// 바이레터럴 필터링
void IppFilterBilateral(IppImage* src, IppImage* dst);