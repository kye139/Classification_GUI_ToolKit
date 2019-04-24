#pragma once
#include "IppImage.h"

// �̹��� ���� ����
void IppInverse(IppImage* src, IppImage* dst);
// �̹��� ��� ��ȯ
void IppBrightness(IppImage* src, IppImage* dst, int n);
// �̹��� ��Ϻ� ��ȯ
void IppContrast(IppImage* src, IppImage* dst, int n);
// ������׷� ���ϱ�
void IppHistogram(IppImage* src, cv::Mat* histo);
// ������׷� ��Ȱȭ
void IppHistogramEqualization(IppImage* src, IppImage* dst);