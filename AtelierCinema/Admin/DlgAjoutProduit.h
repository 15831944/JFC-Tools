#if !defined(AFX_DLGAJOUTPRODUIT_H__83DF1E3C_708B_4B6F_A4A9_9180302C3EDC__INCLUDED_)
#define AFX_DLGAJOUTPRODUIT_H__83DF1E3C_708B_4B6F_A4A9_9180302C3EDC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgAjoutProduit.h : header file
//
#include "ListBoxSelect.h"
#include "ListBox_Complexe.h"

/////////////////////////////////////////////////////////////////////////////
// CDlgAjoutProduit dialog

class CDlgAjoutProduit : public CDialog
{
// Construction
public:

	int m_NouveauProd;
	int m_AjoutDepuis;
	CString m_OldCodeProduit;
	int m_duree; // la durée du produit en cours d'ajout ou de modification
	int IsComplexeAjoute(int idcomplexe,int semaine);
	int IsBacAjoute(int indexbac,int semaine);
	int m_indexModif;
	int m_modif;
	void InitModif(int IndexProduit);
	int IsAjouteBacDuComplexe(int index);
	int IdBacNewProduit(int indextablebac,int semaine);
	int IdCmplxNewProduit(int indextabcomplexe,int semaine);
	CDlgAjoutProduit(CWnd* pParent = NULL);   // standard constructor
	CTableProduit m_TableProduit;
	CTableBAC m_TableBac;
	CTableComplexe m_TableComplexe;

	CString m_OldTarif;

// Dialog Data
	//{{AFX_DATA(CDlgAjoutProduit)
	enum { IDD = IDD_AJOUTPRODUIT };
	CStatic	m_TarifNonModifiable;
	CEdit	m_AnneeTarif;
	CEdit	m_TarifProduit;
	CObj_Gray m_Cadre1;
	CListBoxSelect	m_ListBac;
	ListBox_Complexe	m_ListComplexe;
	CEdit	m_CodeProduit;
	CComboBox	m_Compo;
	CComboBox	m_Duree;
	CEdit	m_LibProduit;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgAjoutProduit)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Affichage du tarif courant pour ce produit
	bool AfficheTarifProduit();

	// Ajoute le tarif du Produit
	void AjouteTarifProduit(CString CodeProduit,CString StrAnnee,double TarifUnique);

	// Modifie le tarif du Produit
	void ModifTarifProduit(CString CodeProduit,CString StrAnnee,double TarifUnique);

	// Affichage dernère année tarifée
	void InitAnneeTarif();

	// Date Corante / année courante
	COleDateTime m_DateCourante;

	// Generated message map functions
	//{{AFX_MSG(CDlgAjoutProduit)
	afx_msg void OnOk();
	afx_msg void OnAnnuler();
	virtual BOOL OnInitDialog();
	afx_msg void OnAjoutcomplexe();
	afx_msg void OnAjoutbac();
	afx_msg void OnSelchangeComboduree();
	afx_msg void OnSupp();
	afx_msg void OnSelchangeCombocompo();
	afx_msg void OnSupp2();
	afx_msg void OnPaint();
	afx_msg void OnDeltaposSpinannee(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	CString m_OldLibelleProduit;
	void RemoveComposition(int nbsemaine);
	BOOL CreateComposition(int nbsemaine);

	// Liste des Complexes par semaine
	CWordArray * m_IndexComplexeSemaine;

	// Liste des bacs par semaine
	CWordArray * m_IndexBacSemaine;

	// Types des bacs 0(probabilisé),1(A),2(M)
	CWordArray * m_TypeBacSemaine;


};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGAJOUTPRODUIT_H__83DF1E3C_708B_4B6F_A4A9_9180302C3EDC__INCLUDED_)
