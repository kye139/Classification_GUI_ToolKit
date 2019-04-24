
#include "stdafx.h"
#include "MainFrm.h"
#include "OutputView.h"
#include "Resource.h"
#include "ImageTool.h"

class COutputViewMenuButton : public CMFCToolBarMenuButton
{
	friend class COutputView;

	DECLARE_SERIAL(COutputViewMenuButton)

public:
	COutputViewMenuButton(HMENU hMenu = NULL) : CMFCToolBarMenuButton((UINT)-1, hMenu, -1)
	{
	}

	virtual void OnDraw(CDC* pDC, const CRect& rect, CMFCToolBarImages* pImages, BOOL bHorz = TRUE,
		BOOL bCustomizeMode = FALSE, BOOL bHighlight = FALSE, BOOL bDrawBorder = TRUE, BOOL bGrayDisabledButtons = TRUE)
	{
		pImages = CMFCToolBar::GetImages();

		CAfxDrawState ds;
		pImages->PrepareDrawImage(ds);

		CMFCToolBarMenuButton::OnDraw(pDC, rect, pImages, bHorz, bCustomizeMode, bHighlight, bDrawBorder, bGrayDisabledButtons);

		pImages->EndDrawImage(ds);
	}
};

IMPLEMENT_SERIAL(COutputViewMenuButton, CMFCToolBarMenuButton, 1)

//////////////////////////////////////////////////////////////////////
// 생성/소멸
//////////////////////////////////////////////////////////////////////

COutputView::COutputView()
	: ckbox(false)
{
	m_nCurrSort = ID_SORTING_GROUPBYTYPE;
}

COutputView::~COutputView()
{
}

BEGIN_MESSAGE_MAP(COutputView, CDockablePane)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_WM_CONTEXTMENU()
	ON_COMMAND(ID_CLASS_ADD_MEMBER_FUNCTION, OnClassAddMemberFunction)
	ON_COMMAND(ID_CLASS_ADD_MEMBER_VARIABLE, OnClassAddMemberVariable)
	ON_COMMAND(ID_CLASS_DEFINITION, OnClassDefinition)
	ON_COMMAND(ID_CLASS_PROPERTIES, OnClassProperties)
	ON_COMMAND(ID_NEW_FOLDER, OnNewFolder)
	ON_WM_PAINT()
	ON_WM_SETFOCUS()
	ON_COMMAND_RANGE(ID_SORTING_GROUPBYTYPE, ID_SORTING_SORTBYACCESS, OnSort)
	ON_UPDATE_COMMAND_UI_RANGE(ID_SORTING_GROUPBYTYPE, ID_SORTING_SORTBYACCESS, OnUpdateSort)
	//	ON_WM_LBUTTONDOWN()
	// ON_UPDATE_COMMAND_UI(ID_NET_CHECKBOX, &COutputView::OnUpdateOutputCheckbox)
	// ON_COMMAND(ID_NET_CHECKBOX, &CNetView::OnNetCheckbox)
	// ON_UPDATE_COMMAND_UI(ID_NET_LOAD, &CNetView::OnUpdateNetLoad)
	// ON_COMMAND(ID_NET_LOAD, &CNetView::OnNetLoad)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CClassView 메시지 처리기

