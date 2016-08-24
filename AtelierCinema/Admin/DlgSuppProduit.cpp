// DlgSuppProduit.cpp : implementation file
//

#include "stdafx.h"
#include "admin.h"
#include "DlgSuppProduit.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// L'Application
extern CAdminApp theApp;

// le chemin des données
extern CString GCheminAccesDonnee;

/////////////////////////////////////////////////////////////////////////////
// CDlgSuppProduit dialog


CDlgSuppProduit::CDlgSuppProduit(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgSuppProduit::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgSuppProduit)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CDlgSuppProduit::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgSuppProduit)
	DDX_Control(pDX, IDC_LISTPRODUIT, m_ListProduit);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgSuppProduit, CDialog)
	//{{AFX_MSG_MAP(CDlgSuppProduit)
	ON_LBN_SELCHANGE(IDC_LISTPRODUIT, OnSelchangeListproduit)
	ON_BN_CLICKED(IDC_OK, OnOk)
	ON_BN_CLICKED(IDC_ANNULER, OnAnnuler)
	ON_WM_PAINT()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgSuppProduit message handlers

void CDlgSuppProduit::OnSelchangeListproduit() 
{
//	m_ListProduit.EnableWindow(0); 	
}

void CDlgSuppProduit::OnOk() 
{
	// supression du produit sélectionné
	int index = m_ListProduit.GetCurSel(); 
	if(index==LB_ERR)
	{
		MessageBox("      La sélection est invalide\nVeuillez sélectionner un produit");
	}
	else
	{
		CString libelle;
		m_ListProduit.GetText(index,libelle);
		index = theApp.IndexProduitDuLibelle(libelle);
		// suppression du fichier associé
		CString CheminFichier = GCheminAccesDonnee+"\\Data";
		CheminFichier += "\\Definition";
		CheminFichier += m_TableProduit.GetCode(index);
		CheminFichier += ".table";
		CFile::Remove(CheminFichier); 

		// suppression des tarifs associés
		int size = theApp.m_Tarifs.GetSize();
		for(int tarif=0;tarif<size;tarif++)
		{
			theApp.m_Tarifs.GetAt(tarif)->RemoveTarif(m_TableProduit.GetCode(index));
		}
		m_TableProduit.RemoveAt(index,1); 
		m_TableProduit.SetModified(1); 
		
		CDialog::OnOK();	
	}
	
}

void CDlgSuppProduit::OnAnnuler() 
{
	EndDialog(IDCANCEL);
}

BOOL CDlgSuppProduit::OnInitDialog() 
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

void CDlgSuppProduit::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	CBrush fond(RGB_BleuJFC);

	CRect rect;
	GetClientRect(&rect);
	dc.FillRect(rect,&fond);
}
