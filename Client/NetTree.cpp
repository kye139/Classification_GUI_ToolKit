
#include "stdafx.h"
#include "ImageTool.h"
#include "NetTree.h"
#include "MainFrm.h"
#include "IppImage\IppImage.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CNetTree

CNetTree::CNetTree()
{
}

CNetTree::~CNetTree()
{
}

BEGIN_MESSAGE_MAP(CNetTree, CTreeCtrl)
	ON_NOTIFY_REFLECT(TVN_SELCHANGED, &CNetTree::OnTvnSelchanged)
	ON_NOTIFY_REFLECT(NM_CLICK, &CNetTree::OnNMClick)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CNetTree �޽��� ó����

// BOOL CNetTree::OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult)
// {
// 	BOOL bRes = CNetTree::OnNotify(wParam, lParam, pResult);
// 
// 	NMHDR* pNMHDR = (NMHDR*)lParam;
// 	ASSERT(pNMHDR != NULL);
// 
// 	if (pNMHDR && pNMHDR->code == TTN_SHOW && GetToolTips() != NULL)
// 	{
// 		GetToolTips()->SetWindowPos(&wndTop, -1, -1, -1, -1, SWP_NOMOVE | SWP_NOACTIVATE | SWP_NOSIZE);
// 	}
// 
// 	return bRes;
// }

