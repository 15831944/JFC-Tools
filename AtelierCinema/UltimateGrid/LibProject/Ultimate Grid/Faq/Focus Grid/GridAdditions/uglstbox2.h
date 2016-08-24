/***********************************************
	Ultimate Grid 97
	Copyright 1994 - 1997 Dundas Software Ltd.

	Class 
		CUGLstBox2
	Purpose
		This class is used by the CUGDropList class
		as the listbox that appears when the droplist
		is activated.
	Details
		This class is responsible for returning any
		selection make in the listbox, and destroying
		itself when an item is selected or when the
		selection process is canceled.
************************************************/
#ifndef UGCT_DROPLISTSELECTEX
#define UGCT_DROPLISTSELECTEX 108
#endif

class CUGLstBox2 : public CUGLstBox
{
// Construction
public:
	CUGLstBox2();

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CUGLstBox2)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CUGLstBox2();

	// Generated message map functions
protected:
	//{{AFX_MSG(CUGLstBox2)
	afx_msg void OnKillFocus(CWnd* pNewWnd);
	afx_msg int OnMouseActivate(CWnd* pDesktopWnd, UINT nHitTest, UINT message);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	afx_msg UINT OnGetDlgCode();
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
public:
};

/////////////////////////////////////////////////////////////////////////////
