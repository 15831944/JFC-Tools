// DlgAjoutComplexeGroupe.cpp : implementation file
//

#include "stdafx.h"
#include "admin.h"
#include "DlgAjoutComplexeGroupe.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// L'Application
extern CAdminApp theApp;
extern CString GCheminAccesDonnee;

/////////////////////////////////////////////////////////////////////////////
// CDlgAjoutComplexeGroupe dialog


CDlgAjoutComplexeGroupe::CDlgAjoutComplexeGroupe(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgAjoutComplexeGroupe::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgAjoutComplexeGroupe)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CDlgAjoutComplexeGroupe::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgAjoutComplexeGroupe)
	DDX_Control(pDX, IDC_LIBCOMPLEXE, m_NomGroupeComplexe);
	DDX_Control(pDX, IDC_LIST_SELCOMPS, m_ListeSelComplexes);
	DDX_Control(pDX, IDC_LISTCOMPDISPO, m_ListeComplexes);
	DDX_Control(pDX, IDC_CADRE1, m_Cadre1);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgAjoutComplexeGroupe, CDialog)
	//{{AFX_MSG_MAP(CDlgAjoutComplexeGroupe)
	ON_WM_PAINT()
	ON_WM_CANCELMODE()
	ON_LBN_DBLCLK(IDC_LISTCOMPDISPO, OnDblclkListcompdispo)
	ON_LBN_DBLCLK(IDC_LIST_SELCOMPS, OnDblclkListSelcomps)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgAjoutComplexeGroupe message handlers

void CDlgAjoutComplexeGroupe::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	CBrush fond(RGB_BleuJFC);

	CRect rect;
	GetClientRect(&rect);
	dc.FillRect(rect,&fond);
}

BOOL CDlgAjoutComplexeGroupe::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	m_ListeComplexes.Initialise(UNISTATE,ETENDUE_JFC,ETENDUE_JFC,GetSysColor(COLOR_HIGHLIGHT),0xAA0000,INDEPENDANTE,0,NULL,0);
	
	// Définition 3 colonnes
	m_ListeComplexes.InitNbColonne(3);

	// Définition taille des colonnes
	m_ListeComplexes.InitSizeColonne(1,207);
	m_ListeComplexes.InitSizeColonne(2,25);
	m_ListeComplexes.InitSizeColonne(3,0);
	// Ajout des libellés complexes dans la liste ainsi que le code Bac et la régie
	int size = m_TableComplexe.GetSize(); 

	for(int cmplx=0;cmplx<size;cmplx++)
	{	
		// Libellé du complexe
		CString libelle = m_TableComplexe.GetLibelle(cmplx);

		// Récupère la régie
		char regie = m_TableComplexe.GetRegie(cmplx);
		
		//Récupération du complexe puis de son index et	deduction du bac 
		tagComplexe Cpx = m_TableComplexe.GetAt(cmplx);
		WORD indexBac = Cpx.m_indexBAC;	//Récupération de l'index Bac
		CString bac=theApp.base.GetTableBac()->GetLibelle(indexBac);

		CString Ligne;
		Ligne.Format("%s\t%c\t%s",libelle,regie,bac);	

		// Ajout à la liste
		m_ListeComplexes.AddString(Ligne); 		
	}
	
	// Chargement de la table des groupes
	m_TableGroupeComplexe.LoadTable(GCheminAccesDonnee);

		// Mise à jour des contrôles
	UpdateData(0);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgAjoutComplexeGroupe::OnCancelMode() 
{
	CDialog::OnCancelMode();
	
	// TODO: Add your message handler code here
	
}

// Sélection d'un complexe à ajouter dans le groupe
void CDlgAjoutComplexeGroupe::OnDblclkListcompdispo() 
{
	int index = m_ListeComplexes.GetCurSel(); 
	if(index==LB_ERR)
	{
		MessageBox("      La sélection est invalide\nVeuillez sélectionner un complexe");
		return;
	}

	// Vérifie que le complexe n'est pas déjà présent
	CString libelle;
	m_ListeComplexes.GetText(index,1, libelle);

	// Index dans la table des complexes
	index = theApp.IndexComplexeDuLibelle(libelle);
	CString codeComplexe = theApp.base.GetTableComplexe()->GetCode(index);


	// Boucle sur la liste des bacs déjà sélectionné
	bool ComplexeDejaSel = false;
	for (int idx = 0; idx < m_ListeSelComplexes.GetCount(); idx++)
	{
		// Libelle du complexe
		CString lib;
		m_ListeSelComplexes.GetText(idx,lib); 

		// Index dans la table des complexes
		index = theApp.IndexComplexeDuLibelle(lib);

		// Recherche code complexe 
		CString codeComplexeSel = theApp.base.GetTableComplexe()->GetCode(index);

		if (codeComplexe == codeComplexeSel)
		{
			// Bac déjà sélectionné
			MessageBox("Ce complexe a déjà été sélectionné !!!");
			ComplexeDejaSel = true; 
			break;
		}
	}

	if (ComplexeDejaSel == false)
	{
		// On ajoute ce nouveau complexe
		m_ListeSelComplexes.AddString(libelle);
	}
	
}

