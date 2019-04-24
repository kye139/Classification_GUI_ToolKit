// PropertyCNNDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "ImageTool.h"
#include "PropertyCNNDlg.h"
#include "afxdialogex.h"
#include "HiddenPropertyDlg.h"
#include "NetNameDlg.h"
#include "MainFrm.h"

#define MAX_LAYER_SIZE 10000


// CPropertyCNNDlg 대화 상자입니다.

IMPLEMENT_DYNAMIC(CPropertyCNNDlg, CDialogEx)

CPropertyCNNDlg::CPropertyCNNDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_PROPERTY_CNN_DLG, pParent)
{

}

CPropertyCNNDlg::~CPropertyCNNDlg()
{
}

void CPropertyCNNDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_INPUT_LAYOUT, m_InputButton);
	DDX_Control(pDX, IDC_OUTPUT_LAYER, m_OutputButton);
	DDX_Control(pDX, IDC_OPTIMIZATION, m_OptimizationButton);
	DDX_Control(pDX, IDC_INPUT_TEXT, m_InputText);
	DDX_Control(pDX, IDC_OUTPUT_TEXT, m_OutputText);
	DDX_Control(pDX, IDC_OPTIMIZE_TEXT, m_OptimizeText);
	DDX_Control(pDX, IDC_INPUT_LAYOUT, m_InputButton);
	DDX_Control(pDX, IDC_OUTPUT_LAYER, m_OutputButton);
	DDX_Control(pDX, IDC_OPTIMIZATION, m_OptimizationButton);
}


BEGIN_MESSAGE_MAP(CPropertyCNNDlg, CDialogEx)
	ON_BN_CLICKED(IDC_ADD_BUTTON, &CPropertyCNNDlg::OnBnClickedAddButton)
	ON_BN_CLICKED(IDC_DEL_BUTTON, &CPropertyCNNDlg::OnBnClickedDelButton)
	ON_BN_CLICKED(IDC_INPUT_LAYOUT, &CPropertyCNNDlg::OnBnClickedInputLayout)
	ON_BN_CLICKED(IDC_OUTPUT_LAYER, &CPropertyCNNDlg::OnBnClickedOutputLayer)
	ON_BN_CLICKED(IDC_OPTIMIZATION, &CPropertyCNNDlg::OnBnClickedOptimization)
END_MESSAGE_MAP()


// CPropertyCNNDlg 메시지 처리기입니다.


