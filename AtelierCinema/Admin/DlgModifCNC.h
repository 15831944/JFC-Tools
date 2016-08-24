#if !defined(AFX_DLGMODIFCNC_H__C3B5756A_7029_4D6F_9F38_63CD8D4A7E0E__INCLUDED_)
#define AFX_DLGMODIFCNC_H__C3B5756A_7029_4D6F_9F38_63CD8D4A7E0E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgModifCNC.h : header file
//

#include "ListBox_Complexe.h"
extern CAdminApp	theApp;

/////////////////////////////////////////////////////////////////////////////
// CDlgModifCNC dialog

class CDlgModifCNC : public CDialog
{
// Construction
public:
	CDlgModifCNC(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDlgModifCNC)
	enum { IDD = IDD_MODIFCNC };
	CEdit	m_NewCNC;
	ListBox_Complexe m_ListComplexe;
	CObj_Gray m_Cadre1;
	//CStatic	m_Cadre1;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgModifCNC)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Chargement des éléments
	void ChargeComplexeCNC(char Alpha);

	// Sauvegarde dernier CNC saisi
	void SaveCNC(CString CodeCmplx,CString NbEntree);

	// Transfert CNC en cours dans grille CNC
	void TransfertNewCNC();

	// Validation de la sélection >> affichage CNC Correspondant
	void AfficheCNC();

	// Derniere valeur CNC
	CString m_LastCNC;

	// Generated message map functions
	//{{AFX_MSG(CDlgModifCNC)
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg void OnSelchangeListComplexe();
	afx_msg void OnTest();
	afx_msg void OnA();
	afx_msg void OnB();
	afx_msg void OnC();
	afx_msg void OnD();
	afx_msg void OnE();
	afx_msg void OnF();
	afx_msg void OnG();
	afx_msg void OnH();
	afx_msg void OnI();
	afx_msg void OnJ();
	afx_msg void OnK();
	afx_msg void OnL();
	afx_msg void OnM();
	afx_msg void OnN();
	afx_msg void OnO();
	afx_msg void OnP();
	afx_msg void OnQ();
	afx_msg void OnR();
	afx_msg void OnS();
	afx_msg void OnT();
	afx_msg void OnU();
	afx_msg void OnV();
	afx_msg void OnW();
	afx_msg void OnX();
	afx_msg void OnY();
	afx_msg void OnZ();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGMODIFCNC_H__C3B5756A_7029_4D6F_9F38_63CD8D4A7E0E__INCLUDED_)
