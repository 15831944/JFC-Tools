#if !defined(AFX_DLGSELECTIONCOMPLEXE_H__811C1F15_F641_478A_9635_2DFBF390BD55__INCLUDED_)
#define AFX_DLGSELECTIONCOMPLEXE_H__811C1F15_F641_478A_9635_2DFBF390BD55__INCLUDED_

#include "..\TableComplexe.h"	// Added by ClassView
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgSelectionComplexe.h : header file
//
#include "ListBox_Complexe.h"
/////////////////////////////////////////////////////////////////////////////
// CDlgSelectionComplexe dialog

class CDlgSelectionComplexe : public CDialog
{
// Construction
public:
	CStringArray m_LibelleAjoute;
	CTableComplexe m_TableComplexe;
	CDlgSelectionComplexe(CWnd* pParent = NULL);   // standard constructor
	CWordArray  indexComplexe;
// Dialog Data
	//{{AFX_DATA(CDlgSelectionComplexe)
	enum { IDD = IDD_SELCOMPLEXE };
	CObj_Gray			m_Cadre1;
	ListBox_Complexe	m_ListComplexe;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgSelectionComplexe)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgSelectionComplexe)
	afx_msg void OnOk();
	afx_msg void OnAnnuler();
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	//CStringArray m_LibelleAjoute;

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGSELECTIONCOMPLEXE_H__811C1F15_F641_478A_9635_2DFBF390BD55__INCLUDED_)
