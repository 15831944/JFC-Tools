// DlgModifProduit.cpp : implementation file
//

#include "stdafx.h"
#include "admin.h"
#include "DlgModifProduit.h"
#include "DlgAjoutProduit.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgModifProduit dialog

// L'Application
extern CAdminApp theApp;


CDlgModifProduit::CDlgModifProduit(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgModifProduit::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgModifProduit)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CDlgModifProduit::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgModifProduit)
	DDX_Control(pDX, IDC_LISTPRODUIT, m_ListProduit);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgModifProduit, CDialog)
	//{{AFX_MSG_MAP(CDlgModifProduit)
	ON_BN_CLICKED(IDC_OK, OnOk)
	ON_BN_CLICKED(IDC_ANNULER, OnAnnuler)
	ON_LBN_SELCHANGE(IDC_LISTPRODUIT, OnSelchangeListproduit)
	ON_WM_PAINT()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgModifProduit message handlers

void CDlgModifProduit::OnOk() 
{
	CDlgAjoutProduit dlgAjout;

	// copie de la table produit
	dlgAjout.m_TableProduit = theApp.base.GetNewInstanceTableProduit();

	// copie de la table bac
	dlgAjout.m_TableBac = theApp.base.GetNewInstanceTableBac();

	// copie de la table complexe
	dlgAjout.m_TableComplexe = theApp.base.GetNewInstanceTableComplexe();

	CString libelle;
	m_ListProduit.GetText(m_ListProduit.GetCurSel(),libelle); 
	int IndexProduit = theApp.IndexProduitDuLibelle(libelle);
	dlgAjout.m_modif=1;
	dlgAjout.m_indexModif=IndexProduit;
	dlgAjout.DoModal();

	// récupération de la table produit
	theApp.base.GetTableProduit()->RemoveAll();
	CString code = dlgAjout.m_TableProduit.GetAt(0).GetCode();  
	theApp.base.SetTableProduit(dlgAjout.m_TableProduit); 

	CDialog::OnOK();

}

void CDlgModifProduit::OnAnnuler() 
{
	EndDialog(IDCANCEL);	
}

BOOL CDlgModifProduit::OnInitDialog() 
{
	CDialog::OnInitDialog();

	// Ajout des libellés produits dans la liste
	int size = m_TableProduit.GetSize(); 
	for(int p=0;p<size;p++)
	{	
		CString libelle = m_TableProduit.GetLibelle(p);
		m_ListProduit.AddString(libelle); 		
	}

	// Mise à jour des contrôles
	UpdateData(0);
	return TRUE;  
}

void CDlgModifProduit::OnSelchangeListproduit() 
{
//	m_ListProduit.EnableWindow(0); 	
}

void CDlgModifProduit::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	CBrush fond(RGB_BleuJFC);

	CRect rect;
	GetClientRect(&rect);
	dc.FillRect(rect,&fond);
}

