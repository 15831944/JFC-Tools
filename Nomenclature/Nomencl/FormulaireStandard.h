#if !defined(AFX_FORMULAIRESTANDARD_H__E9D6C554_2610_4627_99B5_16CF623410DA__INCLUDED_)
#define AFX_FORMULAIRESTANDARD_H__E9D6C554_2610_4627_99B5_16CF623410DA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// FormulaireStandard.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CFormulaireStandard dialog

class CFormulaireStandard : public CDialog
{
// Construction
public:
	CString m_TitreFenStd;
	CFont m_FontTimeBold8;
	CFormulaireStandard(CWnd* pParent = NULL);   // standard constructor

	CString m_StrCodeFStd;
	CString m_StrAliasFStd;
	CString m_StrNomRepFStd;

	// Recup des champs existants
	void MajValChamps();

	// Renvoi les champs existants
	void RenvoiValChamps();

	// Indique s'il s'agit d'un Annonceur
	bool m_TypeAnnonceur;

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

	// Validité infos EDI
	bool m_OkInfoEDI;

	// Mode Ajout(0) ou mode modif (1)
	int m_Mode;

// Dialog Data
	//{{AFX_DATA(CFormulaireStandard)
	enum { IDD = IDD_DIALOG_FORMULAIRE_STANDARD };
	CButton	m_InfoEDI;
	CStatic	m_StaticRef2;
	CObj_Gray m_Cadre5;
	CEdit	m_ChampsCodeFStd;
	CEdit	m_ChampsAliasFStd;
	CEdit	m_ChampsNomRepFStd;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CFormulaireStandard)
	public:
	virtual int DoModal();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CFormulaireStandard)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	afx_msg void OnPaint();
	afx_msg void OnInfosAproposdenomenclature();
	afx_msg void OnInfoedi();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_FORMULAIRESTANDARD_H__E9D6C554_2610_4627_99B5_16CF623410DA__INCLUDED_)
