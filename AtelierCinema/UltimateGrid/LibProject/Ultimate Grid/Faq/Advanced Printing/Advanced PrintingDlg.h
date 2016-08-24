// Advanced PrintingDlg.h : header file
//

#if !defined(AFX_ADVANCEDPRINTINGDLG_H__B74D7337_C5D1_11D2_83D6_0020359647BF__INCLUDED_)
#define AFX_ADVANCEDPRINTINGDLG_H__B74D7337_C5D1_11D2_83D6_0020359647BF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "MyCug97.h"

/////////////////////////////////////////////////////////////////////////////
// CAdvancedPrintingDlg dialog

class CAdvancedPrintingDlg : public CDialog
{
// Construction
public:
	CAdvancedPrintingDlg(CWnd* pParent = NULL);	// standard constructor

	MyCug m_grid;
// Dialog Data
	//{{AFX_DATA(CAdvancedPrintingDlg)
	enum { IDD = IDD_ADVANCEDPRINTING_DIALOG };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAdvancedPrintingDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CAdvancedPrintingDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnPrintHeadings();
	afx_msg void OnPrintNoheadings();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ADVANCEDPRINTINGDLG_H__B74D7337_C5D1_11D2_83D6_0020359647BF__INCLUDED_)
