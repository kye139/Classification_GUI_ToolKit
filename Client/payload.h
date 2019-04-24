
#ifndef __PAYLOAD_H__
#pragma once
#pragma comment(lib,"ws2_32.lib")
#pragma warning(disable : 4996)

#define __PAYLOAD_H__
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
#include<iostream>
#include<cstdio>
#include<stdlib.h>
#include<string.h>
#include <WinSock2.h>

#define BUFSIZE 1024*1024
//네트워크 전송을 하기 위해서 만들었습니다.

#define OPCODE_CONNECTION 0  //최초연결시
#define OPCODE_CREATECLF 1   //분류기 구성시
#define OPCODE_STOREDATE 2   //데이터 서버에 저장 요청
#define OPCODE_PRETRAINING  3  //훈련 준비
#define OPCODE_TRAINSET  4     //훈련 방법 설정
#define OPCODE_REQUESTMODULE 6 //모듈 요청

//계정관련 OPCODE 1X
#define OPCODE_ID_CONNECTION 10 // 최초 서버 연결 시도시
#define OPCODE_ID_LOGIN 11
#define OPCODE_ID_REGISTER 12


//신경망 구성에서 각층을 나타내는 코드 입니다.
//packet의 명령코드가 아니라 신경망의 구성을 위해 사용되는 상수입니다.
#define CODE_INPUT 1001
#define CODE_CONV  1002
#define CODE_POOL  1003
#define CODE_FULLY 1004
#define CODE_ACTIV 1005
#define CODE_BATCH 1006
#define CODE_DROP  1007
#define CODE_OUTP  1008
#define CODE_OPTMZ 1009
#define CODE_END   10099

// char shortBuffer[BUFSIZE]; //약 1MB   둘다 사용하지 않음.
// char bigBuffer[BUFSIZE * 10]; //약 100MB


//한번에 네트워크 전송으로 모두 전송하는 packet 구조체입니다.
//가령 n_opcode가 0인 패킷은 로그인 할때 사용되는 패킷입니다.
//n_opcode에 들어가는 값들은 아레에 #define으로 정의합니다. 
typedef struct _packet {
	int n_opcode;   //명령코드
	int n_size;     //페이로드의 길이
	char* cptr_payload;//전체 페이로드를 나타내는 포인터입니다. 
	                   //가변길이 이므로 포인터로 동적할당을 받도록 했습니다.
}packet;

//패킷에는 여러가지가 있으나 대부분이 가변길이 이므로 귀차니즘으로 구조체를 만들지 않고...
//일일이 함수에서 포인터 연산으로 만든부분이 많습니다... ㅎㅎ
//일단 훈련과정중 배치 크기나 전체 반복횟수등을 전달할때는 무조건 고정이므로 이는 구조체를 만들었습니다.
typedef struct _train_setting_payload {
	int n_batch_size;
	int n_eopch;
	int n_test_cycle;
}train_setting_payload;

SOCKET createSocket(char* ipaddr, int port);

//기본적으로 init_#######Packet과 같은 함수에 packet형 구조체 변수의 주소를 전달합니다.
//그러면 init_######Packet 함수에서 기능에 맞는 opcode와 페이로드를 구성합니다.
//그런후에 opcode, 페이로드의 길이, 페이로드를 init_packet 함수에 전달하는 방식입니다.
//마지막으로 이렇게 구성된 packet형 구조체를 sendToServ함수에 전달하면 전송됩니다.

//packet구조체를 초기화하고 서버와의 수신 송신을 위한 함수입니다. 
void init_packet(packet* ptr, int opcode, int size, char* payload);
//아래의 init_####Packet 함수의 멘 마지막에서 완성된 payload를 packet구조체에 넣는 기능을 합니다.

void init_initialConnectionPacket(packet* pack);
//최초 연결시의 패킷을 초기화하는 함수입니다.
//사용자의 이름을 요청합니다. 일단 영어가 좋습니다...
//가령 사용자의 이름이 david 이라면 david이라는 이름을
//가진 폴더를 서버에 구성하고 그곳에 신경망과 데이터를 넣기위한 
//폴더를 각각 한개씩 구성합니다. 

void init_createClassifierPacket(packet* pack);
//분류기(신경망)을 구성할 때 사용하는 함수 입니다. 
//지금은 고정된 하나의 신경망을 구성하지만 후에 
//함수를 수정해서 다양한 신경망을 구성할 수 있을 것입니다.
//함수에서는 현재 생성할 신경망의 이름을 요구합니다.
//가령 신경망의 이름이 net 이라고 한다면 서버에서는 
//신경망들을 저장하고 있는 폴더에 텐서플로우에서 사용하는
//ckpt(check point)파일을 구성합니다. ex net.ckpt, net.ckpt.meta ... 등등
//즉 서버에서는 사용자가 사용하고자 하는 신경망을 이름으로 구분합니다.

void init_dataSendPacket(packet* pack);
//압축파일을 읽어들인 다음에 전송하는 함수입니다.
//함수에서는 레이블에 해당하는 이름과 압축파일을 요구합니다.
//가령 cat이라는 이름과 cat.zip파일을 요구합니다.
//전송되면 서버에서는 cat이라는 폴더가 생성되며 cat.zip파일을 그곳에 저장하고
//압축을 해제한 후에 이미지 하나씩 읽어 들이고 255로 나누어 전처리 하여 vec.csv라는 파일을
//서버에 저장해 놓습니다. 그 후에 훈련시에 각 폴더(cat, dog, bird)등에
//포함되어 있는 vec.csv 파일을 모아서 하나의 파일로 만들고 학습합니다.
//서버에서는 위 과정이 오래 걸리기 때문에 이를 하나의 프로세스로 떨어트려 작업합니다.
//따라서 동시에 여러개의 파일 업로드가 가능합니다. 
//다만 느려 터졌습니다. 이는 후에 다른 기능을 다 완성하고 수정할 것입니다.. ㅜㅜ 

void init_prepareTrainingPacket(packet* pack);
//현재 학습할 신경망의 이름과 미리 서버로 전송한 파일들중 이번 훈련에 사용할 
//학습 데이터를 선택합니다. 가령 현재 사용하고자 하는 신경망의 이름이 net이고
//학습에 사용하고자 하는 데이터가 dog, cat, bird 라면 이를 일일이 전달해 주어야 합니다..

