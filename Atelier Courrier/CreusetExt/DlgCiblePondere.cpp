// DlgCiblePondere.cpp : fichier d'implémentation
//

#include "stdafx.h"
#include "DlgCiblePondere.h"
#include "TerrainManager.h"
#include "ItemsConstruitsManager.h"
#include ".\dlgciblepondere.h"

// Boîte de dialogue CDlgCiblePondere

IMPLEMENT_DYNAMIC(CDlgCiblePondere, CDialog)
CDlgCiblePondere::CDlgCiblePondere(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgCiblePondere::IDD, pParent)
	, m_GridPonder()

{
	// Init Item de base
	m_ItemBase		= 0;

	// Item pondéré sélectionné
	m_ItemPondere	= 0;

	// Init du map des composants avec leurs coeffs
	m_MapNoLigCompoCoeff.Reset();

	// Par défaut on se met en mode création
	m_ModeCiblePond = MODE_CREATION;

}

CDlgCiblePondere::~CDlgCiblePondere()
{
}

///////////////////////////////////////////////////////////////////////////////////
// Positionne l'offre avec les éléments cible
void CDlgCiblePondere::SetData(JATPDocCreuset * pDocCreuset)
{
	// Récupération du document
	m_pDoc = pDocCreuset;
}


void CDlgCiblePondere::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	// DDX_Control(pDX, IDC_CR_FERMER, m_Btn_FermerCiblePond);
}


BEGIN_MESSAGE_MAP(CDlgCiblePondere, CDialog)
	ON_LBN_SELCHANGE(IDC_CR_LST_COMPOSANTS, OnLbnSelchangeCrLstComposants)
	ON_LBN_SELCHANGE(IDC_CR_LST_ITEMS,		OnLbnSelchangeCrLstItems)
	ON_BN_CLICKED(IDC_CR_VALIDER,			OnBnClickedCrValider)
	ON_BN_CLICKED(IDC_CR_FERMER,			OnBnClickedCrFermer)
	ON_BN_CLICKED(IDC_CR_CREER_CIBLEPOND,	OnBnClickedCrCreerCiblepond)
	ON_BN_CLICKED(IDC_CR_MODIF_CIBLEPOND,	OnBnClickedCrModifCiblepond)
	ON_BN_CLICKED(IDC_CR_SUPPRIMER,			OnBnClickedCrSupprimer)
	ON_LBN_SELCHANGE(IDC_CR_LST_CIBLEPOND,  OnLbnSelchangeCrLstCiblepond)
	ON_WM_SHOWWINDOW()
	ON_WM_CLOSE()
	
END_MESSAGE_MAP()


BOOL CDlgCiblePondere::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Passe au grid la fenêtre parent
	m_GridPonder.SetDlgParent(this);

	// Mise en place des controles
	SetupControls();

	// Le terrain par défaut
	m_IdTerrainDef = 0;
	m_pDoc->m_Offre.m_TBLIdTerrainsAudience.MoveFirst();
	if (m_pDoc->m_Offre.m_TBLIdTerrainsAudience.IsValid())
	{
		// Récup ident terrain par défaut
		m_IdTerrainDef  = m_pDoc->m_Offre.m_TBLIdTerrainsAudience.GetItem();
	}
	
	// Charge les informations
	Init();

	// Active le mode courant (Création ou modification)
	UpdateMode();

	return TRUE;  // return TRUE unless you set the focus to a control
}

///////////////////////////////////////////////////////////////////////////////////////////////////////
// Mise en place des controles
void CDlgCiblePondere::SetupControls()
{
	// Les ressources
	m_Bk_CiblePond.			SubclassDlgItem(IDC_CR_BK_CIBLEPOND,	this);

	m_Btn_CreerCible.		SubclassDlgItem(IDC_CR_CREER_CIBLEPOND, this);
	m_Btn_ModifCible.		SubclassDlgItem(IDC_CR_MODIF_CIBLEPOND, this);
	m_Lst_Items.			SubclassDlgItem(IDC_CR_LST_ITEMS,		this);
	m_Titre_ListeItems.		SubclassDlgItem(IDC_CR_TITRE_LIST1,		this);

	m_Lst_CiblesPonds.		SubclassDlgItem(IDC_CR_LST_CIBLEPOND,	this);

	m_Lst_Composants.		SubclassDlgItem(IDC_CR_LST_COMPOSANTS,  this);
	m_Titre_Composants.		SubclassDlgItem(IDC_CR_TITRE_COMPOS,	this);

	m_Titre_CiblePonderee.	SubclassDlgItem(IDC_CR_TITRE_CIBLEPOND,	this);
	m_Edit_CiblePond.		SubclassDlgItem(IDC_CR_EDIT_CIBLEPOND,  this);

	m_Titre_Ponderation.	SubclassDlgItem(IDC_CR_TITRE_PONDERATION,	this);

	// Les infos compléments
	m_Titre_Complement.		SubclassDlgItem(IDC_CR_TITRE_COMPLEMENT, this);
	m_Prct_Compl_Reel.		SubclassDlgItem(IDC_CR_PRCT_COMPL_REEL,  this);
	m_Prct_Compl_Pond.		SubclassDlgItem(IDC_CR_PRCT_COMPL_POND,  this);

	// Bouton validation
	m_Btn_ValidCiblePond.	SubclassDlgItem(IDC_CR_VALIDER,			 this);

	// Bouton fermeture
	m_Btn_FermerCiblePond.	SubclassDlgItem(IDC_CR_FERMER,			 this);

	// Bouton suppression cible pondérée
	m_Btn_SuppCiblePond.	SubclassDlgItem(IDC_CR_SUPPRIMER,	 this);

	// Initialisation du grid pondérations
	m_GridPonder.			SubclassDlgItem(IDC_CR_GRID_PONDER,		 this);
	m_GridPonder.Initialize(); 

	// Les types d'onglet (par défaut c'est la création d'une cible pondérée)
	m_Btn_CreerCible.	SetTypeOnglet(TRUE, TRUE, TRUE); 
	m_Btn_CreerCible.	BringWindowToTop();
	m_Btn_CreerCible.	RedrawWindow(); 

	// Onglet Modif Cible non actif
	m_Btn_ModifCible.	SetTypeOnglet(TRUE, FALSE, TRUE); 

	// Propriétés du background
	m_Bk_CiblePond.		SetTransparent(false);
	m_Bk_CiblePond.		SetSunken(true);

	// Ajout des contrôles au fond de la boite de dialogue
	m_Bk_CiblePond.		AddTopWindow(m_Titre_ListeItems);
	m_Bk_CiblePond.		AddTopWindow(m_Lst_Items);
	m_Bk_CiblePond.		AddTopWindow(m_Lst_CiblesPonds);
	m_Bk_CiblePond.		AddTopWindow(m_Btn_CreerCible);
	m_Bk_CiblePond.		AddTopWindow(m_Btn_ModifCible);
	m_Bk_CiblePond.		AddTopWindow(m_Lst_Composants);
	m_Bk_CiblePond.		AddTopWindow(m_Titre_Composants);
	m_Bk_CiblePond.		AddTopWindow(m_Titre_CiblePonderee);
	m_Bk_CiblePond.		AddTopWindow(m_Edit_CiblePond);
	m_Bk_CiblePond.		AddTopWindow(m_Titre_Ponderation);
	m_Bk_CiblePond.		AddTopWindow(m_GridPonder);
	m_Bk_CiblePond.		AddTopWindow(m_Titre_Complement);
	m_Bk_CiblePond.		AddTopWindow(m_Prct_Compl_Reel);
	m_Bk_CiblePond.		AddTopWindow(m_Prct_Compl_Pond);
	m_Bk_CiblePond.		AddTopWindow(m_Btn_ValidCiblePond);
	m_Bk_CiblePond.		AddTopWindow(m_Btn_FermerCiblePond);
	m_Bk_CiblePond.		AddTopWindow(m_Btn_SuppCiblePond);

	// Les tooltips par controle
	/* A FAIRE
	CString ToolTip;
	ToolTip.LoadString(IDS_RI_TT_QUESTION); 
	*/


	// Positionne par défaut la création de cible
	CheckRadioButton(IDC_CR_CREER_CIBLEPOND, IDC_CR_MODIF_CIBLEPOND, IDC_CR_CREER_CIBLEPOND);

	// Initialise les couleurs des contrôles
	SetColors();
}

