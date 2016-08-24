#if !defined(AFX_LISTBOXSELECT_H__83D164B4_DE95_4504_AF61_6BEA197A189A__INCLUDED_)
#define AFX_LISTBOXSELECT_H__83D164B4_DE95_4504_AF61_6BEA197A189A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ListBoxSelect.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CListBoxSelect window

class CListBoxSelect : public CObj_LBox
{
// Construction
public:
	CListBoxSelect();

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CListBoxSelect)
	//}}AFX_VIRTUAL

// Implementation
public:
	int SetSel( int nIndex, BOOL bSelect = TRUE );
	void SetTextAlign(int halign);
	virtual ~CListBoxSelect();
	int GetSelCount();
	int GetSelItems(int nMaxItems, LPINT rgIndex );

	// Generated message map functions
protected:
	//{{AFX_MSG(CListBoxSelect)
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	//}}AFX_MSG
	void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);


	DECLARE_MESSAGE_MAP()

private:
	// type d'alignement du texte 
	UINT m_AlignType;
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_LISTBOXSELECT_H__83D164B4_DE95_4504_AF61_6BEA197A189A__INCLUDED_)
