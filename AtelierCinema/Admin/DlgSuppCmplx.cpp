// DlgSuppCmplx.cpp : implementation file
//

#include "stdafx.h"
#include "admin.h"
#include "DlgSuppCmplx.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern CAdminApp theApp;
/////////////////////////////////////////////////////////////////////////////
// CDlgSuppCmplx dialog


CDlgSuppCmplx::CDlgSuppCmplx(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgSuppCmplx::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgSuppCmplx)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

}


void CDlgSuppCmplx::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgSuppCmplx)
	DDX_Control(pDX, IDC_CADRE1, m_Cadre1);
	DDX_Control(pDX, IDC_LISTCOMPLEXE, m_ListComplexe);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgSuppCmplx, CDialog)
	//{{AFX_MSG_MAP(CDlgSuppCmplx)
	ON_BN_CLICKED(IDC_OK, OnOk)
	ON_BN_CLICKED(IDC_ANNULER, OnAnnuler)
	ON_WM_PAINT()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgSuppCmplx message handlers

void CDlgSuppCmplx::OnOk() 
{
	// supression du complexe sélectionné
	int index = m_ListComplexe.GetCurSel(); 
	if(index==LB_ERR)
	{
		MessageBox("      La sélection est invalide\nVeuillez sélectionner un complexe");
	}
	else
	{
		CString libelle;
		m_ListComplexe.GetText(index,1,libelle);
		index = theApp.IndexComplexeDuLibelle(libelle);

		// suppression du nb entree dans la table des entrées
		CTableNbEntreeComplexe TableEntree = theApp.base.GetNewInstanceTableNbEntree();
		CString Code=theApp.base.GetTableComplexe()->GetCode(index);
		int indexTableEntree = TableEntree.FindIndexFromCode(Code);
		if(indexTableEntree>=0)
		{
			TableEntree.RemoveAt(indexTableEntree);
			TableEntree.SetModified(1);  
		}
		theApp.base.SetTableNbEntree(TableEntree);

		// suppression du complexe dans la table
		m_TableComplexe.RemoveAt(index,1); 
		m_TableComplexe.SetModified(1); 
		// suppression des tarifs associés
		int size = theApp.m_Tarifs.GetSize();
		for(int tarif=0;tarif<size;tarif++)
		{
			theApp.m_Tarifs.GetAt(tarif)->RemoveTarif(Code);
		}
		CDialog::OnOK();	
	}

}

void CDlgSuppCmplx::OnAnnuler() 
{
	EndDialog(IDCANCEL);
}

BOOL CDlgSuppCmplx::OnInitDialog() 
{
	CDialog::OnInitDialog();

	// Init liste des complexes
	// Initialisation Liste Complexe Multi-Colonnes
	m_ListComplexe.Initialise(UNISTATE,ETENDUE_JFC,ETENDUE_JFC,GetSysColor(COLOR_HIGHLIGHT),0xAA0000,INDEPENDANTE,0,NULL,0);
	
	// Définition 3 colonnes
	m_ListComplexe.InitNbColonne(3);

	// Définition taille des colonnes
	m_ListComplexe.InitSizeColonne(1,207);
	m_ListComplexe.InitSizeColonne(2,25);
	m_ListComplexe.InitSizeColonne(3,0);


	// Ajout des libellés complexes dans la liste ainsi que le code Bac et la régie
	int size = m_TableComplexe.GetSize(); 
	for(int cmplx=0;cmplx<size;cmplx++)
	{	CString libelle = m_TableComplexe.GetLibelle(cmplx);
		char regie = m_TableComplexe.GetRegie(cmplx); //Récupère la régie
		
		//Récupération du complexe puis de son index et	deduction du bac 
		tagComplexe Cpx = m_TableComplexe.GetAt(cmplx);
		WORD indexBac = Cpx.m_indexBAC;	//Récupération de l'index Bac
		CString bac=theApp.base.GetTableBac()->GetLibelle(indexBac);
		CString Ligne;
		Ligne.Format("%s\t%c\t%s",libelle,regie,bac);	
		m_ListComplexe.AddString(Ligne); 		
	}
	return TRUE; 
}


void CDlgSuppCmplx::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	CBrush fond(RGB_BleuJFC);

	CRect rect;
	GetClientRect(&rect);
	dc.FillRect(rect,&fond);	
	
}