/////////////////////////////////////////////////////////////////////////////////////////
// Couleurs des contrôles
void CDlgCiblePondere::SetColors()
{
	// Le background
	m_Bk_CiblePond.			SetBkColor(CATPColors::GetColorMedium(CATPColors::COLORCREUSET),CATPColors::COLORCREUSET,CATPColors::MEDIUM_DEGRAD);

	// Couleur des titres
	m_Titre_ListeItems.		SetBkColor(CATPColors::GetColorDark(CATPColors::COLORCREUSET),CATPColors::COLORCREUSET,CATPColors::DARK_DEGRAD);
	m_Titre_ListeItems.		SetTextColor(CATPColors::GetColorSuperDark(CATPColors::COLORCREUSET));

	m_Titre_Composants.		SetBkColor(CATPColors::GetColorDark(CATPColors::COLORCREUSET),CATPColors::COLORCREUSET,CATPColors::DARK_DEGRAD);
	m_Titre_Composants.		SetTextColor(CATPColors::GetColorSuperDark(CATPColors::COLORCREUSET));

	m_Titre_CiblePonderee.	SetBkColor(CATPColors::GetColorDark(CATPColors::COLORCREUSET),CATPColors::COLORCREUSET,CATPColors::DARK_DEGRAD);
	m_Titre_CiblePonderee.	SetTextColor(CATPColors::GetColorSuperDark(CATPColors::COLORCREUSET));

	m_Titre_Ponderation.	SetBkColor(CATPColors::GetColorDark(CATPColors::COLORCREUSET),CATPColors::COLORCREUSET,CATPColors::DARK_DEGRAD);
	m_Titre_Ponderation.	SetTextColor(CATPColors::GetColorSuperDark(CATPColors::COLORCREUSET));

	// Couleur des onglets sélection créer ou modifier cible pondérée
	m_Btn_CreerCible.		SetColor(CButtonST::BTNST_COLOR_BK_OUT,		CATPColors::GetColorLight(CATPColors::COLORCREUSET),CATPColors::COLORCREUSET,0);
	m_Btn_CreerCible.		SetColor(CButtonST::BTNST_COLOR_BK_IN,		CATPColors::GetColorMedium(CATPColors::COLORCREUSET),CATPColors::COLORCREUSET);
	m_Btn_CreerCible.		SetColor(CButtonST::BTNST_COLOR_BK_FOCUS,	CATPColors::GetColorDark(CATPColors::COLORCREUSET),CATPColors::COLORCREUSET);

	m_Btn_ModifCible.		SetColor(CButtonST::BTNST_COLOR_BK_OUT,		CATPColors::GetColorLight(CATPColors::COLORCREUSET),CATPColors::COLORCREUSET,0);
	m_Btn_ModifCible.		SetColor(CButtonST::BTNST_COLOR_BK_IN,		CATPColors::GetColorMedium(CATPColors::COLORCREUSET),CATPColors::COLORCREUSET);
	m_Btn_ModifCible.		SetColor(CButtonST::BTNST_COLOR_BK_FOCUS,	CATPColors::GetColorDark(CATPColors::COLORCREUSET),CATPColors::COLORCREUSET);

	// Couleur de fond des listes de sélection
	m_Lst_Composants.SetBackColor(CATPColors::GetColorMedium(CATPColors::COLORCREUSET));

	// Couleur des infso compléments
	m_Titre_Complement.		SetTextColor(CATPColors::GetColorRed());
	m_Prct_Compl_Reel.		SetTextColor(CATPColors::GetColorRed());
	m_Prct_Compl_Pond.		SetTextColor(CATPColors::GetColorRed());

	// Couleur boutons de commande
	m_Btn_ValidCiblePond.	SetColor(CButtonST::BTNST_COLOR_BK_OUT,		CATPColors::GetColorLight(CATPColors::COLORCREUSET),CATPColors::COLORCREUSET,0);
	m_Btn_ValidCiblePond.	SetColor(CButtonST::BTNST_COLOR_BK_IN,		CATPColors::GetColorMedium(CATPColors::COLORCREUSET),CATPColors::COLORCREUSET);
	m_Btn_ValidCiblePond.	SetColor(CButtonST::BTNST_COLOR_BK_FOCUS,	CATPColors::GetColorDark(CATPColors::COLORCREUSET),CATPColors::COLORCREUSET);

	// Couleur boutons de commande
	m_Btn_FermerCiblePond.	SetColor(CButtonST::BTNST_COLOR_BK_OUT,		CATPColors::GetColorLight(CATPColors::COLORCREUSET),CATPColors::COLORCREUSET,0);
	m_Btn_FermerCiblePond.	SetColor(CButtonST::BTNST_COLOR_BK_IN,		CATPColors::GetColorMedium(CATPColors::COLORCREUSET),CATPColors::COLORCREUSET);
	m_Btn_FermerCiblePond.	SetColor(CButtonST::BTNST_COLOR_BK_FOCUS,	CATPColors::GetColorDark(CATPColors::COLORCREUSET),CATPColors::COLORCREUSET);

	// Bouton en relief
	m_Btn_ValidCiblePond.SetFlat(FALSE);
	m_Btn_FermerCiblePond.SetFlat(FALSE);

	// Couleur bouton suppression cible pondérée
	m_Btn_SuppCiblePond.	SetColor(CButtonST::BTNST_COLOR_BK_OUT,		CATPColors::GetColorLight(CATPColors::COLORCREUSET),CATPColors::COLORCREUSET,0);
	m_Btn_SuppCiblePond.	SetColor(CButtonST::BTNST_COLOR_BK_IN,		CATPColors::GetColorMedium(CATPColors::COLORCREUSET),CATPColors::COLORCREUSET);
	m_Btn_SuppCiblePond.	SetColor(CButtonST::BTNST_COLOR_BK_FOCUS,	CATPColors::GetColorDark(CATPColors::COLORCREUSET),CATPColors::COLORCREUSET);

	// Bouton en relief
	m_Btn_SuppCiblePond.SetFlat(FALSE);
}

//////////////////////////////////////////////////////////////////////////////////////////
JVoid CDlgCiblePondere::Init()
{
	// Init liste des terrains
	m_LstTerrains.Reset(); 

	// Récupère les terrains
	for(m_pDoc->m_Offre.m_TBLIdTerrainsAudience.MoveFirst(); 
		m_pDoc->m_Offre.m_TBLIdTerrainsAudience.IsValid(); 
		m_pDoc->m_Offre.m_TBLIdTerrainsAudience.MoveNext())
	{
		// Ajoute le terrain
		CKeyTerrain KeyTerrain(m_pDoc->m_Offre.m_IdSourceAudience, m_pDoc->m_Offre.m_TBLIdTerrainsAudience.GetItem());
		m_LstTerrains.AddTail() = CTerrainManager::GetInstance()->GetTerrain(KeyTerrain);
	}
}	

//////////////////////////////////////////////////////////////////////////////////////////
// Chargement des items disponibles et des composants
JBool CDlgCiblePondere::LoadItems()
{
	// Init liste des items
	m_Lst_Items.ResetContent();
	m_Lst_Composants.ResetContent();

	// Récup les items
	CItemsConstruitsBase * pItemsConstruits = CItemsConstruitsManager::GetInstance()->GetItemsInstance();

	// Chargement des items existants selon les terrains
	JList<IItem const *> vItems;
	if (m_LstTerrains.GetCount())
	{
		// Récupère les items indépendants (sans les cibles pondérées)
		pItemsConstruits->GetItemsIndependant(vItems, m_LstTerrains);

		// Boucle sur tous les items
		for (vItems.MoveFirst(); vItems.IsValid(); vItems.MoveNext())
		{
			// Récupère l'item
			IItem * pItem = (IItem*)vItems.GetItem();

			// Ident de l'item
			JInt32 IdItem		= pItem->GetID();

			if (pItem->IsItemConstruit())
			{
				// Label de l'item
				JLabel LabelItem	= vItems.GetItem()->GetLabel(); 

				// On l'ajoute à la liste des items disponibles (sélection item de base)
				JUnt32 IdxItem = m_Lst_Items.AddString(LabelItem.AsJCharPtr());

				// On récupère l'item data
				m_Lst_Items.SetItemData(IdxItem, IdItem); 	

				// Puis on l'ajoute à la liste des composants à sélectionner
				JUnt32 IdxCompo = m_Lst_Composants.AddString(LabelItem.AsJCharPtr());

				// On récupère l'item data
				m_Lst_Composants.SetItemData(IdxCompo, IdItem); 	
			}
		}

		// Ok chargement
		return true;
	}
	else
	{
		// Problème chargement des items
		AfxMessageBox("Problème chargement des cibles pondérées !!!", MB_ICONINFORMATION);
		return false;
	}
}

