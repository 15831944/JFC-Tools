#if !defined(AFX_DLGMODIFTEMPO_H__56FFA102_F685_46DD_B893_42A53EE07CEF__INCLUDED_)
#define AFX_DLGMODIFTEMPO_H__56FFA102_F685_46DD_B893_42A53EE07CEF__INCLUDED_

#include "..\TableTempo.h"	// Added by ClassView
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgModifTempo.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlgModifTempo dialog

class CDlgModifTempo : public CDialog
{
// Construction
public:
	CString m_OldLibelleTempo;
	CButton m_passage[10][2];
	CTableTempo m_TableTempo;
	CDlgModifTempo(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDlgModifTempo)
	enum { IDD = IDD_MODIFTEMPO };
	CObj_Gray	m_Cadre1;
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
	CListBox	m_ListTempo;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgModifTempo)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgModifTempo)
	afx_msg void OnOk();
	afx_msg void OnAnnuler();
	virtual BOOL OnInitDialog();
	afx_msg void OnSelchangeListtempo();
	afx_msg void OnChangeTempo();
	afx_msg void OnSelchangeComboperiode();
	afx_msg void OnPaint();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	int IsPassage();
	int m_indexTableTempo;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGMODIFTEMPO_H__56FFA102_F685_46DD_B893_42A53EE07CEF__INCLUDED_)
