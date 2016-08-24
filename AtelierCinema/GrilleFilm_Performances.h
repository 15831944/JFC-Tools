#if !defined(AFX_GRILLEFILM_PERFORMANCES_H__1F3298C7_58B1_44E1_AEC5_666CD3A95DFB__INCLUDED_)
#define AFX_GRILLEFILM_PERFORMANCES_H__1F3298C7_58B1_44E1_AEC5_666CD3A95DFB__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// GrilleFilm_Performances.h : header file
//

#include "campagnefilm.h"

/////////////////////////////////////////////////////////////////////////////
// CGrilleFilm_Performances window

class CGrilleFilm_Performances : public CStatic
{
// Construction
public:
	CGrilleFilm_Performances();

	// La campagne films
	CCampagneFilm * m_pCampagneFilm;

// Attributes
public:


// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CGrilleFilm_Performances)
	//}}AFX_VIRTUAL

// Implementation
public:
	int m_NrCible;
	virtual ~CGrilleFilm_Performances();

	// Generated message map functions
protected:
	//{{AFX_MSG(CGrilleFilm_Performances)
	afx_msg void OnPaint();
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_GRILLEFILM_PERFORMANCES_H__1F3298C7_58B1_44E1_AEC5_666CD3A95DFB__INCLUDED_)