/////////////////////////////////////////////////////////////////////////////////////////
// Charge les dernières cibles pondérées
JBool CDlgCiblePondere::LoadCiblesPonds()
{
	// Init liste des cibles pondérées
	m_Lst_CiblesPonds.ResetContent();

	// Récup les items
	// CItemsConstruits * m_pItems = CItemsConstruits::GetInstance();

	// Chargement des items existants selon les terrains
	JList<IItem const *> vItems;
	if (m_LstTerrains.GetCount())
	{
		// Récupère les items indépendants (sans les cibles pondérées)
		// m_pItems->GetItemsPonderes(vItems, m_LstTerrains);
		CItemsConstruitsManager::GetInstance()->GetItemsInstance()->GetItemsPonderes(vItems, m_LstTerrains);

		JInt32 Nb = vItems.GetCount(); 

		// Boucle sur tous les items
		for (vItems.MoveFirst(); vItems.IsValid(); vItems.MoveNext())
		{
			// Récupère l'item
			IItem * pItem = (IItem*)vItems.GetItem();

			// Ident de l'item
			JInt32 IdItem		= pItem->GetID();

			if (pItem->IsItemPondere())
			{
				// Label de l'item
				JLabel LabelItem	= vItems.GetItem()->GetLabel(); 

				// On l'ajoute à la liste des cibles pondérées existantes
				JUnt32 IdxItem = m_Lst_CiblesPonds.AddString(LabelItem.AsJCharPtr());

				// On récupère l'item data
				m_Lst_CiblesPonds.SetItemData(IdxItem, IdItem); 	

				// On déselectionne les composants
				DeselectAllCompos();
			}
		}

		// Ok chargement des cibles pondérées existantes
		return true;
	}
	else
	{
		// Problème chargement des items
		AfxMessageBox("Problème chargement cibles pondérées !!!", MB_ICONINFORMATION);
		return false;
	}
}

/////////////////////////////////////////////////////////////////////////////////////////
// Validité de la sélection des composants
// (Ils doivent être indépendants, afin que les coefficients de pondération aient un sens)
JBool CDlgCiblePondere::SelItemsComposOK()
{
	// Les items
	CItemsConstruitsBase * pItems = CItemsConstruitsManager::GetInstance()->GetItemsInstance();

	// Récupère les éléments sélectionnées
	int NbSel = m_Lst_Composants.GetSelCount();
	int  *pSel = new int[NbSel];
	m_Lst_Composants.GetSelItems(NbSel,pSel);   

	for(m_pDoc->m_Offre.m_TBLIdTerrainsAudience.MoveFirst(); m_pDoc->m_Offre.m_TBLIdTerrainsAudience.IsValid(); m_pDoc->m_Offre.m_TBLIdTerrainsAudience.MoveNext())
	{
		// Récup ident terrain
		JInt32 IdTerrain = m_pDoc->m_Offre.m_TBLIdTerrainsAudience.GetItem();

		// Liste des vecteurs d'items
		JList <const CBitArray *> LstVectItemsD;
		JList <const CBitArray *> LstVectItemsG;

		// Boucle sur les items sélectionnés
		for(int i = 0; i < NbSel ; ++i)
		{
			// Récupère l'ident item
			JInt32 IdItem = m_Lst_Composants.GetItemData(pSel[i]); 
	
			// Récupère l'item
			CItemConstruit * pItem = pItems->GetIItemByID(IdItem)->AsItemConstruit();
			
			// Paramétrage calcul items
			CCalcItemParams Params;

			// Récupère le vecteur de l'item sur ce terrain
			const CBitArray * pBitArrayCible = 0;
			GetVectItem(pItem, IdTerrain, Params);
			pBitArrayCible = Params.GetResults();

			// Récup vecteur de droite
			CBitArray * pResultD = new CBitArray(* pBitArrayCible);
			CBitArray * pResultG = new CBitArray(* pBitArrayCible);

			// Ajout vecteur item à la liste des vecteurs items droite et gauche
			LstVectItemsD.AddTail() = pResultD; //Params.GetResults(); //pBitArrayCible;  
			LstVectItemsG.AddTail() = pResultG;//Params.GetResults(); //pBitArrayCible;
		}

		// Test l'indépendance des vecteurs  (tester les vecteurs 2 à 2)
		if (LstVectItemsD.GetCount()  > 1)
		{
			// Balaye les Bits Arrays de droite
			JInt32 InxBitArrayD = 0;
			for (LstVectItemsD.MoveFirst(); LstVectItemsD.IsValid(); LstVectItemsD.MoveNext())
			{
				// On prend le 1er vecteur
				const CBitArray * pTotBitArray = LstVectItemsD.GetItem();
				
				// Balaye les Bits Arrays de gauche
				JInt32 InxBitArrayG = 0;
				for (LstVectItemsG.MoveFirst(); LstVectItemsG.IsValid(); LstVectItemsG.MoveNext())
				{
					// On ne teste pas les 2 mêmes vecteurs
					if (InxBitArrayD != InxBitArrayG)
					{
						// Récup vecteur de droite
						CBitArray * pResultante = new CBitArray(* pTotBitArray);

						// Récupère le vecteur suivant
						const CBitArray * pBitArray = LstVectItemsG.GetItem();

						// Combine les vecteurs
						*pResultante &= *pBitArray;

						// Vérifie si les 2 vecteurs sont indépendants
						JInt32 NbIndiv = 0;

						// Vérifie si au moins 1 individu en dépendance
						for (int x = 0; x < pResultante->GetSize(); x++)
						{
							JBool ValByte = pResultante->GetAt(x);
							if (ValByte)
							{
								NbIndiv++;
								break;
							}
						}

						// On le détruit le vecteur résultante après vérification non dépendance
						delete pResultante;
						
						// Si au moins 1 indiv commun, vecteurs non indépendants
						if (NbIndiv > 0)
						{
							// Delete les Bit Array Listes tempos
							LstVectItemsG.MoveFirst(); 
							for (LstVectItemsD.MoveFirst(); LstVectItemsD.IsValid(); LstVectItemsD.MoveNext(), LstVectItemsG.MoveNext() )
							{
								delete LstVectItemsD.GetItem();
								delete LstVectItemsG.GetItem();
							}
							delete [] pSel;

							return false;
						}
						
					}

					// Incrémente n° vecteur gauche visité
					InxBitArrayG++;
				}

				// Incrémente n° vecteur droite visité
				InxBitArrayD++;
			}
		}

		// Delete les Bit Array Listes tempos
		LstVectItemsG.MoveFirst(); 
		for (LstVectItemsD.MoveFirst(); LstVectItemsD.IsValid(); LstVectItemsD.MoveNext(), LstVectItemsG.MoveNext() )
		{
			delete LstVectItemsD.GetItem();
			delete LstVectItemsG.GetItem();
		}

		return true;

	} // End for terrains

	delete [] pSel;
	return false;
}


///////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Récupère vecteur de cible
void CDlgCiblePondere::GetVectItem(CItemConstruit * pItem, JInt32 IdTerrain, CCalcItemParams & Params) // const CBitArray ** pBitArrayCible)
{
	// Récupère les informations du terrain
	CTerrain * pTerrain = CTerrainManager::GetInstance()->GetTerrain(m_pDoc->m_Offre.m_IdSourceAudience, IdTerrain);

	// Créer l'instance de calcul pour l'item
	// CCalcItemParams Params;

	// Positionne paramètres
	Params.SetTerrain(pTerrain);
	Params.SetEchantillonID(1);
	Params.SetUniversID(pTerrain->GetDefaultUnivers());
		
	// Récup table des modulations
	const JMTFR03TBLModulations	 & TblModulations   = m_pDoc->m_MoteurPresse.GetTblModulations();

	// Positionne les infos poids
	JMTFR03KEYModulation keyMod;
	keyMod.m_IdSource	= m_pDoc->m_Offre.m_IdSourceAudience;
	keyMod.m_IdTerrain	= IdTerrain;
	TblModulations.MoveTo(keyMod);
	const JMTFR03DATModulation & DatMod = TblModulations.GetItem();

	JList<JInt32> lstSegments;
	for(int i = 0; i<DatMod.m_FLTSegments.GetCount();i++)
	{
		if(DatMod.m_FLTSegments.GetSelItem(i))
			lstSegments.AddTail() = DatMod.m_FLTSegments.Item(i).AsJInt32();
	}

	Params.SetSegmentsIDs(lstSegments);
	CBitArray bArrPopSeg;
	JUnt32 contingent =  0xFFFF;

	//TODO recuperer le bon segment
	pTerrain->GetDefaultPopSegment(bArrPopSeg, contingent);
	Params.SetPopulationSegment(bArrPopSeg);
	
	// Pour le type de poids
	JInt32 IdTypePoids = DatMod.m_FLTPoids.Item(DatMod.m_FLTPoids.GetSelItem()).AsJInt32();
	if (!pTerrain->HasTypePoids(IdTypePoids))
		IdTypePoids = pTerrain->GetDefaultTypePoidsID() ;

	// Et l'univers
	JInt32 IdUnivers;
	IdUnivers = pTerrain->GetDefaultUnivers();

	// Set le type de poids
	Params.SetTypePoidsID(IdTypePoids);
		
	// Calcul du vecteur item
	pItem->Calculate(&Params);
}
	