int COutputView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDockablePane::OnCreate(lpCreateStruct) == -1)
		return -1;

	CRect rectDummy;
	rectDummy.SetRectEmpty();

	// 뷰를 만듭니다.
	const DWORD dwViewStyle = WS_CHILD | WS_VISIBLE | TVS_HASLINES | TVS_LINESATROOT | TVS_HASBUTTONS | WS_CLIPSIBLINGS | WS_CLIPCHILDREN;

	if (!m_wndOutputView.Create(dwViewStyle, rectDummy, this, 2))
	{
		TRACE0("클래스 뷰를 만들지 못했습니다.\n");
		return -1;      // 만들지 못했습니다.
	}

	// 이미지를 로드합니다.
	m_wndToolBar.Create(this, AFX_DEFAULT_TOOLBAR_STYLE, IDR_OUTPUT_VIEW_TOOLBAR);
	m_wndToolBar.LoadToolBar(IDR_OUTPUT_VIEW_TOOLBAR, 0, 0, TRUE /* 잠금 */);

	OnChangeVisualStyle();

	m_wndToolBar.SetPaneStyle(m_wndToolBar.GetPaneStyle() | CBRS_TOOLTIPS | CBRS_FLYBY);
	m_wndToolBar.SetPaneStyle(m_wndToolBar.GetPaneStyle() & ~(CBRS_GRIPPER | CBRS_SIZE_DYNAMIC | CBRS_BORDER_TOP | CBRS_BORDER_BOTTOM | CBRS_BORDER_LEFT | CBRS_BORDER_RIGHT));

	m_wndToolBar.SetOwner(this);

	// 모든 명령은 부모 프레임이 아닌 이 컨트롤을 통해 라우팅됩니다.
	m_wndToolBar.SetRouteCommandsViaFrame(FALSE);

	CMenu menuSort;
	menuSort.LoadMenu(IDR_POPUP_SORT);

	m_wndToolBar.ReplaceButton(ID_SORT_MENU, COutputViewMenuButton(menuSort.GetSubMenu(0)->GetSafeHmenu()));

	COutputViewMenuButton* pButton = DYNAMIC_DOWNCAST(COutputViewMenuButton, m_wndToolBar.GetButton(0));

	if (pButton != NULL)
	{
		pButton->m_bText = FALSE;
		pButton->m_bImage = TRUE;
		pButton->SetImage(GetCmdMgr()->GetCmdImage(m_nCurrSort));
		pButton->SetMessageWnd(this);
	}

	// 정적 트리 뷰 데이터를 더미 코드로 채웁니다.
	FillOutputView();

	return 0;
}

void COutputView::OnSize(UINT nType, int cx, int cy)
{
	CDockablePane::OnSize(nType, cx, cy);
	AdjustLayout();
}

void COutputView::FillOutputView()
{
	HTREEITEM hRoot = m_wndOutputView.InsertItem(_T("FakeApp 클래스"), 0, 0);
	m_wndOutputView.SetItemState(hRoot, TVIS_BOLD, TVIS_BOLD);

	HTREEITEM hClass = m_wndOutputView.InsertItem(_T("CFakeAboutDlg"), 1, 1, hRoot);
	m_wndOutputView.InsertItem(_T("CFakeAboutDlg()"), 3, 3, hClass);

	m_wndOutputView.Expand(hRoot, TVE_EXPAND);

	hClass = m_wndOutputView.InsertItem(_T("CFakeApp"), 1, 1, hRoot);
	m_wndOutputView.InsertItem(_T("CFakeApp()"), 3, 3, hClass);
	m_wndOutputView.InsertItem(_T("InitInstance()"), 3, 3, hClass);
	m_wndOutputView.InsertItem(_T("OnAppAbout()"), 3, 3, hClass);

	hClass = m_wndOutputView.InsertItem(_T("CFakeAppDoc"), 1, 1, hRoot);
	m_wndOutputView.InsertItem(_T("CFakeAppDoc()"), 4, 4, hClass);
	m_wndOutputView.InsertItem(_T("~CFakeAppDoc()"), 3, 3, hClass);
	m_wndOutputView.InsertItem(_T("OnNewDocument()"), 3, 3, hClass);

	hClass = m_wndOutputView.InsertItem(_T("CFakeAppView"), 1, 1, hRoot);
	m_wndOutputView.InsertItem(_T("CFakeAppView()"), 4, 4, hClass);
	m_wndOutputView.InsertItem(_T("~CFakeAppView()"), 3, 3, hClass);
	m_wndOutputView.InsertItem(_T("GetDocument()"), 3, 3, hClass);
	m_wndOutputView.Expand(hClass, TVE_EXPAND);
}