void init_trainSettingPacket(packet* pack);
//학습에 대한 정보를 전달하는 함수입니다. 가령 총 학습 반복 횟수와
//한번에 학습하고자 하는 데이터의 크기(배치크기), 몇번바다 검증 데이터로
//학습이 잘 되고 있는지 검증할지 등을 결정합니다.

void init_moduleRequestPacket(packet* pack);
//클라이언트에서 서버로 현재 학습하고 있는 신경망의 모듈을 반환을 요청합니다.
//아직 구현되지 않았습니다. 

void sendToServ(SOCKET clntsock, packet* pack);
//클라이언트에서 서버로 packet형 구조체를 전달하는 함수입니다.
//기능 시험이 완료되어 대부분상황에 안전하게 동작합니다.
//주의해야 할 점은 이 함수의 마지막에 pack->cptr_payload의 동적할당된 것을
//해제해 주고 있다는 것입니다. 따라서 따로 페이로드에 할당된 것을
//해제해 주지 않아도 됩니다. 

void recvFromServ(SOCKET clntsock, packet* ptr);
//비슷한 구현으로 파이썬에서는 정상 동작을 하였는데 클라이언트에서는 아직
//동작이 확인되지 않았습니다. 

//아래에서는 오직 createClassifier에서 사용되는 즉 신경망을 구성할 때 사용되는 
//구조체들입니다. 이는 항상 고정길이이기 때문에 구조체로 만들면 간편하여 이렇게 하였습니다.
//신경망을 구성하기 위한 페이로드를 나타내는 구조체입니다. void형 포인터가 
//동적 할당된 다른 ####_payload 구조체의 주소를 가르키도록 설계했습니다.


typedef struct _payload {
	void* payload_arr[100];
}payload;

//모든 층은 int형의 n_code 변수를 가집니다. 
//n_code에 위에서 정의한 CODE_####이 할당됩니다.
//입력층을 나타내는 구조체 입니다.
typedef struct _input_payload {
	int n_code;
	int n_features;  //입력층의 특징 갯수입니다.
}input_payload;

//합성곱층을 나타내는 구조체 입니다.
typedef struct _conv_payload {
	int n_code;
	int n_init_method;    //초기화 방법입니다.
	float f_mean;         //평균입니다.
	float f_std;          //표준편차입니다.
	int n_filter_size;    //필터의 크기입니다. (가로 세로 동일하다 가정)
	int n_strides_size;   //보폭의 크기입니다. (가로 세로 동일하다 가정)
	int n_padding_method; //패딩의 방법입니다. 제로패딩, 패딩 적용하지 않음이 있습니다.
	int n_outputs;        //뉴런의 갯수, 또는 출력의 갯수입니다.
	float f_bias;         //합성곱연산 후에 더해지는 편향의 값입니다.
}conv_payload;

//풀링층을 나타내는 구조체입니다.
typedef struct _pooling_payload {
	int n_code;
	int n_filter_size;    //필터 또는 커널의 크기입니다. 
	int n_strides;        //보폭의 크기 입니다.
	int n_padding_method; //패딩의 방법입니다. 제로패딩, 패딩 적용하지 않음이 있습니다.
	int n_pooling_method; //풀링 방법입니다. 최대 풀링, 평균 풀링이 있습니다.
}pooling_payload;

//완전연결계층을 나타내는 구조체입니다.
typedef struct _fully_connect_payload {
	int n_code;
	int n_init_method;    //가중치 초기화 방법입니다.
	float f_mean;         //평균입니다. 
	float f_std;          //표준편차입니다.
	int n_outputs;        //뉴런의 갯수, 또는 출력의 갯수입니다.
	float f_bias;         //편향의 값입니다.
}fully_connect_payload;

//활성화 함수 계층을 나타내는 구조체입니다.
typedef struct _activation_payload {
	int n_code;
	int n_activation_method;//활성화 함수를 나타냅니다.
}activation_payload;

//배치 정규화 계층을 나타내는 구조체 입니다.
typedef struct _batch_norm_payload {
	int n_code;
	float f_momentum;       //모멘텀 변수를 사용하나 일반적으로 0.9가 최적입니다.
}batch_norm_payload;

//드롭아웃 계층을 나타내는 구조체 입니다.
typedef struct _dropout_payload {
	int n_code;
	float f_ratio;          //드롭아웃 비율을 나타냅니다. 일반적으로 0.5를 사용합니다.
}dropout_payload;

//출력(분류) 계층을 나타내는 구조체 입니다.
typedef struct _output_payload {
	int n_code;
	int n_is_onehot;        //원핫 인코딩 여부를 나타내는 변수이나 향후 없어질수 있습니다.
	int n_class_num;        //분류갯수를 나타내는 변수입니다.
}output_payload;

//옵티마이저를 설정해주는 구조체 입니다. 오차 값이 필요하여 주로 분류 계층 후에 옵니다. 
typedef struct _optimizer_payload {
	int n_code;
	int n_optimizer;
	float f_momentum;       //모멘텀 최적화에서 사용되는 변수입니다. 그 이외에는 사용되지 않습니다.
	float f_ratio;          //학습률을 나타내는 변수입니다. 일반적으로 0.01이 사용됩니다.
}optimizer_payload;

//마지막을 나타내는 구조체입니다. 간단히 n_code에 -99의 값이 필요하여 추가하였습니다.
typedef struct _end_payload {
	int n_code;
}end_payload;

// 저장할 디렉터리 가져오는 다이얼로그 띄우기
void GetDirPathDialog(CString* path, HWND targetWnd, LPCWSTR title);

void* init_input_payload(int n_features);
void* init_conv_payload(int n_init_method = 0, float f_mean = 0.0, float f_std = 0.1
	, int n_filtersize = 3, int n_stridesize = 1, int n_padding_method = 0
	, int n_outputs = 16, float f_bias = 0.1);
void* init_fully_connected_payload(int n_init_method = 0, float f_mean = 0.0,
	float f_std = 0.1, int n_outputs = 256, float f_bias = 0.1);
void* init_activation_payload(int n_activation_method = 0);
void* init_pooling_payload(int n_filter_size = 2, int n_stridesize = 2,
	int n_padding_method = 0, int n_pooling_method = 0);
void* init_batchnorm_payload(float f_momentum = 0.9);
void* init_dropout_payload(float f_ratio = 0.5);
void* init_output_payload(int n_class_num, int n_onehot = 0);
void* init_optimizer_payload(int n_optimizer_method = 0,
	float f_learning_rate = 0.01, float f_momentum = 0.9);
