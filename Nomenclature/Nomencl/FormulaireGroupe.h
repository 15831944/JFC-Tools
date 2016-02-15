#if !defined(AFX_FORMULAIREGROUPE_H__D60EDD72_A819_4C80_A0FC_CA42DCF9F63C__INCLUDED_)
#define AFX_FORMULAIREGROUPE_H__D60EDD72_A819_4C80_A0FC_CA42DCF9F63C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// FormulaireGroupe.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CFormulaireGroupe dialog

class CFormulaireGroupe : public CDialog
{
// Construction
public:
	CString m_DateParDefaut;
	CFont m_FontTimeBold8;
	CString m_TitreFenFG;
	CString m_StrDateFG;
	CString m_StrCodeFG;
	CString m_StrAliasFG;
	CString m_StrNomRepFG;
	CFormulaireGroupe(CWnd* pParent = NULL);   // standard constructor

	// Recup des champs existants
	void MajValChamps();


	// Renvoi les champs existants
	void RenvoiValChamps();

	// Annonceur ou pas
	bool m_Annonceur;

	// Mode ajout (0) ou modif (1)
	int m_Mode;

	// Récupération infos EDI Ok ou pas
	bool m_OkInfoEDI;

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

	// Interlocuteur(s) tiers
	CString m_NomContactTiers;		// ** obligatoire (max 35 alphanum)
	CString m_PrenomContactTiers;	// ** obligatoire (max 35 alphanum)
	CString m_AdrMailContactTiers;	// ** obligatoire (max 35 alphanum)
	
	CString m_NoTelTiers;			// (max 35 alphanum)
	CString m_NoFaxTiers;			// (max 35 alphanum)

// Dialog Data
	//{{AFX_DATA(CFormulaireGroupe)
	enum { IDD = IDD_DIALOG_FORMULAIRE_GROUPE };
	CStatic	m_AnneeCur;
	CObj_Gray m_Cadre7;
	CStatic	m_StaticRef4;
	CEdit	m_ChampsDateFG;
	CEdit	m_ChampsCodeFG;
	CEdit	m_ChampsAliasFG;
	CEdit	m_ChampsNomRepFG;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CFormulaireGroupe)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CFormulaireGroupe)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	afx_msg void OnPaint();
	afx_msg void OnInfoedi();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_FORMULAIREGROUPE_H__D60EDD72_A819_4C80_A0FC_CA42DCF9F63C__INCLUDED_)
