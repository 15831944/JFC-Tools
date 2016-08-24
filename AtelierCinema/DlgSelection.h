#if !defined(AFX_DLGSELECTION_H__BB968657_8D89_4C35_8AF6_B08FD96C28C9__INCLUDED_)
#define AFX_DLGSELECTION_H__BB968657_8D89_4C35_8AF6_B08FD96C28C9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgSelection.h : header file
//
#include "ListBoxSelect.h"

/////////////////////////////////////////////////////////////////////////////
// CDlgSelection dialog

class CDlgSelection : public CDialog
{
	
// Construction
public:
	void AddLibelle(CString libelle);

	void UpdateLibelleFromList();
	void UpdateListFromLibelle();
	int GetNbCode();
	CString GetCode(int position);
	void AddCode(CString code);
	CDlgSelection(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDlgSelection)
	enum { IDD = IDD_SELECTION };
	CObj_Gray	m_cadre2;
	CObj_Gray	m_cadre;
	CListBoxSelect	m_ListSelect;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgSelection)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgSelection)
	afx_msg void OnOk();
	afx_msg void OnAnnuler();
	afx_msg void OnAjoutproduit();
	afx_msg void OnAjoutcomplx();
	afx_msg void OnAjoutbac();
	afx_msg void OnSupp();
	afx_msg void OnDescendre();
	afx_msg void OnMonter();
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	CStringArray m_Code;
	CStringArray m_Libelle;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGSELECTION_H__BB968657_8D89_4C35_8AF6_B08FD96C28C9__INCLUDED_)
