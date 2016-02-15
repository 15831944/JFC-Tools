// NomenclatureView.cpp : implementation of the CNomenclatureView class
//

#include "stdafx.h"
#include "MainFrm.h"
#include "Nomenclature.h"

#include "NomenclatureDoc.h"
#include "NomenclatureView.h"

#include "TreeCtrlX.h"
#include "Formulaire.h"
#include "FormulaireNom.h"
#include "FormulaireStandard.h"
#include "FormulaireGroupe.h"
#include "FormulaireImpression.h"
#include "FormulaireTauxFO.h"
#include "FormulaireUtilTaux.h"
#include "DialogNvlleAnnee.h"
#include "DialogTiersEDI.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


extern CNomenclatureApp theApp;

/////////////////////////////////////////////////////////////////////////////
// CNomenclatureView

IMPLEMENT_DYNCREATE(CNomenclatureView, CFormView)

BEGIN_MESSAGE_MAP(CNomenclatureView, CFormView)
	//{{AFX_MSG_MAP(CNomenclatureView)
	ON_NOTIFY(NM_RCLICK, IDC_TREE1, OnRclickTree1)
	ON_NOTIFY(TVN_GETDISPINFO, IDC_TREE1, OnGetdispinfoTree1)
	ON_NOTIFY(TVN_SELCHANGED, IDC_TREE1, OnSelchangedTree1)
	ON_COMMAND(ID_MENUNOMENCL_AJOUTERNVLLEBRANCHE, OnMenunomenclAjouternvllebranche)
	ON_COMMAND(ID_MENUNOMENCL_COLLERBRANCHE, OnMenunomenclCollerbranche)
	ON_COMMAND(ID_MENUNOMENCL_COUPERBRANCHE, OnMenunomenclCouperbranche)
	ON_COMMAND(ID_MENUNOMENCL_INFOPRODUIT, OnMenunomenclInfoproduit)
	ON_COMMAND(ID_MENUNOMENCL_RENOMMERBRANCHE, OnMenunomenclRenommerbranche)
	ON_COMMAND(ID_MENUNOMENCL_SUPPRIMERBRANCHE, OnMenunomenclSupprimerbranche)
	ON_WM_PAINT()
	ON_WM_CREATE()
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, OnFilePrintPreview)
	ON_COMMAND(ID_FILE_PRINTBIS, OnFilePrintbis)
	ON_WM_SIZE()
	ON_NOTIFY(NM_CLICK, IDC_TREE1, OnClickTree1)
	ON_COMMAND(ID_MENUNOMENCL_AJOUTERANNONCEURSANSGROUPE, OnMenunomenclAjouterannonceursansgroupe)
	ON_COMMAND(ID_MENUNOMENCL_AJOUTERANNONCEURSANSGROUPESANSPRODUIT, OnMenunomenclAjouterannonceursansgroupesansproduit)
	ON_COMMAND(ID_MENUNOMENCL_TAUX, OnMenunomenclTaux)
	ON_COMMAND(ID_UTILTAUX, OnUtiltaux)
	ON_UPDATE_COMMAND_UI(ID_UTILTAUX, OnUpdateUtiltaux)
	ON_COMMAND(ID_CENTRALESDACHAT, OnCentralesdachat)
	ON_COMMAND(ID_REGIES, OnRegies)
	//}}AFX_MSG_MAP
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, CFormView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, CFormView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, CFormView::OnFilePrintPreview)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CNomenclatureView construction/destruction

CNomenclatureView::CNomenclatureView()
	: CFormView(CNomenclatureView::IDD)
{
	//{{AFX_DATA_INIT(CNomenclatureView)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_BufHItemCopie =0;
	m_Marque = 0;
	m_Produit =0;
	m_HItemCur = 0;
	
}

CNomenclatureView::~CNomenclatureView()
{
}

void CNomenclatureView::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CNomenclatureView)
	DDX_Control(pDX, IDC_STATIC_TRAIT, m_Trait);
	DDX_Control(pDX, IDC_STATIC1, m_TitreGestNomencl);
	DDX_Control(pDX, IDC_CADRE1, m_Cadre1);
	DDX_Control(pDX, IDC_TREE1, m_TreeNomenclature);
	//}}AFX_DATA_MAP
}

BOOL CNomenclatureView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs
	return CFormView::PreCreateWindow(cs);
}

void CNomenclatureView::OnInitialUpdate()
{
    char buffer[128];
	CPWFile File;
	CString FicMarque;
	int Annee;

	CFormView::OnInitialUpdate();
	GetParentFrame()->RecalcLayout();
	ResizeParentToFit();

	// Resize fenetre application
	m_MM.AddWindow(&m_Cadre1,MM_SIZE_X | MM_SIZE_Y);

	CRect rC,rW;
	GetClientRect(rC);
	GetWindowRect(rW);
	m_MM.SetMasterRect(rW,rC);

	m_Cadre1.AddWindow(&m_TitreGestNomencl,MM_SIZE_X);
	m_Cadre1.AddWindow(&m_Trait,MM_SIZE_X);
	m_Cadre1.AddWindow(&m_TreeNomenclature,MM_SIZE_X|MM_SIZE_Y);
	m_Cadre1.ValideChild(rW);

	// Chargement des images item asociées à l'arbre
	m_TreeNomenclature.LoadBitmapNormal(IDB_BITMAP_ITEM,15);

	m_TitreNomenclature = "Nomenclature";

	// De quel année cette nomenclature
    _strdate(buffer);
	m_DateCur = buffer;

	//Chargement Texte Initiale du Menu
	m_TMenu.SetSize(11);
	m_TMenu[Popup_Ajouter] = "Ajouter";    
	m_TMenu[Popup_AjouterAnnSsGrp] = "Ajouter annonceur sans groupe";
    m_TMenu[Popup_AjouterAnnSsGrpSsProd] = "Ajouter annonceur sans produit"; 
	m_TMenu[Popup_Renommer] = "Modifier";
	m_TMenu[Popup_Coller] = "Coller";
	m_TMenu[Popup_Couper] = "Couper";
	m_TMenu[Popup_Supprimer] = "Supprimer";
	m_TMenu[Popup_Info] = "Info";
	m_TMenu[Popup_TauxFO] = "Taux fin d'ordre";

	// Date courante
	if (atoi(m_DateCur.Right(2)) < 80) 
		theApp.m_AnneeNomencl= "20"+ m_DateCur.Right(2);	
	else
		theApp.m_AnneeNomencl= "19"+ m_DateCur.Right(2);
	
	// Attention si nouvelle année et rép non créé
	FicMarque = theApp.m_AnneeNomencl + "\\marque.arb";
	if (File.OpenFile(theApp.m_PathNomencl + FicMarque) == false)
	{
		Annee = atoi(theApp.m_AnneeNomencl) - 1; 
		theApp.m_AnneeNomencl.Format("%d",Annee);
	}
	else
		File.CloseFile();				

	// Chargement de l'arbre Produit/Marque/Secteur via Fichiers .Arb
	if (ChargeFicProduitMarqueSecteur(theApp.m_AnneeNomencl) == false)
	{	

		theApp.m_AnneeNomencl ="";
		m_TitreGestNomencl.SetWindowText("Gestion de la nomenclature ");
		//AfxMessageBox ("Problème chargement arbre Marques/Annonceurs/Secteurs");
		return;
	}

	// Chargement des années disponibles
	if (ChargeTabAnneeNomencl(true) == false)
		AfxMessageBox ("Problème chargement des années nomenclatures");
	else
		CreerMenuPopupAnnee();

	

	//Init Texte branche ou elmt
	m_TypeElmt.SetSize(2);
	m_TypeElmt[0] = "branche"; 
	m_TypeElmt[1] = "elmt";

	/*
	//Chargement Texte Initiale du Menu
	m_TMenu.SetSize(11);
	m_TMenu[Popup_Ajouter] = "Ajouter";    
	m_TMenu[Popup_AjouterAnnSsGrp] = "Ajouter annonceur sans groupe";
    m_TMenu[Popup_AjouterAnnSsGrpSsProd] = "Ajouter annonceur sans produit"; 
	m_TMenu[Popup_Renommer] = "Modifier";
	m_TMenu[Popup_Coller] = "Coller";
	m_TMenu[Popup_Couper] = "Couper";
	m_TMenu[Popup_Supprimer] = "Supprimer";
	m_TMenu[Popup_Info] = "Info";
	m_TMenu[Popup_TauxFO] = "Taux fin d'ordre";
	*/

	// Définition des fonts
	CClientDC dc (this);
	int nHeight = -(dc.GetDeviceCaps(LOGPIXELSY)*8/72);

	m_FontTime8Bold.CreateFont(nHeight,0,0,0,FW_BOLD,0,0,0,DEFAULT_CHARSET,
		                       OUT_CHARACTER_PRECIS,CLIP_CHARACTER_PRECIS,
							   DEFAULT_QUALITY,DEFAULT_PITCH|FF_DONTCARE,"MS Sans Serif");
	
	// Change en Bold la police Titre Nomenclature
	m_TitreGestNomencl.SetFont(&m_FontTime8Bold, TRUE );

	// Initialisation Utilisateur Taux FO
	theApp.m_UtilTauxCur = "";
	m_MPUtilTauxCur = "";

}

/////////////////////////////////////////////////////////////////////////////
// CNomenclatureView printing

BOOL CNomenclatureView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// Creation du tableau texte ligne associé à la nomenclature (pour module impression)
	// CreerTabTxtNomencl();
	// Si ici 

	return DoPreparePrinting(pInfo);
}

void CNomenclatureView::OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo)
{
	m_Print=new CPrinter;
	m_Print->m_pView=this;
	m_Print->OnBeginPrinting(pDC,pInfo);	
}

void CNomenclatureView::OnEndPrinting(CDC* pDC, CPrintInfo* pInfo)
{
	m_Print->OnEndPrinting(pDC,pInfo);
	delete m_Print;
	m_Print=NULL;
	m_TreeNomenclature.RedrawWindow();	
}

void CNomenclatureView::OnPrint(CDC* pDC, CPrintInfo* pInfo)
{

	m_Print->OnPrint(pDC,pInfo);
}

// Chargement des années nomenclatures existantes
bool CNomenclatureView::ChargeTabAnneeNomencl(bool ViaFichierAnnee)
{
	bool OkAnnee = false;
	CString AnneeCur,AnneeNomencl,Ligne;
	char buffer[5];
	int Annee,AnneeMax,NbAnnee;
	CFileFind finder;
	CPWFile File;
  
	if (ViaFichierAnnee == false)
	{
		// Chargement des années en balayant les répertoires années

		// Annee YYYY courante
		if (atoi(m_DateCur.Right(2)) < 80) 
			AnneeCur= "20"+ m_DateCur.Right(2);	
		else
			AnneeCur= "19"+ m_DateCur.Right(2);	

		// balayage de tous les répertoires année du ss-répertoire /N_Client/
		AnneeMax = atoi(AnneeCur);
		m_TabAnneeNomencl.SetSize(0,AnneeMax-1980+1);

		for (Annee = 1980;Annee <= AnneeMax;Annee++) 
		{				
			_itoa(Annee, buffer,10);
			AnneeNomencl = buffer;
			if (finder.FindFile(theApp.m_PathNomencl + AnneeNomencl))
			{
				m_TabAnneeNomencl.Add(AnneeNomencl);
			}
		}
				
		// retaille Tab Année Disponibles
		m_TabAnneeNomencl.FreeExtra();
		if (m_TabAnneeNomencl.GetSize() > 0) OkAnnee = true; 
	}

	else
	{
		// Idem mais en lisant le fichier Annee.txt
		if (File.OpenFile(theApp.m_PathNomencl + "\\Annee.txt"))
		{
			// lecture nb années disponibles
			File.ReadString(&Ligne);
			NbAnnee = atoi(Ligne.Mid(1));

			// remplissage tableau des années
			if (NbAnnee > 0)
			{
				m_TabAnneeNomencl.SetSize(NbAnnee);
				for (Annee = 0;Annee < NbAnnee;Annee++) 
				{
					File.ReadString(&Ligne);
					m_TabAnneeNomencl.SetAt(Annee,Ligne);
				}

				OkAnnee = true;
			}

			File.CloseFile();
		}
	}	

	TriTableauStringValue(m_TabAnneeNomencl,1);

	return OkAnnee;
}

// Chargement des années posssibles à créer
// (en enlevant les années déjà existantes dans fichier Année.txt)
bool CNomenclatureView::ChargeTabAnneePossible(CStringArray &TabAnneePossible)
{
	CString TxtAnnee;
	bool DatePresente = false;
	int AnneeCur;
	const int AvantApres = 5;

	TabAnneePossible.RemoveAll();

	// Année courante
	TxtAnnee = m_DateCur.Mid(6,2);
	AnneeCur = atoi(TxtAnnee);
	if (AnneeCur > 80)
		AnneeCur = 1900 + AnneeCur;
	else
		AnneeCur = 2000 + AnneeCur;

	// Remplissage Tab Année en enlevant les années déjà existantes
	TabAnneePossible.SetSize(0,2*AvantApres+1);
	for (int i = AnneeCur+AvantApres;i>= AnneeCur-AvantApres;i--)
	{
		TxtAnnee.Format("%d",i);
		DatePresente = false;
		if (m_TabAnneeNomencl.GetSize() >= 0)
		{
			// vérifie date non présente
			for (int j = 0;j<m_TabAnneeNomencl.GetSize();j++)
			{
				if (TxtAnnee == m_TabAnneeNomencl[j])
				{
					DatePresente = true;
					break;
				}	
			}
			if (DatePresente == false)	TabAnneePossible.Add(TxtAnnee);
		}	
	}			
	TabAnneePossible.FreeExtra();

	return (TabAnneePossible.GetSize() > 0);

}

// Chargement des années existante (voir fichier Année.txt)
void CNomenclatureView::ChargeTabAnneeExistante(CStringArray &TabAnneePossible)
{
	int NbAnnee;

	TabAnneePossible.RemoveAll();
	NbAnnee = m_TabAnneeNomencl.GetSize();
	for (int i = 0;i < NbAnnee;i++)
	{
		TabAnneePossible.Add(m_TabAnneeNomencl[i]);
	}	

}

// Ajout d'une année dans fichier des années
void CNomenclatureView::AjouteAnneeNomenclature(CString AnneeNomenclature)
{
	bool DatePresente = false;
	CString Ligne;
	int NbAnneeValid;
	CPWFile File;
	int i;

	// vérifie date non présente
	NbAnneeValid = m_TabAnneeNomencl.GetSize();
	for (i = 0;i<NbAnneeValid;i++)
	{
		if (AnneeNomenclature == m_TabAnneeNomencl[i])
		{
			DatePresente = true;
			break;
		}	
	}

	if (DatePresente == false)
	{
		// Ok nouvelle date on la rajoute au fichier Année.txt et au tableau
		m_TabAnneeNomencl.Add(AnneeNomenclature);
		NbAnneeValid = m_TabAnneeNomencl.GetSize();


		// On détruit tout d'abord l'ancien fichier Année
		TRY
		{
			CFile::Remove(theApp.m_PathNomencl + "\\Annee.txt");
		}
		CATCH(CFileException, e)
		{
			// on s'en fout
		}
		END_CATCH

		// Puis on reconstruit le fichier à partir du tableau année
		if (File.Open(theApp.m_PathNomencl + "\\Annee.txt",CFile::modeCreate | CFile::modeWrite))
		{
			// Nombre d'années existantes				
			Ligne.Format("#%d",NbAnneeValid);
			File.WriteString(Ligne + "\n");

			// Puis les années
			for (i=0;i<NbAnneeValid;i++) File.WriteString(m_TabAnneeNomencl[i] + "\n");

			File.CloseFile();
		}			
	}		
}

// Creation du menu Ouvrir / Menu Popup Annee
void CNomenclatureView::CreerMenuPopupAnnee()
{
	CMenu MenuAnnee;
	
	// Mise à jour du menu Popup Annee
	CMenu* pMenuPrincipal = theApp.m_pMainWnd->GetMenu();
	CMenu* pMenuNomencl   = pMenuPrincipal->GetSubMenu(0);

	MenuAnnee.CreateMenu();

	for (int i= 0;i<m_TabAnneeNomencl.GetSize();i++)
		MenuAnnee.AppendMenu(MF_STRING | MF_ENABLED, ID_MENU_DYN_FIRST + i,m_TabAnneeNomencl.GetAt(i));


	pMenuNomencl->AppendMenu(MF_POPUP | MF_ENABLED, (UINT)MenuAnnee.m_hMenu, "Ouvrir");

	MenuAnnee.Detach();
}

// Chargement des marques existantes
bool CNomenclatureView::ChargeTabMarque(CStringArray &TabMarque)
{
	int NbMarque;
	CNoeud *pNoeudMarque;

	TabMarque.RemoveAll();

	// la branche marque est la premiere de l'arbre
	pNoeudMarque = m_ArbreEnMemoire.m_pNoeud[0];
	NbMarque = pNoeudMarque->m_pNoeud.GetSize();
	if (NbMarque > 0)
	{
		for (int i = 0;i<NbMarque;i++)
		{
			TabMarque.Add(pNoeudMarque->m_pNoeud[i]->m_Alias);
		}
		return true;
	}

	else
		return false;

}

// Chargement des familles existantes
bool CNomenclatureView::ChargeTabFamille(CStringArray &TabFamille)
{
	int NbFamille;
	CNoeud *pNoeudFamille;

	TabFamille.RemoveAll();

	// la branche famille est la 3eme de l'arbre
	pNoeudFamille = m_ArbreEnMemoire.m_pNoeud[2];
	NbFamille = pNoeudFamille->m_pNoeud.GetSize();
	if (NbFamille > 0)
	{
		for (int i = 0;i<NbFamille;i++)
		{
			TabFamille.Add(pNoeudFamille->m_pNoeud[i]->m_Alias);
		}
		return true;
	}

	else
		return false;
}

// Chargement des secteurs existants pour une famille
bool CNomenclatureView::ChargeTabSecteur(CStringArray &TabSecteur,CString Famille)
{

	int NbFamille,NbSecteur;
	CNoeud *pNoeudFamille;

	TabSecteur.RemoveAll();

	// la branche famille est la 3eme de l'arbre
	pNoeudFamille = m_ArbreEnMemoire.m_pNoeud[2];
	NbFamille = pNoeudFamille->m_pNoeud.GetSize();
	if (NbFamille > 0)
	{
		for (int i = 0;i<NbFamille;i++)
		{
			if (pNoeudFamille->m_pNoeud[i]->m_Alias == Famille)
			{
				// Famille correspondant aus secteurs désirés	
				NbSecteur = pNoeudFamille->m_pNoeud[i]->m_pNoeud.GetSize();
				if (NbSecteur > 0)
				{
					for (int j = 0;j<NbSecteur;j++)
					{
						TabSecteur.Add(pNoeudFamille->m_pNoeud[i]->m_pNoeud[j]->m_Alias);
					}
				}
			}
		}
		return true;
	}

	else
		return false;
}

// Création et chargement d'un nouvel arbre nomenclature
bool CNomenclatureView::CreerEtChargerNouvelArbreNomencl(CString AnneeNomenclature) 
{

	CString FicMarque,FicProduit,FicSecteur,Ligne;
	CString TitreBranche1,TitreBranche2,TitreBranche3;
	int NiveauRacine = 0;
	HTREEITEM HTreeItemMarque = 0;
	CPWFile File;
	bool ChargeOk=true;

	theApp.m_AnneeNomencl    = AnneeNomenclature;
	theApp.m_PathNomenclAnnee = theApp.m_PathNomencl + AnneeNomenclature + "\\";

	// Creation des 2 noeuds secondaires
	m_ArbreEnMemoire.m_Libelle = "0";
	m_ArbreEnMemoire.m_Alias = "0";
	m_ArbreEnMemoire.m_Codif = "0";
	m_ArbreEnMemoire.m_pNoeud.SetSize(3);
	for (int i = 0;i< m_ArbreEnMemoire.m_pNoeud.GetSize();i++)
		m_ArbreEnMemoire.m_pNoeud[i] = new CNoeud;

	m_ArbreEnMemoire.m_Niveau = 0;
	m_ArbreEnMemoire.m_pRacine = NULL;
	
	// Ajout en mémoire de la branche Marque
	FicMarque = theApp.m_PathNomenclAnnee + "marque.arb"; 
	FicProduit = theApp.m_PathNomenclAnnee + "produit.arb"; 
	//FicSecteur = theApp.m_PathNomenclAnnee + "secteur.arb"; 
	FicSecteur = theApp.m_PathNomencl + "secteur.arb"; 

	Ligne = "MARQUE";
	m_ArbreEnMemoire.m_pNoeud[0]->m_Libelle = Ligne;	// Titre MARQUE
	m_ArbreEnMemoire.m_pNoeud[0]->m_PathFichier = theApp.m_PathNomenclAnnee + Ligne;
	m_ArbreEnMemoire.m_pNoeud[0]->m_Niveau = 0;
	NiveauRacine = 0;
	m_ArbreEnMemoire.m_pNoeud[0]->m_pNoeud.SetSize(0);
	TitreBranche1 = m_ArbreEnMemoire.m_pNoeud[0]->m_Libelle;

	Ligne = "ANNONCEUR";
	m_ArbreEnMemoire.m_pNoeud[1]->m_Libelle = Ligne;	// Titre ANNONCEUR/PRODUIT
	m_ArbreEnMemoire.m_pNoeud[1]->m_PathFichier = theApp.m_PathNomenclAnnee + Ligne;
	m_ArbreEnMemoire.m_pNoeud[1]->m_Niveau = 0;
	m_ArbreEnMemoire.m_pNoeud[1]->m_pNoeud.SetSize(0);
	TitreBranche2 = m_ArbreEnMemoire.m_pNoeud[1]->m_Libelle;

	Ligne = "SECTEUR";
	m_ArbreEnMemoire.m_pNoeud[2]->m_Libelle = Ligne;    // Titre FAMILLE / SECTEUR
	m_ArbreEnMemoire.m_pNoeud[2]->m_PathFichier = theApp.m_PathNomenclAnnee + Ligne;
	m_ArbreEnMemoire.m_pNoeud[2]->m_Niveau = 0;
	m_ArbreEnMemoire.m_pNoeud[2]->m_pNoeud.SetSize(0);
	TitreBranche3 = m_ArbreEnMemoire.m_pNoeud[2]->m_Libelle;

	// Construction du controle arbre 
	m_TreeNomenclature.PresenteArbre(m_ArbreEnMemoire.m_pNoeud[1],m_ArbreEnMemoire.m_pNoeud[0],m_Marque,TitreBranche1,TitreBranche2,TitreBranche3,HTreeItemMarque);
	m_TreeNomenclature.PresenteArbre(m_ArbreEnMemoire.m_pNoeud[1],m_ArbreEnMemoire.m_pNoeud[1],m_Produit,TitreBranche1,TitreBranche2,TitreBranche3,HTreeItemMarque);
	m_TreeNomenclature.PresenteArbre(m_ArbreEnMemoire.m_pNoeud[1],m_ArbreEnMemoire.m_pNoeud[2],m_Secteur,TitreBranche1,TitreBranche2,TitreBranche3,HTreeItemMarque);

	// Tri des 3 branches arbres
	m_TreeNomenclature.TrierArbre();

	// On synchronise les fichiers pour ordre alphabétique
	// Ajout en mémoire de la branche Marque
	remove(FicMarque);
	if( File.Open(FicMarque,CFile::modeCreate | CFile::modeWrite))
	{
		// fichier marque.arb inexistant
		Ligne = "// Fichier des marques\n";
		File.WriteString(Ligne);
		Ligne = "MARQUE\n";
		File.WriteString(Ligne);
		Ligne = "#0\n";
		File.WriteString(Ligne);
		File.Close();
	}
	else ChargeOk = false;

	remove(FicProduit);
	if( File.Open(FicProduit,CFile::modeCreate | CFile::modeWrite))
	{
		// fichier produit.arb inexistant
		Ligne = "// Fichier des groupes/annonceurs/produits\n";
		File.WriteString(Ligne);
		Ligne = "#ANNONCEUR\n";
		File.WriteString(Ligne);
		Ligne = "#0\n";
		File.WriteString(Ligne);
		File.Close();
	}
	else ChargeOk = false;

	remove(FicSecteur);
	if( File.Open(FicSecteur,CFile::modeCreate | CFile::modeWrite))
	{
		// fichier produit.arb inexistant
		Ligne = "// Fichier des familles/secteurs\n";
		File.WriteString(Ligne);
		Ligne = "#SECTEUR\n";
		File.WriteString(Ligne);
		Ligne = "#0\n";
		File.WriteString(Ligne);
		File.Close();
	}
	else ChargeOk = false;

	// Puis création du ss-répertoire Annonceur
	TitreBranche2.MakeUpper();  	
	if (CreateDirectory(theApp.m_PathNomenclAnnee +"\\" + TitreBranche2,NULL) == false)
		ChargeOk = false;

	return ChargeOk;

}