// ���õ� �̹��� �׸� �Ӽ� â�� ���.
void CNetTree::OnTvnSelchanged(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	HTREEITEM selectItem = pNMTreeView->itemNew.hItem;
	if ((std::vector<void *> *)GetItemData(selectItem) && GetParentItem(selectItem) != GetRootItem())
	{
		wchar_t value[100];
		CMainFrame* pFrame = (CMainFrame *)AfxGetMainWnd();
		// ���Ϻ信�� ����� �̹��� ���� ��������
		std::vector<void *> * propInfo = (std::vector<void *> *)GetItemData(selectItem);
	
		CMFCPropertyGridCtrl * propPtr = &pFrame->m_wndProperties.m_wndPropList;
		propPtr->RemoveAll();
	
		for (int i = 0; i < propInfo->size(); i++)
		{
			int code = *(int *)((*propInfo)[i]);
			if (code == CODE_INPUT)
			{
				input_payload * inputPayload = (input_payload *)((*propInfo)[i]);
				CMFCPropertyGridProperty* inputProp = new CMFCPropertyGridProperty(_T("Input Layer"));

				CMFCPropertyGridProperty * n_feature = new CMFCPropertyGridProperty(_T("Ư¡ ����"), (_variant_t)inputPayload->n_features, _T("n_feature"));
				n_feature->AllowEdit(FALSE);
				inputProp->AddSubItem(n_feature);

				propPtr->AddProperty(inputProp);
			}
			else if (code == CODE_CONV)
			{
				conv_payload * convPayload = (conv_payload *)((*propInfo)[i]);
				CMFCPropertyGridProperty* convProp = new CMFCPropertyGridProperty(_T("Convolution Layer"));

				CMFCPropertyGridProperty * initMethod = NULL;
				if (convPayload->n_init_method == 0) // ���� ���� ���� ����
					initMethod = new CMFCPropertyGridProperty(_T("�ʱ�ȭ ���"), (_variant_t)_T("���� ���� ���� ����"), _T("0"));
				else if (convPayload->n_init_method == 1) // ���� ���� ����
					initMethod = new CMFCPropertyGridProperty(_T("�ʱ�ȭ ���"), (_variant_t)_T("���� ���� ����"), _T("1"));
				initMethod->AllowEdit(FALSE);
				convProp->AddSubItem(initMethod);

				CMFCPropertyGridProperty * mean = new CMFCPropertyGridProperty(_T("���"), (_variant_t)convPayload->f_mean, _T("mean"));
				mean->AllowEdit(FALSE);
				convProp->AddSubItem(mean);

				CMFCPropertyGridProperty * std = new CMFCPropertyGridProperty(_T("ǥ�� ����"), (_variant_t)convPayload->f_std, _T("std"));
				std->AllowEdit(FALSE);
				convProp->AddSubItem(std);

				CMFCPropertyGridProperty * filterSize = new CMFCPropertyGridProperty(_T("���� ũ��"), (_variant_t)convPayload->n_filter_size, _T("filterSize"));
				filterSize->AllowEdit(FALSE);
				convProp->AddSubItem(filterSize);

				CMFCPropertyGridProperty * stride = new CMFCPropertyGridProperty(_T("����"), (_variant_t)convPayload->n_strides_size, _T("stride"));
				stride->AllowEdit(FALSE);
				convProp->AddSubItem(stride);

				CMFCPropertyGridProperty * padding = NULL;
				if (convPayload->n_padding_method == 0) // ���� �е� ���
					padding = new CMFCPropertyGridProperty(_T("�е� ��� ����"), (_variant_t)_T("���� �е� ���"), _T("0"));
				else if (convPayload->n_padding_method == 1) // �е� ������� ����
					padding = new CMFCPropertyGridProperty(_T("�е� ��� ����"), (_variant_t)_T("�е� ������� ����"), _T("1"));
				padding->AllowEdit(FALSE);
				convProp->AddSubItem(padding);

				CMFCPropertyGridProperty * outputNum = new CMFCPropertyGridProperty(_T("��� ����"), (_variant_t)convPayload->n_outputs, _T("output"));
				outputNum->AllowEdit(FALSE);
				convProp->AddSubItem(outputNum);

				CMFCPropertyGridProperty * bias = new CMFCPropertyGridProperty(_T("���� ��"), (_variant_t)convPayload->f_bias, _T("bias"));
				bias->AllowEdit(FALSE);
				convProp->AddSubItem(bias);

				propPtr->AddProperty(convProp);
			}
			else if (code == CODE_FULLY)
			{
				fully_connect_payload * fullyPayload = (fully_connect_payload *)((*propInfo)[i]);
				CMFCPropertyGridProperty* fullyProp = new CMFCPropertyGridProperty(_T("Fully Connect Layer"));

				CMFCPropertyGridProperty * initMethod = NULL;
				if (fullyPayload->n_init_method == 0) // ���� ���� ����
					initMethod = new CMFCPropertyGridProperty(_T("�ʱ�ȭ ���"), (_variant_t)_T("���� ���� ����"), _T("0"));
				else if (fullyPayload->n_init_method == 1) // ���� ���� ����
					initMethod = new CMFCPropertyGridProperty(_T("�ʱ�ȭ ���"), (_variant_t)_T("���� ���� ����"), _T("1"));
				else if (fullyPayload->n_init_method == 2) // Xavier �ʱ�ȭ
					initMethod = new CMFCPropertyGridProperty(_T("�ʱ�ȭ ���"), (_variant_t)_T("Xavier �ʱ�ȭ"), _T("2"));
				else if (fullyPayload->n_init_method == 3) // he �ʱ�ȭ
					initMethod = new CMFCPropertyGridProperty(_T("�ʱ�ȭ ���"), (_variant_t)_T("he �ʱ�ȭ"), _T("3"));
				else if (fullyPayload->n_init_method == 4) // tanh ��õ �ʱ�ȭ
					initMethod = new CMFCPropertyGridProperty(_T("�ʱ�ȭ ���"), (_variant_t)_T("tanh ��õ �ʱ�ȭ"), _T("4"));
				initMethod->AllowEdit(FALSE);
				fullyProp->AddSubItem(initMethod);

				CMFCPropertyGridProperty * mean = new CMFCPropertyGridProperty(_T("���"), (_variant_t)fullyPayload->f_mean, _T("mean"));
				mean->AllowEdit(FALSE);
				fullyProp->AddSubItem(mean);

				CMFCPropertyGridProperty * std = new CMFCPropertyGridProperty(_T("ǥ�� ����"), (_variant_t)fullyPayload->f_std, _T("std"));
				std->AllowEdit(FALSE);
				fullyProp->AddSubItem(std);

				CMFCPropertyGridProperty * outputNum = new CMFCPropertyGridProperty(_T("��� ����"), (_variant_t)fullyPayload->n_outputs, _T("outputNum"));
				outputNum->AllowEdit(FALSE);
				fullyProp->AddSubItem(outputNum);

				CMFCPropertyGridProperty * bias = new CMFCPropertyGridProperty(_T("���� ��"), (_variant_t)fullyPayload->f_bias, _T("bias"));
				bias->AllowEdit(FALSE);
				fullyProp->AddSubItem(bias);

				propPtr->AddProperty(fullyProp);
			}
			else if (code == CODE_ACTIV)
			{
				activation_payload * activePayload = (activation_payload *)((*propInfo)[i]);
				CMFCPropertyGridProperty * activeProp = new CMFCPropertyGridProperty(_T("Activation Layer"));

				CMFCPropertyGridProperty * activeMethod = NULL;
				if (activePayload->n_activation_method == 0) // ReLu
					activeMethod = new CMFCPropertyGridProperty(_T("�ʱ�ȭ ���"), (_variant_t)_T("ReLu"), _T("0"));
				else if (activePayload->n_activation_method == 1) // Sigmoid
					activeMethod = new CMFCPropertyGridProperty(_T("�ʱ�ȭ ���"), (_variant_t)_T("Sigmoid"), _T("1"));
				else if (activePayload->n_activation_method == 2) // Leaky ReLu
					activeMethod = new CMFCPropertyGridProperty(_T("�ʱ�ȭ ���"), (_variant_t)_T("Leaky ReLu"), _T("2"));
				else if (activePayload->n_activation_method == 3) // thanh
					activeMethod = new CMFCPropertyGridProperty(_T("�ʱ�ȭ ���"), (_variant_t)_T("thanh"), _T("3"));
				else if (activePayload->n_activation_method == 4) // ELU
					activeMethod = new CMFCPropertyGridProperty(_T("�ʱ�ȭ ���"), (_variant_t)_T("ELU"), _T("4"));

				activeMethod->AllowEdit(FALSE);
				activeProp->AddSubItem(activeMethod);

				propPtr->AddProperty(activeProp);
			}
			else if (code == CODE_POOL)
			{
				pooling_payload * poolingPayload = (pooling_payload *)((*propInfo)[i]);
				CMFCPropertyGridProperty * poolingProp = new CMFCPropertyGridProperty(_T("Pooling Layer"));

				CMFCPropertyGridProperty * filterSize = new CMFCPropertyGridProperty(_T("���� ũ��"), (_variant_t)poolingPayload->n_filter_size, _T("filterSize"));
				filterSize->AllowEdit(FALSE);
				poolingProp->AddSubItem(filterSize);

				CMFCPropertyGridProperty * stride = new CMFCPropertyGridProperty(_T("���� ũ��"), (_variant_t)poolingPayload->n_strides, _T("strides"));
				stride->AllowEdit(FALSE);
				poolingProp->AddSubItem(stride);

				CMFCPropertyGridProperty * padding = NULL;
				if (poolingPayload->n_padding_method == 0) // ���� �е� ���
					padding = new CMFCPropertyGridProperty(_T("�е� ��� ����"), (_variant_t)_T("���� �е� ���"), _T("0"));
				else if (poolingPayload->n_padding_method == 1) // �е� ������� ����
					padding = new CMFCPropertyGridProperty(_T("�е� ��� ����"), (_variant_t)_T("�д� ������� ����"), _T("1"));
				padding->AllowEdit(FALSE);
				poolingProp->AddSubItem(padding);

				CMFCPropertyGridProperty * poolingMethod = NULL;
				if (poolingPayload->n_pooling_method == 0) // �ƽ� Ǯ�� ���
					poolingMethod = new CMFCPropertyGridProperty(_T("Ǯ�� ���"), (_variant_t)_T("�ƽ� Ǯ�� ���"), _T("0"));
				else if (poolingPayload->n_pooling_method == 1) // ��� Ǯ�� ���
					poolingMethod = new CMFCPropertyGridProperty(_T("Ǯ�� ���"), (_variant_t)_T("��� Ǯ�� ���"), _T("1"));
				poolingMethod->AllowEdit(FALSE);
				poolingProp->AddSubItem(poolingMethod);

				propPtr->AddProperty(poolingProp);
			}
			else if (code == CODE_BATCH)
			{
				batch_norm_payload * batchPayload = (batch_norm_payload *)((*propInfo)[i]);
				CMFCPropertyGridProperty * batchProp = new CMFCPropertyGridProperty(_T("Batch Normalization Layer"));

				CMFCPropertyGridProperty * momentum = new CMFCPropertyGridProperty(_T("�����"), (_variant_t)batchPayload->f_momentum, _T("momentum"));
				momentum->AllowEdit(FALSE);
				batchProp->AddSubItem(momentum);

				propPtr->AddProperty(batchProp);
			}
			else if (code == CODE_DROP)
			{
				dropout_payload * dropPayload = (dropout_payload *)((*propInfo)[i]);
				CMFCPropertyGridProperty * dropProp = new CMFCPropertyGridProperty(_T("Dropout Layer"));

				CMFCPropertyGridProperty * proportion = new CMFCPropertyGridProperty(_T("����"), (_variant_t)dropPayload->f_ratio, _T("ratio"));
				proportion->AllowEdit(FALSE);
				dropProp->AddSubItem(proportion);

				propPtr->AddProperty(dropProp);
			}
			else if (code == CODE_OUTP)
			{
				output_payload * outputPayload = (output_payload *)((*propInfo)[i]);
				CMFCPropertyGridProperty * outputProp = new CMFCPropertyGridProperty(_T("Output Layer"));

				CMFCPropertyGridProperty * numClass = new CMFCPropertyGridProperty(_T("�з� ����"), (_variant_t)outputPayload->n_class_num, _T("numClass"));
				numClass->AllowEdit(FALSE);
				outputProp->AddSubItem(numClass);

				CMFCPropertyGridProperty * oneHot = new CMFCPropertyGridProperty(_T("���� ���ڵ� ����"), (_variant_t)outputPayload->n_is_onehot, _T("oneHot"));
				oneHot->AllowEdit(FALSE);
				outputProp->AddSubItem(oneHot);

				propPtr->AddProperty(outputProp);
			}
			else if (code == CODE_OPTMZ)
			{
				optimizer_payload * optimPayload = (optimizer_payload *)((*propInfo)[i]);
				CMFCPropertyGridProperty * optimProp = new CMFCPropertyGridProperty(_T("Optimization Layer"));

				CMFCPropertyGridProperty * optimMethod = NULL;
				if (optimPayload->n_optimizer == 0) // Adam
					optimMethod = new CMFCPropertyGridProperty(_T("��Ƽ������"), (_variant_t)_T("Adam"), _T("0"));
				else if (optimPayload->n_optimizer == 1) // �����
					optimMethod = new CMFCPropertyGridProperty(_T("��Ƽ������"), (_variant_t)_T("�����"), _T("1"));
				else if (optimPayload->n_optimizer == 2) // �׽��׷��� ���Ӱ��
					optimMethod = new CMFCPropertyGridProperty(_T("��Ƽ������"), (_variant_t)_T("�׽��׷��� ���Ӱ��"), _T("2"));
				else if (optimPayload->n_optimizer == 3) // RMSProp
					optimMethod = new CMFCPropertyGridProperty(_T("��Ƽ������"), (_variant_t)_T("RMSProp"), _T("3"));
				else if (optimPayload->n_optimizer == 4) // ����ϰ���
					optimMethod = new CMFCPropertyGridProperty(_T("��Ƽ������"), (_variant_t)_T("����ϰ���"), _T("4"));
				optimMethod->AllowEdit(FALSE);
				optimProp->AddSubItem(optimMethod);

				CMFCPropertyGridProperty * momentum = NULL;
				if (optimPayload->n_optimizer >= 1 && optimPayload->n_optimizer <= 3)
					momentum = new CMFCPropertyGridProperty(_T("�����"), (_variant_t)optimPayload->f_momentum, _T("momentum"));
				else
					momentum = new CMFCPropertyGridProperty(_T("�����"), (_variant_t)_T("��� ����"), _T("that's no no"));
				momentum->AllowEdit(FALSE);
				optimProp->AddSubItem(momentum);

				CMFCPropertyGridProperty * learningRate = new CMFCPropertyGridProperty(_T("�н���"), (_variant_t)optimPayload->f_momentum, _T("learningRate"));
				learningRate->AllowEdit(FALSE);
				optimProp->AddSubItem(learningRate);

				propPtr->AddProperty(optimProp);
			}
			else
				continue;
		}
	}
	*pResult = 0;
}