void CPropertyCNNDlg::OnBnClickedAddButton()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	if (m_VButtons.size() >= MAX_LAYER_SIZE)
	{
		MessageBox(_T("더 이상의 레이어를 추가하실 수 없습니다."));
		return;
	}
	
	// 다이얼로그 윈도우의 위치 및 크기 정보 얻기
	CRect rect;
	GetWindowRect(&rect);

	int oldWindowX = rect.left;
	int oldWindowY = rect.top;
	int oldWindowWidth = rect.Width();
	int oldWindowHeight = rect.Height();

	// 각 버튼의 위치 및 크기 정보 얻기
	m_InputButton.GetWindowRect(&rect);
	int startX = rect.left;
	int startY = rect.top;
	int btnWidth = rect.Width();
	int btnHeight = rect.Height();

	// 현재 히든 레이어 버튼의 갯수 -> 갯수 + 각 위치 정보 및 크기 정보를 종합해 컨트롤들 재배치
	int numOfHidden = m_VButtons.size();

	// 버튼 사이의 간격 조정
	// int between = (oldWindowWidth - 46 - ((numOfHidden + 3) * btnWidth)) / (numOfHidden + 2);
	m_OptimizationButton.GetWindowRect(&rect);
	int optStartX = rect.left;
	m_OutputButton.GetWindowRect(&rect);
	int outputLastX = rect.right;
	int between = optStartX - outputLastX;

	// 버튼 추가 후 새로운 윈도우 사이즈 조정.
	int newWindowX = oldWindowX - ((btnWidth + between) / 2);
	int newWindowWidth = oldWindowWidth + btnWidth + between;

	// 새 윈도우 생성
	SetWindowPos(NULL, newWindowX, oldWindowY, newWindowWidth, oldWindowHeight, SWP_NOZORDER);
	
	// 히든 레이어 버튼 추가
	CButton * newButton = new CButton;
	newButton->Create(NULL, BS_PUSHBUTTON | WS_VISIBLE | WS_CHILD, rect, this, 20000 + numOfHidden);
	newButton->ShowWindow(SW_SHOW);
	m_VButtons.push_back(newButton); // 버튼 vector 상에 저장

	// 추가된 레이어에 대한 property 구조체 생성 저장 및 디폴트값 설정
	HiddenLayerProperty * prop = new HiddenLayerProperty();
	prop->Initialize();
	m_VProperty.push_back(prop);

	// 처음 시작인 input 레이어의 버튼 정보를 얻기
	m_InputButton.GetWindowRect(&rect);
	ScreenToClient(&rect);
	
	// input 레이어 버튼 배치
	int btnX = 15;
	int btnY = rect.top;
	m_InputButton.MoveWindow(btnX, btnY, btnWidth, btnHeight);

	// input 텍스트 배치
	m_InputText.GetWindowRect(&rect);
	ScreenToClient(&rect);
	int textY = rect.top;
	int textWidth = rect.Width();
	int textHeight = rect.Height();
	m_InputText.MoveWindow(btnX, textY, textWidth, textHeight);

	// vector에 들어있는 히든 레이어 버튼 배치
	for (CButton * i : m_VButtons)
	{
		btnX = btnX + btnWidth + between;
		i->MoveWindow(btnX, btnY, btnWidth, btnHeight);
	}

	// output 버튼 및 텍스트 배치
	btnX = btnX + btnWidth + between;
	m_OutputButton.MoveWindow(btnX, btnY, btnWidth, btnHeight);
	m_InputText.GetWindowRect(&rect);
	ScreenToClient(&rect);
	int textX = btnX - 5;
	textY = rect.top;
	textWidth = rect.Width();
	textHeight = rect.Height();
	m_OutputText.MoveWindow(textX, textY, textWidth, textHeight);

	// Optimization 버튼 및 텍스트 배치
	btnX = btnX + btnWidth + between;
	m_OptimizationButton.MoveWindow(btnX, btnY, btnWidth, btnHeight);
	m_InputText.GetWindowRect(&rect);
	ScreenToClient(&rect);
	textX = btnX - 5;
	textY = rect.top;
	textWidth = rect.Width();
	textHeight = rect.Height();
	m_OptimizeText.MoveWindow(textX, textY, textWidth, textHeight);
	
	Invalidate(TRUE);
}

