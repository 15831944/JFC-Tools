#if !defined(AFX_DLGFONCTIONCIBLE_H__AD1AB880_1555_11D4_989E_004095420987__INCLUDED_)
#define AFX_DLGFONCTIONCIBLE_H__AD1AB880_1555_11D4_989E_004095420987__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgFonctionCible.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlgFonctionCible dialog

class CDlgFonctionCible : public CDialog
{
// Construction
public:
	CString m_Nom;
	int m_NumCible;
	int m_Mode; // 0:charger 1:mémoriser
	CDlgFonctionCible(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDlgFonctionCible)
	enum { IDD = IDD_FONCTIONCIBLE };
	CButton	m_BtnDecharger;
	CObj_Gray	m_Cadre;
	CListBox	m_LB;
	CEdit	m_Edit;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgFonctionCible)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgFonctionCible)
	virtual BOOL OnInitDialog();
	afx_msg void OnSelchangeList1();
	virtual void OnOK();
	afx_msg void OnDblclkList1();
	afx_msg void OnPaint();
	afx_msg void OnDecharger();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGFONCTIONCIBLE_H__AD1AB880_1555_11D4_989E_004095420987__INCLUDED_)