void* init_end_payload();

int calculate_payload_len(payload* ptr_payload);
int copy_payload_to_chararr(payload* ptr_payload, char* chararr);
void free_payload(payload* ptr_payload);


//init_####_payload 함수시리즈는 해당하는 구조체를 동적 생성하여 
//필요한 인자로 받은 필요한 값을 할당해주고 void* 포인터로 형변환하여 반환하는 함수입니다.

//########  init_input_payload함수와 init_output_payload 함수는 반드시 인자를 주어야 합니다!  ##########
//후에 각 함수에서 인자들이 어느 값의 범위를 가져야 하는지를 나중에 추가하겠습니다.
// 
// void* init_input_payload(int n_features) {
// 	input_payload* iptr = (input_payload*)malloc(sizeof(input_payload));
// 	iptr->n_code = CODE_INPUT;
// 	iptr->n_features = n_features;
// 
// 	return (void*)iptr;
// }
// 
// void* init_conv_payload(int n_init_method = 0, float f_mean = 0.0, float f_std = 0.1 
// 	, int n_filtersize = 3, int n_stridesize = 1, int n_padding_method = 0 
// 	, int n_outputs = 16, float f_bias = 0.1) {
// 	conv_payload* cptr = (conv_payload*)malloc(sizeof(conv_payload));
// 	cptr->n_code = CODE_CONV;
// 	cptr->n_init_method = n_init_method;
// 	cptr->f_mean = f_mean;
// 	cptr->f_std = f_std;
// 	cptr->n_filter_size = n_filtersize;
// 	cptr->n_strides_size = n_stridesize;
// 	cptr->n_padding_method = n_padding_method;
// 	cptr->n_outputs = n_outputs;
// 	cptr->f_bias = f_bias;
// 
// 	return (void*)cptr;
// }
// 
// void* init_pooling_payload(int n_filter_size = 2, int n_stridesize = 2,
// 	int n_padding_method = 0, int n_pooling_method = 0) {
// 	pooling_payload* ptr = (pooling_payload*)malloc(sizeof(pooling_payload));
// 	ptr->n_code = CODE_POOL;
// 	ptr->n_filter_size = n_filter_size;
// 	ptr->n_strides = n_stridesize;
// 	ptr->n_padding_method = n_padding_method;
// 	ptr->n_pooling_method = n_pooling_method;
// 
// 	return (void*)ptr;
// }
// 
// void* init_fully_connected_payload(int n_init_method = 0, float f_mean = 0.0,
// 	float f_std = 0.1, int n_outputs = 256, float f_bias = 0.1) {
// 	fully_connect_payload* ptr = (fully_connect_payload*)malloc(sizeof(fully_connect_payload));
// 	ptr->n_code = CODE_FULLY;
// 	ptr->n_init_method = n_init_method;
// 	ptr->f_mean = f_mean;
// 	ptr->f_std = f_std;
// 	ptr->n_outputs = n_outputs;
// 	ptr->f_bias = f_bias;
// 
// 	return (void*)ptr;
// }
// 
// void* init_activation_payload(int n_activation_method = 0) {
// 	activation_payload* ptr = (activation_payload*)malloc(sizeof(activation_payload));
// 	ptr->n_code = CODE_ACTIV;
// 	ptr->n_activation_method = n_activation_method;
// 
// 	return (void*)ptr;
// }
// 
// void* init_batchnorm_payload(float f_momentum = 0.9) {
// 	batch_norm_payload* ptr = (batch_norm_payload*)malloc(sizeof(batch_norm_payload));
// 	ptr->n_code = CODE_BATCH;
// 	ptr->f_momentum = f_momentum;
// 
// 	return (void*)ptr;
// }
// 
// void* init_dropout_payload(float f_ratio = 0.5) {
// 	dropout_payload* ptr = (dropout_payload*)malloc(sizeof(dropout_payload));
// 	ptr->n_code = CODE_DROP;
// 	ptr->f_ratio = f_ratio;
// 
// 	return (void*)ptr;
// }
// 
// void* init_output_payload(int n_class_num, int n_onehot = 0) {
// 	output_payload* ptr = (output_payload*)malloc(sizeof(output_payload));
// 	ptr->n_code = CODE_OUTP;
// 	ptr->n_is_onehot = n_onehot;
// 	ptr->n_class_num = n_class_num;
// 
// 	return (void*)ptr;
// }
// 
// void* init_optimizer_payload(int n_optimizer_method = 0,
// 	float f_learning_rate = 0.01, float f_momentum = 0.9) {
// 	optimizer_payload* ptr = (optimizer_payload*)malloc(sizeof(optimizer_payload));
// 	ptr->n_code = CODE_OPTMZ;
// 	ptr->n_optimizer = n_optimizer_method;
// 	ptr->f_ratio = f_learning_rate;
// 	ptr->f_momentum = f_momentum;
// 
// 	return (void*)ptr;
// }
// 
// void* init_end_payload() {
// 	end_payload* ptr = (end_payload*)malloc(sizeof(end_payload));
// 	ptr->n_code = CODE_END;
// 
// 	return (void*)ptr;
// }
// 
// //payload의 모든 void* 포인터에 동적할당된 주소를 해제해주는 함수입니다.
// void free_payload(payload* ptr_payload) {
// 	int i;
// 	for (i = 0; i < 100; i++) {
// 		int n_code = *((int*)(ptr_payload->payload_arr[i]));
// 		free(ptr_payload->payload_arr[i]);
// 		if (n_code == CODE_END) break;
// 	}
// }
// 
// //페이로드의 전체 길이를 구하는 함수입니다.
// int calculate_payload_len(payload* ptr_payload) {
// 	int i, chararr_idx = 0;
// 	for (i = 0; i < 100; i++) {
// 		int* p_code = (int*)(ptr_payload->payload_arr[i]);
// 
// 		if ((*p_code) == CODE_INPUT) {
// 			input_payload* ptr = (input_payload*)(ptr_payload->payload_arr[i]);
// 			//memcpy(&chararr[chararr_idx], ptr, sizeof(input_payload));
// 			chararr_idx += sizeof(input_payload);
// 
// 			//*payloadSize += sizeof(input_payload);
// 		}
// 		else if ((*p_code) == CODE_CONV) {
// 			conv_payload* ptr = (conv_payload*)(ptr_payload->payload_arr[i]);
// 			//memcpy(&chararr[chararr_idx], ptr, sizeof(conv_payload));
// 			chararr_idx += sizeof(conv_payload);
// 
// 			//*payloadSize += sizeof()
// 		}
// 		else if ((*p_code) == CODE_POOL) {
// 			pooling_payload* ptr = (pooling_payload*)(ptr_payload->payload_arr[i]);
// 			//memcpy(&chararr[chararr_idx], ptr, sizeof(pooling_payload));
// 			chararr_idx += sizeof(pooling_payload);
// 		}
// 		else if ((*p_code) == CODE_FULLY) {
// 			fully_connect_payload* ptr = (fully_connect_payload*)(ptr_payload->payload_arr[i]);
// 			//memcpy(&chararr[chararr_idx], ptr, sizeof(fully_connect_payload));
// 			chararr_idx += sizeof(fully_connect_payload);
// 		}
// 		else if ((*p_code == CODE_ACTIV)) {
// 			activation_payload* ptr = (activation_payload*)(ptr_payload->payload_arr[i]);
// 			//memcpy(&chararr[chararr_idx], ptr, sizeof(activation_payload));
// 			chararr_idx += sizeof(activation_payload);
// 		}
// 		else if ((*p_code) == CODE_BATCH) {
// 			batch_norm_payload* ptr = (batch_norm_payload*)(ptr_payload->payload_arr[i]);
// 			//memcpy(&chararr[chararr_idx], ptr, sizeof(batch_norm_payload));
// 			chararr_idx += sizeof(batch_norm_payload);
// 		}
// 		else if ((*p_code) == CODE_DROP) {
// 			dropout_payload* ptr = (dropout_payload*)(ptr_payload->payload_arr[i]);
// 			//memcpy(&chararr[chararr_idx], ptr, sizeof(dropout_payload));
// 			chararr_idx += sizeof(dropout_payload);
// 		}
// 		else if ((*p_code) == CODE_OUTP) {
// 			output_payload* ptr = (output_payload*)(ptr_payload->payload_arr[i]);
// 			//memcpy(&chararr[chararr_idx], ptr, sizeof(output_payload));
// 			chararr_idx += sizeof(output_payload);
// 		}
// 		else if ((*p_code) == CODE_OPTMZ) {
// 			optimizer_payload* ptr = (optimizer_payload*)(ptr_payload->payload_arr[i]);
// 			//memcpy(&chararr[chararr_idx], ptr, sizeof(optimizer_payload));
// 			chararr_idx += sizeof(optimizer_payload);
// 		}
// 		else if ((*p_code) == CODE_END) {
// 			end_payload* ptr = (end_payload*)(ptr_payload->payload_arr[i]);
// 			//memcpy(&chararr[chararr_idx], ptr, sizeof(end_payload));
// 			chararr_idx += sizeof(end_payload);
// 
// 			break;
// 		}
// 		else {
// 			printf("copy_payload_to_chararr에서 에러! i 는 %d \n", i);
// 		}
// 
// 	}
// 	return chararr_idx;
// }

