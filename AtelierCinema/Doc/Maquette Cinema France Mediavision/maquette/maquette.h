// maquette.h : main header file for the MAQUETTE application
//

#if !defined(AFX_MAQUETTE_H__1821467B_A77C_430A_8DA1_188011E5FAC7__INCLUDED_)
#define AFX_MAQUETTE_H__1821467B_A77C_430A_8DA1_188011E5FAC7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CMaquetteApp:
// See maquette.cpp for the implementation of this class
//

class CMaquetteApp : public CWinApp
{
public:
	CMaquetteApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMaquetteApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CMaquetteApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MAQUETTE_H__1821467B_A77C_430A_8DA1_188011E5FAC7__INCLUDED_)
