#if !defined(AFX_DLGSUPPPRODUIT_H__EFA934AE_3620_4F5A_88FE_04C7171DD411__INCLUDED_)
#define AFX_DLGSUPPPRODUIT_H__EFA934AE_3620_4F5A_88FE_04C7171DD411__INCLUDED_

#include "..\TableProduit.h"	// Added by ClassView
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgSuppProduit.h : header file
//
#include "TableProduit.h"

/////////////////////////////////////////////////////////////////////////////
// CDlgSuppProduit dialog

class CDlgSuppProduit : public CDialog
{
// Construction
public:
	CTableProduit m_TableProduit;
	CDlgSuppProduit(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDlgSuppProduit)
	enum { IDD = IDD_SUPPPRODUIT };
	CListBox	m_ListProduit;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgSuppProduit)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgSuppProduit)
	afx_msg void OnSelchangeListproduit();
	afx_msg void OnOk();
	afx_msg void OnAnnuler();
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGSUPPPRODUIT_H__EFA934AE_3620_4F5A_88FE_04C7171DD411__INCLUDED_)
