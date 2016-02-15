#if !defined(AFX_FORMULAIREINFO_H__30E8168C_6479_4CF6_93E0_92E56A004270__INCLUDED_)
#define AFX_FORMULAIREINFO_H__30E8168C_6479_4CF6_93E0_92E56A004270__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// FormulaireInfo.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CFormulaireInfo dialog

class CFormulaireInfo : public CDialog
{
// Construction
public:
	CFormulaireInfo(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CFormulaireInfo)
	enum { IDD = IDD_DIALOG_INFO };
	CObj_Gray m_Cadre2;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CFormulaireInfo)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CFormulaireInfo)
	afx_msg void OnPaint();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_FORMULAIREINFO_H__30E8168C_6479_4CF6_93E0_92E56A004270__INCLUDED_)
