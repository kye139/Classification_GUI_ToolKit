
#pragma once

#include "ViewTree.h"
#include "IppImage\IppImage.h"

class CFileViewToolBar : public CMFCToolBar
{
	virtual void OnUpdateCmdUI(CFrameWnd* pTarget, BOOL bDisableIfNoHndler)
	{
		CMFCToolBar::OnUpdateCmdUI((CFrameWnd*) GetOwner(), bDisableIfNoHndler);
	}

	virtual BOOL AllowShowOnList() const { return FALSE; }
};

class CFileView : public CDockablePane
{
// �����Դϴ�.
public:
	CFileView();

	void AdjustLayout();
	void OnChangeVisualStyle();

// Ư���Դϴ�.
public:

	CViewTree m_wndFileView;
	CImageList m_FileViewImages;
	CFileViewToolBar m_wndToolBar;

	// ����, �߶󳻱� Ŭ������
	ImageInform		  cpBoard;
	// �߶󳻱�� �ӽ� �����.
	HTREEITEM		  tempCut;
	// üũ�ڽ� on/off
	bool ckbox;

protected:
	void FillFileView();
	void hideCheckBox(HTREEITEM hItem);
	BOOL CheckName(HTREEITEM targetItem, TCHAR* name);

// �����Դϴ�.
public:
	virtual ~CFileView();

protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	afx_msg void OnProperties();
	afx_msg void OnFileOpen();
	afx_msg void OnFileOpenWith();
	afx_msg void OnDummyCompile();
	afx_msg void OnPaint();
	afx_msg void OnSetFocus(CWnd* pOldWnd);

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnUpdateEditCopy(CCmdUI *pCmdUI);
	afx_msg void OnEditPaste();
	afx_msg void OnUpdateEditPaste(CCmdUI *pCmdUI);
	afx_msg void OnUpdateEditCut(CCmdUI *pCmdUI);
	afx_msg void OnUpdateEditClear(CCmdUI *pCmdUI);
	afx_msg void OnNewGroup();
	afx_msg void OnMultiSelect();
	afx_msg void OnEditCut();
	afx_msg void OnEditCopy();
	afx_msg void OnEditClear();
	afx_msg void OnImageTransport();
	afx_msg void OnUpdateImageTransport(CCmdUI *pCmdUI);
};

