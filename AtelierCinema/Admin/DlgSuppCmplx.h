#if !defined(AFX_DLGSUPPCMPLX_H__7B570010_E07A_4A5E_8365_89A36C99EEFB__INCLUDED_)
#define AFX_DLGSUPPCMPLX_H__7B570010_E07A_4A5E_8365_89A36C99EEFB__INCLUDED_

#include "TableComplexe.h"	// Added by ClassView
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgSuppCmplx.h : header file
//
#include "ListBox_Complexe.h"
/////////////////////////////////////////////////////////////////////////////
// CDlgSuppCmplx dialog

class CDlgSuppCmplx : public CDialog
{
// Construction
public:
	CDlgSuppCmplx(CWnd* pParent = NULL);   // standard constructor

	// table de données
	CTableComplexe m_TableComplexe;


// Dialog Data
	//{{AFX_DATA(CDlgSuppCmplx)
	enum { IDD = IDD_SUPPCMPLX };
	CObj_Gray			m_Cadre1;
	ListBox_Complexe	m_ListComplexe;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgSuppCmplx)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation

protected:
	// Generated message map functions
	//{{AFX_MSG(CDlgSuppCmplx)
	afx_msg void OnOk();
	afx_msg void OnAnnuler();
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGSUPPCMPLX_H__7B570010_E07A_4A5E_8365_89A36C99EEFB__INCLUDED_)
