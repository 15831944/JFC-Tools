#include "StdAfx.h"
#include ".\rubriqueitemtree.h"
#include "IItem.h"
#include "ItemMode.h"
#include "DlgItems.h"
#include "DlgClassementItems.h"
#include "FileHeader.h"

#include "JFCConfig.h"

// main symbols
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

/////////////////////////////////////////////////////////////////////////////
// CRubriqueItemTree

//Pour utilisersKindOf
// IMPLEMENT_DYNAMIC( CRubriqueItemTree, CTreeCtrl )

///////////////////////////////////////////////////////////////////////
// Constructeur
CRubriqueItemTree::CRubriqueItemTree()
{
	m_bSelectionComplete = true;
	m_selColor = ::GetSysColor(COLOR_HIGHLIGHT);
	m_bEditable = false;
	m_bExpandAll = false;
	m_bMultiSel = false;

	// Couleur arbre rubriques
	m_CATPColorsApp = CATPColors::COLORREPITEMS;

	// Init du Map Rubriques items
	m_Map_RubriquesItems.Reset(); 

	// Init du Map items à afficher
	m_MapAffichItems.Reset(); 

	// Init rubrique racine et divers
	m_hRoot				= 0;
	m_hRubriqueDivers	= 0;
	m_hRubriqueDefaut   = 0;

	// Type de rubrique par défaut
	m_Type_Rubrique		= TYPE_RUBRIQUE_ITEM;

	// Init mode édition
	m_Mode_Edit = MODE_EDIT_DIRECT;

	// Init fenêtres parent
	m_pDlgItems = NULL;
	m_pDialog   = NULL;

	// Init compteur rubrique client
	m_CptRubrique = 0;

	m_Firstime = true;
}

//////////////////////////////////////////////////////////////////////
// Destructeur
CRubriqueItemTree::~CRubriqueItemTree()
{
	// Sauvegarde des rubriques items
	SaveFileRubrique();
}

//////////////////////////////////////////////////////////////////////
// Positionne le type de rubriques
JVoid CRubriqueItemTree::SetTypeRubrique(JUnt32 TypeRubrique)
{
	m_Type_Rubrique = TypeRubrique;
}

/////////////////////////////////////////////////////////////////////////:
// Chargement des rubriques items via 1 seul terrain (dialogue DlgItems)
bool CRubriqueItemTree::LoadRubriquesItems(CItemsConstruitsBase *pItemsConstruits, CTerrain *pTerrain)
{
	// Mode tree controle avec check box
	/*
	this->ModifyStyle(TVS_CHECKBOXES, 0 );
	this->ModifyStyle( 0, TVS_CHECKBOXES );
	*/

	// En attendant, on charge tous les items dans Rubrique Divers 
	// Remove all items
	SetColors();
	DeleteAllItems();

	// Init Map des rubriques
	m_Map_RubriquesItems.Reset();

	// Init Map des items à afficher
	m_MapAffichItems.Reset();

	// Le noeud racine rubriques items ou palettes ou autres
	CString str;
	if (m_Type_Rubrique == TYPE_RUBRIQUE_PALETTE)
		LOADSTRING(str, IDS_RI_RUBRIQUE_PALETTES);
	else 
		LOADSTRING(str, IDS_RI_RUBRIQUE_ITEMS);

	// Init compteur rubrique client
	m_CptRubrique = 0;

	HTREEITEM hRoot = InsertItem(TVIF_CHILDREN|TVIF_PARAM|TVIF_STATE|TVIF_TEXT,str,0,0,(0x0001)|INDEXTOSTATEIMAGEMASK(1),TVIS_STATEIMAGEMASK|0x000F,0,0,0);
	SetItemData(hRoot, ID_ROOT);

	// Rubrique racine
	m_hRoot = hRoot;
	
	// Si non chargement rubriques via fichier, on initialise le tree rubriques
	m_hRubriqueDefaut  = 0;
	if (!LoadFileRubriques())
	{
		///////////////////////////////////////////////////////////////////////////////
		// Crée la rubrique "Divers"
		///////////////////////////////////////////////////////////////////////////////
		CString Txt;
		Txt.LoadString(IDS_RI_DIVERS);
		HTREEITEM hRubriqueDivers = InsertItem(TVIF_STATE|TVIF_TEXT,Txt,0,0,(0x0001)|INDEXTOSTATEIMAGEMASK(1),TVIS_STATEIMAGEMASK|0x000F,0,hRoot,0);
		SetItemData(hRubriqueDivers, ID_RUBRIQUE_DIVERS);

		// Rubrique divers
		m_hRubriqueDivers = hRubriqueDivers;


		// Ajoute la rubrique divers au map
		m_Map_RubriquesItems.Add(Txt);  
	}
	else
	{
		//////////////////////////////////////////////////////////////////////////////
		// Balaye toutes les rubriques crées précédemment, et remplit le tri rubriques
		//////////////////////////////////////////////////////////////////////////////

		// Aout 2013
		// Recup la rubrique par defaut
		CString TxtRubriquePardefaut = "Divers";
		if (m_Type_Rubrique == TYPE_RUBRIQUE_PALETTE)
			TxtRubriquePardefaut = AfxGetApp()->GetProfileString("Options\\RepItem", "RubriquePalette","Divers");
		else
			TxtRubriquePardefaut = AfxGetApp()->GetProfileString("Options\\RepItem", "RubriqueItem","Divers");
				
		for (m_Map_RubriquesItems.MoveFirst(); m_Map_RubriquesItems.IsValid(); m_Map_RubriquesItems.MoveNext())
		{
			// Récup le nom de la rubrique
			CString TxtRubrique;
			TxtRubrique = m_Map_RubriquesItems.GetKey(); 

			// Ajoute la branche rubrique
			HTREEITEM hRubrique = InsertItem(TVIF_STATE|TVIF_TEXT,TxtRubrique,0,0,(0x0001)|INDEXTOSTATEIMAGEMASK(1),TVIS_STATEIMAGEMASK|0x000F,0,hRoot,0);

			// Aout 2013
			// Indique si c'est la branche divers
			if (TxtRubrique == "Divers")
				m_hRubriqueDivers = hRubrique;

			if (TxtRubrique == TxtRubriquePardefaut)
			{
				SetItemData(hRubrique, ID_RUBRIQUE_DIVERS);
				m_hRubriqueDefaut  = hRubrique;
			}
			else
			{
				// Rubrique créer par utilisateur
				SetItemData(hRubrique, ID_RUBRIQUE_CLIENT + m_CptRubrique);
				m_CptRubrique++;
			}
		}

	}

	// Par défaut la rubrique par défaut sera la rubrique divers
	// Aout 2013
	// m_hRubriqueDefaut = m_hRubriqueDivers;

	// Si pas de rubrique par defaut trouvé, on met rubrique par defaut = rubrique divers
	if (!m_hRubriqueDefaut)
		m_hRubriqueDefaut  = m_hRubriqueDivers;

	// Réactualise au cas où plantage
	ReactualiseItems(pItemsConstruits, pTerrain);

	// Puis on déplie sur toutes les rubriques
	Expand(m_hRoot, TVE_EXPAND);

	// Par défaut marque rubrique en cours = rubrique divers
	// SetItemByDefault(m_hRubriqueDivers);
	// this->SetCheck(m_hRubriqueDivers, true);
	// Aout 2013
	SetItemByDefault(m_hRubriqueDefaut);
	
	return true;
}

