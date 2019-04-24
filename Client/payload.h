
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
//��Ʈ��ũ ������ �ϱ� ���ؼ� ��������ϴ�.

#define OPCODE_CONNECTION 0  //���ʿ����
#define OPCODE_CREATECLF 1   //�з��� ������
#define OPCODE_STOREDATE 2   //������ ������ ���� ��û
#define OPCODE_PRETRAINING  3  //�Ʒ� �غ�
#define OPCODE_TRAINSET  4     //�Ʒ� ��� ����
#define OPCODE_REQUESTMODULE 6 //��� ��û

//�������� OPCODE 1X
#define OPCODE_ID_CONNECTION 10 // ���� ���� ���� �õ���
#define OPCODE_ID_LOGIN 11
#define OPCODE_ID_REGISTER 12


//�Ű�� �������� ������ ��Ÿ���� �ڵ� �Դϴ�.
//packet�� ����ڵ尡 �ƴ϶� �Ű���� ������ ���� ���Ǵ� ����Դϴ�.
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

// char shortBuffer[BUFSIZE]; //�� 1MB   �Ѵ� ������� ����.
// char bigBuffer[BUFSIZE * 10]; //�� 100MB


//�ѹ��� ��Ʈ��ũ �������� ��� �����ϴ� packet ����ü�Դϴ�.
//���� n_opcode�� 0�� ��Ŷ�� �α��� �Ҷ� ���Ǵ� ��Ŷ�Դϴ�.
//n_opcode�� ���� ������ �Ʒ��� #define���� �����մϴ�. 
typedef struct _packet {
	int n_opcode;   //����ڵ�
	int n_size;     //���̷ε��� ����
	char* cptr_payload;//��ü ���̷ε带 ��Ÿ���� �������Դϴ�. 
	                   //�������� �̹Ƿ� �����ͷ� �����Ҵ��� �޵��� �߽��ϴ�.
}packet;

//��Ŷ���� ���������� ������ ��κ��� �������� �̹Ƿ� ������������ ����ü�� ������ �ʰ�...
//������ �Լ����� ������ �������� ����κ��� �����ϴ�... ����
//�ϴ� �Ʒð����� ��ġ ũ�⳪ ��ü �ݺ�Ƚ������ �����Ҷ��� ������ �����̹Ƿ� �̴� ����ü�� ��������ϴ�.
typedef struct _train_setting_payload {
	int n_batch_size;
	int n_eopch;
	int n_test_cycle;
}train_setting_payload;

SOCKET createSocket(char* ipaddr, int port);

//�⺻������ init_#######Packet�� ���� �Լ��� packet�� ����ü ������ �ּҸ� �����մϴ�.
//�׷��� init_######Packet �Լ����� ��ɿ� �´� opcode�� ���̷ε带 �����մϴ�.
//�׷��Ŀ� opcode, ���̷ε��� ����, ���̷ε带 init_packet �Լ��� �����ϴ� ����Դϴ�.
//���������� �̷��� ������ packet�� ����ü�� sendToServ�Լ��� �����ϸ� ���۵˴ϴ�.

//packet����ü�� �ʱ�ȭ�ϰ� �������� ���� �۽��� ���� �Լ��Դϴ�. 
void init_packet(packet* ptr, int opcode, int size, char* payload);
//�Ʒ��� init_####Packet �Լ��� �� ���������� �ϼ��� payload�� packet����ü�� �ִ� ����� �մϴ�.

void init_initialConnectionPacket(packet* pack);
//���� ������� ��Ŷ�� �ʱ�ȭ�ϴ� �Լ��Դϴ�.
//������� �̸��� ��û�մϴ�. �ϴ� ��� �����ϴ�...
//���� ������� �̸��� david �̶�� david�̶�� �̸���
//���� ������ ������ �����ϰ� �װ��� �Ű���� �����͸� �ֱ����� 
//������ ���� �Ѱ��� �����մϴ�. 