// 버튼 이벤트 핸들러
BOOL CPropertyCNNDlg::OnCommand(WPARAM wParam, LPARAM lParam)
{
	wchar_t buf[100];

	// 히든 레이어 버튼만 필터링
	if (wParam >= 20000 && wParam < 20100)
	{
		_itow_s<100>(wParam, buf, 10);
		MessageBox(buf);
		
		CHiddenPropertyDlg dlg(m_VProperty[wParam - 20000]);
		if (dlg.DoModal() == IDOK)
		{

		}

	}

	if (wParam == IDOK)
	{
		// 히든 레이어가 한 개도 없는 경우
		if (m_VButtons.size() == 0)
		{
			AfxMessageBox(_T("히든 레이어가 1개 이상 필요합니다."));
			return false;
		}

		CNetNameDlg nameDlg;
		// 서버로 설정 정보 전송, 이름 입력 받기
		if (nameDlg.DoModal() == IDOK)
		{
			payload load;
			int index = 0; // 레이어의 인덱스

			// 입력 레이어 페이로드 설정
			load.payload_arr[index++] = init_input_payload(inputProp.width * inputProp.height * 3);

			// 히든 레이어 페이로드 설정
			for (int i = 0; i < m_VProperty.size(); i++)
			{
				if (m_VProperty[i]->convOrFully == 0) // 합성곱 레이어 선택 시
				{
					load.payload_arr[index++] = init_conv_payload(m_VProperty[i]->conv.initMethod, m_VProperty[i]->conv.average, m_VProperty[i]->conv.sigma, m_VProperty[i]->conv.filterSize,
						m_VProperty[i]->conv.stride, m_VProperty[i]->conv.padding, m_VProperty[i]->conv.output, m_VProperty[i]->conv.bias);
				}
				else if (m_VProperty[i]->convOrFully == 1) // 완전 연결 레이어 선택 시
				{
					load.payload_arr[index++] = init_fully_connected_payload(m_VProperty[i]->fulCon.initMethod, m_VProperty[i]->fulCon.average,
						m_VProperty[i]->fulCon.sigma, m_VProperty[i]->fulCon.output, m_VProperty[i]->fulCon.bias);
				}
				
				// 활성화 레이어 페이로드 설정
				load.payload_arr[index++] = init_activation_payload(m_VProperty[i]->activ.activationMethod);

				if (m_VProperty[i]->PoolingOnOff) // 풀링 레이어를 설정한 경우
				{
					load.payload_arr[index++] = init_pooling_payload(m_VProperty[i]->pool.filterSize, m_VProperty[i]->pool.stride, m_VProperty[i]->pool.padding
						, m_VProperty[i]->pool.poolingMethod);
				}

				if (m_VProperty[i]->BatchOnOff) // 배치 정규화 레이어를 설정한 경우
				{
					load.payload_arr[index++] = init_batchnorm_payload(0.9);
				}

				if (m_VProperty[i]->DropOnOff) // 드롭 아웃 레이어를 설정한 경우
				{
					load.payload_arr[index++] = init_dropout_payload(m_VProperty[i]->drop.proportion);
				}
			}

			// 출력 레이어 페이로드 설정
			load.payload_arr[index++] = init_output_payload(outputProp.numOfClassification);

			// 옵티마이저 페이로드 설정
			load.payload_arr[index++] = init_optimizer_payload(optimProp.optimizer, optimProp.learningRate, optimProp.momentum);

			// 종료 페이로드 설정
			load.payload_arr[index++] = init_end_payload();

			int payloadSize = calculate_payload_len(&load);
			
			int namelen = nameDlg.m_strNetName.GetLength();

			char* allPayload = (char*)malloc(payloadSize * sizeof(char) + sizeof(int) + namelen);  //페이로드 길이 + 페이로드
			
			memcpy(&allPayload[0], &namelen, sizeof(int));  //신경망의 이름길이를 전달
			memcpy(&allPayload[sizeof(namelen)], CT2A(nameDlg.m_strNetName), namelen);//신경망의 이름을 전달
			int n = copy_payload_to_chararr(&load, &allPayload[sizeof(namelen) + namelen]);//신경망의 구성정보를 전달
			n += sizeof(int) + namelen;

			// 전송
			CMainFrame* frame = (CMainFrame *)AfxGetMainWnd();
			packet netPack;
			netPack.n_opcode = 1;
			netPack.n_size = n;
			netPack.cptr_payload = allPayload;
			sendToServ(frame->hSocket, &netPack);
			
			free_payload(&load);
			free(allPayload);
		}
	}

	return CDialog::OnCommand(wParam, lParam);
}


BOOL CPropertyCNNDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.
	m_InputButton.EnableWindowsTheming(FALSE);
	m_InputButton.SetFaceColor(RGB(200, 100, 100), true);

	m_OutputButton.EnableWindowsTheming(FALSE);
	m_OutputButton.SetFaceColor(RGB(100, 200, 100), true);

	m_OptimizationButton.EnableWindowsTheming(FALSE);
	m_OptimizationButton.SetFaceColor(RGB(100, 100, 200), true);

	inputProp.width = 100; // Input Layer 초기화
	inputProp.height = 100;

	outputProp.numOfClassification = 100; // Output Later 초기화
	outputProp.oneHotOnOff = 0;

	// Optimization Layer 초기화
	optimProp.optimizer = 0;
	optimProp.momentum = 0;
	optimProp.learningRate = 0.01;

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}


