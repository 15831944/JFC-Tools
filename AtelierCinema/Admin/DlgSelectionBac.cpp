// DlgSelectionBac.cpp : implementation file
//

#include "stdafx.h"
#include "admin.h"
#include "DlgSelectionBac.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// L'Application
extern CAdminApp theApp;

/////////////////////////////////////////////////////////////////////////////
// CDlgSelectionBac dialog


CDlgSelectionBac::CDlgSelectionBac(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgSelectionBac::IDD, pParent)
{
	m_TypeRegie=0;
	//{{AFX_DATA_INIT(CDlgSelectionBac)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CDlgSelectionBac::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgSelectionBac)
	DDX_Control(pDX, IDC_CADRE1, m_Cadre1);
	DDX_Control(pDX, IDC_TYPECOMBO, m_ComboType);
	DDX_Control(pDX, IDC_LISTBAC, m_ListBac);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgSelectionBac, CDialog)
	//{{AFX_MSG_MAP(CDlgSelectionBac)
	ON_BN_CLICKED(IDC_OK, OnOk)
	ON_BN_CLICKED(IDC_ANNULER, OnAnnuler)
	ON_CBN_SELCHANGE(IDC_TYPECOMBO, OnFiltreRegie)
	ON_WM_PAINT()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgSelectionBac message handlers

void CDlgSelectionBac::OnOk() 
{
	// récupération des éléments d'une sélection multiple
	int selcount = m_ListBac.GetSelCount(); 
	int * indexBacList = new int[selcount];
	m_ListBac.GetSelItems(selcount,indexBacList);
	for(int listbac=0;listbac<selcount;listbac++)
	{
		int idBac;
		CString libelle;
		m_ListBac.GetText(indexBacList[listbac],libelle);
		if(m_TypeRegie!=0)libelle=libelle.Left(libelle.GetLength()-4); 

		idBac = theApp.IndexBacDuLibelle(libelle);
		m_indexBac.Add(idBac);
		
	}
	delete indexBacList;

	CDialog::OnOK();
	
}

void CDlgSelectionBac::OnAnnuler() 
{
		EndDialog(IDCANCEL);
}

BOOL CDlgSelectionBac::OnInitDialog() 
{
	CDialog::OnInitDialog();
	m_ListBac.Initialise(UNISTATE,ETENDUE_JFC,ETENDUE_JFC,GetSysColor(COLOR_HIGHLIGHT),0xAA0000,INDEPENDANTE,0,NULL,0);
	m_ComboType.ResetContent();

	// Définition des types de bacs.
	m_ComboType.AddString("Probabilisé");
	m_ComboType.AddString("Canal +");
	m_ComboType.AddString("Médiavision");
	m_ComboType.AddString("Indépendants");
	m_ComboType.SetCurSel(0);
	SetListe("P");
	UpdateData(0);

	return TRUE;  
}

// Changement du filtre de régie
void CDlgSelectionBac::OnFiltreRegie() 
{
	// Mise a jour de la liste
	if(m_ComboType.GetCurSel()==0)
	{
		m_TypeRegie=0;
		SetListe("P");
	}
	if(m_ComboType.GetCurSel()==1)
	{
		m_TypeRegie=1;
		SetListe("A");
	}
	if(m_ComboType.GetCurSel()==2)
	{
		m_TypeRegie=2;
		SetListe("M");
	}
	if(m_ComboType.GetCurSel()==3)
	{
		m_TypeRegie=3;
		SetListe("N");
	}
	UpdateData(0);
	
}

bool CDlgSelectionBac::SetListe(const CString regie)
{
	int NblibelleAjoute=0;
	m_ListBac.ResetContent();

	for(int bac=0;bac<theApp.base.GetTableBac()->GetSize();bac++)
	{
		CString code = theApp.base.GetTableBac()->GetCode(bac);
		// ajout de tous les libellés bacs
		if(code.GetLength()!=6)
		{
#ifdef _DEBUG
			MessageBox("Ceci n'est pas un code bac");
#endif
		}
		CString libelle = theApp.base.GetTableBac()->GetLibelle(bac);
				
		// On boucle sur les libellés ajoutés
		for(int idxlib=0;idxlib<m_BacAjoute.GetSize();idxlib++)
		{
			if(libelle==m_BacAjoute[idxlib])goto nextbac;
		}
		if(regie=="A")
		{
			libelle+=" (A)";
		}
		if(regie=="M")
		{
			libelle+=" (M)";
		}
		if(regie=="N")
		{
			libelle+=" (N)";
		}

		m_ListBac.AddString(libelle);
		NblibelleAjoute++;
nextbac:;
		}

	if(!NblibelleAjoute)return FALSE; 


	//m_LibelleAjoute.RemoveAll();
	return TRUE;
}
void CDlgSelectionBac::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	CBrush fond(RGB_BleuJFC);
	CRect rect;
	GetClientRect(&rect);
	dc.FillRect(rect,&fond);
}
