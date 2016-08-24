#if !defined(AFX_DLGEDITMODIFTARIF_H__76AC5F9E_2EE2_447F_8D88_1D159301BE16__INCLUDED_)
#define AFX_DLGEDITMODIFTARIF_H__76AC5F9E_2EE2_447F_8D88_1D159301BE16__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgEditModifTarif.h : header file
//

#include "obj_edit.h"
#include "TableComplexe.h"
#include "TableBAC.h"
#include "TableProduit.h"
#include "ListBox_Complexe.h"

extern CAdminApp	theApp;

/////////////////////////////////////////////////////////////////////////////
// CDlgEditModifTarif dialog
class CDlgEditModifTarif : public CDialog
{
// Construction
public:
	CDlgEditModifTarif(const TCHAR* pszTitle = NULL, long MaxSel = 1, BOOL Sort = FALSE, CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDlgEditModifTarif)
	enum { IDD = IDD_EDITMODIFTARIF };
	CStatic	m_OptionRegie;
	CComboBox	m_ComboRegie;
	ListBox_Complexe	m_ListSemTarif;
	CEdit				m_AnneeTarif;
	CButton				m_FrameElmt;
	CComboBox			m_ComboElmt;
	CSpinButtonCtrl		m_SpinAnnee	;
	CEdit				m_SelTarif;
	CObj_Gray			m_Cadre1;
	//}}AFX_DATA

	// la fonction pour obtenir le flag de modification
	bool IsModified();

	// la fonction pour ajouter les éléments
	void AddItem(const TCHAR* pszItem);
	
public :	

	// N° associé à la boite dialogue (complexe ou Bac ou Produit)
	int m_TypeElmt;

	// Table des complexes
	CTableComplexe	m_TableComplexe;

	// Table des BACs
	CTableBAC		m_TableBAC;

	// Table des produits
	CTableProduit	m_TableProduit;

	// Date Corante / année courante
	COleDateTime m_DateCourante;

	// Flag modification tarif (pour sauvegarde ou pas des nouveaux tarifs)
	bool m_FlagTarifModif;

	// Regie courante
	int m_RegieCur;

	// Code élmt en cours
	CString m_CodeElmt;

	// No Tarif en cours dans table des tarifs
	int m_NoTarif;


// Implementation
protected:
	// la fonction pour trier les éléments
	void SortItems();

	// chargement des éléments tarifés
	bool ChargeElmt();

	// chargement liste des tarifs par semaine de l'élmt courant
	bool ChargeTarifElmt();

	// sauve provisoirement les tarifs pour un élmt (les 53 semaines)
	bool SaveTarifElmt();

	// récupère ancien tarif du type d'élémnt en question
	bool RecupOldTarif();

	// Affichage dernère année tarifée
	void InitAnneeTarif();

	// Dernier tarif en cours
	CString m_LastTarif;

	// elmt en cours à tarifer ou visu tarifs
	int m_ElmtActif;

	// Transfert Tarif en cours dans grille tarifaire
	void TransfertNewTarif();

	// Init boite édition saisie tarif
	void InitSaisieTarif();

	// Affiche Info Tarifs et autres
	void AfficheInfoElmt();

// Implementation
protected:
	// la fonction pour mettre à jour le libellé
	void UpdateLibelle();

// Implementation
protected:
	long m_MaxSel; // le nombre maximum de sélection

// Implementation
protected:
	CArray<long, long> m_SelItems; // la sélection des éléments
	CArray<long, long> m_SelExtrn; // la sélection externe

// Implementation
protected:
	CArray<long, long> m_Index; // les indices des éléments

// Implementation
protected:
	bool m_Modifie; // le flag de modification

// Implementation
protected:
	CStringArray m_Items; // les éléments
	CString      m_Title; // le titre

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgEditModifTarif)
	public:
	virtual BOOL OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CDlgEditModifTarif)
	virtual BOOL OnInitDialog();
	afx_msg void OnSelChange();
	virtual void OnOK();
	afx_msg void OnDeltaposSpinannee(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnSelchangeListSemtarif();
	afx_msg void OnKillfocusEditTarif();
	afx_msg void OnSelchangeComboElmt();
	afx_msg void OnSelchangeComboregie();
	afx_msg int OnVKeyToItem(UINT nKey, CListBox* pListBox, UINT nIndex);
	virtual void OnCancel();
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnPaint();
	afx_msg void OnEditupdateComboElmt();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGEDITMODIFTARIF_H__76AC5F9E_2EE2_447F_8D88_1D159301BE16__INCLUDED_)
