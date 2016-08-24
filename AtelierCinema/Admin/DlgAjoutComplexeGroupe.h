#if !defined(AFX_DLGAJOUTCOMPLEXEGROUPE_H__9AFA47D6_DA1C_407D_A695_C3107535AD54__INCLUDED_)
#define AFX_DLGAJOUTCOMPLEXEGROUPE_H__9AFA47D6_DA1C_407D_A695_C3107535AD54__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgAjoutComplexeGroupe.h : header file
//

#include "TableComplexe.h"	// Added by ClassView
#include "ListBox_Complexe.h"
#include "TableGroupeComplexe.h"

/////////////////////////////////////////////////////////////////////////////
// CDlgAjoutComplexeGroupe dialog

class CDlgAjoutComplexeGroupe : public CDialog
{
// Construction
public:
	CDlgAjoutComplexeGroupe(CWnd* pParent = NULL);   // standard constructor

	// table de données
	CTableComplexe m_TableComplexe;

	// Table des groupes de Complexes
	CTableGroupeComplexe m_TableGroupeComplexe;

// Dialog Data
	//{{AFX_DATA(CDlgAjoutComplexeGroupe)
	enum { IDD = IDD_AJOUT_COMPLEXE_GROUPE };
	CEdit				m_NomGroupeComplexe;
	CListBox			m_ListeSelComplexes;
	ListBox_Complexe	m_ListeComplexes;
	CObj_Gray			m_Cadre1;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgAjoutComplexeGroupe)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgAjoutComplexeGroupe)
	afx_msg void OnPaint();
	virtual BOOL OnInitDialog();
	afx_msg void OnCancelMode();
	afx_msg void OnDblclkListcompdispo();
	afx_msg void OnDblclkListSelcomps();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:

	// Test si groupe existant
	bool GroupeComplexeExistant(CString LibGroupe);

	// Recup infos composition groupe
	bool RecupTabCodeComplexes(CArray<int,int> &CompoGrp);
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGAJOUTCOMPLEXEGROUPE_H__9AFA47D6_DA1C_407D_A695_C3107535AD54__INCLUDED_)
