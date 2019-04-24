#include "stdafx.h"
#include "payload.h"
#pragma warning(disable : 4996)

SOCKET createSocket(char* ipaddr, int port)
{
	// 소켓 생성
	WSADATA wsa;
	SOCKET clientsock;
	struct sockaddr_in sockinfo;
	char message[30];
	int len;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
		exit(-1);

	clientsock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

	if (clientsock == INVALID_SOCKET)
		exit(-1);

	memset(&sockinfo, 0, sizeof(sockinfo));

	sockinfo.sin_family = AF_INET;
	sockinfo.sin_port = htons(port);
	sockinfo.sin_addr.s_addr = inet_addr(ipaddr);

	if (connect(clientsock, (SOCKADDR*)&sockinfo, sizeof(sockinfo)) == SOCKET_ERROR)
		exit(-1);

	return clientsock;
}

//현재 사용되는 코드입니다.
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
	*/

}


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
}

void GetDirPathDialog(CString* path, HWND targetWnd, LPCWSTR title)
{
	BROWSEINFO BrInfo;
	TCHAR szBuffer[512];                                      // 경로저장 버퍼 

	::ZeroMemory(&BrInfo, sizeof(BROWSEINFO));
	::ZeroMemory(szBuffer, 512);

	BrInfo.hwndOwner = targetWnd;
	BrInfo.lpszTitle = title;
	BrInfo.ulFlags = BIF_NEWDIALOGSTYLE | BIF_EDITBOX | BIF_RETURNONLYFSDIRS;
	LPITEMIDLIST pItemIdList = ::SHBrowseForFolder(&BrInfo);
	::SHGetPathFromIDList(pItemIdList, szBuffer);             // 파일경로 읽어오기
	if (*szBuffer == NULL)
		return;

	(*path).Format(_T("%s"), szBuffer);
}

void init_dataSendPacket(packet* pack) {
	int opcode = 2, payloadSize = 0, namelen = 0, zipSize = 0, statResult;
	char namebuf[100], zipbuf[100], *name = NULL, *allPayload = NULL;
	struct _stat statbuf;
	FILE* fp = NULL;
	printf("이미지의 이름 입력(ex. cat)<<");
	scanf("%s", namebuf);
	namelen = strlen(namebuf) + 1;
	payloadSize += sizeof(namelen) + namelen;

	name = (char*)malloc(namelen);
	strcpy_s(name, namelen, namebuf);

	printf("압축파일의 이름을 입력(ex. cat.zip)<<");
	scanf("%s", zipbuf);
	// 이곳에 압축파일을 로드하는 기능을 구현하자
	statResult = _stat(zipbuf, &statbuf);
	if (statResult != 0) {
		printf("stat error");
		printf("statResult: %d \n", statResult);
	}
	else {
		printf("디버깅용: filesize : %d \n", statbuf.st_size);
		payloadSize += statbuf.st_size;

	}
	allPayload = (char*)malloc(payloadSize);

	memcpy(&allPayload[0], &namelen, sizeof(namelen));
	memcpy(&allPayload[sizeof(namelen)], name, namelen);

	fp = fopen(zipbuf, "rb");
	fread(&allPayload[sizeof(namelen) + namelen], statbuf.st_size, 1, fp);
	fclose(fp);
}