//구조가 완성된 payload와 char형 배열을 입력으로 받습니다.
//char형 배열은 소켓으로 보내기 위한 동적할당된 바이트 배열이라고 가정했습니다.
//payload의 void* 포인터가 가르키는 구조체의 종류를 파악해서
//형변환을 한 후에 char 배열에 구조체의 길이만큼 memcpy를 해줍니다.
//종료층을 만나면 복사를 종료합니다.
// int copy_payload_to_chararr(payload* ptr_payload, char* chararr) {
// 	int i,chararr_idx = 0;
// 	for (i = 0; i < 100; i++) {
// 		int* p_code = (int*)(ptr_payload->payload_arr[i]);
// 
// 		if ((*p_code) == CODE_INPUT) {
// 			input_payload* ptr = (input_payload*)(ptr_payload->payload_arr[i]);
// 			memcpy(&chararr[chararr_idx], ptr, sizeof(input_payload));
// 			chararr_idx += sizeof(input_payload);
// 			
// 			//*payloadSize += sizeof(input_payload);
// 		}
// 		else if ((*p_code) == CODE_CONV) {
// 			conv_payload* ptr = (conv_payload*)(ptr_payload->payload_arr[i]);
// 			memcpy(&chararr[chararr_idx], ptr, sizeof(conv_payload));
// 			chararr_idx += sizeof(conv_payload);
// 
// 			//*payloadSize += sizeof()
// 		}
// 		else if ((*p_code) == CODE_POOL) {
// 			pooling_payload* ptr = (pooling_payload*)(ptr_payload->payload_arr[i]);
// 			memcpy(&chararr[chararr_idx], ptr, sizeof(pooling_payload));
// 			chararr_idx += sizeof(pooling_payload);
// 		}
// 		else if ((*p_code) == CODE_FULLY) {
// 			fully_connect_payload* ptr = (fully_connect_payload*)(ptr_payload->payload_arr[i]);
// 			memcpy(&chararr[chararr_idx], ptr, sizeof(fully_connect_payload));
// 			chararr_idx += sizeof(fully_connect_payload);
// 		}
// 		else if ((*p_code == CODE_ACTIV)) {
// 			activation_payload* ptr = (activation_payload*)(ptr_payload->payload_arr[i]);
// 			memcpy(&chararr[chararr_idx], ptr, sizeof(activation_payload));
// 			chararr_idx += sizeof(activation_payload);
// 		}
// 		else if ((*p_code) == CODE_BATCH) {
// 			batch_norm_payload* ptr = (batch_norm_payload*)(ptr_payload->payload_arr[i]);
// 			memcpy(&chararr[chararr_idx], ptr, sizeof(batch_norm_payload));
// 			chararr_idx += sizeof(batch_norm_payload);
// 		}
// 		else if ((*p_code) == CODE_DROP) {
// 			dropout_payload* ptr = (dropout_payload*)(ptr_payload->payload_arr[i]);
// 			memcpy(&chararr[chararr_idx], ptr, sizeof(dropout_payload));
// 			chararr_idx += sizeof(dropout_payload);
// 		}
// 		else if ((*p_code) == CODE_OUTP) {
// 			output_payload* ptr = (output_payload*)(ptr_payload->payload_arr[i]);
// 			memcpy(&chararr[chararr_idx], ptr, sizeof(output_payload));
// 			chararr_idx += sizeof(output_payload);
// 		}
// 		else if ((*p_code) == CODE_OPTMZ) {
// 			optimizer_payload* ptr = (optimizer_payload*)(ptr_payload->payload_arr[i]);
// 			memcpy(&chararr[chararr_idx], ptr, sizeof(optimizer_payload));
// 			chararr_idx += sizeof(optimizer_payload);
// 		}
// 		else if ((*p_code) == CODE_END) {
// 			end_payload* ptr = (end_payload*)(ptr_payload->payload_arr[i]);
// 			memcpy(&chararr[chararr_idx], ptr, sizeof(end_payload));
// 			chararr_idx += sizeof(end_payload);
// 
// 			break;
// 		}
// 		else {
// 			printf("copy_payload_to_chararr에서 에러! i 는 %d \n",i);
// 		}
// 		
// 	}
// 	return chararr_idx;
// }


