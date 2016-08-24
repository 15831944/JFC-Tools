#if !defined(AFX_DLGCHANGERPERIODE_H__747B9123_8587_11D2_AC40_0080C708A895__INCLUDED_)
#define AFX_DLGCHANGERPERIODE_H__747B9123_8587_11D2_AC40_0080C708A895__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgChangerPeriode.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlgChangerPeriode dialog

class CDlgChangerPeriode : public CDialog
{
// Construction
public:
	CDlgChangerPeriode(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDlgChangerPeriode)
	enum { IDD = IDD_CHANGER_PERIODE };
	CObj_Gray	m_Cadre;
	COleDateTime	m_DateDebut;
	int		m_NbSemaine;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgChangerPeriode)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgChangerPeriode)
	virtual BOOL OnInitDialog();
	afx_msg void OnDeltaposSpinDuree(NMHDR* pNMHDR, LRESULT* pResult);
	virtual void OnOK();
	afx_msg void OnPaint();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGCHANGERPERIODE_H__747B9123_8587_11D2_AC40_0080C708A895__INCLUDED_)
