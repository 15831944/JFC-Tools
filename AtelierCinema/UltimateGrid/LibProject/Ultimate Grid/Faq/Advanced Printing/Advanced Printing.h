// Advanced Printing.h : main header file for the ADVANCED PRINTING application
//

#if !defined(AFX_ADVANCEDPRINTING_H__B74D7335_C5D1_11D2_83D6_0020359647BF__INCLUDED_)
#define AFX_ADVANCEDPRINTING_H__B74D7335_C5D1_11D2_83D6_0020359647BF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CAdvancedPrintingApp:
// See Advanced Printing.cpp for the implementation of this class
//

class CAdvancedPrintingApp : public CWinApp
{
public:
	CAdvancedPrintingApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAdvancedPrintingApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CAdvancedPrintingApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ADVANCEDPRINTING_H__B74D7335_C5D1_11D2_83D6_0020359647BF__INCLUDED_)