//init_#####Packet 함수를 이용해서 packet 형 구조체를 구성합니다.
//init_####PAcket 함수내에서 init_packet 함수를 호출 합니다.
//payloadSize변수는 전체 페이로드의 길이를 나타내며 
//pack.n_size 에 할당될 것입니다. 즉 payload의 길이입니다.
//payloadSize 변수가 구해지면 char* 형 allPayload변수를 통해
//동적 payloadSize만큼 동적 할당을 받습니다.
//그후 payload가 될 부분만을 차레로 allPayload 변수에 memcpy해주고
//opcode, payloadSize, allPayload변수를 init_packet 함수를 호출합니다.
//모두 이런식으로 구현하려 합니다.

// void init_createClassifierPacket(packet* pack) {
// 	int opcode = 1, namelen = 0;
// 	int payloadSize = 0; //전체 페이로드의 길이
// 	char namebuf[100];
// 	char* name = NULL;
// 	printf("신경망의 이름<<");
// 	scanf("%s", namebuf);
// 	namelen = strlen(namebuf) +1;
// 	name = (char*)malloc(namelen);
// 	strcpy_s(name, namelen, namebuf);
// 	printf("신경망의 이름>> %s\n", name);
// 
// 	payloadSize += (sizeof(namelen) + namelen);
// 
// 	payload pay_load;
// 	pay_load.payload_arr[0] = init_input_payload(3072);
// //	payloadSize += sizeof(input_payload);
// 
// 	pay_load.payload_arr[1] = init_conv_payload();
// //	payloadSize += sizeof(conv_payload);
// 
// 	pay_load.payload_arr[2] = init_activation_payload();
// 	//payloadSize += sizeof(activation_payload);
// 
// 	pay_load.payload_arr[3] = init_conv_payload();
// 	//payloadSize += sizeof(conv_payload);
// 
// 	pay_load.payload_arr[4] = init_activation_payload();
// 	//payloadSize += sizeof(activation_payload);
// 
// 	pay_load.payload_arr[5] = init_fully_connected_payload();
// 	//payloadSize + sizeof(fully_connect_payload);
// 
// 	pay_load.payload_arr[6] = init_activation_payload();
// 	//payloadSize += sizeof(activation_payload);
// 
// 	pay_load.payload_arr[7] = init_fully_connected_payload();
// 	//payloadSize += sizeof(fully_connect_payload);
// 
// 	pay_load.payload_arr[8] = init_activation_payload();
// 
// 	pay_load.payload_arr[9] = init_output_payload(3);
// 	//payloadSize += sizeof(output_payload);
// 
// 	pay_load.payload_arr[10] = init_optimizer_payload();
// 	//payloadSize += sizeof(optimizer_payload);
// 
// 	pay_load.payload_arr[11] = init_end_payload();
// 	//payloadSize += sizeof(end_payload);
// 
// 	int calculatelen = calculate_payload_len(&pay_load);
// 	payloadSize += calculatelen;
// 	char* allPayload = (char*)malloc(payloadSize * sizeof(char));  //페이로드 길이 + 페이로드
// 	
// 	memcpy(&allPayload[0], &namelen, sizeof(int));  //신경망의 이름길이를 전달
// 	memcpy(&allPayload[sizeof(namelen)], name, namelen);//신경망의 이름을 전달
// 	int n = copy_payload_to_chararr(&pay_load, &allPayload[sizeof(namelen) + namelen]);//신경망의 구성정보를 전달
// 
// 
// 	free_payload(&pay_load);
// 
// 
// 	printf("n: %d, calcuatelen: %d,  all: %d \n\n", n, calculatelen, payloadSize); //192 - 8  168
// 	init_packet(pack, opcode, payloadSize, allPayload);
// }

void init_initialConnectionPacket(packet* pack) {
	int opcode = 0, len = 0;
	char buf[100];
	char* cptr;
	printf("아직 서버에서 예외처리가 되지 않아서 반드시 사용자 이름을 입력하고 사용해야 합니다!!\n");
	printf("사용자의 이름<<");
	scanf("%s", buf);
	len = strlen(buf) + 1;
	cptr = (char*)malloc(len+sizeof(len));
	memcpy(cptr, &len, sizeof(len));
	memcpy(&cptr[sizeof(len)], buf, len);
	//strcpy_s(cptr, len, buf);
	printf("디버깅용: 사용자의 이름>> %s\n", &cptr[sizeof(len)]);
	printf("디버깅용: 이름의 길이>> %d\n", len);

	// 페이로드의 길이는 사용자의 이름 길이를 나타내는 int(4) + 사용자의 이름의 길이
	init_packet(pack, opcode, sizeof(len) + len, cptr);

}

