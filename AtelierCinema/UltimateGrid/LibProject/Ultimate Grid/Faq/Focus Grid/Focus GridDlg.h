// Focus GridDlg.h : header file
//

#if !defined(AFX_FOCUSGRIDDLG_H__FEF15AC7_4FF1_11D3_9AE4_0020359647BF__INCLUDED_)
#define AFX_FOCUSGRIDDLG_H__FEF15AC7_4FF1_11D3_9AE4_0020359647BF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CFocusGridDlg dialog

#include "MyCug97.h"

class CFocusGridDlg : public CDialog
{
// Construction
public:
	CFocusGridDlg(CWnd* pParent = NULL);	// standard constructor
	MyCug m_grid;

// Dialog Data
	//{{AFX_DATA(CFocusGridDlg)
	enum { IDD = IDD_FOCUSGRID_DIALOG };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CFocusGridDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CFocusGridDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_FOCUSGRIDDLG_H__FEF15AC7_4FF1_11D3_9AE4_0020359647BF__INCLUDED_)
