#if !defined(AFX_GRILLE_GRAPHE_H__23804002_1C61_11D4_989E_004095420987__INCLUDED_)
#define AFX_GRILLE_GRAPHE_H__23804002_1C61_11D4_989E_004095420987__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Grille_Graphe.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CGrille_Graphe window



// paramètres pysique
#define LARGCOLONE_ACCOGRAP (16) // largeur de la colone pour les vagues
#define INTERECH_ACCOGRAP (3) // espace non utilisé dans l'echelle (haut et bas)
#define POSXECHELLE_ACCOGRAP (50) // trait verticale de l'echelle
#define NBPIXLEFTCRAN_ACCOGRAP (5) // largeur du cran à gauche
#define NBPIXRIGHTCRAN_ACCOGRAP (5) // largeur du cran à droite
#define NBPIXLEFTCRANI_ACCOGRAP (0) // largeur du cran intermédiaire à gauche
#define NBPIXRIGHTCRANI_ACCOGRAP (4) // largeur du cran intermédiaire à droite

#include "campagne.h"

class CGrille_Graphe : public CStatic
{
// Construction
public:
	CCampagne * m_pCampagne;
	CGrille_Graphe();

	void SetTextACCOGRAP(HDC hDC, char * txt , RECT  R);
	void DessineBitmapACCOGRAP(HDC hDC,RECT R,HBITMAP hBmp);
	void CalculEchelleACCOGRAP();

	
	// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CGrille_Graphe)
	//}}AFX_VIRTUAL

// Implementation
public:
	int m_Part; //0:Calendrier 1:Tom 2:Echelle 3:grille
	int m_Style; // 0: semaine 1:mois(panorama)

	int m_LargColone;
	int m_NbColone;
	int m_HauteurGraphe;
	int m_OrigineY;
	int m_HautLigneTitre;

	int m_PosAscH;
//	HWND m_hAscH;


	double m_Echelle; // 100%
	int m_NbCran;
	int m_NbPixCran;
	int m_NbInterCran;
	float m_CoeffY;
	

	virtual ~CGrille_Graphe();

	// Generated message map functions
protected:
	//{{AFX_MSG(CGrille_Graphe)
	afx_msg void OnPaint();
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_GRILLE_GRAPHE_H__23804002_1C61_11D4_989E_004095420987__INCLUDED_)
