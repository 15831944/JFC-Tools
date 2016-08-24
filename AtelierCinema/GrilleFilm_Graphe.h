#if !defined(AFX_GRILLEFILM_GRAPHE_H__79EDF166_08EB_49C4_9CD3_6F6241A9BEA1__INCLUDED_)
#define AFX_GRILLEFILM_GRAPHE_H__79EDF166_08EB_49C4_9CD3_6F6241A9BEA1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// GrilleFilm_Graphe.h : header file
//

#include "campagnefilm.h"

/////////////////////////////////////////////////////////////////////////////
// CGrilleFilm_Graphe window

class CGrilleFilm_Graphe : public CStatic
{
// Construction
public:
	CCampagneFilm * m_pCampagneFilm;
	CGrilleFilm_Graphe();

	void SetTextACCOGRAP(HDC hDC, char * txt , RECT  R);
	void DessineBitmapACCOGRAP(HDC hDC,RECT R,HBITMAP hBmp);
	void CalculEchelleACCOGRAP();

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CGrilleFilm_Graphe)
	//}}AFX_VIRTUAL

// Implementation
public:

	// Paramètrage grille graphe
	int m_Part;  //0:Calendrier 1:Tom 2:Echelle 3:grille
	int m_Style; // 0: semaine 1:mois(panorama)

	// Dimensionnement
	int m_LargColone;
	int m_NbColone;
	int m_HauteurGraphe;
	int m_OrigineY;
	int m_HautLigneTitre;

	int m_PosAscH;


	double m_Echelle; // 100%
	int m_NbCran;
	int m_NbPixCran;
	int m_NbInterCran;
	float m_CoeffY;
	
	virtual ~CGrilleFilm_Graphe();

	// Generated message map functions
protected:
	//{{AFX_MSG(CGrilleFilm_Graphe)
	afx_msg void OnPaint();
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_GRILLEFILM_GRAPHE_H__79EDF166_08EB_49C4_9CD3_6F6241A9BEA1__INCLUDED_)
