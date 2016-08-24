#if !defined(AFX_DLGMAINTENANCE_H__54BB7A00_321D_11D4_989E_004095420987__INCLUDED_)
#define AFX_DLGMAINTENANCE_H__54BB7A00_321D_11D4_989E_004095420987__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgMaintenance.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlgMaintenance dialog

class CDlgMaintenance : public CDialog
{
// Construction
public:
	void UpdateVisu();
	CDlgMaintenance(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDlgMaintenance)
	enum { IDD = IDD_MAINTENANCE };
	CComboBox	m_CBAnnee;
	CObj_Gray	m_Cadre1;
	int		m_TarifSaisonDuree;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgMaintenance)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgMaintenance)
	virtual BOOL OnInitDialog();
	afx_msg void OnRadio1();
	afx_msg void OnRadio2();
	afx_msg void OnRadio3();
	afx_msg void OnSelchangeComboannee();
	afx_msg void OnPaint();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	CGXGridWnd m_wndGrid;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGMAINTENANCE_H__54BB7A00_321D_11D4_989E_004095420987__INCLUDED_)
