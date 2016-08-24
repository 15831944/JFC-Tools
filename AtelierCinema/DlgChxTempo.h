#if !defined(AFX_DLGCHXTEMPO_H__4E96E8A0_267C_11D4_989E_004095420987__INCLUDED_)
#define AFX_DLGCHXTEMPO_H__4E96E8A0_267C_11D4_989E_004095420987__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgChxTempo.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlgChxTempo dialog

class CDlgChxTempo : public CDialog
{
// Construction
public:
	int m_Tempo;
	CDlgChxTempo(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDlgChxTempo)
	enum { IDD = IDD_CHOIXTEMPO };
	CObj_Gray	m_Cadre;
	CComboBox	m_CB;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgChxTempo)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgChxTempo)
	virtual void OnOK();
	afx_msg void OnPaint();
	virtual BOOL OnInitDialog();
	afx_msg void OnDblclkCombo1();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGCHXTEMPO_H__4E96E8A0_267C_11D4_989E_004095420987__INCLUDED_)
