#if !defined(AFX_OBJ_TITRE_H__58AD6DE1_9DA2_11D3_989D_D45D8708BEEA__INCLUDED_)
#define AFX_OBJ_TITRE_H__58AD6DE1_9DA2_11D3_989D_D45D8708BEEA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Obj_Titre.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CObj_Titre window

class CObj_Titre : public CStatic
{
// Construction
public:
	CObj_Titre();

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CObj_Titre)
	//}}AFX_VIRTUAL

// Implementation
public:
	void SetText(LPCSTR txt);
	virtual ~CObj_Titre();

	// Generated message map functions
protected:
	//{{AFX_MSG(CObj_Titre)
	afx_msg void OnPaint();
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
	CString m_txt;
	COLORREF m_BkColor;
	CFont m_Font;
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_OBJ_TITRE_H__58AD6DE1_9DA2_11D3_989D_D45D8708BEEA__INCLUDED_)
