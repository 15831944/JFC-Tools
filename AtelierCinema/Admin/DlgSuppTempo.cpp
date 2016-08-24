// DlgSuppTempo.cpp : implementation file
//

#include "stdafx.h"
#include "admin.h"
#include "DlgSuppTempo.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern CAdminApp theApp;

/////////////////////////////////////////////////////////////////////////////
// CDlgSuppTempo dialog


CDlgSuppTempo::CDlgSuppTempo(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgSuppTempo::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgSuppTempo)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CDlgSuppTempo::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgSuppTempo)
	DDX_Control(pDX, IDC_LISTTEMPO, m_ListTempo);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgSuppTempo, CDialog)
	//{{AFX_MSG_MAP(CDlgSuppTempo)
	ON_BN_CLICKED(IDC_OK, OnOk)
	ON_BN_CLICKED(IDC_ANNULER, OnAnnuler)
	ON_WM_PAINT()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgSuppTempo message handlers

BOOL CDlgSuppTempo::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// Ajout des libellés complexes dans la liste
	int size = m_TableTempo.GetSize(); 
	for(int tempo=0;tempo<size;tempo++)
	{	CString libelle = m_TableTempo.GetLibelle(tempo);
		m_ListTempo.AddString(libelle); 		
	}
	
	return TRUE;  
}

void CDlgSuppTempo::OnOk() 
{
	// supression du tempo sélectionné
	int index = m_ListTempo.GetCurSel(); 
	if(index==LB_ERR)
	{
		MessageBox("      La sélection est invalide\nVeuillez sélectionner un tempo");
	}
	else
	{
		CString libelle;
		m_ListTempo.GetText(index,libelle);
		index = theApp.IndexTempoDuLibelle(libelle);
		m_TableTempo.RemoveAt(index,1); 
		m_TableTempo.SetModified(1); 
		CDialog::OnOK();	
	}
	EndDialog(IDOK);
}

void CDlgSuppTempo::OnAnnuler() 
{
	EndDialog(IDCANCEL);
}

void CDlgSuppTempo::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	CBrush fond(RGB_BleuJFC);

	CRect rect;
	GetClientRect(&rect);
	dc.FillRect(rect,&fond);
}
