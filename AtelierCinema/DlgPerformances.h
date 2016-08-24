#if !defined(AFX_DLGPERFORMANCES_H__563E8941_16CE_11D4_989E_004095420987__INCLUDED_)
#define AFX_DLGPERFORMANCES_H__563E8941_16CE_11D4_989E_004095420987__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgPerformances.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlgPerformances dialog
#include "campagne.h"
#include "grille_performances.h"
#include "Couleur.h"

class CDlgPerformances : public CDialog
{
// Construction
public:
	void Initialise(CCampagne * pCampagne);

	CCampagne * m_pCampagne;

	CDlgPerformances(CWnd* pParent = NULL);   // standard constructor
	void MajAffichagePerf(void);
	void MajCouleur(void);

// Dialog Data
	//{{AFX_DATA(CDlgPerformances)
	enum { IDD = IDD_PERFORMANCES };
	CCouleur	m_CoulHypValide;
	CCouleur	m_CoulHypActive;
	CGrille_Performances	m_Grille;
	CObj_Gray	m_Cadre;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgPerformances)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgPerformances)
	afx_msg void OnPaint();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGPERFORMANCES_H__563E8941_16CE_11D4_989E_004095420987__INCLUDED_)
