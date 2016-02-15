#if !defined(AFX_FORMULAIREIMPRESSION_H__7C292575_FE6B_4AEE_8A0D_A5C696D63703__INCLUDED_)
#define AFX_FORMULAIREIMPRESSION_H__7C292575_FE6B_4AEE_8A0D_A5C696D63703__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// FormulaireImpression.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CFormulaireImpression dialog

class CFormulaireImpression : public CDialog
{
// Construction
public:
	CFormulaireImpression(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CFormulaireImpression)
	enum { IDD = IDD_DIALOG_FORMULAIRE_IMPRESSION };
	CButton	m_BtnRadioAnnonceurMarque;
	CObj_Gray m_Cadre6;
	int		m_ChoixAnnonceurMarque;
	BOOL	m_CheckCampagne;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CFormulaireImpression)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CFormulaireImpression)
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_FORMULAIREIMPRESSION_H__7C292575_FE6B_4AEE_8A0D_A5C696D63703__INCLUDED_)
