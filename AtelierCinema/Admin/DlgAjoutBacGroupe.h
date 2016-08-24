#if !defined(AFX_DLGAJOUTBACGROUPE_H__3AA63EAD_C129_49E1_9690_8A1324211CE0__INCLUDED_)
#define AFX_DLGAJOUTBACGROUPE_H__3AA63EAD_C129_49E1_9690_8A1324211CE0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgAjoutBacGroupe.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlgAjoutBacGroupe dialog

#include "TableBAC.h"
#include "TableGroupeBAC.h"


class CDlgAjoutBacGroupe : public CDialog
{
// Construction
public:
	CDlgAjoutBacGroupe(CWnd* pParent = NULL);   // standard constructor

	// Table des Bacs
	CTableBAC m_TableBac;

	// Table des groupes de Bacs
	CTableGroupeBAC m_TableGroupeBac;


// Dialog Data
	//{{AFX_DATA(CDlgAjoutBacGroupe)
	enum { IDD = IDD_AJOUT_BAC_GROUPE };
	CComboBox	m_ComboRegie;
	CEdit		m_NomGrpBacs;
	CObj_Gray	m_Cadre1;
	CObj_Gray	m_Cadre2;
	CListBox	m_ListeSelBacs;
	CListBox	m_ListeBacs;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgAjoutBacGroupe)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgAjoutBacGroupe)
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg void OnDblclkListbacdispo();
	afx_msg void OnDblclkListSelbacs();
	virtual void OnOK();
	afx_msg void OnFiltreRegie();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:

	// Test si groupe existant
	bool GroupeBacExistant(CString LibGroupe);

	// Recup infos composition groupe
	bool RecupTabCodeBacs(CArray<long, long> &CompoGrp);
	// bool RecupTabCodeBacs(CArray<TCHAR, TCHAR> &CompoGrp);

	// Sélection des bacs
	bool SetListeBacs(const CString regie="");
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGAJOUTBACGROUPE_H__3AA63EAD_C129_49E1_9690_8A1324211CE0__INCLUDED_)
