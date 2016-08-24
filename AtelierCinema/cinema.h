// cinema.h : main header file for the CINEMA application
//

#if !defined(AFX_CINEMA_H__33A58744_0AED_11D4_989E_004095420987__INCLUDED_)
#define AFX_CINEMA_H__33A58744_0AED_11D4_989E_004095420987__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"       // main symbols
#include "cible.h"
#include "basecinema.h"


/////////////////////////////////////////////////////////////////////////////
// CCinemaApp:
// See cinema.cpp for the implementation of this class
//

// Nombre de blocs BAC
#define NB_BLOC_BAC			4

// Les couleurs
#define NB_COULEURS			38
#define CLR_NOIR			0
#define CLR_BLANC			1
#define CLR_GRISF			2
#define CLR_GRISC			3
#define CLR_GRISCF			4
#define CLR_ROUGE			5
#define CLR_VERT			6
#define CLR_BLEUE			7
#define CLR_JAUNE			8
#define CLR_MDI				9
#define CLR_PAILLE			10
#define CLR_VERTPAILLE		11

#define CLR_FONDPASSAGE		12
#define CLR_FONDPRESENT		13
#define CLR_BORDPASSAGE		14
#define CLR_FONDPASSENDPL	15
#define CLR_FONDGRILLEINACTIF	16
#define CLR_FONDDLGGRAPH	17
#define CLR_FONDHYP1		18
#define CLR_FONDHYP2		19
#define CLR_FONDHYP3		20
#define CLR_FONDHYP4		21
#define CLR_FONDHYP5		22
#define CLR_HYPOTHESE1		23
#define CLR_HYPOTHESE2		24
#define CLR_HYPOTHESE3		25
#define CLR_HYPOTHESE4		26
#define CLR_HYPOTHESE5		27

#define CLR_FONDDLGPERF		28
#define CLR_FONDSELECTBINOME 30
#define CLR_FONDSELECTCALEND 31
#define CLR_TRAITSELECTCALEND 32
#define CLR_TRAITNRSEMAINEX	33
#define CLR_FONDNIVEAU1		34
#define CLR_FONDBOITEOFFRE	35
#define CLR_CELLULEGRISEE	36
#define CLR_PRODUITFIGE     37



// les fonts
#define NB_FONTS			6

#define FNT_COURIERD		0
#define FNT_HELV8			1
#define FNT_SMALL7B			2
#define FNT_SMALL6			3
#define FNT_COURIERN8		4
#define FNT_SMALL7			5



//#define NBSUPPORT 7 AVIRER
//#define NBPARCPROBA 14 AVIRER
#define PROBABILISATION (100)

#include "Campagne.h"
#include "CampagneFilm.h"

class CCinemaApp : public CWinApp
{
public:
	CCampagne	  * GetCampagne();
	CCampagneFilm * GetCampagneFilm();
	CStringArray m_LibMois;
	CString m_PostID;
	CString m_Chemin; // Chemin d'accès aux tables
	bool m_flagOptimisation;


	//bool LoadSaisons();
	int GetSaisons(int NrBinome, int Annee, int NrSemaine);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	void UnloadSource();
	bool LoadSource();
	BOOL LoadProba();
	void LibereProba();

	DWORD NbIndiv;
	double * CoeffEx;

	// table des données
	CBaseCinema m_TableCinema;

	// Nom de la vague
	CString m_NomVagueAtelierCinema;


	// DATAEVAL
	CStringArray m_CodeBacProbabilise;
	DWORD m_NbBacProbabilise;
	CMap <CString,LPCSTR,int,int&> m_MapIndexBac;
	DWORD *m_NbIndivSupp; // par support
	DWORD **m_NrIndivSupp;// par support, par individus(>64K)
	BYTE **m_Proba1;// par support, par individus(>64K)
	BYTE **m_Proba2;// par support, par individus(>64K)
	BYTE **m_Proba30;// par support, par individus(>64K)
	DWORD *m_NrIndivDirect; // par support, -1 si indiv non touché
	DWORD *m_NrBacParIndiv; // Nr de BAC pour chaque indiv, -1 si indiv non touché
	double * m_CoeffCorrectionCNC; // par [BAC]
	double *m_PT1,*m_PT0; //par [PROBABILISATION]

	// accelerateur des probas Matrice de Proba et Prime
	BYTE **m_NbMPP; // taille du MPP 2=0 et 1 fois...
	double ***m_MPP; // de zero à u
	// DATAEVAL

	CCinemaApp();
	void DeletePens();
	void CreatePens();
	void DeleteBrushes();
	void CreateBrushes();
	void InitColours();
	COLORREF GetRGB(char * Defcolors,char * NomColor);
	void DeleteFonts();
	void CreateFonts();


	COLORREF rgb[NB_COULEURS];
	HBRUSH	hBrush[NB_COULEURS];
	HPEN	hPen[NB_COULEURS],hPen2Noir;
	HFONT	hFont[NB_FONTS];

	HCURSOR hCurMove,hCurAjoutSupp,hCurGlissement;
	HCURSOR hCurBlocTempo,hCurBlocEfface,hCurBlocChange,hCurBlocMove;
	HCURSOR hCurInterdit,hCurBlocDeplacement,hCurBlocCopierColler;
	HCURSOR hCurWait;

	HBITMAP hBmpTom;
	
	HICON hIcoFleche,hIcoFleche2;

	CCible m_Cible;
	BOOL ChargerPoids();


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCinemaApp)
	public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();
	//}}AFX_VIRTUAL

// Implementation
	//{{AFX_MSG(CCinemaApp)
	afx_msg void OnAppAbout();
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	void CalculCoeffCorrectionCNC();
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CINEMA_H__33A58744_0AED_11D4_989E_004095420987__INCLUDED_)
