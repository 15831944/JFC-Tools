#if !defined(AFX_DLGATTENTE_H__71B6C7E0_2B4C_11D4_989E_004095420987__INCLUDED_)
#define AFX_DLGATTENTE_H__71B6C7E0_2B4C_11D4_989E_004095420987__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgAttente.h : header file
//
/////////////////////////////////////////////////////////////////////////////
// CDlgAttente dialog

class CDlgAttente : public CDialog
{
// Construction
public:
	bool m_OptimisationTermine; // parfois le end dialog arrive avant même que la boite soit ouverte, ce qui provoque un bloquage.
	int m_StopRequest;
	bool m_FlagAnnuler;
	int		m_Progression;
	CDlgAttente(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDlgAttente)
	enum { IDD = IDD_ATTENTE };
	CGradientProgressCtrl	m_ProgressBar;
	CObj_Gray	m_Cadre2;
	CObj_Gray	m_Cadre;
	int		m_NbPlan1;
	int		m_NbPlan2;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgAttente)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgAttente)
	afx_msg void OnPaint();
	virtual void OnCancel();
	virtual BOOL OnInitDialog();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnStopper();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGATTENTE_H__71B6C7E0_2B4C_11D4_989E_004095420987__INCLUDED_)
