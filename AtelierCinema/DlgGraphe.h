#if !defined(AFX_DLGGRAPHE_H__23804001_1C61_11D4_989E_004095420987__INCLUDED_)
#define AFX_DLGGRAPHE_H__23804001_1C61_11D4_989E_004095420987__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgGraphe.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlgGraphe dialog


#define LARGCOLONE_ACCOGRAP (16) // largeur de la colone pour les vagues
#define INTERECH_ACCOGRAP (3) // espace non utilisé dans l'echelle (haut et bas)
#define POSXECHELLE_ACCOGRAP (50) // trait verticale de l'echelle
#define NBPIXLEFTCRAN_ACCOGRAP (5) // largeur du cran à gauche
#define NBPIXRIGHTCRAN_ACCOGRAP (5) // largeur du cran à droite
#define NBPIXLEFTCRANI_ACCOGRAP (0) // largeur du cran intermédiaire à gauche
#define NBPIXRIGHTCRANI_ACCOGRAP (4) // largeur du cran intermédiaire à droite

#include "campagne.h"
#include "grille_graphe.h"

class CDlgGraphe : public CDialog
{
// Construction
public:
	void InitAscenceur();
	CDlgGraphe(CWnd* pParent = NULL);   // standard constructor
	void Initialise(CCampagne * pCampagne,int style);
	CCampagne * m_pCampagne;
	void MajAffichageACCOGRAP();

// Dialog Data
	//{{AFX_DATA(CDlgGraphe)
	enum { IDD = IDD_GRAPHE };
	CScrollBar	m_Scroll;
	CGrille_Graphe	m_Grille4;
	CGrille_Graphe	m_Grille3;
	CGrille_Graphe	m_Grille2;
	CGrille_Graphe	m_Grille1;
	CObj_Gray	m_Cadre;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgGraphe)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgGraphe)
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGGRAPHE_H__23804001_1C61_11D4_989E_004095420987__INCLUDED_)
