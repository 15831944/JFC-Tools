#if !defined(AFX_VERSIONAPP_H__E05877A7_68D0_4AE6_9802_42AA4C1E3EAD__INCLUDED_)
#define AFX_VERSIONAPP_H__E05877A7_68D0_4AE6_9802_42AA4C1E3EAD__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// VersionApp.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CVersionApp dialog

class CVersionApp : public CDialog
{
// Construction
public:
	CVersionApp(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CVersionApp)
	enum { IDD = IDD_ABOUTAPP };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CVersionApp)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CVersionApp)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_VERSIONAPP_H__E05877A7_68D0_4AE6_9802_42AA4C1E3EAD__INCLUDED_)