//////////////////////////////////////////////////////////////////////////
// Chargement des rubriques items via plusieurs terrains (dialogue Offre)
bool CRubriqueItemTree::LoadRubriquesItems(CItemsConstruitsBase *pItemsConstruits, JList <CTerrain *> &LstTerrains)
{
	// En attendant, on charge tous les items dans Rubrique Divers 
	// Remove all items
	SetColors();
	DeleteAllItems();

	// Init Map des rubriques
	m_Map_RubriquesItems.Reset();

	// Init Map des items à afficher
	m_MapAffichItems.Reset();

	// Le noeud racine rubriques items ou palettes ou autres
	CString str;
	if (m_Type_Rubrique == TYPE_RUBRIQUE_PALETTE)
		LOADSTRING(str, IDS_RI_RUBRIQUE_PALETTES);
	else 
		LOADSTRING(str, IDS_RI_RUBRIQUE_ITEMS);

	// Init compteur rubrique client
	m_CptRubrique = 0;

	HTREEITEM hRoot = InsertItem(TVIF_CHILDREN|TVIF_PARAM|TVIF_STATE|TVIF_TEXT,str,0,0,(0x0001)|INDEXTOSTATEIMAGEMASK(1),TVIS_STATEIMAGEMASK|0x000F,0,0,0);
	SetItemData(hRoot, ID_ROOT);

	// Rubrique racine
	m_hRoot = hRoot;
	
	// Si non chargement rubriques via fichier, on initialise le tree rubriques
	if (!LoadFileRubriques())
	{
		///////////////////////////////////////////////////////////////////////////////
		// Crée la rubrique "Divers"
		///////////////////////////////////////////////////////////////////////////////
		CString Txt;
		Txt.LoadString(IDS_RI_DIVERS);
		HTREEITEM hRubriqueDivers = InsertItem(TVIF_STATE|TVIF_TEXT,Txt,0,0,(0x0001)|INDEXTOSTATEIMAGEMASK(1),TVIS_STATEIMAGEMASK|0x000F,0,hRoot,0);
		SetItemData(hRubriqueDivers, ID_RUBRIQUE_DIVERS);

		// Rubrique divers
		m_hRubriqueDivers = hRubriqueDivers;

		// Ajoute la rubrique divers au map
		m_Map_RubriquesItems.Add(Txt);  
	}
	else
	{
		//////////////////////////////////////////////////////////////////////////////
		// Balaye toutes les rubriques crées précédemment, et remplit le tri rubriques
		//////////////////////////////////////////////////////////////////////////////
		for (m_Map_RubriquesItems.MoveFirst(); m_Map_RubriquesItems.IsValid(); m_Map_RubriquesItems.MoveNext())
		{
			// Récup le nom de la rubrique
			CString TxtRubrique;
			TxtRubrique = m_Map_RubriquesItems.GetKey(); 

			// Ajoute la branche rubrique
			HTREEITEM hRubrique = InsertItem(TVIF_STATE|TVIF_TEXT,TxtRubrique,0,0,(0x0001)|INDEXTOSTATEIMAGEMASK(1),TVIS_STATEIMAGEMASK|0x000F,0,hRoot,0);

			// Indique si c'est la branche divers
			if (TxtRubrique == "Divers")
			{
				SetItemData(hRubrique, ID_RUBRIQUE_DIVERS);
				m_hRubriqueDivers = hRubrique;
			}
			else
			{
				SetItemData(hRubrique, ID_RUBRIQUE_CLIENT + m_CptRubrique);
				m_CptRubrique++;
			}
		}
	}

	// Réactualise au cas où plantage
	ReactualiseItems(pItemsConstruits, LstTerrains);

	// Puis on déplie sur toutes les rubriques
	Expand(m_hRoot, TVE_EXPAND);

	return true;
}

//////////////////////////////////////////////////////////////////////
// Réordonne listes des rubriques par ordre chronologique
JVoid CRubriqueItemTree::TriRubriques()
{
	// Tri tous les élmts via la racine rubrique
	this->SortChildren(m_hRoot);
}

//////////////////////////////////////////////////////////////////////
// Chargement fichier rubriques
JBool CRubriqueItemTree::LoadFileRubriques()
{
	// Si fichier vide, on crée la rubrique divers et on la remplit des items existants
	SetCurrentDirectory(GETPROFILESTRINGD(SECTION_DATA, KEY_USER_PATH, 0));

	// Nom du fichier rubrique
	CString FicRubrique;
	FicRubrique.Format("/%s%d.%s","Rubrique",m_Type_Rubrique,"lst");  
	JStringEx PathRubrique;
	PathRubrique.Append(FicRubrique);

	try
	{
		// Création 
		JArchive *pFile		= JArchive::Create(PathRubrique.AsJCharPtr());

		// Chargement entête
		LoadFileHeader(pFile);

		// Charge les rubriques
		LoadRubriques(pFile);
		delete pFile;
	}
	catch(JException *pE)
	{
		// Problème ouverture fichier
		pE->GetMessage();
		SetCurrentDirectory(GETPROFILESTRINGD(SECTION_DATA, KEY_SOURCES_PATH, 0));
		return false;
	}

	SetCurrentDirectory(GETPROFILESTRINGD(SECTION_DATA, KEY_SOURCES_PATH, 0));
	return true;

}

//////////////////////////////////////////////////////////////////////
// Chargement entete fichier rubrique
JVoid CRubriqueItemTree::LoadFileHeader(JArchive *pFile)
{
	// Version fichier
	const JInt32 CURRENT_VERSION = 1;

	m_FileHeader.Recv(*pFile);
	
	// Check compagnie name
	if(m_FileHeader.GetCompagnie () != 0x0043464A)
		throw JInternalError::GetInstance(); //("File Header, compagnie field error");

	// Verify the magic number
	if(m_FileHeader.GetMagicNumber () != 0x6FD3481C)
		//("File Header, Magic Number field error");
		throw JInternalError::GetInstance(); 

	// No compression
	if(m_FileHeader.GetCompression () != 0)
		//("File Header, Compression field error");
		throw JInternalError::GetInstance(); 

	// No cryptage
	if(m_FileHeader.GetCryptage () != 0)
		//("File Header, Cryptage field error");
		throw JInternalError::GetInstance(); 

	// Must not be greater than my current version
	if(m_FileHeader.GetVersion () > CURRENT_VERSION)
		//("File Header, Version error");
		throw JInternalError::GetInstance(); 
}

//////////////////////////////////////////////////////////////////////
// Charge les rubriques
JVoid CRubriqueItemTree::LoadRubriques(JArchive *pFile)
{
	// Init map des rubriques
	m_Map_RubriquesItems.Reset(); 

	// Récupération nombre de rubriques
	JUnt32 NbRubrique;
	pFile->Recv(NbRubrique);

	// Récupération de chaque rubrique
	for (JUnt32 InxRubrique = 0; InxRubrique < NbRubrique; InxRubrique++)
	{
		// Récup label rubrique
		JString LabelRubrique;
		LabelRubrique.Recv(*pFile); 

		// Récup type rubrique
		JUnt32 TypeRubrique;
		pFile->Recv(TypeRubrique); 

		// Test si c'est une rubrique du type recherché (items, palettes ou classes)
		if (TypeRubrique == m_Type_Rubrique)
		{
			// Récup liste des items associés
			JList <JInt32> &LstItems = m_Map_RubriquesItems.Add(LabelRubrique.AsJCharPtr());  
			
			// Récup nb items associés
			JUnt32 NbItems;
			pFile->Recv(NbItems);
			
			// Et les idents des items associés
			for (JUnt32 InxItem = 0; InxItem < NbItems; InxItem++)
			{
				// Récup ident items
				JInt32 IdItem;
				pFile->Recv(IdItem);

				// Ajout item à la liste des items associés
				LstItems.AddTail() = IdItem; 

				// Ajoute au map des items à afficher
				m_MapAffichItems.MoveTo(IdItem);
				if (!m_MapAffichItems.IsValid()) 
					m_MapAffichItems.Add(IdItem) = false;
			}
		}
	}
}

