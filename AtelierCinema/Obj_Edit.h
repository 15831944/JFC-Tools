#if !defined(AFX_OBJ_EDIT_H__10EFFB80_15D4_11D4_989E_004095420987__INCLUDED_)
#define AFX_OBJ_EDIT_H__10EFFB80_15D4_11D4_989E_004095420987__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Obj_Edit.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CObj_Edit window

class CObj_Edit : public CEdit
{
// Construction
public:
	CObj_Edit();

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CObj_Edit)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CObj_Edit();

	// Generated message map functions
protected:
	//{{AFX_MSG(CObj_Edit)
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
private:
	CBrush m_BrushFond;
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_OBJ_EDIT_H__10EFFB80_15D4_11D4_989E_004095420987__INCLUDED_)