void COutputView::OnContextMenu(CWnd* pWnd, CPoint point)
{
	CTreeCtrl* pWndTree = (CTreeCtrl*)&m_wndOutputView;
	ASSERT_VALID(pWndTree);

	if (pWnd != pWndTree)
	{
		CDockablePane::OnContextMenu(pWnd, point);
		return;
	}

	if (point != CPoint(-1, -1))
	{
		// 클릭한 항목을 선택합니다.
		CPoint ptTree = point;
		pWndTree->ScreenToClient(&ptTree);

		UINT flags = 0;
		HTREEITEM hTreeItem = pWndTree->HitTest(ptTree, &flags);
		if (hTreeItem != NULL)
		{
			pWndTree->SelectItem(hTreeItem);
		}
	}

	pWndTree->SetFocus();
	CMenu menu;
	menu.LoadMenu(IDR_POPUP_SORT);

	CMenu* pSumMenu = menu.GetSubMenu(0);

	if (AfxGetMainWnd()->IsKindOf(RUNTIME_CLASS(CMDIFrameWndEx)))
	{
		CMFCPopupMenu* pPopupMenu = new CMFCPopupMenu;

		if (!pPopupMenu->Create(this, point.x, point.y, (HMENU)pSumMenu->m_hMenu, FALSE, TRUE))
			return;

		((CMDIFrameWndEx*)AfxGetMainWnd())->OnShowPopupMenu(pPopupMenu);
		UpdateDialogControls(this, FALSE);
	}
}

void COutputView::AdjustLayout()
{
	if (GetSafeHwnd() == NULL)
	{
		return;
	}

	CRect rectClient;
	GetClientRect(rectClient);

	int cyTlb = m_wndToolBar.CalcFixedLayout(FALSE, TRUE).cy;

	m_wndToolBar.SetWindowPos(NULL, rectClient.left, rectClient.top, rectClient.Width(), cyTlb, SWP_NOACTIVATE | SWP_NOZORDER);
	m_wndOutputView.SetWindowPos(NULL, rectClient.left + 1, rectClient.top + cyTlb + 1, rectClient.Width() - 2, rectClient.Height() - cyTlb - 2, SWP_NOACTIVATE | SWP_NOZORDER);
}

BOOL COutputView::PreTranslateMessage(MSG* pMsg)
{
	return CDockablePane::PreTranslateMessage(pMsg);
}

void COutputView::OnSort(UINT id)
{
	if (m_nCurrSort == id)
	{
		return;
	}

	m_nCurrSort = id;

	COutputViewMenuButton* pButton = DYNAMIC_DOWNCAST(COutputViewMenuButton, m_wndToolBar.GetButton(0));

	if (pButton != NULL)
	{
		pButton->SetImage(GetCmdMgr()->GetCmdImage(id));
		m_wndToolBar.Invalidate();
		m_wndToolBar.UpdateWindow();
	}
}

void COutputView::OnUpdateSort(CCmdUI* pCmdUI)
{
	pCmdUI->SetCheck(pCmdUI->m_nID == m_nCurrSort);
}

void COutputView::OnClassAddMemberFunction()
{
	AfxMessageBox(_T("멤버 함수 추가..."));
}

void COutputView::OnClassAddMemberVariable()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
}

void COutputView::OnClassDefinition()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
}

void COutputView::OnClassProperties()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
}