// Création d'un nouvel arbre nomenclature
bool CNomenclatureView::CreerNouvelArbreNomencl(CString AnneeNomenclature) 
{
	CString FicMarque,FicProduit,FicSecteur,Ligne;
	CString TitreBranche2;
	CPWFile File;
	bool CreationOk = true;

	// Si répertoire année inexistant >> on le crée
	if (CreateDirectory(theApp.m_PathNomencl + "\\" + AnneeNomenclature ,NULL) == false)
	{
		CreationOk = false;
	}

	else
	{
		// Ajout fichier marque vide
		FicMarque = theApp.m_PathNomencl + AnneeNomenclature + "\\marque.arb";
		remove(FicMarque);
		if( File.Open(FicMarque,CFile::modeCreate | CFile::modeWrite))
		{
			// fichier marque.arb inexistant
			Ligne = "// Fichier des marques\n";
			File.WriteString(Ligne);
			Ligne = "MARQUE\n";
			File.WriteString(Ligne);
			Ligne = "#0\n";
			File.WriteString(Ligne);
			File.Close();
		}
		else CreationOk = false;

		FicProduit = theApp.m_PathNomencl + AnneeNomenclature + "\\produit.arb";
		remove(FicProduit);
		if( File.Open(FicProduit,CFile::modeCreate | CFile::modeWrite))
		{
			// fichier produit.arb inexistant
			Ligne = "// Fichier des groupes/annonceurs/produits\n";
			File.WriteString(Ligne);
			Ligne = "#ANNONCEUR\n";
			File.WriteString(Ligne);
			Ligne = "#0\n";
			File.WriteString(Ligne);
			File.Close();
		}
		else CreationOk = false;

		//FicSecteur = theApp.m_PathNomencl + AnneeNomenclature + "\\secteur.arb";
		FicSecteur = theApp.m_PathNomencl + "\\secteur.arb";

		if (File.Open(FicSecteur,CFile::modeRead))
		{
			File.Close();
		}
		else
		{
			remove(FicSecteur);
			if( File.Open(FicSecteur,CFile::modeCreate | CFile::modeWrite))
			{
				// fichier produit.arb inexistant
				Ligne = "// Fichier des familles/secteurs\n";
				File.WriteString(Ligne);
				Ligne = "#SECTEUR\n";
				File.WriteString(Ligne);
				Ligne = "#0\n";
				File.WriteString(Ligne);
				File.Close();
			}
			else CreationOk = false;
		}

		// Puis création du ss-répertoire Annonceur
		TitreBranche2 = m_ArbreEnMemoire.m_pNoeud[1]->m_Libelle;
		if (CreateDirectory(theApp.m_PathNomencl + AnneeNomenclature +"\\ANNONCEUR",NULL) == false)
			CreationOk = false;

	}

	return CreationOk;
}

// Test si nomenclature de l'année est vide (sans référence)
bool CNomenclatureView::NomenclatureVide(CString AnneeNomenclature)
{
	bool NomenclVide = true;
	CString Ligne;
	CString FicMarque,FicProduit,FicSecteur;
	CPWFile File;

	FicMarque = theApp.m_PathNomencl + AnneeNomenclature + "\\marque.arb";
	if (File.OpenFile(FicMarque))
	{
		// Test Nb marques en 3eme position
		File.ReadString(&Ligne);
		File.ReadString(&Ligne);
		File.ReadString(&Ligne);
		if (Ligne != "#0") NomenclVide = false;
		File.CloseFile();	
	}

	FicProduit = theApp.m_PathNomencl + AnneeNomenclature + "\\produit.arb";
	if (NomenclVide == true && 
		File.OpenFile(FicProduit))
	{
		// Test Nb produits en 3eme position
		File.ReadString(&Ligne);
		File.ReadString(&Ligne);
		File.ReadString(&Ligne);
		if (Ligne != "#0") NomenclVide = false;
		File.CloseFile();	
	}
	return NomenclVide;

}

// Duplication d'une nomenclature année
bool CNomenclatureView::DupliqueAnneeNomenclature(CString AnneeSrc,CString AnneeDst)
{

	CPWFile File;

	//if(AfxMessageBox("Duplication nomenclatureLe serveur semble utilisé.\nVoulez-vous attendre 30 secondes que le serveur se libère ?",MB_YESNO|MB_ICONQUESTION)!=IDYES)
	if (NomenclatureVide(AnneeSrc)== true)
	{
		// Ici Nomenclature Année Origine vide, inutile de la copier
		AfxMessageBox("Inutile de dupliquer la nomenclature " + AnneeSrc + "\nElle est vide");
		return false;
	}
	else if (NomenclatureVide(AnneeDst) == false)
	{
		// Ici la nomenclature année destination n'est pas vide, on pose la question si on veut la remplacer
		if(AfxMessageBox("La nomenclature de l'année destination " + AnneeDst + " n'est pas vide\n" +
			             "Voulez-vous quand même la remplacer ?",MB_YESNO|MB_ICONQUESTION) == IDNO)
			return false;		
	}
	
	// Tout d'abord on copie tous les fichiers *.arb (produit.arb,marque.arb,secteur.arb)
	CopyFile(theApp.m_PathNomencl + AnneeSrc + "\\marque.arb",theApp.m_PathNomencl + AnneeDst + "\\marque.arb",false);
	CopyFile(theApp.m_PathNomencl + AnneeSrc + "\\produit.arb",theApp.m_PathNomencl + AnneeDst + "\\produit.arb",false);

	// Puis on supprime répertoire Annonceur pour remplacer par le nouveau
	RemoveDirectory(theApp.m_PathNomencl + AnneeDst + "\\ANNONCEUR");
	CopyDirectory(theApp.m_PathNomencl + AnneeSrc ,theApp.m_PathNomencl + AnneeDst);

	return true;

}

// Chargement de la nomenclature année sélectionnée
bool CNomenclatureView::ChargeFicProduitMarqueSecteur(CString AnneeNomenclature) 
{
	CString FicMarque,FicProduit,FicSecteur,Ligne;
	CString TitreBranche1,TitreBranche2,TitreBranche3;
	CPWFile File;
	bool ChargeOk=true;
	int NiveauRacine = 0;
	HTREEITEM HTreeItemMarque = 0;

	theApp.m_AnneeNomencl    = AnneeNomenclature;
	theApp.m_PathNomenclAnnee = theApp.m_PathNomencl + AnneeNomenclature + "\\";

	// Titre Nomenclature + Année
	CString tt="Gestion de la nomenclature ";
	tt+=theApp.m_AnneeNomencl;
	m_TitreGestNomencl.SetWindowText(tt);

	// Creation des 2 noeuds secondaires
	m_ArbreEnMemoire.m_Libelle = "0";
	m_ArbreEnMemoire.m_Alias = "0";
	m_ArbreEnMemoire.m_Codif = "0";
	m_ArbreEnMemoire.m_pNoeud.SetSize(3);
	for (int i = 0;i< m_ArbreEnMemoire.m_pNoeud.GetSize();i++)
		m_ArbreEnMemoire.m_pNoeud[i] = new CNoeud;

	m_ArbreEnMemoire.m_Niveau = 0;
	m_ArbreEnMemoire.m_pRacine = NULL;
	
	// Ajout en mémoire de la branche Marque
	FicMarque = theApp.m_PathNomenclAnnee + "marque.arb"; 

	if (File.OpenFile(FicMarque))
	{
		File.ReadString(&Ligne);							// Ligne commentaire
		File.ReadString(&Ligne);							// Ligne Entete Racine
		m_ArbreEnMemoire.m_pNoeud[0]->m_Libelle = Ligne;	// Titre MARQUE
		m_ArbreEnMemoire.m_pNoeud[0]->m_PathFichier = theApp.m_PathNomenclAnnee + Ligne;
		m_ArbreEnMemoire.m_pNoeud[0]->m_Niveau = 0;
		TitreBranche1 = Ligne;

		File.ReadString(&Ligne);
		m_ArbreEnMemoire.AjoutArborescenceMarque(File,m_ArbreEnMemoire.m_pNoeud[0],Ligne,NiveauRacine);
		File.CloseFile();	

		// Ajout en mémoire de la branche Produit
		FicProduit = theApp.m_PathNomenclAnnee  + "produit.arb";

		// Teste si bon format EDI // sinon on reformatte le fichier
		if (NomenclatureFormatEDI(FicProduit) == false)
		{
			AfxMessageBox ("Problème chargement fichier " + FicProduit,MB_ICONEXCLAMATION);
			return false;
		}

		if (File.OpenFile(FicProduit))
		{
			File.ReadString(&Ligne);								// Ligne commentaire
			File.ReadString(&Ligne);								// Ligne Entete Racine

			m_ArbreEnMemoire.m_pNoeud[1]->m_Libelle = Ligne.Mid(1);	// Titre Branche Produits
			m_ArbreEnMemoire.m_pNoeud[1]->m_PathFichier = theApp.m_PathNomenclAnnee + m_ArbreEnMemoire.m_pNoeud[1]->m_Libelle;
			m_ArbreEnMemoire.m_pNoeud[1]->m_Niveau = 0;
			TitreBranche2 = m_ArbreEnMemoire.m_pNoeud[1]->m_Libelle;

			File.ReadString(&Ligne);
			NiveauRacine = 0;
			m_ArbreEnMemoire.AjoutArborescenceProduit(File,m_ArbreEnMemoire.m_pNoeud[1],Ligne,NiveauRacine,m_ArbreEnMemoire.m_pNoeud[0]);
			File.CloseFile();	

			// Ajout en mémoire de la branche secteur
			//FicSecteur = theApp.m_PathNomenclAnnee  + "secteur.arb";
			FicSecteur = theApp.m_PathNomencl  + "secteur.arb";

			if (File.OpenFile(FicSecteur))

			{
				File.ReadString(&Ligne);								// Ligne commentaire
				File.ReadString(&Ligne);								// Ligne Entete Racine

				m_ArbreEnMemoire.m_pNoeud[2]->m_Libelle = Ligne.Mid(1);	// Titre Branche Secteurs
				m_ArbreEnMemoire.m_pNoeud[2]->m_PathFichier = theApp.m_PathNomenclAnnee + m_ArbreEnMemoire.m_pNoeud[2]->m_Libelle;
				m_ArbreEnMemoire.m_pNoeud[2]->m_Niveau = 0;
				TitreBranche3 = m_ArbreEnMemoire.m_pNoeud[2]->m_Libelle;			

				File.ReadString(&Ligne);
				NiveauRacine = 0;
				m_ArbreEnMemoire.AjoutArborescenceSecteur(File,m_ArbreEnMemoire.m_pNoeud[2],Ligne,NiveauRacine);
				File.CloseFile();	

				// Construction du controle arbre 
				m_TreeNomenclature.PresenteArbre(m_ArbreEnMemoire.m_pNoeud[1],m_ArbreEnMemoire.m_pNoeud[0],m_Marque,TitreBranche1,TitreBranche2,TitreBranche3,HTreeItemMarque);
				m_TreeNomenclature.PresenteArbre(m_ArbreEnMemoire.m_pNoeud[1],m_ArbreEnMemoire.m_pNoeud[1],m_Produit,TitreBranche1,TitreBranche2,TitreBranche3,HTreeItemMarque);
				m_TreeNomenclature.PresenteArbre(m_ArbreEnMemoire.m_pNoeud[1],m_ArbreEnMemoire.m_pNoeud[2],m_Secteur,TitreBranche1,TitreBranche2,TitreBranche3,HTreeItemMarque);

				// Tri des 3 branches arbres
				m_TreeNomenclature.TrierArbre();

				// On synchronise les fichiers pour ordre alphabétique
				SynchroArbre_FicArb();
				ChargeOk = true;
			}

		}

		else ChargeOk = false;		

	}

	else ChargeOk = false;
	

	if (ChargeOk == false)

	{
		if (MessageBox( "Problème chargement arbre Marques/Annonceurs/Secteurs \nDésirez-vous créer une nouvelle nomenclature de base", "Confirmer", MB_OKCANCEL|MB_ICONQUESTION ) == IDOK)
		{
			// Pb chargement , on le signale et on charge une nomenclature minimal
			ChargeOk = false;
		}		
	}
		
	return ChargeOk;

}

// Libération noeuds et arbre de la nomenclature en cours......
bool CNomenclatureView::DegageNomenclatureCourante()
{
	// Destruction des noeuds items
	m_ArbreEnMemoire.FusillerSsNoeud();

	// Destruction de l'arbre nomenclature
	m_TreeNomenclature.DeleteAllItems();
	return true;
}

bool CNomenclatureView::ChargeFicMarque(CNoeud *m_ArbreMarque) 
{
	CString FicMarque,Ligne;
	CPWFile File;
	int NiveauRacine = 0;

	// Creation noeud marque
	m_ArbreMarque->m_Niveau = NiveauRacine;
	m_ArbreMarque->m_pRacine = NULL;
	
	// Ajout en mémoire de la branche Marque
	FicMarque = theApp.m_PathNomencl + "marque.arb"; 
	
	if (File.OpenFile(FicMarque))
	{
		File.ReadString(&Ligne);							// Ligne commentaire
		File.ReadString(&Ligne);							// Ligne Entete Racine
		m_ArbreMarque->m_Libelle = Ligne;					// Titre MARQUE
		m_ArbreMarque->m_PathFichier = theApp.m_PathNomencl + Ligne;

		File.ReadString(&Ligne);
		m_ArbreMarque->AjoutArborescenceMarque(File,m_ArbreMarque,Ligne,NiveauRacine);
		File.CloseFile();	

		return true;
	}
	else
		return false;

}

bool CNomenclatureView::ChargeFicProduit(CNoeud *m_ArbreProduit,CNoeud *m_ArbreMarque) 
{
	CString FicProduit,Ligne;
	CPWFile File;
	int NiveauRacine = 0;

	// Creation des 2 noeuds secondaires
	m_ArbreProduit->m_Niveau = NiveauRacine;
	m_ArbreProduit->m_pRacine = NULL;

	// Ajout en mémoire de la branche Produit
	FicProduit = theApp.m_PathNomencl + "produit.arb";

	if (File.OpenFile(FicProduit))
	{
		File.ReadString(&Ligne);								// Ligne commentaire
		File.ReadString(&Ligne);								// Ligne Entete Racine

		m_ArbreProduit->m_Libelle = Ligne.Mid(1);				// Titre Branche Produits
		m_ArbreProduit->m_PathFichier = theApp.m_PathNomencl + m_ArbreProduit->m_Libelle;

		File.ReadString(&Ligne);
		NiveauRacine = 0;
		m_ArbreProduit->AjoutArborescenceProduit(File,m_ArbreProduit,Ligne,NiveauRacine,m_ArbreMarque);
		File.CloseFile();		

		return true;
	}

	else
		return false;

}

bool CNomenclatureView::ChargeFicSecteur(CNoeud *m_ArbreSecteur) 
{
	CString FicSecteur,Ligne;
	CPWFile File;
	int NiveauRacine = 0;

	// Creation des 2 noeuds secondaires
	m_ArbreSecteur->m_Niveau = NiveauRacine;
	m_ArbreSecteur->m_pRacine = NULL;

	// Ajout en mémoire de la branche Secteur (commun à toutes les années)
	//FicSecteur = theApp.m_PathNomenclAnnee + "secteur.arb";
	FicSecteur = theApp.m_PathNomencl + "secteur.arb";


	if (File.OpenFile(FicSecteur))
	{
		File.ReadString(&Ligne);								// Ligne commentaire
		File.ReadString(&Ligne);								// Ligne Entete Racine

		m_ArbreSecteur->m_Libelle = Ligne.Mid(1);				// Titre Branche Secteur
		m_ArbreSecteur->m_PathFichier = theApp.m_PathNomencl + m_ArbreSecteur->m_Libelle;

		File.ReadString(&Ligne);
		NiveauRacine = 0;
		m_ArbreSecteur->AjoutArborescenceSecteur(File,m_ArbreSecteur,Ligne,NiveauRacine);
		File.CloseFile();		

		return true;
	}

	else
		return false;

}

// Test si possibilité de copier l'élmt sur la branche parent en question
bool CNomenclatureView::CopieItemPossible(HTREEITEM &HItemCopie,HTREEITEM &HItemParent,bool AvecMessage)
{

	int CodeDessinCopie = 0;
	int CodeDessinEnfantParent = 0;
	int CodeDessinParent = 0;
	CNoeud *pNoeud;

	// recup code dessin enfant de la branche à suivre
	if (m_TreeNomenclature.GetItemData(m_HItemCur) != 0)
	{
		// recup code dessin enfant de la branche parent
		pNoeud = (CNoeud*)m_TreeNomenclature.GetItemData(HItemParent);		
		CodeDessinEnfantParent = pNoeud->m_CodeImage % 100;
        CodeDessinParent = pNoeud->m_CodeImage/100;
	}

	if (CodeDessinEnfantParent != 0)
	{
		m_TreeNomenclature.GetItemImage(HItemCopie,CodeDessinCopie,CodeDessinCopie);

		if (CodeDessinCopie == CodeDessinEnfantParent || 
			//(CodeDessinParent == CTreeCtrlX::ImageGroupe && CodeDessinCopie == CTreeCtrlX::ImageCampagne) ||
			(CodeDessinParent == CTreeCtrlX::ImageAnnonceur && CodeDessinCopie == CTreeCtrlX::ImageCampagne))
			
		{
			if (CodeDessinCopie != CTreeCtrlX::ImageCampagne)
			{
				// copie autre qu'une campagne (donc répertoire)
				if (CopieRepItemPossible(HItemCopie,HItemParent) == true)
					return true;
				else
				{
					if (AvecMessage==true) AfxMessageBox("Impossible de copier, certains répertoires portent le même nom");	
					return false;
				}
			}	
			else
			{
				// copie fichier campagne
				if (CopieFicItemPossible(HItemCopie,HItemParent,"") == true)
					return true;
				else
				{
					if (AvecMessage==true) AfxMessageBox("Impossible de copier fichier campagne dèjà existant sous le même nom");	
					return false;
				}					
			}


		}		

		else if (CodeDessinParent == CTreeCtrlX::ImageGroupe && CodeDessinCopie == CTreeCtrlX::ImageCampagne)
		{
			// pas de copie campagne au niveau groupe
			return false;
		}
		else
		{	// On ne peut rien copier sur une élmt primaire	non parent arbrement parlant
			// sauf pour les campagnes
			if (AvecMessage==true) AfxMessageBox("Impossible de copier sur cet élément");	
			return false;
		}
	}
	else
	{	// On ne peut rien copier sur une élmt primaire	
		if (AvecMessage==true) AfxMessageBox("Impossible de copier sur cet élément");	
		return false;
	}
	
}

// Recupère uniquement le handle item branche ou elmt arborescence
//void CNomenclatureView::OnMenunomenclCopierbranche() 
//{
//	if (m_HItemCur != 0)	m_BufHItemCopie = m_HItemCur;
//	m_InxMenuCur = 2;
//}

void CNomenclatureView::OnMenunomenclCollerbranche() 
{
	bool Ok;
	HTREEITEM HBranchMarque;
	HTREEITEM HBranchFamille;
	HTREEITEM HItemExParent;
	CNoeud *pNoeudParent;
	CNoeud *pNoeudEnfant;
	CNoeud *pNoeudExParent;
	CNoeud Noeud;
	int NbNoeud;
	int CodeDessin;
	int CodeDessinParent;
	int CodeDessinExParent;
	CString FicACopier,NvFic;
	CString TxtCampagne;

	if (m_BufHItemCopie != m_HItemCur)
	{
		if (m_BufHItemCopie != 0 && m_HItemCur !=0)
		{
			// Test si copier/coller possible
			if (CopieItemPossible(m_BufHItemCopie,m_HItemCur,true) == true)
			{

				// Ici élmt à copier n'est pas une campagne, donc répertoire
				HTREEITEM HNvItem = m_TreeNomenclature.NewCopierBranche(m_BufHItemCopie,m_HItemCur);

				// rétablit nouveau lien en mémoire, récup noeud parent en mémoire
				pNoeudParent = (CNoeud*)m_TreeNomenclature.GetItemData(m_HItemCur);
				m_TreeNomenclature.GetItemImage(m_HItemCur,CodeDessinParent,CodeDessinParent);				
				pNoeudEnfant = (CNoeud*)m_TreeNomenclature.GetItemData(m_BufHItemCopie);

				if (pNoeudParent != NULL && pNoeudEnfant != NULL)
				{
					pNoeudParent->m_pNoeud.Add(pNoeudEnfant);
				}

				// Test si campagne associé à un produit
				// Dans ce cas il faut mettre à jour la campagne associé au produit au niveau de l'arbo marque
				HItemExParent= m_TreeNomenclature.GetParentItem(m_BufHItemCopie);
				pNoeudExParent = (CNoeud*)m_TreeNomenclature.GetItemData(HItemExParent);
				m_TreeNomenclature.GetItemImage(m_BufHItemCopie,CodeDessin,CodeDessin);
				m_TreeNomenclature.GetItemImage(HItemExParent,CodeDessinExParent,CodeDessinExParent);

				if (CodeDessin == CTreeCtrlX::ImageCampagne &&
				   (CodeDessinParent == CTreeCtrlX::ImageProduit || CodeDessinExParent == CTreeCtrlX::ImageProduit))
				{
					// ici déplacement d'une campagne (au moins 1 produit impliqué)
					// attention aux implications sur les différentes branches marque et secteur
					HBranchMarque = m_TreeNomenclature.GetRootItem();
					HBranchMarque = m_TreeNomenclature.GetNextItem(HBranchMarque,TVGN_NEXT);
					HBranchFamille = m_TreeNomenclature.GetNextItem(HBranchMarque,TVGN_NEXT);

					TxtCampagne = m_TreeNomenclature.GetItemText(m_BufHItemCopie);

					// Déplacement au niveau de l'arbo marque
					m_TreeNomenclature.DeplacerCampagneProduitDansMarque(HBranchMarque,pNoeudExParent->m_Marque,pNoeudExParent->m_Alias,pNoeudParent->m_Marque,pNoeudParent->m_Alias,TxtCampagne);

					// Déplacement au niveau de l'arbo famille secteur
					m_TreeNomenclature.DeplacerCampagneProduitDansSecteur(HBranchFamille,pNoeudExParent->m_Famille,pNoeudExParent->m_Secteur,pNoeudExParent->m_Alias,pNoeudParent->m_Famille,pNoeudParent->m_Secteur,pNoeudParent->m_Alias,TxtCampagne);
				}


				// En +++
				if (CodeDessin == CTreeCtrlX::ImageCampagne &&
				   (CodeDessinParent == CTreeCtrlX::ImageAnnonceur || CodeDessinExParent == CTreeCtrlX::ImageAnnonceur))
				{
					HBranchFamille = m_TreeNomenclature.GetRootItem();
					HBranchFamille = m_TreeNomenclature.GetNextItem(HBranchFamille,TVGN_NEXT);
					HBranchFamille = m_TreeNomenclature.GetNextItem(HBranchFamille,TVGN_NEXT);

					TxtCampagne = m_TreeNomenclature.GetItemText(m_BufHItemCopie);

					// Déplacement au niveau de l'arbo famille secteur
					m_TreeNomenclature.DeplacerCampagneAnnonceurDansSecteur(HBranchFamille,pNoeudExParent->m_Famille,pNoeudExParent->m_Secteur,pNoeudExParent->m_Alias,pNoeudParent->m_Famille,pNoeudParent->m_Secteur,pNoeudParent->m_Alias,TxtCampagne);
				}
			
				// Recup Noeud parent
				if (pNoeudEnfant != NULL)
				{
					// Puis on vire le lien ancien parent / enfant déplacer
					// Maj ExParent.m_pNoeud[]
					// Ce n'est donc pas une feuille CAMPAGNE
					NbNoeud = pNoeudExParent->m_pNoeud.GetSize();

					for (int i= 0;i<NbNoeud;i++)
					{
						if (pNoeudExParent->m_pNoeud[i] == pNoeudEnfant)
						{
							pNoeudExParent->m_pNoeud.RemoveAt(i);
							break;
						}
					}
				}

				// ici on a choisi un Couper/Coller on vire la branche ou élmt
				if (m_InxMenuCur == Popup_Couper)   
				{
					m_TreeNomenclature.GetItemImage(m_BufHItemCopie,CodeDessin,CodeDessin);
					if (CodeDessin == CTreeCtrlX::ImageCampagne)
					{
						// Déplacement du fichier campagne
						FicACopier = pNoeudExParent->m_PathFichier + "\\" + m_TreeNomenclature.GetItemText(m_BufHItemCopie) + ".cmp";
						NvFic = pNoeudParent->m_PathFichier + "\\" + m_TreeNomenclature.GetItemText(m_BufHItemCopie) + ".cmp";
						if (CopyFile(FicACopier,NvFic,TRUE) !=0)
						{
							// suppression du fichier
							if (SuppressionFichier(m_BufHItemCopie,"cmp") == true)
							{
								// suppression de la feuille campagne
								Ok = m_TreeNomenclature.SupprimerElmt(m_BufHItemCopie);	
							}	

							// déplacement de la feuille campagne dans arbo marque si associé à produit
							// A FAIRE
						}
					}
						
					else
					{
						// Suppression branche sans virer noeud mémoire
						HBranchMarque = m_TreeNomenclature.GetRootItem();
						HBranchMarque = m_TreeNomenclature.GetNextItem(HBranchMarque,TVGN_NEXT);
						HBranchFamille = m_TreeNomenclature.GetNextItem(HBranchMarque,TVGN_NEXT);
						Ok = m_TreeNomenclature.SupprimerBranche(m_BufHItemCopie,m_BufHItemCopie,false,false,HBranchMarque,HBranchFamille);	
					}

				}

				// Synchronisation arborescence avec fichier arb
				m_TreeNomenclature.TrierArbre();
				SynchroArbre_FicArb();

			}
		}

		m_BufHItemCopie = 0;
		m_InxMenuCur = Popup_Coller;


	}

	else
	{
		// Impossible de copier une branche sur la meme branche ?????
		AfxMessageBox("Impossible de copier au même emplacement");
	}
	
}

