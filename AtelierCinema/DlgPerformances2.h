#if !defined(AFX_DLGPERFORMANCES2_H__F27A9D20_2FF2_11D4_989E_004095420987__INCLUDED_)
#define AFX_DLGPERFORMANCES2_H__F27A9D20_2FF2_11D4_989E_004095420987__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgPerformances2.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlgPerformances2 dialog

#include "campagne.h"
#include "grille_performances.h"
#include "couleur.h"
class CDlgPerformances2 : public CDialog
{
// Construction
public:
	CDlgPerformances2(CWnd* pParent = NULL);   // standard constructor
	void Initialise(CCampagne * pCampagne);
	CCampagne * m_pCampagne;
	void MajAffichagePerf(void);
	void MajCouleur(void);


// Dialog Data
	//{{AFX_DATA(CDlgPerformances2)
	enum { IDD = IDD_PERFORMANCES2 };
	CCouleur	m_CoulHypValide2;
	CCouleur	m_CoulHypValide1;
	CCouleur	m_CoulHypActive2;
	CCouleur	m_CoulHypActive1;
	CGrille_Performances	m_Grille2;
	CGrille_Performances	m_Grille1;
	CObj_Gray	m_Cadre;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgPerformances2)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgPerformances2)
	afx_msg void OnPaint();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGPERFORMANCES2_H__F27A9D20_2FF2_11D4_989E_004095420987__INCLUDED_)
