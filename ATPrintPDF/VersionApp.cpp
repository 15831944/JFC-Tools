// VersionApp.cpp : implementation file
//

#include "stdafx.h"
#include "atprintpdf.h"
#include "VersionApp.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CVersionApp dialog


CVersionApp::CVersionApp(CWnd* pParent /*=NULL*/)
	: CDialog(CVersionApp::IDD, pParent)
{
	//{{AFX_DATA_INIT(CVersionApp)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CVersionApp::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CVersionApp)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CVersionApp, CDialog)
	//{{AFX_MSG_MAP(CVersionApp)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CVersionApp message handlers
