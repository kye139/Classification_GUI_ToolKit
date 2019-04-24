
#pragma once

#include "DataTree.h"

typedef struct {
	CString dirName;
	int count;
}ImageData;

class CClassToolBar : public CMFCToolBar
{
	virtual void OnUpdateCmdUI(CFrameWnd* /*pTarget*/, BOOL bDisableIfNoHndler)
	{
		CMFCToolBar::OnUpdateCmdUI((CFrameWnd*) GetOwner(), bDisableIfNoHndler);
	}

	virtual BOOL AllowShowOnList() const { return FALSE; }
};

class CClassView : public CDockablePane
{
public:
	CClassView();
	virtual ~CClassView();

	void AdjustLayout();
	void OnChangeVisualStyle();

	// 체크박스 on/off
	bool ckbox;

protected:
	CClassToolBar m_wndToolBar;
	CDataTree m_wndClassView;
	CImageList m_ClassViewImages;
	UINT m_nCurrSort;

	void FillClassView();
	void hideCheckBox(HTREEITEM hItem);
	//void ToggleFirstLayerCheckBox(HTREEITEM root);

// 재정의입니다.
public:
	void SelToCString(void);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	std::vector<ImageData> imageDataList;
	std::vector<CString> m_vSelString;

protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	afx_msg void OnClassAddMemberFunction();
	afx_msg void OnClassAddMemberVariable();
	afx_msg void OnClassDefinition();
	afx_msg void OnClassProperties();
	afx_msg void OnNewFolder();
	afx_msg void OnPaint();
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	afx_msg LRESULT OnChangeActiveTab(WPARAM, LPARAM);
	afx_msg void OnSort(UINT id);
	afx_msg void OnUpdateSort(CCmdUI* pCmdUI);

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnUpdateDataCheckbox(CCmdUI *pCmdUI);
	afx_msg void OnDataCheckbox();
	afx_msg void OnUpdateDataLoad(CCmdUI *pCmdUI);
	afx_msg void OnDataLoad();
	afx_msg void OnDataDownload();
	afx_msg void OnDataLearning();
};

