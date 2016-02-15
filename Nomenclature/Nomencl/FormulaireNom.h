#if !defined(AFX_FORMULAIRENOM_H__B26776C3_0134_4D8C_AF02_7C15FE4FF0D3__INCLUDED_)
#define AFX_FORMULAIRENOM_H__B26776C3_0134_4D8C_AF02_7C15FE4FF0D3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// FormulaireNom.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CFormulaireNom dialog

class CFormulaireNom : public CDialog
{
// Construction
public:
	CStringArray m_TabSecteur;
	CStringArray m_TabFamille;
	CString m_TitreFenN;
	int m_IdImageItem;
	CStringArray m_TabMarque;
	CFormulaireNom(CWnd* pParent = NULL);   // standard constructor
	CString m_StrAliasFN;

	// Recup des champs existants
	void MajValChamps();
	// Renvoi les champs existants
	void RenvoiValChamps();

// Dialog Data
	//{{AFX_DATA(CFormulaireNom)
	enum { IDD = IDD_DIALOG_FORMULAIRE_NOM };
	CObj_Gray m_Cadre3;
	CEdit	m_ChampsAliasFN;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CFormulaireNom)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CFormulaireNom)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	afx_msg void OnPaint();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_FORMULAIRENOM_H__B26776C3_0134_4D8C_AF02_7C15FE4FF0D3__INCLUDED_)
