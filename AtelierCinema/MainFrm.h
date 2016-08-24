// MainFrm.h : interface of the CMainFrame class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_MAINFRM_H__33A58748_0AED_11D4_989E_004095420987__INCLUDED_)
#define AFX_MAINFRM_H__33A58748_0AED_11D4_989E_004095420987__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CMainFrame : public SECMDIFrameWnd
{
	DECLARE_DYNAMIC(CMainFrame)
public:
	CMainFrame();

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMainFrame)
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CMainFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:  // control bar embedded members
//	CStatusBar  m_wndStatusBar;
	SECStatusBar  m_wndStatusBar;
	SECCustomToolBar m_wndToolBar;
	SECCustomToolBar    m_wndOutils;

// Generated message map functions
protected:
	//{{AFX_MSG(CMainFrame)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnAffichageRpertoiredecibles();
	afx_msg void OnAffichageBarredoutilssouris();
	afx_msg void OnUpdateAffichageBarredoutilssouris(CCmdUI* pCmdUI);
	//afx_msg BOOL OnFileNewproduitfilms();
	//afx_msg void OnFileOpenProduitfilms();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MAINFRM_H__33A58748_0AED_11D4_989E_004095420987__INCLUDED_)