void init_createClassifierPacket(packet* pack);
//�з���(�Ű��)�� ������ �� ����ϴ� �Լ� �Դϴ�. 
//������ ������ �ϳ��� �Ű���� ���������� �Ŀ� 
//�Լ��� �����ؼ� �پ��� �Ű���� ������ �� ���� ���Դϴ�.
//�Լ������� ���� ������ �Ű���� �̸��� �䱸�մϴ�.
//���� �Ű���� �̸��� net �̶�� �Ѵٸ� ���������� 
//�Ű������ �����ϰ� �ִ� ������ �ټ��÷ο쿡�� ����ϴ�
//ckpt(check point)������ �����մϴ�. ex net.ckpt, net.ckpt.meta ... ���
//�� ���������� ����ڰ� ����ϰ��� �ϴ� �Ű���� �̸����� �����մϴ�.

void init_dataSendPacket(packet* pack);
//���������� �о���� ������ �����ϴ� �Լ��Դϴ�.
//�Լ������� ���̺� �ش��ϴ� �̸��� ���������� �䱸�մϴ�.
//���� cat�̶�� �̸��� cat.zip������ �䱸�մϴ�.
//���۵Ǹ� ���������� cat�̶�� ������ �����Ǹ� cat.zip������ �װ��� �����ϰ�
//������ ������ �Ŀ� �̹��� �ϳ��� �о� ���̰� 255�� ������ ��ó�� �Ͽ� vec.csv��� ������
//������ ������ �����ϴ�. �� �Ŀ� �Ʒýÿ� �� ����(cat, dog, bird)�
//���ԵǾ� �ִ� vec.csv ������ ��Ƽ� �ϳ��� ���Ϸ� ����� �н��մϴ�.
//���������� �� ������ ���� �ɸ��� ������ �̸� �ϳ��� ���μ����� ����Ʈ�� �۾��մϴ�.
//���� ���ÿ� �������� ���� ���ε尡 �����մϴ�. 
//�ٸ� ���� �������ϴ�. �̴� �Ŀ� �ٸ� ����� �� �ϼ��ϰ� ������ ���Դϴ�.. �̤� 

void init_prepareTrainingPacket(packet* pack);
//���� �н��� �Ű���� �̸��� �̸� ������ ������ ���ϵ��� �̹� �Ʒÿ� ����� 
//�н� �����͸� �����մϴ�. ���� ���� ����ϰ��� �ϴ� �Ű���� �̸��� net�̰�
//�н��� ����ϰ��� �ϴ� �����Ͱ� dog, cat, bird ��� �̸� ������ ������ �־�� �մϴ�..

void init_trainSettingPacket(packet* pack);
//�н��� ���� ������ �����ϴ� �Լ��Դϴ�. ���� �� �н� �ݺ� Ƚ����
//�ѹ��� �н��ϰ��� �ϴ� �������� ũ��(��ġũ��), ����ٴ� ���� �����ͷ�
//�н��� �� �ǰ� �ִ��� �������� ���� �����մϴ�.

void init_moduleRequestPacket(packet* pack);
//Ŭ���̾�Ʈ���� ������ ���� �н��ϰ� �ִ� �Ű���� ����� ��ȯ�� ��û�մϴ�.
//���� �������� �ʾҽ��ϴ�. 

void sendToServ(SOCKET clntsock, packet* pack);
//Ŭ���̾�Ʈ���� ������ packet�� ����ü�� �����ϴ� �Լ��Դϴ�.
//��� ������ �Ϸ�Ǿ� ��κл�Ȳ�� �����ϰ� �����մϴ�.
//�����ؾ� �� ���� �� �Լ��� �������� pack->cptr_payload�� �����Ҵ�� ����
//������ �ְ� �ִٴ� ���Դϴ�. ���� ���� ���̷ε忡 �Ҵ�� ����
//������ ���� �ʾƵ� �˴ϴ�. 

void recvFromServ(SOCKET clntsock, packet* ptr);
//����� �������� ���̽㿡���� ���� ������ �Ͽ��µ� Ŭ���̾�Ʈ������ ����
//������ Ȯ�ε��� �ʾҽ��ϴ�. 

