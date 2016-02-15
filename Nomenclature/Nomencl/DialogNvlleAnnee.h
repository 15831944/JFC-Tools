#if !defined(AFX_DIALOGNVLLEANNEE_H__4F2FBAB8_8D88_4112_ACF8_809570755B59__INCLUDED_)
#define AFX_DIALOGNVLLEANNEE_H__4F2FBAB8_8D88_4112_ACF8_809570755B59__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DialogNvlleAnnee.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDialogNvlleAnnee dialog

class CDialogNvlleAnnee : public CDialog
{
// Construction
public:
	CString m_AnneeCur;
	bool m_FlagNvlleAnnee;
	CString m_TitreDialogAnnee;
	CString m_AnneeSelect;
	CStringArray m_TabAnneePossible;
	CDialogNvlleAnnee(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDialogNvlleAnnee)
	enum { IDD = IDD_DIALOG_NLLE_ANNEE };
	CObj_Gray	m_Cadre8;
	CListBox	m_ListAnneePossible;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDialogNvlleAnnee)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDialogNvlleAnnee)
	virtual void OnOK();
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DIALOGNVLLEANNEE_H__4F2FBAB8_8D88_4112_ACF8_809570755B59__INCLUDED_)
