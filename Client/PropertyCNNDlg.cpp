// PropertyCNNDlg.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "ImageTool.h"
#include "PropertyCNNDlg.h"
#include "afxdialogex.h"
#include "HiddenPropertyDlg.h"
#include "NetNameDlg.h"
#include "MainFrm.h"

#define MAX_LAYER_SIZE 10000


// CPropertyCNNDlg ��ȭ �����Դϴ�.

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


// CPropertyCNNDlg �޽��� ó�����Դϴ�.


void CPropertyCNNDlg::OnBnClickedAddButton()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	if (m_VButtons.size() >= MAX_LAYER_SIZE)
	{
		MessageBox(_T("�� �̻��� ���̾ �߰��Ͻ� �� �����ϴ�."));
		return;
	}
	
	// ���̾�α� �������� ��ġ �� ũ�� ���� ���
	CRect rect;
	GetWindowRect(&rect);

	int oldWindowX = rect.left;
	int oldWindowY = rect.top;
	int oldWindowWidth = rect.Width();
	int oldWindowHeight = rect.Height();

	// �� ��ư�� ��ġ �� ũ�� ���� ���
	m_InputButton.GetWindowRect(&rect);
	int startX = rect.left;
	int startY = rect.top;
	int btnWidth = rect.Width();
	int btnHeight = rect.Height();

	// ���� ���� ���̾� ��ư�� ���� -> ���� + �� ��ġ ���� �� ũ�� ������ ������ ��Ʈ�ѵ� ���ġ
	int numOfHidden = m_VButtons.size();

	// ��ư ������ ���� ����
	// int between = (oldWindowWidth - 46 - ((numOfHidden + 3) * btnWidth)) / (numOfHidden + 2);
	m_OptimizationButton.GetWindowRect(&rect);
	int optStartX = rect.left;
	m_OutputButton.GetWindowRect(&rect);
	int outputLastX = rect.right;
	int between = optStartX - outputLastX;

	// ��ư �߰� �� ���ο� ������ ������ ����.
	int newWindowX = oldWindowX - ((btnWidth + between) / 2);
	int newWindowWidth = oldWindowWidth + btnWidth + between;

	// �� ������ ����
	SetWindowPos(NULL, newWindowX, oldWindowY, newWindowWidth, oldWindowHeight, SWP_NOZORDER);
	
	// ���� ���̾� ��ư �߰�
	CButton * newButton = new CButton;
	newButton->Create(NULL, BS_PUSHBUTTON | WS_VISIBLE | WS_CHILD, rect, this, 20000 + numOfHidden);
	newButton->ShowWindow(SW_SHOW);
	m_VButtons.push_back(newButton); // ��ư vector �� ����

	// �߰��� ���̾ ���� property ����ü ���� ���� �� ����Ʈ�� ����
	HiddenLayerProperty * prop = new HiddenLayerProperty();
	prop->Initialize();
	m_VProperty.push_back(prop);

	// ó�� ������ input ���̾��� ��ư ������ ���
	m_InputButton.GetWindowRect(&rect);
	ScreenToClient(&rect);
	
	// input ���̾� ��ư ��ġ
	int btnX = 15;
	int btnY = rect.top;
	m_InputButton.MoveWindow(btnX, btnY, btnWidth, btnHeight);

	// input �ؽ�Ʈ ��ġ
	m_InputText.GetWindowRect(&rect);
	ScreenToClient(&rect);
	int textY = rect.top;
	int textWidth = rect.Width();
	int textHeight = rect.Height();
	m_InputText.MoveWindow(btnX, textY, textWidth, textHeight);

	// vector�� ����ִ� ���� ���̾� ��ư ��ġ
	for (CButton * i : m_VButtons)
	{
		btnX = btnX + btnWidth + between;
		i->MoveWindow(btnX, btnY, btnWidth, btnHeight);
	}

	// output ��ư �� �ؽ�Ʈ ��ġ
	btnX = btnX + btnWidth + between;
	m_OutputButton.MoveWindow(btnX, btnY, btnWidth, btnHeight);
	m_InputText.GetWindowRect(&rect);
	ScreenToClient(&rect);
	int textX = btnX - 5;
	textY = rect.top;
	textWidth = rect.Width();
	textHeight = rect.Height();
	m_OutputText.MoveWindow(textX, textY, textWidth, textHeight);

	// Optimization ��ư �� �ؽ�Ʈ ��ġ
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

