#if !defined(AFX_DLGPERFAGGLO_H__E52F892C_A8A9_423D_837F_A2E86BB37ECC__INCLUDED_)
#define AFX_DLGPERFAGGLO_H__E52F892C_A8A9_423D_837F_A2E86BB37ECC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgPerfAgglo.h : header file
//

#include "campagne.h"
#include "grille_perfagglo.h"


/////////////////////////////////////////////////////////////////////////////
// CDlgPerfAgglo dialog

class CDlgPerfAgglo : public CDialog
{
// Construction
public:
	CDlgPerfAgglo(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDlgPerfAgglo)
	enum { IDD = IDD_PERFAGLO };
	CObj_Gray	m_Cadre;
	CGrille_PerfAgglo	m_Grille;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgPerfAgglo)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgPerfAgglo)
	afx_msg void OnPaint();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

public:
	void Initialise(CCampagneAgglo * pCampagne);
	CCampagne * m_pCampagne;
	void MajAffichagePerf(void);


};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGPERFAGGLO_H__E52F892C_A8A9_423D_837F_A2E86BB37ECC__INCLUDED_)