// void init_dataSendPacket(packet* pack) {
// 	int opcode = 2, payloadSize = 0, namelen = 0, zipSize = 0, statResult;
// 	char namebuf[100], zipbuf[100], *name = NULL, *allPayload = NULL;
// 	struct _stat statbuf;
// 	FILE* fp = NULL;
// 	printf("이미지의 이름 입력(ex. cat)<<");
// 	scanf("%s", namebuf);
// 	namelen = strlen(namebuf) + 1;
// 	payloadSize += sizeof(namelen) + namelen;
// 
// 	name = (char*)malloc(namelen);
// 	strcpy_s(name, namelen, namebuf);
// 
// 	printf("압축파일의 이름을 입력(ex. cat.zip)<<");
// 	scanf("%s", zipbuf);
// 	// 이곳에 압축파일을 로드하는 기능을 구현하자
// 	statResult = _stat(zipbuf, &statbuf);
// 	if (statResult != 0) {
// 		printf("stat error");
// 		printf("statResult: %d \n", statResult);
// 	}
// 	else {
// 		printf("디버깅용: filesize : %d \n", statbuf.st_size);
// 		payloadSize += statbuf.st_size;
// 
// 	}
// 	allPayload = (char*)malloc(payloadSize);
// 
// 	memcpy(&allPayload[0], &namelen, sizeof(namelen));
// 	memcpy(&allPayload[sizeof(namelen)], name, namelen);
// 
// 	fp = fopen(zipbuf, "rb");
// 	fread(&allPayload[sizeof(namelen) + namelen], statbuf.st_size, 1, fp);
// 	fclose(fp);
// 
// 
// 	init_packet(pack, opcode, payloadSize, allPayload);
// 
// }

void init_prepareTrainingPacket(packet* pack) {
	int opcode = 3, payloadSize = 0, namebuflen = 0;
	char namebuf[100], zipbuf[100], *name = NULL, *allPayload = NULL,*tmp = NULL;

	std::cout << "분류기 선택입니다>>";
	scanf("%s", namebuf);
	namebuflen = strlen(namebuf) + 1;
	payloadSize += sizeof(namebuflen) + namebuflen;

	allPayload = (char*)malloc(payloadSize);
	memcpy(allPayload, &namebuflen, sizeof(namebuflen));
	memcpy(&allPayload[sizeof(namebuflen)], namebuf, namebuflen);

	while (1) {
		std::cout << "훈련 데이터를 입력하세요 -1 입력시 종료합니다 >>";
		memset(namebuf, 0, sizeof(namebuf));
		scanf("%s", namebuf);
		if (namebuf[0] == '-' && namebuf[1] == '1') {
			int minusone = -1;
			if (tmp != NULL) {
				free(tmp);
				tmp = NULL;
			}
			tmp = (char*)malloc(payloadSize);
			memcpy(tmp, allPayload, payloadSize);
			payloadSize += sizeof(minusone);
			free(allPayload);
			allPayload = (char*)malloc(payloadSize);
			memcpy(allPayload, tmp, payloadSize - sizeof(minusone));
			memcpy(&allPayload[payloadSize - sizeof(minusone)], &minusone, sizeof(minusone));
			free(tmp);
			break;
		}
		namebuflen = strlen(namebuf) + 1;
		if (tmp != NULL) {
			free(tmp);
			tmp = NULL;
		}
		tmp = (char*)malloc(payloadSize);
		memcpy(tmp, allPayload, payloadSize);
		payloadSize += sizeof(namebuflen) + namebuflen;
		free(allPayload);
		allPayload = (char*)malloc(payloadSize);
		memcpy(allPayload, tmp, payloadSize - (sizeof(namebuflen) + namebuflen));
		memcpy(&allPayload[payloadSize - (sizeof(namebuflen) + namebuflen)], &namebuflen, sizeof(namebuflen));
		memcpy(&allPayload[payloadSize - namebuflen], namebuf, namebuflen);
		
	}

	init_packet(pack, opcode, payloadSize, allPayload);
}


void init_trainSettingPacket(packet* pack) {
	int opcode = 4, payloadSize = 0;
	train_setting_payload * ptr = (train_setting_payload*)malloc(sizeof(train_setting_payload));
	std::cout << "배치사이즈를 입력하세요>>";
	scanf("%d", &(ptr->n_batch_size));
	std::cout << "총 반복횟수를 입력하세요>>";
	scanf("%d", &(ptr->n_eopch));
	std::cout << "검증 주기를 입력하세요>>";
	scanf("%d", &(ptr->n_test_cycle));
	payloadSize += sizeof(train_setting_payload);
	init_packet(pack, opcode, payloadSize, (char*)ptr);
}

void init_moduleRequestPacket(packet* pack) {
	int opcode = OPCODE_REQUESTMODULE;
	int payloadSize = 0,namelen = 0;
	char buf[100];
	char* allPayload = NULL;
	std::cout << "요청할 신경망의 이름을 입력하세요>>";
	scanf("%s", buf);
	namelen = strlen(buf) + 1;
	payloadSize += sizeof(namelen);
	payloadSize += namelen;
	allPayload = (char*)malloc(payloadSize);
	memcpy(allPayload, &namelen, sizeof(namelen));
	memcpy(&allPayload[sizeof(namelen)], buf, namelen);
	init_packet(pack, opcode, payloadSize, allPayload);
}


//사용자로부터 받은 zip파일을 저장하고 동적할당된 메모리를 해제해주는 함수입니다.
void storeZipData(packet* pack) {
	FILE* fp = fopen("test.zip", "wb");
	fwrite(pack->cptr_payload, pack->n_size, 1, fp);
	fclose(fp);
	free(pack->cptr_payload);
}


void init_packet(packet* ptr, int opcode, int size, char* payload) {
	ptr->n_opcode = opcode;
	ptr->n_size = size;
	ptr->cptr_payload = payload;
}

