#if !defined(AFX_VIEWPORTWINDOW_H__1ACBACCB_91A8_4C08_A396_F8576F04C3D5__INCLUDED_)
#define AFX_VIEWPORTWINDOW_H__1ACBACCB_91A8_4C08_A396_F8576F04C3D5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ViewportWindow.h : header file
//

#include "PageInfo.h"
#include "./draw/JFCDraw.h"


/////////////////////////////////////////////////////////////////////////////
// CViewportWindow window

class CViewportWindow : public CWnd
{
// Construction
public:
	CViewportWindow(CPageInfo * pageinfo);

// Attributes
protected:
	// l'objet de conversion
	DecimilToUnit * m_pCoord;

	// la référence sur les informations de la page
	CPageInfo * m_PageInfo;
	
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CViewportWindow)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CViewportWindow();

	// Generated message map functions
protected:
	//{{AFX_MSG(CViewportWindow)
	afx_msg void OnPaint();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_VIEWPORTWINDOW_H__1ACBACCB_91A8_4C08_A396_F8576F04C3D5__INCLUDED_)
