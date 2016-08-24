// DlgSuppBac.cpp : implementation file
//

#include "stdafx.h"
#include "admin.h"
#include "DlgSuppBac.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern CAdminApp theApp;

/////////////////////////////////////////////////////////////////////////////
// CDlgSuppBac dialog


CDlgSuppBac::CDlgSuppBac(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgSuppBac::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgSuppBac)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CDlgSuppBac::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgSuppBac)
	DDX_Control(pDX, IDC_LISTBAC, m_ListBac);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgSuppBac, CDialog)
	//{{AFX_MSG_MAP(CDlgSuppBac)
	ON_BN_CLICKED(IDC_OK, OnOk)
	ON_BN_CLICKED(IDC_ANNULER, OnAnnuler)
	ON_WM_PAINT()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgSuppBac message handlers

void CDlgSuppBac::OnOk() 
{
	// supression du complexe sélectionné
	int index = m_ListBac.GetCurSel(); 
	if(index==LB_ERR)
	{
		MessageBox("      La sélection est invalide\nVeuillez sélectionner un bac");
	}
	else
	{
		CString libelle;
		m_ListBac.GetText(index,libelle);
		CString codeBac = theApp.CodeBacDuLibelle(libelle);
		index = theApp.IndexBacDuLibelle(libelle);

		// On supprime le bac si aucun complexe n'est attaché à ce bac
		if(!theApp.ComplexeAttacheAuBac(codeBac))
		{

			m_TableBac.RemoveAt(index,1);
			m_TableBac.SetModified(1); 
			// suppression du tarif associé
			int size = theApp.m_Tarifs.GetSize();
			for(int tarif=0;tarif<size;tarif++)
			{
				theApp.m_Tarifs.GetAt(tarif)->RemoveTarif(codeBac);
			}

			CDialog::OnOK();	
		}
		else
		{
			MessageBox("                      Ce bac n'est pas supprimable\nVeuillez d'abord supprimer les complexes attachés à ce bac");
		}
	}

}

void CDlgSuppBac::OnAnnuler() 
{
	EndDialog(IDCANCEL);	
}

BOOL CDlgSuppBac::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// Ajout des libellés bacs dans la liste
	int size = m_TableBac.GetSize(); 
	for(int bac=0;bac<size;bac++)
	{	
		CString libelle = m_TableBac.GetAt(bac).GetLibelle();
		ASSERT(libelle!="ZZZ");
		m_ListBac.AddString(libelle); 		
	}

	return TRUE;  
}

void CDlgSuppBac::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	CBrush fond(RGB_BleuJFC);

	CRect rect;
	GetClientRect(&rect);
	dc.FillRect(rect,&fond);
}