void init_createClassifierPacket(packet* pack) {
	int opcode = 1, namelen = 0;
	int payloadSize = 0; //전체 페이로드의 길이
	char namebuf[100];
	char* name = NULL;
	printf("신경망의 이름<<");
	scanf("%s", namebuf);
	namelen = strlen(namebuf) + 1;
	name = (char*)malloc(namelen);
	strcpy_s(name, namelen, namebuf);
	printf("신경망의 이름>> %s\n", name);

	payloadSize += (sizeof(namelen) + namelen);

	// payload pay_load;
	// pay_load.payload_arr[0] = init_input_payload(3072);
	// //	payloadSize += sizeof(input_payload);
	// 
	// pay_load.payload_arr[1] = init_conv_payload();
	// //	payloadSize += sizeof(conv_payload);
	// 
	// pay_load.payload_arr[2] = init_activation_payload();
	// //payloadSize += sizeof(activation_payload);
	// 
	// pay_load.payload_arr[3] = init_conv_payload();
	// //payloadSize += sizeof(conv_payload);
	// 
	// pay_load.payload_arr[4] = init_activation_payload();
	// //payloadSize += sizeof(activation_payload);
	// 
	// pay_load.payload_arr[5] = init_fully_connected_payload();
	// //payloadSize + sizeof(fully_connect_payload);
	// 
	// pay_load.payload_arr[6] = init_activation_payload();
	// //payloadSize += sizeof(activation_payload);
	// 
	// pay_load.payload_arr[7] = init_fully_connected_payload();
	// //payloadSize += sizeof(fully_connect_payload);
	// 
	// pay_load.payload_arr[8] = init_activation_payload();
	// 
	// pay_load.payload_arr[9] = init_output_payload(3);
	// //payloadSize += sizeof(output_payload);
	// 
	// pay_load.payload_arr[10] = init_optimizer_payload();
	// //payloadSize += sizeof(optimizer_payload);
	// 
	// pay_load.payload_arr[11] = init_end_payload();
	// //payloadSize += sizeof(end_payload);

	// int calculatelen = calculate_payload_len(&pay_load);
	// payloadSize += calculatelen;
	// char* allPayload = (char*)malloc(payloadSize * sizeof(char));  //페이로드 길이 + 페이로드
	// 
	// memcpy(&allPayload[0], &namelen, sizeof(int));  //신경망의 이름길이를 전달
	// memcpy(&allPayload[sizeof(namelen)], name, namelen);//신경망의 이름을 전달
	// int n = copy_payload_to_chararr(&pay_load, &allPayload[sizeof(namelen) + namelen]);//신경망의 구성정보를 전달
	// 
	// 
	// free_payload(&pay_load);
	// 
	// 
	// printf("n: %d, calcuatelen: %d,  all: %d \n\n", n, calculatelen, payloadSize); //192 - 8  168
	// init_packet(pack, opcode, payloadSize, allPayload);
}

//init_####_payload 함수시리즈는 해당하는 구조체를 동적 생성하여 
//필요한 인자로 받은 필요한 값을 할당해주고 void* 포인터로 형변환하여 반환하는 함수입니다.

//########  init_input_payload함수와 init_output_payload 함수는 반드시 인자를 주어야 합니다!  ##########
//후에 각 함수에서 인자들이 어느 값의 범위를 가져야 하는지를 나중에 추가하겠습니다.

void* init_input_payload(int n_features) {
	input_payload* iptr = (input_payload*)malloc(sizeof(input_payload));
	iptr->n_code = CODE_INPUT;
	iptr->n_features = n_features;

	return (void*)iptr;
}

void* init_conv_payload(int n_init_method, float f_mean, float f_std
	, int n_filtersize, int n_stridesize, int n_padding_method
	, int n_outputs, float f_bias) {
	conv_payload* cptr = (conv_payload*)malloc(sizeof(conv_payload));
	cptr->n_code = CODE_CONV;
	cptr->n_init_method = n_init_method;
	cptr->f_mean = f_mean;
	cptr->f_std = f_std;
	cptr->n_filter_size = n_filtersize;
	cptr->n_strides_size = n_stridesize;
	cptr->n_padding_method = n_padding_method;
	cptr->n_outputs = n_outputs;
	cptr->f_bias = f_bias;

	return (void*)cptr;
}

void* init_pooling_payload(int n_filter_size, int n_stridesize,
	int n_padding_method, int n_pooling_method) {
	pooling_payload* ptr = (pooling_payload*)malloc(sizeof(pooling_payload));
	ptr->n_code = CODE_POOL;
	ptr->n_filter_size = n_filter_size;
	ptr->n_strides = n_stridesize;
	ptr->n_padding_method = n_padding_method;
	ptr->n_pooling_method = n_pooling_method;

	return (void*)ptr;
}

void* init_fully_connected_payload(int n_init_method, float f_mean,
	float f_std, int n_outputs, float f_bias) {
	fully_connect_payload* ptr = (fully_connect_payload*)malloc(sizeof(fully_connect_payload));
	ptr->n_code = CODE_FULLY;
	ptr->n_init_method = n_init_method;
	ptr->f_mean = f_mean;
	ptr->f_std = f_std;
	ptr->n_outputs = n_outputs;
	ptr->f_bias = f_bias;

	return (void*)ptr;
}

void* init_activation_payload(int n_activation_method) {
	activation_payload* ptr = (activation_payload*)malloc(sizeof(activation_payload));
	ptr->n_code = CODE_ACTIV;
	ptr->n_activation_method = n_activation_method;

	return (void*)ptr;
}

