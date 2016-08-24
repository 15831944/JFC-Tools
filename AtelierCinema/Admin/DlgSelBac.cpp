// DlgSelBac.cpp : implementation file
//

#include "stdafx.h"
#include "admin.h"
#include "DlgSelBac.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgSelBac dialog


CDlgSelBac::CDlgSelBac(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgSelBac::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgSelBac)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CDlgSelBac::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgSelBac)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgSelBac, CDialog)
	//{{AFX_MSG_MAP(CDlgSelBac)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgSelBac message handlers
