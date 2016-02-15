#if !defined(AFX_FORMULAIREEDI_H__1DDB8163_08D5_11D7_BA51_00D0592329A7__INCLUDED_)
#define AFX_FORMULAIREEDI_H__1DDB8163_08D5_11D7_BA51_00D0592329A7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// FormulaireEDI.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CFormulaireEDI dialog

class CFormulaireEDI : public CDialog
{
// Construction
public:
	CFormulaireEDI(CWnd* pParent = NULL);   // standard constructor

	// titre de la fenetre informations EDI
	CString m_TitreFenEDI;

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

	// les infos interlocuteurs régie ou centrale d'achat
	struct SInfoContact
	{
		public :
		SInfoContact & operator=(const SInfoContact & Data)
		{
			m_NomContactTiers		= Data.m_NomContactTiers;
			m_PrenomContactTiers	= Data.m_PrenomContactTiers;
			m_AdrMailContactTiers   = Data.m_AdrMailContactTiers;
			return *this;
		}

		// Interlocuteur(s) tiers
		CString m_NomContactTiers;		// ** obligatoire (max 35 alphanum)
		CString m_PrenomContactTiers;	// ** obligatoire (max 35 alphanum)
		CString m_AdrMailContactTiers;	// ** obligatoire (max 35 alphanum)
	};
	
	typedef CArray <SInfoContact,SInfoContact> CInfoContactArray;

	CInfoContactArray m_TabInfoContact;

	SInfoContact m_InfoContact;

	// nombre de contact
	int m_NbContact;

	CString m_NoTelTiers;			// (max 35 alphanum)
	CString m_NoFaxTiers;			// (max 35 alphanum)

	// Index Interlocuteur courant
	int m_IndexContact;

	// Flag indiquant la conformité des infos saisis
	bool m_OkInfoEDI;

	// mode ajout (0) ou modification (1)
	int m_Mode;

// Dialog Data
	//{{AFX_DATA(CFormulaireEDI)
	enum { IDD = IDD_DIALOG_EDI };
	CButton	m_AddContact;
	CButton	m_CadreContact;
	CButton	m_SuivContact;
	CButton	m_PrecContact;
	CButton	m_SuppContact;
	CEdit	m_ChampsNoFax;
	CEdit	m_ChampsNoTel;
	CEdit	m_ChampsVille;
	CEdit	m_ChampsPrenomContact;
	CEdit	m_ChampsNomContact;
	CEdit	m_ChampsCodePostal;
	CEdit	m_ChampsAdrMailContact;
	CEdit	m_ChampsAdr2;
	CEdit	m_ChampsAdr1;
	CComboBox	m_ComboPays;
	CStatic	m_Oblig5;
	CStatic	m_Oblig4;
	CStatic	m_Oblig3;
	CStatic	m_Oblig2;
	CStatic	m_Oblig1;
	CEdit	m_ChampsTypeCode;
	CEdit	m_ChampsType;
	CEdit	m_ChampsNom;
	CEdit	m_ChampsCode;
	CEdit	m_ChampsCodeGLN;
	CObj_Gray m_Cadre1;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CFormulaireEDI)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Récup des champs existants
	void MajValChamps();

	// Recupére les valeurs controles >> variables m_Str
	void RenvoiValChamps();

	// Teste la validité des informations
	bool InformationEDIValid();

	// Validité de chaque champs
	bool ChampsConforme();

	// Saisie des champs obligatoire ???
	bool ChampsObligatoireOk();

	// Limitation taille texte de chaque champs de saisie
	void InitialiseMaxChamps();

	// Affichage valeur champs dans contrôle
	void AfficheValeurChamps(CEdit &EditText, CString &Texte);


// Constantes EDI
public:

	// Longueur maxi des libellés longs
	static const int m_MaxCarLongLib;
	static const int m_MaxCarSuperLongLib;

	// Et les autres longueurs libellés
	static const int m_MaxCarTypeCode;
	static const int m_MaxCarCodePays;
	static const int m_MaxCarTelFax;
	static const int m_MaxCarCodePostal;

protected:

	// Tableau des pays et abréviations pays
	struct SInfoPays
	{
		CString StrPays;	// nom du pays
		CString AbrPays;    // abréviation du pays sur 3 caracteres
	};

	CArray <SInfoPays,SInfoPays&> TInfoPays;

	// Chargement fichier pays
	bool ChargePays();

	// Récupère abréviation pays
	CString AbrPays();

	// Récupère Nom pays sélectionner via AbrPays
	CString CFormulaireEDI::StrPays(CString &AbrPays);

	// Routines test validité texte
	bool AlphaNumText(CString &Texte,bool OnlyNum,bool AcceptSpace,bool AcceptPonctuation);

	// Test champs contact utilisateur
	bool ChampsContactOk(SInfoContact &InfoContact);

	// Test Champs Adresse Mail
	bool AdrMailValid(CString &AdrMail);

	// Maj Info N° Interlocuteur
	void MajNoInterlocuteur();

	// Generated message map functions
	//{{AFX_MSG(CFormulaireEDI)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	afx_msg void OnPaint();
	afx_msg void OnSuppContact();
	afx_msg void OnPrecContact();
	afx_msg void OnSuivContact();
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnAddContact();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_FORMULAIREEDI_H__1DDB8163_08D5_11D7_BA51_00D0592329A7__INCLUDED_)
