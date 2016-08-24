#if !defined(AFX_DLGSUPPFORMAT_H__B3165086_EDFE_4FD2_AC9A_75DF7E0A0963__INCLUDED_)
#define AFX_DLGSUPPFORMAT_H__B3165086_EDFE_4FD2_AC9A_75DF7E0A0963__INCLUDED_

#include "..\TableFormat.h"	// Added by ClassView
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgSuppFormat.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlgSuppFormat dialog

class CDlgSuppFormat : public CDialog
{
// Construction
public:
	CTableFormat m_TableFormat;
	CDlgSuppFormat(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDlgSuppFormat)
	enum { IDD = IDD_SUPPFORMAT };
	CListBox	m_ListFormat;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgSuppFormat)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgSuppFormat)
	afx_msg void OnOk();
	afx_msg void OnAnnuler();
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	void TriParDuree();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGSUPPFORMAT_H__B3165086_EDFE_4FD2_AC9A_75DF7E0A0963__INCLUDED_)
