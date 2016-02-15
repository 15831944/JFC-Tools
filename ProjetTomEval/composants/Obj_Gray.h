#if !defined(AFX_OBJ_GRAY_H__19E5FE82_1A43_11D2_843A_444553540000__INCLUDED_)
#define AFX_OBJ_GRAY_H__19E5FE82_1A43_11D2_843A_444553540000__INCLUDED_

#include "MoveManager.h"	// Added by ClassView
#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// Obj_Gray.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CObj_Gray window

class CObj_Gray : public CStatic
{
// Construction
public:
	CObj_Gray();
	void ValideChild(CRect);
	void AddWindow(CWnd*,short style);
	void AddChildInRect(HWND);
	CRect m_MyRect;

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CObj_Gray)
	//}}AFX_VIRTUAL

// Implementation
public:
	CMoveManager * GetPrivateMoveManager();
	void SetFond(COLORREF Color);
	virtual ~CObj_Gray();

	// Generated message map functions
protected:
	//{{AFX_MSG(CObj_Gray)
	afx_msg void OnPaint();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnMove(int x, int y);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
private:
	COLORREF m_Fond;
	CMoveManager m_MovMan;
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_OBJ_GRAY_H__19E5FE82_1A43_11D2_843A_444553540000__INCLUDED_)