///////////////////////////////////////////////////////////////////////////////////////////
// Sauvegarde de l'item pondéré
JBool CDlgCiblePondere::SaveItemPondere()
{
	// L'item de base doit être défini, ainsi qu'au moins 1 composant pondéré
	if (m_ItemBase > 0 && m_MapNoLigCompoCoeff.GetCount() >= 1)
	{
		// Les items
		CItemsConstruitsBase * pItems = CItemsConstruitsManager::GetInstance()->GetItemsInstance();

		// Créer un nouvel item construit
		CItemPondere * pItemPondere = (CItemPondere *)pItems->GetNewItemPondere();

		// Avec le 1er terrain
		m_pDoc->m_Offre.m_TBLIdTerrainsAudience.MoveFirst(); 
		if (m_pDoc->m_Offre.m_TBLIdTerrainsAudience.IsValid())
		{
			// Ajoute le terrain
			CKeyTerrain KeyTerrain(m_pDoc->m_Offre.m_IdSourceAudience, m_pDoc->m_Offre.m_TBLIdTerrainsAudience.GetItem());
			CTerrain * pTerrain = CTerrainManager::GetInstance()->GetTerrain(KeyTerrain);

			// Récup builder objet de l'item de base
			CItemConstruit * pItemConstruit = pItems->GetIItemByID(m_ItemBase)->AsItemConstruit(); 
			
			// Récup Item Builder
			CIBuilder IBuilder;
			pItemConstruit->GetItemBuilder(pTerrain, IBuilder); 

			// Sauve le libellé de l'item pondéré
			pItemPondere->SetLabel("Item pondere"); 

			// Ajout l'item de base
			pItemPondere->AddItemBase(pItemConstruit); 
	
			// Ajout des composants et de leurs coefficients
			for (m_MapNoLigCompoCoeff.MoveFirst(); m_MapNoLigCompoCoeff.IsValid(); m_MapNoLigCompoCoeff.MoveNext())
			{
				// Récup ident composant
				JInt32 IdCompo = m_MapNoLigCompoCoeff.GetKey(); 

				// Récup pondération
				CInfoCoeff & InfoCoeff = m_MapNoLigCompoCoeff.GetItem();
				JFlt64 PonderCompo = InfoCoeff.Coeff; 

				// On l'ajoute à la liste des composants de l'item pondéré en cours de construction
				CItemConstruit * pItemCompo = pItems->GetIItemByID(IdCompo)->AsItemConstruit(); 
				pItemPondere->AddItem(pItemCompo, PonderCompo); 
			}
		}

		// Enregistrement de l'item pondéré
		if(!pItemPondere->IsRegistered())
		{
			// Récup le nom de la cible pondérée
			CString TxtCiblePond;
			m_Edit_CiblePond.GetWindowText(TxtCiblePond);
			JLabel LblCiblePond = TxtCiblePond;

			// Test si le nom de la nouvelle cible a bien été saisie
			if (TxtCiblePond != "")
			{
				if (pItemPondere->SetLabel(LblCiblePond))
				{
					const CItemPondere * pData = pItems->RegisterItemPondere(pItemPondere);
					if(pData)
					{
						// Sauve la base items construits
						pItems->Save();
					}
					else
					{
						AfxMessageBox("Impossible d'enregistrer la cible pondérée!",MB_ICONINFORMATION);
						return false;
					}
				}
				else
				{
					AfxMessageBox("Le libellé de la cible pondérée est invalide !!!",MB_ICONINFORMATION);
					return false;
				}
			}
			else
			{
				AfxMessageBox("Le libellé de la cible pondérée est manquant !!!",MB_ICONINFORMATION);
				return false;
			}
		}
		else
		{
			// Init edit box libellé cible pondérée
			m_Edit_CiblePond.SetWindowText("");
		}
	}
	else
	{
		// Problème de sauvegarde de l'item pondéré
		AfxMessageBox("Cible de base non défini, ou aucun composant pondéré, sauvegarde impossible", MB_ICONINFORMATION);
		return false;
	}

	return true;
}

///////////////////////////////////////////////////////////////////////////////////////////
// Modification d'une cible pondérée existante
JBool CDlgCiblePondere::ModifItemPondere()
{
	// L'item de base doit être défini, ainsi qu'au moins 1 composant pondéré
	if (m_ItemBase > 0 && m_MapNoLigCompoCoeff.GetCount() > 1)
	{
		// Les items
		CItemsConstruitsBase * pItems = CItemsConstruitsManager::GetInstance()->GetItemsInstance();

		// Récup l'item pondéré en cours de modification
		CItemPondere * pItemPondere = (CItemPondere *)pItems->GetItemPondere(m_ItemPondere);

		// Récupère les élmts composants
		JList <const CItemConstruit *>	ItemsCompos;
		JList <JFlt64>					PonderItems;
		pItemPondere->GetItems(ItemsCompos, PonderItems); 

		// Boucle sur les composants et modifie les valeurs pondérations
		for (m_MapNoLigCompoCoeff.MoveFirst(); m_MapNoLigCompoCoeff.IsValid(); m_MapNoLigCompoCoeff.MoveNext())
		{
			// Récup ident composant
			JInt32 IdCompo = m_MapNoLigCompoCoeff.GetKey(); 

			// Récup pondération
			CInfoCoeff & InfoNewCoeff = m_MapNoLigCompoCoeff.GetItem();

			// Se poitionne sur l'ancienne info composant
			PonderItems.MoveFirst(); 
			for (ItemsCompos.MoveFirst(); ItemsCompos.IsValid(); ItemsCompos.MoveNext(), PonderItems.MoveNext())
			{
				// Vérifie que c'est le composant en cours
				const CItemConstruit * pItemCompo = ItemsCompos.GetItem();

				JInt32 Id = pItemCompo->GetID();

				if (IdCompo == pItemCompo->GetID())
				{
					// On change son coefficient
					pItemPondere->ModifPonder(IdCompo,InfoNewCoeff.Coeff);
					break;
				}
			}	
		}

		// On sauve
		pItems->Save();
	}
	else
	{
		// Problème de sauvegarde de l'item pondéré
		AfxMessageBox("Cible de base non défini, ou aucun composant pondéré, sauvegarde impossible", MB_ICONINFORMATION);
		return false;
	}

	return true;
}

///////////////////////////////////////////////////////////////////////////////////////////
// Suppression d'une cible pondérée existante
JBool CDlgCiblePondere::SupprimeItemPondere()
{
	if (m_ItemPondere)
	{
		// Les items
		CItemsConstruitsBase * pItems = CItemsConstruitsManager::GetInstance()->GetItemsInstance();

		IItem * pItem = pItems->GetIItemByID(m_ItemPondere);
		if (pItem != 0)
		{
			if (pItem->IsItemPondere())
			{
				// Récup l'item pondéré en cours de modification
				CItemPondere * pItemPondere = (CItemPondere *)pItems->GetItemPondere(m_ItemPondere);

				// Message de prévention
				CString Message;

				// Peut-on vraiment le désenregister ???
				JMap<JInt32, JLabel> MapUpwards;
				if (pItems->CanUnRegisterItemPondere(m_ItemPondere, &MapUpwards))
				{
					Message.Format("Voulez-vous supprimer la cible pondérée \"%s\" ?", pItemPondere->GetLabel().AsJCharPtr());
					if(IDYES != MessageBox(Message, "SUPPRESSION CIBLE PONDEREE", MB_YESNO|MB_ICONQUESTION))
						return false;
				}
				else
				{
					// Affichage Boite de composition de l'item
					Message.Format("La cible pondérée \"%s\" ne peut pas être supprimé.", pItemPondere->GetLabel().AsJCharPtr());
					MessageBox(Message, "SUPPRESSION CIBLE PONDEREE", MB_OK|MB_ICONEXCLAMATION);
					return false;
				}
				
				// Remove the Item from the pseudo question if present
				if (pItemPondere)  //  && pItemPondere->HasItem(m_ItemPondere))
				{
					if (pItems->UnRegisterItemPondere(m_ItemPondere))
					{
						// On sauve
						pItems->Save();

						// Charge les dernières cibles pondérées (si au moins 1 existante)
						UpdateMode();

						// La suppression s'est bien déroulée
						return true;
					}
				}
			}
		}
	}
	else
	{
		// Problème suppression de l'item pondéré
		AfxMessageBox("Suppression impossible", MB_ICONINFORMATION);
		return false;
	}

	return false;
}

