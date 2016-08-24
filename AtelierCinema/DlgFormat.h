#if !defined(AFX_DLGDUREE_H__D45C43C3_1B7F_11D4_989E_004095420987__INCLUDED_)
#define AFX_DLGDUREE_H__D45C43C3_1B7F_11D4_989E_004095420987__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgDuree.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlgDuree dialog

class CDlgFormat : public CDialog
{
// Construction
public:
	CStringArray m_ListFormat;
	CWordArray m_IndexFormatCampagne;

	CDlgFormat(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDlgDuree)
	enum { IDD = IDD_DUREE };
	CObj_Gray	m_Duree;
	CListBox	m_LB2;
	CListBox	m_LB1;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgDuree)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgDuree)
	virtual void OnOK();
	afx_msg void OnSelchangeList1();
	afx_msg void OnSelchangeList2();
	afx_msg void OnPaint();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGDUREE_H__D45C43C3_1B7F_11D4_989E_004095420987__INCLUDED_)