///////////////////////////////////////////////////////////////////////////////
// Tous les nouveaux items seront d'office dans rubrique divers (via DlgItems)
void CRubriqueItemTree::ReactualiseItems(CItemsConstruitsBase *pItemsConstruits, CTerrain *pTerrain)
{
	// On se positionne sur la rubrique divers 
	CString TxtRubriqueDivers;
	TxtRubriqueDivers.LoadString(IDS_RI_DIVERS);
	m_Map_RubriquesItems.MoveTo(TxtRubriqueDivers); 
	if (m_Map_RubriquesItems.IsValid())
	{
		// Récup liste des items associés à la rubrique divers
		JList <JInt32> &LstDiversItems = m_Map_RubriquesItems.GetItem();

		// Par défaut branche rubrique divers non sélectionnée
		JBool EtatAffichRubrique = false;

		// Recherche branche rubrique divers
		HTREEITEM hti = GetRootItem();
		while(hti)
		{
			if (ItemHasChildren(hti))
			{
				// Test si c'est la branche divers
				if (GetItemData(GetChildItem(hti)) == ID_RUBRIQUE_DIVERS)
				{
					// Récupère état sélection rubrique divers
					UINT StateRubrique = GetItemState(GetChildItem(hti), TVIS_STATEIMAGEMASK); 
					if((StateRubrique>>12) == 2)
						EtatAffichRubrique = true;
					break;
				}
			}
			hti = GetNextItem(hti,TVGN_NEXT);
		}

		// Chargement des items non classés si rubrique items
		if (m_Type_Rubrique == TYPE_RUBRIQUE_ITEM)
		{
			// Repère les items non initialisés
			JList<IItem const *> vItems;
			// m_pDlgItems->GetItems(()->GetItemsIndependant(vItems,m_pDlgItems->GetTerrain());
			pItemsConstruits->GetItemsIndependant(vItems,pTerrain);

			///////////////////////////////////////////////////////////////////////////////////
			// Get only those valid for this terrain
			for (vItems.MoveFirst(); vItems.IsValid(); vItems.MoveNext())
			{
				// Ident de l'item
				JInt32 IdItem = vItems.GetItem()->GetID();

				// Vérifie non déjà présent
				m_MapAffichItems.MoveTo(IdItem);
				if (!m_MapAffichItems.IsValid())
				{
					// Nouvel item - on l'ajoute avec état sélection rubrique divers
					m_MapAffichItems.Add(IdItem) = EtatAffichRubrique;

					// Puis on le stocke dans rubrique divers
					LstDiversItems.AddTail() = IdItem; 
				}
			}
		}
		// Chargement des palettes non classées si rubrique palettes
		else if (m_Type_Rubrique == TYPE_RUBRIQUE_PALETTE)
		{
			JList<IItem const *> vItems;
			// m_pDlgItems->GetItems()->GetPalettes(vItems, m_pDlgItems->GetTerrain());
			pItemsConstruits->GetPalettes(vItems, pTerrain);

			// Get only those valid for this terrain
			for (vItems.MoveFirst(); vItems.IsValid(); vItems.MoveNext())
			{
				// Ident de l'item
				JInt32 IdItem = vItems.GetItem()->GetID();

				// Vérifie non déjà présent
				m_MapAffichItems.MoveTo(IdItem);
				if (!m_MapAffichItems.IsValid())
				{
					// Nouvel item - on l'ajoute avec état sélection rubrique divers
					m_MapAffichItems.Add(IdItem) = EtatAffichRubrique;

					// Puis on le stocke dans rubrique divers
					LstDiversItems.AddTail() = IdItem; 
				}
			}
		}
	}
}

////////////////////////////////////////////////////////////////////////////////////
// Idem fonction précédente mais en passant une liste de terrain (via dialog Offre)
void CRubriqueItemTree::ReactualiseItems(CItemsConstruitsBase *pItemsConstruits, JList <CTerrain *> &LstTerrains)
{
	// On se positionne sur la rubrique divers 
	CString TxtRubriqueDivers;
	TxtRubriqueDivers.LoadString(IDS_RI_DIVERS);
	m_Map_RubriquesItems.MoveTo(TxtRubriqueDivers); 
	if (m_Map_RubriquesItems.IsValid())
	{
		// Récup liste des items associés à la rubrique divers
		JList <JInt32> &LstDiversItems = m_Map_RubriquesItems.GetItem();

		// Par défaut branche rubrique divers non sélectionnée
		JBool EtatAffichRubrique = false;

		// Recherche branche rubrique divers
		HTREEITEM hti = GetRootItem();
		while(hti)
		{
			if (ItemHasChildren(hti))
			{
				// Test si c'est la branche divers
				if (GetItemData(GetChildItem(hti)) == ID_RUBRIQUE_DIVERS)
				{
					// Récupère état sélection rubrique divers
					UINT StateRubrique = GetItemState(GetChildItem(hti), TVIS_STATEIMAGEMASK); 
					if((StateRubrique>>12) == 2)
						EtatAffichRubrique = true;
					break;
				}
			}
			hti = GetNextItem(hti,TVGN_NEXT);
		}

		// Chargement des items non classés si rubrique items
		if (m_Type_Rubrique == TYPE_RUBRIQUE_ITEM)
		{
			// Repère les items non initialisés
			JList<IItem const *> vItems;
			// m_pDlgItems->GetItems(()->GetItemsIndependant(vItems,m_pDlgItems->GetTerrain());
			pItemsConstruits->GetItemsIndependant(vItems, LstTerrains);

			///////////////////////////////////////////////////////////////////////////////////
			// Get only those valid for this terrain
			for (vItems.MoveFirst(); vItems.IsValid(); vItems.MoveNext())
			{
				// Ident de l'item
				JInt32 IdItem = vItems.GetItem()->GetID();

				// Vérifie non déjà présent
				m_MapAffichItems.MoveTo(IdItem);
				if (!m_MapAffichItems.IsValid())
				{
					// Nouvel item - on l'ajoute avec état sélection rubrique divers
					m_MapAffichItems.Add(IdItem) = EtatAffichRubrique;

					// Puis on le stocke dans rubrique divers
					LstDiversItems.AddTail() = IdItem; 
				}
			}
		}
		// Chargement des palettes non classées si rubrique palettes
		else if (m_Type_Rubrique == TYPE_RUBRIQUE_PALETTE)
		{
			JList<IItem const *> vItems;
			// m_pDlgItems->GetItems()->GetPalettes(vItems, m_pDlgItems->GetTerrain());
			pItemsConstruits->GetPalettes(vItems, LstTerrains);

			// Get only those valid for this terrain
			for (vItems.MoveFirst(); vItems.IsValid(); vItems.MoveNext())
			{
				// Ident de l'item
				JInt32 IdItem = vItems.GetItem()->GetID();

				// Vérifie non déjà présent
				m_MapAffichItems.MoveTo(IdItem);
				if (!m_MapAffichItems.IsValid())
				{
					// Nouvel item - on l'ajoute avec état sélection rubrique divers
					m_MapAffichItems.Add(IdItem) = EtatAffichRubrique;

					// Puis on le stocke dans rubrique divers
					LstDiversItems.AddTail() = IdItem; 
				}
			}
		}
	}
}


//////////////////////////////////////////////////////////////////////
// Sauvegarde du fichier rubriques en cours
JBool CRubriqueItemTree::SaveFileRubrique()
{
	// On ne sauve les rubriques que si l'aborescence rubrique est déployé
	// Modif pour corriger la suppression de l'arborescence lors d'un mauvais lancement application
	// 18 Février 2008
	JUnt32 NbRubrique = m_Map_RubriquesItems.GetCount();

	if (NbRubrique > 0)
	{
		// Nom du fichier à sauver
		JStringEx FileRubrique;
		SetCurrentDirectory(GETPROFILESTRINGD(SECTION_DATA, KEY_USER_PATH, 0));

		// Nom du fichier rubrique
		CString FicRubrique;
		FicRubrique.Format("/%s%d.%s","Rubrique",m_Type_Rubrique,"lst");  
		JStringEx PathRubrique; 
		// FileRubrique.Append ("/Rubrique.lst");
		PathRubrique.Append(FicRubrique);

		JArchive * pFile = 0;

		const JInt32 CURRENT_VERSION = 1;
		try
		{
			pFile = JArchive::Create(PathRubrique.AsJCharPtr(), true);
			
			// Sauve header file rubrique
			SaveFileHeader(pFile);

			//Sauve les rubriques avec liste des items associés
			SaveRubriques(pFile);

			// Vide le tampon
			pFile->Flush();
		}
		catch(JException *pE)
		{
			// Supprime  le tampon
			delete pFile;
			CString MessErr = pE->GetMessage();
			SetCurrentDirectory(GETPROFILESTRINGD(SECTION_DATA, KEY_SOURCES_PATH, 0));
			return false;
		}

		// Supprime  le tampon
		delete pFile;

		try
		{
			// Mise à jour du fichier
			JPathManager::UpdateFile(PathRubrique.AsJCharPtr());
		}
		catch(JException * pE)
		{
			SetCurrentDirectory(GETPROFILESTRINGD(SECTION_DATA, KEY_SOURCES_PATH, 0));
			return false;
		}
		// Remet répertoire courant
		SetCurrentDirectory(GETPROFILESTRINGD(SECTION_DATA, KEY_SOURCES_PATH, 0));
	}

	return true;
}

