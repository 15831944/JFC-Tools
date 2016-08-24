#if !defined(AFX_DLGMULTICIBLE_H__885267CF_FD74_437D_BC7A_B7C9CD977C5B__INCLUDED_)
#define AFX_DLGMULTICIBLE_H__885267CF_FD74_437D_BC7A_B7C9CD977C5B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgMulticible.h : header file
//
#include "Campagne.h"

/////////////////////////////////////////////////////////////////////////////
// CDlgMulticible dialog

class CDlgMulticible : public CDialog
{
// Construction
public:
	CDlgMulticible(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDlgMulticible)
	enum { IDD = IDD_MULTICIBLE };
	CComboBox	m_Cible5;
	CComboBox	m_Cible4;
	CComboBox	m_Cible3;
	CComboBox	m_Cible2;
	CComboBox	m_Cible1;
	CObj_Gray	m_Cadre;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgMulticible)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
public:
	int m_NumeroCible [NBCIBLECALCUL];

	// Generated message map functions
	//{{AFX_MSG(CDlgMulticible)
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg void OnSelchangeCible1();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	int GetNumero(CString libelle);
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGMULTICIBLE_H__885267CF_FD74_437D_BC7A_B7C9CD977C5B__INCLUDED_)