// ��ư �̺�Ʈ �ڵ鷯
BOOL CPropertyCNNDlg::OnCommand(WPARAM wParam, LPARAM lParam)
{
	wchar_t buf[100];

	// ���� ���̾� ��ư�� ���͸�
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
		// ���� ���̾ �� ���� ���� ���
		if (m_VButtons.size() == 0)
		{
			AfxMessageBox(_T("���� ���̾ 1�� �̻� �ʿ��մϴ�."));
			return false;
		}

		CNetNameDlg nameDlg;
		// ������ ���� ���� ����, �̸� �Է� �ޱ�
		if (nameDlg.DoModal() == IDOK)
		{
			payload load;
			int index = 0; // ���̾��� �ε���

			// �Է� ���̾� ���̷ε� ����
			load.payload_arr[index++] = init_input_payload(inputProp.width * inputProp.height * 3);

			// ���� ���̾� ���̷ε� ����
			for (int i = 0; i < m_VProperty.size(); i++)
			{
				if (m_VProperty[i]->convOrFully == 0) // �ռ��� ���̾� ���� ��
				{
					load.payload_arr[index++] = init_conv_payload(m_VProperty[i]->conv.initMethod, m_VProperty[i]->conv.average, m_VProperty[i]->conv.sigma, m_VProperty[i]->conv.filterSize,
						m_VProperty[i]->conv.stride, m_VProperty[i]->conv.padding, m_VProperty[i]->conv.output, m_VProperty[i]->conv.bias);
				}
				else if (m_VProperty[i]->convOrFully == 1) // ���� ���� ���̾� ���� ��
				{
					load.payload_arr[index++] = init_fully_connected_payload(m_VProperty[i]->fulCon.initMethod, m_VProperty[i]->fulCon.average,
						m_VProperty[i]->fulCon.sigma, m_VProperty[i]->fulCon.output, m_VProperty[i]->fulCon.bias);
				}
				
				// Ȱ��ȭ ���̾� ���̷ε� ����
				load.payload_arr[index++] = init_activation_payload(m_VProperty[i]->activ.activationMethod);

				if (m_VProperty[i]->PoolingOnOff) // Ǯ�� ���̾ ������ ���
				{
					load.payload_arr[index++] = init_pooling_payload(m_VProperty[i]->pool.filterSize, m_VProperty[i]->pool.stride, m_VProperty[i]->pool.padding
						, m_VProperty[i]->pool.poolingMethod);
				}

				if (m_VProperty[i]->BatchOnOff) // ��ġ ����ȭ ���̾ ������ ���
				{
					load.payload_arr[index++] = init_batchnorm_payload(0.9);
				}

				if (m_VProperty[i]->DropOnOff) // ��� �ƿ� ���̾ ������ ���
				{
					load.payload_arr[index++] = init_dropout_payload(m_VProperty[i]->drop.proportion);
				}
			}

			// ��� ���̾� ���̷ε� ����
			load.payload_arr[index++] = init_output_payload(outputProp.numOfClassification);

			// ��Ƽ������ ���̷ε� ����
			load.payload_arr[index++] = init_optimizer_payload(optimProp.optimizer, optimProp.learningRate, optimProp.momentum);

			// ���� ���̷ε� ����
			load.payload_arr[index++] = init_end_payload();

			int payloadSize = calculate_payload_len(&load);
			
			int namelen = nameDlg.m_strNetName.GetLength();

			char* allPayload = (char*)malloc(payloadSize * sizeof(char) + sizeof(int) + namelen);  //���̷ε� ���� + ���̷ε�
			
			memcpy(&allPayload[0], &namelen, sizeof(int));  //�Ű���� �̸����̸� ����
			memcpy(&allPayload[sizeof(namelen)], CT2A(nameDlg.m_strNetName), namelen);//�Ű���� �̸��� ����
			int n = copy_payload_to_chararr(&load, &allPayload[sizeof(namelen) + namelen]);//�Ű���� ���������� ����
			n += sizeof(int) + namelen;

			// ����
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

	// TODO:  ���⿡ �߰� �ʱ�ȭ �۾��� �߰��մϴ�.
	m_InputButton.EnableWindowsTheming(FALSE);
	m_InputButton.SetFaceColor(RGB(200, 100, 100), true);

	m_OutputButton.EnableWindowsTheming(FALSE);
	m_OutputButton.SetFaceColor(RGB(100, 200, 100), true);

	m_OptimizationButton.EnableWindowsTheming(FALSE);
	m_OptimizationButton.SetFaceColor(RGB(100, 100, 200), true);

	inputProp.width = 100; // Input Layer �ʱ�ȭ
	inputProp.height = 100;

	outputProp.numOfClassification = 100; // Output Later �ʱ�ȭ
	outputProp.oneHotOnOff = 0;

	// Optimization Layer �ʱ�ȭ
	optimProp.optimizer = 0;
	optimProp.momentum = 0;
	optimProp.learningRate = 0.01;

	return TRUE;  // return TRUE unless you set the focus to a control
				  // ����: OCX �Ӽ� �������� FALSE�� ��ȯ�ؾ� �մϴ�.
}


void CPropertyCNNDlg::OnBnClickedDelButton()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.

	if (m_VButtons.size() > 0)
	{
		// ���̾�α� �������� ��ġ �� ũ�� ���� ���
		CRect rect;
		GetWindowRect(&rect);

		int oldWindowX = rect.left;
		int oldWindowY = rect.top;
		int oldWindowWidth = rect.Width();
		int oldWindowHeight = rect.Height();

		// �� ��ư�� ��ġ �� ũ�� ���� ���
		m_InputButton.GetWindowRect(&rect);
		int startX = rect.left;
		int startY = rect.top;
		int btnWidth = rect.Width();
		int btnHeight = rect.Height();

		// ���� ���� ���̾� ��ư�� ���� -> ���� + �� ��ġ ���� �� ũ�� ������ ������ ��Ʈ�ѵ� ���ġ
		int numOfHidden = m_VButtons.size();

		// ��ư ������ ���� ����
		//int between = (oldWindowWidth - 46 - ((numOfHidden + 2) * btnWidth)) / (numOfHidden + 1);
		m_OptimizationButton.GetWindowRect(&rect);
		int optStartX = rect.left;
		m_OutputButton.GetWindowRect(&rect);
		int outputLastX = rect.right;
		int between = optStartX - outputLastX;

		// ��ư �߰� �� ���ο� ������ ������ ����.
		int newWindowX = oldWindowX + ((btnWidth + between) / 2);
		int newWindowWidth = oldWindowWidth - btnWidth - between;

		// �� ������ ����
		SetWindowPos(NULL, newWindowX, oldWindowY, newWindowWidth, oldWindowHeight, SWP_NOZORDER);

		// ������ ���� ���̾� ��ư ����
		CButton* lastHidden = m_VButtons.back();
		lastHidden->ShowWindow(SW_ERASE);
		delete lastHidden;
		m_VButtons.pop_back();

		// ������ ���̾ ���� property ����ü ����
		HiddenLayerProperty* prop = m_VProperty.back();
		delete prop;
		m_VProperty.pop_back();

		// ó�� ������ input ���̾��� ��ư ������ ���
		m_InputButton.GetWindowRect(&rect);
		ScreenToClient(&rect);

		// input ���̾� ��ư ��ġ
		int btnX = 15;
		int btnY = rect.top;
		m_InputButton.MoveWindow(btnX, btnY, btnWidth, btnHeight);

		// input �ؽ�Ʈ ��ġ
		m_InputText.GetWindowRect(&rect);
		ScreenToClient(&rect);
		int textY = rect.top;
		int textWidth = rect.Width();
		int textHeight = rect.Height();
		m_InputText.MoveWindow(btnX, textY, textWidth, textHeight);

		// vector�� ����ִ� ���� ���̾� ��ư ��ġ
		for (CButton * i : m_VButtons)
		{
			btnX = btnX + btnWidth + between;
			i->MoveWindow(btnX, btnY, btnWidth, btnHeight);
		}

		// output ��ư �� �ؽ�Ʈ ��ġ
		btnX = btnX + btnWidth + between;
		m_OutputButton.MoveWindow(btnX, btnY, btnWidth, btnHeight);
		m_InputText.GetWindowRect(&rect);
		ScreenToClient(&rect);
		int textX = btnX - 5;
		textY = rect.top;
		textWidth = rect.Width();
		textHeight = rect.Height();
		m_OutputText.MoveWindow(textX, textY, textWidth, textHeight);

		// Optimization ��ư �� �ؽ�Ʈ ��ġ
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
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	CInputPropertyDlg dlg(inputProp);
	if (dlg.DoModal() == IDOK)
	{
		inputProp.width = dlg.m_nWidth;
		inputProp.height = dlg.m_nHeight;
	}
}


void CPropertyCNNDlg::OnBnClickedOutputLayer()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	COutputPropertyDlg dlg(outputProp);
	if (dlg.DoModal() == IDOK)
	{
		outputProp.numOfClassification = dlg.m_nClassNum;
		outputProp.oneHotOnOff = dlg.m_nOneHotOnOff;
	}
}


void CPropertyCNNDlg::OnBnClickedOptimization()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	COptimizationPropertyDlg dlg(optimProp);
	if (dlg.DoModal() == IDOK)
	{
		optimProp.optimizer = dlg.m_nOptimizer;
		optimProp.momentum = dlg.optimProp.momentum;
		optimProp.learningRate = dlg.m_fLearningRate;
	}
}
