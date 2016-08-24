#if !defined(AFX_DLGSELTEMPO_H__1AD09AB5_6577_4370_AE81_5C90E6EEFA42__INCLUDED_)
#define AFX_DLGSELTEMPO_H__1AD09AB5_6577_4370_AE81_5C90E6EEFA42__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgSelTempo.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlgSelTempo dialog

class CDlgSelTempo : public CDialog
{
// Construction
public:
	CDlgSelTempo(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDlgSelTempo)
	enum { IDD = IDD_SELTEMPO };
	CListBox	m_ListTempo;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgSelTempo)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgSelTempo)
	afx_msg void OnAnnuler();
	afx_msg void OnOk();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGSELTEMPO_H__1AD09AB5_6577_4370_AE81_5C90E6EEFA42__INCLUDED_)
