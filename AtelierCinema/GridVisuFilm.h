#if !defined(AFX_GRIDVISUFILM_H__131BC963_5AB8_49C1_8B2A_451D8F33DC42__INCLUDED_)
#define AFX_GRIDVISUFILM_H__131BC963_5AB8_49C1_8B2A_451D8F33DC42__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// GridVisuFilm.h : header file
//

#include "CampagneFilm.h"

/////////////////////////////////////////////////////////////////////////////
// CGridVisuFilm window

class CGridVisuFilm : public CGXGridWnd
{
// Construction
public:
	CGridVisuFilm();

	
// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CGridVisuFilm)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CGridVisuFilm();

	// Modification cellule
	virtual void OnModifyCell (ROWCOL nRow, ROWCOL nCol);

	// Initialisation campagne films associé
	void Init(CCampagneFilm * CampagneFilm);

	// Ligne courante valide
	ROWCOL m_RowCur;

private:
	CCampagneFilm * m_pCampagneFilm;


	// Generated message map functions
protected:
	//{{AFX_MSG(CGridVisuFilm)
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnActivateApp(BOOL bActive, HTASK hTask);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnAskCbFormatName(UINT nMaxCount, LPTSTR lpszString);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_GRIDVISUFILM_H__131BC963_5AB8_49C1_8B2A_451D8F33DC42__INCLUDED_)