void COutputView::hideCheckBox(HTREEITEM hItem)
{
	if (hItem == NULL)
		return;

	m_wndOutputView.SetItemState(hItem, INDEXTOSTATEIMAGEMASK(0), TVIS_STATEIMAGEMASK);

	hideCheckBox(m_wndOutputView.GetChildItem(hItem));
	hideCheckBox(m_wndOutputView.GetNextItem(hItem, TVGN_NEXT));
}
/*
void CClassView::ToggleFirstLayerCheckBox(HTREEITEM root)
{
HTREEITEM firstLevelPointer = m_wndClassView.GetChildItem(root);
HTREEITEM childPointer = m_wndClassView.GetChildItem(firstLevelPointer);

while (firstLevelPointer)
{
while (childPointer)
{
hideCheckBox(childPointer);
childPointer = m_wndClassView.GetNextItem(childPointer, TVGN_NEXT);
}
firstLevelPointer = m_wndClassView.GetNextItem(firstLevelPointer, TVGN_NEXT);
childPointer = m_wndClassView.GetChildItem(firstLevelPointer);
}
}
*/
void COutputView::OnNewFolder()
{
	// 체크박스가 활성화되어 있는 경우
	if (ckbox)
	{
		//m_wndFileView.SetExtendedStyle(nowStyle, nowStyle &= ~TVS_CHECKBOXES);
		hideCheckBox(m_wndOutputView.GetRootItem());
		m_wndOutputView.ModifyStyle(TVS_CHECKBOXES, 0);
		ckbox = false;
	}
	// 체크박스가 비활성화 되어 있는 경우
	else
	{
		//m_wndFileView.SetExtendedStyle(nowStyle, TVS_CHECKBOXES);
		// First remove the checkbox style
		m_wndOutputView.ModifyStyle(TVS_CHECKBOXES, 0);
		// Then explicitly set it
		m_wndOutputView.ModifyStyle(0, TVS_CHECKBOXES);
		//ToggleFirstLayerCheckBox(m_wndClassView.GetRootItem());
		ckbox = true;
	}
}

void COutputView::OnPaint()
{
	CPaintDC dc(this); // 그리기를 위한 디바이스 컨텍스트입니다.

	CRect rectTree;
	m_wndOutputView.GetWindowRect(rectTree);
	ScreenToClient(rectTree);

	rectTree.InflateRect(1, 1);
	dc.Draw3dRect(rectTree, ::GetSysColor(COLOR_3DSHADOW), ::GetSysColor(COLOR_3DSHADOW));
}

void COutputView::OnSetFocus(CWnd* pOldWnd)
{
	CDockablePane::OnSetFocus(pOldWnd);

	m_wndOutputView.SetFocus();
}

void COutputView::OnChangeVisualStyle()
{
	m_OutputViewImages.DeleteImageList();

	UINT uiBmpId = theApp.m_bHiColorIcons ? IDB_CLASS_VIEW_24 : IDB_CLASS_VIEW;

	CBitmap bmp;
	if (!bmp.LoadBitmap(uiBmpId))
	{
		TRACE(_T("비트맵을 로드할 수 없습니다. %x\n"), uiBmpId);
		ASSERT(FALSE);
		return;
	}

	BITMAP bmpObj;
	bmp.GetBitmap(&bmpObj);

	UINT nFlags = ILC_MASK;

	nFlags |= (theApp.m_bHiColorIcons) ? ILC_COLOR24 : ILC_COLOR4;

	m_OutputViewImages.Create(16, bmpObj.bmHeight, nFlags, 0, 0);
	m_OutputViewImages.Add(&bmp, RGB(255, 0, 0));

	m_wndOutputView.SetImageList(&m_OutputViewImages, TVSIL_NORMAL);

	m_wndToolBar.CleanUpLockedImages();
	m_wndToolBar.LoadBitmap(theApp.m_bHiColorIcons ? IDB_OUTPUT_VIEW_TOOLBAR : IDB_OUTPUT_VIEW_TOOLBAR, 0, 0, TRUE /* 잠금 */);
}