///////////////////////////////////////////////////////////////////////////////////////
// Sauvegarde entête fichier rubriques
JVoid CRubriqueItemTree::SaveFileHeader(JArchive *pFile)
{
	const JInt32 CURRENT_VERSION = 1;

	// Check compagnie name
	m_FileHeader.SetCompagnie (0x0043464A);
	// Verify the magic number
	m_FileHeader.SetMagicNumber (0x6FD3481C);
	// No compression
	m_FileHeader.SetCompression (0);
	// No cryptage
	m_FileHeader.SetCryptage(0);
	// Must not be greater than my current version
	m_FileHeader.SetVersion(CURRENT_VERSION);

	m_FileHeader.Send(*pFile);
}

////////////////////////////////////////////////////////////////////
// Sauvegarde rubriques items en cours
JVoid CRubriqueItemTree::SaveRubriques(JArchive *pFile)
{
	// Sauvegarde nombre de rubriques
	JUnt32 NbRubrique = m_Map_RubriquesItems.GetCount();
	pFile->Send(NbRubrique);

	// Sauvegarde de chaque rubrique
	for (m_Map_RubriquesItems.MoveFirst(); m_Map_RubriquesItems.IsValid(); m_Map_RubriquesItems.MoveNext())
	{
		// Puis pour chacune le nom de la rubrique
		JString TxtRubrique;
		TxtRubrique = m_Map_RubriquesItems.GetKey();
		TxtRubrique.Send(*pFile);

		// Type de rubrique (0 = pour items, 1 pour couplages etc....
		JUnt32 TypeRubrique = m_Type_Rubrique; // TYPE_RUBRIQUE_ITEM;
		pFile->Send(TypeRubrique); 

		// suivi Du nombre d'items associés
		JList <JInt32> &LstItems = m_Map_RubriquesItems.GetItem();  
		JUnt32 NbItems = LstItems.GetCount();
		pFile->Send(NbItems);

		// Et les idents des items associés
		for (LstItems.MoveFirst(); LstItems.IsValid(); LstItems.MoveNext())
		{
			JInt32 IdItem = LstItems.GetItem();
			pFile->Send(IdItem);	
		}
	}	

}

///////////////////////////////////////////////////////////////////
// Visibilité ou pas d'un item
bool CRubriqueItemTree::ShowItem(JInt32 IdItem)
{
	// On se place sur le map affichage via l'ident item
	m_MapAffichItems.MoveTo(IdItem);

	if (m_MapAffichItems.IsValid())
		// Renvoi état affichage item
		return m_MapAffichItems.GetItem(); 

	return false;
}


BEGIN_MESSAGE_MAP(CRubriqueItemTree, CTreeCtrl)
	//{{AFX_MSG_MAP(CRubriqueItemTree)
	ON_WM_LBUTTONDOWN()
	ON_WM_RBUTTONDOWN()
	//}}AFX_MSG_MAP


	ON_NOTIFY_REFLECT(TVN_BEGINLABELEDIT,			OnBeginLabelEdit)
	ON_NOTIFY_REFLECT(TVN_ENDLABELEDIT,				OnEndLabelEdit)
	ON_NOTIFY_REFLECT(TVN_BEGINDRAG,				OnBeginDrag)
	ON_NOTIFY_REFLECT(TVN_ITEMEXPANDING,			OnExpandingRubricItems)

	// Redessine texte des items
	ON_NOTIFY_REFLECT(NM_CUSTOMDRAW,				OnNMCustomdraw)

	ON_COMMAND(ID_RI_AJOUTER_RUBRIQUE,				OnCtxRubriqueAdd)
	ON_COMMAND(ID_RI_SUPPRIMER_RUBRIQUE,			OnCtxRubriqueSupp)
	ON_COMMAND(ID_RI_RENOMMER_RUBRIQUE,				OnCtxRubriqueRename)
	ON_COMMAND(ID_RI_SUPPRIMER_RUBRIQUE_ET_ITEMS,	OnCtxRubriqueEtItemsSupp)
	ON_COMMAND(ID_RI_CLASSER_ITEMS,					OnCtxClasserItems)
	ON_COMMAND(ID_RI_RUBRIQUE_PARDEFAUT,			OnCtxRubriquePardefaut)
	ON_UPDATE_COMMAND_UI(ID_RI_RUBRIQUE_PARDEFAUT,	OnUpdateCtxRubriquePardefaut)

	ON_WM_MOUSEMOVE()
	
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// Déploiement rubrique(s) items
void CRubriqueItemTree::OnLButtonDown(UINT nFlags, CPoint point) 
{
	UINT uFlags=0;
	HTREEITEM hti = HitTest(point,&uFlags);
	
	if( uFlags & (TVHT_ONITEMSTATEICON))
	{
		// Sélection du noeud courant
		ToggleItemState(hti);

		// Modifie sélection du noeud courant
		SetSelectTree(hti);

		// Mise à jour de la liste dans dialogue items
		UpdateItemList();
	}
	else
	{
		CTreeCtrl::OnLButtonDown(nFlags, point);
	}
}

/////////////////////////////////////////////////////////////////////////////
// Affichage menu contextuel
void CRubriqueItemTree::OnRButtonDown(UINT nFlags, CPoint point) 
{
	CTreeCtrl::OnLButtonDown(nFlags, point);
}

void CRubriqueItemTree::EnableEdit(bool bEnable)
{
	m_bEditable = bEnable;
	SetNewStyle(TVS_EDITLABELS, bEnable);
}

void CRubriqueItemTree::EnableExpandAll(bool bEnable)
{
	m_bExpandAll = bEnable;
}

void CRubriqueItemTree::EnableMultipleSel(bool bEnable)
{
	m_bMultiSel = bEnable;
}

//Permet de changer le style du tree
void CRubriqueItemTree::SetNewStyle(long lStyleMask, BOOL bSetBits)
{
	long        lStyleOld;

	lStyleOld = GetWindowLong(m_hWnd, GWL_STYLE);
	lStyleOld &= ~lStyleMask;
	if (bSetBits)
		lStyleOld |= lStyleMask;

	SetWindowLong(m_hWnd, GWL_STYLE, lStyleOld);
	SetWindowPos(NULL, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER);
}

void CRubriqueItemTree::SetSelectionColor(COLORREF selColor)
{
	m_selColor = selColor;
}

///////////////////////////////////////////////////////////////////////////////////////
// Mise à jour de la liste des items sélectionnées via rubriques items
void CRubriqueItemTree::UpdateItemList()
{
	// Se place sur la branche racine des rubriques
	HTREEITEM hti = GetRootItem();
	UpdateItemList(hti);

	// Prévient la boite de dialogue que l'affichage des items change
	// m_pDlgItems->PostMessage(WM_USER_ITEMLISTUPDATED, 0, 0);
	m_pDialog->PostMessage(WM_USER_ITEMLISTUPDATED, 0, 0); 
}

/////////////////////////////////////////////////////////////////////////
// Mise à jour affichage des items via le noeud rubrique
void CRubriqueItemTree::UpdateItemList(HTREEITEM hti)
{
	while(hti)
	{
		if (ItemHasChildren(hti))
		{
			// Si enfant existe, on passe à update niveau suivant
			UpdateItemList(GetChildItem(hti));
		}
		else
		{
			// Etat sélection rubrique
			UINT StateRubrique = GetItemState(hti, TVIS_STATEIMAGEMASK); 

			// Récup Texte rubrique
			CString TxtRubrique;
			TxtRubrique = GetItemText(hti);

			// On se positionne sur Map des rubriques
			m_Map_RubriquesItems.MoveTo(TxtRubrique);
			if (m_Map_RubriquesItems.IsValid())
			{
				// Ajout de tous les élmts items de cette rubrique pour affichage items
				JList <JInt32> &LstAffichItems = m_Map_RubriquesItems.GetItem();
				for (LstAffichItems.MoveFirst(); LstAffichItems.IsValid(); LstAffichItems.MoveNext()) 
				{
					// Récup id item et on l'ajoute à liste des items à afficher
					JInt32 IdItem = LstAffichItems.GetItem(); 
					
					// Etat affichage en cours de l'item
					m_MapAffichItems.MoveTo(IdItem);
					if (m_MapAffichItems.IsValid())
					{
						// Lise à jour état affichage via sélection ou pas de la rubrique
						bool &EtatAffich = m_MapAffichItems.GetItem();
						if((StateRubrique>>12) == 2)
							EtatAffich = true;
						else
							EtatAffich = false;

						if (m_pDlgItems)
						{
							if (m_pDlgItems->GetShowMode() == SHOWMODE_SIMPLE && m_Firstime)
								EtatAffich = true;
						}
					}
				}
			}
		}
		hti = GetNextItem(hti,TVGN_NEXT);
	}
}