// Suppression de la branche en cours ou d'un elmt campagne
void CNomenclatureView::OnMenunomenclSupprimerbranche() 
{
	HTREEITEM HItemMarque;
	HTREEITEM HItemFamille;
	HTREEITEM HItemParent;
	CNoeud *pNoeudParent;
	CNoeud *pNoeud;
	int CodeDessin;
	int NbNoeud;
	int NbSecteur;
	int i;
	CString TxtItem,TxtFamille;
	bool Ok;
	bool SuppressionPossible = true;

	if (m_HItemCur != 0)
	{
		m_TreeNomenclature.GetItemImage(m_HItemCur,CodeDessin,CodeDessin);		
		TxtItem = m_TreeNomenclature.GetItemText(m_HItemCur);
		//if (TxtItem.GetLenght() > 2) TxtItem = TxtItem.Left(3);
		TxtItem.TrimLeft();
		TxtItem.TrimRight();

		if (CodeDessin == CTreeCtrlX::ImageCampagne)
		{		
			// Suppression campagne
			//if (AfxMessageBox("Suppression fichier campagne " + TxtItem,MB_OKCANCEL) == IDOK)
			if (MessageBox( "Voulez-vous supprimer le fichier campagne "+ TxtItem, "Confirmer", MB_OKCANCEL|MB_ICONQUESTION ) == IDOK)
			{
				// Suppression du fichier campagne
				if (SuppressionFichier(m_HItemCur,"cmp") == true)
				{
					Ok = m_TreeNomenclature.SupprimerElmt(m_HItemCur);	
				}
			}
		}

		else
		{
			// Test si au moins une campagne sur la branche
			if (m_TreeNomenclature.FichiersCampagnePresent(m_HItemCur) == true)
			{
				// impossible de supprimer une branche avec des campagnes					
				AfxMessageBox("La suppression de cette branche de la nomenclature est impossible, car elle contient des campagnes");
			}

			else
			{
				// Test des cas particuliers Marque,Famille,Secteur
				
				if (CodeDessin == CTreeCtrlX::ImageMarque) 
				{
					// Marque suppression possible si aucun produit associé
					pNoeud = (CNoeud*)m_TreeNomenclature.GetItemData(m_HItemCur);
					if (pNoeud->m_pNoeud.GetSize() > 0)
					{
						// Ceratins produits sont associés à cette marque
						AfxMessageBox("Suppression de cette marque imposssible \nDes produits lui sont référencés");
						SuppressionPossible = false;
					}
				}

				else if (CodeDessin == CTreeCtrlX::ImageFamille)
				{
					// Famille suppression possible si aucun secteur de cette famille avec produit
					pNoeud = (CNoeud*)m_TreeNomenclature.GetItemData(m_HItemCur);
					NbSecteur = pNoeud->m_pNoeud.GetSize();
					if (NbSecteur > 0)
					{
						// Test si au moins 1 produit sur 1 secteur de cette famille
						for (i= 0;i< NbSecteur;i++) 
						{
							if (pNoeud->m_pNoeud[i]->m_pNoeud.GetSize() > 0)
							{
								// ici au moins 1 produit associé à ce secteur
								AfxMessageBox("Suppression de cette famille impossible \n Certains secteurs ont des produits associés");
								SuppressionPossible = false;
								break;
							}
						}
					}

					// on doit également vérifier pour les autres nomenclatures années
					// aucun produit n'est associé à cette famille
					if (SuppressionPossible == true && FamilleAuMoins1ProduitAutresAnnees(TxtItem) == true)
					{
						AfxMessageBox("Attention cette famille est utilisée dans d'autres nomenclatures, vous ne pouvez pas la supprimer");
						SuppressionPossible = false;
					}
	

				}

				else if (CodeDessin == CTreeCtrlX::ImageSecteur)
				{
					// Secteur suppression possible si aucun secteur de cette famille avec produit
					pNoeud = (CNoeud*)m_TreeNomenclature.GetItemData(m_HItemCur);
					if (pNoeud->m_pNoeud.GetSize() > 0)
					{
						// Certains produits sont associés à ce secteur
						AfxMessageBox("Suppression de ce secteur impossible \nDes produits lui sont référencés");	
						SuppressionPossible = false;
					}

					// on doit également vérifier pour les autres nomenclatures années
					// aucun produit n'est associé à ce secteur
					// A FAIRE...... (voir produit.arb)
					HItemParent= m_TreeNomenclature.GetParentItem(m_HItemCur);
					TxtFamille = m_TreeNomenclature.GetItemText(HItemParent);
					if (SuppressionPossible == true && SecteurAuMoins1ProduitAutresAnnees(TxtFamille,TxtItem) == true)
					{
						AfxMessageBox("Attention ce secteur est utilisé dans d'autres nomenclatures, vous ne pouvez pas le supprimer");
						SuppressionPossible = false;
					}
				}


				else if (CodeDessin == CTreeCtrlX::ImageGroupe && TxtItem == "...")
				{
					// Groupe spécial muet
					if (m_TreeNomenclature.ItemHasChildren(m_HItemCur))
					{
						AfxMessageBox("Attention groupe muet ayant des annonceurs,\n avant de le supprimer il faut supprimer les annonceurs un par un");
						SuppressionPossible = false;
					}
				}

				if (SuppressionPossible == true)
				{
					// Suppression Répertoire associé à cette branche
					if (MessageBox( "Voulez-vous supprimer la branche "+ TxtItem, "Confirmer", MB_OKCANCEL|MB_ICONQUESTION ) == IDOK)
					{
						pNoeud = (CNoeud*)m_TreeNomenclature.GetItemData(m_HItemCur);

						HItemParent= m_TreeNomenclature.GetParentItem(m_HItemCur);
						pNoeudParent = (CNoeud*)m_TreeNomenclature.GetItemData(HItemParent);

						// Suppression de la branche
						HItemMarque = m_TreeNomenclature.GetRootItem();
						HItemMarque = m_TreeNomenclature.GetNextItem(HItemMarque,TVGN_NEXT);
						HItemFamille = m_TreeNomenclature.GetNextItem(HItemMarque,TVGN_NEXT);

						TxtItem = m_TreeNomenclature.GetItemText(HItemMarque);
						Ok = m_TreeNomenclature.SupprimerBranche(m_HItemCur,m_HItemCur,true,true,HItemMarque,HItemFamille);	

						// Suppression du lien parent/enfant
						NbNoeud = pNoeudParent->m_pNoeud.GetSize();
						for (i= 0;i<NbNoeud;i++)
						{
							if (pNoeudParent->m_pNoeud[i] == pNoeud)
							{
								pNoeudParent->m_pNoeud.RemoveAt(i);
								break;
							}
						}

						// Synchronisation arborescence avec fichier arb
						RedrawWindow();
						SynchroArbre_FicArb();

					}
				}
			}
		}
	}	
	m_InxMenuCur = Popup_Supprimer;

}



// Ici on renomme le nom d'une branche ou d'une elmt
void CNomenclatureView::OnMenunomenclRenommerbranche() 
{
	CFormulaireNom FormulaireNom;
	CFormulaire FormulaireP;
	CFormulaireGroupe FormulaireG;
	CFormulaireStandard FormulaireStd;
	CFileFind File;
	CString ExNom,NvNom,ExAlias;
	CString NomRep,NvNomRep,ExNomRep;
	CString ExMarque;
	CString NomFamille;
	CString	TextItem;
	CString TxtItemParent;
	CNoeud *pNoeud;
	CNoeud *pNoeudParent;
	HTREEITEM HItemProduit = NULL;
	HTREEITEM HItemMarque = NULL;
	HTREEITEM HItemFamille = NULL;
	HTREEITEM HItemParent = NULL;
	HTREEITEM HItem;
	int CodeDessin,CodeDessinParent;
	CString NvNomFichier,OldNomFichier;
	bool AnnonceurDirect = false;

	// De quel élmt il s'agit
	m_TreeNomenclature.GetItemImage(m_HItemCur,CodeDessin,CodeDessin);
	pNoeud = (CNoeud*)m_TreeNomenclature.GetItemData(m_HItemCur);

	// le noeud parent au cas ou
	HItemParent = m_TreeNomenclature.GetParentItem(m_HItemCur);
	if (HItemParent != NULL) pNoeudParent = (CNoeud*)m_TreeNomenclature.GetItemData(HItemParent);

	// De quel parent
	HItemParent = m_TreeNomenclature.GetParentItem(m_HItemCur);
	m_TreeNomenclature.GetItemImage(HItemParent,CodeDessinParent,CodeDessinParent);

	// Nom du parent (permet de détecter les annonceurs sans groupe / bidouille !!!)
	TxtItemParent = m_TreeNomenclature.GetItemText(HItemParent);
	//if (TxtItemParent.GetLenght() > 2) TxtItemParent = TxtItemParent.Left(3);
	TxtItemParent.TrimLeft();
	TxtItemParent.TrimRight();
	if (TxtItemParent == "...") 	AnnonceurDirect = true;


	// Entete Fenetre de dialogue
	FormulaireP.m_TypeAnnonceur   = false;
	FormulaireStd.m_TypeAnnonceur = false;
	switch (CodeDessin)
	{
		case CTreeCtrlX::ImageMarque :
		{
			FormulaireP.m_TitreFenFP = "Définir les références de la marque";
			break;
		}
		case CTreeCtrlX::ImageProduit :
		{
			FormulaireP.m_TitreFenFP = "Définir les références du produit";
			break;
		}
		case CTreeCtrlX::ImageGroupe :
		{
			FormulaireP.m_TitreFenFP = "Définir les références du groupe  (Année exercice " + theApp.m_AnneeNomencl    + ")";
			break;
		}
		case CTreeCtrlX::ImageAnnonceur :
		{
			FormulaireP.m_TitreFenFP      = "Définir les références de l'annonceur";
			FormulaireP.m_TypeAnnonceur   = true;
			FormulaireStd.m_TypeAnnonceur = true;
			break;
		}
		case CTreeCtrlX::ImageCampagne :
		{
			FormulaireP.m_TitreFenFP = "Définir les références de la campagne";
			break;
		}
		case CTreeCtrlX::ImageFamille :
		{
			FormulaireP.m_TitreFenFP = "Définir les références de la famille";
			break;
		}
		case CTreeCtrlX::ImageSecteur :
		{
			FormulaireP.m_TitreFenFP = "Définir les références du secteur";
			break;
		}
	}

	FormulaireStd.m_TitreFenStd =  FormulaireP.m_TitreFenFP;
	FormulaireG.m_TitreFenFG =  FormulaireP.m_TitreFenFP;

	if (CodeDessin == CTreeCtrlX::ImageMarque ||
		CodeDessin == CTreeCtrlX::ImageCampagne ||
		CodeDessin == CTreeCtrlX::ImageFamille ||
		CodeDessin == CTreeCtrlX::ImageSecteur) 
		
	{
		// Affiche nom branche ou elmt dans formulaire
		FormulaireNom.m_StrAliasFN = m_TreeNomenclature.GetItemText(m_HItemCur);

		if (CodeDessin == CTreeCtrlX::ImageMarque ||
			CodeDessin == CTreeCtrlX::ImageCampagne ||
			CodeDessin == CTreeCtrlX::ImageFamille ||
			CodeDessin == CTreeCtrlX::ImageSecteur) 
			ExNom = FormulaireNom.m_StrAliasFN;

		// Attention on peut renommer marque / groupe / annonceur / produit
		// donc on récup l'id image pour le renvoyer au formulaire
		FormulaireNom.m_IdImageItem = CodeDessin;


		// Vide tous les tableaux marque,famille,secteur
		FormulaireNom.m_TabMarque.RemoveAll();
		FormulaireNom.m_TabFamille.RemoveAll();
		FormulaireNom.m_TabSecteur.RemoveAll();

		switch (CodeDessin)
		{
			case CTreeCtrlX::ImageMarque :
			{
				// Renommer une marque / on charge les marques existantes pour exister les doublons
				FormulaireNom.m_TitreFenN = "Définir le nom de la marque";
				ChargeTabMarque(FormulaireNom.m_TabMarque);
				break;
			}
			case CTreeCtrlX::ImageCampagne:
			{
				// Renommer une campagne
				FormulaireNom.m_TitreFenN = "Définir le nom de la campagne";
				break;
			}
			case CTreeCtrlX::ImageFamille:
			{
				// Renommer une famille / on charge les familles existantes pour exister les doublons
				FormulaireNom.m_TitreFenN = "Définir le nom de la famille";
				ChargeTabFamille(FormulaireNom.m_TabFamille);
				break;
			}
			case CTreeCtrlX::ImageSecteur:
			{
				// Renommer un secteur / on charge les secteurs existants pour exister les doublons
				FormulaireNom.m_TitreFenN = "Définir le nom du secteur";
				NomFamille = m_TreeNomenclature.GetItemText(HItemParent);
				ChargeTabSecteur(FormulaireNom.m_TabSecteur,NomFamille);
				break;
			}
		}

		if (FormulaireNom.DoModal() == IDOK)
		{
			// Recup nouvelles modifs pour la branche ou élmt
			if (CodeDessin == CTreeCtrlX::ImageMarque ||
				CodeDessin == CTreeCtrlX::ImageFamille ||
				CodeDessin == CTreeCtrlX::ImageSecteur)

			{
				// Maj Alias noeud à renommer
				ExAlias = pNoeud->m_Alias;
				pNoeud->m_Alias = FormulaireNom.m_StrAliasFN;

				// Enfin maj nom marque pour les produits concernés
				NvNom = FormulaireNom.m_StrAliasFN;

				// on pointe sur item racines des produits
				HItemProduit = m_TreeNomenclature.GetRootItem();
				//HItemProduit = m_TreeNomenclature.GetNextItem(HItemProduit,TVGN_NEXT);
				
				if (CodeDessin == CTreeCtrlX::ImageMarque)
					// Maj de l'info marque produit
					m_TreeNomenclature.MajMarqueProduit(HItemProduit,ExNom,NvNom);
				else if (CodeDessin == CTreeCtrlX::ImageFamille)
					// Maj de l'info famille
					m_TreeNomenclature.MajFamilleProduit(HItemProduit,ExNom,NvNom);
				else
					// Maj de l'info famille
					m_TreeNomenclature.MajSecteurProduit(HItemProduit,ExNom,NvNom);
				
				// Maj Texte Libelle Branche
				m_TreeNomenclature.SetItemText(m_HItemCur,FormulaireNom.m_StrAliasFN);

			}


			else if ((CodeDessin == CTreeCtrlX::ImageCampagne) && (ExNom != FormulaireNom.m_StrAliasFN))
			{
				// recup branche parent
				HItemParent =m_TreeNomenclature.GetParentItem(m_HItemCur);
				pNoeudParent = (CNoeud*)m_TreeNomenclature.GetItemData(HItemParent);

				// cas changement nom fichier campagne
				OldNomFichier = pNoeudParent->m_PathFichier + "\\" + ExNom;
				NvNomFichier = pNoeudParent->m_PathFichier + "\\" + FormulaireNom.m_StrAliasFN;

				if (CopieFicItemPossible(m_HItemCur,HItemParent,NvNomFichier) == true)
				{						
					// nouveau nom fichier non utilisé
					rename(OldNomFichier+".cmp",NvNomFichier+".cmp");
					m_TreeNomenclature.SetItemText(m_HItemCur,FormulaireNom.m_StrAliasFN);
				}
				else
				{
					// nom déjà utilsé
					AfxMessageBox("Nom fichier déjà utilisé");
				}

			}

		}	
	}

	else
	{
		// Ici il s'agit de modifier également les autres champs
		if (CodeDessin == CTreeCtrlX::ImageProduit && CodeDessinParent == CTreeCtrlX::ImageAnnonceur)
		{
			// Modif produit il faut charger la liste des marques disponibles
			FormulaireP.m_StrAliasFP = pNoeud->m_Alias;
			FormulaireP.m_StrCodeFP = pNoeud->m_Codif;
			FormulaireP.m_StrNomRepFP = pNoeud->m_Libelle;
			FormulaireP.m_StrMarqueFP = pNoeud->m_Marque;
			FormulaireP.m_StrFamilleFP = pNoeud->m_Famille;
			FormulaireP.m_StrSecteurFP = pNoeud->m_Secteur;
			FormulaireP.m_pNoeudFamille = m_ArbreEnMemoire.m_pNoeud[2];
			
			// Chargement des marques disponibles
			if (ChargeTabMarque(FormulaireP.m_TabMarque)== true)
			{
				if (ChargeTabFamille(FormulaireP.m_TabFamille)== true)
				{
					if (FormulaireP.DoModal() == IDOK)
					{
						// Changement nom alias produit
						if (FormulaireP.m_StrAliasFP != pNoeud->m_Alias )
						{
							// Maj nom du produit sur arborescence marque
							HItemMarque = m_TreeNomenclature.GetRootItem();
							HItemMarque = m_TreeNomenclature.GetNextItem(HItemMarque,TVGN_NEXT);
						
							m_TreeNomenclature.RenommerProduitMarque(HItemMarque,pNoeud->m_Marque,pNoeud->m_Alias ,FormulaireP.m_StrAliasFP);

							// Maj Nom du produit sur arborescence famille
							HItemFamille = m_TreeNomenclature.GetNextItem(HItemMarque,TVGN_NEXT);
							m_TreeNomenclature.RenommerProduitFamille(HItemFamille,pNoeud->m_Famille,pNoeud->m_Secteur,pNoeud->m_Alias ,FormulaireP.m_StrAliasFP);

							// Modif nom au niveau noeud et arborescence
							pNoeud->m_Alias = FormulaireP.m_StrAliasFP;
							m_TreeNomenclature.SetItemText(m_HItemCur,pNoeud->m_Alias);
						}

						// Changement codif produit
						if (FormulaireP.m_StrCodeFP != pNoeud->m_Codif)
						{
							pNoeud->m_Codif = FormulaireP.m_StrCodeFP;
						}

						// Modif nom répertoire, il faut le remonner
						if (FormulaireP.m_StrNomRepFP != pNoeud->m_Libelle)
						{
							NomRep = pNoeudParent->m_PathFichier + "\\"  +  pNoeud->m_Libelle;
							NvNomRep = pNoeudParent->m_PathFichier + "\\" + FormulaireP.m_StrNomRepFP;

							if (SsRepExist(pNoeudParent,NomRep) == true)
							{
								// Nom répertoire déjà utilisé
								AfxMessageBox("Répertoire déjà existant dans " + pNoeudParent->m_PathFichier);
							}
							else
							{
								if (rename(NomRep,NvNomRep) == 0)
								{
									// maj nom rep au niveau du noeud produit
									pNoeud->m_Libelle = FormulaireP.m_StrNomRepFP;
									pNoeud ->m_PathFichier = NvNomRep;

									// Maj Path des niveaux inférieures
									MajPathNoeudNiveauInf(pNoeud);
								}
							}
						}
		
						// Modif marque produit >> maj produit marque
						if (FormulaireP.m_StrMarqueFP != pNoeud ->m_Marque)
						{
							HItemMarque = m_TreeNomenclature.GetRootItem();
							HItemMarque = m_TreeNomenclature.GetNextItem(HItemMarque,TVGN_NEXT);

							// Suppression produit de l'ancienne marque
							m_TreeNomenclature.VirerProduitDansMarque(HItemMarque,pNoeud ->m_Marque,pNoeud->m_Alias);

							// Ajout du nouveau produit dans marque
							m_TreeNomenclature.AjouterProduitDansMarque(HItemMarque,
																		FormulaireP.m_StrMarqueFP,
																		FormulaireP.m_StrAliasFP,
																		FormulaireP.m_StrNomRepFP,
																		FormulaireP.m_StrCodeFP);

							// Modif Bug Manque Campagne Produit déplacé marque (Juin 2000)
							// Déplacement campagne au niveau de l'arbo marque
							if (m_TreeNomenclature.ItemHasChildren(m_HItemCur))
							{
								// enfants sur cette branche
								HItem = m_TreeNomenclature.GetChildItem(m_HItemCur);
								while (HItem != NULL)
								{
									TextItem = m_TreeNomenclature.GetItemText(HItem);
									m_TreeNomenclature.DeplacerCampagneProduitDansMarque(HItemMarque,pNoeud->m_Marque, pNoeud->m_Alias,FormulaireP.m_StrMarqueFP,pNoeud->m_Alias,TextItem);
									HItem = m_TreeNomenclature.GetNextItem(HItem,TVGN_NEXT);			
								}
							}		
							// remet nouveau nom au niveau du noeud
							pNoeud->m_Marque = FormulaireP.m_StrMarqueFP;

						}

						// Modif famille/secteur produit >> maj produit famille/secteur
						if (FormulaireP.m_StrFamilleFP != pNoeud ->m_Famille || FormulaireP.m_StrSecteurFP != pNoeud ->m_Secteur)
						{
							HItemMarque = m_TreeNomenclature.GetRootItem();
							HItemMarque = m_TreeNomenclature.GetNextItem(HItemMarque,TVGN_NEXT);
							HItemFamille =  m_TreeNomenclature.GetNextItem(HItemMarque,TVGN_NEXT);

							// Suppression produit de l'ancienne famille/secteur
							m_TreeNomenclature.VirerProduitDansFamilleSecteur(HItemFamille,pNoeud ->m_Famille,pNoeud ->m_Secteur,pNoeud->m_Alias);

							// Ajout du nouveau produit dans famille/secteur
							m_TreeNomenclature.AjouterProduitDansFamilleSecteur(HItemFamille,
																			    FormulaireP.m_StrFamilleFP,
																				FormulaireP.m_StrSecteurFP,
																				FormulaireP.m_StrAliasFP,
																				FormulaireP.m_StrNomRepFP,
																				FormulaireP.m_StrCodeFP);
	
							// Modif Bug Manque Campagne Produit déplacé famille/secteur (Juin 2000)
							// Déplacement campagne au niveau de l'arbo famille secteur
							if (m_TreeNomenclature.ItemHasChildren(m_HItemCur))
							{
								// enfants sur cette branche
								HItem = m_TreeNomenclature.GetChildItem(m_HItemCur);
								while (HItem != NULL)
								{
									TextItem = m_TreeNomenclature.GetItemText(HItem);
									m_TreeNomenclature.DeplacerCampagneProduitDansSecteur(HItemFamille,pNoeud->m_Famille,pNoeud->m_Secteur,pNoeud->m_Alias,FormulaireP.m_StrFamilleFP,FormulaireP.m_StrSecteurFP,pNoeud->m_Alias,TextItem);
									HItem = m_TreeNomenclature.GetNextItem(HItem,TVGN_NEXT);			
								}
							}		

							// remet nouveau nom au niveau du noeud
							pNoeud->m_Famille = FormulaireP.m_StrFamilleFP;
							pNoeud->m_Secteur = FormulaireP.m_StrSecteurFP;


						}
					}
				}
			}
		}

		// Renommer groupe ou Annonceur Direct avec Famille et Secteur
		else if (
				(CodeDessin == CTreeCtrlX::ImageGroupe) ||
				(AnnonceurDirect == true  && (pNoeud->m_Famille == "" || pNoeud->m_Famille == "xxx"))
				)
		{
			FormulaireG.m_StrAliasFG = pNoeud->m_Alias;
			FormulaireG.m_StrCodeFG =pNoeud->m_Codif;
			FormulaireG.m_StrNomRepFG = pNoeud->m_Libelle;
			FormulaireG.m_StrDateFG = pNoeud->m_DateExercice;
			FormulaireG.m_DateParDefaut = "0101" + theApp.m_AnneeNomencl;

			// distinction si annonceur direct
			FormulaireG.m_Annonceur = AnnonceurDirect; 

			// mode modification
			FormulaireG.m_Mode = 1;

			if (FormulaireG.DoModal() == IDOK)
			{
				// Changement nom alias groupe
				if (FormulaireG.m_StrAliasFG != pNoeud->m_Alias)
				{
					// Modif nom au niveau noeud et arborescence
					pNoeud->m_Alias = FormulaireG.m_StrAliasFG;
					m_TreeNomenclature.SetItemText(m_HItemCur,pNoeud->m_Alias);
				}

				// Changement codif produit
				if (FormulaireG.m_StrCodeFG != pNoeud->m_Codif)
					pNoeud->m_Codif = FormulaireG.m_StrCodeFG;

				// Changement date exercice
				if (FormulaireG.m_StrDateFG != pNoeud->m_DateExercice)
					pNoeud->m_DateExercice = FormulaireG.m_StrDateFG;

				// Modif nom répertoire, il faut le remonner
				if (FormulaireG.m_StrNomRepFG != pNoeud->m_Libelle)
				{
					NomRep = pNoeudParent->m_PathFichier + "\\" + pNoeud->m_Libelle;
					NvNomRep = pNoeudParent->m_PathFichier + "\\" + FormulaireG.m_StrNomRepFG;

					if (SsRepExist(pNoeudParent,FormulaireG.m_StrNomRepFG) == true)
					{
						// Nom répertoire déjà utilisé
						AfxMessageBox("Répertoire déjà existant dans " + pNoeudParent->m_PathFichier);
					}
					else
					{
						if (rename(NomRep,NvNomRep) == 0)
						{
							// maj nom rep au niveau du noeud 
							pNoeud->m_Libelle = FormulaireG.m_StrNomRepFG;
							pNoeud ->m_PathFichier = NvNomRep;

							// Maj Path des niveaux inférieures
							MajPathNoeudNiveauInf(pNoeud);
						}
					}
				}
			}
		}
		
		else if (AnnonceurDirect == true && pNoeud->m_Famille != "" && pNoeud->m_Famille != "xxx")

		// Gestion des annonceurs sans groupe sans produit
		{
			// Cas d'un annonceur avec famille et secteur à choisir
			// Modif produit il faut charger la liste des marques disponibles
			FormulaireP.m_StrAliasFP = pNoeud->m_Alias;
			FormulaireP.m_StrCodeFP = pNoeud->m_Codif;
			FormulaireP.m_StrNomRepFP = pNoeud->m_Libelle;
			FormulaireP.m_StrMarqueFP = pNoeud->m_Marque;
			FormulaireP.m_StrFamilleFP = pNoeud->m_Famille;
			FormulaireP.m_StrSecteurFP = pNoeud->m_Secteur;
			FormulaireP.m_StrDateFP = pNoeud->m_DateExercice;
			FormulaireP.m_DateParDefaut = "0101" + theApp.m_AnneeNomencl;
			FormulaireP.m_pNoeudFamille = m_ArbreEnMemoire.m_pNoeud[2];
		
			// Chargement des marques disponibles
			if (ChargeTabFamille(FormulaireP.m_TabFamille)== true)
			{
				if (FormulaireP.DoModal() == IDOK)
				{
					// Changement nom alias produit
					if (FormulaireP.m_StrAliasFP != pNoeud->m_Alias )
					{
						HItemMarque = m_TreeNomenclature.GetRootItem();
						HItemMarque = m_TreeNomenclature.GetNextItem(HItemMarque,TVGN_NEXT);

						// Maj Nom Annonceur sur arborescence famille
						HItemFamille = m_TreeNomenclature.GetNextItem(HItemMarque,TVGN_NEXT);

						// Mise a jour nom annonceur
						m_TreeNomenclature.RenommerAnnonceurFamille(HItemFamille,pNoeud->m_Famille,pNoeud->m_Secteur,pNoeud->m_Alias ,FormulaireP.m_StrAliasFP);

						// Modif nom au niveau noeud et arborescence
						pNoeud->m_Alias = FormulaireP.m_StrAliasFP;
						m_TreeNomenclature.SetItemText(m_HItemCur,pNoeud->m_Alias);
					}

					// Changement codif produit
					if (FormulaireP.m_StrCodeFP != pNoeud->m_Codif)
					{
						pNoeud->m_Codif = FormulaireP.m_StrCodeFP;
					}

					// Changement date exercice
					if (FormulaireP.m_StrDateFP != pNoeud->m_DateExercice)
					{
						pNoeud->m_DateExercice = FormulaireP.m_StrDateFP;
					}
					// Modif nom répertoire, il faut le remonner
					if (FormulaireP.m_StrNomRepFP != pNoeud->m_Libelle)
					{
						NomRep = pNoeudParent->m_PathFichier + "\\"  +  pNoeud->m_Libelle;
						NvNomRep = pNoeudParent->m_PathFichier + "\\" + FormulaireP.m_StrNomRepFP;

						if (SsRepExist(pNoeudParent,NomRep) == true)
						{
							// Nom répertoire déjà utilisé
							AfxMessageBox("Répertoire déjà existant dans " + pNoeudParent->m_PathFichier);
						}
						else
						{
							if (rename(NomRep,NvNomRep) == 0)
							{
								// maj nom rep au niveau du noeud annonceur
								pNoeud->m_Libelle = FormulaireP.m_StrNomRepFP;
								pNoeud ->m_PathFichier = NvNomRep;

								// Maj Path des niveaux inférieures
								MajPathNoeudNiveauInf(pNoeud);
							}
						}
					}
	
					// Modif famille/secteur produit >> maj produit famille/secteur
					if (FormulaireP.m_StrFamilleFP != pNoeud ->m_Famille || FormulaireP.m_StrSecteurFP != pNoeud ->m_Secteur)
					{
						HItemMarque = m_TreeNomenclature.GetRootItem();
						HItemMarque = m_TreeNomenclature.GetNextItem(HItemMarque,TVGN_NEXT);
						HItemFamille =  m_TreeNomenclature.GetNextItem(HItemMarque,TVGN_NEXT);

						// Suppression produit de l'ancienne famille/secteur
						m_TreeNomenclature.VirerAnnonceurDansFamilleSecteur(HItemFamille,pNoeud ->m_Famille,pNoeud ->m_Secteur,pNoeud->m_Alias);

						// Ajout du nouvel annonceur dans famille/secteur
						m_TreeNomenclature.AjouterAnnonceurDansFamilleSecteur(HItemFamille,
																			  FormulaireP.m_StrFamilleFP,
																			  FormulaireP.m_StrSecteurFP,
																			  FormulaireP.m_StrAliasFP,
																			  FormulaireP.m_StrNomRepFP,
																			  FormulaireP.m_StrCodeFP,
																			  FormulaireP.m_StrDateFP);

						
						// Déplacement campagne de l'annonceur au niveau de l'arbo famille secteur
						if (m_TreeNomenclature.ItemHasChildren(m_HItemCur))
						{
							// enfants sur cette branche
							HItem = m_TreeNomenclature.GetChildItem(m_HItemCur);
							while (HItem != NULL)
							{
								TextItem = m_TreeNomenclature.GetItemText(HItem);
								m_TreeNomenclature.DeplacerCampagneAnnonceurDansSecteur(HItemFamille,pNoeud->m_Famille,pNoeud->m_Secteur,pNoeud->m_Alias,FormulaireP.m_StrFamilleFP,FormulaireP.m_StrSecteurFP,pNoeud->m_Alias,TextItem);
								HItem = m_TreeNomenclature.GetNextItem(HItem,TVGN_NEXT);			
							}
						}
	

						// remet nouveau nom au niveau du noeud
						pNoeud->m_Famille = FormulaireP.m_StrFamilleFP;
						pNoeud->m_Secteur = FormulaireP.m_StrSecteurFP;

					}
				}				
			}

		}

		else
		{
			// Modif des autres types nomenclature (Groupe,annonceur)
			FormulaireStd.m_StrAliasFStd = pNoeud->m_Alias;
			FormulaireStd.m_StrCodeFStd =pNoeud->m_Codif;
			FormulaireStd.m_StrNomRepFStd = pNoeud->m_Libelle;


			// Recup Info EDI pour envoi Formulaire
			FormulaireStd.m_TypeTiers       = pNoeud->m_TypeTiers;
			FormulaireStd.m_CodeTiers       = pNoeud->m_CodeTiers;			
			FormulaireStd.m_TypeCodeTiers   = pNoeud->m_TypeCodeTiers;		
			FormulaireStd.m_NomTiers        = pNoeud->m_NomTiers;				
			FormulaireStd.m_AdresseTiers1   = pNoeud->m_AdresseTiers1;		
			FormulaireStd.m_AdresseTiers2   = pNoeud->m_AdresseTiers2;;		
			FormulaireStd.m_CodePostalTiers = pNoeud->m_CodePostalTiers;		
			FormulaireStd.m_VilleTiers      = pNoeud->m_VilleTiers;			
			FormulaireStd.m_CodePaysTiers   = pNoeud->m_CodePaysTiers;		

			// Interlocuteur(s) tiers
			FormulaireStd.m_NomContactTiers     = pNoeud->m_NomContactTiers;		
			FormulaireStd.m_PrenomContactTiers  = pNoeud->m_PrenomContactTiers;	
			FormulaireStd.m_AdrMailContactTiers = pNoeud->m_AdrMailContactTiers;	
			
			FormulaireStd.m_NoTelTiers          = pNoeud->m_NoTelTiers;			
			FormulaireStd.m_NoFaxTiers          = pNoeud->m_NoFaxTiers;		  	

			// Mode Modification
			FormulaireStd.m_Mode = 1;

			if (FormulaireStd.DoModal() == IDOK)
			{	
				// Changement nom alias produit
				if (FormulaireStd.m_StrAliasFStd != pNoeud->m_Alias)
				{
					// Modif nom au niveau noeud et arborescence
					pNoeud->m_Alias = FormulaireStd.m_StrAliasFStd;
					m_TreeNomenclature.SetItemText(m_HItemCur,pNoeud->m_Alias);
				}

				// Changement codif produit
				if (FormulaireStd.m_StrCodeFStd != pNoeud->m_Codif)
					pNoeud->m_Codif = FormulaireStd.m_StrCodeFStd;

				// Modif nom répertoire, il faut le remonner
				if (FormulaireStd.m_StrNomRepFStd != pNoeud->m_Libelle)
				{
					NomRep = pNoeudParent->m_PathFichier + "\\" + pNoeud->m_Libelle;
					NvNomRep = pNoeudParent->m_PathFichier + "\\" + FormulaireStd.m_StrNomRepFStd;

					if (SsRepExist(pNoeudParent,FormulaireStd.m_StrNomRepFStd) == true)
					{
						// Nom répertoire déjà utilisé
						AfxMessageBox("Répertoire déjà existant dans " + pNoeudParent->m_PathFichier);
					}
					else
					{
						if (rename(NomRep,NvNomRep) == 0)
						{
							// maj nom rep au niveau du noeud 
							pNoeud->m_Libelle = FormulaireStd.m_StrNomRepFStd;
							pNoeud ->m_PathFichier = NvNomRep;

							// Maj Path des niveaux inférieures
							MajPathNoeudNiveauInf(pNoeud);
						}
					}
				}

				// A FAIRE Mise à jour des infos EDI
				pNoeud->m_TypeTiers			  = FormulaireStd.m_TypeTiers;
				pNoeud->m_CodeTiers			  = FormulaireStd.m_CodeTiers;			
				pNoeud->m_TypeCodeTiers		  = FormulaireStd.m_TypeCodeTiers;		
				pNoeud->m_NomTiers			  = FormulaireStd.m_NomTiers;				
				pNoeud->m_AdresseTiers1		  = FormulaireStd.m_AdresseTiers1;		
				pNoeud->m_AdresseTiers2		  = FormulaireStd.m_AdresseTiers2;		
				pNoeud->m_CodePostalTiers	  = FormulaireStd.m_CodePostalTiers;		
				pNoeud->m_VilleTiers		  = FormulaireStd.m_VilleTiers;			
				pNoeud->m_CodePaysTiers		  = FormulaireStd.m_CodePaysTiers;		

				// Interlocuteur(s) tiers
				pNoeud->m_NomContactTiers	  = FormulaireStd.m_NomContactTiers;		
				pNoeud->m_PrenomContactTiers  = FormulaireStd.m_PrenomContactTiers;	
				pNoeud->m_AdrMailContactTiers = FormulaireStd.m_AdrMailContactTiers;	
				
				pNoeud->m_NoTelTiers		  = FormulaireStd.m_NoTelTiers;			
				pNoeud->m_NoFaxTiers		  = FormulaireStd.m_NoFaxTiers;		  	

			}

		}

	}

	m_InxMenuCur = Popup_Renommer;

	// Synchronisation arborescence avec fichier arb
	m_TreeNomenclature.TrierArbre();	
	RedrawWindow();
	SynchroArbre_FicArb();


}

