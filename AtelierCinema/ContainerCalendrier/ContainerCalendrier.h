// ContainerCalendrier.h : main header file for the CONTAINERCALENDRIER application
//

#if !defined(AFX_CONTAINERCALENDRIER_H__89AC709F_531C_4613_B3C3_156E2CA5E7C4__INCLUDED_)
#define AFX_CONTAINERCALENDRIER_H__89AC709F_531C_4613_B3C3_156E2CA5E7C4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CContainerCalendrierApp:
// See ContainerCalendrier.cpp for the implementation of this class
//

class CContainerCalendrierApp : public CWinApp
{
public:
	CContainerCalendrierApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CContainerCalendrierApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CContainerCalendrierApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CONTAINERCALENDRIER_H__89AC709F_531C_4613_B3C3_156E2CA5E7C4__INCLUDED_)