////////////////////////////////////////////////////////////////////////
// Affichage menu contextuel gestion des rubriques
void CRubriqueItemTree::OnContextMenu(CWnd* pWnd, CPoint point) 
{
	if(m_bEditable)
	{
		HTREEITEM   hitem;
		UINT        flags;
		CMenu Menu;
		CPoint pt;
		pt = point;
		ScreenToClient(&pt);
		hitem = HitTest(pt, &flags);
		Menu.LoadMenu(IDR_RI_MENU_RUBRIQUES);

		LOADMENUITEMSTRING(Menu, ID_RI_AJOUTER_RUBRIQUE);
		LOADMENUITEMSTRING(Menu, ID_RI_SUPPRIMER_RUBRIQUE);
		LOADMENUITEMSTRING(Menu, ID_RI_RENOMMER_RUBRIQUE);
		LOADMENUITEMSTRING(Menu, ID_RI_SUPPRIMER_RUBRIQUE_ET_ITEMS);
		LOADMENUITEMSTRING(Menu, ID_RI_CLASSER_ITEMS);
		LOADMENUITEMSTRING(Menu, ID_RI_RUBRIQUE_PARDEFAUT);

		//si on click sur un item
		if (flags & TVHT_ONITEM)
		{
			// Sélection de l'item
			SelectItem(hitem);
			
			// Si item racine ou item rubrique divers >> pas de suppression ou renommage
			if (GetItemData(hitem) == ID_ROOT || GetItemData(hitem) == ID_RUBRIQUE_DIVERS)
			{
				// Suppression option suppression rubrique
				Menu.GetSubMenu(0)->DeleteMenu(ID_RI_SUPPRIMER_RUBRIQUE, MF_BYCOMMAND);

				// Suppression option renommer rubrique
				Menu.GetSubMenu(0)->DeleteMenu(ID_RI_RENOMMER_RUBRIQUE, MF_BYCOMMAND);

				// Suppression option suppression rubrique + items
				Menu.GetSubMenu(0)->DeleteMenu(ID_RI_SUPPRIMER_RUBRIQUE_ET_ITEMS, MF_BYCOMMAND);

				// Suppression séparation (sinon double séparation)
				Menu.GetSubMenu(0)->DeleteMenu(2, MF_BYPOSITION);

				
				if (m_Map_RubriquesItems.GetCount() <= 1)
				{
					// Si seulement 1 rubrique, on vire l'option classement
					Menu.GetSubMenu(0)->EnableMenuItem(ID_RI_CLASSER_ITEMS,MF_BYCOMMAND|MF_DISABLED|MF_GRAYED)?true:false;
				}

				// Avant Aout 2013
				/*
				if (GetItemData(hitem) == ID_ROOT || m_Type_Rubrique == TYPE_RUBRIQUE_PALETTE)
				{
					// Pour la racine rubrique pas de déf rubrique par défaut
					Menu.GetSubMenu(0)->DeleteMenu(ID_RI_RUBRIQUE_PARDEFAUT, MF_BYCOMMAND);
				}
				else
				{
					// Indique la rubrique par défaut
					if (hitem == m_hItemByDefault) 
						// Menu.GetSubMenu(0)->EnableMenuItem(ID_RI_RUBRIQUE_PARDEFAUT,MF_BYCOMMAND|MF_CHECKED);
						Menu.GetSubMenu(0)->CheckMenuItem(ID_RI_RUBRIQUE_PARDEFAUT, MF_CHECKED); 
					else
						// Menu.GetSubMenu(0)->EnableMenuItem(ID_RI_RUBRIQUE_PARDEFAUT,MF_BYCOMMAND|MF_UNCHECKED);
						Menu.GetSubMenu(0)->CheckMenuItem(ID_RI_RUBRIQUE_PARDEFAUT, MF_UNCHECKED); 
				}
				*/
				
				// Aout 2013
				// Indique la rubrique par défaut
				if (hitem == m_hItemByDefault) 
					// Menu.GetSubMenu(0)->EnableMenuItem(ID_RI_RUBRIQUE_PARDEFAUT,MF_BYCOMMAND|MF_CHECKED);
					Menu.GetSubMenu(0)->CheckMenuItem(ID_RI_RUBRIQUE_PARDEFAUT, MF_CHECKED); 
				else
					// Menu.GetSubMenu(0)->EnableMenuItem(ID_RI_RUBRIQUE_PARDEFAUT,MF_BYCOMMAND|MF_UNCHECKED);
					Menu.GetSubMenu(0)->CheckMenuItem(ID_RI_RUBRIQUE_PARDEFAUT, MF_UNCHECKED); 
				
				// Apparition menu rubriques
				Menu.GetSubMenu(0)->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, point.x, point.y, this);
			}
			else
			{
				/* Avant Aout 2013
				// Pas de rubrique par défaut dans le cas des palettes
				if (m_Type_Rubrique == TYPE_RUBRIQUE_PALETTE)
					Menu.GetSubMenu(0)->DeleteMenu(ID_RI_RUBRIQUE_PARDEFAUT, MF_BYCOMMAND);
				else
				{
					// Indique la rubrique par défaut
					if (hitem == m_hItemByDefault) 
						// Menu.GetSubMenu(0)->EnableMenuItem(ID_RI_RUBRIQUE_PARDEFAUT,MF_BYCOMMAND|MF_CHECKED);
						Menu.GetSubMenu(0)->CheckMenuItem(ID_RI_RUBRIQUE_PARDEFAUT, MF_CHECKED); 
					else
						// Menu.GetSubMenu(0)->EnableMenuItem(ID_RI_RUBRIQUE_PARDEFAUT,MF_BYCOMMAND|MF_UNCHECKED);
						Menu.GetSubMenu(0)->CheckMenuItem(ID_RI_RUBRIQUE_PARDEFAUT, MF_UNCHECKED); 
				}
				*/

				// Aout 2013
				// Indique la rubrique par défaut
				if (hitem == m_hItemByDefault) 
					// Menu.GetSubMenu(0)->EnableMenuItem(ID_RI_RUBRIQUE_PARDEFAUT,MF_BYCOMMAND|MF_CHECKED);
					Menu.GetSubMenu(0)->CheckMenuItem(ID_RI_RUBRIQUE_PARDEFAUT, MF_CHECKED); 
				else
					// Menu.GetSubMenu(0)->EnableMenuItem(ID_RI_RUBRIQUE_PARDEFAUT,MF_BYCOMMAND|MF_UNCHECKED);
					Menu.GetSubMenu(0)->CheckMenuItem(ID_RI_RUBRIQUE_PARDEFAUT, MF_UNCHECKED); 

				// Apparition menu rubriques
				Menu.GetSubMenu(0)->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, point.x, point.y, this);
			}
		}
	}
}

////////////////////////////////////////////////////////
//Menu contextuel ajout d'une rubrique item
void CRubriqueItemTree::OnCtxRubriqueAdd()
{
	// Mode ajout rubrique
	m_Mode_Edit = MODE_ADD_RUBRIQUE;

	// Nom d'une nouvelle rubrique à créer
	CString TxtNewRubrique;

	// Récupére nom nvlle rubrique 
	TxtNewRubrique = GetTxtNewRubrique();

	//Creation de l'element du tree
	TV_INSERTSTRUCT tvisRubrique;
	tvisRubrique.item.mask				= TVIF_PARAM|TVIF_STATE|TVIF_TEXT;
	tvisRubrique.item.pszText			= (char*)(LPCTSTR)TxtNewRubrique;
	tvisRubrique.item.lParam			= (LPARAM)NULL; 
	tvisRubrique.item.iImage			= 0;
	tvisRubrique.item.iSelectedImage	= 0;
	tvisRubrique.hInsertAfter			= TVI_LAST;
	tvisRubrique.item.state				= (0x0001)|INDEXTOSTATEIMAGEMASK(1);
	tvisRubrique.item.stateMask			= TVIS_STATEIMAGEMASK|0x000F; 
	tvisRubrique.hParent				= GetRootItem();

	// insertion rubrique (itemdata = 1)
	HTREEITEM hNewItem = InsertItem (&tvisRubrique);
	SetItemData(hNewItem, ID_RUBRIQUE_CLIENT + m_CptRubrique);
	m_CptRubrique++;
	
	// selection du nouvel element
	BOOL bret = SelectItem (hNewItem);

	//on lance l'edition du label
	CEdit* pmyEdit = EditLabel(hNewItem);
	ASSERT(pmyEdit != NULL);
}