void* init_batchnorm_payload(float f_momentum) {
	batch_norm_payload* ptr = (batch_norm_payload*)malloc(sizeof(batch_norm_payload));
	ptr->n_code = CODE_BATCH;
	ptr->f_momentum = f_momentum;

	return (void*)ptr;
}

void* init_dropout_payload(float f_ratio) {
	dropout_payload* ptr = (dropout_payload*)malloc(sizeof(dropout_payload));
	ptr->n_code = CODE_DROP;
	ptr->f_ratio = f_ratio;

	return (void*)ptr;
}

void* init_output_payload(int n_class_num, int n_onehot) {
	output_payload* ptr = (output_payload*)malloc(sizeof(output_payload));
	ptr->n_code = CODE_OUTP;
	ptr->n_is_onehot = n_onehot;
	ptr->n_class_num = n_class_num;

	return (void*)ptr;
}

void* init_optimizer_payload(int n_optimizer_method,
	float f_learning_rate, float f_momentum) {
	optimizer_payload* ptr = (optimizer_payload*)malloc(sizeof(optimizer_payload));
	ptr->n_code = CODE_OPTMZ;
	ptr->n_optimizer = n_optimizer_method;
	ptr->f_ratio = f_learning_rate;
	ptr->f_momentum = f_momentum;

	return (void*)ptr;
}

void* init_end_payload() {
	end_payload* ptr = (end_payload*)malloc(sizeof(end_payload));
	ptr->n_code = CODE_END;

	return (void*)ptr;
}

//payload의 모든 void* 포인터에 동적할당된 주소를 해제해주는 함수입니다.
void free_payload(payload* ptr_payload) {
	int i;
	for (i = 0; i < 100; i++) {
		int n_code = *((int*)(ptr_payload->payload_arr[i]));
		if (n_code == CODE_END)
		{
			free(ptr_payload->payload_arr[i]);
			break;
		}
		free(ptr_payload->payload_arr[i]);
	}
}

//페이로드의 전체 길이를 구하는 함수입니다.
int calculate_payload_len(payload* ptr_payload) {
	int i, chararr_idx = 0;
	for (i = 0; i < 100; i++) {
		int* p_code = (int*)(ptr_payload->payload_arr[i]);

		if ((*p_code) == CODE_INPUT) {
			input_payload* ptr = (input_payload*)(ptr_payload->payload_arr[i]);
			//memcpy(&chararr[chararr_idx], ptr, sizeof(input_payload));
			chararr_idx += sizeof(input_payload);

			//*payloadSize += sizeof(input_payload);
		}
		else if ((*p_code) == CODE_CONV) {
			conv_payload* ptr = (conv_payload*)(ptr_payload->payload_arr[i]);
			//memcpy(&chararr[chararr_idx], ptr, sizeof(conv_payload));
			chararr_idx += sizeof(conv_payload);

			//*payloadSize += sizeof()
		}
		else if ((*p_code) == CODE_POOL) {
			pooling_payload* ptr = (pooling_payload*)(ptr_payload->payload_arr[i]);
			//memcpy(&chararr[chararr_idx], ptr, sizeof(pooling_payload));
			chararr_idx += sizeof(pooling_payload);
		}
		else if ((*p_code) == CODE_FULLY) {
			fully_connect_payload* ptr = (fully_connect_payload*)(ptr_payload->payload_arr[i]);
			//memcpy(&chararr[chararr_idx], ptr, sizeof(fully_connect_payload));
			chararr_idx += sizeof(fully_connect_payload);
		}
		else if ((*p_code == CODE_ACTIV)) {
			activation_payload* ptr = (activation_payload*)(ptr_payload->payload_arr[i]);
			//memcpy(&chararr[chararr_idx], ptr, sizeof(activation_payload));
			chararr_idx += sizeof(activation_payload);
		}
		else if ((*p_code) == CODE_BATCH) {
			batch_norm_payload* ptr = (batch_norm_payload*)(ptr_payload->payload_arr[i]);
			//memcpy(&chararr[chararr_idx], ptr, sizeof(batch_norm_payload));
			chararr_idx += sizeof(batch_norm_payload);
		}
		else if ((*p_code) == CODE_DROP) {
			dropout_payload* ptr = (dropout_payload*)(ptr_payload->payload_arr[i]);
			//memcpy(&chararr[chararr_idx], ptr, sizeof(dropout_payload));
			chararr_idx += sizeof(dropout_payload);
		}
		else if ((*p_code) == CODE_OUTP) {
			output_payload* ptr = (output_payload*)(ptr_payload->payload_arr[i]);
			//memcpy(&chararr[chararr_idx], ptr, sizeof(output_payload));
			chararr_idx += sizeof(output_payload);
		}
		else if ((*p_code) == CODE_OPTMZ) {
			optimizer_payload* ptr = (optimizer_payload*)(ptr_payload->payload_arr[i]);
			//memcpy(&chararr[chararr_idx], ptr, sizeof(optimizer_payload));
			chararr_idx += sizeof(optimizer_payload);
		}
		else if ((*p_code) == CODE_END) {
			end_payload* ptr = (end_payload*)(ptr_payload->payload_arr[i]);
			//memcpy(&chararr[chararr_idx], ptr, sizeof(end_payload));
			chararr_idx += sizeof(end_payload);

			break;
		}
		else {
			printf("copy_payload_to_chararr에서 에러! i 는 %d \n", i);
		}

	}
	return chararr_idx;
}

