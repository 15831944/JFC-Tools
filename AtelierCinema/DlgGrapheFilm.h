#if !defined(AFX_DLGGRAPHEFILM_H__105E57E7_A130_4EEA_96C6_8DAFD823A1CE__INCLUDED_)
#define AFX_DLGGRAPHEFILM_H__105E57E7_A130_4EEA_96C6_8DAFD823A1CE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgGrapheFilm.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlgGrapheFilm dialog

#define LARGCOLONE_ACCOGRAP (16) // largeur de la colone pour les vagues
#define INTERECH_ACCOGRAP (3) // espace non utilisé dans l'echelle (haut et bas)

#include "campagnefilm.h"
#include "grillefilm_graphe.h"


class CDlgGrapheFilm : public CDialog
{
// Construction
public:
	CDlgGrapheFilm(CWnd* pParent = NULL);   // standard constructor

	void Initialise(CCampagneFilm * pCampagneFilm,int style);
	void InitAscenceur();
	CCampagneFilm * m_pCampagneFilm;

	void MajAffichageACCOGRAP();

// Dialog Data
	//{{AFX_DATA(CDlgGrapheFilm)
	enum { IDD = IDD_GRAPHEFILM };
	CScrollBar	        m_Scroll;
	CGrilleFilm_Graphe	m_GrilleFilm4;
	CGrilleFilm_Graphe	m_GrilleFilm3;
	CGrilleFilm_Graphe	m_GrilleFilm2;
	CGrilleFilm_Graphe	m_GrilleFilm1;
	CObj_Gray   	    m_Cadre;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgGrapheFilm)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgGrapheFilm)
	afx_msg void OnPaint();
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGGRAPHEFILM_H__105E57E7_A130_4EEA_96C6_8DAFD823A1CE__INCLUDED_)
