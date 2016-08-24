// DlgSelTempo.cpp : implementation file
//

#include "stdafx.h"
#include "admin.h"
#include "DlgSelTempo.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgSelTempo dialog


CDlgSelTempo::CDlgSelTempo(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgSelTempo::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgSelTempo)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CDlgSelTempo::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgSelTempo)
	DDX_Control(pDX, IDC_LISTTEMPO, m_ListTempo);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgSelTempo, CDialog)
	//{{AFX_MSG_MAP(CDlgSelTempo)
	ON_BN_CLICKED(IDC_ANNULER, OnAnnuler)
	ON_BN_CLICKED(IDC_OK, OnOk)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgSelTempo message handlers

void CDlgSelTempo::OnAnnuler() 
{
	EndDialog(IDCANCEL);
		
}

void CDlgSelTempo::OnOk() 
{
	// Appel de la boîte de dialogue de modification 
	/*CDlgAjoutTempo	dlgModifTempo;

	// Récupération de la table des tempos
	dlgModifTempo.m_TableTempo = theApp.base.GetInstanceTableTempo(); 
	dlgModifTempo.SetWindowText("Modifier un tempo"); 
	if(dlgModifTempo.DoModal()!=IDOK)return;
	theApp.base.GetTableTempo()->RemoveAll();
	theApp.base.SetTableTempo(dlgModifTempo.m_TableTempo);*/

	EndDialog(IDOK);
	
}