void CNomenclatureView::OnMenunomenclAjouternvllebranche() 
{
	// Ajout d'une nouvelle branche ou elmt
    // Attention on ajoute des elmts que sur le dernier niveau
	CFormulaire FormulaireP;
	CFormulaireNom FormulaireN;
	CFormulaireStandard FormulaireStd;
	CFormulaireGroupe FormulaireG;
	CString Txt1 = "xxx";
	CString Txt2 = "xxx";
	CString Txt3 = "xxx";
	CString Txt4 = "xxx";
	CString Txt5 = "xxx";
	CString Txt6 = "xxx";
	CString RepCourantCreation ="";
	CNoeud *pNoeud;

	HTREEITEM HNvItem = NULL;
	HTREEITEM hBrancheMarque = NULL;
	HTREEITEM hBrancheFamille = NULL;
	int IdDessin;
	int IdDessinEnfant;

	// Recup Ident Image Item
	m_TreeNomenclature.GetItemImage(m_HItemCur,IdDessin,IdDessin);

	// Recup Ident Image Enfant
	pNoeud = (CNoeud*)m_TreeNomenclature.GetItemData(m_HItemCur);		
	IdDessinEnfant = pNoeud->m_CodeImage % 100;

	// Entete Fenetre de dialogue
	FormulaireStd.m_TypeAnnonceur = false;
	switch (IdDessinEnfant)
	{
		case CTreeCtrlX::ImageMarque :
		{
			FormulaireStd.m_TitreFenStd = "Définir les références de la marque";
			break;
		}
		case CTreeCtrlX::ImageProduit :
		{
			FormulaireStd.m_TitreFenStd = "Définir les références du produit";
			break;
		}
		case CTreeCtrlX::ImageGroupe :
		{
			FormulaireStd.m_TitreFenStd = "Définir les références du groupe (Année exercice " + theApp.m_AnneeNomencl    + ")";
			break;
		}
		case CTreeCtrlX::ImageAnnonceur :
		{
			FormulaireStd.m_TitreFenStd = "Définir les références de l'annonceur";
			FormulaireStd.m_TypeAnnonceur = true;
			break;
		}
		case CTreeCtrlX::ImageCampagne :
		{
			FormulaireStd.m_TitreFenStd = "Définir les références de la campagne";
			break;
		}
		case CTreeCtrlX::ImageFamille :
		{
			FormulaireStd.m_TitreFenStd = "Définir les références de la famille";
			break;
		}
		case CTreeCtrlX::ImageSecteur :
		{
			FormulaireStd.m_TitreFenStd = "Définir les références du secteur";
			break;
		}
	}
	FormulaireP.m_TitreFenFP = FormulaireStd.m_TitreFenStd;
	FormulaireG.m_TitreFenFG = FormulaireStd.m_TitreFenStd;


	// Recup Chemin Courant Création Répertoire
	RepCourantCreation = pNoeud->m_PathFichier;

	// Selon type d'élemt à ajouter on affiche le formulaire adéquate
	if (IdDessinEnfant == CTreeCtrlX::ImageMarque ||
		IdDessinEnfant == CTreeCtrlX::ImageFamille ||
		IdDessinEnfant == CTreeCtrlX::ImageSecteur)
	{
		FormulaireN.m_StrAliasFN = "";

		switch (IdDessinEnfant)
		{
			case CTreeCtrlX::ImageMarque :		
			{
				FormulaireN.m_TitreFenN = "Définir le nom de la nouvelle marque";
				// Charge les marques existantes
				ChargeTabMarque(FormulaireN.m_TabMarque);
				break;
			}
			case CTreeCtrlX::ImageFamille :		
			{
				FormulaireN.m_TitreFenN = "Définir le nom de la nouvelle famille";
				// Charge les familles existantes
				ChargeTabFamille(FormulaireN.m_TabFamille);
				break;
			}
			case CTreeCtrlX::ImageSecteur :		
			{
				FormulaireN.m_TitreFenN = "Définir le nom du secteur";
				// Charge les secteurs  existants pour la famille mère
				ChargeTabSecteur(FormulaireN.m_TabSecteur,pNoeud->m_Famille);
				break;
			}
		}


		if (FormulaireN.DoModal() == IDOK)
		{
			HNvItem = m_TreeNomenclature.AjouterBranche(m_HItemCur,FormulaireN.m_StrAliasFN,Txt1,Txt2,Txt3,Txt4,Txt5,Txt6,false);	
		}
	}

	else if (IdDessin == CTreeCtrlX::ImageAnnonceur)
	{
		// Ajout d'un nouveau produit il faut charger la liste des marques disponibles
		// ainsi que la liste des familles et secteurs disponibles
		FormulaireP.m_StrAliasFP = "";
		FormulaireP.m_StrCodeFP ="";
		FormulaireP.m_StrNomRepFP = "";
		FormulaireP.m_StrMarqueFP = "";
		FormulaireP.m_pNoeudFamille = m_ArbreEnMemoire.m_pNoeud[2];

		// Chargement des marques disponibles
		if (ChargeTabMarque(FormulaireP.m_TabMarque)== true)
		{
			if (ChargeTabFamille(FormulaireP.m_TabFamille) == true)
			{
				if (FormulaireP.DoModal() == IDOK)
				{
					// Avant tout on vérifie possibilté de créér le répertoire
					RepCourantCreation = RepCourantCreation + "\\" + FormulaireP.m_StrNomRepFP;

					// Ajout Répertoire branche si branche correctement crée
					if (AjoutRepNvlleBranche(RepCourantCreation) == true)
					{
						// Ajout de la nvlle branche produit/ branche Annonceurs
						HNvItem = m_TreeNomenclature.AjouterBranche(m_HItemCur,FormulaireP.m_StrAliasFP,FormulaireP.m_StrNomRepFP,FormulaireP.m_StrCodeFP,FormulaireP.m_StrMarqueFP,FormulaireP.m_StrFamilleFP,FormulaireP.m_StrSecteurFP,Txt6,false);	

						// Ajout Produit au niveau de l'arborescence marque
						hBrancheMarque = m_TreeNomenclature.GetRootItem();
						hBrancheMarque = m_TreeNomenclature.GetNextItem(hBrancheMarque,TVGN_NEXT);

						m_TreeNomenclature.AjouterProduitDansMarque(hBrancheMarque,
																	FormulaireP.m_StrMarqueFP,
																	FormulaireP.m_StrAliasFP,
																	FormulaireP.m_StrNomRepFP,
																	FormulaireP.m_StrCodeFP);

						// Ajout Produit au niveau de l'arborescence famille/secteur
						hBrancheFamille = m_TreeNomenclature.GetRootItem();
						hBrancheFamille = m_TreeNomenclature.GetNextItem(hBrancheFamille,TVGN_NEXT);
						hBrancheFamille = m_TreeNomenclature.GetNextItem(hBrancheFamille,TVGN_NEXT);

						m_TreeNomenclature.AjouterProduitDansFamilleSecteur(hBrancheFamille,
																			FormulaireP.m_StrFamilleFP,
																			FormulaireP.m_StrSecteurFP,
																			FormulaireP.m_StrAliasFP,
																			FormulaireP.m_StrNomRepFP,
																			FormulaireP.m_StrCodeFP);
					}
				}
			}
		}

		else
		{
			AfxMessageBox ("Impossible d'ajouter un produit, aucune marque existante");
		}
		
	}

	else if (IdDessinEnfant == CTreeCtrlX::ImageGroupe)
	{
		// Ajout d'un nouveau groupe
		FormulaireG.m_StrAliasFG = "";
		FormulaireG.m_StrCodeFG ="";
		FormulaireG.m_StrNomRepFG = "";

		// Annee execice par défaut = 1er janvier année courante
		FormulaireG.m_StrDateFG = "0101" + theApp.m_AnneeNomencl;

		// c'est un groupe
		FormulaireG.m_Annonceur = false; 

		// mode ajout
		FormulaireG.m_Mode = 0;

		if (FormulaireG.DoModal() == IDOK)
		{	
			// Avant tout on vérifie possibilté de créér le répertoire
			RepCourantCreation = RepCourantCreation + "\\" + FormulaireG.m_StrNomRepFG;

			// Ajout Répertoire branche si branche correctement crée
			if (AjoutRepNvlleBranche(RepCourantCreation)==true)
			{
				// Ajout de la branche groupe sur arbo 
				HNvItem = m_TreeNomenclature.AjouterBranche(m_HItemCur,FormulaireG.m_StrAliasFG,FormulaireG.m_StrNomRepFG,FormulaireG.m_StrCodeFG,Txt1,Txt2,Txt3,FormulaireG.m_StrDateFG,false);	
			}
		}

	}

	else
	{
		// Les infos standards annonceur
		FormulaireStd.m_StrAliasFStd = "";
		FormulaireStd.m_StrCodeFStd ="";
		FormulaireStd.m_StrNomRepFStd = "";

		// Mode ajout annonceur
		FormulaireStd.m_Mode = 0;

		if (FormulaireStd.DoModal() == IDOK)
		{	
			// Avant tout on vérifie possibilté de créér le répertoire
			RepCourantCreation = RepCourantCreation + "\\" + FormulaireStd.m_StrNomRepFStd;

			// Ajout Répertoire branche si branche correctement crée
			if (AjoutRepNvlleBranche(RepCourantCreation)==true)
			{

				// Recup les infos EDI saisies
				pNoeud->m_TypeTiers			  = FormulaireStd.m_TypeTiers;
				pNoeud->m_CodeTiers			  = FormulaireStd.m_CodeTiers;			
				pNoeud->m_TypeCodeTiers		  = FormulaireStd.m_TypeCodeTiers;		
				pNoeud->m_NomTiers			  = FormulaireStd.m_NomTiers;				
				pNoeud->m_AdresseTiers1		  = FormulaireStd.m_AdresseTiers1;		
				pNoeud->m_AdresseTiers2		  = FormulaireStd.m_AdresseTiers2;		
				pNoeud->m_CodePostalTiers	  = FormulaireStd.m_CodePostalTiers;		
				pNoeud->m_VilleTiers		  = FormulaireStd.m_VilleTiers;			
				pNoeud->m_CodePaysTiers		  = FormulaireStd.m_CodePaysTiers;		

				// Interlocuteur(s) tiers
				pNoeud->m_NomContactTiers	  = FormulaireStd.m_NomContactTiers;		
				pNoeud->m_PrenomContactTiers  = FormulaireStd.m_PrenomContactTiers;	
				pNoeud->m_AdrMailContactTiers = FormulaireStd.m_AdrMailContactTiers;	
				
				pNoeud->m_NoTelTiers		  = FormulaireStd.m_NoTelTiers;			
				pNoeud->m_NoFaxTiers		  = FormulaireStd.m_NoFaxTiers;		  	
				
				// Ajout de la branche sur arbo 
				//HNvItem = m_TreeNomenclature.AjouterBranche(m_HItemCur,FormulaireStd.m_StrAliasFStd,FormulaireStd.m_StrNomRepFStd,FormulaireStd.m_StrCodeFStd,Txt1,Txt4,Txt5,Txt6,false);	
				HNvItem = m_TreeNomenclature.AjouterBrancheAnnonceur(m_HItemCur,FormulaireStd.m_StrAliasFStd,FormulaireStd.m_StrNomRepFStd,FormulaireStd.m_StrCodeFStd,Txt1,Txt4,Txt5,Txt6,
																	 FormulaireStd.m_TypeTiers,FormulaireStd.m_CodeTiers,FormulaireStd.m_TypeCodeTiers,
																	 FormulaireStd.m_NomTiers,FormulaireStd.m_AdresseTiers1,FormulaireStd.m_AdresseTiers2,
																	 FormulaireStd.m_CodePostalTiers,FormulaireStd.m_VilleTiers,FormulaireStd.m_CodePaysTiers,
																	 FormulaireStd.m_NoTelTiers,FormulaireStd.m_NoFaxTiers,
					  												 FormulaireStd.m_NomContactTiers,FormulaireStd.m_PrenomContactTiers,FormulaireStd.m_AdrMailContactTiers,false);
			}
		}
	}

	

	// Sauve derniere sélection menu popup
	m_InxMenuCur = Popup_Ajouter;

	// Synchronisation arborescence avec fichier arb
	m_TreeNomenclature.TrierArbre();
	RedrawWindow();
	SynchroArbre_FicArb();
	
}

