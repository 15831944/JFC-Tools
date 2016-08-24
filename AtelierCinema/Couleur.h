#if !defined(AFX_COULEUR_H__83A6C461_790C_11D4_BA64_D6E1BA8BF83D__INCLUDED_)
#define AFX_COULEUR_H__83A6C461_790C_11D4_BA64_D6E1BA8BF83D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Couleur.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CCouleur window

class CCouleur : public CStatic
{
// Construction
public:
	CCouleur();
	int m_Numero;
// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCouleur)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CCouleur();

	// Generated message map functions
protected:
	//{{AFX_MSG(CCouleur)
	afx_msg void OnPaint();
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_COULEUR_H__83A6C461_790C_11D4_BA64_D6E1BA8BF83D__INCLUDED_)
