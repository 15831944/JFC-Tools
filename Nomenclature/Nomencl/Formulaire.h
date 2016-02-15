#if !defined(AFX_FORMULAIRE_H__6259C9D1_13F5_459A_A6D4_153881085E24__INCLUDED_)
#define AFX_FORMULAIRE_H__6259C9D1_13F5_459A_A6D4_153881085E24__INCLUDED_

#include "Noeud.h"	// Added by ClassView
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Formulaire.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CFormulaire dialog

class CFormulaire : public CDialog
{
// Construction
public:
	bool m_AnnonceurIsole;
	CString m_DateParDefaut;

	bool m_AnnonceurSansProduit;
	CString m_StrDateFP;
	CNoeud *m_pNoeudFamille;
	CStringArray m_TabSecteur;
	CString m_StrSecteurFP;
	CString m_StrFamilleFP;
	CStringArray m_TabFamille;
	CFont m_FontTimeBold8;
	CStringArray m_TabMarque;
	int m_Formulaire;
	CString m_StrCodeFP;
	CString m_StrAliasFP;
	CString m_StrNomRepFP;
	CString m_StrMarqueFP;
	CString m_TitreFenFP;
	CFormulaire(CWnd* pParent = NULL);   // standard constructor
	enum TypeFormulaire{FormulaireStandard = 0,FormulaireProduit,FormulaireNom};

	// s'agit t-il d'un annonceur ??
	bool m_TypeAnnonceur; 

	// Recup des champs existants
	void MajValChamps();

	// Renvoi les champs existants
	void RenvoiValChamps();

	// Remplissage liste secteurs via Famille courante (pour les produits)
	void RemplirComboSecteur(CString Famille);

    // Les infos Tiers EDI
	CString m_TypeTiers;			// type tiers (PO/centrale,BY/annonceur,DS/Regie)
	CString m_CodeTiers;			// ** obligatoire (max 35 alphanum)
	CString m_TypeCodeTiers;		// tjrs 107 (faut pas chercher à comprendre)
	CString m_NomTiers;				// ** obligatoire en fait nom annonceur ou centrale ou régie (max 35 alphanum)
	CString m_AdresseTiers1;		// 1ere partie adresse tiers ((max 35 alphanum)
	CString m_AdresseTiers2;		// 1ere partie adresse tiers ((max 35 alphanum)
	CString m_CodePostalTiers;		// (max 9 alphanum)
	CString m_VilleTiers;			// (max 35 alphanum)
	CString m_CodePaysTiers;		// (max 3 alphanum via un combo-box pays de correspondance)

	// Interlocuteur Annonceur
	CString m_NomContactTiers;		// ** obligatoire (max 35 alphanum)
	CString m_PrenomContactTiers;	// ** obligatoire (max 35 alphanum)
	CString m_AdrMailContactTiers;	// ** obligatoire (max 35 alphanum)
	
	CString m_NoTelTiers;			// (max 35 alphanum)
	CString m_NoFaxTiers;			// (max 35 alphanum)

	// Validité infos EDI
	bool m_OkInfoEDI;

	// Mode ajout (0) ou mode modification (1)
	int m_Mode;

// Dialog Data
	//{{AFX_DATA(CFormulaire)
	enum { IDD = IDD_DIALOG_FORMULAIRE_PRODUIT };
	CButton	m_InfoEDI;
	CStatic	m_AnneeCur;
	CEdit	m_ChampsDateFP;
	CComboBox	m_ComboSecteur;
	CComboBox	m_ComboFamille;
	CStatic	m_StaticRef;
	CObj_Gray m_Cadre4;
	CEdit	m_ChampsCodeFP;
	CEdit	m_ChampsAliasFP;
	CEdit	m_ChampsNomRepFP;
	CComboBox	m_ComboMarque;
	//}}AFX_DATA

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CFormulaire)
	public:

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CFormulaire)
	virtual void OnOK();
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg void OnSelchangeCombofamille2();
	afx_msg void OnInfoedi();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_FORMULAIRE_H__6259C9D1_13F5_459A_A6D4_153881085E24__INCLUDED_)
