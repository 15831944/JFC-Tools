#if !defined(AFX_DLGSUPPBAC_H__1E1B71BB_4748_4C16_84F0_E67B98199A19__INCLUDED_)
#define AFX_DLGSUPPBAC_H__1E1B71BB_4748_4C16_84F0_E67B98199A19__INCLUDED_

#include "..\TableBAC.h"	// Added by ClassView
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgSuppBac.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlgSuppBac dialog

class CDlgSuppBac : public CDialog
{
// Construction
public:
	CTableBAC m_TableBac;
	CDlgSuppBac(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDlgSuppBac)
	enum { IDD = IDD_SUPPBAC };
	CListBox	m_ListBac;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgSuppBac)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgSuppBac)
	afx_msg void OnOk();
	afx_msg void OnAnnuler();
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGSUPPBAC_H__1E1B71BB_4748_4C16_84F0_E67B98199A19__INCLUDED_)