void CNomenclatureView::OnMenunomenclCouperbranche() 
{
	// Couper élmt ou branche en vue d'un coller 
	if (m_HItemCur != 0)
	{
		m_BufHItemCopie = m_HItemCur;
	}
	
	m_InxMenuCur = Popup_Couper;

}

void CNomenclatureView::OnMenunomenclInfoproduit() 
{

	int CodeDessin;
	CString DateExerciceGrp;


	// Affichage panneau informations produit
	CNoeud *pNoeud;


	if (m_HItemCur != NULL)
	{
		m_TreeNomenclature.GetItemImage(m_HItemCur,CodeDessin,CodeDessin);
	    pNoeud = (CNoeud*)m_TreeNomenclature.GetItemData(m_HItemCur);		
		if (CodeDessin == CTreeCtrlX::ImageProduit)
		{
			// affichage de la marque du produit, de la famille et du secteur
			MessageBox( "Ce produit est lié à la marque  : " + pNoeud->m_Marque + "\n"
						"                   à la famille : " + pNoeud->m_Famille + "\n"
						"                   au secteur   : " + pNoeud->m_Secteur, "Information", MB_OK|MB_ICONINFORMATION );
		}
		else if (CodeDessin == CTreeCtrlX::ImageGroupe)
		{
			// affichage de la date exercice de ce groupe
			DateExerciceGrp = pNoeud->m_DateExercice.Left(2) + "/" + pNoeud->m_DateExercice.Mid(2,2) + "/" + theApp.m_AnneeNomencl; 
			MessageBox( "La date début d'exercice du groupe  " + pNoeud->m_Alias + "\n est le " + DateExerciceGrp, "Information", MB_OK|MB_ICONINFORMATION );
		}
	}

}

// Synchronisation Arborescence et Fichiers Arb
bool CNomenclatureView:: SynchroArbre_FicArb()
{
	CPWFile File;
	CPWFile FileOrg;
	CNoeud *pNoeud;
	CString Ligne;
	int NbMarque;
	int NbGroupe,NbAnnonceur,NbProduit;
	int NbFamille,NbSecteur;
	int i,j,k;
	int CodeIm;
	CString PathFichier;
	bool GrpMuetOk = false;

	bool const Maj = true;
	
	// Ajout en mémoire de la branche Marque
	if( File.Open(theApp.m_PathNomencl + theApp.m_AnneeNomencl+"\\marque.arx",CFile::modeCreate | CFile::modeWrite))
	{
		// Balayage de l'arbre marque et ajout marque dans fichie
		if (FileOrg.OpenFile(theApp.m_PathNomencl + theApp.m_AnneeNomencl    + "\\marque.arb"))
		{
			// recup les 2 premieres lignes
			for (i= 0;i<2;i++)
			{
				FileOrg.ReadString(&Ligne);
				Ligne = Ligne + "\n";
				File.WriteString(Ligne);
			}
			FileOrg.CloseFile();
		}

		else

		{
			// fichier marque.arb inexistant
			Ligne = "// Fichier des marques\n";
			File.WriteString(Ligne);
			Ligne = "MARQUE\n";
			File.WriteString(Ligne);
		}
		

		// balayage en mémoire de l'arbre noeud/marque
		pNoeud = m_ArbreEnMemoire.m_pNoeud[0];
		NbMarque = pNoeud->m_pNoeud.GetSize();
		Ligne.Format("%s%d\n","#",NbMarque);
		File.WriteString(Ligne);
		for (i=0;i<NbMarque;i++)
		{
			Ligne = pNoeud->m_pNoeud[i]->m_Alias + "\n";
			File.WriteString(Ligne);
		}

		File.WriteString("#0\n");			

		// Sauvegarde du nouveau fichier				
		File.CloseFile();

		if (Maj == true)
		{
			File.Rename(theApp.m_PathNomencl + theApp.m_AnneeNomencl    + "\\marque.arb",theApp.m_PathNomencl +theApp.m_AnneeNomencl    + "\\marque.ary");
			File.Rename(theApp.m_PathNomencl + theApp.m_AnneeNomencl    + "\\marque.arx",theApp.m_PathNomencl + theApp.m_AnneeNomencl    +  "\\marque.arb");
			File.Remove(theApp.m_PathNomencl + theApp.m_AnneeNomencl    + "\\marque.ary");
		}
	}


	// Ajout en mémoire de la branche Produit
	if (File.Open(theApp.m_PathNomencl + theApp.m_AnneeNomencl    + "\\produit.arx",CFile::modeCreate | CFile::modeWrite))
	{

		// Balayage de l'arbre marque et ajout marque dans fichie
		if (FileOrg.OpenFile(theApp.m_PathNomencl + theApp.m_AnneeNomencl    + "\\produit.arb"))
		{
			// recup les 2 premieres lignes
			for (i= 0;i<2;i++)
			{
				FileOrg.ReadString(&Ligne);
				Ligne = Ligne + "\n";
				File.WriteString(Ligne);
			}
			FileOrg.CloseFile();
		}

		// balayage en mémoire de l'arbre noeud/produit (sans les Annonceurs Specifs sans groupe)
		pNoeud = m_ArbreEnMemoire.m_pNoeud[1];
		NbGroupe = pNoeud->m_pNoeud.GetSize();
		Ligne.Format("%s%d\n","#",NbGroupe);
		File.WriteString(Ligne);

		for (i=0;i<NbGroupe;i++)
		{
			// PARAGRAPHE GROUPE (sans les annonceurs sans groupe)
			CodeIm = pNoeud->m_pNoeud[i]->m_CodeImage / 100;

			// libelle du groupe
			Ligne = pNoeud->m_pNoeud[i]->m_Libelle + "\n";
			if (Ligne =="") Ligne ="xxx";
			File.WriteString(Ligne);

			// alias du groupe
			Ligne = pNoeud->m_pNoeud[i]->m_Alias + "\n";
			if (Ligne =="") Ligne ="xxx";
			File.WriteString(Ligne);

			// code du groupe
			Ligne = pNoeud->m_pNoeud[i]->m_Codif + "\n";
			if (Ligne =="") Ligne ="xxx";
			File.WriteString(Ligne);

			// date exercice du groupe
			Ligne = pNoeud->m_pNoeud[i]->m_DateExercice + "\n";
			if (Ligne =="") Ligne ="xxx";
			File.WriteString(Ligne);

			// Type Branche Groupe
			int CodeIm = pNoeud->m_pNoeud[i]->m_CodeImage / 100;
			Ligne.Format("%d\n", CodeIm);
			File.WriteString(Ligne);	

			// 2 lignes suppl.
			Ligne = "xxx\n";
			File.WriteString(Ligne);	
			File.WriteString(Ligne);	

			// PARAGRAPHE ANNONCEUR
			CString Libel = pNoeud->m_Libelle;

			NbAnnonceur = pNoeud->m_pNoeud[i]->m_pNoeud.GetSize();
			Ligne.Format("%s%d\n","#",NbAnnonceur);
			File.WriteString(Ligne);
			for (j= 0;j<NbAnnonceur;j++)
			{
				// libelle de l'annonceur
				Ligne = pNoeud->m_pNoeud[i]->m_pNoeud[j]->m_Libelle + "\n";
				if (Ligne =="") Ligne ="xxx";
				File.WriteString(Ligne);

				// alias de l'annonceur
				Ligne = pNoeud->m_pNoeud[i]->m_pNoeud[j]->m_Alias + "\n";
				if (Ligne =="") Ligne ="xxx";
				File.WriteString(Ligne);

				// code de l'annonceur
				Ligne = pNoeud->m_pNoeud[i]->m_pNoeud[j]->m_Codif + "\n";
				if (Ligne =="") Ligne ="xxx";
				File.WriteString(Ligne);

				// A partir de Juin 2000 (+ date exercice + famille + secteur)
				// Date Exercice
				Ligne = pNoeud->m_pNoeud[i]->m_pNoeud[j]->m_DateExercice + "\n";
				if (Ligne =="") Ligne ="xxx";
				File.WriteString(Ligne);

				// Famille Annonceur
				Ligne = pNoeud->m_pNoeud[i]->m_pNoeud[j]->m_Famille + "\n";
				if (Ligne =="" || Ligne == " ") Ligne ="xxx";
				File.WriteString(Ligne);

				// Secteur Annonceur
				Ligne = pNoeud->m_pNoeud[i]->m_pNoeud[j]->m_Secteur + "\n";
				if (Ligne =="" || Ligne == " ") Ligne ="xxx";
				File.WriteString(Ligne);

				// Type Branche Annonceur
				int CodeIm = pNoeud->m_pNoeud[i]->m_pNoeud[j]->m_CodeImage / 100;
				Ligne.Format("%d\n", CodeIm);
				File.WriteString(Ligne);	

				// 2 lignes suppl.
				Ligne = "xxx\n";
				File.WriteString(Ligne);	
				File.WriteString(Ligne);	

				// NEW FORMAT EDI !!!! Lecture des 14 lignes supplémentaires
				// Type tiers toujours BY pour annonceur
				Ligne = pNoeud->m_pNoeud[i]->m_pNoeud[j]->m_TypeTiers + "\n";
				if (Ligne =="" || Ligne == "\n") Ligne ="xxx\n";
				File.WriteString(Ligne);	

				// Code Tiers
				Ligne = pNoeud->m_pNoeud[i]->m_pNoeud[j]->m_CodeTiers + "\n";
				if (Ligne =="" || Ligne == "\n") Ligne ="xxx\n";
				File.WriteString(Ligne);	
				
				// Type code tiers
				Ligne = pNoeud->m_pNoeud[i]->m_pNoeud[j]->m_TypeCodeTiers + "\n";
				if (Ligne =="" || Ligne == "\n") Ligne ="xxx\n";
				File.WriteString(Ligne);	

				// Nom annonceur (redondant !!! mais bon)
				Ligne = pNoeud->m_pNoeud[i]->m_pNoeud[j]->m_NomTiers + "\n";
				if (Ligne =="" || Ligne == "\n") Ligne ="xxx\n";
				File.WriteString(Ligne);	

				// Adresse 1
				Ligne = pNoeud->m_pNoeud[i]->m_pNoeud[j]->m_AdresseTiers1 + "\n";
				if (Ligne =="" || Ligne == "\n") Ligne ="xxx\n";
				File.WriteString(Ligne);	
	
				// Adresse 2
				Ligne = pNoeud->m_pNoeud[i]->m_pNoeud[j]->m_AdresseTiers2 + "\n";
				if (Ligne =="" || Ligne == "\n") Ligne ="xxx\n";
				File.WriteString(Ligne);

				// Code Postal
				Ligne = pNoeud->m_pNoeud[i]->m_pNoeud[j]->m_CodePostalTiers + "\n";
				if (Ligne =="" || Ligne == "\n") Ligne ="xxx\n";
				File.WriteString(Ligne);

				// Ville
				Ligne = pNoeud->m_pNoeud[i]->m_pNoeud[j]->m_VilleTiers + "\n";
				if (Ligne =="" || Ligne == "\n") Ligne ="xxx\n";
				File.WriteString(Ligne);
				
				// Code pays
				Ligne = pNoeud->m_pNoeud[i]->m_pNoeud[j]->m_CodePaysTiers + "\n";
				if (Ligne =="" || Ligne == "\n") Ligne ="xxx\n";
				File.WriteString(Ligne);					

				// No tel
				Ligne = pNoeud->m_pNoeud[i]->m_pNoeud[j]->m_NoTelTiers + "\n";
				if (Ligne =="" || Ligne == "\n") Ligne ="xxx\n";
				File.WriteString(Ligne);					

				// No fax
				Ligne = pNoeud->m_pNoeud[i]->m_pNoeud[j]->m_NoFaxTiers + "\n";
				if (Ligne =="" || Ligne == "\n") Ligne ="xxx\n";
				File.WriteString(Ligne);

				// Nom Interlocuteur tiers
				Ligne = pNoeud->m_pNoeud[i]->m_pNoeud[j]->m_NomContactTiers + "\n";
				if (Ligne =="" || Ligne == "\n") Ligne ="xxx\n";
				File.WriteString(Ligne);

				// Prénom Interlocuteur tiers
				Ligne = pNoeud->m_pNoeud[i]->m_pNoeud[j]->m_PrenomContactTiers + "\n";
				if (Ligne =="" || Ligne == "\n") Ligne ="xxx\n";
				File.WriteString(Ligne);

				// Adr Mail Interlocuteur tiers
				Ligne = pNoeud->m_pNoeud[i]->m_pNoeud[j]->m_AdrMailContactTiers + "\n";
				if (Ligne =="" || Ligne == "\n") Ligne ="xxx\n";
				File.WriteString(Ligne);

				// PARAGRAPHE PRODUIT
				NbProduit = pNoeud->m_pNoeud[i]->m_pNoeud[j]->m_pNoeud.GetSize();
				Ligne.Format("%s%d\n","#",NbProduit);
				File.WriteString(Ligne);
				for (k= 0;k<NbProduit;k++)
				{
					// libelle du produit
					Ligne = pNoeud->m_pNoeud[i]->m_pNoeud[j]->m_pNoeud[k]->m_Libelle + "\n";
					if (Ligne =="") Ligne ="xxx";
					File.WriteString(Ligne);

					// alias du produit
					Ligne = pNoeud->m_pNoeud[i]->m_pNoeud[j]->m_pNoeud[k]->m_Alias + "\n";
					if (Ligne =="") Ligne ="xxx";
					File.WriteString(Ligne);

					// code du produit
					Ligne = pNoeud->m_pNoeud[i]->m_pNoeud[j]->m_pNoeud[k]->m_Codif + "\n";
					if (Ligne =="") Ligne ="xxx";
					File.WriteString(Ligne);

					// marque du produit
					Ligne = pNoeud->m_pNoeud[i]->m_pNoeud[j]->m_pNoeud[k]->m_Marque + "\n";
					if (Ligne =="") Ligne ="xxx";
					File.WriteString(Ligne);

					// famille et secteur du produit
					Ligne = pNoeud->m_pNoeud[i]->m_pNoeud[j]->m_pNoeud[k]->m_Famille + "\n";
					if (Ligne =="") Ligne ="xxx";
					File.WriteString(Ligne);
					Ligne = pNoeud->m_pNoeud[i]->m_pNoeud[j]->m_pNoeud[k]->m_Secteur + "\n";
					if (Ligne =="") Ligne ="xxx";
					File.WriteString(Ligne);

					// Type Branche Produit
					int CodeIm = pNoeud->m_pNoeud[i]->m_pNoeud[j]->m_pNoeud[k]->m_CodeImage / 100;
					Ligne.Format("%d\n",CodeIm);
					File.WriteString(Ligne);	

					// 2 lignes suppl.
					Ligne = "xxx\n";
					File.WriteString(Ligne);	
					File.WriteString(Ligne);	

					// fin produit
					File.WriteString("#0\n");			
				}
			}
		}

		File.WriteString("#0\n");			

		// Sauvegarde du nouveau fichier		
		File.CloseFile();

		if (Maj == true)
		{
			File.Rename(theApp.m_PathNomencl + theApp.m_AnneeNomencl    + "\\produit.arb",theApp.m_PathNomencl + theApp.m_AnneeNomencl    +  "\\produit.ary");
			File.Rename(theApp.m_PathNomencl + theApp.m_AnneeNomencl    + "\\produit.arx",theApp.m_PathNomencl + theApp.m_AnneeNomencl    + "\\produit.arb");
			File.Remove(theApp.m_PathNomencl + theApp.m_AnneeNomencl    + "\\produit.ary");
		}

	}


	// Ajout en mémoire de la branche Famille/Secteur
	//if( File.Open(theApp.m_PathNomencl + theApp.m_AnneeNomencl+"\\secteur.arx",CFile::modeCreate | CFile::modeWrite))
	if( File.Open(theApp.m_PathNomencl +"\\secteur.arx",CFile::modeCreate | CFile::modeWrite))
	{
		// Balayage de l'arbre famille/secteur et ajout famille ou secteur dans fichie
		//if (FileOrg.OpenFile(theApp.m_PathNomencl + theApp.m_AnneeNomencl    + "\\secteur.arb"))
		if (FileOrg.OpenFile(theApp.m_PathNomencl +  "\\secteur.arb"))
		{
			// recup les 2 premieres lignes
			for (int i= 0;i<2;i++)
			{
				FileOrg.ReadString(&Ligne);
				Ligne = Ligne + "\n";
				File.WriteString(Ligne);
			}
			FileOrg.CloseFile();
		}

		else
		{
			// fichier secteur.arb inexistant
			Ligne = "// Fichier des familles/secteurs\n";
			File.WriteString(Ligne);
			Ligne = "#SECTEUR\n";
			File.WriteString(Ligne);
		}

		// balayage en mémoire de l'arbre noeud/famille-secteur
		pNoeud = m_ArbreEnMemoire.m_pNoeud[2];
		NbFamille = pNoeud->m_pNoeud.GetSize();
		Ligne.Format("%s%d\n","#",NbFamille);
		File.WriteString(Ligne);
		// pour test NbFamille = 0;
		if (NbFamille > 0)
		{	
			// Ecriture des libelles familles
			for (int i=0;i<NbFamille;i++)
			{
				Ligne = pNoeud->m_pNoeud[i]->m_Alias + "\n";
				File.WriteString(Ligne);

				NbSecteur = pNoeud->m_pNoeud[i]->m_pNoeud.GetSize();
				Ligne.Format("%s%d\n","#",NbSecteur);
				File.WriteString(Ligne);

				if (NbSecteur > 0)
				{
					// Ecriture des libelles secteurs pour cette famille
					for (int j=0;j<NbSecteur;j++)
					{
						Ligne = pNoeud->m_pNoeud[i]->m_pNoeud[j]->m_Alias + "\n";
						File.WriteString(Ligne);

					}
					// fin secteur
					File.WriteString("#0\n");			
				}
			}
		}

		File.WriteString("#0\n");			

		// Sauvegarde du nouveau fichier				
		File.CloseFile();

		if (Maj == true)
		{
			/*
			File.Rename(theApp.m_PathNomencl + theApp.m_AnneeNomencl    + "\\secteur.arb",theApp.m_PathNomencl +theApp.m_AnneeNomencl    + "\\secteur.ary");
			File.Rename(theApp.m_PathNomencl + theApp.m_AnneeNomencl    + "\\secteur.arx",theApp.m_PathNomencl + theApp.m_AnneeNomencl    +  "\\secteur.arb");
			File.Remove(theApp.m_PathNomencl + theApp.m_AnneeNomencl    + "\\secteur.ary");
			*/
			File.Rename(theApp.m_PathNomencl +  "\\secteur.arb",theApp.m_PathNomencl + "\\secteur.ary");
			File.Rename(theApp.m_PathNomencl +  "\\secteur.arx",theApp.m_PathNomencl + "\\secteur.arb");
			File.Remove(theApp.m_PathNomencl +  "\\secteur.ary");
		}
	}

	return true;

}


// Gestion des répertoires -- Ajout Nouveau Repertoire Branche
bool CNomenclatureView::AjoutRepNvlleBranche(CString RepACreer)
{
	CString Buf;

	if (CreateDirectory(RepACreer,NULL) == false)
	{
		// Interdition de certains caractères pour la création répertoire
		if (RepACreer.Find('\\') != -1 ||
			RepACreer.Find('/') != -1  ||
			RepACreer.Find(':') != -1  ||
			RepACreer.Find('*') != -1  ||
			RepACreer.Find('?') != -1  ||
			RepACreer.Find('!') != -1  ||
			RepACreer.Find('"') != -1  ||
			RepACreer.Find('<') != -1  ||
			RepACreer.Find('>') != -1  ||
			RepACreer.Find('|') != -1) 

			Buf = "Problème certains caractères (\\,/,:,*,?,!,'""',<,>,|) nom répertoire non autorisés ";
		else
			Buf = "Problème création répertoire déjà existant " + RepACreer;

		AfxMessageBox (Buf);
		return false;
	}

	else 
		return true;


}

bool CNomenclatureView::RenommerRepBranche(CString NvRepBranche,CString ExRepBranche)
{
	
	CString Buf;

	if (rename(ExRepBranche,NvRepBranche) != 0)
	{
		Buf = "Impossible de renommer le répertoire " + ExRepBranche
			  + "\n" + "en répertoire " + NvRepBranche ;
		AfxMessageBox (Buf);
		return false;
	}
		
	else 
		return true;

 
}

// Suppression d'un répertoire associé à une arborescence
bool CNomenclatureView::SuppressionRepertoire(CNoeud *pNoeud)
{
	int NbSsNoeud;

	// Recup noeud associé
	if (pNoeud != NULL)
	{
		NbSsNoeud = pNoeud->m_pNoeud.GetSize();

		if (NbSsNoeud > 0)
		{
			// Suppression des ss/répertoire lié au ss-noeud
			for (int i= 0;i<NbSsNoeud;i++)
			{
				SuppressionRepertoire(pNoeud->m_pNoeud[i]);						
			}
			// supression branche mere
			SuppressionRepertoire(pNoeud);
		}
		else
		{
			if (pNoeud != NULL)
			{
				// Suppression d'un répertoire pointé par Branche HItem	
				RemoveDirectory(pNoeud->m_PathFichier);
				return true;
			}
			else return false;
		}
	}		
	else return false;
	return true;
	
}

// Test les répertoires pour savoir s'il est possible de copier
bool CNomenclatureView::CopieRepItemPossible(HTREEITEM HItemCopie,HTREEITEM HItemParent)
{
	CNoeud *pNoeudParent;
	CNoeud *pNoeudCopie;
	bool RepIdemTrouve = true;

	pNoeudParent = (CNoeud*)m_TreeNomenclature.GetItemData(HItemParent);		
	pNoeudCopie = (CNoeud*)m_TreeNomenclature.GetItemData(HItemCopie);		

	for (int i= 0;i <pNoeudParent->m_pNoeud.GetSize();i++)
	{
		if (pNoeudParent->m_pNoeud[i]->m_Libelle == pNoeudCopie->m_Libelle)
		{
			// ici meme nom ss-rep trouvé >> copie impossible
			RepIdemTrouve = false;
			break;
		}
	}

	return RepIdemTrouve;
}

// Test si copie fichier possible
bool CNomenclatureView::CopieFicItemPossible(HTREEITEM HItemCopie,HTREEITEM HItemParent,CString FicCopie)
{
	bool FicIdemTrouve = false;
	CString FicExistant;
	HTREEITEM hItem;


	if (m_TreeNomenclature.ItemHasChildren(HItemParent))
	{
		// Nom du fichier à copier
		if (FicCopie == "")
		{
			FicCopie = m_TreeNomenclature.GetItemText(HItemCopie);		
			FicCopie.MakeUpper();
		}

		// enfants sur cette branche
		hItem = m_TreeNomenclature.GetChildItem(HItemParent);
		
		while (hItem != NULL && FicIdemTrouve == false)
		{
			
			FicExistant = m_TreeNomenclature.GetItemText(hItem);		
			FicExistant.MakeUpper();
			if (FicExistant == FicCopie)
			{
				// fichier déjà existant à ce niveau d'arborescence > copie impossible
				FicIdemTrouve = true;
				break;
			}
			hItem = m_TreeNomenclature.GetNextItem(hItem,TVGN_NEXT);
		}

	}

	return !FicIdemTrouve;

}

// Suppression d'un fichier (campagne par exemple) associé à une feuille
bool CNomenclatureView::SuppressionFichier(HTREEITEM HItem,CString ExtensionFic)
{
	CNoeud *pNoeudParent;
	HTREEITEM HItemParent;
	CString PathFichier;

	
	if (HItem != NULL)
	{
		HItemParent = m_TreeNomenclature.GetParentItem(HItem);
		if (HItemParent != NULL)
		{
			// Recup noeud parent de cette feuille / fichier
			pNoeudParent = (CNoeud*)m_TreeNomenclature.GetItemData(HItemParent);		

			// Suppression d'un fichier par feuille HItem	
			PathFichier = pNoeudParent->m_PathFichier + "\\" +m_TreeNomenclature.GetItemText(HItem) + "." + ExtensionFic;
			remove(PathFichier);
			return true;
		}

		else return false;

	}

	else
		return false;

	
}





