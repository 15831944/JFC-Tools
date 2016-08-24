#if !defined(AFX_DLGBUDGETNET_H__D45C43C1_1B7F_11D4_989E_004095420987__INCLUDED_)
#define AFX_DLGBUDGETNET_H__D45C43C1_1B7F_11D4_989E_004095420987__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgBudgetNet.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlgBudgetNet dialog

class CDlgBudgetNet : public CDialog
{
// Construction
public:
	CDlgBudgetNet(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDlgBudgetNet)
	enum { IDD = IDD_BUDGETNET };
	CObj_Gray	m_Cadre;
	UINT	m_BudgetNet;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgBudgetNet)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgBudgetNet)
	virtual void OnOK();
	afx_msg void OnPaint();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGBUDGETNET_H__D45C43C1_1B7F_11D4_989E_004095420987__INCLUDED_)
