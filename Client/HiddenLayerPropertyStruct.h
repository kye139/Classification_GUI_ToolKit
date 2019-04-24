#pragma once

typedef struct {
	int initMethod;
	float average;
	float sigma;
	int filterSize;
	int stride;
	int padding;
	int output;
	float bias;
} ConvolutionProperty;

typedef struct {
	int initMethod;
	float average;
	float sigma;
	int output;
	float bias;
} FullyConnectedProperty;

typedef struct {
	int activationMethod;
} ActivationProperty;

typedef struct {
	int filterSize;
	int stride;
	int padding;
	int poolingMethod;
} PoolingProperty;

typedef struct {
	float proportion;
} DropoutProperty;

class HiddenLayerProperty {
public:
	ConvolutionProperty conv;
	FullyConnectedProperty fulCon;
	ActivationProperty activ;
	PoolingProperty pool;
	DropoutProperty drop;
	bool PoolingOnOff;
	int convOrFully;
	bool BatchOnOff;
	bool DropOnOff;

private:
	void ConvolutionInitialize(void)
	{
		conv.initMethod = 0;
		conv.average = 0.0f;
		conv.sigma = 0.1f;
		conv.filterSize = 0;
		conv.stride = 1;
		conv.padding = 0;
		conv.output = 16;
		conv.bias = 0.1f;
	}

	void FullyConnectedInitialize(void)
	{
		fulCon.initMethod = 0;
		fulCon.average = 0.0;
		fulCon.sigma = 0.1;
		fulCon.output = 256;
		fulCon.bias = 0.1;
	}
	
	void ActiviationInitialize(void)
	{
		activ.activationMethod = 0;
	}

	void PoolingInitialize(void)
	{
		pool.filterSize = 2;
		pool.stride = 2;
		pool.padding = 0;
		pool.poolingMethod = 0;
	}

	void DropoutIntialize(void)
	{
		drop.proportion = 0.5;
	}

public:
	void Initialize(void)
	{
		ConvolutionInitialize();
		FullyConnectedInitialize();
		ActiviationInitialize();
		PoolingInitialize();
		DropoutIntialize();
		PoolingOnOff = false; // 풀링 레이어 비활성화를 디폴트
		convOrFully = 0; // 합성곱 레이어 활성화가 디폴트
		BatchOnOff = false; // 배치 정규화 레이어 비활성화를 디폴트
		DropOnOff = false; // 드롭아웃 레이어 비활성화를 디폴트
	}
};