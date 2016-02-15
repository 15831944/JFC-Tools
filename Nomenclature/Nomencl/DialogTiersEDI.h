#if !defined(AFX_DIALOGTIERSEDI_H__3F574440_0A89_11D7_BA51_00D0592329A7__INCLUDED_)
#define AFX_DIALOGTIERSEDI_H__3F574440_0A89_11D7_BA51_00D0592329A7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DialogTiersEDI.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDialogTiersEDI dialog

class CDialogTiersEDI : public CDialog
{
// Construction
public:
	CDialogTiersEDI(CWnd* pParent = NULL);   // standard constructor

	// titre de la fenetre informations Régies / Centrales achat
	CString m_TitreFenTiersEDI;

	// type du tiers EDI (Régie ou Centrale Achat)
	CString m_TypeTiers;

// Dialog Data
	//{{AFX_DATA(CDialogTiersEDI)
	enum { IDD = IDD_DIALOG_TIERSEDI };
	CButton	m_SuppTiers;
	CButton	m_AjoutTiers;
	CListBox	m_ListTiersEDI;
	CObj_Gray m_Cadre1;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDialogTiersEDI)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// les infos interlocuteurs régie ou centrale d'achat
	struct SInfoContact
	{
		// Interlocuteur(s) tiers
		CString m_NomContactTiers;		// ** obligatoire (max 35 alphanum)
		CString m_PrenomContactTiers;	// ** obligatoire (max 35 alphanum)
		CString m_AdrMailContactTiers;	// ** obligatoire (max 35 alphanum)
	};
	
	SInfoContact m_InfoContact;

	typedef CArray <SInfoContact,SInfoContact> CInfoContactArray;

	struct SInfoGeneralEDI
	{
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
		CString m_NoTelTiers;			// (max 35 alphanum)
		CString m_NoFaxTiers;			// (max 35 alphanum)
	};

	// structure globale des infos tableau elmt EDI
	struct SInfoEDI
	{
		public :
	
		SInfoEDI & operator=(const SInfoEDI & Data)
		{
			m_InfoGeneralEDI = Data.m_InfoGeneralEDI;
			m_TContact.RemoveAll();
			for (int i=0;i<Data.m_TContact.GetSize();i++)
				m_TContact.Add(Data.m_TContact[i]);
			return *this;
		}

		SInfoGeneralEDI		m_InfoGeneralEDI;	   // Les infos élmt EDI
		CInfoContactArray	m_TContact;            // tableau des contacts
	};
	typedef CArray <SInfoEDI,SInfoEDI&> CInfoTabEDI;

	CInfoTabEDI m_TabInfoEDI;

	SInfoEDI m_InfoEDI;

	// Tableau des régies et abréviations régies
	struct SInfoRegie
	{
		CString StrRegie;	// nom complet de la régie
		CString AbrRegie;   // abréviation de la régie (5 car max)
	};

	CArray <SInfoRegie,SInfoRegie&> m_TInfoRegie;

	// Chargement fichier régie
	bool ChargeRegie();

	// Recherche nom régie via abr régie
	CString NomCompletRegie(CString &AbrRegie);

	// Recherche nom abrégé régie via libellé régie
	CString NomAbrRegie(CString &LibelleRegie);

	// Chargement des régies
	bool ChargeTiersEDI_Regie();

	// Chargement du tableau des infos régies
	bool ChargeTabInfoRegie();

	// Synchronistaion fichier Régie Atelier RAdio et Fichier Régie courant Nomenclature
	void SynchroniseFichiersRegieRadio();

	// Chargement des centrales d'achat
	bool ChargeTiersEDI_Centrale();

	// Modification d'une régie ou d'une centrale d'achat
	bool ModifieTiersEDI();

	// Suppression d'une régie ou d'une centrale d'achat
	bool SupprimeTiersEDI();

	// Sauvegarde elmts EDI courant dans fichier
	bool SaveElmtEDI();

	// Vérifie existence élmt EDI dans liste des élmts courants et renvoi indice tab élmts EDI
	bool ElmtEDIExist(CString &ElmtEDI,int &IndexTab,bool TypeRegie);

	// Generated message map functions
	//{{AFX_MSG(CDialogTiersEDI)
	afx_msg void OnAjouter();
	afx_msg void OnModifier();
	afx_msg void OnSupprimer();
	afx_msg void OnPaint();
	virtual BOOL OnInitDialog();
	afx_msg void OnDblclkListTiersedi();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DIALOGTIERSEDI_H__3F574440_0A89_11D7_BA51_00D0592329A7__INCLUDED_)
