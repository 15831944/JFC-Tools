// MainFrm.h : interface of the CMainFrame class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_MAINFRM_H__180773FB_D2BF_403F_83FD_2B95258F070D__INCLUDED_)
#define AFX_MAINFRM_H__180773FB_D2BF_403F_83FD_2B95258F070D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "ChildView.h"

class CMainFrame : public CFrameWnd
{
	
public:
	CMainFrame();
protected: 
	DECLARE_DYNAMIC(CMainFrame)

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMainFrame)
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual BOOL OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CMainFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:  // control bar embedded members
	CStatusBar  m_wndStatusBar;
	CToolBar    m_wndToolBar;
	CChildView    m_wndView;

// Generated message map functions
protected:
	//{{AFX_MSG(CMainFrame)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSetFocus(CWnd *pOldWnd);
	afx_msg void OnComplexeAjout();
	afx_msg void OnComplexeModifier();
	afx_msg void OnComplexeSupprimer();

	afx_msg void OnComplexeAjoutGroupe(); 
	afx_msg void OnComplexeModifierGroupe();
	afx_msg void OnComplexeSupprimerGroupe(); 

	afx_msg void OnBaseMettrejour();
	afx_msg void OnBacAjouter();
	afx_msg void OnBacModifier();
	afx_msg void OnBacSupprimer();

	afx_msg void OnBacAjoutGroupe(); 
	afx_msg void OnBacModifierGroupe();
	afx_msg void OnBacSupprimerGroupe();

	afx_msg void OnClose();
	afx_msg void OnProduitAjouter();
	afx_msg void OnProduitModifier();
	afx_msg void OnProduitSupprimer();
	afx_msg void OnTempoAjouter();
	afx_msg void OnTempoModifier();
	afx_msg void OnTempoSupprimer();
	afx_msg void OnFormatAjouter();
	afx_msg void OnFormatSupprimer();
	afx_msg void OnComplexeImportdutarif();
	afx_msg void OnBacImportdutarif();
	afx_msg void OnProduitImportdutarif();
	afx_msg void OnDonnescncImporter();
	afx_msg void ImporterSaisonnalite();
	afx_msg void OnProduitAjoutedepuis();
	afx_msg void OnComplexeEditmodifieTarif();
	afx_msg void OnBacEditmodifieTarif();
	afx_msg void OnProduitEditmodifieTarif();
	afx_msg void OnDonneescncModif();
	afx_msg void OnPaint();
	afx_msg void OnProduitfilmsAjoutedepuis();
	afx_msg void OnProduitfilmsAjouter();
	afx_msg void OnProduitfilmsEditermodifiertarif();
	afx_msg void OnProduitfilmsImportdutarif();
	afx_msg void OnProduitfilmsModifier();
	afx_msg void OnProduitfilmsSupprimer();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MAINFRM_H__180773FB_D2BF_403F_83FD_2B95258F070D__INCLUDED_)