bool CNomenclatureView:: SsRepExist(CNoeud *pNoeudParent,CString NomSsRep)
{
	int NbNoeud;
	bool SsRepTrouve = false;
	CString Txt1,Txt2;

	NbNoeud = pNoeudParent->m_pNoeud.GetSize();
	for (int i=0;i<NbNoeud;i++)
	{
		Txt1 = pNoeudParent->m_pNoeud[i]->m_Libelle;
		Txt1.MakeUpper();
		Txt2 = NomSsRep;
		Txt2.MakeUpper();

		if (Txt1 == Txt2) 
		{
			SsRepTrouve = true;
			break;
		}
	}

	return SsRepTrouve;


}

// Mise à jour des path répertoires des noeuds inférieures
void CNomenclatureView :: MajPathNoeudNiveauInf(CNoeud *pNoeud)
{
	if (pNoeud->m_pNoeud.GetSize() > 0)
	{
		// on a des noeuds fils à mettre à jour
		for (int i = 0;i<pNoeud->m_pNoeud.GetSize();i++)
		{
			pNoeud->m_pNoeud[i]->m_PathFichier = pNoeud->m_PathFichier + "\\" + pNoeud->m_pNoeud[i]->m_Libelle;
			MajPathNoeudNiveauInf(pNoeud->m_pNoeud[i]);
		}
	}
}


/////////////////////////////////////////////////////////////////////////////
// CNomenclatureView diagnostics

#ifdef _DEBUG
void CNomenclatureView::AssertValid() const
{
	CFormView::AssertValid();
}

void CNomenclatureView::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}

CNomenclatureDoc* CNomenclatureView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CNomenclatureDoc)));
	return (CNomenclatureDoc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CNomenclatureView message handlers

void CNomenclatureView::OnRclickTree1(NMHDR* pNMHDR, LRESULT* pResult) 
{
	TV_HITTESTINFO HitTest;
	CString TxtBranche,NvTxtBranche;
	POINT Point;
	MENUITEMINFO ItemInfo;
	HTREEITEM HItemParent;
	HTREEITEM HItemGrandParent;
	char Text [41];
	int NbItem;
	int	CodeDessin = 0;
	int CodeDessinEnfant= 0;
	int CodeDessinParent= 0;
	int CodeDessinGrandParent = 0;
	int CodeDessinAColler = 0;
	unsigned int IdItemMenu;
	bool ItemMenuValid;
	CNoeud *pNoeud = NULL;
	CNoeud *pNoeudMere = NULL;
	CNoeud *pNoeudGrandMere = NULL;
	CString StrElmt,StrElmtCoupe,TxtItem,TextItemParent;
	bool AffichPopPup = true;
	bool AnnonceurAvecProduit = true;

	// A VIRER par la suite
	bool FlagOnlyAjout = true;


	if (theApp.m_NiveauAcces >= 2)
	{

		// Création et chargement du menu Popup (pour gérer l'arborescence)
		CMenu Menu;
		Menu.LoadMenu(IDR_MENUPOPUP);

		// Positionnement du menu
		GetCursorPos(&Point);

		// Selon branche sélectionnée, on modifie texte du menu contextuel
		ItemInfo.cbSize = sizeof(ItemInfo);
		ItemInfo.cch = sizeof(Text);
		ItemInfo.fMask=MIIM_TYPE;
		ItemInfo.dwTypeData = Text; 

		// Branche branche sélectionnée
		GetCursorPos(&HitTest.pt);
		m_TreeNomenclature.ScreenToClient(&HitTest.pt);
		m_HItemCur = m_TreeNomenclature.HitTest(&HitTest);
		if (m_HItemCur == NULL) return;
		
		// Texte associé à la branche courante
		TxtItem = m_TreeNomenclature.GetItemText(m_HItemCur);
		if (TxtItem.GetLength() > 0)
		{
			TxtItem.TrimLeft();
			TxtItem.TrimRight();
		}

		// Va au menu Popup
		CMenu *MenuPopup;
		MenuPopup = Menu.GetSubMenu(0);

		// Nb item du menu Popup
		NbItem = MenuPopup->GetMenuItemCount(); 

		// recup code dessin de la branche
		m_TreeNomenclature.GetItemImage(m_HItemCur,CodeDessin,CodeDessin);

		// recup
		CodeDessinParent = 0;
		CodeDessinEnfant = 0;

		// Recup code image du dernier couper (test si campagne)
		if (m_BufHItemCopie != NULL)
			m_TreeNomenclature.GetItemImage(m_BufHItemCopie,CodeDessinAColler,CodeDessinAColler);

		// recup code dessin enfant de la branche à suivre
		if (m_TreeNomenclature.GetItemData(m_HItemCur) != 0)
		{
			// recup code dessin enfant de la branche à suivre
			pNoeud = (CNoeud*)m_TreeNomenclature.GetItemData(m_HItemCur);		

			// Maj flag info annonceur avec produit
			if (pNoeud->m_Famille != "xxx" && pNoeud->m_Famille != "" &&
				CodeDessin == CTreeCtrlX::ImageAnnonceur)
				AnnonceurAvecProduit = false;

			CodeDessinEnfant = pNoeud->m_CodeImage % 100;

			// recup code dessin de la branche parent
			HItemParent = m_TreeNomenclature.GetParentItem(m_HItemCur);
			TextItemParent = m_TreeNomenclature.GetItemText(HItemParent);
			TextItemParent.TrimLeft();
			TextItemParent.TrimRight();
			if (HItemParent != NULL)
			{
				pNoeudMere = (CNoeud*)m_TreeNomenclature.GetItemData(HItemParent);
				if (pNoeudMere != NULL) CodeDessinParent = pNoeudMere->m_CodeImage / 100;
			}
			else
				// ce sont les 2 branches racines
				CodeDessinParent = 0;

		}

		else
		{
			// recup code dessin de la branche parent
			HItemParent = m_TreeNomenclature.GetParentItem(m_HItemCur);
			if (HItemParent != NULL)
			{
				pNoeudMere = (CNoeud*)m_TreeNomenclature.GetItemData(HItemParent);
				if (pNoeudMere != NULL) CodeDessinParent = pNoeudMere->m_CodeImage / 100;
				HItemGrandParent= m_TreeNomenclature.GetParentItem(HItemParent);
				if (HItemGrandParent != NULL)
				{
					pNoeudGrandMere = (CNoeud*)m_TreeNomenclature.GetItemData(HItemGrandParent);
					if (pNoeudGrandMere != NULL) CodeDessinGrandParent = pNoeudGrandMere->m_CodeImage / 100;
				}

			}
		}


		// Texte de l'élément courant (Marque/Groupe/Annonceur/Produit/Campagne...)
		StrElmt ="";
		switch (CodeDessin)
		{
			case CTreeCtrlX::ImageMarque: 
			{
				StrElmt = "marque";
				break;
			}
			case CTreeCtrlX::ImageGroupe: 
			{
				StrElmt = "groupe";
				break;
			}
			case CTreeCtrlX::ImageAnnonceur: 
			{
				StrElmt = "annonceur";
				break;
			}
			case CTreeCtrlX::ImageProduit: 
			{
				StrElmt = "produit";
				break;
			}
			case CTreeCtrlX::ImageCampagne: 
			{
				StrElmt = "campagne";
				break;
			}
			case CTreeCtrlX::ImageFamille: 
			{
				StrElmt = "famille";
				break;
			}
			case CTreeCtrlX::ImageSecteur: 
			{
				StrElmt = "secteur";
				break;
			}
		}

		// ni menu popup pour produit dans arbo marque
		// ni pour campagne dans marque/produit
		if (CodeDessin == CTreeCtrlX::ImageProduit && (CodeDessinParent == CTreeCtrlX::ImageMarque || CodeDessinParent == CTreeCtrlX::ImageSecteur))
			AffichPopPup = false;
		if (CodeDessin == CTreeCtrlX::ImageCampagne && (CodeDessinGrandParent == CTreeCtrlX::ImageSecteur))
			AffichPopPup = false;


		if  (AffichPopPup == true)
		{
			// Change texte menu selon la branche ou elmt courant
			for (int i = 0;i<NbItem;i++)
			{
				NvTxtBranche = "";
				ItemMenuValid = true;
				switch (i)
				{
					case Popup_Ajouter :  // Ajouter branche ssi <> branche campagne
					{
						if (theApp.m_NiveauAcces < 2)
						{
							// pas de droit en dehors de l'administrateur acces >= 2
							ItemMenuValid = false;
						}
						else
						{
							switch (CodeDessinEnfant)
							{
								case CTreeCtrlX::ImageMarque :
								{
									NvTxtBranche = m_TMenu[i] + " marque";
									break;
								}
								case CTreeCtrlX::ImageGroupe :
								{
									NvTxtBranche = m_TMenu[i] + " groupe";
									break;
								}	
								case CTreeCtrlX::ImageAnnonceur :
								{
									
									NvTxtBranche = m_TMenu[i] + " annonceur";
									if (TxtItem == "...")  //"Annonceur Direct")
									{
										// si annonceur sans groupe
										ItemMenuValid = false;
									}
									break;
								}	
								case CTreeCtrlX::ImageFamille :
								{
									NvTxtBranche = m_TMenu[i] + " famille";
									break;
								}	
								case CTreeCtrlX::ImageSecteur :
								{
									NvTxtBranche = m_TMenu[i] + " secteur";
									break;
								}	
								case CTreeCtrlX::ImageProduit :
								{
									if (
									   // Annonceur avec groupe et sans définition famille/secteur donc produit
									   (CodeDessin == CTreeCtrlX::ImageAnnonceur && CodeDessinParent == CTreeCtrlX::ImageGroupe && AnnonceurAvecProduit == true) ||
									   // Annonceur sans groupe et sans définition famille/secteur donc produit
									   (CodeDessin == CTreeCtrlX::ImageAnnonceur && CodeDessinParent != CTreeCtrlX::ImageGroupe && AnnonceurAvecProduit == true)
									   )	
									{
										NvTxtBranche = m_TMenu[i] + " produit";
									}
									else
									{
										// pas d'ajout possible produit au niveau marque ni au niveau secteur
										NvTxtBranche = m_TMenu[i];
										ItemMenuValid = false;
									}
									break;
								}	
								case CTreeCtrlX::ImageCampagne :
								case CTreeCtrlX:: NoImage :
								{
									NvTxtBranche = m_TMenu[i];
									ItemMenuValid = false;
									break;
								}	

							}
							break;

						}
					}

					case Popup_AjouterAnnSsGrp :  // Ajouter branche annonceur sans groupe
					{
						if (CodeDessinEnfant == CTreeCtrlX::ImageGroupe && theApp.m_NiveauAcces >= 2 ||
							TxtItem == "..." && theApp.m_NiveauAcces >= 2)
						{
							NvTxtBranche = m_TMenu[i];

							// A VIRER quand Annonceur sans groupe Ok
							//ItemMenuValid = false;
						}
						else
						{
							NvTxtBranche = "";
							ItemMenuValid = false;	
						}
						break;
					}

					case Popup_AjouterAnnSsGrpSsProd :  // Ajouter branche annonceur sans groupe
					{
						if (CodeDessinEnfant == CTreeCtrlX::ImageGroupe && theApp.m_NiveauAcces >= 2 ||
							TxtItem == "..." && theApp.m_NiveauAcces >= 2 )
						{
							NvTxtBranche = "Ajouter annonceur isolé";

							// A VIRER quand Annonceur sans groupe Ok
							//ItemMenuValid = false;
						}
						else if (CodeDessinEnfant == CTreeCtrlX::ImageAnnonceur && theApp.m_NiveauAcces >= 2)
						{
							NvTxtBranche = m_TMenu[i];
						}

						else if (theApp.m_NiveauAcces < 2)
						{
							NvTxtBranche = "";
							ItemMenuValid = false;	
						}
						break;
					}

					case Popup_Renommer :  // Renommer ou modifier un élmt (sauf les 2 branches racines)
					{
						if (CodeDessin == CTreeCtrlX::ImageMarque ||
							CodeDessin == CTreeCtrlX::ImageCampagne ||
							CodeDessin == CTreeCtrlX::ImageFamille ||
							CodeDessin == CTreeCtrlX::ImageSecteur)
							NvTxtBranche = 	"Renommer";
						else
							NvTxtBranche = 	m_TMenu[i];
		
						if (CodeDessin == CTreeCtrlX:: NoImage || 
							CodeDessin == CTreeCtrlX::ImageOpen ||
							CodeDessin == CTreeCtrlX::ImageClose ||
							TxtItem == "..." ||
							theApp.m_NiveauAcces < 2)
							ItemMenuValid = false;
						else
							NvTxtBranche = 	NvTxtBranche + " " + StrElmt ;

						break;
					}
							   
					case Popup_Couper :  // Couper elmt ou branche (saus les 2 racines)
					{
						if(CodeDessinEnfant == CTreeCtrlX::ImageMarque ||
						   CodeDessinEnfant == CTreeCtrlX::ImageGroupe ||
						   CodeDessinEnfant == CTreeCtrlX::ImageFamille ||
						   CodeDessin == CTreeCtrlX::ImageFamille ||
						   CodeDessin == CTreeCtrlX::ImageSecteur ||
						   CodeDessin == CTreeCtrlX::ImageMarque ||
						   CodeDessin == CTreeCtrlX::ImageGroupe ||
						   // cas d'un annonceur direct (sans groupe, sans produit)
						   (CodeDessin == CTreeCtrlX::ImageAnnonceur && CodeDessinParent != CTreeCtrlX::ImageGroupe) || 
						   (CodeDessin == CTreeCtrlX::ImageAnnonceur && TextItemParent == "...") ||
						   theApp.m_NiveauAcces < 2)
						{
							NvTxtBranche = 	m_TMenu[i];
							ItemMenuValid = false;
						}

						else
						{
							HTREEITEM HItemEnf = m_TreeNomenclature.GetChildItem(m_HItemCur);
							if (HItemEnf == NULL)
							{
								NvTxtBranche = m_TMenu[i] + " " + StrElmt; //" " + m_TypeElmt[1] + " " + TxtBranche;  
							}
							else
							{	
								NvTxtBranche = m_TMenu[i] + " " + StrElmt; //" " + m_TypeElmt[0] + " " + TxtBranche;  
							}
						}
						break;
					}

					/*
					case 2 : // affichage texte copier
					{
						if 	(CodeDessin == CTreeCtrlX:: NoImage ||
							 CodeDessin == CTreeCtrlX::ImageOpen ||
							 CodeDessin == CTreeCtrlX::ImageClose)
						{
							NvTxtBranche = 	m_TMenu[i];
							ItemMenuValid = false;
						}
						else
							NvTxtBranche = 	m_TMenu[i] + " " +  m_TypeElmt[1] + " " + TxtBranche;
						break;
					}
					*/

					case Popup_Coller : // affichage texte coller et déplacer
					{
						// Texte objet à copier
						StrElmtCoupe = "";
						if (m_BufHItemCopie != 0) StrElmtCoupe = m_TreeNomenclature.GetItemText(m_BufHItemCopie);

						if (StrElmtCoupe == "" ||
							CodeDessin == CTreeCtrlX::ImageCampagne ||
							CodeDessin == CTreeCtrlX::ImageMarque ||
							CodeDessin == CTreeCtrlX::ImageFamille ||
							CodeDessin == CTreeCtrlX::ImageSecteur ||
							// cas d'un annonceur direct (sans groupe, sans produit) uniquement coller campagne autorisé
						   (CodeDessin == CTreeCtrlX::ImageAnnonceur && CodeDessinParent != CTreeCtrlX::ImageGroupe) ||
						   (CodeDessin == CTreeCtrlX::ImageAnnonceur && TextItemParent == "...") && CodeDessinAColler != CTreeCtrlX::ImageCampagne ||
						   (AnnonceurAvecProduit == false) ||	

						   // attention seul administrateur base autorisé
							theApp.m_NiveauAcces < 2)
						{
							// cas d'un annonceur sans groupe mais avec produits possible)
							if (AnnonceurAvecProduit  && CodeDessinAColler == CTreeCtrlX::ImageProduit)
								NvTxtBranche = 	m_TMenu[i] + " " + StrElmtCoupe;	
							else
							{
								NvTxtBranche = 	m_TMenu[i];
								ItemMenuValid = false;
							}

						}

						else
						{
							NvTxtBranche = 	m_TMenu[i] + " " + StrElmtCoupe; //+ " sur " + TxtBranche;
						}

						// on test avant si possibilité de copier sur ce niveau
						if (m_BufHItemCopie != NULL)
							if (CopieItemPossible(m_BufHItemCopie,m_HItemCur,false) != true)
							{
								ItemMenuValid = false;
							}

						break;
					}

					case Popup_Supprimer : // Suppression d'un elmt
					{
						if(CodeDessinEnfant == CTreeCtrlX::ImageMarque ||
						   CodeDessinEnfant == CTreeCtrlX::ImageGroupe ||
						   CodeDessinEnfant == CTreeCtrlX::ImageFamille ||
						   // Groupe muet
						   //(CodeDessin == CTreeCtrlX::ImageGroupe && TxtItem == "...") ||
						   //TxtItem == "Annonceur Direct" ||
						   theApp.m_NiveauAcces < 2)
						{
							NvTxtBranche = 	m_TMenu[i];
							ItemMenuValid = false;
						}

						else
						{
							if (CodeDessin != CTreeCtrlX::ImageCampagne && CodeDessin != 0)
							{
								NvTxtBranche = 	m_TMenu[i] + " " + StrElmt; //m_TypeElmt[0] + " " + TxtBranche;
							}
							else
							{
								NvTxtBranche = 	m_TMenu[i] + " " +  StrElmt; m_TypeElmt[1];// + " " + TxtBranche;
							}
						}
						break;
					}

					case Popup_Info : // Affichage infos (si produit)
					{
						if (CodeDessin == CTreeCtrlX::ImageProduit ||
						   (CodeDessin == CTreeCtrlX::ImageGroupe && TxtItem != "..."))
						{
							NvTxtBranche = 	m_TMenu[i] + " " + StrElmt; //TxtBranche;
						}

						else
						{
							NvTxtBranche = 	m_TMenu[i];
							ItemMenuValid = false;
						}
						break; 
					}

					case Popup_TauxFO : // Mise à jour Taux FO
					{
						if (CodeDessinEnfant == CTreeCtrlX::ImageMarque || CodeDessinEnfant == CTreeCtrlX::ImageSecteur || (CodeDessin == CTreeCtrlX::ImageProduit && (CodeDessinParent == CTreeCtrlX::ImageMarque || CodeDessinParent == CTreeCtrlX::ImageSecteur)) || CodeDessin == CTreeCtrlX::ImageMarque)
							ItemMenuValid = false;
						else if (CodeDessin == CTreeCtrlX::ImageSecteur || CodeDessinParent == CTreeCtrlX :: ImageSecteur || CodeDessinGrandParent == CTreeCtrlX :: ImageSecteur)
							ItemMenuValid = false;
						else if (CodeDessin == CTreeCtrlX::ImageOpen || CodeDessin == CTreeCtrlX::ImageClose)
							ItemMenuValid = false;
						else if (pNoeud != NULL)
							if (pNoeud->m_Codif == "") ItemMenuValid = false;
						else
						{
							NvTxtBranche = 	m_TMenu[i];
							ItemMenuValid = true;
						}
						break;
					}

				}

				// Modif ou inhibation des lignes du menu popup
				// ??? sous NT problème MenuPopup->GetMenuItemInfo(i,&ItemInfo,TRUE);
				IdItemMenu = MenuPopup->GetMenuItemID(i);

				if (ItemMenuValid == true && NvTxtBranche != "")
				{
					MenuPopup->ModifyMenu(i,MF_BYPOSITION | MF_STRING,IdItemMenu,NvTxtBranche);
				}	
				else
				{
					MenuPopup->EnableMenuItem(IdItemMenu,MF_GRAYED);   
				}	


			}

			// Revisualise le menu popup aprés modif texte
			//Menu.GetSubMenu(0)->TrackPopupMenu(TPM_LEFTBUTTON   | TPM_LEFTALIGN,Point.x,Point.y,this,NULL);
			Menu.GetSubMenu(0)->TrackPopupMenu(TPM_RIGHTBUTTON |TPM_LEFTALIGN,Point.x,Point.y,this,NULL);
		}
	}

	else if (theApp.m_NiveauAcces >= 1)
	{
		// Création et chargement du menu Popup (pour gérer l'arborescence)
		CMenu MenuTaux;
		MenuTaux.LoadMenu(IDR_MENUPOPUP);

		// Positionnement du menu
		GetCursorPos(&Point);

		// Selon branche sélectionnée, on modifie texte du menu contextuel
		ItemInfo.cbSize = sizeof(ItemInfo);
		ItemInfo.cch = sizeof(Text);
		ItemInfo.fMask=MIIM_TYPE;
		ItemInfo.dwTypeData = Text; 

		// Branche branche sélectionnée
		GetCursorPos(&HitTest.pt);
		m_TreeNomenclature.ScreenToClient(&HitTest.pt);
		m_HItemCur = m_TreeNomenclature.HitTest(&HitTest);
		if (m_HItemCur == NULL) return;
		
		// Texte associé à la branche courante
		TxtItem = m_TreeNomenclature.GetItemText(m_HItemCur);
		TxtItem.TrimLeft();
		TxtItem.TrimRight();
		//if (TxtItem.GetLenght() > 2) TxtItem = TxtItem.Left(3);


		// Va au menu Popup
		CMenu *MenuPopupTaux;
		MenuPopupTaux = MenuTaux.GetSubMenu(0);

		// Nb item du menu Popup
		NbItem = MenuPopupTaux->GetMenuItemCount(); 

		// recup code dessin de la branche
		m_TreeNomenclature.GetItemImage(m_HItemCur,CodeDessin,CodeDessin);

		// recup
		CodeDessinParent = 0;
		CodeDessinEnfant = 0;

		// Recup code image du dernier couper (test si campagne)
		if (m_BufHItemCopie != NULL)
			m_TreeNomenclature.GetItemImage(m_BufHItemCopie,CodeDessinAColler,CodeDessinAColler);

		// recup code dessin enfant de la branche à suivre
		if (m_TreeNomenclature.GetItemData(m_HItemCur) != 0)
		{
			// recup code dessin enfant de la branche à suivre
			pNoeud = (CNoeud*)m_TreeNomenclature.GetItemData(m_HItemCur);		

			// Maj flag info annonceur avec produit
			if (pNoeud->m_Famille != "xxx" && pNoeud->m_Famille != "" &&
				CodeDessin == CTreeCtrlX::ImageAnnonceur)
				AnnonceurAvecProduit = false;

			CodeDessinEnfant = pNoeud->m_CodeImage % 100;

			// recup code dessin de la branche parent
			HItemParent = m_TreeNomenclature.GetParentItem(m_HItemCur);
			TextItemParent = m_TreeNomenclature.GetItemText(HItemParent);
			TextItemParent.TrimLeft();
			TextItemParent.TrimRight();
			if (HItemParent != NULL)
			{
				pNoeudMere = (CNoeud*)m_TreeNomenclature.GetItemData(HItemParent);
				if (pNoeudMere != NULL) CodeDessinParent = pNoeudMere->m_CodeImage / 100;
			}
			else
				// ce sont les 2 branches racines
				CodeDessinParent = 0;

		}

		else
		{
			// recup code dessin de la branche parent
			HItemParent = m_TreeNomenclature.GetParentItem(m_HItemCur);
			if (HItemParent != NULL)
			{
				pNoeudMere = (CNoeud*)m_TreeNomenclature.GetItemData(HItemParent);
				if (pNoeudMere != NULL) CodeDessinParent = pNoeudMere->m_CodeImage / 100;
				HItemGrandParent= m_TreeNomenclature.GetParentItem(HItemParent);
				if (HItemGrandParent != NULL)
				{
					pNoeudGrandMere = (CNoeud*)m_TreeNomenclature.GetItemData(HItemGrandParent);
					if (pNoeudGrandMere != NULL) CodeDessinGrandParent = pNoeudGrandMere->m_CodeImage / 100;
				}

			}
		}


		// Texte de l'élément courant (Marque/Groupe/Annonceur/Produit/Campagne...)
		StrElmt ="";
		switch (CodeDessin)
		{
			case CTreeCtrlX::ImageMarque: 
			{
				StrElmt = "marque";
				break;
			}
			case CTreeCtrlX::ImageGroupe: 
			{
				StrElmt = "groupe";
				break;
			}
			case CTreeCtrlX::ImageAnnonceur: 
			{
				StrElmt = "annonceur";
				break;
			}
			case CTreeCtrlX::ImageProduit: 
			{
				StrElmt = "produit";
				break;
			}
			case CTreeCtrlX::ImageCampagne: 
			{
				StrElmt = "campagne";
				break;
			}
			case CTreeCtrlX::ImageFamille: 
			{
				StrElmt = "famille";
				break;
			}
			case CTreeCtrlX::ImageSecteur: 
			{
				StrElmt = "secteur";
				break;
			}
		}

		// ni menu popup pour produit dans arbo marque
		// ni pour campagne dans marque/produit
		if (CodeDessin == CTreeCtrlX::ImageProduit && (CodeDessinParent == CTreeCtrlX::ImageMarque || CodeDessinParent == CTreeCtrlX::ImageSecteur))
			AffichPopPup = false;
		if (CodeDessin == CTreeCtrlX::ImageCampagne && (CodeDessinGrandParent == CTreeCtrlX::ImageSecteur))
			AffichPopPup = false;


		if  (AffichPopPup == true)
		{
			// Change texte menu selon la branche ou elmt courant
			for (int i = 0;i<NbItem;i++)
			{
				NvTxtBranche = "";
				ItemMenuValid = false;
				switch (i)
				{

					case Popup_TauxFO : // Mise à jour Taux FO
					{
						if (CodeDessinEnfant == CTreeCtrlX::ImageMarque || CodeDessinEnfant == CTreeCtrlX::ImageSecteur || (CodeDessin == CTreeCtrlX::ImageProduit && (CodeDessinParent == CTreeCtrlX::ImageMarque || CodeDessinParent == CTreeCtrlX::ImageSecteur)) || CodeDessin == CTreeCtrlX::ImageMarque)
							ItemMenuValid = false;
						else if (CodeDessin == CTreeCtrlX::ImageSecteur || CodeDessinParent == CTreeCtrlX :: ImageSecteur || CodeDessinGrandParent == CTreeCtrlX :: ImageSecteur)
							ItemMenuValid = false;
						else if (CodeDessin == CTreeCtrlX::ImageOpen || CodeDessin == CTreeCtrlX::ImageClose)
							ItemMenuValid = false;
						else if (pNoeud != NULL)
							if (pNoeud->m_Codif == "") ItemMenuValid = false;
						else
						{
							NvTxtBranche = 	m_TMenu[i];
							ItemMenuValid = true;
						}
						break;
					}

				}

				// Modif ou inhibation des lignes du menu popup
				// ??? sous NT problème MenuPopup->GetMenuItemInfo(i,&ItemInfo,TRUE);
				IdItemMenu = MenuPopupTaux->GetMenuItemID(i);

				if (ItemMenuValid == true && NvTxtBranche != "")
				{
					MenuPopupTaux->ModifyMenu(i,MF_BYPOSITION | MF_STRING,IdItemMenu,NvTxtBranche);
				}	
				else
				{
					MenuPopupTaux->EnableMenuItem(IdItemMenu,MF_GRAYED);   
				}	


			}

			// Revisualise le menu popup aprés modif texte
			//Menu.GetSubMenu(0)->TrackPopupMenu(TPM_LEFTBUTTON   | TPM_LEFTALIGN,Point.x,Point.y,this,NULL);
			MenuTaux.GetSubMenu(0)->TrackPopupMenu(TPM_RIGHTBUTTON |TPM_LEFTALIGN,Point.x,Point.y,this,NULL);
		}
	}
	else
		AfxMessageBox("Accés modification nomenclature non autorisée");

	*pResult = 0;	

}

void CNomenclatureView::MajTabTxtNomencl(HTREEITEM hItem,BOOL AvecCampagne)
{
	CString Txt;
	HTREEITEM hItemParent;
	CNoeud *pNoeud;
	CNoeud *pNoeudParent;	
	int CodeDessin;
	int Niveau;

	// Récupération du niveau arborescence
	m_TreeNomenclature.GetItemImage(hItem,CodeDessin,CodeDessin);
	if (CodeDessin == CTreeCtrlX::ImageCampagne)
	{
		// feuille campagne, il faut remonter à la racine parent
		hItemParent = m_TreeNomenclature.GetParentItem(hItem);
		pNoeudParent = (CNoeud*)m_TreeNomenclature.GetItemData(hItemParent);	
		Niveau = pNoeudParent->m_Niveau + 1;
	}
	else
	{
		// récupére les données du noeud
		pNoeud = (CNoeud*)m_TreeNomenclature.GetItemData(hItem);
		Niveau = pNoeud->m_Niveau;
	}

	// Si rubrique groupe ou marque, saut de ligne
	if (CodeDessin == CTreeCtrlX::ImageGroupe ||CodeDessin == CTreeCtrlX::ImageMarque)
	{
		Txt = "    ";
		m_TabTxtNomencl.Add(Txt);
	}
	
	// insére dans tableau le texte précédé du niveau et code dessin item
	Txt.Format("%d%d", Niveau,CodeDessin);
	Txt = Txt + m_TreeNomenclature.GetItemText(hItem);

	if ((CodeDessin != CTreeCtrlX::ImageCampagne) ||
		(CodeDessin == CTreeCtrlX::ImageCampagne) && AvecCampagne == TRUE)
	{
		// Attention pour les campagnes en option
		m_TabTxtNomencl.Add(Txt);
	}	

	if (m_TreeNomenclature.ItemHasChildren(hItem))
	{		
		// passe à la branche enfant
		hItem = m_TreeNomenclature.GetChildItem(hItem);	
		while (hItem != NULL)
		{
			MajTabTxtNomencl(hItem,AvecCampagne);
			hItem = m_TreeNomenclature.GetNextSiblingItem(hItem);						
		}
	}

}

// Tri d'aun tableau de String Value (1 mode décroissant,2 mode croissant)
void CNomenclatureView::TriTableauStringValue(CStringArray &TabArray,int Mode)
{
	int IndexDep = 0;
	int IndexMax,IndexMin;
	int NbElmtTab;
	CString StrTmp;
	int val1,val2;

	NbElmtTab = TabArray.GetSize();

	if (NbElmtTab > 1)
	{
		if (Mode = 1)
		{
			// Tri décroissant
			for (int i = IndexDep;i < NbElmtTab;i++)
			{
				IndexMax = i;
				for (int j = i+1;j < NbElmtTab;j++)
				{
					val1 = atoi(TabArray[j]);
					val2 = atoi(TabArray[IndexMax]);
					if (atoi(TabArray[j]) > atoi(TabArray[IndexMax]))
						IndexMax = j;
				}				

				// permut IndexMax et i
				if (IndexMax != i) 
				{
					StrTmp = TabArray[IndexMax];
					TabArray[IndexMax] = TabArray[i];
					TabArray[i] = StrTmp;

				}
			}

			for (int i=0;i<NbElmtTab;i++)
				val1 = atoi(TabArray[i]);
		}
	
		else
		{
			// Tri croissant
			for (int i = IndexDep;i < NbElmtTab;i++)
			{
				IndexMin = i;
				for (int j = i+1;j < NbElmtTab;j++)
				{
					if (atoi(TabArray[j]) < atoi(TabArray[IndexMin]))
						IndexMin = j;
				}				

				// permut IndexMax et i
				if (IndexMin != i) 
				{
					StrTmp = TabArray[IndexMin];
					TabArray[IndexMin] = TabArray[i];
					TabArray[i] = StrTmp;

				}
			}
		}
	}

}

void CNomenclatureView::OnGetdispinfoTree1(NMHDR* pNMHDR, LRESULT* pResult) 
{
	TV_DISPINFO* pTVDispInfo = (TV_DISPINFO*)pNMHDR;
	// TODO: Add your control notification handler code here
	
	*pResult = 0;
}

void CNomenclatureView::OnSelchangedTree1(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_TREEVIEW* pNMTreeView = (NM_TREEVIEW*)pNMHDR;

	CTreeCtrlX* pTree = (CTreeCtrlX*) GetDlgItem(IDC_TREE1);

	// Recup handle branche sélectionnée et ses infos
	HTREEITEM hSelected = pNMTreeView->itemNew.hItem;
	
	if (hSelected != NULL)
	{
		char text [41];
		m_ItemCur.mask = TVIF_HANDLE | TVIF_TEXT;
		m_ItemCur.hItem = hSelected;
		m_ItemCur.pszText = text;
		m_ItemCur.cchTextMax = 40;
		VERIFY(pTree->GetItem(&m_ItemCur));
	}

	*pResult = 0;
}




void CNomenclatureView::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	// Coloriage du fond de ce formulaire
	CRect rect;

	// Coloriage du fond de ce formulaire
	//CBrush fond(RGB_MarronCacadoie);
	CBrush fond(RGB_BleuJFC);
	GetClientRect(&rect);
	dc.FillRect(rect,&fond);
}