// void COutputView::OnUpdateOutputCheckbox(CCmdUI *pCmdUI)
// {
// 	// TODO: 여기에 명령 업데이트 UI 처리기 코드를 추가합니다.
// 	pCmdUI->Enable(TRUE);
// }
// 
// 
// void COutputView::OnOutputCheckbox()
// {
// 	// TODO: 여기에 명령 처리기 코드를 추가합니다.
// 	// DWORD nowStyle = m_wndFileView.GetExtendedStyle();
// 
// 	// 체크박스가 활성화되어 있는 경우
// 	if (ckbox)
// 	{
// 		//m_wndFileView.SetExtendedStyle(nowStyle, nowStyle &= ~TVS_CHECKBOXES);
// 		hideCheckBox(m_wndOutputView.GetRootItem());
// 		m_wndOutputView.ModifyStyle(TVS_CHECKBOXES, 0);
// 		m_wndOutputView.m_vSelectList.clear();
// 		ckbox = false;
// 	}
// 	// 체크박스가 비활성화 되어 있는 경우
// 	else
// 	{
// 		HTREEITEM child = m_wndOutputView.GetChildItem(m_wndOutputView.GetRootItem());
// 		m_wndOutputView.Expand(child, TVE_COLLAPSE);
// 		while (child = m_wndOutputView.GetNextItem(child, TVGN_NEXT))
// 			m_wndOutputView.Expand(child, TVE_COLLAPSE);
// 		//m_wndFileView.SetExtendedStyle(nowStyle, TVS_CHECKBOXES);
// 		// First remove the checkbox style
// 		m_wndOutputView.ModifyStyle(TVS_CHECKBOXES, 0);
// 		// Then explicitly set it
// 		m_wndOutputView.ModifyStyle(0, TVS_CHECKBOXES);
// 		child = m_wndOutputView.GetChildItem(m_wndOutputView.GetRootItem());
// 		m_wndOutputView.SetItemState(m_wndOutputView.GetRootItem(), INDEXTOSTATEIMAGEMASK(0), TVIS_STATEIMAGEMASK);
// 		HTREEITEM hideChild = m_wndOutputView.GetChildItem(child);
// 		m_wndOutputView.SetItemState(hideChild, INDEXTOSTATEIMAGEMASK(0), TVIS_STATEIMAGEMASK);
// 		while (hideChild = m_wndOutputView.GetNextItem(hideChild, TVGN_NEXT))
// 			m_wndOutputiew.SetItemState(hideChild, INDEXTOSTATEIMAGEMASK(0), TVIS_STATEIMAGEMASK);
// 		while (child = m_wndOutputView.GetNextItem(child, TVGN_NEXT))
// 		{
// 			hideChild = m_wndOutputView.GetChildItem(child);
// 			m_wndOutputView.SetItemState(hideChild, INDEXTOSTATEIMAGEMASK(0), TVIS_STATEIMAGEMASK);
// 			while (hideChild = m_wndOutputView.GetNextItem(hideChild, TVGN_NEXT))
// 				m_wndOutputView.SetItemState(hideChild, INDEXTOSTATEIMAGEMASK(0), TVIS_STATEIMAGEMASK);
// 		}
// 		ckbox = true;
// 	}
// }
// 
// 
// void COutputView::OnUpdateOutputLoad(CCmdUI *pCmdUI)
// {
// 	// TODO: 여기에 명령 업데이트 UI 처리기 코드를 추가합니다.
// 	pCmdUI->Enable(TRUE);
// }