//�Ʒ������� ���� createClassifier���� ���Ǵ� �� �Ű���� ������ �� ���Ǵ� 
//����ü���Դϴ�. �̴� �׻� ���������̱� ������ ����ü�� ����� �����Ͽ� �̷��� �Ͽ����ϴ�.
//�Ű���� �����ϱ� ���� ���̷ε带 ��Ÿ���� ����ü�Դϴ�. void�� �����Ͱ� 
//���� �Ҵ�� �ٸ� ####_payload ����ü�� �ּҸ� ����Ű���� �����߽��ϴ�.


typedef struct _payload {
	void* payload_arr[100];
}payload;

//��� ���� int���� n_code ������ �����ϴ�. 
//n_code�� ������ ������ CODE_####�� �Ҵ�˴ϴ�.
//�Է����� ��Ÿ���� ����ü �Դϴ�.
typedef struct _input_payload {
	int n_code;
	int n_features;  //�Է����� Ư¡ �����Դϴ�.
}input_payload;

//�ռ������� ��Ÿ���� ����ü �Դϴ�.
typedef struct _conv_payload {
	int n_code;
	int n_init_method;    //�ʱ�ȭ ����Դϴ�.
	float f_mean;         //����Դϴ�.
	float f_std;          //ǥ�������Դϴ�.
	int n_filter_size;    //������ ũ���Դϴ�. (���� ���� �����ϴ� ����)
	int n_strides_size;   //������ ũ���Դϴ�. (���� ���� �����ϴ� ����)
	int n_padding_method; //�е��� ����Դϴ�. �����е�, �е� �������� ������ �ֽ��ϴ�.
	int n_outputs;        //������ ����, �Ǵ� ����� �����Դϴ�.
	float f_bias;         //�ռ������� �Ŀ� �������� ������ ���Դϴ�.
}conv_payload;

//Ǯ������ ��Ÿ���� ����ü�Դϴ�.
typedef struct _pooling_payload {
	int n_code;
	int n_filter_size;    //���� �Ǵ� Ŀ���� ũ���Դϴ�. 
	int n_strides;        //������ ũ�� �Դϴ�.
	int n_padding_method; //�е��� ����Դϴ�. �����е�, �е� �������� ������ �ֽ��ϴ�.
	int n_pooling_method; //Ǯ�� ����Դϴ�. �ִ� Ǯ��, ��� Ǯ���� �ֽ��ϴ�.
}pooling_payload;

//������������� ��Ÿ���� ����ü�Դϴ�.
typedef struct _fully_connect_payload {
	int n_code;
	int n_init_method;    //����ġ �ʱ�ȭ ����Դϴ�.
	float f_mean;         //����Դϴ�. 
	float f_std;          //ǥ�������Դϴ�.
	int n_outputs;        //������ ����, �Ǵ� ����� �����Դϴ�.
	float f_bias;         //������ ���Դϴ�.
}fully_connect_payload;

//Ȱ��ȭ �Լ� ������ ��Ÿ���� ����ü�Դϴ�.
typedef struct _activation_payload {
	int n_code;
	int n_activation_method;//Ȱ��ȭ �Լ��� ��Ÿ���ϴ�.
}activation_payload;

//��ġ ����ȭ ������ ��Ÿ���� ����ü �Դϴ�.
typedef struct _batch_norm_payload {
	int n_code;
	float f_momentum;       //����� ������ ����ϳ� �Ϲ������� 0.9�� �����Դϴ�.
}batch_norm_payload;

//��Ӿƿ� ������ ��Ÿ���� ����ü �Դϴ�.
typedef struct _dropout_payload {
	int n_code;
	float f_ratio;          //��Ӿƿ� ������ ��Ÿ���ϴ�. �Ϲ������� 0.5�� ����մϴ�.
}dropout_payload;

//���(�з�) ������ ��Ÿ���� ����ü �Դϴ�.
typedef struct _output_payload {
	int n_code;
	int n_is_onehot;        //���� ���ڵ� ���θ� ��Ÿ���� �����̳� ���� �������� �ֽ��ϴ�.
	int n_class_num;        //�з������� ��Ÿ���� �����Դϴ�.
}output_payload;