int CNomenclatureView::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CFormView::OnCreate(lpCreateStruct) == -1)
		return -1;


	
	return 0;
}

// Preview Impression Nomenclature / avant appel formulaire paramètrages
void CNomenclatureView::OnFilePrintPreview() 
{

	HTREEITEM hItemAnnonceur;
	HTREEITEM hItemMarque;
	HTREEITEM hItemSecteur;

	CFormulaireImpression FormulaireImpression;

	// lance d'abord boite paramètres impression
	if (FormulaireImpression.DoModal() == IDOK)
	{
		//Remplissage du Tableau Texte selon Option Annonceur ou Marque (avec ou sans campagne)
		hItemAnnonceur = m_TreeNomenclature.GetRootItem();
		hItemMarque = m_TreeNomenclature.GetNextItem(hItemAnnonceur,TVGN_NEXT);
		hItemSecteur = m_TreeNomenclature.GetNextItem(hItemMarque,TVGN_NEXT);

		// Creation du tableau texte ligne associé à la nomenclature (pour module impression)
		m_TabTxtNomencl.RemoveAll();
		if (FormulaireImpression.m_ChoixAnnonceurMarque == 0)
		{
			// Preview de la nomenclature annonceurs
			MajTabTxtNomencl(hItemAnnonceur,FormulaireImpression.m_CheckCampagne);
			theApp.m_TypeNomencl = "Annonceur";
		}
		else if (FormulaireImpression.m_ChoixAnnonceurMarque == 1)
		{
			// Preview de la nomenclature marques
			MajTabTxtNomencl(hItemMarque,FormulaireImpression.m_CheckCampagne);
			theApp.m_TypeNomencl = "Marque";
		}
		else
			// Preview de la nomenclature marques
		{
			MajTabTxtNomencl(hItemSecteur,FormulaireImpression.m_CheckCampagne);
			theApp.m_TypeNomencl = "Secteur";
		}

		// on lance la boite Preview
		CFormView ::OnFilePrintPreview();
	}
		
}

// Preview Impression Nomenclature / avant appel formulaire paramètrages
void CNomenclatureView::OnFilePrintbis() 
{
	HTREEITEM hItemAnnonceur;
	HTREEITEM hItemMarque;
	HTREEITEM hItemSecteur;

	CFormulaireImpression FormulaireImpression;

	// lance d'abord boite paramètres impression
	if (FormulaireImpression.DoModal() == IDOK)
	{
		//Remplissage du Tableau Texte selon Option Annonceur ou Marque (avec ou sans campagne)
		hItemAnnonceur = m_TreeNomenclature.GetRootItem();
		hItemMarque = m_TreeNomenclature.GetNextItem(hItemAnnonceur,TVGN_NEXT);
		hItemSecteur = m_TreeNomenclature.GetNextItem(hItemMarque,TVGN_NEXT);

		// Creation du tableau texte ligne associé à la nomenclature (pour module impression)
		m_TabTxtNomencl.RemoveAll();
		if (FormulaireImpression.m_ChoixAnnonceurMarque == 0)
		{
			// Impression de la nomenclature annonceurs
			MajTabTxtNomencl(hItemAnnonceur,FormulaireImpression.m_CheckCampagne);
			theApp.m_TypeNomencl = "Annonceur";
		}
		else if (FormulaireImpression.m_ChoixAnnonceurMarque == 1)
		{
			// Impression de la nomenclature marques
			MajTabTxtNomencl(hItemMarque,FormulaireImpression.m_CheckCampagne);
			theApp.m_TypeNomencl = "Marque";
		}
		else
		{
			// Impression de la nomenclature secteurs
			MajTabTxtNomencl(hItemSecteur,FormulaireImpression.m_CheckCampagne);
			theApp.m_TypeNomencl = "Secteur";
		}

		// on lance l'impression
		CFormView ::OnFilePrint();
	}
}


// Gestion du menu Année
BOOL CNomenclatureView::OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo)
{

	CDialogNvlleAnnee DialogNvlleAnnee;
	CString AnneeSelect;

	// Test si on traite les ss-menus dynamiques Année
	if ((pHandlerInfo == NULL) &&
		((nID >= ID_MENU_DYN_FIRST && nID <= ID_MENU_DYN_LAST)||nID == ID_ANNE_NOUVELLE|| nID == ID_ANNE_DUPLIQUER))
	{
		switch (nCode)
		{
			case CN_UPDATE_COMMAND_UI:
				// rend le ss-menu exploitable
				if (pExtra != NULL)	((CCmdUI*)pExtra)->Enable(TRUE);
				return TRUE;
				break;

			case CN_COMMAND:

				if (nID >= ID_MENU_DYN_FIRST && nID <= ID_MENU_DYN_LAST)
				{
					// traitement de l'année sélectionnée
					{
						CMenu* pMenuPrincipal = theApp.m_pMainWnd->GetMenu();
						pMenuPrincipal->GetMenuString(nID,AnneeSelect,MF_BYCOMMAND);
						if (theApp.m_AnneeNomencl    != AnneeSelect )
						{
							// Déchargement de la nouvelle nomenclature année en cours						
							if (DegageNomenclatureCourante())
							{
								// Chargement de la nouvelle nomenclature année
								ChargeFicProduitMarqueSecteur(AnneeSelect); 						
							}
						}

						return TRUE;
						break;
					}
				}

				else if (nID == ID_ANNE_NOUVELLE)
				{
					// Charge tab des années possibles à créer
					if (ChargeTabAnneePossible(DialogNvlleAnnee.m_TabAnneePossible))
					{
						// Création d'une nouvelle année
						if (DialogNvlleAnnee.m_TabAnneePossible.GetSize() > 0)
						{
							DialogNvlleAnnee.m_FlagNvlleAnnee = true;
							DialogNvlleAnnee.m_AnneeCur = theApp.m_AnneeNomencl;
							DialogNvlleAnnee.m_TitreDialogAnnee = "Création nouvelle année nomenclature";
							if (DialogNvlleAnnee.DoModal() == IDOK)
							{
								// Création d'une nouvelle nomenclature
								CreerNouvelArbreNomencl(DialogNvlleAnnee.m_AnneeSelect);

								// Ajout Année dans Tab des Années
								AjouteAnneeNomenclature(DialogNvlleAnnee.m_AnneeSelect);

								// Trier ce tableau
								TriTableauStringValue(m_TabAnneeNomencl,1);

								// Ajout Annee dans menu Popup
								/*
								CMenu* pMenuPrincipal = theApp.m_pMainWnd->GetMenu();
								CMenu* pMenuNomencl = pMenuPrincipal->GetSubMenu(0);
								CMenu* pMenuAnnee = pMenuNomencl->GetSubMenu(3);
								pMenuAnnee->AppendMenu(MF_STRING | MF_ENABLED, ID_MENU_DYN_FIRST + m_TabAnneeNomencl.GetSize()-1,DialogNvlleAnnee.m_AnneeSelect);
								*/

								// on vire l'ancien menu popup
								CMenu* pMenuPrincipal = theApp.m_pMainWnd->GetMenu();
								CMenu* pMenuNomencl = pMenuPrincipal->GetSubMenu(0);
								pMenuNomencl->RemoveMenu( 3, MF_BYPOSITION);

								// puis on le recrée avec bon ordre année
								CreerMenuPopupAnnee();
							}
						}
					}
				}

				else if (nID == ID_ANNE_DUPLIQUER)
				{
					// Dupliquer une année sur une autre
					ChargeTabAnneeExistante(DialogNvlleAnnee.m_TabAnneePossible);

					// Si suffisamment d'année autre que l'année courante, choix de l'année destination
					if (DialogNvlleAnnee.m_TabAnneePossible.GetSize() > 1)
					{
						DialogNvlleAnnee.m_FlagNvlleAnnee = false;
						DialogNvlleAnnee.m_AnneeCur = theApp.m_AnneeNomencl;
						DialogNvlleAnnee.m_TitreDialogAnnee = "Duplication nomenclature année "+ theApp.m_AnneeNomencl + " vers ...";
						if (DialogNvlleAnnee.DoModal() == IDOK)
						{
							// Duplication de l'année courante vers l'année sélectionnée
							DupliqueAnneeNomenclature(theApp.m_AnneeNomencl,DialogNvlleAnnee.m_AnneeSelect);
						}
					}
				}


			default:
				break;
		}
	}
	
	return CFormView::OnCmdMsg(nID, nCode, pExtra, pHandlerInfo);
}

// Suppression d'un répertoire ainsi que toute son arborescence (ss-rép + fichiers)
bool CNomenclatureView::DeleteDirectory(CString DirSup)
{

	CPWFile File;
	CFileFind Finder;
	BOOL Filefind = FALSE ;
	CString PathFile;

	Filefind = Finder.FindFile(DirSup);
	while (Filefind)
	{
		PathFile = Finder.GetFilePath();

		if (Finder.IsDirectory())
			// C'est un répertoire on le détruit
			DeleteDirectory(PathFile);				
		else
		{
			// C'est un fichier on le détruit
			File.Remove(PathFile);
		}

		// passe au fichier ou répertoire suivant
  		Filefind = Finder.FindNextFile();
	}

	return true;
}

// Copie d'un directory sur un autre (avec toute l'arborescence ss-rép/fichiers)
bool CNomenclatureView::CopyDirectory(CString DirSrc, CString DirDst)
{
	CFileFind Finder;
	BOOL Filefind = FALSE ;
	CString PathFile;
	CString FileName;

	Filefind = Finder.FindFile(DirSrc + "\\*.*");

	while (Filefind)
	{

		Filefind = Finder.FindNextFile();
		PathFile = Finder.GetFilePath();
		FileName = Finder.GetFileName();

		if (FileName != "." && FileName != "..")
		{
			if (Finder.IsDirectory())
			{
				// C'est un répertoire on le détruit
				CreateDirectory(DirDst + "\\" + FileName  ,NULL);
				CopyDirectory(PathFile,DirDst +"\\" + FileName);				;
			}
			else
			{
				// C'est un fichier on le détruit
				CopyFile(PathFile ,DirDst +  "\\"  +FileName,false);
			}	
		}
	}

	return true;
}

void CNomenclatureView::OnSize(UINT nType, int cx, int cy) 
{
	CFormView::OnSize(nType, cx, cy);
	m_MM.Resize(cx,cy);	
}

void CNomenclatureView::OnClickTree1(NMHDR* pNMHDR, LRESULT* pResult) 
{

	/* Test Visu Date Exercice branche sélectionnée
	TV_HITTESTINFO HitTest;	
	CString DateExercice;

	// Branche branche sélectionnée
	GetCursorPos(&HitTest.pt);
	m_TreeNomenclature.ScreenToClient(&HitTest.pt);
	m_HItemCur = m_TreeNomenclature.HitTest(&HitTest);
	if (m_HItemCur == NULL) return;

	DateExercice = m_TreeNomenclature.DateExerciceBranche(m_HItemCur);
	AfxMessageBox ("Date Exercice = " + DateExercice);
	*/

	*pResult = 0;

}

void CNomenclatureView::OnMenunomenclAjouterannonceursansgroupe() 
{
	// Ajout d'un nouvel annonceur sans groupe
	CFormulaireGroupe FormulaireG;
	CString Txt4 = "xxx";
	CString Txt1 = "xxx";
	CString Txt2 = "xxx";
	CString Txt3 = "xxx";
	CString RepCourantCreation ="";
	CNoeud *pNoeud;
	HTREEITEM HItem;
	bool BrancheGrpVideExist = false;
	bool Ok;

	HTREEITEM HNvItem = NULL;
	HTREEITEM hBrancheMarque = NULL;
	HTREEITEM hBrancheFamille = NULL;
	int IdDessin;
	CString TxtItem;

	// Recup Ident Image Item
	m_TreeNomenclature.GetItemImage(m_HItemCur,IdDessin,IdDessin);

	// Ident Image Enfant = forcément un annonceur
	pNoeud = (CNoeud*)m_TreeNomenclature.GetItemData(m_HItemCur);		
	FormulaireG.m_TitreFenFG = "Définir les références de l'annonceur direct sans groupe";

	// C'est un annonceur et on est en mode ajout
	FormulaireG.m_Annonceur = true;
	FormulaireG.m_Mode = 0;

	// Recup Chemin Courant Création Répertoire
	RepCourantCreation = pNoeud->m_PathFichier;
	
	if (IdDessin != CTreeCtrlX::ImageGroupe)
	{
		// Recup Chemin Courant Création Répertoire / on le crée si inexistant
		RepCourantCreation = pNoeud->m_PathFichier + "\\AnnonceurDirect";
		Ok = CreateDirectory(RepCourantCreation ,NULL);

		// Si branche groupe vide non présent on l'ajoute
		if (m_TreeNomenclature.ItemHasChildren(m_HItemCur))
		{
			// enfants sur cette branche
			HItem = m_TreeNomenclature.GetChildItem(m_HItemCur);
			while (HItem != NULL && BrancheGrpVideExist == false)
			{
				TxtItem = m_TreeNomenclature.GetItemText(HItem);
				TxtItem.TrimLeft();
				TxtItem.TrimRight();
				//if (TxtItem.GetLenght() > 2) TxtItem = TxtItem.Left(3);
				if (TxtItem == "...")
					BrancheGrpVideExist = true;
				else
					HItem = m_TreeNomenclature.GetNextItem(HItem,TVGN_NEXT);	
			}
		}

		if (!BrancheGrpVideExist) 
			// Groupe vide non existant on crée la branche
			HItem = m_TreeNomenclature.AjouterBranche(m_HItemCur,"...","AnnonceurDirect","xxx",Txt1,Txt2,Txt3,Txt4,false);	
	}

	else
		// Ici on est déjà dans l'arborescence annonceur direct
		HItem = m_HItemCur;

	// Ajout d'un nouvel annonceur sans groupe
	FormulaireG.m_StrAliasFG = "";
	FormulaireG.m_StrCodeFG ="";
	FormulaireG.m_StrNomRepFG = "";

	// Annee exercice par défaut = 1er janvier année courante
	FormulaireG.m_StrDateFG = "0101" + theApp.m_AnneeNomencl;

	// C'est un annonceur et on est en mode ajout
	FormulaireG.m_Annonceur = true;
	FormulaireG.m_Mode = 0;

	if (FormulaireG.DoModal() == IDOK)
	{
		// Avant tout on vérifie possibiilté de créér le répertoire
		RepCourantCreation = RepCourantCreation + "\\" + FormulaireG.m_StrNomRepFG;

		// Ajout Répertoire branche si branche correctement crée
		if (AjoutRepNvlleBranche(RepCourantCreation) == true)
		{
			// Ajout de la nvlle branche annonceur direct
			//HNvItem = m_TreeNomenclature.AjouterBrancheAnnonceurSsGroupe(m_HItemCur,FormulaireStd.m_StrAliasFStd,FormulaireStd.m_StrNomRepFStd,FormulaireStd.m_StrCodeFStd,Txt1);	
			if (HItem != NULL)
			{
				// Recup les infos EDI saisies
				pNoeud->m_TypeTiers			  = FormulaireG.m_TypeTiers;
				pNoeud->m_CodeTiers			  = FormulaireG.m_CodeTiers;			
				pNoeud->m_TypeCodeTiers		  = FormulaireG.m_TypeCodeTiers;		
				pNoeud->m_NomTiers			  = FormulaireG.m_NomTiers;				
				pNoeud->m_AdresseTiers1		  = FormulaireG.m_AdresseTiers1;		
				pNoeud->m_AdresseTiers2		  = FormulaireG.m_AdresseTiers2;		
				pNoeud->m_CodePostalTiers	  = FormulaireG.m_CodePostalTiers;		
				pNoeud->m_VilleTiers		  = FormulaireG.m_VilleTiers;			
				pNoeud->m_CodePaysTiers		  = FormulaireG.m_CodePaysTiers;		

				// Interlocuteur(s) tiers
				pNoeud->m_NomContactTiers	  = FormulaireG.m_NomContactTiers;		
				pNoeud->m_PrenomContactTiers  = FormulaireG.m_PrenomContactTiers;	
				pNoeud->m_AdrMailContactTiers = FormulaireG.m_AdrMailContactTiers;	
				
				pNoeud->m_NoTelTiers		  = FormulaireG.m_NoTelTiers;			
				pNoeud->m_NoFaxTiers		  = FormulaireG.m_NoFaxTiers;
				HNvItem = m_TreeNomenclature.AjouterBranche(HItem,FormulaireG.m_StrAliasFG,FormulaireG.m_StrNomRepFG,FormulaireG.m_StrCodeFG,Txt1,Txt2,Txt3,FormulaireG.m_StrDateFG,false);	
			}
		}
	}

	// Sauve derniere sélection menu popup
	m_InxMenuCur = Popup_AjouterAnnSsGrp;

	// Synchronisation arborescence avec fichier arb
	m_TreeNomenclature.TrierArbre();
	RedrawWindow();
	SynchroArbre_FicArb();
	
}