int copy_payload_to_chararr(payload* ptr_payload, char* chararr) {
	int i, chararr_idx = 0;
	for (i = 0; i < 100; i++) {
		int* p_code = (int*)(ptr_payload->payload_arr[i]);

		if ((*p_code) == CODE_INPUT) {
			input_payload* ptr = (input_payload*)(ptr_payload->payload_arr[i]);
			memcpy(&chararr[chararr_idx], ptr, sizeof(input_payload));
			chararr_idx += sizeof(input_payload);

			//*payloadSize += sizeof(input_payload);
		}
		else if ((*p_code) == CODE_CONV) {
			conv_payload* ptr = (conv_payload*)(ptr_payload->payload_arr[i]);
			memcpy(&chararr[chararr_idx], ptr, sizeof(conv_payload));
			chararr_idx += sizeof(conv_payload);

			//*payloadSize += sizeof()
		}
		else if ((*p_code) == CODE_POOL) {
			pooling_payload* ptr = (pooling_payload*)(ptr_payload->payload_arr[i]);
			memcpy(&chararr[chararr_idx], ptr, sizeof(pooling_payload));
			chararr_idx += sizeof(pooling_payload);
		}
		else if ((*p_code) == CODE_FULLY) {
			fully_connect_payload* ptr = (fully_connect_payload*)(ptr_payload->payload_arr[i]);
			memcpy(&chararr[chararr_idx], ptr, sizeof(fully_connect_payload));
			chararr_idx += sizeof(fully_connect_payload);
		}
		else if ((*p_code == CODE_ACTIV)) {
			activation_payload* ptr = (activation_payload*)(ptr_payload->payload_arr[i]);
			memcpy(&chararr[chararr_idx], ptr, sizeof(activation_payload));
			chararr_idx += sizeof(activation_payload);
		}
		else if ((*p_code) == CODE_BATCH) {
			batch_norm_payload* ptr = (batch_norm_payload*)(ptr_payload->payload_arr[i]);
			memcpy(&chararr[chararr_idx], ptr, sizeof(batch_norm_payload));
			chararr_idx += sizeof(batch_norm_payload);
		}
		else if ((*p_code) == CODE_DROP) {
			dropout_payload* ptr = (dropout_payload*)(ptr_payload->payload_arr[i]);
			memcpy(&chararr[chararr_idx], ptr, sizeof(dropout_payload));
			chararr_idx += sizeof(dropout_payload);
		}
		else if ((*p_code) == CODE_OUTP) {
			output_payload* ptr = (output_payload*)(ptr_payload->payload_arr[i]);
			memcpy(&chararr[chararr_idx], ptr, sizeof(output_payload));
			chararr_idx += sizeof(output_payload);
		}
		else if ((*p_code) == CODE_OPTMZ) {
			optimizer_payload* ptr = (optimizer_payload*)(ptr_payload->payload_arr[i]);
			memcpy(&chararr[chararr_idx], ptr, sizeof(optimizer_payload));
			chararr_idx += sizeof(optimizer_payload);
		}
		else if ((*p_code) == CODE_END) {
			end_payload* ptr = (end_payload*)(ptr_payload->payload_arr[i]);
			memcpy(&chararr[chararr_idx], ptr, sizeof(end_payload));
			chararr_idx += sizeof(end_payload);

			break;
		}
		else {
			printf("copy_payload_to_chararr에서 에러! i 는 %d \n", i);
		}

	}
	return chararr_idx;
}