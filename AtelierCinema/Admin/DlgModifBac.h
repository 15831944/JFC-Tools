#if !defined(AFX_DLGMODIFBAC_H__68B2CDBC_ECC0_4956_A947_7EEC35F28AF8__INCLUDED_)
#define AFX_DLGMODIFBAC_H__68B2CDBC_ECC0_4956_A947_7EEC35F28AF8__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgModifBac.h : header file
//
#include "TableBAC.h"
/////////////////////////////////////////////////////////////////////////////
// CDlgModifBac dialog

class CDlgModifBac : public CDialog
{
// Construction
public:
	CDlgModifBac(CWnd* pParent = NULL);   // standard constructor
	CTableBAC m_TableBac;
// Dialog Data
	//{{AFX_DATA(CDlgModifBac)
	enum { IDD = IDD_MODIFBAC };
	CStatic	m_TarifNonModifiable;
	CComboBox	m_ComboRegie;
	CEdit	m_TarifBAC;
	CEdit	m_AnneeTarif;
	CObj_Gray	m_Cadre1;
	CEdit	m_LibelleBac;
	CEdit	m_CodeBac;
	CListBox	m_ListBac;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgModifBac)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Ajoute Nouveau Tarif, nouveau Bac
	//void AjouteTarifBAC(CString CodeBAC,CString StrAnnee,double TarifUnique);

	// Modifie le tarif du BAC
	void ModifTarifBAC(CString CodeBAC,CString StrAnnee,double TarifUnique);

	// Affichage du tarif courant pour ce BAC
	bool AfficheTarifBAC();

	// Affichage dernère année tarifée
	void InitAnneeTarif();

	// Date Corante / année courante
	COleDateTime m_DateCourante;

	// Generated message map functions
	//{{AFX_MSG(CDlgModifBac)
	afx_msg void OnOk();
	afx_msg void OnAnnuler();
	virtual BOOL OnInitDialog();
	afx_msg void OnChangeCodebac();
	afx_msg void OnChangeLibbac();
	afx_msg void OnSelchangeListbac();
	afx_msg void OnPaint();
	afx_msg void OnDeltaposSpinannee(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnSelchangeComboregie();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	CString m_OldCodeBac;
	CString m_OldLibelleBac;
	CString m_OldRegieBac;
	CString m_OldTarif;
	int m_IndexBac;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGMODIFBAC_H__68B2CDBC_ECC0_4956_A947_7EEC35F28AF8__INCLUDED_)
