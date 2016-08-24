#if !defined(AFX_DLGPRODUITFILM_H__68F13154_F3CD_434E_864B_762599EA21F8__INCLUDED_)
#define AFX_DLGPRODUITFILM_H__68F13154_F3CD_434E_864B_762599EA21F8__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgProduitFilm.h : header file
//

#include "CampagneFilm.h"
#include "GridCoeffUsure.h" 
#include "GridCoeffModa.h"

#define RGB_CoeffUsurePremSem RGB(200,200,200)
#define RGB_CoeffFreq         RGB(216,243,255)


/////////////////////////////////////////////////////////////////////////////
// CDlgProduitFilm dialog

class CDlgProduitFilm : public CDialog
{
// Construction
public:
	CDlgProduitFilm(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDlgProduitFilm)
	enum { IDD = IDD_FORMULAIRE_FILM };
	CEdit	m_EditTitre;
	CButton	m_BtnSuppFilm;
	CComboBox	m_CmbCritere;
	CComboBox	 m_CmbReseauDistr;
	CObj_Gray	 m_Cadre3;
	CObj_Gray	 m_Cadre2;
	CObj_Gray	 m_Cadre1;
	COleDateTime m_DateDeb;
	long	     m_NbEntreeCNC;
	CString	m_Titre;
	long	m_Tarif;
	int		m_NbSemaine;
	CString	m_NoFilm;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgProduitFilm)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// attributes
public:

	// Grid affichage et modification coeff usure
	CGridCoeffUsure m_GridCoeffUsure;

	// Grid affichage et modification des coefficients modalités
	CGridCoeffModa m_GridCoeffModa;

	// Environnement campagne film
	CCampagneFilm  *m_pCampagneFilm;

	// Mode boite de dialogue
	bool m_ModePlusieurFilm;

	// Numéro du film à afficher
	int m_NumFilm;

	// N° de la page film courante
	int m_NumPageFilmCur;

	// Nb tot pages
	int m_NbTotPageFilm;

	// Code produit pour le réseau de dsitribution
	CString m_CodeReseauDistr;


private:

	
	// Initialisation de la page saisie (NoFilm = 0   / Init page vierge)
	//									( NoFilm > 0  / Init avec ex info films					
	void InitPageSaisie(int NoFilm,bool OnlyMaj);

	// Initialisation des infos film
	void InitInfoFilm(int NoFilm);

	// Initialisation Grid Coefficients Usure par semaines
	void InitGridCoeffUsure();

	// Initialisation Grid Coefficients Modalités
	void InitGridCoeffModa();

	// Chargement des modalités critère courant
	bool LoadGridCritere();

	// Chargement des critères disponibles
	bool ChargeCritere();

	// Recherche le n° du critère
	int GetCodeCritere(CString LibCritere);

	// Chargement des réseaux de distribution
	bool LoadReseauDistribution();

	// Mise à jour grille usure
	void UpdateGrilleUsure(int NoFilm);

	// Récupération des coeffs fréq et pluri-fréq
	void UpdateGrilleModa(int NoFilm);

	// Changement de la date 
	bool ChangeDateFilm();
	COleDateTime ExDateDebut;

	// Recherche le libellé du produit
	CString GetLibProduit(CString CodeProduit);

	// Recherche le code du produit
	CString GetCodeProduit(CString LibProduit);

	// Récupération tableau coeffs usure
	void RecupTabCoeffUsure(CArray<float,float&> &TCoeffUsure);

	// Récupération tableau coeffs fréquence et pluri-fréq.
	void RecupTabCoeffModalite(CArray<float,float&> &TCoeffFreq,CArray<float,float&> &TCoeffPluriFreq);

	// sauve les infos film en cours	
	void SaveProduitFilm(int NoFilm);

	// Infos colonnes
	enum ColonneGridFilm{ColSelectFilm= 0,ColLibFilm=1,ColTarif=2,ColCNC=3};
	enum ColonneGridModa{ColLibModa=0,ColFreq=1,ColPluriFreq=2,};

	// Test si tous les champs obligatoires saisis
	bool SaisieProduitOk();

	// Test si titre film existant
	bool TitreFilmExistant(CString TitreFilm);


// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgProduitFilm)
	afx_msg void OnSelchangeComboReseaudistr();
	afx_msg void OnPrecFilm();
	afx_msg void OnPaint();
	virtual BOOL OnInitDialog();
	afx_msg void OnSuivFilm();
	afx_msg void OnDeltaposSpinNbsem(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnCloseupDatetime(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnSuppfilm();
	virtual void OnOK();
	afx_msg void OnSelchangeComboCritere();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGPRODUITFILM_H__68F13154_F3CD_434E_864B_762599EA21F8__INCLUDED_)
