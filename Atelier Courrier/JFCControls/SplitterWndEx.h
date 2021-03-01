/*
//	JFC Informatique et Média
//	2004
//
//	Auteur:
//	Jérémy DEVILERS
//
//
//	Classe:
//	CSplitterWndEx
//
//	Descriptif:
//	Extention de CSplitterWnd, permet de bloquer le rataillage et de remplacer dynamiquement la vue fille
*/

#if !defined(AFX_USEFULSPLITTERWND_H__61D2F7E7_7AAA_11D1_9F4C_008029E98A75__INCLUDED_)
#define AFX_USEFULSPLITTERWND_H__61D2F7E7_7AAA_11D1_9F4C_008029E98A75__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// SplitterWndEx.h : header file
//
/////////////////////////////////////////////////////////////////////////////
// CSplitterWndEx window

class AFX_EXT_CLASS CSplitterWndEx : public CSplitterWnd
{
// Construction
public:
	CSplitterWndEx();

// Attributes
public:

private:
	BOOL m_bBarLocked;

// Operations
public:
	BOOL IsBarLocked(){return m_bBarLocked;}
	void LockBar(BOOL bState=TRUE){m_bBarLocked=bState;}
	BOOL ReplaceView(int row, int col,CRuntimeClass * pViewClass,SIZE size);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSplitterWndEx)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CSplitterWndEx();

	// Generated message map functions
protected:
	//{{AFX_MSG(CSplitterWndEx)
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_USEFULSPLITTERWND_H__61D2F7E7_7AAA_11D1_9F4C_008029E98A75__INCLUDED_)
