#if !defined(AFX_OBJ_TTLE_H__19E5FE81_1A43_11D2_843A_444553540000__INCLUDED_)
#define AFX_OBJ_TTLE_H__19E5FE81_1A43_11D2_843A_444553540000__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// Obj_ttle.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CObj_ttle window

class CObj_ttle : public CStatic
{
// Construction
public:
	CObj_ttle();

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CObj_ttle)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CObj_ttle();

	// Generated message map functions
protected:
	//{{AFX_MSG(CObj_ttle)
	afx_msg void OnPaint();
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
private:
	CFont m_font;
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_OBJ_TTLE_H__19E5FE81_1A43_11D2_843A_444553540000__INCLUDED_)
