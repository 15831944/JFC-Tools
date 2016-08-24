// ContainerCalendrierDlg.h : header file
//

#if !defined(AFX_CONTAINERCALENDRIERDLG_H__427C86D6_9557_47B5_9E86_3AF13BD83F40__INCLUDED_)
#define AFX_CONTAINERCALENDRIERDLG_H__427C86D6_9557_47B5_9E86_3AF13BD83F40__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CContainerCalendrierDlg dialog

class CContainerCalendrierDlg : public CDialog
{
// Construction
public:
	CContainerCalendrierDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CContainerCalendrierDlg)
	enum { IDD = IDD_CONTAINERCALENDRIER_DIALOG };
	COleDateTime	m_date;
	int		m_Semaine;
	int		m_Mois;
	int		m_Annee;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CContainerCalendrierDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CContainerCalendrierDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnDatetimechangeDatetimepicker1(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CONTAINERCALENDRIERDLG_H__427C86D6_9557_47B5_9E86_3AF13BD83F40__INCLUDED_)
