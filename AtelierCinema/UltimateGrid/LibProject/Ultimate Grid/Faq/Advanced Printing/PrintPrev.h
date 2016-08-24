#if !defined(AFX_PRINTPREV_H__7A34AFD8_C84C_11D2_83DA_0020359647BF__INCLUDED_)
#define AFX_PRINTPREV_H__7A34AFD8_C84C_11D2_83DA_0020359647BF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PrintPrev.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CPrintPrev dialog

class CPrintPrev : public CDialog
{
// Construction
public:
	CPrintPrev(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CPrintPrev)
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPrintPrev)
	public:
	virtual BOOL Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext = NULL);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CPrintPrev)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PRINTPREV_H__7A34AFD8_C84C_11D2_83DA_0020359647BF__INCLUDED_)
