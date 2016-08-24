#if !defined(AFX_DLGSELECTCHEMIN_H__14B0D2A6_0745_4ED6_8E6D_87DB2CC48745__INCLUDED_)
#define AFX_DLGSELECTCHEMIN_H__14B0D2A6_0745_4ED6_8E6D_87DB2CC48745__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgSelectChemin.h : header file
//
#include "browedit.h"

/////////////////////////////////////////////////////////////////////////////
// CDlgSelectChemin dialog

class CDlgSelectChemin : public CDialog
{

// Construction
public:
	CString m_OldChemin;
	CDlgSelectChemin(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDlgSelectChemin)
	enum { IDD = IDD_MODIFCHEMIN };
	CObj_Gray m_Cadre1;
	CString	m_chemin;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgSelectChemin)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgSelectChemin)
	afx_msg void OnOk();
	afx_msg void OnAnnuler();
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	SECBrowseDirEdit m_editPath;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGSELECTCHEMIN_H__14B0D2A6_0745_4ED6_8E6D_87DB2CC48745__INCLUDED_)