void CPropertyCNNDlg::OnBnClickedDelButton()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	if (m_VButtons.size() > 0)
	{
		// 다이얼로그 윈도우의 위치 및 크기 정보 얻기
		CRect rect;
		GetWindowRect(&rect);

		int oldWindowX = rect.left;
		int oldWindowY = rect.top;
		int oldWindowWidth = rect.Width();
		int oldWindowHeight = rect.Height();

		// 각 버튼의 위치 및 크기 정보 얻기
		m_InputButton.GetWindowRect(&rect);
		int startX = rect.left;
		int startY = rect.top;
		int btnWidth = rect.Width();
		int btnHeight = rect.Height();

		// 현재 히든 레이어 버튼의 갯수 -> 갯수 + 각 위치 정보 및 크기 정보를 종합해 컨트롤들 재배치
		int numOfHidden = m_VButtons.size();

		// 버튼 사이의 간격 조정
		//int between = (oldWindowWidth - 46 - ((numOfHidden + 2) * btnWidth)) / (numOfHidden + 1);
		m_OptimizationButton.GetWindowRect(&rect);
		int optStartX = rect.left;
		m_OutputButton.GetWindowRect(&rect);
		int outputLastX = rect.right;
		int between = optStartX - outputLastX;

		// 버튼 추가 후 새로운 윈도우 사이즈 조정.
		int newWindowX = oldWindowX + ((btnWidth + between) / 2);
		int newWindowWidth = oldWindowWidth - btnWidth - between;

		// 새 윈도우 생성
		SetWindowPos(NULL, newWindowX, oldWindowY, newWindowWidth, oldWindowHeight, SWP_NOZORDER);

		// 마지막 히든 레이어 버튼 삭제
		CButton* lastHidden = m_VButtons.back();
		lastHidden->ShowWindow(SW_ERASE);
		delete lastHidden;
		m_VButtons.pop_back();

		// 삭제된 레이어에 대한 property 구조체 삭제
		HiddenLayerProperty* prop = m_VProperty.back();
		delete prop;
		m_VProperty.pop_back();

		// 처음 시작인 input 레이어의 버튼 정보를 얻기
		m_InputButton.GetWindowRect(&rect);
		ScreenToClient(&rect);

		// input 레이어 버튼 배치
		int btnX = 15;
		int btnY = rect.top;
		m_InputButton.MoveWindow(btnX, btnY, btnWidth, btnHeight);

		// input 텍스트 배치
		m_InputText.GetWindowRect(&rect);
		ScreenToClient(&rect);
		int textY = rect.top;
		int textWidth = rect.Width();
		int textHeight = rect.Height();
		m_InputText.MoveWindow(btnX, textY, textWidth, textHeight);

		// vector에 들어있는 히든 레이어 버튼 배치
		for (CButton * i : m_VButtons)
		{
			btnX = btnX + btnWidth + between;
			i->MoveWindow(btnX, btnY, btnWidth, btnHeight);
		}

		// output 버튼 및 텍스트 배치
		btnX = btnX + btnWidth + between;
		m_OutputButton.MoveWindow(btnX, btnY, btnWidth, btnHeight);
		m_InputText.GetWindowRect(&rect);
		ScreenToClient(&rect);
		int textX = btnX - 5;
		textY = rect.top;
		textWidth = rect.Width();
		textHeight = rect.Height();
		m_OutputText.MoveWindow(textX, textY, textWidth, textHeight);

		// Optimization 버튼 및 텍스트 배치
		btnX = btnX + btnWidth + between;
		m_OptimizationButton.MoveWindow(btnX, btnY, btnWidth, btnHeight);
		m_InputText.GetWindowRect(&rect);
		ScreenToClient(&rect);
		textX = btnX - 5;
		textY = rect.top;
		textWidth = rect.Width();
		textHeight = rect.Height();
		m_OptimizeText.MoveWindow(textX, textY, textWidth, textHeight);

		Invalidate(TRUE);
	}
}


void CPropertyCNNDlg::OnBnClickedInputLayout()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CInputPropertyDlg dlg(inputProp);
	if (dlg.DoModal() == IDOK)
	{
		inputProp.width = dlg.m_nWidth;
		inputProp.height = dlg.m_nHeight;
	}
}


void CPropertyCNNDlg::OnBnClickedOutputLayer()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	COutputPropertyDlg dlg(outputProp);
	if (dlg.DoModal() == IDOK)
	{
		outputProp.numOfClassification = dlg.m_nClassNum;
		outputProp.oneHotOnOff = dlg.m_nOneHotOnOff;
	}
}


void CPropertyCNNDlg::OnBnClickedOptimization()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	COptimizationPropertyDlg dlg(optimProp);
	if (dlg.DoModal() == IDOK)
	{
		optimProp.optimizer = dlg.m_nOptimizer;
		optimProp.momentum = dlg.optimProp.momentum;
		optimProp.learningRate = dlg.m_fLearningRate;
	}
}
