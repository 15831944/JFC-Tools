#if !defined(AFX_GRILLE_PERFORMANCES_H__563E8940_16CE_11D4_989E_004095420987__INCLUDED_)
#define AFX_GRILLE_PERFORMANCES_H__563E8940_16CE_11D4_989E_004095420987__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Grille_Performances.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CGrille_Performances window
#include "campagne.h"

class CGrille_Performances : public CStatic
{
// Construction
public:
	CCampagne * m_pCampagne;
	CGrille_Performances();

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CGrille_Performances)
	//}}AFX_VIRTUAL

// Implementation
public:
	int m_NrCible;
	virtual ~CGrille_Performances();

	// Generated message map functions
protected:
	//{{AFX_MSG(CGrille_Performances)
	afx_msg void OnPaint();
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_GRILLE_PERFORMANCES_H__563E8940_16CE_11D4_989E_004095420987__INCLUDED_)