BOOL CNetTree::PreTranslateMessage(MSG* pMsg)
{
	// TODO: ���⿡ Ư��ȭ�� �ڵ带 �߰� ��/�Ǵ� �⺻ Ŭ������ ȣ���մϴ�.
	if (pMsg->message == WM_KEYDOWN)
	{
		if (GetEditControl() && (pMsg->wParam == VK_RETURN))
		{
			::TranslateMessage(pMsg);
			::DispatchMessage(pMsg);
			return TRUE;
		}
	}

	return CTreeCtrl::PreTranslateMessage(pMsg);
}

void CNetTree::OnNMClick(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	AfxMessageBox(_T("bbb"));

	// CPoint point;
	// UINT nFlags = 0;
	// 
	// GetCursorPos(&point);
	// ::ScreenToClient(m_hWnd, &point);
	// 
	// HTREEITEM hItem = HitTest(point, &nFlags);
	// 
	// // �������� �����ϰ� üũ�ڽ��� �̺�Ʈ�� �߻��ߴٸ�
	// if (hItem != NULL && (nFlags & TVHT_ONITEMSTATEICON) != 0)
	// {
	// 	// �ش� �������� üũ�Ǿ� �ִٸ�
	// 	if (GetCheck(hItem))
	// 	{
	// 		// UnCheckChildItems(GetChildItem(hItem));
	// 		int * id = (int *)GetItemData(hItem);
	// 		std::vector<int>::iterator iter;
	// 		for (iter = m_vSelectList.begin(); iter != m_vSelectList.end(); iter++)
	// 		{
	// 			if (*iter == *id)
	// 			{
	// 				m_vSelectList.erase(iter);
	// 				return;
	// 			}
	// 		}
	// 	}
	// 	// �ش� �������� üũ�Ǿ� ���� �ʴٸ�
	// 	else
	// 	{
	// 		// CheckChildItems(GetChildItem(hItem));
	// 		int* id = (int*)GetItemData(hItem);
	// 		m_vSelectList.push_back(*id);
	// 	}
	// }
	// 
	// *pResult = 0;
}
