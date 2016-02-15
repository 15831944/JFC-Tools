#if !defined(AFX_FORMULAIREUTILTAUX_H__E9E77261_B268_11D5_8A63_0010B5865AAB__INCLUDED_)
#define AFX_FORMULAIREUTILTAUX_H__E9E77261_B268_11D5_8A63_0010B5865AAB__INCLUDED_



#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// FormulaireUtilTaux.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CFormulaireUtilTaux dialog

class CFormulaireUtilTaux : public CDialog
{
// Construction
public:
	CFormulaireUtilTaux(CWnd* pParent = NULL);   // standard constructor
	bool m_MajUtilTaux;


	// Gestion des utilisateurs
	bool AjouteUtilisateur(CString Nom,CString MotDePasse);
	bool SupprimeUtilisateur(CString Nom,CString MotDePasse);
	bool UtilisateurDejaPresent(CString Nom,CString &ExMotDePasse,int &NoUtilisateur);

// Dialog Data
	//{{AFX_DATA(CFormulaireUtilTaux)
	enum { IDD = IDD_DIALOG_UTILTAUX };
	CEdit	m_EditMPUtilTaux;
	CEdit	m_EditUtilTaux;
	CButton	m_SuppUtilTaux;
	CButton	m_AjoutUtilTaux;
	CObj_Gray	m_Cadre10;
	CString	m_MotDePasseUtilTaux;
	CString	m_NomUtilTaux;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CFormulaireUtilTaux)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CFormulaireUtilTaux)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	afx_msg void OnAjoututiltaux();
	afx_msg void OnSupputiltaux();
	afx_msg void OnChangeMpUtiltaux();
	afx_msg void OnPaint();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_FORMULAIREUTILTAUX_H__E9E77261_B268_11D5_8A63_0010B5865AAB__INCLUDED_)
