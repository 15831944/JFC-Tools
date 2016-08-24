#if !defined(AFX_DLGCRITERE_H__24CA7706_4074_4492_8B32_5F3A840C16AC__INCLUDED_)
#define AFX_DLGCRITERE_H__24CA7706_4074_4492_8B32_5F3A840C16AC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgCritere.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlgCritere dialog

class CDlgCritere : public CDialog
{
// Construction
public:
	CDlgCritere(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDlgCritere)
	enum { IDD = IDD_CRITERE };
	CListBox	m_ListCritere;
	CEdit	    m_CritereBase;
	CObj_Gray	m_Cadre1;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgCritere)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// attributes
public :

	// N° du critère de base et nombre de moadalités associés
	int m_NoCritere;
	int m_NbModalite;

private :

	// Chargement des critères disponibles
	bool ChargeCritere();

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgCritere)
	afx_msg void OnPaint();
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	afx_msg void OnSelchangeListcritere();
	afx_msg void OnDblclkListcritere();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGCRITERE_H__24CA7706_4074_4492_8B32_5F3A840C16AC__INCLUDED_)