////////////////////////////////////////////////////////////////////////
// Ajout d'une rubrique (sans items - vierge)
bool CRubriqueItemTree:: AddRubrique(HTREEITEM htiRubrique )
{
	// Récupère le texte
	CString TxtRubrique = GetItemText(htiRubrique);

	// Test si non présent
	m_Map_RubriquesItems.MoveTo(TxtRubrique); 
	if (!m_Map_RubriquesItems.IsValid())
	{
		m_Map_RubriquesItems.Add(TxtRubrique);
		return true;
	}

	return false;
}	


/////////////////////////////////////////////////////////////////////////
// Fabrique texte pour nouvelle rubrique
CString CRubriqueItemTree::GetTxtNewRubrique()
{
	// Par défaut ce sera Nouvelle Rubrique
	CString TxtNewRubrique;
	TxtNewRubrique.LoadString(IDS_RI_NEW_RUBRIQUE);
	int LgTxtNewRubrique = TxtNewRubrique.GetLength(); 

	// N° en cours nouvelle rubrique
	int NoRubrique = 0;

	// Recherche nom de rubrique dispo
	HTREEITEM hItem = GetRootItem();
	if (ItemHasChildren(hItem))
	{
		hItem = GetChildItem(hItem);
		while (hItem != NULL)
		{
			// Récupère texte branche rubrique
			CString TxtRubrique = GetItemText(hItem);

			// Vérifie qu'aucune ne s'appelle "Nouvelle Rubrique xxxx"
			int Pos = TxtRubrique.Find(TxtNewRubrique,0);
			if (Pos >= 0)
			{
				// Récup n° rubrique
				CString Txt = TxtRubrique.Mid(Pos+LgTxtNewRubrique);
				if (atoi(TxtRubrique.Mid(Pos+LgTxtNewRubrique)) >= NoRubrique)
					NoRubrique = atoi(TxtRubrique.Mid(Pos+LgTxtNewRubrique)) + 1;
			}
			
			// Passe à la branche Rubrique suivante
			hItem = GetNextSiblingItem(hItem);
		}

		// Constitue le nom de la nouvelle Rubrique à créer
		if (NoRubrique > 0)
			TxtNewRubrique.Format("%s %d",TxtNewRubrique,NoRubrique);
	}

	return TxtNewRubrique;
}

/////////////////////////////////////////////////////////////////////////
// Ajout d'un item dans la rubrique divers
void CRubriqueItemTree::AddItemRubriqueDivers(JInt32 IdItem, JLabel LabelItem)
{
	if (m_hRubriqueDivers)
		// Appel ajout item dans la rubrique divers
		AddItemRubrique(m_hRubriqueDivers, IdItem, LabelItem);
	else
	{
		// A voir il faudrait créer une rubrique divers
		// ???? A FAIRE
	}
}


/////////////////////////////////////////////////////////////////////////
// Ajout d'un item dans une rubrique
void CRubriqueItemTree::AddItemRubrique(HTREEITEM htiRubrique, JInt32 IdItem, JLabel LabelItem)
{
	// Texte associé à la branche divers
	CString TxtRubrique = GetItemText(htiRubrique);

	// Etat sélection rubrique
	UINT StateRubrique = GetItemState(htiRubrique, TVIS_STATEIMAGEMASK); 

	// Récupère la liste des items associés à cette rubrique
	m_Map_RubriquesItems.MoveTo(TxtRubrique);
	if (m_Map_RubriquesItems.IsValid())
	{
		// Ajout item à la liste, si non déja présent
		JList <JInt32> &LstItems = m_Map_RubriquesItems.GetItem();

		// Test si non déjà présent
		bool ItemPresent = false;
		for (LstItems.MoveFirst(); LstItems.IsValid(); LstItems.MoveNext())
		{
			JInt32 IdItemCur = LstItems.GetItem();
			if (IdItem == IdItemCur)
			{
				ItemPresent = true;
				break;
			}
		}
		// Si non présent on l'ajoute à la liste des items de la rubrique
		if (!ItemPresent)
			LstItems.AddTail() = IdItem; 
	
		// Puis on initialise l'élmt map item affichage
		m_MapAffichItems.MoveTo(IdItem);
		if (!m_MapAffichItems.IsValid())
		{
			bool &EtatItem = m_MapAffichItems.Add(IdItem);

			// Positionne flag sélection afficahge via état sélection rubrique
			if((StateRubrique>>12) == 2)
				EtatItem = true;
			else
				EtatItem = false;
		}
	}
	
	// Mise à jour de la liste des items à afficher
	UpdateItemList();
}

////////////////////////////////////////////////////////
//Menu contextuel suppression d'une rubrique item
void CRubriqueItemTree::OnCtxRubriqueSupp()
{
	// Récupère élmt tree rubriques
	HTREEITEM htiRubrique = GetSelectedItem();

	// Vérifie que c'est bien une rubrique et que ce n'est pas la rubrique divers
	if (GetItemData(htiRubrique) != ID_ROOT && GetItemData(htiRubrique) != ID_RUBRIQUE_DIVERS)
	{
		
		// Nom de la rubrique à supprimer
		CString TxtRubrique;
		TxtRubrique = GetItemText(htiRubrique);

		int res = IDNO;

		// Titre fenêtre prévention suppression
		CString ttl;
		ttl = "Atelier Presse";

		if (htiRubrique)
		{
			CString txt;
			switch ( GetProfondeur(htiRubrique) )
			{
			case 0:
				// niveau root rubrique
				// A FAIRE éventuellement détruire toutes les rubriques
				break;

			case 1:
				//niveau rubrique	
				txt.LoadString(IDS_RI_SUPP_RUBRIQUE); 
				txt.Format("%s : %s","Suppression de la rubrique",TxtRubrique);
				res = this->MessageBox( txt,ttl, MB_YESNO | MB_ICONQUESTION);
				break;

			default:
				res = IDNO;
			}

			if ( res == IDYES)
			{
				// Remettre rubrique duvers si c'était la rubrique par défaut
				if (GetItemByDefault() == htiRubrique)
				{
					// Repositionne la rubrique par défaut comme rubrique divers
					SetItemByDefault(m_hRubriqueDivers);

					// Aout 2013
					// Sauvegarde rubrique par defaut
					CString TxtRubrique = GetItemText(htiRubrique);

					if (m_Type_Rubrique == TYPE_RUBRIQUE_PALETTE)
						AfxGetApp()->WriteProfileString("Options\\RepItem", "RubriquePalette", TxtRubrique);
					else
						AfxGetApp()->WriteProfileString("Options\\RepItem", "RubriqueItem", TxtRubrique);

				}

				// puis on la supprime du map
				SuppRubrique(htiRubrique);

				//on le retire du tree
				DeleteItem(htiRubrique);
				GetParent()->Invalidate();
			}
		}
	}

	// Mise à jour de la liste des items à afficher
	UpdateItemList();
}

