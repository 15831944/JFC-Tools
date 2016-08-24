#if !defined(AFX_DLAJOUTFORMAT_H__0151974C_39B5_45F1_8B2A_FFF15AE20AEC__INCLUDED_)
#define AFX_DLAJOUTFORMAT_H__0151974C_39B5_45F1_8B2A_FFF15AE20AEC__INCLUDED_

#include "..\TableFormat.h"	// Added by ClassView
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlAjoutFormat.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlAjoutFormat dialog

class CDlAjoutFormat : public CDialog
{
// Construction
public:
	CTableFormat m_TableFormat;
	CDlAjoutFormat(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDlAjoutFormat)
	enum { IDD = IDD_AJOUTFORMAT };
	CObj_Gray	m_Cadre1;
	CEdit		m_coeff;
	CEdit		m_libelle;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlAjoutFormat)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlAjoutFormat)
	afx_msg void OnOk();
	afx_msg void OnAnnuler();
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLAJOUTFORMAT_H__0151974C_39B5_45F1_8B2A_FFF15AE20AEC__INCLUDED_)
