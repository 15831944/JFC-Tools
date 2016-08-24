#if !defined(AFX_DLGOPTIONCALCULMEMO_H__89D74F67_9329_477B_AEB7_E7A78A7666D2__INCLUDED_)
#define AFX_DLGOPTIONCALCULMEMO_H__89D74F67_9329_477B_AEB7_E7A78A7666D2__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgOptionCalculMemo.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlgOptionCalculMemo dialog

class CDlgOptionCalculMemo : public CDialog
{
// Construction
public:
	CDlgOptionCalculMemo(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDlgOptionCalculMemo)
	enum { IDD = IDD_OPTIONCALCULMEMO };
	int		m_DebutCalend;
	int		m_FinCalend;
	int		m_NrSemaineX;
	int		m_Dememorisation;
	CObj_Gray	m_Cadre1;
	CObj_Gray	m_Cadre2;
	CObj_Gray	m_Cadre3;
	CObj_Gray	m_Cadre9;
	int		m_BaseFilmRegionale;
	//}}AFX_DATA
	int m_NbSemaineCampagne;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgOptionCalculMemo)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgOptionCalculMemo)
	virtual void OnOK();
	afx_msg void OnPaint();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGOPTIONCALCULMEMO_H__89D74F67_9329_477B_AEB7_E7A78A7666D2__INCLUDED_)