////////////////////////////////////////////////////////////////////////////////////////
// Passe en mode création cible pondérée
void CDlgCiblePondere::OnBnClickedCrCreerCiblepond()
{
	m_ModeCiblePond = MODE_CREATION;
	UpdateMode();
}

////////////////////////////////////////////////////////////////////////////////////////
// Passe en mode modification
void CDlgCiblePondere::OnBnClickedCrModifCiblepond()
{
	m_ModeCiblePond = MODE_MODIFICATION;
	UpdateMode();
}

///////////////////////////////////////////////////////////////////////////////////////
// Active le mode création ou modification
void CDlgCiblePondere::UpdateMode()
{
	switch (m_ModeCiblePond)
	{
		case MODE_CREATION:
		{
			// Charge les dernières cibles pondérées (si au moins 1 existante)
			if (LoadItems())
			{
				// Positionne par la création de cible
				CheckRadioButton(IDC_CR_CREER_CIBLEPOND, IDC_CR_MODIF_CIBLEPOND, IDC_CR_CREER_CIBLEPOND);
				m_Btn_CreerCible.SetFocus(); 
				m_Btn_CreerCible.RedrawWindow(); 
				m_Btn_CreerCible.ShowWindow(SW_SHOW); 

				// Cache la liste des cibles pondérées existantes
				// Rend visible la liste des items et cache la liste des cibles pondérés existants
				m_Lst_Items.ShowWindow(SW_SHOW);
				m_Lst_CiblesPonds.ShowWindow(SW_HIDE);

				// Change libellé > Entete cible de base
				m_Titre_CiblePonderee.SetWindowText("Nom de la cible pondérée");

				// Change libellé titre 1ere liste
				m_Titre_ListeItems.SetWindowText("Cible de base");

				// Change libellé 2ème titre
				m_Titre_Composants.SetWindowText("Segments de pondération");

				// Vire toutes les lignes de la grille
				m_GridPonder.DeleteLines(); 

				// Init du map des composants avec leurs coeffs
				m_MapNoLigCompoCoeff.Reset(); 

				// Rend invisible le bouton suppression cible pondérée (inutile ici)
				m_Btn_SuppCiblePond.ShowWindow(SW_HIDE); 
			}

			// Affiche titre fenêtre principale
			this->SetWindowText("Création d'une cible pondérée"); 

			// Vide l'édition nom de la nvlle cible pondérée
			m_Edit_CiblePond.SetWindowText("");

			// On modifie aspect bouton (type onglet actif)
			m_Btn_ModifCible.SetTypeOnglet(TRUE, FALSE,	TRUE);
			m_Btn_ModifCible.SetCheck(BST_UNCHECKED); 

			// Bouton onglet création cible pondérée actif et en 1er plan
			m_Btn_CreerCible.SetTypeOnglet(TRUE, TRUE, TRUE);
			m_Btn_CreerCible.SetCheck(BST_CHECKED); 
			m_Btn_CreerCible.EnableWindow(TRUE);
			m_Btn_CreerCible.BringWindowToTop();
			m_Btn_CreerCible.RedrawWindow(); 
		}
		break;

		case MODE_MODIFICATION:
		{
			// Charge les dernières cibles pondérées (si au moins 1 existante)
			if (LoadCiblesPonds())
			{
				// Positionne la modification création de cible
				CheckRadioButton(IDC_CR_CREER_CIBLEPOND, IDC_CR_MODIF_CIBLEPOND, IDC_CR_MODIF_CIBLEPOND);
				m_Btn_ModifCible.SetFocus();
				m_Btn_ModifCible.RedrawWindow(); 
				m_Btn_ModifCible.ShowWindow(SW_SHOW);

				// Cache la liste des items et rend visible la liste des cibles pondérés existants
				m_Lst_Items.ShowWindow(SW_HIDE);
				m_Lst_CiblesPonds.ShowWindow(SW_SHOW);

				// Rend invisible le bouton suppression cible pondérée (inutile ici)
				m_Btn_SuppCiblePond.ShowWindow(SW_SHOW); 

				// Puis on se positionne sur 1ere cible pondéré si elle existe
				if (m_Lst_CiblesPonds.GetCount())
				{
					// Sélection 1er elmt de la liste
					m_Lst_CiblesPonds.SetCurSel(0);

					// Récup ident item pondéré
					m_ItemPondere = m_Lst_CiblesPonds.GetItemData(0);

					// Et on la visualise (composants et coeffs)
					ActiveCiblePond();
				}
				else
				{
					// Reset liste ds composants utilisés
					m_Lst_Composants.ResetContent();

					// Vire toutes les lignes de a grille
					m_GridPonder.DeleteLines(); 

					// Init nom de la cible de base dans l'édit box
					m_Edit_CiblePond.SetWindowText(""); 
				}

				// Change libellé > Entete cible de base
				m_Titre_CiblePonderee.SetWindowText("Cible de base");


				// Affiche titre fenêtre principale
				this->SetWindowText("Modification d'une cible pondérée"); 

				// Change libellé titre 1ere liste
				m_Titre_ListeItems.SetWindowText("Cible pondérée");

				// Change libellé 2ème titre
				m_Titre_Composants.SetWindowText("Segments de pondération");
			}			

			// On modifie aspect bouton (type onglet actif)
			m_Btn_CreerCible.SetTypeOnglet(TRUE, FALSE,	TRUE);
			m_Btn_CreerCible.SetCheck(BST_UNCHECKED); 
	
			// Bouton onglet modification cible pondérée actif et en 1er plan
			m_Btn_ModifCible.SetTypeOnglet(TRUE, TRUE, TRUE);
			m_Btn_ModifCible.SetCheck(BST_CHECKED);
			m_Btn_ModifCible.EnableWindow(TRUE);
			m_Btn_ModifCible.BringWindowToTop();
			m_Btn_ModifCible.RedrawWindow();
		}
		break;
	}

	// Bloque ou débloque liste des composants (selon le mode)
	m_Lst_Composants.EnableWindow(m_ModeCiblePond == MODE_CREATION); 

	// Et on débloque l'éditeur de nom cible
	m_Edit_CiblePond.EnableWindow(m_ModeCiblePond == MODE_CREATION);

	// Couleur de fond pour la liste active
	m_Lst_Items.		SetBackColor(CATPColors::GetColorLight(CATPColors::COLORCREUSET));
	m_Lst_Composants.	SetBackColor(CATPColors::GetColorLight(CATPColors::COLORCREUSET));
}

////////////////////////////////////////////////////////////////////////////////////////
// Validation de la cible pondérée en cours de création
void CDlgCiblePondere::OnBnClickedCrValider()
{
	// Avant toute chose on vérifie qu'il n'y ait aucun composant dont coeff réél = 0 et coeff composant != 0
	if (CoeffCompoValide())
	{
		// Si c'est une nouvelle cible pondérée
		if (m_ModeCiblePond == MODE_CREATION)
			// Création d'une nouvelle cible pondérée dans la base
			SaveItemPondere();
		else
			// Modification d'une cible pondérée existante
			ModifItemPondere();
	}
	else
	{
		// Problème de coefficient composant
		AfxMessageBox("La configuration % Base = 0 et % Pond != 0 est impossible à réaliser !!!, MB_ICONINFORMATION");
	}
}

////////////////////////////////////////////////////////////////////////////////////////
// Vérification de la cohérence de la configuration des composants pondérés
JBool CDlgCiblePondere::CoeffCompoValide()
{
	for (JInt32 InxRow = 1; InxRow <= m_GridPonder.GetRowCount(); InxRow++)
	{
		// Récupère la valeur de base
		JFlt64 CoeffBase = m_GridPonder.GetCellValue(InxRow, JFCSpinEditGrid::segReel);

		// Récupère la valeur pondération
		JFlt64 CoeffPonder = m_GridPonder.GetCellValue(InxRow, JFCSpinEditGrid::segPondere);

		// Si le coefficient réel est nul, le coeff pondération doit aussi être nul
		if (CoeffBase == 0 && CoeffPonder!=0)
			return false;
	}
	return true;
}

