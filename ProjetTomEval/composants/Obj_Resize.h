#if !defined(AFX_OBJ_RESIZE_H__65105A03_A8E3_11D4_BA64_BF97CE9AD111__INCLUDED_)
#define AFX_OBJ_RESIZE_H__65105A03_A8E3_11D4_BA64_BF97CE9AD111__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Obj_Resize.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CObj_Resize window

#include "movemanager.h"
class CObj_Resize : public CStatic
{
// Construction
public:
	CObj_Resize();
	void SetMoveManager(CMoveManager * pMM);

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CObj_Resize)
	//}}AFX_VIRTUAL

// Implementation
public:
	void SetLeftRange(CWnd * pWnd,int NbPixels);
	void SetRightRange(CWnd *pWnd, int NbPixels);
	virtual ~CObj_Resize();

	// Generated message map functions
protected:
	//{{AFX_MSG(CObj_Resize)
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
private:
	CWnd * m_pWndLeft;
	CWnd * m_pWndRight;
	int m_NbPixelsLeft;
	int m_NbPixelsRight;
	bool m_fCapture;
	CRect m_Rect;
	CMoveManager * m_pMM;
	CPoint m_StartPoint;
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_OBJ_RESIZE_H__65105A03_A8E3_11D4_BA64_BF97CE9AD111__INCLUDED_)
