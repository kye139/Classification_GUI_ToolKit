
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
// CNetTree 메시지 처리기

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

// 선택된 이미지 항목 속성 창에 출력.
void CNetTree::OnTvnSelchanged(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	HTREEITEM selectItem = pNMTreeView->itemNew.hItem;
	if ((std::vector<void *> *)GetItemData(selectItem) && GetParentItem(selectItem) != GetRootItem())
	{
		wchar_t value[100];
		CMainFrame* pFrame = (CMainFrame *)AfxGetMainWnd();
		// 파일뷰에서 저장된 이미지 정보 가져오기
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

				CMFCPropertyGridProperty * n_feature = new CMFCPropertyGridProperty(_T("특징 개수"), (_variant_t)inputPayload->n_features, _T("n_feature"));
				n_feature->AllowEdit(FALSE);
				inputProp->AddSubItem(n_feature);

				propPtr->AddProperty(inputProp);
			}
			else if (code == CODE_CONV)
			{
				conv_payload * convPayload = (conv_payload *)((*propInfo)[i]);
				CMFCPropertyGridProperty* convProp = new CMFCPropertyGridProperty(_T("Convolution Layer"));

				CMFCPropertyGridProperty * initMethod = NULL;
				if (convPayload->n_init_method == 0) // 절단 정규 분포 난수
					initMethod = new CMFCPropertyGridProperty(_T("초기화 방법"), (_variant_t)_T("절단 정규 분포 난수"), _T("0"));
				else if (convPayload->n_init_method == 1) // 정규 분포 난수
					initMethod = new CMFCPropertyGridProperty(_T("초기화 방법"), (_variant_t)_T("정규 분포 난수"), _T("1"));
				initMethod->AllowEdit(FALSE);
				convProp->AddSubItem(initMethod);

				CMFCPropertyGridProperty * mean = new CMFCPropertyGridProperty(_T("평균"), (_variant_t)convPayload->f_mean, _T("mean"));
				mean->AllowEdit(FALSE);
				convProp->AddSubItem(mean);

				CMFCPropertyGridProperty * std = new CMFCPropertyGridProperty(_T("표준 편차"), (_variant_t)convPayload->f_std, _T("std"));
				std->AllowEdit(FALSE);
				convProp->AddSubItem(std);

				CMFCPropertyGridProperty * filterSize = new CMFCPropertyGridProperty(_T("필터 크기"), (_variant_t)convPayload->n_filter_size, _T("filterSize"));
				filterSize->AllowEdit(FALSE);
				convProp->AddSubItem(filterSize);

				CMFCPropertyGridProperty * stride = new CMFCPropertyGridProperty(_T("보폭"), (_variant_t)convPayload->n_strides_size, _T("stride"));
				stride->AllowEdit(FALSE);
				convProp->AddSubItem(stride);

				CMFCPropertyGridProperty * padding = NULL;
				if (convPayload->n_padding_method == 0) // 제로 패딩 사용
					padding = new CMFCPropertyGridProperty(_T("패딩 사용 여부"), (_variant_t)_T("제로 패딩 사용"), _T("0"));
				else if (convPayload->n_padding_method == 1) // 패딩 사용하지 않음
					padding = new CMFCPropertyGridProperty(_T("패딩 사용 여부"), (_variant_t)_T("패딩 사용하지 않음"), _T("1"));
				padding->AllowEdit(FALSE);
				convProp->AddSubItem(padding);

				CMFCPropertyGridProperty * outputNum = new CMFCPropertyGridProperty(_T("출력 개수"), (_variant_t)convPayload->n_outputs, _T("output"));
				outputNum->AllowEdit(FALSE);
				convProp->AddSubItem(outputNum);

				CMFCPropertyGridProperty * bias = new CMFCPropertyGridProperty(_T("편향 값"), (_variant_t)convPayload->f_bias, _T("bias"));
				bias->AllowEdit(FALSE);
				convProp->AddSubItem(bias);

				propPtr->AddProperty(convProp);
			}
			else if (code == CODE_FULLY)
			{
				fully_connect_payload * fullyPayload = (fully_connect_payload *)((*propInfo)[i]);
				CMFCPropertyGridProperty* fullyProp = new CMFCPropertyGridProperty(_T("Fully Connect Layer"));

				CMFCPropertyGridProperty * initMethod = NULL;
				if (fullyPayload->n_init_method == 0) // 절단 정규 분포
					initMethod = new CMFCPropertyGridProperty(_T("초기화 방법"), (_variant_t)_T("절단 정규 분포"), _T("0"));
				else if (fullyPayload->n_init_method == 1) // 정규 분포 난수
					initMethod = new CMFCPropertyGridProperty(_T("초기화 방법"), (_variant_t)_T("정규 분포 난수"), _T("1"));
				else if (fullyPayload->n_init_method == 2) // Xavier 초기화
					initMethod = new CMFCPropertyGridProperty(_T("초기화 방법"), (_variant_t)_T("Xavier 초기화"), _T("2"));
				else if (fullyPayload->n_init_method == 3) // he 초기화
					initMethod = new CMFCPropertyGridProperty(_T("초기화 방법"), (_variant_t)_T("he 초기화"), _T("3"));
				else if (fullyPayload->n_init_method == 4) // tanh 추천 초기화
					initMethod = new CMFCPropertyGridProperty(_T("초기화 방법"), (_variant_t)_T("tanh 추천 초기화"), _T("4"));
				initMethod->AllowEdit(FALSE);
				fullyProp->AddSubItem(initMethod);

				CMFCPropertyGridProperty * mean = new CMFCPropertyGridProperty(_T("평균"), (_variant_t)fullyPayload->f_mean, _T("mean"));
				mean->AllowEdit(FALSE);
				fullyProp->AddSubItem(mean);

				CMFCPropertyGridProperty * std = new CMFCPropertyGridProperty(_T("표준 편차"), (_variant_t)fullyPayload->f_std, _T("std"));
				std->AllowEdit(FALSE);
				fullyProp->AddSubItem(std);

				CMFCPropertyGridProperty * outputNum = new CMFCPropertyGridProperty(_T("출력 개수"), (_variant_t)fullyPayload->n_outputs, _T("outputNum"));
				outputNum->AllowEdit(FALSE);
				fullyProp->AddSubItem(outputNum);

				CMFCPropertyGridProperty * bias = new CMFCPropertyGridProperty(_T("편향 값"), (_variant_t)fullyPayload->f_bias, _T("bias"));
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
					activeMethod = new CMFCPropertyGridProperty(_T("초기화 방법"), (_variant_t)_T("ReLu"), _T("0"));
				else if (activePayload->n_activation_method == 1) // Sigmoid
					activeMethod = new CMFCPropertyGridProperty(_T("초기화 방법"), (_variant_t)_T("Sigmoid"), _T("1"));
				else if (activePayload->n_activation_method == 2) // Leaky ReLu
					activeMethod = new CMFCPropertyGridProperty(_T("초기화 방법"), (_variant_t)_T("Leaky ReLu"), _T("2"));
				else if (activePayload->n_activation_method == 3) // thanh
					activeMethod = new CMFCPropertyGridProperty(_T("초기화 방법"), (_variant_t)_T("thanh"), _T("3"));
				else if (activePayload->n_activation_method == 4) // ELU
					activeMethod = new CMFCPropertyGridProperty(_T("초기화 방법"), (_variant_t)_T("ELU"), _T("4"));

				activeMethod->AllowEdit(FALSE);
				activeProp->AddSubItem(activeMethod);

				propPtr->AddProperty(activeProp);
			}
			else if (code == CODE_POOL)
			{
				pooling_payload * poolingPayload = (pooling_payload *)((*propInfo)[i]);
				CMFCPropertyGridProperty * poolingProp = new CMFCPropertyGridProperty(_T("Pooling Layer"));

				CMFCPropertyGridProperty * filterSize = new CMFCPropertyGridProperty(_T("필터 크기"), (_variant_t)poolingPayload->n_filter_size, _T("filterSize"));
				filterSize->AllowEdit(FALSE);
				poolingProp->AddSubItem(filterSize);

				CMFCPropertyGridProperty * stride = new CMFCPropertyGridProperty(_T("보폭 크기"), (_variant_t)poolingPayload->n_strides, _T("strides"));
				stride->AllowEdit(FALSE);
				poolingProp->AddSubItem(stride);

				CMFCPropertyGridProperty * padding = NULL;
				if (poolingPayload->n_padding_method == 0) // 제로 패딩 사용
					padding = new CMFCPropertyGridProperty(_T("패딩 사용 여부"), (_variant_t)_T("제로 패딩 사용"), _T("0"));
				else if (poolingPayload->n_padding_method == 1) // 패딩 사용하지 않음
					padding = new CMFCPropertyGridProperty(_T("패딩 사용 여부"), (_variant_t)_T("패당 사용하지 않음"), _T("1"));
				padding->AllowEdit(FALSE);
				poolingProp->AddSubItem(padding);

				CMFCPropertyGridProperty * poolingMethod = NULL;
				if (poolingPayload->n_pooling_method == 0) // 맥스 풀링 사용
					poolingMethod = new CMFCPropertyGridProperty(_T("풀링 방법"), (_variant_t)_T("맥스 풀링 사용"), _T("0"));
				else if (poolingPayload->n_pooling_method == 1) // 평균 풀링 사용
					poolingMethod = new CMFCPropertyGridProperty(_T("풀링 방법"), (_variant_t)_T("평균 풀링 사용"), _T("1"));
				poolingMethod->AllowEdit(FALSE);
				poolingProp->AddSubItem(poolingMethod);

				propPtr->AddProperty(poolingProp);
			}
			else if (code == CODE_BATCH)
			{
				batch_norm_payload * batchPayload = (batch_norm_payload *)((*propInfo)[i]);
				CMFCPropertyGridProperty * batchProp = new CMFCPropertyGridProperty(_T("Batch Normalization Layer"));

				CMFCPropertyGridProperty * momentum = new CMFCPropertyGridProperty(_T("모멘텀"), (_variant_t)batchPayload->f_momentum, _T("momentum"));
				momentum->AllowEdit(FALSE);
				batchProp->AddSubItem(momentum);

				propPtr->AddProperty(batchProp);
			}
			else if (code == CODE_DROP)
			{
				dropout_payload * dropPayload = (dropout_payload *)((*propInfo)[i]);
				CMFCPropertyGridProperty * dropProp = new CMFCPropertyGridProperty(_T("Dropout Layer"));

				CMFCPropertyGridProperty * proportion = new CMFCPropertyGridProperty(_T("비율"), (_variant_t)dropPayload->f_ratio, _T("ratio"));
				proportion->AllowEdit(FALSE);
				dropProp->AddSubItem(proportion);

				propPtr->AddProperty(dropProp);
			}
			else if (code == CODE_OUTP)
			{
				output_payload * outputPayload = (output_payload *)((*propInfo)[i]);
				CMFCPropertyGridProperty * outputProp = new CMFCPropertyGridProperty(_T("Output Layer"));

				CMFCPropertyGridProperty * numClass = new CMFCPropertyGridProperty(_T("분류 개수"), (_variant_t)outputPayload->n_class_num, _T("numClass"));
				numClass->AllowEdit(FALSE);
				outputProp->AddSubItem(numClass);

				CMFCPropertyGridProperty * oneHot = new CMFCPropertyGridProperty(_T("원핫 인코딩 여부"), (_variant_t)outputPayload->n_is_onehot, _T("oneHot"));
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
					optimMethod = new CMFCPropertyGridProperty(_T("옵티마이저"), (_variant_t)_T("Adam"), _T("0"));
				else if (optimPayload->n_optimizer == 1) // 모멘텀
					optimMethod = new CMFCPropertyGridProperty(_T("옵티마이저"), (_variant_t)_T("모멘텀"), _T("1"));
				else if (optimPayload->n_optimizer == 2) // 네스테로프 가속경사
					optimMethod = new CMFCPropertyGridProperty(_T("옵티마이저"), (_variant_t)_T("네스테로프 가속경사"), _T("2"));
				else if (optimPayload->n_optimizer == 3) // RMSProp
					optimMethod = new CMFCPropertyGridProperty(_T("옵티마이저"), (_variant_t)_T("RMSProp"), _T("3"));
				else if (optimPayload->n_optimizer == 4) // 경사하강법
					optimMethod = new CMFCPropertyGridProperty(_T("옵티마이저"), (_variant_t)_T("경사하강법"), _T("4"));
				optimMethod->AllowEdit(FALSE);
				optimProp->AddSubItem(optimMethod);

				CMFCPropertyGridProperty * momentum = NULL;
				if (optimPayload->n_optimizer >= 1 && optimPayload->n_optimizer <= 3)
					momentum = new CMFCPropertyGridProperty(_T("모멘텀"), (_variant_t)optimPayload->f_momentum, _T("momentum"));
				else
					momentum = new CMFCPropertyGridProperty(_T("모멘텀"), (_variant_t)_T("사용 안함"), _T("that's no no"));
				momentum->AllowEdit(FALSE);
				optimProp->AddSubItem(momentum);

				CMFCPropertyGridProperty * learningRate = new CMFCPropertyGridProperty(_T("학습률"), (_variant_t)optimPayload->f_momentum, _T("learningRate"));
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
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
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
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	AfxMessageBox(_T("bbb"));

	// CPoint point;
	// UINT nFlags = 0;
	// 
	// GetCursorPos(&point);
	// ::ScreenToClient(m_hWnd, &point);
	// 
	// HTREEITEM hItem = HitTest(point, &nFlags);
	// 
	// // 아이템이 존재하고 체크박스에 이벤트가 발생했다면
	// if (hItem != NULL && (nFlags & TVHT_ONITEMSTATEICON) != 0)
	// {
	// 	// 해당 아이템이 체크되어 있다면
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
	// 	// 해당 아이템이 체크되어 있지 않다면
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