bool CNomenclatureView::FamilleAuMoins1ProduitAutresAnnees(CString TxtFamille)
{

	CString FicProduitAnneeCour;
	CString Ligne;
	CPWFile File;
	bool FamilleUtilise = false;
	int i = 0;
	
	while (i<m_TabAnneeNomencl.GetSize() && FamilleUtilise == false)
	{
		if (m_TabAnneeNomencl[i] != theApp.m_AnneeNomencl)
		{
			FicProduitAnneeCour=theApp.m_PathNomencl + m_TabAnneeNomencl[i] + "\\produit.arb";
			if (File.OpenFile(FicProduitAnneeCour))
			{
				File.ReadString(&Ligne);							// Ligne commentaire
				File.ReadString(&Ligne);

				while (File.ReadString(&Ligne) && Ligne != "" && FamilleUtilise == false)
				{
					if (TxtFamille == Ligne)
					{
						// en plus on vérifie qu'il s'agit d'un produit (indice produit = 4)
						File.ReadString(&Ligne);
						File.ReadString(&Ligne);
						if (Ligne == "4") FamilleUtilise = true;
					}
				}
		
				File.CloseFile();
			}
		}

		// passe à l'année suivante
		if (FamilleUtilise) break;
		i++;
	}

	return FamilleUtilise;
}


bool CNomenclatureView::SecteurAuMoins1ProduitAutresAnnees(CString TxtFamille,CString TxtSecteur)
{
	CString FicProduitAnneeCour;
	CString Ligne;
	CPWFile File;
	bool SecteurUtilise = false;
	int i = 0;
	
	while (i<m_TabAnneeNomencl.GetSize() && SecteurUtilise == false)
	{
		if (m_TabAnneeNomencl[i] != theApp.m_AnneeNomencl)
		{
			FicProduitAnneeCour=theApp.m_PathNomencl + m_TabAnneeNomencl[i] + "\\produit.arb";
			if (File.OpenFile(FicProduitAnneeCour))
			{
				File.ReadString(&Ligne);							// Ligne commentaire
				File.ReadString(&Ligne);

				while (File.ReadString(&Ligne) && Ligne != "" && SecteurUtilise == false)
				{
					if (TxtFamille == Ligne)
					{
						File.ReadString(&Ligne);
						if (TxtSecteur == Ligne)
						{
						// en plus on vérifie qu'il s'agit d'un produit (indice produit = 4)
						File.ReadString(&Ligne);
						if (Ligne == "4") SecteurUtilise = true;
						}	
					}
				}
				File.CloseFile();
			}
		}

		if (SecteurUtilise) break;

		// passe à l'année suivante
		i++;
		

	}

	return SecteurUtilise;

}


void CNomenclatureView::OnMenunomenclAjouterannonceursansgroupesansproduit() 
{
	
	// Attention cette procédure serira également pour créer un annonceur
	// avec groupe mais SANS PRODUIT (Date Exercice non active)

	// Ajout d'un nouvel annonceur sans groupe ni produit
	// Annonceur particulier avec notion de famille et secteur
	CFormulaire FormulaireP;
	CString Txt1 = "xxx";
	CString Txt2 = "xxx";
	CString Txt3 = "xxx";
	CString Txt4 = "xxx";
	CString RepCourantCreation ="";
	CString TxtItem;
	CNoeud *pNoeud;
	bool BrancheGrpVideExist = false;
	bool Ok;


	m_AnnonceurIsole = true;

	HTREEITEM HItem = NULL;
	HTREEITEM HNvItem = NULL;
	HTREEITEM hBrancheMarque = NULL;
	HTREEITEM hBrancheFamille = NULL;
	int IdDessin;

	// Recup Ident Image Item
	m_TreeNomenclature.GetItemImage(m_HItemCur,IdDessin,IdDessin);

	// Ident Image Enfant = forcément un annonceur
	pNoeud = (CNoeud*)m_TreeNomenclature.GetItemData(m_HItemCur);
	if (pNoeud->m_Alias == "..." || pNoeud->m_Alias == "")
	{
		// Il s'agit bien ici d'un annonceur isolé (sans groupe, sans produit)
		FormulaireP.m_TitreFenFP = "Définir les références de l'annonceur isolé";
		FormulaireP.m_AnnonceurIsole = true;
	}

	else
	{
		// Il s'agit bien ici d'un annonceur sans produit
		FormulaireP.m_TitreFenFP = "Définir les références de l'annonceur sans produit";
		FormulaireP.m_AnnonceurIsole = false;
	}

	// Recup Chemin Courant Création Répertoire
	RepCourantCreation = pNoeud->m_PathFichier;

	if (IdDessin != CTreeCtrlX::ImageGroupe)
	{
		// Recup Chemin Courant Création Répertoire / on le crée si inexistant
		RepCourantCreation = pNoeud->m_PathFichier + "\\AnnonceurDirect";
		Ok = CreateDirectory(RepCourantCreation ,NULL);

		// Si branche groupe vide non présent on l'ajoute
		if (m_TreeNomenclature.ItemHasChildren(m_HItemCur))
		{
			// enfants sur cette branche
			HItem = m_TreeNomenclature.GetChildItem(m_HItemCur);
			while (HItem != NULL && BrancheGrpVideExist == false)
			{
				TxtItem = m_TreeNomenclature.GetItemText(HItem);
				TxtItem.TrimLeft();
				TxtItem.TrimRight();
				//if (TxtItem.GetLenght() > 2) TxtItem = TxtItem.Left(3);
				if (TxtItem == "...")
					BrancheGrpVideExist = true;
				else
					HItem = m_TreeNomenclature.GetNextItem(HItem,TVGN_NEXT);	
			}
		}

		if (!BrancheGrpVideExist) 
			// Groupe vide non existant on crée la branche
			HItem = m_TreeNomenclature.AjouterBranche(m_HItemCur,"...","AnnonceurDirect","xxx",Txt1,Txt2,Txt3,Txt4,true);	
	}

	else
		// Ici on est déjà dans l'arborescence annonceur direct
		HItem = m_HItemCur;

	

	// Ajout d'un nouvel annonceur direct il faut charger la liste des marques disponibles
	// ainsi que la liste des familles et secteurs disponibles
	FormulaireP.m_StrAliasFP = "";
	FormulaireP.m_StrCodeFP ="";
	FormulaireP.m_StrNomRepFP = "";
	FormulaireP.m_StrMarqueFP = "0";


	// Annee exercice par défaut = 1er janvier année courante
	FormulaireP.m_StrDateFP = "0101" + theApp.m_AnneeNomencl;
	FormulaireP.m_pNoeudFamille = m_ArbreEnMemoire.m_pNoeud[2];


	// pas de chargement des marques disponibles / c'est un annonceur
	FormulaireP.m_TabMarque.RemoveAll();

	if (ChargeTabFamille(FormulaireP.m_TabFamille) == true)
	{
		if (FormulaireP.DoModal() == IDOK)
		{
			// Avant tout on vérifie possibilté de créér le répertoire
			

			RepCourantCreation = RepCourantCreation + "\\" + FormulaireP.m_StrNomRepFP;

			// Ajout Répertoire branche si branche correctement crée
			if (AjoutRepNvlleBranche(RepCourantCreation) == true)
			{
				// Ajout de la nvlle branche annonceur direct
				//HNvItem = m_TreeNomenclature.AjouterBrancheAnnonceurDirect(m_HItemCur,FormulaireP.m_StrAliasFP,FormulaireP.m_StrNomRepFP,FormulaireP.m_StrCodeFP,FormulaireP.m_StrMarqueFP,FormulaireP.m_StrFamilleFP,FormulaireP.m_StrSecteurFP,Txt6);	
				if (HItem != NULL)
				{
					// Les infos EDI
					pNoeud->m_TypeTiers			  = FormulaireP.m_TypeTiers;
					pNoeud->m_CodeTiers			  = FormulaireP.m_CodeTiers;			
					pNoeud->m_TypeCodeTiers		  = FormulaireP.m_TypeCodeTiers;		
					pNoeud->m_NomTiers			  = FormulaireP.m_NomTiers;				
					pNoeud->m_AdresseTiers1		  = FormulaireP.m_AdresseTiers1;		
					pNoeud->m_AdresseTiers2		  = FormulaireP.m_AdresseTiers2;		
					pNoeud->m_CodePostalTiers	  = FormulaireP.m_CodePostalTiers;		
					pNoeud->m_VilleTiers		  = FormulaireP.m_VilleTiers;			
					pNoeud->m_CodePaysTiers		  = FormulaireP.m_CodePaysTiers;		

					// Interlocuteur(s) tiers
					pNoeud->m_NomContactTiers	  = FormulaireP.m_NomContactTiers;		
					pNoeud->m_PrenomContactTiers  = FormulaireP.m_PrenomContactTiers;	
					pNoeud->m_AdrMailContactTiers = FormulaireP.m_AdrMailContactTiers;	
					
					pNoeud->m_NoTelTiers		  = FormulaireP.m_NoTelTiers;			
					pNoeud->m_NoFaxTiers		  = FormulaireP.m_NoFaxTiers;

					HNvItem = m_TreeNomenclature.AjouterBranche(HItem,FormulaireP.m_StrAliasFP,FormulaireP.m_StrNomRepFP,FormulaireP.m_StrCodeFP,FormulaireP.m_StrMarqueFP,FormulaireP.m_StrFamilleFP,FormulaireP.m_StrSecteurFP,FormulaireP.m_StrDateFP,false);	

					// Ajout Annonceur au niveau de l'arborescence famille/secteur
					hBrancheFamille = m_TreeNomenclature.GetRootItem();
					hBrancheFamille = m_TreeNomenclature.GetNextItem(hBrancheFamille,TVGN_NEXT);
					hBrancheFamille = m_TreeNomenclature.GetNextItem(hBrancheFamille,TVGN_NEXT);



					m_TreeNomenclature.AjouterAnnonceurDansFamilleSecteur(hBrancheFamille,
													  					  FormulaireP.m_StrFamilleFP,
																		  FormulaireP.m_StrSecteurFP,
																		  FormulaireP.m_StrAliasFP,
																		  FormulaireP.m_StrNomRepFP,
																		  FormulaireP.m_StrCodeFP,
																		  FormulaireP.m_StrDateFP);
				}
				 
			}
		}
	}

	// Sauve derniere sélection menu popup
	m_InxMenuCur = Popup_AjouterAnnSsGrpSsProd;

	// Synchronisation arborescence avec fichier arb
	m_TreeNomenclature.TrierArbre();
	RedrawWindow();
	SynchroArbre_FicArb();
	
}

void CNomenclatureView::OnMenunomenclTaux() 
{
	CFormulaireTauxFO FormulaireTauxFO;
	CNoeud *pNoeud = NULL;
	CNoeud *pNoeudTmp; 
	HTREEITEM HItem;

	// Passage Info Item Nomenclature sélectionné
	if (m_HItemCur == NULL) return;
		
	// Texte associé à la branche courante
	FormulaireTauxFO.m_LibelleNomencl = m_TreeNomenclature.GetItemText(m_HItemCur);

	// Code associé à ce noeud nomenclature
	FormulaireTauxFO.m_HItemCur;
	pNoeud = (CNoeud*)m_TreeNomenclature.GetItemData(m_HItemCur);
	if (pNoeud == NULL) return;


	// ************************************************************************************
	// Test si utilisateur autorisé à entrer les taux
	if (UtilisateurTauxOk() == false)
	{
		AfxMessageBox ("Vous n'êtes pas autorisé à changer ou modifier les taux fin d'ordre");
		return;
	}
	// ************************************************************************************

	// Fabriquue chaine code nomenclature = code courant +"_"+ codes parents (séparés par "_")
	FormulaireTauxFO.m_CodeNomencl = pNoeud->m_Codif;
	HItem = m_HItemCur;
	pNoeudTmp = pNoeud;
	while (pNoeudTmp != NULL && HItem != NULL)
	{
	
		HItem= m_TreeNomenclature.GetParentItem(HItem);
		if (HItem == NULL) break;

		pNoeudTmp = (CNoeud*)m_TreeNomenclature.GetItemData(HItem);

		// Si noeud parent existe, on ajoute code nomencl parent
		if (pNoeudTmp == NULL) break; 
		
		if (pNoeudTmp->m_Codif != "")
			FormulaireTauxFO.m_CodeNomencl = FormulaireTauxFO.m_CodeNomencl + "_" + pNoeudTmp->m_Codif;

	}

	FormulaireTauxFO.m_LibelleNomencl = pNoeud->m_Libelle;

	// Passe le noeud courant pour gestion des taux
	if (m_HItemCur != NULL)
		FormulaireTauxFO.m_pNoeudCur = pNoeud;
	else
		FormulaireTauxFO.m_pNoeudCur = NULL; 

	// Affichage de la boite de dialogue remplissage Taux fin d'ordre noeud courant
	if (FormulaireTauxFO.DoModal() == IDOK)
	{
		m_InxMenuCur = Popup_TauxFO;
	}
	
	return;

}

// Test si utilisateur autorisé à gérer les taux fin d'ordre
bool CNomenclatureView::UtilisateurTauxOk()
{
	CFormulaireUtilTaux FormulaireUtilTaux;
	CString ExMotDePasse;
	int NoUtilisateur;

	// Si utilisateur déjà vérifié, on passe
	if (theApp.m_UtilTauxCur != "") return true;

	// Demande nom utilisateur et mot de passe
	FormulaireUtilTaux.m_MajUtilTaux = false;
	if (FormulaireUtilTaux.DoModal() == IDOK)
	{
		// Mot de passe ne doit pas dépasser 6 caractères
		if (FormulaireUtilTaux.m_MotDePasseUtilTaux.GetLength() > 6)
		{
			AfxMessageBox("Mot de passe trop long (6 caractères maxi.) !!!!");
			return false;
		}

		if (UtilisateurDejaPresent(FormulaireUtilTaux.m_NomUtilTaux,ExMotDePasse,NoUtilisateur) && ExMotDePasse.Compare(FormulaireUtilTaux.m_MotDePasseUtilTaux) == 0)
		{
			// Affectation nouvel utilisateur maj taux FO
			theApp.m_UtilTauxCur = FormulaireUtilTaux.m_NomUtilTaux;
			return true;
		}
		else
			return false;
	}
	else
		// Annulation accés maj taux
		return false;

}

// Test si utilisateur présent dans liste des utilisateurs possibles
bool CNomenclatureView::UtilisateurDejaPresent(CString Nom,CString &ExMotDePasse,int &NoUtilisateur)
{
	CString NomUtil;
	CString Str;
	CHAR Buffer[255];
	Buffer[254] = 0;
	CString BufTmp;
	int NbUtilisateur; 
	int PosSep;
	int i;

	// Lecture nombre d'utilisateurs déjà présent
	GetPrivateProfileString ("Utilisateur", "NbUtil","0", Buffer, 255,theApp.m_PathNomencl + "UtilTFO.ini");
	NbUtilisateur = atoi(Buffer);
	
	// Aucun utilisateur présent
	if (NbUtilisateur == 0) return false; 

	// Test sur tous les utilisateurs
	for (i=1;i<=NbUtilisateur;i++)
	{
		Str.Format("%s%d","Util",i);
		GetPrivateProfileString ("Utilisateur", Str,"0", Buffer, 255,theApp.m_PathNomencl + "UtilTFO.ini");
		BufTmp = Buffer;
		PosSep = BufTmp.Find(",",0);
		if (PosSep != 0)
		{
			NomUtil = BufTmp.Left(PosSep);
			ExMotDePasse = BufTmp.Mid(PosSep+1);
			NoUtilisateur = i;
		}
		if (NomUtil.Compare(Nom) == 0) return true;
	}

	return false;
}




void CNomenclatureView::OnUtiltaux() 
{
	// Gestionnaire des utilisateurs autorisés à ma mise à jour des taux Fin D'Ordre
	CFormulaireUtilTaux FormulaireUtilTaux;

	FormulaireUtilTaux.m_MajUtilTaux = true;

	

	if (FormulaireUtilTaux.DoModal() == IDOK)
	{
		return;
	}
}

void CNomenclatureView::OnUpdateUtiltaux(CCmdUI* pCmdUI) 
{
	// Si administrateur accés autorisé à gestion des utilisateurs taux FO
	if (theApp.m_NiveauAcces >= 2)
		// Acces possible au ss-menu création/modif utilisateurs taux FO
		pCmdUI->Enable(TRUE);
	else
		pCmdUI->Enable(FALSE);
	
}

///////////////////////////////////////////////////////////////////////////////////////////////
// Gestion de centrales d'achat
void CNomenclatureView::OnCentralesdachat() 
{
	// Affichage dialogue gestion Tiers EDI
	CDialogTiersEDI DialogTiersEDI;
	DialogTiersEDI.m_TitreFenTiersEDI = "Gestion des centrales d'achat"; 
	DialogTiersEDI.m_TypeTiers = "PO";

	if (DialogTiersEDI.DoModal() == IDOK)
	{
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////
// Gestion des régies
void CNomenclatureView::OnRegies() 
{

	// Affichage dialogue gestion Tiers EDI
	CDialogTiersEDI DialogTiersEDI;
	DialogTiersEDI.m_TitreFenTiersEDI = "Gestion des régies";
	DialogTiersEDI.m_TypeTiers = "DS";

	if (DialogTiersEDI.DoModal() == IDOK)
	{
	}
}

// Routine partucilère récupération données Nomenclature et mise au format EDI
bool CNomenclatureView::NomenclatureFormatEDI(CString &FicAnnonceur)
{

	// Lecture du fichier nomenclature et stockage lignes dans tableau temporaire
	CArray <CString,CString&> TabLignes;
	CStdioFile File;
	CString FicProduit;
	CString Ligne;
	bool FormatEDI		= false;

	// Ouverture de fichier en mode lecture
	if (File.Open(FicAnnonceur,CFile::modeRead,NULL))
	{
		if (File.ReadString(Ligne) == true)
		{
			// Voir si fichier déjà formatté au format EDI
			if (Ligne.Left(10) == "#NoVersion")
				FormatEDI= true;
		}
		else
		{
			// Ici problème lecture 1ere ligne fichier
			AfxMessageBox("Problème lecture fichier produit.arb <NomenclatureFormatEDI()>",MB_ICONEXCLAMATION);
			File.Close();
			return false;
		}
	}
	// Fermeture du fichier
	File.Close();

	if (FormatEDI == false)
	{
		// Ouverture de fichier en mode lecture
		if (File.Open(FicAnnonceur,CFile::modeRead,NULL))
		{

			// stockage de toutes les lignes
			TabLignes.RemoveAll(); 
			int NbLigne = 0;
			while (File.ReadString(Ligne) == TRUE)
			{
				TabLignes.Add(Ligne);
				NbLigne++;
			}

			// Fermeture du fichier
			File.Close();
		}

		// Puis on réécrit le fichier au nouveau format
		if (File.Open(FicAnnonceur,CFile::modeWrite,NULL))
		{
			// Ecriture des 2 1ere lignes
			File.WriteString("#NoVersion 01  // Description arborescence Groupe //\n");
			File.WriteString("#ANNONCEUR\n");

			// Ecriture des infos nomenclature 
			int IndexCur = 2;
			Ligne = TabLignes[2];
			int NbGroupe = atoi(Ligne.Mid(1));

			Ligne.Format("#%d\n",NbGroupe);
			File.WriteString(Ligne);
			IndexCur++;
			
			// balayage  de tous les groupes
			for (int Groupe=0;Groupe<NbGroupe;Groupe++)
			{
				// Réécriture des 7 lignes groupes
				for (int i=0;i<7;i++)
				{
					Ligne.Format("%s\n",TabLignes[IndexCur]);
					File.WriteString(Ligne);	
					IndexCur++;
				}

				// Nombre élmt annonceur
				Ligne = TabLignes[IndexCur];
				int NbAnnonceur = atoi(Ligne.Mid(1));

				Ligne.Format("#%d\n",NbAnnonceur);
				File.WriteString(Ligne);
				IndexCur++;

				// balayage de tous les annonceurs
				for (int Annonceur=0;Annonceur<NbAnnonceur;Annonceur++)
				{
					CString NomAnnonceur;

					// Réécriture des 9 lignes format précédent pour annonceur
					for (int j=0;j<9;j++)
					{
						// Récup nom de l'annonceur
						if (j==1) NomAnnonceur = TabLignes[IndexCur];
						Ligne.Format("%s\n",TabLignes[IndexCur]);
						File.WriteString(Ligne);	
						IndexCur++;
					}
					
					// Ajoute les nouvelles lignes annonceurs (14 lignes suppl)
					/*
					9a/ (*) Type du tiers EDI (ici toujours BY pour Annonceur)
					9b/ (*) Code du tiers EDI (en France Code SIRET ou International)
					9c/ (*) Type de code tiers > toujours 107
					9d/ (*) Nom du tiers(= Nom annonceur / info redondante)
					9e/ Adresse ligne 1 de l'annonceur
					9f/ Adresse ligne 2 de l'annonceur
					9g/ Code postal de l'annonceur
					9h/ Ville de l'annonceur
					9i/ Code pays de l'annonceur
					9j/ N° téléphone de l'annonceur
					9k/ N° de fax de l'annonceur
					9l/ (*) Nom contact interlocuteur pour cet annonceur
					9m/ (*) Prénom contact interlocuteur
					9n/ (*) Adresse email contact interlocuteur
					*/

					// type du tiers
					Ligne.Format("%s\n","BY");
					File.WriteString(Ligne);	
											
					// Code tiers (rien pour le moment
					Ligne.Format("%s\n","xxx");
					File.WriteString(Ligne);	
					
					// Type de code tiers
					Ligne.Format("%s\n","107");
					File.WriteString(Ligne);	
											
					// Nom annonceur, le même que celui trouvé en position 2 annonceurs
					Ligne.Format("%s\n",NomAnnonceur);
					File.WriteString(Ligne);	
					
					// Adresse 1
					Ligne.Format("%s\n","xxx");
					File.WriteString(Ligne);	
					
					// Adresse 2
					Ligne.Format("%s\n","xxx");
					File.WriteString(Ligne);	
					
					// Code postal
					Ligne.Format("%s\n","xxx");
					File.WriteString(Ligne);	
					
					// Ville
					Ligne.Format("%s\n","xxx");
					File.WriteString(Ligne);	
					
					// Code Pays
					Ligne.Format("%s\n","xxx");
					File.WriteString(Ligne);	
					
					// N° Tel
					Ligne.Format("%s\n","xxx");
					File.WriteString(Ligne);	
					
					// N° Fax
					Ligne.Format("%s\n","xxx");
					File.WriteString(Ligne);	
					
					// Nom interlocuteur
					Ligne.Format("%s\n","xxx");
					File.WriteString(Ligne);	
					
					// Prénom Interlocuteur
					Ligne.Format("%s\n","xxx");
					File.WriteString(Ligne);	
					
					// Adresse mail
					Ligne.Format("%s\n","xxx");
					File.WriteString(Ligne);	
					
					// Lecture Nb Produits
					Ligne = TabLignes[IndexCur];
					int NbProduit = atoi(Ligne.Mid(1));

					Ligne.Format("#%d\n",NbProduit);
					File.WriteString(Ligne);
					IndexCur++;

					// balayage et écriture des infos produit
					for (int Produit=0;Produit<NbProduit;Produit++)
					{
						// écriture des 9 infos produit
						for (int k=0;k<9;k++)
						{
							Ligne.Format("%s\n",TabLignes[IndexCur]);
							File.WriteString(Ligne);	
							IndexCur++;
						}

						// puis #0 à chaque fin de produit
						Ligne.Format("%s\n","#0");
						File.WriteString(Ligne);	
						IndexCur++;
					}
				}
			}

			// fermeture du fichier
			File.Close();
		}

	}

	return true;
}