////////////////////////////////////////////////////////////////////////////////////////
// Suppression de la cible pondérée en cours de création
void CDlgCiblePondere::OnBnClickedCrSupprimer()
{
	// Uniquement possible en mode modification
	if (m_ModeCiblePond == MODE_MODIFICATION)
	{
		SupprimeItemPondere();
	}
}

/////////////////////////////////////////// Evénements sur les listes /////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////////////////////
// Modification de la cible de base
void CDlgCiblePondere::OnLbnSelchangeCrLstItems()
{
	m_ItemBase = 0;
	if (m_Lst_Items.GetCurSel() != LB_ERR)
	{
		// Récupère l'ident de l'item de base
		m_ItemBase = m_Lst_Items.GetItemData(m_Lst_Items.GetCurSel()); 
	}
}

/////////////////////////////////////////////////////////////////////////////////////////
// Changement sélection composants
void CDlgCiblePondere::OnLbnSelchangeCrLstComposants()
{
	if (m_ItemBase)
	{
		// Map temporaire
		JMap <JInt32, JFlt64> MapTempoCoeff;
		MapTempoCoeff.Reset(); 

		// Vérifie que les items composants sont bien indépendants
		if (SelItemsComposOK())
		{
			// Récupère les informations du terrain
			CTerrain * pTerrain = CTerrainManager::GetInstance()->GetTerrain(m_pDoc->m_Offre.m_IdSourceAudience, m_IdTerrainDef);

			// Récupère les infos dans le map composants + coeffs
			int NbSel = m_Lst_Composants.GetSelCount();
			int  *pSel = new int[NbSel];
			m_Lst_Composants.GetSelItems(NbSel,pSel); 

			// Boucle sur les items sélectionnés
			for(int i = 0; i < NbSel ; ++i)
			{
				// Récupère l'ident item
				JInt32 IdItem = m_Lst_Composants.GetItemData(pSel[i]); 

				// Ajoute au map des composants grille si non déjà existant
				m_MapNoLigCompoCoeff.MoveTo(IdItem);
				if (!m_MapNoLigCompoCoeff.IsValid())
				{
					// Récup le coeff origine
					JFlt64 CoeffReel = 0.0;

					// A FAIRECalcCoeffReel(m_ItemBase, IdCompo)
					JFlt64 PoidsItemBase;
					JFlt64 PoidsItemCompo = PoidsItemComposant(m_pDoc->m_Offre.m_IdSourceAudience, pTerrain, m_ItemBase, PoidsItemBase, IdItem);

					// Calcul du coeff réel
					CoeffReel = (JFlt64)PoidsItemCompo / (JFlt64) PoidsItemBase * 100.00; 

					// Insère nouvel élmt avec pondération initialisée à mmeme valeur que coeff réel
					MapTempoCoeff.Add(IdItem) = CoeffReel; 
				}
				else
				{
					// Récupère ancienne valeur
					m_MapNoLigCompoCoeff.MoveTo(IdItem);
					if (m_MapNoLigCompoCoeff.IsValid())
					{
						// On récupère le no de ligne
						const CInfoCoeff & InfoCoeff = m_MapNoLigCompoCoeff.GetItem();
						
						// Le remet dans map tempo
						MapTempoCoeff.Add(IdItem) = InfoCoeff.Coeff; 
					}
				}
			}

			// Update des composants dans la grille
			m_MapNoLigCompoCoeff.Reset();
			UpdateGrilleComposants(MapTempoCoeff);
		}
		else
		{
			// Les items sont dépendants, pas de possibilité de créer l'item pondéré
			AfxMessageBox("Les segments sélectionnés ne sont pas disjoints !!!!", MB_ICONINFORMATION);

			// Reset le map des coefficients
			m_MapNoLigCompoCoeff.Reset();
			MapTempoCoeff.Reset();
	
			// Update des composants dans la grille
			UpdateGrilleComposants(MapTempoCoeff);	

			// Enlever toutes les sélections composants
			for (JUnt32 InxLst = 0; InxLst < m_Lst_Composants.GetCount(); InxLst++)
				m_Lst_Composants.SetSel(InxLst, false); 
		}
	}
	else
	{
		// Attention aucun item de base sélectionné
		AfxMessageBox("Sélectionner d'abord une cible de base à pondérer",MB_ICONINFORMATION);

		// Enlever toutes les sélections composants
		for (JUnt32 InxLst = 0; InxLst < m_Lst_Composants.GetCount(); InxLst++)
			m_Lst_Composants.SetSel(InxLst, false); 

	}
}

//////////////////////////////////////////////////////////////////////////
// Update des composants dans la grille
void CDlgCiblePondere::UpdateGrilleComposants(const JMap <JInt32, JFlt64> & MapCoeff)
{
	// Les items existants
	CItemsConstruitsBase * pItems = CItemsConstruitsManager::GetInstance()->GetItemsInstance();

	// Vire toutes les lignes du grille
	m_GridPonder.DeleteLines();

	// Init des cumuls coeffs et pondérations de tous les composants
	JFlt64 CumulCoeff = 0.0;
	JFlt64 CumulPonder = 0.0;

	// Boucle sur le map des composants
	JUnt32 IndexLigne = 1;
	for (MapCoeff.MoveFirst(); MapCoeff.IsValid(); MapCoeff.MoveNext())
	{
		// Récup ident composant
		JInt32 IdCompo = MapCoeff.GetKey(); 

		// On l'ajoute à la liste des composants de l'item pondéré en cours de construction
		CItemConstruit * pItemCompo = pItems->GetIItemByID(IdCompo)->AsItemConstruit(); 

		// Récup le label du composant
		JLabel LblCompo = pItemCompo->GetLabel();

		// Récup le coeff origine
		JFlt64 CoeffReel = 0.0;

		CoeffReel = CalcCoeffReel(m_ItemBase, IdCompo);

		// Cumul coeff des composants
		CumulCoeff += CoeffReel;

		// Récup la pondération
		JFlt64 PonderCompo = MapCoeff.GetItem(); 

		// Et cumul les pondérations
		CumulPonder += PonderCompo;

		// On ajoute ligne composant à la grille avec les informations en cours
		m_GridPonder.AddLines();
		UpdateGridLigne(IndexLigne, LblCompo, CoeffReel, PonderCompo);

		// Sauve le map avec no ligne
		CInfoCoeff & InfoCoeff = m_MapNoLigCompoCoeff.Add(IdCompo);
		InfoCoeff.Coeff	= PonderCompo;
		InfoCoeff.NoLigne = m_GridPonder.GetRowCount(); 
		 
		// Passe à la ligne suivante
		IndexLigne++;
	}

	// Update de la ligne complément
	UpdateInfoComplement(CumulCoeff, CumulPonder);

}

//////////////////////////////////////////////////////////////////////////
// Calcul coeff reel item composant via l'item de base
JFlt64 CDlgCiblePondere::CalcCoeffReel(JUnt32 IdItemBase, JUnt32 IdCompo)
{
	// Récupère les informations du terrain
	CTerrain * pTerrain = CTerrainManager::GetInstance()->GetTerrain(m_pDoc->m_Offre.m_IdSourceAudience, m_IdTerrainDef);

	// Récup le coeff origine
	JFlt64 CoeffReel = 0.0;

	// A FAIRECalcCoeffReel(m_ItemBase, IdCompo)
	JFlt64 PoidsItemBase;
	JFlt64 PoidsItemCompo = PoidsItemComposant(m_pDoc->m_Offre.m_IdSourceAudience, pTerrain, IdItemBase, PoidsItemBase, IdCompo);

	// Calcul du coeff réel
	CoeffReel = (JFlt64)PoidsItemCompo / (JFlt64) PoidsItemBase * 100.00; 

	return CoeffReel;
}

//////////////////////////////////////////////////////////////////////////
// Update de la ligne complément
void CDlgCiblePondere::UpdateInfoComplement(JFlt64 CumulCoeff, JFlt64 CumulPonder)
{

	CString TxtCoeff;
	TxtCoeff.Format("%0.1f", 100.0 - CumulCoeff); 
	m_Prct_Compl_Reel.SetWindowText(TxtCoeff);

	// Valeur % pondéré du complément via les composants
	CString TxtPonder;
	TxtPonder.Format("%0.1f", 100.0 - CumulPonder);
	m_Prct_Compl_Pond.SetWindowText(TxtPonder);
}

