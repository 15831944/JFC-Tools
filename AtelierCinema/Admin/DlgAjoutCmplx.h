#if !defined(AFX_DLGAJOUTCMPLX_H__B728B93B_BB79_4FBF_B1DC_A27A22F61CAF__INCLUDED_)
#define AFX_DLGAJOUTCMPLX_H__B728B93B_BB79_4FBF_B1DC_A27A22F61CAF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgAjoutCmplx.h : header file
#include "TableComplexe.h"

/////////////////////////////////////////////////////////////////////////////
// CDlgAjoutCmplx dialog

class CDlgAjoutCmplx : public CDialog
{
// Construction
public:
	CDlgAjoutCmplx(CWnd* pParent = NULL);   // standard constructor
	// table de données
	CTableComplexe m_TableComplexe;
// Dialog Data
	//{{AFX_DATA(CDlgAjoutCmplx)
	enum { IDD = IDD_AJOUTCMPLX };
	CEdit	m_EditCNC;
	CObj_Gray			m_Cadre1;
	CSpinButtonCtrl	m_ChangeAnnee;
	CEdit	m_AnneeTarif;
	CEdit	m_TarifComplexe;
	CComboBox	m_ComboRegie;
	CEdit	m_LibComplexe;
	CEdit	m_CodeComplexe;
	CComboBox	m_ComboBac;
	CListBox	m_ListComplexe;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgAjoutCmplx)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Affichage du tarif courant pour cet élémt
	bool AfficheTarifCmplx();

	// Ajoute le tarif du complexe
	void AjouteTarifCmplx(CString CodeComplexe,CString StrAnnee,double TarifUnique);

	// Ajoute le nombre d'entrée du nouveau complexe
	void AjouteCNCCmplx(CString CodeComplexe,int NbEntree);

	// Affichage dernère année tarifée
	void InitAnneeTarif();

	// Date Corante / année courante
	COleDateTime m_DateCourante;

	// Generated message map functions
	//{{AFX_MSG(CDlgAjoutCmplx)
	afx_msg void OnOk();
	virtual BOOL OnInitDialog();
	afx_msg void OnAnnuler();
	afx_msg void OnPaint();
	afx_msg void OnDeltaposSpinannee(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnChangeCodecomplexe();
	afx_msg void OnChangeEditCnc();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGAJOUTCMPLX_H__B728B93B_BB79_4FBF_B1DC_A27A22F61CAF__INCLUDED_)
