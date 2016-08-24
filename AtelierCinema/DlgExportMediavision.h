#if !defined(AFX_DLGEXPORTMEDIAVISION_H__D8596C81_70C4_4921_8096_6CA9A89065BC__INCLUDED_)
#define AFX_DLGEXPORTMEDIAVISION_H__D8596C81_70C4_4921_8096_6CA9A89065BC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgExportMediavision.h : header file
//

#include "campagne.h"

/////////////////////////////////////////////////////////////////////////////
// CDlgExportMediavision dialog

class CDlgExportMediavision : public CDialog
{
// Construction
public:
	CDlgExportMediavision(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDlgExportMediavision)
	enum { IDD = IDD_EXPORT_MEDIAVISION };
	CListBox	m_LstSelCiblesExportM;
	CListBox	m_LstCiblesExportM;
	CObj_Gray	m_CadreCibles;
	CObj_Gray	m_CadreTitres;
	CString		m_DateDebut;
	CString		m_DateFin;
	CString		m_SsTitre;
	CString		m_Titre;
	//}}AFX_DATA

	// Tableau des ident cibles sélectionnées
	CArray <int,int&> m_TSelCibleExportM;

	// Pointeur sur la campagne en cours
	CCampagne *m_pCampagne;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgExportMediavision)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgExportMediavision)
	afx_msg void OnPaint();
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	afx_msg void OnDblclkListcibles();
	afx_msg void OnDblclkListselcibles();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

	// Chargement des cibles disponibles
	BOOL ChargeLstCible();

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGEXPORTMEDIAVISION_H__D8596C81_70C4_4921_8096_6CA9A89065BC__INCLUDED_)
