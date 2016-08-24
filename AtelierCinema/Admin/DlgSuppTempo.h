#if !defined(AFX_DLGSUPPTEMPO_H__9B02EDEF_2EC3_4F04_B4BF_5B40D5E95339__INCLUDED_)
#define AFX_DLGSUPPTEMPO_H__9B02EDEF_2EC3_4F04_B4BF_5B40D5E95339__INCLUDED_

#include "..\TableTempo.h"	// Added by ClassView
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgSuppTempo.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlgSuppTempo dialog

class CDlgSuppTempo : public CDialog
{
// Construction
public:
	CTableTempo m_TableTempo;
	CDlgSuppTempo(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDlgSuppTempo)
	enum { IDD = IDD_SUPPTEMPO };
	CListBox	m_ListTempo;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgSuppTempo)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgSuppTempo)
	virtual BOOL OnInitDialog();
	afx_msg void OnOk();
	afx_msg void OnAnnuler();
	afx_msg void OnPaint();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGSUPPTEMPO_H__9B02EDEF_2EC3_4F04_B4BF_5B40D5E95339__INCLUDED_)
