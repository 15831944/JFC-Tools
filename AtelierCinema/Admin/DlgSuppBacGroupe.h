#if !defined(AFX_DLGSUPPBACGROUPE_H__19B7C3C8_05B1_4F0D_9E85_CE4EB44870E8__INCLUDED_)
#define AFX_DLGSUPPBACGROUPE_H__19B7C3C8_05B1_4F0D_9E85_CE4EB44870E8__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgSuppBacGroupe.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlgSuppBacGroupe dialog

class CDlgSuppBacGroupe : public CDialog
{
// Construction
public:
	CDlgSuppBacGroupe(CWnd* pParent = NULL);   // standard constructor

	// Table des Bacs
	CTableBAC m_TableBac;

	// Table des groupes de Bacs
	CTableGroupeBAC m_TableGroupeBac;

	// Chargement de la liste des groupes de bacs
	bool SetListeGrpBacs();

// Dialog Data
	//{{AFX_DATA(CDlgSuppBacGroupe)
	enum { IDD = IDD_SUPPBAC_GROUPE };
	CListBox	m_ListCompoGroupe;
	CListBox	m_ListGroupeBacs;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgSuppBacGroupe)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgSuppBacGroupe)
	virtual void OnOK();
	virtual BOOL OnInitDialog();
	afx_msg void OnSelchangeListbacGroupe();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGSUPPBACGROUPE_H__19B7C3C8_05B1_4F0D_9E85_CE4EB44870E8__INCLUDED_)
