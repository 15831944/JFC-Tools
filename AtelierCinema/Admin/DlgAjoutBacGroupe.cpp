// DlgAjoutBacGroupe.cpp : implementation file
//

#include "stdafx.h"
#include "admin.h"
#include "DlgAjoutBacGroupe.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// L'Application
extern CAdminApp theApp;
extern CString GCheminAccesDonnee;

/////////////////////////////////////////////////////////////////////////////
// CDlgAjoutBacGroupe dialog


CDlgAjoutBacGroupe::CDlgAjoutBacGroupe(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgAjoutBacGroupe::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgAjoutBacGroupe)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CDlgAjoutBacGroupe::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgAjoutBacGroupe)
	DDX_Control(pDX, IDC_CADRE2, m_Cadre2);
	DDX_Control(pDX, IDC_COMBOREGIE, m_ComboRegie);
	DDX_Control(pDX, IDC_LIBBAC, m_NomGrpBacs);
	DDX_Control(pDX, IDC_CADRE1, m_Cadre1);
	DDX_Control(pDX, IDC_LIST_SELBACS, m_ListeSelBacs);
	DDX_Control(pDX, IDC_LISTBACDISPO, m_ListeBacs);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgAjoutBacGroupe, CDialog)
	//{{AFX_MSG_MAP(CDlgAjoutBacGroupe)
	ON_WM_PAINT()
	ON_LBN_DBLCLK(IDC_LISTBACDISPO, OnDblclkListbacdispo)
	ON_LBN_DBLCLK(IDC_LIST_SELBACS, OnDblclkListSelbacs)
	ON_CBN_SELCHANGE(IDC_COMBOREGIE, OnFiltreRegie)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgAjoutBacGroupe message handlers

BOOL CDlgAjoutBacGroupe::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// Init Combo Régie
	m_ComboRegie.ResetContent();
	m_ComboRegie.AddString("T");
	m_ComboRegie.AddString("P");
	m_ComboRegie.AddString("A");
	m_ComboRegie.AddString("M");
	m_ComboRegie.AddString("N");
	m_ComboRegie.SetCurSel(0);

	// Création des bacs régie
	if (!theApp.m_FlagBacRegieCharge)
	{
		if(!theApp.base.CreateBacRegie()) 
			return false;

		theApp.m_FlagBacRegieCharge = TRUE;
	}
	

	// TEST - A VIRER
	/*
	m_ListeBacs.ResetContent();
	m_TableBac = theApp.base.GetNewInstanceTableBac();
	int NbBacs = m_TableBac.GetSize();
	for(int bac=0; bac<NbBacs; bac++)
	{
		CString code = m_TableBac.GetCodeComposite(bac);
		bool Pk = true;
	}
	*/
	
	// Ajout des libellés bacs dans la liste
	/*
	m_ListeBacs.ResetContent();
	int size = m_TableBac.GetSize(); 
	for(int bac=0;bac<size;bac++)
	{	
		CString libelle = m_TableBac.GetAt(bac).GetLibelle();
		m_ListeBacs.AddString(libelle); 		
	}
	*/

	// Affiche selon type de regie
	m_TableBac = theApp.base.GetNewInstanceTableBac();
	int NbBacs = m_TableBac.GetSize();

	SetListeBacs();

	// Chargement de la table des groupes
	m_TableGroupeBac.LoadTable(GCheminAccesDonnee);

	// Mise à jour des contrôles
	UpdateData(0);


	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgAjoutBacGroupe::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	CBrush fond(RGB_BleuJFC);

	CRect rect;
	GetClientRect(&rect);
	dc.FillRect(rect,&fond);
}

