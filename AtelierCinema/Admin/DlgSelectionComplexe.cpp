// DlgSelectionComplexe.cpp : implementation file
//

#include "stdafx.h"
#include "admin.h"
#include "DlgSelectionComplexe.h"
#include "ListBox_Complexe.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// L'Application
extern CAdminApp theApp;

/////////////////////////////////////////////////////////////////////////////
// CDlgSelectionComplexe dialog


CDlgSelectionComplexe::CDlgSelectionComplexe(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgSelectionComplexe::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgSelectionComplexe)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CDlgSelectionComplexe::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgSelectionComplexe)
	DDX_Control(pDX, IDC_CADRE1, m_Cadre1);
	DDX_Control(pDX, IDC_LISTCOMPLEXE, m_ListComplexe);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgSelectionComplexe, CDialog)
	//{{AFX_MSG_MAP(CDlgSelectionComplexe)
	ON_BN_CLICKED(IDC_OK, OnOk)
	ON_BN_CLICKED(IDC_ANNULER, OnAnnuler)
	ON_WM_PAINT()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgSelectionComplexe message handlers

void CDlgSelectionComplexe::OnOk() 
{
	// récupération des éléments d'une sélection multiple
	int selcount = m_ListComplexe.GetSelCount(); 
	int * indexComplexeList = new int[selcount];
	
	m_ListComplexe.GetSelItems(selcount,indexComplexeList);

	for(int listcomplexe=0;listcomplexe<selcount;listcomplexe++)
	{
		int indexcmplx;
		CString libelle;
		m_ListComplexe.GetText(indexComplexeList[listcomplexe],1,libelle);
		indexcmplx = theApp.IndexComplexeDuLibelle(libelle);
		indexComplexe.Add(indexcmplx);
	}


	delete indexComplexeList;
	CDialog::OnOK();

}

void CDlgSelectionComplexe::OnAnnuler() 
{
		EndDialog(IDCANCEL);
}

BOOL CDlgSelectionComplexe::OnInitDialog() 
{
	CDialog::OnInitDialog();

	// Initialisation config liste complexe
	//m_ListComplexe.Initialise(UNISTATE,ETENDUE_JFC,ETENDUE_JFC,GetSysColor(COLOR_HIGHLIGHT),0xAA0000,INDEPENDANTE,0,NULL,0);
	m_ListComplexe.Initialise(UNISTATE,ETENDUE_JFC,ETENDUE_JFC,GetSysColor(COLOR_HIGHLIGHT));
	m_ListComplexe.ResetContent();

	// Définition 3 colonnes
	m_ListComplexe.InitNbColonne(3);

	// Définition taille des colonnes
	m_ListComplexe.InitSizeColonne(1,207);
	m_ListComplexe.InitSizeColonne(2,25);
	m_ListComplexe.InitSizeColonne(3,0);

	// Ajout de la liste des complexes
	int size = m_TableComplexe.GetSize(); 
	for(int cmplx=0;cmplx<size;cmplx++)
	{	
	
		CString libelle = m_TableComplexe.GetAt(cmplx).GetLibelle(); 

		//Récupère la régie
		char regie = m_TableComplexe.GetRegie(cmplx); 

		//Récupération du complexe puis de son index et	deduction du bac 
		tagComplexe Cpx = m_TableComplexe.GetAt(cmplx);

		//Récupération de l'index Bac
		WORD indexBac = Cpx.m_indexBAC;	
		CString bac=theApp.base.GetTableBac()->GetLibelle(indexBac);
	
		CString Ligne;

		/*
		// Affichage des tarifs semaine
		CString TxtNoSem;
		CString TxtDate;
		CString Tarif;

		// Positionnement 1er jour année courante
		COleDateTime DateCur = COleDateTime::GetCurrentTime();
		TxtNoSem.Format("%02d",1);
		TxtDate = DateCur.Format("%d-%m-%Y");
		Tarif.Format("%d",1000);
		Ligne.Format("%s\t%s\t\t\t\t%s",TxtNoSem,TxtDate,Tarif);	
		*/

		Ligne.Format("%s\t%c\t%s",libelle,regie,bac);	

		// On boucle sur les libellés ajoutés
		for(int idxlib=0;idxlib<m_LibelleAjoute.GetSize();idxlib++)
		{
			if(libelle==m_LibelleAjoute[idxlib])goto nextcomplexe;
				
		}

		m_ListComplexe.AddString(Ligne);
		nextcomplexe:;	 
			
	}

	UpdateData(0);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgSelectionComplexe::OnPaint() 
{
	CPaintDC dc(this); // device context for painting

	CBrush fond(RGB_BleuJFC);

	CRect rect;
	GetClientRect(&rect);
	dc.FillRect(rect,&fond);
}