//��Ƽ�������� �������ִ� ����ü �Դϴ�. ���� ���� �ʿ��Ͽ� �ַ� �з� ���� �Ŀ� �ɴϴ�. 
typedef struct _optimizer_payload {
	int n_code;
	int n_optimizer;
	float f_momentum;       //����� ����ȭ���� ���Ǵ� �����Դϴ�. �� �̿ܿ��� ������ �ʽ��ϴ�.
	float f_ratio;          //�н����� ��Ÿ���� �����Դϴ�. �Ϲ������� 0.01�� ���˴ϴ�.
}optimizer_payload;

//�������� ��Ÿ���� ����ü�Դϴ�. ������ n_code�� -99�� ���� �ʿ��Ͽ� �߰��Ͽ����ϴ�.
typedef struct _end_payload {
	int n_code;
}end_payload;

// ������ ���͸� �������� ���̾�α� ����
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


//init_####_payload �Լ��ø���� �ش��ϴ� ����ü�� ���� �����Ͽ� 
//�ʿ��� ���ڷ� ���� �ʿ��� ���� �Ҵ����ְ� void* �����ͷ� ����ȯ�Ͽ� ��ȯ�ϴ� �Լ��Դϴ�.

//########  init_input_payload�Լ��� init_output_payload �Լ��� �ݵ�� ���ڸ� �־�� �մϴ�!  ##########
//�Ŀ� �� �Լ����� ���ڵ��� ��� ���� ������ ������ �ϴ����� ���߿� �߰��ϰڽ��ϴ�.
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
// //payload�� ��� void* �����Ϳ� �����Ҵ�� �ּҸ� �������ִ� �Լ��Դϴ�.
// void free_payload(payload* ptr_payload) {
// 	int i;
// 	for (i = 0; i < 100; i++) {
// 		int n_code = *((int*)(ptr_payload->payload_arr[i]));
// 		free(ptr_payload->payload_arr[i]);
// 		if (n_code == CODE_END) break;
// 	}
// }
// 
// //���̷ε��� ��ü ���̸� ���ϴ� �Լ��Դϴ�.
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
// 			printf("copy_payload_to_chararr���� ����! i �� %d \n", i);
// 		}
// 
// 	}
// 	return chararr_idx;
// }

//������ �ϼ��� payload�� char�� �迭�� �Է����� �޽��ϴ�.
//char�� �迭�� �������� ������ ���� �����Ҵ�� ����Ʈ �迭�̶�� �����߽��ϴ�.
//payload�� void* �����Ͱ� ����Ű�� ����ü�� ������ �ľ��ؼ�
//����ȯ�� �� �Ŀ� char �迭�� ����ü�� ���̸�ŭ memcpy�� ���ݴϴ�.
//�������� ������ ���縦 �����մϴ�.
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
// 			printf("copy_payload_to_chararr���� ����! i �� %d \n",i);
// 		}
// 		
// 	}
// 	return chararr_idx;
// }


//init_#####Packet �Լ��� �̿��ؼ� packet �� ����ü�� �����մϴ�.
//init_####PAcket �Լ������� init_packet �Լ��� ȣ�� �մϴ�.
//payloadSize������ ��ü ���̷ε��� ���̸� ��Ÿ���� 
//pack.n_size �� �Ҵ�� ���Դϴ�. �� payload�� �����Դϴ�.
//payloadSize ������ �������� char* �� allPayload������ ����
//���� payloadSize��ŭ ���� �Ҵ��� �޽��ϴ�.
//���� payload�� �� �κи��� ������ allPayload ������ memcpy���ְ�
//opcode, payloadSize, allPayload������ init_packet �Լ��� ȣ���մϴ�.
//��� �̷������� �����Ϸ� �մϴ�.

