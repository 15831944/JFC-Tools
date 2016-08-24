// Focus Grid.h : main header file for the FOCUS GRID application
//

#if !defined(AFX_FOCUSGRID_H__FEF15AC5_4FF1_11D3_9AE4_0020359647BF__INCLUDED_)
#define AFX_FOCUSGRID_H__FEF15AC5_4FF1_11D3_9AE4_0020359647BF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CFocusGridApp:
// See Focus Grid.cpp for the implementation of this class
//

class CFocusGridApp : public CWinApp
{
public:
	CFocusGridApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CFocusGridApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CFocusGridApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_FOCUSGRID_H__FEF15AC5_4FF1_11D3_9AE4_0020359647BF__INCLUDED_)
