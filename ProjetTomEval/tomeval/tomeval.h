// tomeval.h : main header file for the TOMEVAL application
//

#if !defined(AFX_TOMEVAL_H__E843E165_9722_11D1_A42E_004005327F70__INCLUDED_)
#define AFX_TOMEVAL_H__E843E165_9722_11D1_A42E_004005327F70__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CTomevalApp:
// See tomeval.cpp for the implementation of this class
//


class CTomevalApp : public CWinApp
{
public:
	CTomevalApp();

	CString m_NomFicTOMJOB;
	CString m_NomFicRESTOM;
	HWND m_hWndMaitre;



//BEGIN_MESSAGE_MAP(CTomevalApp, CWinApp)
//    ON_MESSAGE(MSGTOM_BONJOUR, OnMyMessage)
//END_MESSAGE_MAP()

	
	
	
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTomevalApp)
	public:
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

// Microsoft Developer Studio will insert additional declarations immediately before the previous line.



#endif // !defined(AFX_TOMEVAL_H__E843E165_9722_11D1_A42E_004005327F70__INCLUDED_)




