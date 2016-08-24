#if !defined(AFX_DLGSELECTIONPRODUIT_H__D23D05B6_40B2_424A_B678_8BD226A18638__INCLUDED_)
#define AFX_DLGSELECTIONPRODUIT_H__D23D05B6_40B2_424A_B678_8BD226A18638__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgSelectionProduit.h : header file
//

#include "ListBoxSelect.h"

/////////////////////////////////////////////////////////////////////////////
// CDlgSelectionProduit dialog

class CDlgSelectionProduit : public CDialog
{
// Construction
public:
	CDlgSelectionProduit(CWnd* pParent = NULL);   // standard constructor

	void UpdateLibelleFromList();
	void UpdateListFromLibelle();

	int GetNbCode();
	CString GetCode(int position);
	void AddCode(CString code);

// Dialog Data
	//{{AFX_DATA(CDlgSelectionProduit)
	enum { IDD = IDD_SELECTIONPRODUIT };
	CListBoxSelect	m_ListSelect;
	CObj_Gray	m_Cadre11;
	CObj_Gray	m_Cadre10;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgSelectionProduit)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgSelectionProduit)
	virtual void OnOK();
	virtual void OnCancel();
	afx_msg void OnAjout();
	afx_msg void OnSupp();
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	CStringArray m_Code;
	CStringArray m_Libelle;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGSELECTIONPRODUIT_H__D23D05B6_40B2_424A_B678_8BD226A18638__INCLUDED_)
