#if !defined(AFX_DLGMODIFCMPLX_H__B327248F_4B3E_4C04_9E9F_2BEDC4C1D017__INCLUDED_)
#define AFX_DLGMODIFCMPLX_H__B327248F_4B3E_4C04_9E9F_2BEDC4C1D017__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgModifCmplx.h : header file
//
#include "TableComplexe.h"
#include "..\TableTempo.h"	// Added by ClassView

/////////////////////////////////////////////////////////////////////////////
// CDlgModifCmplx dialog

class CDlgModifCmplx : public CDialog
{
// Construction
public:
	CDlgModifCmplx(CWnd* pParent = NULL);   // standard constructor

	// Table complexe
	CTableComplexe m_TableComplexe;

// Dialog Data
	//{{AFX_DATA(CDlgModifCmplx)
	enum { IDD = IDD_MODIFCMPLX };
	CStatic	m_TarifNonModifiable;
	CEdit	m_AnneeTarif;
	CEdit	m_TarifComplexe;
	CObj_Gray	m_Cadre1;
	CComboBox	m_ComboRegie;
	CEdit	m_CodeComplexe;
	CEdit	m_LibelleComplexe;
	CComboBox	m_ComboBac;
	CListBox	m_ListComplexe;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgModifCmplx)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Affichage du tarif courant pour cet élémt
	bool AfficheTarifCmplx();

	// Sauve le nouveau tarif du complexe
	void ModifTarifCmplx(CString CodeCmplx,CString StrAnnee,double TarifUnique);

	// Affichage dernère année tarifée
	void InitAnneeTarif();

	// Date Corante / année courante
	COleDateTime m_DateCourante;

	// Generated message map functions
	//{{AFX_MSG(CDlgModifCmplx)
	afx_msg void OnOk();
	afx_msg void OnAnnuler();
	virtual BOOL OnInitDialog();
	afx_msg void OnSelchangeListcomplexe();
	afx_msg void OnChangeLibcomplexe();
	afx_msg void OnChangeCodecomplexe();
	afx_msg void OnSelchangeCombobac();
	afx_msg void OnPaint();
	afx_msg void OnDeltaposSpinannee(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	CString m_OldCodeComplexe;
	CString m_OldLibelleComplexe;
	CString m_OldRegie;
	CString m_OldCodeBac;
	CString m_OldTarif;
	int m_IndexComplexe;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGMODIFCMPLX_H__B327248F_4B3E_4C04_9E9F_2BEDC4C1D017__INCLUDED_)