/////////////////////////////////// SELECTION D'UN BAC POUR LA COMPOSITION DU GROUPE ////////////////////////////
//
void CDlgAjoutBacGroupe::OnDblclkListbacdispo() 
{
	int index = m_ListeBacs.GetCurSel(); 
	if(index==LB_ERR)
	{
		MessageBox("      La sélection est invalide\nVeuillez sélectionner un bac");
		return;
	}

	// Vérifie que lke bac n'est pas déjà présent
	CString libelle;
	m_ListeBacs.GetText(index,libelle);
	CString codeBac = theApp.CodeBacDuLibelle(libelle);
	index = theApp.IndexBacDuLibelle(libelle);

	// Boucle sur la liste des bacs déjà sélectionné
	bool BacDejaSel = false;
	for (int idx = 0; idx < m_ListeSelBacs.GetCount(); idx++)
	{
		// Libelle du bac
		CString lib;
		m_ListeSelBacs.GetText(idx,lib); 

		/*
		// Recherche code bac 
		CString codeBacSel = theApp.CodeBacDuLibelle(lib);

		if (codeBac == codeBacSel)
		{
			// Bac déjà sélectionné
			MessageBox("Ce bac a déjà été sélectionné !!!");
			BacDejaSel = true; 
			break;
		}
		*/

		if (libelle == lib)
		{
			// Bac déjà sélectionné
			MessageBox("Ce bac a déjà été sélectionné !!!");
			BacDejaSel = true; 
			break;
		}	
	}

	if (BacDejaSel == false)
	{
		// On ajoute ce noueva bac
		m_ListeSelBacs.AddString(libelle);
	}
}


void CDlgAjoutBacGroupe::OnDblclkListSelbacs() 
{
	
	int index = m_ListeSelBacs.GetCurSel(); 
	if(index==LB_ERR)
	{
		MessageBox("La sélection est invalide\nVeuillez sélectionner un bac");
	}
	else
	{
		//Suppression du bac sélectionné
		m_ListeSelBacs.DeleteString(index);
	}
}

/////////////////////////////////// SAUVEGARDE DU GROUPE DE BACS ////////////////////////
void CDlgAjoutBacGroupe::OnOK() 
{
	// Récupère nom du groupe
	UpdateData(1);
	CString LibelleGroupe; 
	m_NomGrpBacs.GetWindowText(LibelleGroupe);
	
	// Vérifier si au moins 1 bac
	if (m_ListeSelBacs.GetCount() <= 1)
	{
		MessageBox("Veuillez sélectionner au moins 2 bacs pour ce groupe de bacs");
		return;
	}

	// Vérifie que le nom a bien été saisie
	if (LibelleGroupe == "")
	{
		MessageBox("Veuillez saisir un nom pour ce groupe de bacs");
		return;
	}

	// Et qu'il n'exsite pas déjà
	if (!GroupeBacExistant(LibelleGroupe))
	{
		// Mise à jour de la table des groupe de bacs
		tagBACGroupe BacGroupe;

		// Infos groupe de bacs
		// 
		// Nom du groupe
		BacGroupe.m_LibelleGroupe	= LibelleGroupe;

		// Recupere tableau des codes bacs et taille
		if (RecupTabCodeBacs(BacGroupe.m_CompoBacs))
		{
			// Mise à jour Nb Bacs
			BacGroupe.m_NbBacs = BacGroupe.m_CompoBacs.GetSize();

			// On a bien récupéré le nombre de bacs et les codes bacs correspondants
			// Ajout dans la table des groupes
			m_TableGroupeBac.Add(BacGroupe); 
			m_TableGroupeBac.SetModified(1); 

			// Sauvegarde 
			m_TableGroupeBac.SaveTable();
		}
		else
		{
			MessageBox("Problème sauvegarde groupe !!!");
			return;
		}

	}
	else
	{
		MessageBox("Ce groupe existe déjà.\nVeuillez donner un autre nom pour ce groupe de bacs");
		return;
	}


	CDialog::OnOK();
}


// Test si groupe déjà existant
bool CDlgAjoutBacGroupe::GroupeBacExistant(CString LibGroupe)
{
	if(!m_TableGroupeBac.CodeExist(LibGroupe))
	{
		// C'est un nouveau nom de groupe
		return false;
	}
	else
		// Groupe déjà existant
		return true;
}

