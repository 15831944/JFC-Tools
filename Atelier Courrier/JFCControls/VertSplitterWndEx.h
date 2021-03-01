#if !defined(AFX_CHIDESPLITTERWNDEX_H__61D2F7E7_7AAA_11D1_9F4C_008029E98A75__INCLUDED_)
#define AFX_CHIDESPLITTERWNDEX_H__61D2F7E7_7AAA_11D1_9F4C_008029E98A75__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// VertSplitterWndEx.h : header file
//

#include "SplitterWndEx.h"
/////////////////////////////////////////////////////////////////////////////
// CHideSplitterWndEx window

class AFX_EXT_CLASS CHideSplitterWndEx : public CSplitterWndEx
{
// Construction
public:
	CHideSplitterWndEx();

// Attributes
public:

private:
	BOOL m_bClick;
	BOOL m_bDblClick;

// Operations
public:
	void ClickBar(BOOL bState=TRUE){m_bClick=bState;}
	void DblClickBar(BOOL bState=TRUE){m_bDblClick=bState;}
	void HideBar(int nCol = 1);
	void HideBar(int nCol, bool bShow);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CHideSplitterWndEx)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CHideSplitterWndEx();


	// Generated message map functions
protected:
	//{{AFX_MSG(CHideSplitterWndEx)
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

	int Sens();
public:
	virtual void OnDrawSplitter(CDC* pDC, ESplitType nType, const CRect& rect);
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CHIDESPLITTERWNDEX_H__61D2F7E7_7AAA_11D1_9F4C_008029E98A75__INCLUDED_)
