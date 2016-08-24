#if !defined(AFX_DLGSUPPCPMLXGROUPE_H__583B63DA_EC36_4B16_A9E2_33EC3015C7C0__INCLUDED_)
#define AFX_DLGSUPPCPMLXGROUPE_H__583B63DA_EC36_4B16_A9E2_33EC3015C7C0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgSuppCpmlxGroupe.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlgSuppCpmlxGroupe dialog

class CDlgSuppCpmlxGroupe : public CDialog
{
// Construction
public:
	CDlgSuppCpmlxGroupe(CWnd* pParent = NULL);   // standard constructor

	// table de données
	CTableComplexe m_TableComplexe;

	// Table des groupes de Complexes
	CTableGroupeComplexe m_TableGroupeComplexe;

	// Chargement de la liste des groupes de complexes
	bool SetListeGrpComplexes();

// Dialog Data
	//{{AFX_DATA(CDlgSuppCpmlxGroupe)
	enum { IDD = IDD_SUPPCMPLX_GROUPE };
	CListBox	m_ListGroupeCompo;
	CListBox	m_ListGroupeComplexes;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgSuppCpmlxGroupe)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgSuppCpmlxGroupe)
	virtual void OnOK();
	virtual BOOL OnInitDialog();
	afx_msg void OnSelchangeListcomplexeGroupe();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGSUPPCPMLXGROUPE_H__583B63DA_EC36_4B16_A9E2_33EC3015C7C0__INCLUDED_)
