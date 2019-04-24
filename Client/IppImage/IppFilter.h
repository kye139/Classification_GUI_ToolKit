#pragma once
#include "IppImage.h"

// ��� �� ���͸� (����)
void IppFilterMean(IppImage* src, IppImage* dst);
// ���� ��� �� ���͸�
void IppFilterMedian(IppImage* src, IppImage* dst);
// ����þ� ���͸�
void ippFilterGausian(IppImage* src, IppImage* dst, float sigma);
// ���̷��ͷ� ���͸�
void IppFilterBilateral(IppImage* src, IppImage* dst);