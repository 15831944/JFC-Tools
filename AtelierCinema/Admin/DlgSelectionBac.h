#if !defined(AFX_DLGSELECTIONBAC_H__55A530D6_4FD1_4689_82B5_F02B103F1EB4__INCLUDED_)
#define AFX_DLGSELECTIONBAC_H__55A530D6_4FD1_4689_82B5_F02B103F1EB4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgSelectionBac.h : header file
//
#include "TableBAC.h"
#include "ListBoxSelect.h"

/////////////////////////////////////////////////////////////////////////////
// CDlgSelectionBac dialog

class CDlgSelectionBac : public CDialog
{
// Construction
public:
	CStringArray m_BacAjoute;
	CDlgSelectionBac(CWnd* pParent = NULL);   // standard constructor
	CTableBAC m_TableBac;
	CWordArray  m_indexBac;
	int m_TypeRegie;
// Dialog Data
	//{{AFX_DATA(CDlgSelectionBac)
	enum { IDD = IDD_SELBAC };
	CObj_Gray	m_Cadre1;
	CComboBox	m_ComboType;
	CListBoxSelect	m_ListBac;
	//}}AFX_DATA

	// Implementation
	int GetBacType()
	{
		return m_TypeRegie;
	};

	bool SetListe(const CString regie);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgSelectionBac)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgSelectionBac)
	afx_msg void OnOk();
	afx_msg void OnAnnuler();
	virtual BOOL OnInitDialog();
	afx_msg void OnFiltreRegie();
	afx_msg void OnPaint();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGSELECTIONBAC_H__55A530D6_4FD1_4689_82B5_F02B103F1EB4__INCLUDED_)
