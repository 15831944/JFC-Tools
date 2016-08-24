#if !defined(AFX_DLGOPTIONCALCUL_H__D45C43C0_1B7F_11D4_989E_004095420987__INCLUDED_)
#define AFX_DLGOPTIONCALCUL_H__D45C43C0_1B7F_11D4_989E_004095420987__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgOptionCalcul.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlgOptionCalcul dialog

class CDlgOptionCalcul : public CDialog
{
// Construction
public:
	CDlgOptionCalcul(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDlgOptionCalcul)
	enum { IDD = IDD_OPTIONCALCUL };
	CObj_Gray	m_Cadre12;
	CObj_Gray	m_Cadre9;
	CObj_Gray	m_cadre8;
	CObj_Gray	m_Cadre4;
	CObj_Gray	m_Cadre3;
	CObj_Gray	m_Cadre2;
	int		m_Dememorisation;
	int		m_Saisonnalite;
	int		m_DureeTouteActive;
	int		m_CNCActif_Inactif;
	int		m_BaseRegionale;
	int		m_RadioAgglo;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgOptionCalcul)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgOptionCalcul)
	virtual void OnOK();
	afx_msg void OnPaint();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGOPTIONCALCUL_H__D45C43C0_1B7F_11D4_989E_004095420987__INCLUDED_)
