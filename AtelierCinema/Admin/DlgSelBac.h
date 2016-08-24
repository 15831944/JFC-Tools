#if !defined(AFX_DLGSELBAC_H__A9EDC907_51FA_4262_ABAC_0FD70A995E7E__INCLUDED_)
#define AFX_DLGSELBAC_H__A9EDC907_51FA_4262_ABAC_0FD70A995E7E__INCLUDED_

#include "..\TableBAC.h"	// Added by ClassView
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgSelBac.h : header file
//
#include "TableBAC.h"

/////////////////////////////////////////////////////////////////////////////
// CDlgSelBac dialog

class CDlgSelBac : public CDialog
{
// Construction
public:
	CDlgSelBac(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDlgSelBac)
	enum { IDD = IDD_SELBAC };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgSelBac)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgSelBac)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	CTableBAC m_TableBac;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGSELBAC_H__A9EDC907_51FA_4262_ABAC_0FD70A995E7E__INCLUDED_)