// Récupération de la composition du groue en cours de création
bool CDlgAjoutBacGroupe::RecupTabCodeBacs(CArray<long, long> &CompoGrp)
{
	// Taille de la composition
	int NbElmts = m_ListeSelBacs.GetCount();

	if (NbElmts > 0)
	{
		// Init tableau des codes bacs
		CompoGrp.RemoveAll();

		// Créer le tableau des codes
		for (int idx = 0; idx < m_ListeSelBacs.GetCount(); idx++)
		{
			// Libelle du bac
			CString lib;
			m_ListeSelBacs.GetText(idx,lib); 

			// Recherche code bac 
			//CString codeBacSel = theApp.CodeBacDuLibelle(lib);
			CString codeBacSel = theApp.CodeBacCompositeDuLibelle(lib);
			
			// Vire le préfixe "B"
			codeBacSel = codeBacSel.Mid(1);

			// Code composite
			CString CodeComposite = "";
			CodeComposite = codeBacSel.Right(1);
			
			// Rappel
			/*
			m_ComboRegie.AddString("T");
			m_ComboRegie.AddString("P");
			m_ComboRegie.AddString("A");
			m_ComboRegie.AddString("M");
			m_ComboRegie.AddString("N");
			*/

			if (CodeComposite == "P") 
			{
				codeBacSel = "1" + codeBacSel;  
			}
			else if (CodeComposite == "A")
			{
				codeBacSel = "2" + codeBacSel;
			}
			else if (CodeComposite == "M")
			{
				codeBacSel = "3" + codeBacSel;
			}
			else if (CodeComposite == "N")
			{
				codeBacSel = "4" + codeBacSel;
			}
			else
			{
				codeBacSel = "1" + codeBacSel;
			}
	
			// Convertit en integer
			long Code = atoi(codeBacSel);

			// Ajout du code bac
			CompoGrp.Add(Code);

			/*
			LPTSTR lpszData = new TCHAR[codeBacSel.GetLength()+1];
			_tcscpy(lpszData , codeBacSel);

			CompoGrp.Add(lpszData);
			
			delete[] lpszData;			// don't forget to do this.
			*/

		}
		return true;
	}
	else
		return false;

}	


// Sélection des bacs selon la régie en cours
bool CDlgAjoutBacGroupe::SetListeBacs(const CString regie)
{
	m_ListeBacs.ResetContent();

	// int NbElmt = theApp.m_TableCinema.GetTableBac().GetSize();

	// CTableBAC table = theApp.m_TableCinema.GetTableBac();
	
	// titre
	for(int bac=0;	bac< m_TableBac.GetSize();	bac++)
	{
		CString code = m_TableBac.GetCodeComposite(bac);
		tagBAC b = m_TableBac.GetAt(bac);

		// ajout de tous les libellés bacs
		/*
		if(code.GetLength()!=7)
		{
			#ifdef _DEBUG
				MessageBox("Ceci n'est pas un code composite");
			#endif
		}
		*/

		if(b.m_Quota==0)continue;

		if(regie=="A")
		{
			if(code[6]!='A')continue;
		}
		if(regie=="M")
		{
			if(code[6]!='M')continue;
		}
		if(regie=="N")
		{
			if(code[6]!='N')continue;
		}
		if(regie=="P")
		{
			if(code[6]!='P')continue;
		}

		CString libelle = m_TableBac.GetLibelle(bac);

		m_ListeBacs.AddString(libelle);

	}

	if(m_ListeBacs.GetCount() <= 0)
		return FALSE; 

	return TRUE;
}


// Sélection de la régie pouur affichage des bacs
void CDlgAjoutBacGroupe::OnFiltreRegie() 
{

	// Mise a jour de la liste
	if(m_ComboRegie.GetCurSel()==0)	
		SetListeBacs();

	if(m_ComboRegie.GetCurSel()==1)	
		SetListeBacs("P");

	if(m_ComboRegie.GetCurSel()==2)	
		SetListeBacs("A");

	if(m_ComboRegie.GetCurSel()==3)	
		SetListeBacs("M");

	if(m_ComboRegie.GetCurSel()==4)	
		SetListeBacs("N");

	UpdateData(0);
	
}