//////////////////////////////////////////////////////////////////
//Menu contextuel suppression d'une rubrique item et de ses items
void CRubriqueItemTree::OnCtxRubriqueEtItemsSupp()
{
	// Récupère élmt tree rubriques
	HTREEITEM htiRubrique = GetSelectedItem();

	// Vérifie que c'est bien une rubrique et que ce n'est pas la rubrique divers
	if (GetItemData(htiRubrique) != ID_ROOT && GetItemData(htiRubrique) != ID_RUBRIQUE_DIVERS)
	{

		// Nom de la rubrique à supprimer
		CString TxtRubrique;
		TxtRubrique = GetItemText(htiRubrique);

		int res = IDNO;

		// Titre fenêtre prévention suppression
		CString ttl;
		ttl = "Atelier Presse";

		if (htiRubrique)
		{
			CString txt;
			switch ( GetProfondeur(htiRubrique) )
			{
			case 0:
				// niveau root rubrique
				// A FAIRE éventuellement détruire toutes les rubriques
				break;

			case 1:
				//niveau rubrique	
				txt.LoadString(IDS_RI_SUPP_RUBRIQUE); 
				txt.Format("%s : %s","Suppression de la rubrique ",TxtRubrique);
				res = this->MessageBox( txt,ttl, MB_YESNO | MB_ICONQUESTION);
				break;

			default:
				res = IDNO;
			}

			if ( res == IDYES)
			{
				// Remettre rubrique duvers si c'était la rubrique par défaut
				if (GetItemByDefault() == htiRubrique)
				{
					// Repositionne la rubrique par défaut comme rubrique divers
					SetItemByDefault(m_hRubriqueDivers);

					// Aout 2013
					// Sauvegarde rubrique par defaut
					CString TxtRubrique = GetItemText(htiRubrique);

					if (m_Type_Rubrique == TYPE_RUBRIQUE_PALETTE)
						AfxGetApp()->WriteProfileString("Options\\RepItem", "RubriquePalette", TxtRubrique);
					else
						AfxGetApp()->WriteProfileString("Options\\RepItem", "RubriqueItem", TxtRubrique);
				}

				// puis on la supprime du map
				SuppRubriqueEtItems(htiRubrique);

				//on le retire du tree
				DeleteItem(htiRubrique);
				GetParent()->Invalidate();
			}
		}
	}

	// Mise à jour de la liste des items à afficher
	UpdateItemList();
}

/////////////////////////////////////////////////////////////////////////
// Suppression d'une rubrique (attention si elle contient des items,
// on les reportera dans rubrique divers)
bool CRubriqueItemTree:: SuppRubrique(HTREEITEM htiRubrique)
{
	// Récupère le texte
	CString TxtRubrique = GetItemText(htiRubrique);

	// Test si présent pour le supprimer
	m_Map_RubriquesItems.MoveTo(TxtRubrique); 
	if (m_Map_RubriquesItems.IsValid())
	{
		// Récup liste des items associés
		JList <JInt32> &LstItems = m_Map_RubriquesItems.GetItem();
		JList <JInt32> LstSuppItems;
		LstSuppItems.Swap(LstItems); 
		if (LstSuppItems.GetCount() > 0)
		{
			// des items associés existent >> report dans rubrique divers
			for (LstSuppItems.MoveFirst(); LstSuppItems.IsValid(); LstSuppItems.MoveNext())
			{
				// Récup ident item, et ajout dans rubrique divers	
				JInt32 IdItem = LstSuppItems.GetItem();
				AddItemRubriqueDivers(IdItem,"");
			}
		}

		// Puis suppression rubrique du map
		m_Map_RubriquesItems.MoveTo(TxtRubrique); 
		m_Map_RubriquesItems.Remove(); 

		// Puis sauvegarde dernières modifs dans fichier
		SaveFileRubrique();

		return true;
	}
	return false;
}

/////////////////////////////////////////////////////////////////////////
// Suppression d'une rubrique (attention si elle contient des items,
// on les reportera dans rubrique divers)
bool CRubriqueItemTree:: SuppRubriqueEtItems(HTREEITEM htiRubrique)
{
	// Récupère le texte
	CString TxtRubrique = GetItemText(htiRubrique);

	// Test si présent pour le supprimer
	m_Map_RubriquesItems.MoveTo(TxtRubrique); 
	if (m_Map_RubriquesItems.IsValid())
	{
		// Récup liste des items associés
		JList <JInt32> &LstItems = m_Map_RubriquesItems.GetItem();
		JList <JInt32> LstSuppItems;
		LstSuppItems.Swap(LstItems); 
		if (LstSuppItems.GetCount() > 0)
		{
			// Tableau des items en cours
			JMap <JUnt32,CString> m_MapItems;	
			JList<IItem const *> vItems;
			m_MapItems.Reset();
 			this->m_pItemsConstruits->GetItemsIndependant(vItems,m_pTerrain);

			// Get only those valid for this terrain
			for (vItems.MoveFirst(); vItems.IsValid(); vItems.MoveNext())
			{
				// Ident de l'item
				JInt32 IdItem		= vItems.GetItem()->GetID();

				// Label de l'item
				JLabel LabelItem	= vItems.GetItem()->GetLabel(); 

				// Ajoute infos items
				m_MapItems.Add(IdItem) = LabelItem.AsJCharPtr();   
			}
			
			// On affiche par précaution la liste des items à virer
			CString TxtItems = "Voulez-vous supprimer également les items de cette rubrique ? \n\n";

			// des items associés existent >> report dans rubrique divers
			for (LstSuppItems.MoveFirst(); LstSuppItems.IsValid(); LstSuppItems.MoveNext())
			{
				// Récup ident item
				JInt32 IdItem = LstSuppItems.GetItem();

				m_MapItems.MoveTo(IdItem);   			
				if (m_MapItems.IsValid())
				{
					// Récup label item
					TxtItems += m_MapItems.GetItem();	
					TxtItems += "\n";
				}
			}

			if (AfxMessageBox(TxtItems,MB_ICONQUESTION | MB_YESNO) == IDYES)
			{
				// des items associés existent >> report dans rubrique divers
				for (LstSuppItems.MoveFirst(); LstSuppItems.IsValid(); LstSuppItems.MoveNext())
				{
					// Récup ident item, et on les supprime si c'est possible,
					// sinon on les reporte dans rubrique divers	
					JInt32 IdItem = LstSuppItems.GetItem();

					// Vérifier si on peut détruire l'item construit
					if (m_pDlgItems && !m_pDlgItems->GetCurMode()->DeleteItemConstruit(IdItem))
					{
						// Suppression impossible, on transfére l'item dans rubrique divers
						AddItemRubriqueDivers(IdItem,"");
					}
				}
			}
			else
			{
				// On conserve les items dans rubrique général "divers", mais on vire la rubrique
				// des items associés existent >> report dans rubrique divers
				for (LstSuppItems.MoveFirst(); LstSuppItems.IsValid(); LstSuppItems.MoveNext())
				{
					// Récup ident item
					JInt32 IdItem = LstSuppItems.GetItem();

					// On transfére l'item dans rubrique divers
					AddItemRubriqueDivers(IdItem,"");
				}
			}
		}

		// Puis suppression rubrique du map
		m_Map_RubriquesItems.MoveTo(TxtRubrique); 
		m_Map_RubriquesItems.Remove(); 

		// Puis sauvegarde dernières modifs dans fichier
		SaveFileRubrique();

		return true;
	}
	return false;
}

////////////////////////////////////////////////////////
//Menu contextuel renommer d'une rubrique item
void CRubriqueItemTree::OnCtxRubriqueRename()
{
	// Mode rename rubrique
	m_Mode_Edit = MODE_RENAME_RUBRIQUE;

	// Item rubrique sélectionner
	HTREEITEM htiRubrique = GetSelectedItem();

	// Vérifie que c'est bien une rubrique et que ce n'est pas la rubrique divers
	if (GetItemData(htiRubrique) != ID_ROOT && GetItemData(htiRubrique) != ID_RUBRIQUE_DIVERS)
	{
		// Récup ancien label rubrique
		CString OldTxtRubrique;
		OldTxtRubrique = GetItemText(GetSelectedItem());

		// Passe en mode édition
		CEdit* pmyEdit = EditLabel(GetSelectedItem());
		ASSERT(pmyEdit != NULL);

		// Renomme dans le map la rubrique
		// RenameRubrique(htiRubrique, OldTxtRubrique);
	}
}

/////////////////////////////////////////////////////////////////////////
// Renomme une rubrique
bool CRubriqueItemTree:: RenameRubrique(HTREEITEM htiRubrique, CString OldLabel)
{
	// Vérifier que la rubrique existe
	m_Map_RubriquesItems.MoveTo(OldLabel);
	if (m_Map_RubriquesItems.IsValid())
	{
		// Il faut en fait récupérer la liste des items assiociés
		JList <JInt32> & LstItems = m_Map_RubriquesItems.GetItem();

		// Nouveau label rubrique
		CString TxtNewRubrique = GetItemText(htiRubrique);

		// Ajouter un nouvel elmt avec les mêmes items associés
		m_Map_RubriquesItems.MoveTo(TxtNewRubrique);
		if (!m_Map_RubriquesItems.IsValid())
		{
			JList <JInt32> & NewLstItems = m_Map_RubriquesItems.Add(TxtNewRubrique);
			if (LstItems.GetCount() > 0)
			{
				// des items associés existent >> report dans rubrique divers
				for (LstItems.MoveFirst(); LstItems.IsValid(); LstItems.MoveNext())
				{
					// Récup ident item, et ajout dans rubrique divers	
					NewLstItems.AddTail() = LstItems.GetItem();
				}
			}

			// Supprimer elmt rubrique du map
			m_Map_RubriquesItems.MoveTo(OldLabel);
			m_Map_RubriquesItems.Remove();

			return true;
		}
	}
	return false;
}