// void init_createClassifierPacket(packet* pack) {
// 	int opcode = 1, namelen = 0;
// 	int payloadSize = 0; //��ü ���̷ε��� ����
// 	char namebuf[100];
// 	char* name = NULL;
// 	printf("�Ű���� �̸�<<");
// 	scanf("%s", namebuf);
// 	namelen = strlen(namebuf) +1;
// 	name = (char*)malloc(namelen);
// 	strcpy_s(name, namelen, namebuf);
// 	printf("�Ű���� �̸�>> %s\n", name);
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
// 	char* allPayload = (char*)malloc(payloadSize * sizeof(char));  //���̷ε� ���� + ���̷ε�
// 	
// 	memcpy(&allPayload[0], &namelen, sizeof(int));  //�Ű���� �̸����̸� ����
// 	memcpy(&allPayload[sizeof(namelen)], name, namelen);//�Ű���� �̸��� ����
// 	int n = copy_payload_to_chararr(&pay_load, &allPayload[sizeof(namelen) + namelen]);//�Ű���� ���������� ����
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
	printf("���� �������� ����ó���� ���� �ʾƼ� �ݵ�� ����� �̸��� �Է��ϰ� ����ؾ� �մϴ�!!\n");
	printf("������� �̸�<<");
	scanf("%s", buf);
	len = strlen(buf) + 1;
	cptr = (char*)malloc(len+sizeof(len));
	memcpy(cptr, &len, sizeof(len));
	memcpy(&cptr[sizeof(len)], buf, len);
	//strcpy_s(cptr, len, buf);
	printf("������: ������� �̸�>> %s\n", &cptr[sizeof(len)]);
	printf("������: �̸��� ����>> %d\n", len);

	// ���̷ε��� ���̴� ������� �̸� ���̸� ��Ÿ���� int(4) + ������� �̸��� ����
	init_packet(pack, opcode, sizeof(len) + len, cptr);

}

// void init_dataSendPacket(packet* pack) {
// 	int opcode = 2, payloadSize = 0, namelen = 0, zipSize = 0, statResult;
// 	char namebuf[100], zipbuf[100], *name = NULL, *allPayload = NULL;
// 	struct _stat statbuf;
// 	FILE* fp = NULL;
// 	printf("�̹����� �̸� �Է�(ex. cat)<<");
// 	scanf("%s", namebuf);
// 	namelen = strlen(namebuf) + 1;
// 	payloadSize += sizeof(namelen) + namelen;
// 
// 	name = (char*)malloc(namelen);
// 	strcpy_s(name, namelen, namebuf);
// 
// 	printf("���������� �̸��� �Է�(ex. cat.zip)<<");
// 	scanf("%s", zipbuf);
// 	// �̰��� ���������� �ε��ϴ� ����� ��������
// 	statResult = _stat(zipbuf, &statbuf);
// 	if (statResult != 0) {
// 		printf("stat error");
// 		printf("statResult: %d \n", statResult);
// 	}
// 	else {
// 		printf("������: filesize : %d \n", statbuf.st_size);
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

	std::cout << "�з��� �����Դϴ�>>";
	scanf("%s", namebuf);
	namebuflen = strlen(namebuf) + 1;
	payloadSize += sizeof(namebuflen) + namebuflen;

	allPayload = (char*)malloc(payloadSize);
	memcpy(allPayload, &namebuflen, sizeof(namebuflen));
	memcpy(&allPayload[sizeof(namebuflen)], namebuf, namebuflen);

	while (1) {
		std::cout << "�Ʒ� �����͸� �Է��ϼ��� -1 �Է½� �����մϴ� >>";
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
	std::cout << "��ġ����� �Է��ϼ���>>";
	scanf("%d", &(ptr->n_batch_size));
	std::cout << "�� �ݺ�Ƚ���� �Է��ϼ���>>";
	scanf("%d", &(ptr->n_eopch));
	std::cout << "���� �ֱ⸦ �Է��ϼ���>>";
	scanf("%d", &(ptr->n_test_cycle));
	payloadSize += sizeof(train_setting_payload);
	init_packet(pack, opcode, payloadSize, (char*)ptr);
}

