// DlgSelectProduit.cpp : implementation file
//

#include "stdafx.h"
#include "admin.h"
#include "DlgSelectProduit.h"
#include "DlgAjoutProduit.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// L'Application
extern CAdminApp theApp;

/////////////////////////////////////////////////////////////////////////////
// CDlgSelectProduit dialog


CDlgSelectProduit::CDlgSelectProduit(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgSelectProduit::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgSelectProduit)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CDlgSelectProduit::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgSelectProduit)
	DDX_Control(pDX, IDC_LISTPRODUIT, m_ListProduit);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgSelectProduit, CDialog)
	//{{AFX_MSG_MAP(CDlgSelectProduit)
	ON_BN_CLICKED(IDC_ANNULER, OnAnnuler)
	ON_BN_CLICKED(IDC_OK, OnOk)
	ON_WM_PAINT()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgSelectProduit message handlers

void CDlgSelectProduit::OnAnnuler() 
{
	EndDialog(IDCANCEL);	
}

BOOL CDlgSelectProduit::OnInitDialog() 
{
	CDialog::OnInitDialog();

	// Ajout des libellés produits dans la liste
	int size = m_TableProduit.GetSize(); 
	for(int p=0;p<size;p++)
	{	
		CString libelle = m_TableProduit.GetLibelle(p);
		m_ListProduit.AddString(libelle); 		
	}
	return TRUE;
}



void CDlgSelectProduit::OnOk() 
{
	CDlgAjoutProduit DlgAjoutProduitFrom;
	// copie de la table produit
	DlgAjoutProduitFrom.m_TableProduit = theApp.base.GetNewInstanceTableProduit();
	
	// copie de la table bac
	DlgAjoutProduitFrom.m_TableBac = theApp.base.GetNewInstanceTableBac();
	
	// copie de la table complexe
	DlgAjoutProduitFrom.m_TableComplexe = theApp.base.GetNewInstanceTableComplexe();

	CString libelle;
	m_ListProduit.GetText(m_ListProduit.GetCurSel(),libelle); 
	int IndexProduit = theApp.IndexProduitDuLibelle(libelle);
	
	DlgAjoutProduitFrom.m_AjoutDepuis=1;
	DlgAjoutProduitFrom.m_indexModif=IndexProduit;

	DlgAjoutProduitFrom.DoModal();

	// récupération de la table produit
	theApp.base.GetTableProduit()->RemoveAll();
	CString code = DlgAjoutProduitFrom.m_TableProduit.GetAt(0).GetCode();
	theApp.base.SetTableProduit(DlgAjoutProduitFrom.m_TableProduit); 
	CDialog::OnOK();
}

void CDlgSelectProduit::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	CBrush fond(RGB_BleuJFC);

	CRect rect;
	GetClientRect(&rect);
	dc.FillRect(rect,&fond);
}
