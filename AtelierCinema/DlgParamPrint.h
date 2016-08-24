#if !defined(AFX_DLGPARAMPRINT_H__EA493C61_2CC3_11D4_989E_004095420987__INCLUDED_)
#define AFX_DLGPARAMPRINT_H__EA493C61_2CC3_11D4_989E_004095420987__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgParamPrint.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlgParamPrint dialog

#include "DlgMulticible.h"
class CDlgParamPrint : public CDialog
{
// Construction
public:
	CDlgMulticible m_DlgMulticible;
	CDlgParamPrint(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDlgParamPrint)
	enum { IDD = IDD_PARAMPRINT };
	CObj_Gray	m_CadreLibelle;
	CObj_Gray	m_Cadre;
	CString	m_NomCampagne;
	CString	m_NomCentrale;
	CString	m_NomProduit;
	CString	m_NomAnnonceur;
	BOOL	m_Graphe;
	BOOL	m_Calendrier;
	BOOL	m_Performances;
	BOOL	m_PerfAgglo;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgParamPrint)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgParamPrint)
	virtual void OnOK();
	afx_msg void OnPaint();
	afx_msg void OnCibles();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGPARAMPRINT_H__EA493C61_2CC3_11D4_989E_004095420987__INCLUDED_)
