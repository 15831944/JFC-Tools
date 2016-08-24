#if !defined(AFX_DLGPERFORMANCEFILM_H__ED85A8D4_9A4D_4D7E_B361_B17E18F536CB__INCLUDED_)
#define AFX_DLGPERFORMANCEFILM_H__ED85A8D4_9A4D_4D7E_B361_B17E18F536CB__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgPerformanceFilm.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlgPerformanceFilm dialog

#include "campagnefilm.h"
#include "grillefilm_performances.h"

class CDlgPerformanceFilm : public CDialog
{
// Construction
public:
	CDlgPerformanceFilm(CWnd* pParent = NULL);   // standard constructor

	void Initialise(CCampagneFilm * pCampagneFilm);
	void MajAffichagePerf();

	CCampagneFilm * m_pCampagneFilm;

// Dialog Data
	//{{AFX_DATA(CDlgPerformanceFilm)
	enum { IDD = IDD_PERFORMANCESFILM };
	CObj_Gray	m_Cadre;
	CGrilleFilm_Performances	m_GrilleFilm;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgPerformanceFilm)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgPerformanceFilm)
	afx_msg void OnPaint();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGPERFORMANCEFILM_H__ED85A8D4_9A4D_4D7E_B361_B17E18F536CB__INCLUDED_)