//////////////////////////////////////////////////////////////////////////
// Modification sélection cible pondérée existante pour modification
void CDlgCiblePondere::OnLbnSelchangeCrLstCiblepond()
{
	m_ItemPondere = 0;
	if (m_Lst_CiblesPonds.GetCurSel() != LB_ERR)
	{
		// Récupère l'ident de l'item de base
		m_ItemPondere = m_Lst_CiblesPonds.GetItemData(m_Lst_CiblesPonds.GetCurSel()); 

		// Active la cible pondérée sélectionnée
		ActiveCiblePond();
	}
}

//////////////////////////////////////////////////////////////////////////
// Activation de la cible pondérée sélectionnée
JVoid CDlgCiblePondere::ActiveCiblePond()
{
	// Test d'abord si cible pondérée en cours de modification
	if (m_ItemPondere > 0)
	{
		// Récup les items
		CItemsConstruitsBase * pItemsConstruits = CItemsConstruitsManager::GetInstance()->GetItemsInstance();

		// Déselect les composants
		DeselectAllCompos();

		// On récupère l'item pondéré
		CItemPondere * pItem = pItemsConstruits->GetIItemByID(m_ItemPondere)->AsItemPondere(); 

		// Vérifie existence du pointeur sur l'item
		if (!pItem) return;
		
		// Vérifie que c'est bien un item pondéré
		if (pItem->IsItemPondere())
		{
			// Récupère le label de la cile pondérée
			JLabel LblItem = pItem->GetLabel();

			// Ident de la cible de base
			JInt32 IDItemBase = pItem->GetIDItemBase(); 
			m_ItemBase = IDItemBase;

			// On récupère l'item de base
			CItemConstruit * pItemBase = pItemsConstruits->GetIItemByID(IDItemBase)->AsItemConstruit();

			// Affiche nom de la cible de base dans l'édit box
			m_Edit_CiblePond.SetWindowText(pItemBase->GetLabel().AsJCharPtr()); 

			// Reset liste ds composants utilisés
			m_Lst_Composants.ResetContent();

			// Récupère les élmts composants
			JList <const CItemConstruit *>	ItemsCompos;
			JList <JFlt64>					PonderItems;
			pItem->GetItems(ItemsCompos, PonderItems);

			// Vire toutes les lignes de la grille
			m_GridPonder.DeleteLines(); 

			// A FAIRE En mode modification ActiveCiblePond à remplir HK
			m_MapNoLigCompoCoeff.Reset(); 

			// Total pondération et coeff réel
			JFlt64 TotPonder = 0.0;
			JFlt64 TotReel   = 0.0;

			// Affiche les composants
			PonderItems.MoveFirst(); 
			for (ItemsCompos.MoveFirst(); ItemsCompos.IsValid(); ItemsCompos.MoveNext(), PonderItems.MoveNext())
			{
				// Ident compo
				const CItemConstruit * ItemCompo = ItemsCompos.GetItem(); 
				JInt32 IDCompo = ItemCompo->GetID(); 

				// Récup label du composant
				JLabel LblCompo = ItemCompo->GetLabel(); 
				
				// Puis on l'ajoute à la liste des composants à sélectionner
				JUnt32 IdxCompo = m_Lst_Composants.AddString(LblCompo.AsJCharPtr());

				// On récupère l'item data
				m_Lst_Composants.SetItemData(IdxCompo, IDCompo); 

				// Récupére la pondération affectée à ce composant
				const JFlt64 Ponder = PonderItems.GetItem();

				// Cumul total pondérations
				TotPonder += Ponder;

				// Info coefficients
				CInfoCoeff &InfoCoeff = m_MapNoLigCompoCoeff.Add(IDCompo);
				InfoCoeff.Coeff		= Ponder; 
				InfoCoeff.NoLigne	= m_GridPonder.GetRowCount() + 1;

				// On ajoute ligne composant à la grille
				m_GridPonder.AddLines(); 

				// Calcul du coefficient réel
				JFlt64 CoeffReel = 0.0;
				CoeffReel = CalcCoeffReel(IDItemBase, IDCompo);

				// Cumul total coefficient réel
				TotReel += CoeffReel;

				// Affiche la ligne composant
				UpdateGridLigne(m_GridPonder.GetRowCount(), LblCompo, CoeffReel,  Ponder);
			}

			// Update de la ligne complément
			UpdateInfoComplement(TotReel, TotPonder);
		}	
	}	
}

///////////////////////////////////////////////////////////////////////////
// Ecriture d'une ligne du grid
// on passe le texte, le coeff réel, le coeff pondéré
void CDlgCiblePondere::UpdateGridLigne(JUnt32 IndexLigne, JLabel Libelle, JFlt64 CoeffReel, JFlt64 CoeffPond)
{
	// On ajoute le libelle du composant
	m_GridPonder.SetCellText(IndexLigne, JFCSpinEditGrid::segLibelle, Libelle.AsJCharPtr());

	// On ajoute % réel par rapport à la cible de base
	m_GridPonder.SetCellText(IndexLigne, JFCSpinEditGrid::segReel, CoeffReel);

	// On ajoute % Pondéré
	m_GridPonder.SetCellText(IndexLigne, JFCSpinEditGrid::segPondere, CoeffPond);

	// Hauteur de la nouvelle ligne
	m_GridPonder.SetRowHeight(IndexLigne, IndexLigne, 30);
}

