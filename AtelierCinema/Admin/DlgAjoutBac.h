#if !defined(AFX_DLGAJOUTBAC_H__A20CC7CC_5E20_4D52_A05D_7AF30467D3D0__INCLUDED_)
#define AFX_DLGAJOUTBAC_H__A20CC7CC_5E20_4D52_A05D_7AF30467D3D0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgAjoutBac.h : header file
//
#include "TableBAC.h"
/////////////////////////////////////////////////////////////////////////////
// CDlgAjoutBac dialog

class CDlgAjoutBac : public CDialog
{
// Construction
public:
	CDlgAjoutBac(CWnd* pParent = NULL);   // standard constructor
	CTableBAC m_TableBac;

// Dialog Data
	//{{AFX_DATA(CDlgAjoutBac)
	enum { IDD = IDD_AJOUTBAC };
	CComboBox	m_ComboRegie;
	CEdit	m_TarifBAC;
	CEdit	m_AnneeTarif;
	CObj_Gray m_Cadre1;
	CEdit	m_CodeBac;
	CEdit	m_LibelleBac;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgAjoutBac)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Affichage du tarif courant pour ce BAC
	bool AfficheTarifBAC();

	// Ajoute le tarif du BAC
	void AjouteTarifBAC(CString CodeBAC,CString StrAnnee,double TarifUnique);

	// Affichage dernère année tarifée
	void InitAnneeTarif();

	// Date Corante / année courante
	COleDateTime m_DateCourante;

	// Generated message map functions
	//{{AFX_MSG(CDlgAjoutBac)
	afx_msg void OnOk();
	afx_msg void OnAnnuler();
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg void OnDeltaposSpinannee(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGAJOUTBAC_H__A20CC7CC_5E20_4D52_A05D_7AF30467D3D0__INCLUDED_)
