// Tomeval.h : main header file for the TOMEVAL application
//

#if !defined(AFX_TOMEVAL_H__9B11E005_ECED_476F_8CBD_7CC2BEA7839A__INCLUDED_)
#define AFX_TOMEVAL_H__9B11E005_ECED_476F_8CBD_7CC2BEA7839A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CTomevalApp:
// See Tomeval.cpp for the implementation of this class
//

//##ModelId=3D9AB7D40258
class CTomevalApp : public CWinApp
{
public:
	//##ModelId=3D9AB7D40262
	CTomevalApp();

	//##ModelId=3D9AB7D40263
	CString m_NomFicTOMJOB;
	//##ModelId=3D9AB7D4026C
	CString m_NomFicRESTOM;
	//##ModelId=3D9AB7D4026D
	HWND m_hWndMaitre;

	// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTomevalApp)
	public:
	//##ModelId=3D9AB7D40276
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CTomevalApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TOMEVAL_H__9B11E005_ECED_476F_8CBD_7CC2BEA7839A__INCLUDED_)
