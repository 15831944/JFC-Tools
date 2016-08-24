// maquetteDlg.h : header file
//

#if !defined(AFX_MAQUETTEDLG_H__F494D397_FFCD_4C00_B88B_7788F76F1C2C__INCLUDED_)
#define AFX_MAQUETTEDLG_H__F494D397_FFCD_4C00_B88B_7788F76F1C2C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CMaquetteDlg dialog

class CMaquetteDlg : public CDialog
{
// Construction
public:
	CMaquetteDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CMaquetteDlg)
	enum { IDD = IDD_MAQUETTE_DIALOG };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMaquetteDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CMaquetteDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnButton2();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MAQUETTEDLG_H__F494D397_FFCD_4C00_B88B_7788F76F1C2C__INCLUDED_)
