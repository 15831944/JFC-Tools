#if !defined(AFX_DLGSELCOMPLEXE_H__CDF65F54_8461_4FE6_8D92_946F12BB1202__INCLUDED_)
#define AFX_DLGSELCOMPLEXE_H__CDF65F54_8461_4FE6_8D92_946F12BB1202__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgSelComplexe.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlgSelComplexe dialog
#include "ListBox_Complexe.h"
#include "ListBoxSelect.h"

class CDlgSelComplexe : public CDialog
{
// Construction
public:
	CStringArray& GetLibelleAjoute();
	void SetLibelleAjoute(CStringArray& libelle);
	bool SetListe(int option);
	CDlgSelComplexe(CWnd* pParent = NULL);   // standard constructor

	// Chargement de la liste des groupes de complexes
	bool SetListeGrpComplexes();

// Dialog Data
	//{{AFX_DATA(CDlgSelComplexe)
	enum { IDD = IDD_SELCOMPLEXE };
	CComboBox			m_combo;
	CObj_Gray			m_CadreOption;
	CObj_Gray			m_cadre;
	CObj_Gray			m_cadregrp;
	ListBox_Complexe	m_ListComplexe; //Objet de type ListBox_Complexe
	CListBoxSelect  	m_ListGrpComplexe;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgSelComplexe)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgSelComplexe)
	afx_msg void OnAnnuler();
	afx_msg void OnOk();
	afx_msg void OnSeltout();
	afx_msg void OnInverser();
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg void OnFiltreRegie();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	int m_type;
	CStringArray m_LibelleAjoute;

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGSELCOMPLEXE_H__CDF65F54_8461_4FE6_8D92_946F12BB1202__INCLUDED_)
