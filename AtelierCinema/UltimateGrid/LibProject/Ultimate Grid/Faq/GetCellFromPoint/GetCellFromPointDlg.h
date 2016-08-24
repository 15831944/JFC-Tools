// GetCellFromPointDlg.h : header file
//

#if !defined(AFX_GETCELLFROMPOINTDLG_H__A482CA6D_3CC6_4DE0_9FC7_3DC2AC20868A__INCLUDED_)
#define AFX_GETCELLFROMPOINTDLG_H__A482CA6D_3CC6_4DE0_9FC7_3DC2AC20868A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CGetCellFromPointDlg dialog
#include "MyCug97.h"

class CGetCellFromPointDlg : public CDialog
{
// Construction
public:
	CGetCellFromPointDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CGetCellFromPointDlg)
	enum { IDD = IDD_GETCELLFROMPOINT_DIALOG };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CGetCellFromPointDlg)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;
	MyCug m_grid;

	// Generated message map functions
	//{{AFX_MSG(CGetCellFromPointDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_GETCELLFROMPOINTDLG_H__A482CA6D_3CC6_4DE0_9FC7_3DC2AC20868A__INCLUDED_)
