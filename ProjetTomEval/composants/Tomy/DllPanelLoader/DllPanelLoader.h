// DllPanelLoader.h : main header file for the DLLPANELLOADER DLL
//

#if !defined(AFX_DLLPANELLOADER_H__4103DD6B_1EB7_4258_82DC_1DFDEB2BB38B__INCLUDED_)
#define AFX_DLLPANELLOADER_H__4103DD6B_1EB7_4258_82DC_1DFDEB2BB38B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CDllPanelLoaderApp
// See DllPanelLoader.cpp for the implementation of this class
//

class CDllPanelLoaderApp : public CWinApp
{
public:
	CDllPanelLoaderApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDllPanelLoaderApp)
	public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();
	//}}AFX_VIRTUAL

	//{{AFX_MSG(CDllPanelLoaderApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLLPANELLOADER_H__4103DD6B_1EB7_4258_82DC_1DFDEB2BB38B__INCLUDED_)
