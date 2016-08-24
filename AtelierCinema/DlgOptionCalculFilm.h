#if !defined(AFX_DLGOPTIONCALCULFILM_H__9111893C_010A_4443_83E9_DFD211E12B40__INCLUDED_)
#define AFX_DLGOPTIONCALCULFILM_H__9111893C_010A_4443_83E9_DFD211E12B40__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgOptionCalculFilm.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlgOptionCalculFilm dialog

class CDlgOptionCalculFilm : public CDialog
{
// Construction
public:
	CDlgOptionCalculFilm(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDlgOptionCalculFilm)
	enum { IDD = IDD_OPTIONCALCULFILM };
	int		m_Dememorisation;
	CObj_Gray	m_Cadre2;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgOptionCalculFilm)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgOptionCalculFilm)
	virtual void OnOK();
	afx_msg void OnPaint();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGOPTIONCALCULFILM_H__9111893C_010A_4443_83E9_DFD211E12B40__INCLUDED_)
