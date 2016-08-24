#if !defined(AFX_DLGMODIFPRODUIT_H__9AED55A4_8977_4749_B303_F97054FEAEF7__INCLUDED_)
#define AFX_DLGMODIFPRODUIT_H__9AED55A4_8977_4749_B303_F97054FEAEF7__INCLUDED_

#include "..\TableProduit.h"	// Added by ClassView
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgModifProduit.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlgModifProduit dialog

class CDlgModifProduit : public CDialog
{
// Construction
public:
	CTableProduit m_TableProduit;
	CDlgModifProduit(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDlgModifProduit)
	enum { IDD = IDD_MODIFPRODUIT };
	CListBox	m_ListProduit;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgModifProduit)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgModifProduit)
	afx_msg void OnOk();
	afx_msg void OnAnnuler();
	virtual BOOL OnInitDialog();
	afx_msg void OnSelchangeListproduit();
	afx_msg void OnPaint();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGMODIFPRODUIT_H__9AED55A4_8977_4749_B303_F97054FEAEF7__INCLUDED_)