// Suppression d'un complexe sur le groupe en cours de construction
void CDlgAjoutComplexeGroupe::OnDblclkListSelcomps() 
{
	
	int index = m_ListeSelComplexes.GetCurSel(); 
	if(index==LB_ERR)
	{
		MessageBox("La sélection est invalide\nVeuillez sélectionner un complexe");
	}
	else
	{
		//Suppression du complexe sélectionné
		m_ListeSelComplexes.DeleteString(index);
	}
}

// Test si groupe déjà existant
bool CDlgAjoutComplexeGroupe::GroupeComplexeExistant(CString LibGroupe)
{
	if(!m_TableGroupeComplexe.CodeExist(LibGroupe))
	{
		// C'est un nouveau nom de groupe
		return false;
	}
	else
		// Groupe déjà existant
		return true;
}

// Recup infos composition groupe
bool CDlgAjoutComplexeGroupe::RecupTabCodeComplexes(CArray<int,int> &CompoGrp)
{
	// Taille de la composition
	int NbElmts = m_ListeSelComplexes.GetCount();

	if (NbElmts > 0)
	{
		// Init tableau des codes complexes
		CompoGrp.RemoveAll();

		// Créer le tableau des codes
		for (int idx = 0; idx < m_ListeSelComplexes.GetCount(); idx++)
		{
			// Libelle du complexe
			CString lib;
			m_ListeSelComplexes.GetText(idx,lib); 

			// Recherche code complexe 
			CString codeComplexeSel = theApp.base.FindCodeFromLibelle(lib);
			codeComplexeSel = codeComplexeSel.Mid(1);

			// Convertit en integer
			int Code = atoi(codeComplexeSel);

			// Ajout du code complexe
			CompoGrp.Add(Code);
		}
		return true;
	}
	else
		return false;

}


// Sauvegarde du groupe de complexe en cours de construction
void CDlgAjoutComplexeGroupe::OnOK() 
{

	// Récupère nom du groupe
	UpdateData(1);
	CString LibelleGroupe; 
	m_NomGroupeComplexe.GetWindowText(LibelleGroupe);
	
	// Vérifier si au moins 1 bac
	if (m_ListeSelComplexes.GetCount() <= 1)
	{
		MessageBox("Veuillez sélectionner au moins 2 complexes pour ce groupe de complexes");
		return;
	}

	// Vérifie que le nom a bien été saisie
	if (LibelleGroupe == "")
	{
		MessageBox("Veuillez saisir un nom pour ce groupe de complexes");
		return;
	}

	// Et qu'il n'exsite pas déjà
	if (!GroupeComplexeExistant(LibelleGroupe))
	{
		// Mise à jour de la table des groupe de complexes
		tagComplexeGroupe ComplexeGroupe;

		// Infos groupe de Complexes
		// 
		// Nom du groupe
		ComplexeGroupe.m_LibelleGroupe	= LibelleGroupe;

		// Recupere tableau des codes complexes et taille
		if (RecupTabCodeComplexes(ComplexeGroupe.m_CompoComplexes))
		{
			// Mise à jour Nb Complexes
			ComplexeGroupe.m_NbComplexes = ComplexeGroupe.m_CompoComplexes.GetSize();

			// On a bien récupéré le nombre de complexes et les codes complexes correspondants
			// Ajout dans la table des groupes
			m_TableGroupeComplexe.Add(ComplexeGroupe); 
			m_TableGroupeComplexe.SetModified(1); 

			// Sauvegarde 
			m_TableGroupeComplexe.SaveTable();
		}
		else
		{
			MessageBox("Problème sauvegarde groupe !!!");
			return;
		}

	}
	else
	{
		MessageBox("Ce groupe existe déjà.\nVeuillez donner un autre nom pour ce groupe de complexes");
		return;
	}

	CDialog::OnOK();
}
