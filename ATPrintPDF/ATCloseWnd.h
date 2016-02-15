#if !defined(AFX_ATCLOSEWND_H__270E6A9F_DD8A_4362_93EA_5E9B2FB7612A__INCLUDED_)
#define AFX_ATCLOSEWND_H__270E6A9F_DD8A_4362_93EA_5E9B2FB7612A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CATCloseWnd window

class CATCloseWnd : public CWnd
{
public:
	// le constructeur
	CATCloseWnd();

	// la fonction pour créer la fenêtre
	BOOL Create(UINT nID = 0xFFFF);

	// le destructeur
	virtual ~CATCloseWnd();
	
public:
	// la fonction d'enregistrement de la classe
	static BOOL RegisterClass();

// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CATCloseWnd)
	//}}AFX_VIRTUAL

// Generated message map functions
protected:
	//{{AFX_MSG(CATCloseWnd)
	afx_msg void OnDestroy();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////
//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ATCLOSEWND_H__270E6A9F_DD8A_4362_93EA_5E9B2FB7612A__INCLUDED_)
