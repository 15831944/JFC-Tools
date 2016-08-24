/***********************************************
	Ultimate Grid 97
	Copyright 1994 - 1997 Dundas Software Ltd.

	Class 
		CUGEdit2
	Purpose
		This is the standard edit control for the 
		grid. The grid can use any control derived
		from CWnd as a edit control.
	Details
		-This control automatically expands to the
		 right as text is entered, once the control
		 reaches the right side of the grid then it
		 expands downward until it reaches the bottom.
		 Once it reaches the bottom then it will start
		 scrolling text as it is entered.
		-When editing first starts the control also
		 automatically expands to fit the inital text.
		-When editing finishes it is the responsability
		 of the edit control to call the grid's 
		 CUGCtrl::EditCtrlFinished method to notify
		 the grid that editing has finished.
		-The edit control can also call the grid's
		 OnEditVerify during editing as well to give
		 the programmer greater control over what is
		 being entered.
		-New edit controls can be used by the grid
		 by calling CUGCtrl::SetNewEditClass. This
		 function can be called just before editing
		 starts, thus allowing different cells to use
		 different edit controls.
************************************************/

class CUGEdit2 : public CUGEdit
{
// Construction
public:
	CUGEdit2();

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CUGEdit2)
	public:
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CUGEdit2();

	// Generated message map functions
protected:
	//{{AFX_MSG(CUGEdit2)
	afx_msg void OnKillFocus(CWnd* pNewWnd);
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg int OnMouseActivate(CWnd* pDesktopWnd, UINT nHitTest, UINT message);
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	afx_msg void OnUpdate();
	afx_msg UINT OnGetDlgCode();
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////