//////////////////////////////////////////////////////////////////////////////////////
// Début édition libellé rubriqe
void CRubriqueItemTree::OnBeginLabelEdit(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMTVDISPINFO pTVDispInfo = reinterpret_cast<LPNMTVDISPINFO>(pNMHDR);
	
	// Par défaut on sera en mode "renommer rubrique"
	*pResult = 0;
}

////////////////////////////////////////////////////////////////////////////////
// Validation ou pas d'une nouvelle rubrique
void CRubriqueItemTree::OnEndLabelEdit(LPNMHDR pnmhdr, LRESULT *pLResult)
{
	TV_DISPINFO     *ptvinfo;
	ptvinfo = (TV_DISPINFO *)pnmhdr;

	// Test avant de valider le nom de l'item, s'il n'existe pas déjà
	HTREEITEM hRubrique = GetSelectedItem();

	// Item root et item divers non renommable
	if (hRubrique == m_hRoot || hRubrique == m_hRubriqueDivers)
		return;

	// Ancien text
	CString OldTxtRubrique;
	OldTxtRubrique = GetItemText(hRubrique);
	
	// Vérifie texte non déjà utilisé
	m_Map_RubriquesItems.MoveTo(ptvinfo->item.pszText);
	if (m_Map_RubriquesItems.IsValid())
	{
		// Nom rubrique déjà existant
		CString Txt;
		Txt.LoadString(IDS_RI_RUBR_DEJA_EXISTANTE); 
		AfxMessageBox(Txt);
	}
	
	//si le texte entré est valide
	else if (ptvinfo->item.pszText != NULL )
	{
		//on renomme l'element du tree
		ptvinfo->item.mask = TVIF_TEXT;
		SetItem(&ptvinfo->item);
	}
	
	// Puis mis à jour map rubrique soit en mode ajout, soit en mode renommage
	if (hRubrique)
	{
		if (m_Mode_Edit == MODE_ADD_RUBRIQUE)
			// Ajout d'une rubrique
			AddRubrique(hRubrique);
		else if (m_Mode_Edit == MODE_EDIT_DIRECT || m_Mode_Edit == MODE_RENAME_RUBRIQUE)
			// Renommer une rubrique (via menu, ou directement)
			RenameRubrique(hRubrique, OldTxtRubrique);

		// Puis sauvegarde dernières modifs dans fichir
		SaveFileRubrique();
	}

	// Tri la liste des rubriques
	TriRubriques();

	// Repositionne mode edit libellé
	m_Mode_Edit = MODE_EDIT_DIRECT;

	*pLResult = FALSE;
}

BOOL CRubriqueItemTree::PreTranslateMessage(MSG* pMsg) 
{
	// When an item is being edited make sure the edit control
	// receives certain important key strokes
	if (GetEditControl())
	{
		::TranslateMessage(pMsg);
		::DispatchMessage(pMsg);
		return TRUE; // DO NOT process further
	}
	return CTreeCtrl::PreTranslateMessage(pMsg);
}


//////////////////////////////////////////////////////////////////////////
// Démarrage drag&drop d'un item ou de plusieurs items
void CRubriqueItemTree::OnBeginDrag(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);
	*pResult = TRUE;
}

////////////////////////////////////////////////////////////////////////////////////////////
// Cours cicuite le déploiement des rubriques (pas de déploiement si items)
void CRubriqueItemTree::OnExpandingRubricItems(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);
	
	// A partir du niveau 2, pas de déploiement
	if (this->GetProfondeur(pNMTreeView->itemNew.hItem) > 0)
	{
		*pResult = 1;
	}
	else
	{
		// Désinhibe les enfants des branches rubriques
		SetEnfant(pNMTreeView->itemNew.hItem,0);
		
		*pResult = 0;
	}
}

//////////////////////////////////////////////////////////////////////////
// Va dans le classement des items
void CRubriqueItemTree::OnCtxClasserItems()
{
	// Affichage de la fenêtre dialog classement items
	// CDlgClassementItems DlgClassementItems(m_pDlgItems);
	CDlgClassementItems DlgClassementItems(m_pDialog);
	
	// Remplissage liste des rubriques sources
	// DlgClassementItems.m_pDlgItems = this->m_pDlgItems;
	DlgClassementItems.m_pItemsConstruits	= this->m_pItemsConstruits;
	DlgClassementItems.m_pTerrain			= this->m_pTerrain;

	DlgClassementItems.m_pItemsConstruits	= m_pDlgItems->GetItems();
	DlgClassementItems.m_pTerrain			= m_pDlgItems->GetTerrain();

	// Récup la nomenclature en cours
	DlgClassementItems.m_pTreeRubrique = this;

	// Ouverture de la boite
	DlgClassementItems.DoModal();

	// Mise à jour de la liste des items à afficher
	UpdateItemList();

	// Sauvegarde dernière modif
	SaveFileRubrique();
}

////////////////////////////////////////////////////////////////////////////////////////////////
// Redéfinition de la rubrique par défaut
void CRubriqueItemTree::OnCtxRubriquePardefaut()
{
	// Récupère élmt tree rubriques
	HTREEITEM htiRubrique = GetSelectedItem();

	// Repositionne la rubrique par défaut
	SetItemByDefault(htiRubrique);

	// Aout 2013
	// Sauvegarde rubrique par defaut
	CString TxtRubrique = GetItemText(htiRubrique);

	if (m_Type_Rubrique == TYPE_RUBRIQUE_PALETTE)
		AfxGetApp()->WriteProfileString("Options\\RepItem", "RubriquePalette", TxtRubrique);
	else
		AfxGetApp()->WriteProfileString("Options\\RepItem", "RubriqueItem", TxtRubrique);

	// Update design
	this->Invalidate();

}

void CRubriqueItemTree::OnUpdateCtxRubriquePardefaut(CCmdUI *pCmdUI)
{
	// ??? ça marche pas, il faut le faire avant le TrackPopup

	/*
	// Récupère élmt tree rubrique sélectionné
	HTREEITEM htiRubrique = GetSelectedItem();

	// Récupère la rubrique par défaut
	HTREEITEM htiRubriqueDefaut = GetItemByDefault();

	// Check la rubrique par défaut
	if (htiRubriqueDefaut == htiRubrique) 
		pCmdUI->SetCheck(TRUE);
	else
		pCmdUI->SetCheck(FALSE);
	*/
}

// Désactive ou réactive affichage des enfants des branches fils
void CRubriqueItemTree::SetEnfant(HTREEITEM hti, JUnt32 FlagEnfant)
{
	// Branche rubrique enfant existante
	if (ItemHasChildren(hti))
	{
		// Branche divers
		hti = GetChildItem(hti);
		while (hti)
		{
			// Inhibe le flag children
			TVITEM tv_item = {0};
			tv_item.hItem = hti;
			tv_item.mask = TVIF_HANDLE | TVIF_CHILDREN;
			GetItem(&tv_item);
			tv_item.cChildren = FlagEnfant;
			SetItem(&tv_item);

			// Passe à la rubrique suivante
			hti = GetNextItem(hti,TVGN_NEXT);
		}
	}
}

///////////////////////////////////////////////////////////////////////////
// Test mouse Move
void CRubriqueItemTree::OnMouseMove(UINT nFlags, CPoint point)
{
	CThemeTree::OnMouseMove(nFlags, point);
}

///////////////////////////////////////////////////////////////////////////
// expand de la totalité des rubriques
void CRubriqueItemTree::ExpandRubriqueDivers()
{
	if (m_pDlgItems)
	{
		if (m_pDlgItems->GetShowMode() == SHOWMODE_SIMPLE)
		{
			this->SetCheck(m_hRoot, true); 
			this->SetCheck(m_hRubriqueDivers, true);
			this->ExpandAll();
			UpdateItemList(m_hRubriqueDivers);

			m_Firstime = false;
		}
	}
}



