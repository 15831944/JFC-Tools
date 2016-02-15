// ATPrintPDF.h : main header file for the ATPRINTPDF application
//

#if !defined(AFX_ATPRINTPDF_H__67397049_C5A0_4353_9BC0_FE901753343A__INCLUDED_)
#define AFX_ATPRINTPDF_H__67397049_C5A0_4353_9BC0_FE901753343A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"       // main symbols
#include "pdflib\\utzadico.h"

/////////////////////////////////////////////////////////////////////////////
// CATPrintPDFApp:
// See ATPrintPDF.cpp for the implementation of this class
//

// ********************** DICO STR ************************

class CATPrintPDFApp : public CWinApp
{
public:
	CATPrintPDFApp();

public:
	void SetPortrait();
	void SetPaysage();

public:
	BOOL m_ModePaysage;
	int m_NumDico;
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CATPrintPDFApp)
	public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();
	//}}AFX_VIRTUAL

// Implementation
	//{{AFX_MSG(CATPrintPDFApp)
		afx_msg void OnAppAbout();
		afx_msg void OnUpdateAppAbout(CCmdUI* pCmdUI);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ATPRINTPDF_H__67397049_C5A0_4353_9BC0_FE901753343A__INCLUDED_)