// void COutputView::OnOutputLoad()
// {
// 	// TODO: 여기에 명령 처리기 코드를 추가합니다.
// 	int requestCode = 14;
// 	CMainFrame* frame = (CMainFrame *)AfxGetMainWnd();
// 
// 	packet pack{ requestCode, 0, NULL };
// 	sendToServ(frame->hSocket, &pack);
// 	recvFromServ(frame->hSocket, &pack);
// 
// 	if (pack.n_opcode == 14)
// 	{
// 		int outputNum = *(int *)pack.cptr_payload;
// 		char* startPointer = pack.cptr_payload + sizeof(int);
// 
// 		for (int i = 0; i < outputNum; i++)
// 		{
// 			// Output 디렉터리 이름 로드
// 			int outputNameLen = *(int *)startPointer;
// 			char * outputName = (char *)malloc(outputNameLen + 1);
// 			memcpy(outputName, startPointer + sizeof(int), outputNameLen);
// 			outputName[outputNameLen] = 0;
// 
// 			// 해당 Output 구성 설정 정보 로드
// 			startPointer = startPointer + sizeof(int) + outputNameLen;
// 
// 			// 설정 정보를 저장할 벡터를 동적으로 생성
// 			std::vector<void *> * outputProp = new std::vector<void *>();
// 
// 			// input 레이어 정보.
// 			if (((input_payload *)(startPointer))->n_code == CODE_INPUT)
// 			{
// 				input_payload * inputProp = (input_payload *)malloc(sizeof(input_payload));
// 				inputProp->n_features = ((input_payload *)(startPointer))->n_features;
// 				inputProp->n_code = ((input_payload *)startPointer)->n_code;
// 				outputProp->push_back((void *)inputProp);
// 			}
// 			else
// 			{
// 				AfxMessageBox(_T("input Prop Error"));
// 				return;
// 			}
// 
// 			// 히든 레이어 정보
// 			startPointer = startPointer + sizeof(input_payload);
// 			while (true)
// 			{
// 				if (*(int *)startPointer == CODE_CONV)
// 				{
// 					conv_payload * convProp = (conv_payload *)malloc(sizeof(conv_payload));
// 					convProp->n_init_method = ((conv_payload *)startPointer)->n_init_method;
// 					convProp->f_bias = ((conv_payload *)startPointer)->f_bias;
// 					convProp->f_mean = ((conv_payload *)startPointer)->f_mean;
// 					convProp->f_std = ((conv_payload *)startPointer)->f_std;
// 					convProp->n_code = ((conv_payload *)startPointer)->n_code;
// 					convProp->n_filter_size = ((conv_payload *)startPointer)->n_filter_size;
// 					convProp->n_outputs = ((conv_payload *)startPointer)->n_outputs;
// 					convProp->n_padding_method = ((conv_payload *)startPointer)->n_padding_method;
// 					convProp->n_strides_size = ((conv_payload *)startPointer)->n_strides_size;
// 					startPointer = startPointer + sizeof(conv_payload);
// 					outputProp->push_back((void *)convProp);
// 					continue;
// 				}
// 				else if (*(int *)startPointer == CODE_FULLY)
// 				{
// 					fully_connect_payload * fullyProp = (fully_connect_payload *)malloc(sizeof(fully_connect_payload));
// 					fullyProp->f_bias = ((fully_connect_payload *)startPointer)->f_bias;
// 					fullyProp->f_mean = ((fully_connect_payload *)startPointer)->f_mean;
// 					fullyProp->f_std = ((fully_connect_payload *)startPointer)->f_std;
// 					fullyProp->n_code = ((fully_connect_payload *)startPointer)->n_code;
// 					fullyProp->n_init_method = ((fully_connect_payload *)startPointer)->n_init_method;
// 					fullyProp->n_outputs = ((fully_connect_payload *)startPointer)->n_outputs;
// 					startPointer = startPointer + sizeof(fully_connect_payload);
// 					outputProp->push_back((void *)fullyProp);
// 					continue;
// 				}
// 				else if (*(int *)startPointer == CODE_ACTIV)
// 				{
// 					activation_payload * activProp = (activation_payload *)malloc(sizeof(activation_payload));
// 					activProp->n_activation_method = ((activation_payload *)startPointer)->n_activation_method;
// 					activProp->n_code = ((activation_payload *)startPointer)->n_code;
// 					startPointer = startPointer + sizeof(activation_payload);
// 					outputProp->push_back((void *)activProp);
// 					continue;
// 				}
// 				else if (*(int *)startPointer == CODE_POOL)
// 				{
// 					pooling_payload * poolingProp = (pooling_payload *)malloc(sizeof(pooling_payload));
// 					poolingProp->n_code = ((pooling_payload *)startPointer)->n_code;
// 					poolingProp->n_filter_size = ((pooling_payload *)startPointer)->n_filter_size;
// 					poolingProp->n_padding_method = ((pooling_payload *)startPointer)->n_padding_method;
// 					poolingProp->n_pooling_method = ((pooling_payload *)startPointer)->n_pooling_method;
// 					poolingProp->n_strides = ((pooling_payload *)startPointer)->n_strides;
// 					startPointer = startPointer + sizeof(pooling_payload);
// 					outputProp->push_back((void *)poolingProp);
// 					continue;
// 				}
// 				else if (*(int *)startPointer == CODE_BATCH)
// 				{
// 					batch_norm_payload * batchProp = (batch_norm_payload *)malloc(sizeof(batch_norm_payload));
// 					batchProp->f_momentum = ((batch_norm_payload *)startPointer)->f_momentum;
// 					batchProp->n_code = ((batch_norm_payload *)startPointer)->n_code;
// 					startPointer = startPointer + sizeof(batch_norm_payload);
// 					outputProp->push_back((void *)batchProp);
// 					continue;
// 				}
// 				else if (*(int *)startPointer == CODE_DROP)
// 				{
// 					dropout_payload * dropoutProp = (dropout_payload *)malloc(sizeof(dropout_payload));
// 					dropoutProp->f_ratio = ((dropout_payload *)startPointer)->f_ratio;
// 					dropoutProp->n_code = ((dropout_payload *)startPointer)->n_code;
// 					startPointer = startPointer + sizeof(dropout_payload);
// 					outputProp->push_back((void *)dropoutProp);
// 					continue;
// 				}
// 				else if (*(int *)startPointer == CODE_OUTP)
// 					break;
// 			}
// 
// 			// Output 설정
// 			if (*(int *)startPointer == CODE_OUTP)
// 			{
// 				output_payload * _outputProp = (output_payload *)malloc(sizeof(output_payload));
// 				_outputProp->n_class_num = ((output_payload *)startPointer)->n_class_num;
// 				_outputProp->n_code = ((output_payload *)startPointer)->n_code;
// 				_outputProp->n_is_onehot = ((output_payload *)startPointer)->n_is_onehot;
// 				startPointer = startPointer + sizeof(output_payload);
// 				outputProp->push_back((void *)outputProp);
// 			}
// 			else
// 			{
// 				AfxMessageBox(_T("output Layer Error"));
// 				return;
// 			}
// 
// 			// optimize 설정
// 			if (*(int *)startPointer == CODE_OPTMZ)
// 			{
// 				optimizer_payload * optimProp = (optimizer_payload *)malloc(sizeof(optimizer_payload));
// 				optimProp->f_momentum = ((optimizer_payload *)startPointer)->f_momentum;
// 				optimProp->f_ratio = ((optimizer_payload *)startPointer)->f_ratio;
// 				optimProp->n_code = ((optimizer_payload *)startPointer)->n_code;
// 				optimProp->n_optimizer = ((optimizer_payload *)startPointer)->n_optimizer;
// 				startPointer = startPointer + sizeof(optimizer_payload);
// 				outputProp->push_back((void *)optimProp);
// 			}
// 			else
// 			{
// 				AfxMessageBox(_T("optimizer Layer Error"));
// 				return;
// 			}
// 
// 			// 트리 컨트롤에 출력
// 			HTREEITEM hRoot = m_wndOutputView.GetRootItem();
// 			HTREEITEM hItem = m_wndOutputView.InsertItem(CA2T(outputName), hRoot, TVI_LAST);
// 			free(outputName);
// 			m_wndOutputView.SetItemData(hItem, (DWORD_PTR)outputProp);
// 		}
// 	}
// }
