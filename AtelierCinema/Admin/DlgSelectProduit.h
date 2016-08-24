#if !defined(AFX_DLGSELECTPRODUIT_H__6AD5F02E_9205_478A_8307_693A964764AE__INCLUDED_)
#define AFX_DLGSELECTPRODUIT_H__6AD5F02E_9205_478A_8307_693A964764AE__INCLUDED_

#include "..\TableProduit.h"	// Added by ClassView
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgSelectProduit.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlgSelectProduit dialog

class CDlgSelectProduit : public CDialog
{
// Construction
public:
	CListBox m_ListProduit;
	CTableProduit m_TableProduit;
	CDlgSelectProduit(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDlgSelectProduit)
	enum { IDD = IDD_SELPRODUIT };
	
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgSelectProduit)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgSelectProduit)
	afx_msg void OnAnnuler();
	virtual BOOL OnInitDialog();
	afx_msg void OnOk();
	afx_msg void OnPaint();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGSELECTPRODUIT_H__6AD5F02E_9205_478A_8307_693A964764AE__INCLUDED_)