//////////////////////////////////////////////////////////////////////////
// Désélection de tous les composants
JVoid CDlgCiblePondere::DeselectAllCompos()
{
	if (m_Lst_Composants.GetCount())
		m_Lst_Composants.SelItemRange(false, 0, m_Lst_Composants.GetCount()-1);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Calcul des effectifs item de base, et item composant via l'item de base
//
// On calculera l'effectif de l'item de base, puis en tenant compte de cet univers l'effectif de l'itm composant
JFlt64 CDlgCiblePondere::PoidsItemComposant(const JUnt32 IdSource, const CTerrain * pTerrain, const JUnt32 IDItemBase, JFlt64 & PoidsItemBase, const JUnt32 IDItem)
{
	// Récup les items
	CItemsConstruitsBase * pItemsConstruits = CItemsConstruitsManager::GetInstance()->GetItemsInstance();

	// Récupère l'item de base
	CItemConstruit * pItemBase = pItemsConstruits->GetIItemByID(IDItemBase)->AsItemConstruit();

	// Récupère le vecteur de l'item de base sur ce terrain
	CCalcItemParams ParamsItemBase;
	const CBitArray * pBitArrayItemBase = 0;
	GetVectItem(pItemBase, m_IdTerrainDef, ParamsItemBase);
	pBitArrayItemBase = ParamsItemBase.GetResults();

	// Récupère l'item de base
	CItemConstruit * pItem = pItemsConstruits->GetIItemByID(IDItem)->AsItemConstruit();

	// Récupère le vecteur de l'item composant sur ce terrain
	CCalcItemParams ParamsItem;
	const CBitArray * pBitArrayItem = 0;
	GetVectItem(pItem, m_IdTerrainDef, ParamsItem);
	pBitArrayItem = ParamsItem.GetResults();

	// On fabrique le vecteur résultant
	JUnt32 NbItemBase	= pBitArrayItemBase->GetSize();
	JUnt32 NbItem		= pBitArrayItem->GetSize(); 
	ASSERT (NbItemBase == NbItem);
	CBitArray * pBitArrayResultant = new CBitArray(* pBitArrayItemBase);

	// Combine avec vecteur Item Composant
	*pBitArrayResultant &= *pBitArrayItem;

	// Récup tableau des terrains via le moteur
	const JMTFR03TBLTerrains & TblTerrains = m_pDoc->m_MoteurPresse.GetTblTerrains();

	// Récup table des modulations
	const JMTFR03TBLModulations	 & TblModulations = m_pDoc->m_MoteurPresse.GetTblModulations();

	// Paramètres de modulation
	JMTFR03KEYModulation keyMod;
	keyMod.m_IdSource = IdSource;
	keyMod.m_IdTerrain = m_IdTerrainDef;
	TblModulations.MoveTo(keyMod);
	const JMTFR03DATModulation & DatMod = TblModulations.GetItem();

	// Calcul des 2 effectifs via les poids du terrain
	JMTFR03KEYTerrain KeyTerrain;
	KeyTerrain.m_IdSource  = IdSource; 
	KeyTerrain.m_IdTerrain = pTerrain->GetTerrainID(); 
	TblTerrains.MoveTo(KeyTerrain);
	if (TblTerrains.IsValid())
	{
		// Récupération des informations terrain 
		const JMTFR03DATTerrain & DATTerrain = TblTerrains.GetItem(); 

		JVector<JInt32> vect;
		pTerrain->GetTypesPoids(vect); 

		// Type de poids utilisé
		JInt32 IdTypePoids = DatMod.m_FLTPoids.Item(DatMod.m_FLTPoids.GetSelItem()).AsJInt32();
		if (!pTerrain->HasTypePoids(IdTypePoids))
			IdTypePoids = pTerrain->GetDefaultTypePoidsID() ;

		// Univers de travail
		JInt32 IdUnivers;
		IdUnivers = pTerrain->GetDefaultUnivers();

		JMTFR03KEYPoids KEYPoids;
		KEYPoids.m_IdPoids   = IdTypePoids ;
		KEYPoids.m_IdUnivers = IdUnivers ;

		DATTerrain.m_TBLPoids.MoveTo(KEYPoids); 
		JInt32 NbPoids = DATTerrain.m_TBLPoids.GetItem().m_ItemPoids.GetCount();

		// Les poids
		PoidsItemBase		= 0.0;
		JFlt64 PoidsItem	= 0.0;

		for (JInt32 InxPoids = 0; InxPoids < DATTerrain.m_TBLPoids.GetItem().m_ItemPoids.GetCount(); InxPoids++)
		{
			// Cumul des poids pour l'item de base
			JBool ValByte = pBitArrayItemBase->GetAt(InxPoids);
			if (ValByte)
				PoidsItemBase += (JFlt64) DATTerrain.m_TBLPoids.GetItem().m_ItemPoids.Item(InxPoids).AsJInt32();

			// Cumul des poids pour l'item via l'item de base
			ValByte = pBitArrayResultant->GetAt(InxPoids);
			if (ValByte)
			{
				// Cumul poids du composant
				PoidsItem += (JFlt64) DATTerrain.m_TBLPoids.GetItem().m_ItemPoids.Item(InxPoids).AsJInt32();
			}
		}

		// Renvoie poids de l'item su l'item de base
		return PoidsItem;
	}

	// Problème calcul de poids
	return 0.0;
}

//////////////////////////////////////////////////////////////////////////
// Update valeur d'une cellule envoyée par le grille pondération
void CDlgCiblePondere::UpdateValue(ROWCOL nRow, ROWCOL nCol)
{
	// Vérifie validité de la position cellule en ligne et colonne
	ASSERT (nRow <= m_GridPonder.GetRowCount() && nCol <= JFCSpinEditGrid::segPondere);

	// Uniquement dans le cas d'une cellule éditable, c.a.d en position "Cellule Pondération"
	if (nCol == JFCSpinEditGrid::segPondere)
	{
		// Récupère la valeur pondération
		JFlt64 ValPonder = m_GridPonder.GetCellValue(nRow, JFCSpinEditGrid::segPondere);

		// Récup nouveau cumul
		JFlt64 TotPonder = 0.0;

		// Test si totalité des pondérations <= 100%
		if (GetTotPonderation(nRow, ValPonder) > 100.0)
		{
			// Ex valeur avant la modification
			JFlt64 ExPonder = 0.0;	

			// Boucle sur toutes les pondérations 
			for (m_MapNoLigCompoCoeff.MoveFirst(); m_MapNoLigCompoCoeff.IsValid(); m_MapNoLigCompoCoeff.MoveNext())
			{
				// Récupère la pondération
				const CInfoCoeff & InfoCoeff = m_MapNoLigCompoCoeff.GetItem();

				// Se positionne sur la ligne à remodifier
				if (nRow == InfoCoeff.NoLigne)
				{
					// Récupère ancienne pondération et on sort
					ExPonder = InfoCoeff.Coeff; 	
					break;
				}
			}

			// On remet la valeur
			m_GridPonder.SetCellText(nRow, JFCSpinEditGrid::segPondere, ExPonder);

			// Récup nouveau cumul avec ex-value
			TotPonder = GetTotPonderation(nRow, ExPonder);
		}
		else
		{
			// On sauvegarde la nouvelle valeur dans le map
			// Boucle sur toutes les pondérations 
			for (m_MapNoLigCompoCoeff.MoveFirst(); m_MapNoLigCompoCoeff.IsValid(); m_MapNoLigCompoCoeff.MoveNext())
			{
				// Récupère la pondération
				CInfoCoeff & InfoCoeff = m_MapNoLigCompoCoeff.GetItem();

				// Se positionne sur la ligne à remodifier
				if (nRow == InfoCoeff.NoLigne)
				{
					// Récupère ancienne pondération et on sort
					InfoCoeff.Coeff = ValPonder; 	
					break;
				}
			}

			// Récup nouveau cumul avec new-value
			TotPonder = GetTotPonderation(nRow, ValPonder);
		}

		// Récup coeff réel fixe
		CString TxtCoeffReel;
		m_Prct_Compl_Reel.GetWindowText(TxtCoeffReel);
		JFlt64 TotCoeffReel = 100.00 - atof(TxtCoeffReel);

		// Update de la ligne complément
		UpdateInfoComplement(TotCoeffReel, TotPonder);
	}
}

//////////////////////////////////////////////////////////////////////////
// Total pondération en cours
JFlt64 CDlgCiblePondere::GetTotPonderation(const ROWCOL nRow, const JFlt64 NewValue)
{
	// Init total pondération
	JFlt64 TotPonderation = 0.0;

	// Boucle sur toutes les pondérations en cours
	for (m_MapNoLigCompoCoeff.MoveFirst(); m_MapNoLigCompoCoeff.IsValid(); m_MapNoLigCompoCoeff.MoveNext())
	{
		// Récupère la pondération
		const CInfoCoeff & InfoCoeff = m_MapNoLigCompoCoeff.GetItem();

		// Et on cumule si ce n'est pas la ligne en cours de modification
		if (InfoCoeff.NoLigne != nRow)
			TotPonderation += InfoCoeff.Coeff; 		
	}

	// Puis on ajoute la nouvelle pondération de la ligne en cours de modif
	TotPonderation += NewValue;	

	// Renvoi la totalité pondération
	return TotPonderation;
}

//////////////////////////////////////////////////////////////////////////
// Affichage de la fenêtre de dialogue
void CDlgCiblePondere::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CDialog::OnShowWindow(bShow, nStatus);

	// A REVOIR, pour sortir de la boite de dialogue
	if (m_IdTerrainDef == 0)
	{
		AfxMessageBox("Terrains non définis !!!", MB_ICONINFORMATION);
		// on quitte la boite de dialogue
		OnClose();
	}

	// Libellé des entetes grid pondérations
	m_GridPonder.SetHeaderText(JFCSpinEditGrid::segLibelle, "Libellé");
	m_GridPonder.SetHeaderText(JFCSpinEditGrid::segReel,	"% Base");  // "% Réel");
	m_GridPonder.SetHeaderText(JFCSpinEditGrid::segPondere, "% Ponder.");

	// Largeur des colonnes headers
	m_GridPonder.SetColWidth(JFCSpinEditGrid::segLibelle,	JFCSpinEditGrid::segLibelle,	160);
	m_GridPonder.SetColWidth(JFCSpinEditGrid::segReel,		JFCSpinEditGrid::segReel,		 50);
	m_GridPonder.SetColWidth(JFCSpinEditGrid::segPondere,	JFCSpinEditGrid::segPondere,     50);

	// Paramètrage nb décimales sur toutes les lignes pour les spinédit
	m_GridPonder.SetSpinCellsNumDecimales(Const_NbDecimale);

	// on definit les bornes des ponderations (%)
	m_GridPonder.SetSpinCellsBounds(0.0, 100.0);

	// on définit le nombre de décimales de la colonne coeff réel (en%)
	m_GridPonder.SetStyleRange(CGXRange().SetCols(JFCSpinEditGrid::segReel), 
							   CGXStyle().SetFloatCell(TRUE)
										 .SetValueType(GX_VT_NUMERIC )
										 .SetFormat(GX_FMT_FIXED)
										 .SetPlaces(Const_NbDecimale));

	// On bloque les 2 colonnes fixes (libellé et coeff réel)
	m_GridPonder.SetFrozenCols(Const_NbColFixe, 0,  GX_UPDATENOW);
}


////////////////////////////////////////////////////////////////////////////////////////////////
// Quitte la boite de dialogue
void CDlgCiblePondere::OnClose()
{
	this->EndDialog(IDOK);
	// CDialog::OnClose();
}


////////////////////////////////////////////////////////////////////////////////////////////////
// Fermeture de la boite de dialogue cibles pondérées
void CDlgCiblePondere::OnBnClickedCrFermer()
{
	this->EndDialog(IDOK);
	// CDialog::OnClose();
}
