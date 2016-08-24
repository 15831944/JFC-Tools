#if !defined(AFX_CHILDFILM_H__FF12812A_7363_48A4_A5B5_19CA06FA9677__INCLUDED_)
#define AFX_CHILDFILM_H__FF12812A_7363_48A4_A5B5_19CA06FA9677__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ChildFilm.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CChildFilm frame

class CChildFilm : public SECMDIChildWnd
{
	DECLARE_DYNCREATE(CChildFilm)
protected:
	CChildFilm();           // protected constructor used by dynamic creation

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CChildFilm)
	protected:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~CChildFilm();

	// Generated message map functions
	//{{AFX_MSG(CChildFilm)
	afx_msg void OnGetMinMaxInfo(MINMAXINFO FAR* lpMMI);
	afx_msg void OnClose();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CHILDFILM_H__FF12812A_7363_48A4_A5B5_19CA06FA9677__INCLUDED_)
