#if !defined(AFX_DLGEXPORTSCREENVISION_H__CD731BA8_0C4E_499D_9744_0BCEF03C7C46__INCLUDED_)
#define AFX_DLGEXPORTSCREENVISION_H__CD731BA8_0C4E_499D_9744_0BCEF03C7C46__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgExportScreenvision.h : header file
//
#include "campagne.h"

/////////////////////////////////////////////////////////////////////////////
// CDlgExportScreenvision dialog

class CDlgExportScreenvision : public CDialog
{
// Construction
public:
	CDlgExportScreenvision(CWnd* pParent = NULL);   // standard constructor

	// Dialog Data
	//{{AFX_DATA(CDlgExportScreenvision)
	enum { IDD = IDD_EXPORT_SCREENVISION };
	CListBox	m_LstSelCiblesExportSV;
	CListBox	m_LstCiblesExportSV;
	CObj_Gray	m_CadreCibles;
	CObj_Gray	m_CadreTitres;
	CString		m_DateDebut;
	CString		m_DateFin;
	CString		m_SsTitre;
	CString		m_Titre;
	CString		m_CiblePerfSem;
	CStatic		m_CtrlCiblePerfSem;
	//}}AFX_DATA

	// Tableau des ident cibles sélectionnées
	CArray <int,int&> m_TSelCibleExportSV;

	// Index cible pour les performances semaines
	int m_IndexCiblePerfSem;

	// Pointeur sur la campagne en cours
	CCampagne *m_pCampagne;

	// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgExportScreenvision)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgExportScreenvision)
	afx_msg void OnPaint();
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	afx_msg void OnDblclkListcibles();
	afx_msg void OnDblclkListselcibles();
	afx_msg void OnSelchangeListselcibles();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

	// Chargement des cibles disponibles
	BOOL ChargeLstCible();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGEXPORTSCREENVISION_H__CD731BA8_0C4E_499D_9744_0BCEF03C7C46__INCLUDED_)