void init_moduleRequestPacket(packet* pack) {
	int opcode = OPCODE_REQUESTMODULE;
	int payloadSize = 0,namelen = 0;
	char buf[100];
	char* allPayload = NULL;
	std::cout << "��û�� �Ű���� �̸��� �Է��ϼ���>>";
	scanf("%s", buf);
	namelen = strlen(buf) + 1;
	payloadSize += sizeof(namelen);
	payloadSize += namelen;
	allPayload = (char*)malloc(payloadSize);
	memcpy(allPayload, &namelen, sizeof(namelen));
	memcpy(&allPayload[sizeof(namelen)], buf, namelen);
	init_packet(pack, opcode, payloadSize, allPayload);
}


//����ڷκ��� ���� zip������ �����ϰ� �����Ҵ�� �޸𸮸� �������ִ� �Լ��Դϴ�.
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

//�ϴ� ������ �ʴ� �ڵ� �Դϴ�.
// void sendToServWORKING(SOCKET clntsock, packet* pack) {
// 	//buf�� ���� �Ҵ����� ���� 8����Ʈ�� �����Ŀ� pack->cptr_payload�� �����غ���.
// 	int remain = 0;
// 	//opcode�� payload�� ���� payload�� �����ϴ� ũ��
// 	//�� Ȥ�� �޸� ������ �Ͼ��? ���� �׷��ٸ� �̰����� ���� �Ҵ��� ���� ���ƾ� �ұ�?
// 	//�����Ҵ� ���� �ʴ°� ���� �ƴѰ� ����. �޸� ������ �Ͼ���� Ȯ���غ���.
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
// 	//���⼭ ������ ���� ������ ���� ���۵��� ���� ���� ���� �ϱ� �����ΰ�?
// 	//�̰��� ���� �Ҵ� ���۸� ������ ���� ���ÿ� ���۸� ����� �ΰ� �غ���.
// }
// 
//���� ���Ǵ� �ڵ��Դϴ�.
/*
void sendToServ(SOCKET clntsock, packet* pack) {
	int remain = 0;

	remain = pack->n_size;
	send(clntsock, (char*)pack, sizeof(int) * 2, 0);
	//��Ŷ�� opcode�� ���̷ε��� ����(n_size)�� ���� �����մϴ�.
	//�̴� �������� ����ڵ带 �а� � ������� Ȯ���� �� �ְ�
	//�ڿ� n_size�� �о�鿩 ���̷ε��� ���̸� Ȯ���ϰ�
	//���̷ε��� ���̸�ŭ�� �о���̱� �����Դϴ�.


	//������ ���̷ε��� ���̰� BUFSIZE���� ������ �ѹ��� ������
	//�׷��� ������ ������ �޸� ������ �����ϰ� �������� 
	//��� �׳� send(clntsock, pack->cptr_payload, pack->n_size, 0);
	//�ѹ��̸� �� �������� �����ϴ�. �̴� ���߿� ������ ������ �մϴ�.
	send(clntsock, pack->cptr_payload, pack->n_size, 0);
	free(pack->cptr_payload);


	/*
	if (remain < BUFSIZE) {
		send(clntsock, pack->cptr_payload, pack->n_size, 0);
		free(pack->cptr_payload);
	}
	else {
		//���� �κ��� ������ �� ���� �ִ�. 
		//bifBuffer�� ������� �ʰ� cptr_payload�� �����ϴ� �ڵ�� �ٽ� ��������
		//���ۿ� �ø��°� �߶� �ø��� �ʾƵ� ���� �ʴ°�? �׳� �ѹ��� �� ������ �ɰ� ����.
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
//����� ������ �ʴ� �ڵ��Դϴ�. 
void sendToServ_ORIGINAL(SOCKET clntsock, packet* pack) {

	//���� �����Ҵ� �ް� pack�� ��� ������ char������ ����ٿ��ֱ� �Ѵ�.
	//���� �������� �����Ѵ�. 
	//�� �Լ��� �������� payload�� free�� �ʱ�ȭ �Ѵ�.
	int totLen, curLen, payloadLen, remain;
	totLen = curLen = payloadLen = remain = 0;

	payloadLen = pack->n_size;
	char* buf = (char*)malloc(pack->n_size + 2 * sizeof(int));
	//opcode�� payload�� ���� payload�� �����ϴ� ũ��
	//�� Ȥ�� �޸� ������ �Ͼ��? ���� �׷��ٸ� �̰����� ���� �Ҵ��� ���� ���ƾ� �ұ�?
	//�����Ҵ� ���� �ʴ°� ���� �ƴѰ� ����. �޸� ������ �Ͼ���� Ȯ���غ���.
	remain = sizeof(int) * 2 + payloadLen;

	//memcpy�� Ȱ���ؼ� �� ���簡 �ƴ϶� �޸𸮸� ������ ���� �ӵ��� ������ ����
	memcpy(&buf[0], pack, sizeof(int) * 2); //����ڵ�� ����� ����� ���� �����
	memcpy(&buf[sizeof(int) * 2], pack->cptr_payload, payloadLen); //�Ⱑ������ �� ����ȴ�.

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
	free(buf); //���⼭ pack->cptr_payload�� buf�� memcpy�Ѱ͵� ���� free ��Ű�Ƿ� ��� ���� �Ǵ°� ����.
}

/*
//��ɽ����� �ʿ��մϴ�.
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

	//��� ���⼭ �ݵ�� 8����Ʈ�� �о���� �� �����Ŷ�� ������ ����.
	ptr->cptr_payload = (char*)malloc(ptr->n_size);
	if (ptr->cptr_payload == NULL) {
		printf("recvFromServ���� malloc ���� \n");
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


//���� ������ �ʴ� �Լ��Դϴ�.
void send_chararr(char chararr[],int arrlen = 1024) {
	SOCKET clientsock;
	WSADATA wsa;
	struct sockaddr_in sockinfo;
	char message[30];
	int len;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
		printf("�ʱ�ȭ ����\n");

	clientsock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

	if (clientsock == INVALID_SOCKET)
		printf("���� ���� ����\n");

	memset(&sockinfo, 0, sizeof(sockinfo));

	sockinfo.sin_family = AF_INET;
	sockinfo.sin_port = htons(10000);
	sockinfo.sin_addr.s_addr = inet_addr("127.0.0.1");

	if (connect(clientsock, (SOCKADDR*)&sockinfo, sizeof(sockinfo)) == SOCKET_ERROR)
		printf(" ���� ���� ���� ");

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

//��������� �ʴ� �Լ��Դϴ�.
void show(void* ptr[]) {
	input_payload* iptr = (input_payload*)ptr[0];
	conv_payload* cptr = (conv_payload*)ptr[1];
	printf("%d %d \n", iptr->n_code, iptr->n_features);
	printf("%d %f %f %d %d %d %d %d %f", cptr->n_code, cptr->n_init_method, cptr->f_mean, cptr->f_std, cptr->n_filter_size
		, cptr->n_strides_size, cptr->n_padding_method, cptr->n_outputs, cptr->f_bias);
}

//���� ������ �ʴ� �Լ��Դϴ�.
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



//���� main�Լ��Դϴ�.
//������ �ʱ�ȭ�ϰ� ������ ������ �Ŀ� �ݺ����� ���� ����� �����մϴ�.
void testmain3() {
	SOCKET clientsock;
	WSADATA wsa;
	struct sockaddr_in sockinfo;
	char message[30];
	int len;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
		printf("�ʱ�ȭ ����\n");

	clientsock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

	if (clientsock == INVALID_SOCKET)
		printf("���� ���� ����\n");

	memset(&sockinfo, 0, sizeof(sockinfo));

	sockinfo.sin_family = AF_INET;
	sockinfo.sin_port = htons(10000);
	sockinfo.sin_addr.s_addr = inet_addr("127.0.0.1");

	if (connect(clientsock, (SOCKADDR*)&sockinfo, sizeof(sockinfo)) == SOCKET_ERROR)
		printf(" ���� ���� ���� ");


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