
#pragma once

#include "NetTree.h"

class CNetToolBar : public CMFCToolBar
{
	virtual void OnUpdateCmdUI(CFrameWnd* /*pTarget*/, BOOL bDisableIfNoHndler)
	{
		CMFCToolBar::OnUpdateCmdUI((CFrameWnd*)GetOwner(), bDisableIfNoHndler);
	}

	virtual BOOL AllowShowOnList() const { return FALSE; }
};

class CNetView : public CDockablePane
{
public:
	CNetView();
	virtual ~CNetView();

	void AdjustLayout();
	void OnChangeVisualStyle();

	// 체크박스 on/off
	bool ckbox;

protected:
	CNetToolBar m_wndToolBar;
	CImageList m_NetViewImages;
	UINT m_nCurrSort;

	void FillNetView();
	void hideCheckBox(HTREEITEM hItem);
	//void ToggleFirstLayerCheckBox(HTREEITEM root);

	// 재정의입니다.
public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	CNetTree m_wndNetView;

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
	afx_msg void OnUpdateNetCheckbox(CCmdUI *pCmdUI);
	afx_msg void OnNetCheckbox();
	afx_msg void OnUpdateNetLoad(CCmdUI *pCmdUI);
	afx_msg void OnNetLoad();
};