//일단 사용되지 않는 코드 입니다.
// void sendToServWORKING(SOCKET clntsock, packet* pack) {
// 	//buf를 새로 할당하지 말고 8바이트를 보낸후에 pack->cptr_payload를 전송해보자.
// 	int remain = 0;
// 	//opcode와 payload의 길이 payload를 포함하는 크기
// 	//흠 혹시 메모리 누수가 일어날까? 만약 그렇다면 이곳에서 동적 할당을 받지 말아야 할까?
// 	//동적할당 받지 않는건 답이 아닌것 같다. 메모리 누수가 일어나는지 확인해보자.
// 	remain = pack->n_size;
// 	send(clntsock, (char*)pack, sizeof(int) * 2, 0);
// 
// 	if (remain < BUFSIZE) {
// 		memset(shortBuffer, 0, sizeof(shortBuffer));
// 		strcpy_s(shortBuffer, pack->n_size, pack->cptr_payload);
// 		//free(pack->cptr_payload);
// 
// 		send(clntsock, &shortBuffer[0], pack->n_size, 0);
// 	}
// 	else {
// 		memset(bigBuffer, 0, sizeof(bigBuffer));
// 		memcpy(bigBuffer, pack->cptr_payload, pack->n_size);
// 		//free(pack->cptr_payload);
// 
// 		int start = 0;
// 		while (remain > 0) {
// 			if (remain < BUFSIZE) {
// 				send(clntsock, &(bigBuffer[start]), remain, 0);
// 				start += remain;
// 				remain -= remain;
// 			}
// 			else {
// 				send(clntsock, &(bigBuffer[start]), BUFSIZE, 0);
// 				start += BUFSIZE;
// 				remain -= BUFSIZE;
// 			}
// 		}
// 	}
// 
// 	//여기서 에러가 나는 이유는 아직 전송되지 않은 것을 해제 하기 때문인가?
// 	//이곳의 동적 할당 버퍼를 만들지 말고 스택에 버퍼를 만들어 두고 해보자.
// }
// 
//현재 사용되는 코드입니다.
/*
void sendToServ(SOCKET clntsock, packet* pack) {
	int remain = 0;

	remain = pack->n_size;
	send(clntsock, (char*)pack, sizeof(int) * 2, 0);
	//패킷의 opcode와 페이로드의 길이(n_size)를 먼저 전달합니다.
	//이는 서버에서 명령코드를 읽고 어떤 기능인지 확인할 수 있고
	//뒤에 n_size를 읽어들여 페이로드의 길이를 확인하고
	//페이로드의 길이만큼만 읽어들이기 위함입니다.


	//지금은 페이로드의 길이가 BUFSIZE보다 작으면 한번에 보내고
	//그렇지 않으면 고정된 메모리 공간에 복사하고 보내지만 
	//사실 그냥 send(clntsock, pack->cptr_payload, pack->n_size, 0);
	//한번이면 다 보내질것 같습니다. 이는 나중에 수정해 보려고 합니다.
	send(clntsock, pack->cptr_payload, pack->n_size, 0);
	free(pack->cptr_payload);


	/*
	if (remain < BUFSIZE) {
		send(clntsock, pack->cptr_payload, pack->n_size, 0);
		free(pack->cptr_payload);
	}
	else {
		//여기 부분이 에러가 날 수도 있다. 
		//bifBuffer를 사용하지 않고 cptr_payload를 전송하는 코드로 다시 복귀하자
		//버퍼에 올리는건 잘라서 올리지 않아도 되지 않는가? 그냥 한번에 쭉 보내도 될것 같다.
		memset(bigBuffer, 0, sizeof(bigBuffer));
		memcpy(bigBuffer, pack->cptr_payload, pack->n_size);
		free(pack->cptr_payload);

		int start = 0;
		while (remain > 0) {
			if (remain < BUFSIZE) {
				send(clntsock, &(bigBuffer[start]), remain, 0);
				start += remain;
				remain -= remain;
			}
			else {
				send(clntsock, &(bigBuffer[start]), BUFSIZE, 0);
				start += BUFSIZE;
				remain -= BUFSIZE;
			}
		}
	}
	

}*/
//현재는 사용되지 않는 코드입니다. 
void sendToServ_ORIGINAL(SOCKET clntsock, packet* pack) {

	//버퍼 동적할당 받고 pack의 모든 정보를 char형으로 복사붙여넣기 한다.
	//그후 차레차레 전송한다. 
	//이 함수의 마지막에 payload를 free로 초기화 한다.
	int totLen, curLen, payloadLen, remain;
	totLen = curLen = payloadLen = remain = 0;

	payloadLen = pack->n_size;
	char* buf = (char*)malloc(pack->n_size + 2 * sizeof(int));
	//opcode와 payload의 길이 payload를 포함하는 크기
	//흠 혹시 메모리 누수가 일어날까? 만약 그렇다면 이곳에서 동적 할당을 받지 말아야 할까?
	//동적할당 받지 않는건 답이 아닌것 같다. 메모리 누수가 일어나는지 확인해보자.
	remain = sizeof(int) * 2 + payloadLen;

	//memcpy를 활용해서 값 복사가 아니라 메모리를 복사함 따라서 속도가 느리지 않음
	memcpy(&buf[0], pack, sizeof(int) * 2); //명령코드와 사이즈가 복사될 것을 기대함
	memcpy(&buf[sizeof(int) * 2], pack->cptr_payload, payloadLen); //기가막히게 잘 복사된다.

	int start = 0;
	while (remain > 0) {

		if (remain < BUFSIZE) {
			send(clntsock, &buf[start], remain, 0);
			start += remain;
			remain -= remain;
		}
		else {
			send(clntsock, &buf[start], BUFSIZE, 0);
			start += BUFSIZE;
			remain -= BUFSIZE;
		}

	}
	free(buf); //여기서 pack->cptr_payload를 buf에 memcpy한것도 같이 free 시키므로 모두 해제 되는것 같다.
}

/*
//기능시험이 필요합니다.
void recvFromServ(SOCKET clntsock, packet* ptr) {
	int remain = 0, start;

	int recvlen = 0;
	while (1) {
		int curlen = recv(clntsock, &(((char*)ptr)[recvlen]), 8 - recvlen, 0);
		recvlen += curlen;
		printf("curlen : %d, recvlen : %d \n", curlen, recvlen);
		if (recvlen == 8) break;
	}
	printf("n_opcode : %d, n_size: %d \n", ptr->n_opcode, ptr->n_size);

	//사실 여기서 반드시 8바이트를 읽어들일 수 있을거라는 보장이 없다.
	ptr->cptr_payload = (char*)malloc(ptr->n_size);
	if (ptr->cptr_payload == NULL) {
		printf("recvFromServ에서 malloc 에러 \n");
		exit(1);
	}
	remain = ptr->n_size;
	start = 0;
	while (remain > 0) {
		int recvLen = 0;
		if (remain < BUFSIZE) {
			recvLen = recv(clntsock, &(ptr->cptr_payload[start]), remain, 0);
			start += recvLen;
			remain -= recvLen;
		}
		else {
			recvLen = recv(clntsock, &(ptr->cptr_payload[start]), BUFSIZE, 0);
			start += recvLen;
			remain -= recvLen;
		}
	}
}*/


