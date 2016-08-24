#if !defined(AFX_DLGAJOUTTEMPO_H__3506D7F9_A3A4_4991_9A3A_70321117E793__INCLUDED_)
#define AFX_DLGAJOUTTEMPO_H__3506D7F9_A3A4_4991_9A3A_70321117E793__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "Obj_Gray.h"
#include "..\TableTempo.h"	// Added by ClassView

// DlgAjoutTempo.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlgAjoutTempo dialog

class CDlgAjoutTempo : public CDialog
{
// Construction
public:
	int IsPassage();
	CTableTempo m_TableTempo;
	CDlgAjoutTempo(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDlgAjoutTempo)
	enum { IDD = IDD_AJOUTTEMPO };
	CButton	m_passage92;
	CButton	m_passage91;
	CButton	m_passage82;
	CButton	m_passage81;
	CButton	m_passage72;
	CButton	m_passage71;
	CButton	m_passage62;
	CButton	m_passage61;
	CButton	m_passage52;
	CButton	m_passage51;
	CButton	m_passage42;
	CButton	m_passage41;
	CButton	m_passage32;
	CButton	m_passage31;
	CButton	m_passage22;
	CButton	m_passage21;
	CButton	m_passage12;
	CButton	m_passage11;
	CButton	m_passage102;
	CButton	m_passage101;
	CEdit	m_libtempo;
	CComboBox	m_periode;
	CObj_Gray	m_cadre;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgAjoutTempo)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgAjoutTempo)
	afx_msg void OnAnnuler();
	afx_msg void OnOk();
	virtual BOOL OnInitDialog();
	afx_msg void OnSelchangeComboperiode();
	afx_msg void OnPaint();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	CButton m_passage[10][2];
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGAJOUTTEMPO_H__3506D7F9_A3A4_4991_9A3A_70321117E793__INCLUDED_)
