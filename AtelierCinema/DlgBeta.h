#if !defined(AFX_DLGBETA_H__D45C43C2_1B7F_11D4_989E_004095420987__INCLUDED_)
#define AFX_DLGBETA_H__D45C43C2_1B7F_11D4_989E_004095420987__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgBeta.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlgBeta dialog

class CDlgBeta : public CDialog
{
// Construction
public:
	CDlgBeta(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDlgBeta)
	enum { IDD = IDD_BETA };
	CObj_Gray	m_Cadre;
	int		m_Beta;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgBeta)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgBeta)
	virtual void OnOK();
	afx_msg void OnPaint();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGBETA_H__D45C43C2_1B7F_11D4_989E_004095420987__INCLUDED_)