//현재 사용되지 않는 함수입니다.
void send_chararr(char chararr[],int arrlen = 1024) {
	SOCKET clientsock;
	WSADATA wsa;
	struct sockaddr_in sockinfo;
	char message[30];
	int len;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
		printf("초기화 실패\n");

	clientsock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

	if (clientsock == INVALID_SOCKET)
		printf("소켓 생성 실패\n");

	memset(&sockinfo, 0, sizeof(sockinfo));

	sockinfo.sin_family = AF_INET;
	sockinfo.sin_port = htons(10000);
	sockinfo.sin_addr.s_addr = inet_addr("127.0.0.1");

	if (connect(clientsock, (SOCKADDR*)&sockinfo, sizeof(sockinfo)) == SOCKET_ERROR)
		printf(" 서버 접속 실패 ");

//	printf("%s \n", (char*)&data1);


	printf("charlarr len -->  %d \n", sizeof(chararr));
	len = send(clientsock, chararr, arrlen, 0);
	/*
	int a = 10;
	input_payload* ptr = (input_payload*)malloc(sizeof(input_payload));
	ptr->n_code = CODE_INPUT;
	ptr->n_features = 100;
	send(clientsock, (char*)ptr, sizeof(input_payload), 0);

	free(ptr);
	*/
	

	closesocket(clientsock);
	WSACleanup();

}

//현재사용되지 않는 함수입니다.
void show(void* ptr[]) {
	input_payload* iptr = (input_payload*)ptr[0];
	conv_payload* cptr = (conv_payload*)ptr[1];
	printf("%d %d \n", iptr->n_code, iptr->n_features);
	printf("%d %f %f %d %d %d %d %d %f", cptr->n_code, cptr->n_init_method, cptr->f_mean, cptr->f_std, cptr->n_filter_size
		, cptr->n_strides_size, cptr->n_padding_method, cptr->n_outputs, cptr->f_bias);
}

//현재 사용되지 않는 함수입니다.
void testshow(char chararr[]) {
	int idx = 0;
	input_payload* iptr = (input_payload*)&chararr[idx];
	idx += sizeof(input_payload);
	conv_payload* cptr = (conv_payload*)&chararr[idx];
	idx += sizeof(conv_payload);

	printf("%d %d \n", iptr->n_code, iptr->n_features);
	printf("code: %d\ninit: %d\nmean: %f\nstd: %f\nfiltersize: %d\nstridessize: %d\npadding: %d\noutput: %d\nbias: %f\n", 
		cptr->n_code, cptr->n_init_method, cptr->f_mean, cptr->f_std, cptr->n_filter_size
		, cptr->n_strides_size, cptr->n_padding_method, cptr->n_outputs, cptr->f_bias);
}



//현재 main함수입니다.
//소켓을 초기화하고 서버에 연결한 후에 반복문을 돌며 기능을 수행합니다.
void testmain3() {
	SOCKET clientsock;
	WSADATA wsa;
	struct sockaddr_in sockinfo;
	char message[30];
	int len;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
		printf("초기화 실패\n");

	clientsock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

	if (clientsock == INVALID_SOCKET)
		printf("소켓 생성 실패\n");

	memset(&sockinfo, 0, sizeof(sockinfo));

	sockinfo.sin_family = AF_INET;
	sockinfo.sin_port = htons(10000);
	sockinfo.sin_addr.s_addr = inet_addr("127.0.0.1");

	if (connect(clientsock, (SOCKADDR*)&sockinfo, sizeof(sockinfo)) == SOCKET_ERROR)
		printf(" 서버 접속 실패 ");


	packet pack;
	pack.cptr_payload = NULL;

	while (1) {
		int op = 0;
		printf("0.exit 1.Connection 2.Classifier 3.sendData 4.prepareTraining 5.trainSetting 6.requestModule \n>>");
		scanf("%d", &op);

		if (op == 0) {
			break;
		}
		else if (op == 1) {
			init_initialConnectionPacket(&pack);
			sendToServ(clientsock, &pack);
		}
		else if (op == 2) {
			init_createClassifierPacket(&pack);
			sendToServ(clientsock, &pack);
		}
		else if (op == 3) {
			// init_dataSendPacket(&pack);
			sendToServ(clientsock, &pack);
		}
		else if (op == 4) {
			init_prepareTrainingPacket(&pack);
			sendToServ(clientsock, &pack);
		}
		else if (op == 5) {
			init_trainSettingPacket(&pack);
			sendToServ(clientsock, &pack);
		}
		else if (op == 6) {
			init_moduleRequestPacket(&pack);
			sendToServ(clientsock, &pack);
			recvFromServ(clientsock, &pack);
			storeZipData(&pack);
		}
		else {
			continue;
		}
	}
	closesocket(clientsock);
	WSACleanup();
}


void testmain2() {
	char chararr[1024];
	payload pay_load;
	pay_load.payload_arr[0] = init_input_payload(300);
	pay_load.payload_arr[1] = init_conv_payload();
	pay_load.payload_arr[2] = init_activation_payload();
	pay_load.payload_arr[3] = init_conv_payload();
	pay_load.payload_arr[4] = init_activation_payload();
	pay_load.payload_arr[5] = init_fully_connected_payload();
	pay_load.payload_arr[6] = init_activation_payload();
	pay_load.payload_arr[7] = init_fully_connected_payload();
	pay_load.payload_arr[8] = init_output_payload(5);
	pay_load.payload_arr[9] = init_optimizer_payload();
	pay_load.payload_arr[10] = init_end_payload();

	int size = copy_payload_to_chararr(&pay_load, chararr);
	
	printf("%d \n", ((int*)chararr)[0]);
	printf("%d \n", ((int*)chararr)[1]);
	printf("chararr size : %d \n", size);
	//testshow(chararr);
	send_chararr(chararr,1024);


	free_payload(&pay_load);

}

void testmain1() {
	payload _payload;
	_payload.payload_arr[0] = init_input_payload(150);
	_payload.payload_arr[1] = init_conv_payload();
	_payload.payload_arr[2] = init_activation_payload();
	_payload.payload_arr[3] = init_conv_payload();
	_payload.payload_arr[4] = init_activation_payload();
	_payload.payload_arr[5] = init_end_payload();

	show(_payload.payload_arr);


}


void testmain() {
	input_payload i_payload{ -1,100 };
	input_payload* i_ptr = (input_payload*)malloc(sizeof(input_payload));
	i_ptr->n_code = -1;
	i_ptr->n_features = 126;
	conv_payload c_payload{ -2,0,0.0,1.0,3,1,0,16,0.1 };
	void* ptr[100];
	ptr[0] = (void*)i_ptr;
	ptr[1] = (void*)&c_payload;
	show(ptr);

	free(ptr[0]);

	
}


#endif //  __PAYLOAD_H__