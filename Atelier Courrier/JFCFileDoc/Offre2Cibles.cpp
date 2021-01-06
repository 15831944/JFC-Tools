// DlgOffre2_Cibles.cpp : fichier d'implémentation
//

#include "stdafx.h"

#include "Offre2Cibles.h"
#include "Offre2MainDialog.h"
#include "ItemsConstruitsManager.h"
#include "ItemsConstruitsBase.h"
#include "../RepItemsExt/Resource.h"

// headers pour les données
#include "IItem.h"
#include "Terrain.h"
#include "OffreTerrainManager.h"

// header STL
#include <set>
#include <algorithm>
#include <functional>
#include <stdext.hpp>

#include "JFCConfig.h"
#include "../JFCOffre/resource.h" // pour le contexte menu 'reference affinité'

// timer mise à jour chaine de recherche
#define IdTimer_SearchStrTargets WM_USER + 201


//=============================================================================	
// forward declarations des functors internes au fichier
struct IItemHasId;

//=============================================================================	
// Boîte de dialogue COffre2Cibles

IMPLEMENT_DYNAMIC(COffre2Cibles, COffre2OngletBase)

//=============================================================================	
// Gestionnaires de messages de COffre2Cibles
//=============================================================================	

BEGIN_MESSAGE_MAP(COffre2Cibles, COffre2OngletBase)
	ON_WM_TIMER()
	ON_WM_CTLCOLOR()
	ON_LBN_SELCHANGE		(IDC_LST_ITEMS_PALETTES,		&COffre2Cibles::OnLbnSelchangeDisplayChoiceLst)
	ON_EN_CHANGE			(IDC_CIBLES_SEARCH_STR,			&COffre2Cibles::OnEnChangeCiblesSearchStr)
	ON_NOTIFY				(NM_CLICK, IDC_FD_TREE_CIBLES,	&COffre2Cibles::OnNMClickFdTreeCibles)
	ON_EN_CHANGE			(IDC_CIBLES_SEARCH_STR,			&COffre2Cibles::OnEnChangeCiblesSearchStr)
	ON_BN_CLICKED			(IDC_RAZSELECT,					&COffre2Cibles::OnBnClickedToutselect)
	ON_LBN_SELCHANGE		(IDC_LST_CIBLES,				&COffre2Cibles::OnLbnSelchangeLstCibles)
	ON_REGISTERED_MESSAGE	(WMU_SOURCE_ELEMENTS_CHANGE,					OnSourceElementsChange)
END_MESSAGE_MAP()

//=============================================================================	
static const char * ppLibellesFolders[]	= {"Items", "Palettes", '\0'}; 

CString csDivers;

//=============================================================================	
COffre2Cibles::COffre2Cibles(COffre2MainDialog * pDialogOffre)
	:	COffre2OngletBase(IDD, pDialogOffre)
	,	m_SearchStr(_T(""))
	,	m_TargetList(pDialogOffre)
//=============================================================================	
{
	// par défaut on affiche les items
	m_CurDisplay = effItems;
//	m_pItemsConstruits = CItemsConstruitsManager::GetInstance()->GetItemsInstance();

	m_bItemsLoaded = false;
	m_bPalettesLoaded = false;

	m_bItemsDependOnTerrainsLoaded = false;
	m_bPalettesDependOnTerrainsLoaded = false;

	m_hRoot = 0;
	m_hRubriqueDivers = 0;

	// on indique que c'est le 1er affichage
	m_bIsFirstDisplayItems = true;
	m_bIsFirstDisplayPalettes = true;

	// on indique que c'est la 1ère initialisation
	m_bFirstInit = true;

	csDivers.LoadString(IDS_RI_DIVERS);
}

//=============================================================================	
COffre2Cibles::~COffre2Cibles()
//=============================================================================	
{
	FreeRubrikMap(effItems);
	FreeRubrikMap(effPalettes);
}

//=============================================================================	
void COffre2Cibles::DoDataExchange(CDataExchange* pDX)
//=============================================================================	
{
	COffre2OngletBase::DoDataExchange(pDX);
	DDX_Control	(pDX,	IDC_FD_TREE_CIBLES,			m_TreeCibles);
	DDX_Text	(pDX,	IDC_CIBLES_SEARCH_STR,		m_Filter.strSearch);
	DDX_Control	(pDX,	IDC_LST_ITEMS_PALETTES,		m_DisplayChoiceList);
	DDX_Control	(pDX,	IDC_ELEMS_INDICATOR,		m_indicator);
	DDX_Control	(pDX,	IDC_RAZSELECT,				m_RAZCiblesSelBtn);
	DDX_Control	(pDX,	IDC_FD_STATIC_TTLTITRES,	m_ttlCibles);
	DDX_Control	(pDX,	IDC_FD_STATIC_TTLFILTRES,	m_ttlFiltres);
	DDX_Control	(pDX,	IDC_LST_CIBLES,				m_TargetList);
}

//=============================================================================	
BOOL COffre2Cibles::OnInitDialog()
//=============================================================================	
{
	COffre2OngletBase::OnInitDialog();

	SetImageList();

	// on assigne les teplates à la liste d'affichage des cibles
	m_TargetList.SetAppColor(m_pDlgOffre->GetAppColor());

	// onn indique que l'on va avoir besoin de charger les nouveaux elements
	m_bNewItemsReloaded = false;
	m_bNewPalettesReloaded = false;

	m_sLastSearchStr.Empty();
	m_Filter.strSearch.Empty();

	//////////
	// couleurs des labels

	// label 'filtres'
	m_ttlFiltres.SetFontName(CATPColors::GetFontLabelName());
	m_ttlFiltres.SetFontSize(CATPColors::GetFontLabelSize());
	m_ttlFiltres.SetBorder(false);
	m_ttlFiltres.SetBkColor(CATPColors::GetColorDark(m_pDlgOffre->GetAppColor()),m_pDlgOffre->GetAppColor(),CATPColors::DARK_DEGRAD);
	m_ttlFiltres.SetTextColor(CATPColors::GetColorSuperDark(m_pDlgOffre->GetAppColor()));

	// label 'cibles'
	m_ttlCibles.SetFontName(CATPColors::GetFontLabelName());
	m_ttlCibles.SetFontSize(CATPColors::GetFontLabelSize());
	m_ttlCibles.SetBorder(false);
	m_ttlCibles.SetBkColor(CATPColors::GetColorDark(m_pDlgOffre->GetAppColor()),m_pDlgOffre->GetAppColor(),CATPColors::DARK_DEGRAD);
	m_ttlCibles.SetTextColor(CATPColors::GetColorSuperDark(m_pDlgOffre->GetAppColor()));

	// bouton 'RAZ' sélection de cibles
	const char pszTooltip [] = "Permet de sélectionner/déselectionner toutes les cibles affichées";	
	m_RAZCiblesSelBtn.SetTooltipText(pszTooltip);
	m_RAZCiblesSelBtn.SetColor(CButtonST::BTNST_COLOR_BK_IN,	CATPColors::GetColorLight(m_pDlgOffre->GetAppColor()),m_pDlgOffre->GetAppColor(),0);
	m_RAZCiblesSelBtn.SetColor(CButtonST::BTNST_COLOR_BK_OUT,	CATPColors::GetColorMedium(m_pDlgOffre->GetAppColor()),m_pDlgOffre->GetAppColor(),0);
	m_RAZCiblesSelBtn.SetColor(CButtonST::BTNST_COLOR_BK_FOCUS,	CATPColors::GetColorDark(m_pDlgOffre->GetAppColor()),m_pDlgOffre->GetAppColor());

	// on définit la racine de l'arbre items/palettes
	m_hRoot = m_TreeCibles.InsertItem(TVIF_CHILDREN|TVIF_PARAM|TVIF_STATE|TVIF_TEXT, "",0,0,(0x0001)|INDEXTOSTATEIMAGEMASK(etisNotSelected),TVIS_STATEIMAGEMASK|0x000F,0,0,0);	
	m_TreeCibles.SetItemData(m_hRoot, eidtrRoot);
	ASSERT(m_hRoot);

	//////////
	// on remplit la listbox qui gère l'affichage des items ou des palettes

	// on remplit la liste des libellés de filtre
	m_DisplayChoiceList.ResetContent();
	const char ** ppLibelle = ppLibellesFolders;
	while(*ppLibelle)
	{
		// on ajoute le libellé, et on définit sa couleur de fond
		int idx = m_DisplayChoiceList.AddLine(*ppLibelle, m_pDlgOffre->GetAppColorTemplate());
		ASSERT(idx != CB_ERR);
		ppLibelle++;
	}
	ASSERT(m_DisplayChoiceList.GetCount() == effNumFolders);

	// on sélectionne items ou palettes dans la listbox
	m_DisplayChoiceList.SetCurSel(m_CurDisplay);

	// on définit la reference affinite dans la liste de cibles
	ASSERT(m_pDlgOffre->GetCiblesSelection().ReferenceAffinite > 0);
	m_TargetList.SetReferenceAffiniteItem(
		CItemsConstruitsManager::GetInstance()->GetItemsInstance()->GetIItemByID(m_pDlgOffre->GetCiblesSelection().ReferenceAffinite));

	// on charge les terrains et la source sélectionnés
	ReloadTerrainsSelection();	

	// on force le chargement des elements permettant le cacul des seuils (terrains, source)
	m_TargetList.ReloadCacheSeuilsElements();

	// on force le chargement et l'affichage des elements (items ou palettes)
	OnLbnSelchangeDisplayChoiceLst();

	return TRUE;
}

//=============================================================================	
HBRUSH COffre2Cibles::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
//=============================================================================	
{
	int ctrlid;

	switch (nCtlColor)
	{
		// on paint le dialogue lui même
		case CTLCOLOR_DLG:
			return m_pDlgOffre->GetBackgroundBrush();

		case CTLCOLOR_BTN:
		case CTLCOLOR_STATIC:

			ctrlid = pWnd->GetDlgCtrlID();
			// on paint l'indicateur de la couleur claire
			if (ctrlid == IDC_ELEMS_INDICATOR)
			{
				pDC->SetBkColor(CATPColors::GetColorLight(m_pDlgOffre->GetAppColor()));    // change the background
				return static_cast<HBRUSH> (m_pDlgOffre->GetBackgroundBrush());
			}
			else
			{
				// on 'paint' tout le reste en transparent
				pDC->SetBkMode(TRANSPARENT);
				return (HBRUSH)::GetStockObject(NULL_BRUSH);
			}
			break;

		case CTLCOLOR_EDIT:
		case CTLCOLOR_MSGBOX:
		
			// on paint l'indicateur de la couleur claire
			if (pWnd->GetDlgCtrlID() == IDC_ELEMS_INDICATOR)
			{
				pDC->SetBkColor(CATPColors::GetColorLight(m_pDlgOffre->GetAppColor()));    // change the background
				return static_cast<HBRUSH> (m_pDlgOffre->GetBackgroundBrush());
			}
			break;
	}

	// sinon on paint comme d'haibute
	return (CDialog::OnCtlColor(pDC, pWnd, nCtlColor));
}

//=============================================================================	
// remplit la partie de l'offre correspondant à l'onglet (Cibles)
void COffre2Cibles::FillOffre(JATPOffre * pOffre)
//=============================================================================	
{
	// on crée une liste item + palettes réunis
	IItemList_t TargetsUnion;
	std::merge(m_SelectionItems.begin(), m_SelectionItems.end(),
		m_SelectionPalettes.begin(), m_SelectionPalettes.end(), std::insert_iterator<IItemList_t>(TargetsUnion, TargetsUnion.begin()));
	
	// suppression des items actuels de l'offre
	pOffre->m_TBLIdItems.Reset();
	pOffre->m_mapLblItem.Reset(); 

	// on construit les tables de l'offre (ids + libelles des cibles)
	IItemList_t::const_iterator item_it = TargetsUnion.begin();
	while(item_it != TargetsUnion.end())
	{
		if (*item_it)
		{
			pOffre->m_TBLIdItems.AddTail() = (*item_it)->GetID();
			pOffre->m_mapLblItem.Add((*item_it)->GetID()) = (*item_it)->GetLabel();
		}
		++item_it;
	}

	// on définit la reference affinité
	pOffre->m_IdItemAffinite = m_TargetList.GetReferenceAffiniteId();
}

//=============================================================================	
// provoque le chargement de la source et des terrains sélectionnés
void COffre2Cibles::ReloadTerrainsSelection()
//=============================================================================	
{
	// on récupère la sélection source et terrains
	const std::list<JUnt32> & SelectedTerrains
		= m_pDlgOffre->GetSupportsSelection().SelectedTerrains;		
	const JUnt32 IdSource = m_pDlgOffre->GetSupportsSelection().IdSource; 

	// on reforme la liste des terrains sélectionnés actuellement dans l'offre
	m_SelectedTerrains.clear();

	COffreTerrainManager * pOffreTerrainMgr = COffreTerrainManager::GetInstance();

	std::list<JUnt32>::const_iterator idter_it = SelectedTerrains.begin();
	while(idter_it != SelectedTerrains.end())
	{
		CKeyTerrain keyterrain(IdSource, *idter_it);
		m_SelectedTerrains.push_back(pOffreTerrainMgr->GetTerrain(keyterrain));
		++idter_it;
	}

	// on indique que l'on aura besoin de recharger les items depuis l'item manager
	m_bItemsDependOnTerrainsLoaded = false;
	m_bPalettesDependOnTerrainsLoaded = false;
}


//=============================================================================	
void COffre2Cibles::AssertItemsDependantOnTerrainsLoaded()
//=============================================================================	
{
	switch (m_CurDisplay)
	{
	case effItems:
		if (!m_bItemsDependOnTerrainsLoaded)
		{
			// on vide l'ensemble des items pour le recréer avec les indépendants + ceux provenant des fichiers
			m_AllItems.clear();
			
			// on recharge les items en fonction de la sélection de terrains
			CItemsConstruitsManager::GetInstance()->GetItemsInstance()->
				GetItemsIndependant(m_AllItems, m_SelectedTerrains);
			m_bItemsDependOnTerrainsLoaded = true;
		}
		break;
	case effPalettes:
		if (!m_bPalettesDependOnTerrainsLoaded)
		{
			// on vide l'ensemble des items pour le recréer avec les indépendants + ceux provenant des fichiers
			m_AllPalettes.clear();

			// on recharge les palettes en fonction de la sélection de terrains
			CItemsConstruitsManager::GetInstance()->GetItemsInstance()->
				GetPalettes(m_AllPalettes, m_SelectedTerrains);
			m_bPalettesDependOnTerrainsLoaded = true;
		}
		break;
	}
}

//=============================================================================	
void COffre2Cibles::OnLbnSelchangeDisplayChoiceLst()
//=============================================================================	
{
	m_CurDisplay = static_cast<eFilterFolder>(m_DisplayChoiceList.GetCurSel());
	switch(m_CurDisplay)
	{
		case effItems:

			m_Filter.modePalettes = false;

			// on tente de charger les items
			if (!m_bItemsLoaded)
				m_bItemsLoaded = LoadRubriques();
			if (m_bItemsLoaded)
				DisplayItems();
			break;
		case effPalettes:

			m_Filter.modePalettes = true;

			// on tente de charger les palettes
			if (!m_bPalettesLoaded)
				m_bPalettesLoaded = LoadRubriques();
			if (m_bPalettesLoaded)
				DisplayPalettes();
			break;
	}

	// on indique à la liste de cibles quel est l'affichage courant
	m_TargetList.SetCurrentDisplay(m_CurDisplay);

	if (m_bFirstInit)
	{
		// on force le chargement des items et des palettes
		m_CurDisplay == effItems;	 AssertItemsDependantOnTerrainsLoaded();
		m_CurDisplay == effPalettes; AssertItemsDependantOnTerrainsLoaded();

		// toutes les cibles sélectionnées
		std::list<const IItem*>::const_iterator GlobalSel_it
			= m_pDlgOffre->GetCiblesSelection().SelectedTargets.begin();
		while(GlobalSel_it != m_pDlgOffre->GetCiblesSelection().SelectedTargets.end())
		{
			// on cherche dans les items
			if (m_AllItems.find(*GlobalSel_it) != m_AllItems.end())
				// oui, on l'ajoute à la sélection d'items
				m_SelectionItems.insert(*GlobalSel_it);
			// on cherche dans les palettes
			else if (m_AllPalettes.find(*GlobalSel_it) != m_AllPalettes.end())
				// oui, on l'ajoute à la sélection de palettes
				m_SelectionPalettes.insert(*GlobalSel_it);
			++GlobalSel_it;
		}
		m_bFirstInit = false;
	}

	// Mise à jour de la liste dans dialogue items
	UpdateTargetList();

	// on récupère la sélection
	GetControlSelection(m_lastIndexes, m_TargetList);

	// on met à jour l'indicateur du nombre d'elements
	UpdateIndicator();
}

//=============================================================================	
struct IsRubrikContainItem
	: public std::unary_function<const JUnt32, bool>
//=============================================================================	
{
	IsRubrikContainItem(const COffre2Cibles::RubrikConstIter & rubrik_elem) : _rubrik_elem(rubrik_elem) {}

	bool operator () (const JUnt32 IdToFind)
	{
		// on cherche s'il existe au moins un id egal à l'id de l'item passé
		// en parametre dans les id de la rubrique passé au constructeur
		return (_rubrik_elem->second->end()
			!= std::find(_rubrik_elem->second->begin(), _rubrik_elem->second->end(), IdToFind));
	}
	const COffre2Cibles::RubrikConstIter & _rubrik_elem;
};


//=============================================================================	
bool IsRubrikContainOneItemOf(const COffre2Cibles::RubrikConstIter & rubrik_elem,
							  const std::list<const IItem*> & selectedItems, IItemList_t & foundItemIds)
//=============================================================================	
{
	// on cherche les id de cibles sélectionnés sont présent dans la rubrique,
	// on copie les id dans foundItems, on retourne true si au moins un id est présent

	bool bAtLeastOneFound = false;
	JUnt32 IdTarget;
	IsRubrikContainItem _IsRubrikContainItem(rubrik_elem);

	std::list<const IItem*>::const_iterator sel_it = selectedItems.begin();
	while(sel_it != selectedItems.end())
	{
		IdTarget = (*sel_it)->GetID();
		if (_IsRubrikContainItem(IdTarget))
		{
			bAtLeastOneFound = true;
			foundItemIds.insert(*sel_it);
		}
		++sel_it;
	}
	return bAtLeastOneFound;
}

//=============================================================================	
// on choisit d'afficher les items
void COffre2Cibles::DisplayItems()
//=============================================================================	
{
	ASSERT(m_hRoot);
	DeleteAllChildrenOf(m_hRoot);

	// on définit le texte de l'element racine
	CString sRootText; sRootText.LoadString(IDS_RI_RUBRIQUE_ITEMS);
	m_TreeCibles.SetItemText(m_hRoot, sRootText);

	// compteur de rubriques client
	long CptRubrique = 0;

	// on charge les elements qui dépendent des terrains
	ReloadNewItems();

	eTreeItemStates itemState = etisNotSelected;
	CString sTxtRubrique;

	// on balaye les rubriques que l'on vient de charger, on remplit l'arbre
	RubrikConstIter rubrik_it = m_RubriquesItems.begin();
	while (rubrik_it != m_RubriquesItems.end())
	{
		// Récup le nom de la rubrique
		sTxtRubrique = (rubrik_it->first);

		// préselection ?
		if (m_bIsFirstDisplayItems)
		{
			//if (IsRubrikContainOneItemOf(rubrik_it, m_pDlgOffre->GetCiblesSelection().SelectedTargets, m_SelectionItems))
			//{
			//	itemState = etisSelected;
			//	m_RubrikItemsSelection[sTxtRubrique] = itemState;
			//}

			// 1ere affichage, on sélectionne toutes les rubriques
			itemState = etisSelected;
		}
		else
		{
			// on cherche s'il faut selectionner cette rubrique
			RubrikSelectionMap_t::const_iterator itemsel_it = m_RubrikItemsSelection.find(sTxtRubrique);
			if (itemsel_it == m_RubrikItemsSelection.end())
				itemState = etisNotSelected;
			else itemState = itemsel_it->second;
		}

		// Ajoute la branche rubrique
		HTREEITEM hRubrique = m_TreeCibles.InsertItem(TVIF_STATE | TVIF_TEXT, sTxtRubrique, 0, 0,
			(0x0001) | INDEXTOSTATEIMAGEMASK(itemState),
			TVIS_STATEIMAGEMASK | 0x000F, 0, m_hRoot, 0);

		// Indique si c'est la branche divers
		if (sTxtRubrique == csDivers)
		{
			m_TreeCibles.SetItemData(hRubrique, reinterpret_cast<DWORD_PTR>((LPCTSTR)sTxtRubrique));
			m_hRubriqueDivers = hRubrique;
		}
		else
		{
			m_TreeCibles.SetItemData(hRubrique, reinterpret_cast<DWORD_PTR>((LPCTSTR)sTxtRubrique));
			CptRubrique++;
		}

		if (m_bIsFirstDisplayItems)
			SelectRubrik(hRubrique, itemState);

		++rubrik_it;
	}

	// plus de préselection pour les prochains chargement
	if (m_bIsFirstDisplayItems) m_bIsFirstDisplayItems = false;

	// on donne la bonne sélection à l'item racine
	m_TreeCibles.SetItemState(m_hRoot, INDEXTOSTATEIMAGEMASK(ComputeRootSelection()), TVIS_STATEIMAGEMASK);

	// Puis on déplie sur toutes les rubriques
	m_TreeCibles.Expand(m_hRoot, TVE_EXPAND);
}

//=============================================================================	
// on choisit d'afficher les palettes
void COffre2Cibles::DisplayPalettes()
//=============================================================================	
{
	ASSERT(m_hRoot);
	DeleteAllChildrenOf(m_hRoot);

	// on définit le texte de l'element racine
	CString sRootText; sRootText.LoadString(IDS_RI_RUBRIQUE_PALETTES);
	m_TreeCibles.SetItemText(m_hRoot, sRootText);

	// compteur de rubriques client
	long CptRubrique = 0;

	// on charge les elements qui dépendent des terrains
	ReloadNewItems();

	eTreeItemStates itemState = etisNotSelected;
	CString sTxtRubrique;

	// on balaye les rubriques que l'on vient de charger, on remplit l'arbre
	RubrikConstIter rubrik_it = m_RubriquesPalettes.begin();
	while (rubrik_it != m_RubriquesPalettes.end())
	{
		// Récup le nom de la rubrique
		sTxtRubrique = (rubrik_it->first);

		// préselection ?
		if (m_bIsFirstDisplayPalettes)
		{
			//if (IsRubrikContainOneItemOf(rubrik_it, m_pDlgOffre->GetCiblesSelection().SelectedTargets, m_SelectionPalettes))
			//{
			//	itemState = etisSelected;
			//	m_RubrikPalettesSelection[sTxtRubrique] = itemState;
			//}

			// 1ere affichage, on sélectionne toutes les rubriques
			itemState = etisSelected;
		}
		else
		{
			// on cherche s'il faut selectionner cette rubrique
			RubrikSelectionMap_t::const_iterator palettesel_it = m_RubrikPalettesSelection.find(sTxtRubrique);
			if (palettesel_it == m_RubrikPalettesSelection.end())
				itemState = etisNotSelected;
			else itemState = palettesel_it->second;
		}

		// Ajoute la branche rubrique
		HTREEITEM hRubrique = m_TreeCibles.InsertItem(TVIF_STATE | TVIF_TEXT,sTxtRubrique, 0, 0,
			(0x0001) | INDEXTOSTATEIMAGEMASK(itemState),
			TVIS_STATEIMAGEMASK | 0x000F, 0, m_hRoot, 0);

		// Indique si c'est la branche divers
		if (sTxtRubrique == csDivers)
		{
			//m_TreeCibles.SetItemData(hRubrique, eidtrDivers);
			m_TreeCibles.SetItemData(hRubrique, reinterpret_cast<DWORD_PTR>((LPCTSTR)sTxtRubrique));
			m_hRubriqueDivers = hRubrique;
		}
		else
		{
			//m_TreeCibles.SetItemData(hRubrique, eidtrClient + CptRubrique);
			m_TreeCibles.SetItemData(hRubrique, reinterpret_cast<DWORD_PTR>((LPCTSTR)sTxtRubrique));
			CptRubrique++;
		}

		if (m_bIsFirstDisplayPalettes)
			SelectRubrik(hRubrique, itemState);

		++rubrik_it;
	}

	// plus de préselection pour les prochains chargement
	if (m_bIsFirstDisplayPalettes) m_bIsFirstDisplayPalettes = false;

	// on donne la bonne sélection à l'item racine
	m_TreeCibles.SetItemState(m_hRoot, INDEXTOSTATEIMAGEMASK(ComputeRootSelection()), TVIS_STATEIMAGEMASK);

	// Puis on déplie sur toutes les rubriques
	m_TreeCibles.Expand(m_hRoot, TVE_EXPAND);
}

//=============================================================================	
void COffre2Cibles::OnBnClickedCkItemsFromSource()
//=============================================================================	
{
	// TODO : ajoutez ici le code de votre gestionnaire de notification de contrôle
}

//=============================================================================	
// définit avec quels elements le dialogue s'affiche ne premier lieu
void COffre2Cibles::SetStartupMode(const eFilterFolder startup_mode)
//=============================================================================	
{
	m_CurDisplay = startup_mode;
}

//=============================================================================	
// chargement des items depuis fichier
bool COffre2Cibles::LoadRubriques()
//=============================================================================	
{
	// Le noeud racine rubriques items ou palettes ou autres
	CString str;str.LoadString(IDS_RI_RUBRIQUE_ITEMS);

	// on tente de charger les rubriques
	if (LoadFileRubriques())
	{
		if (m_CurDisplay == effItems)
		{
			// on ajoute la rubrique divers avec une liste vide (si elle n'existe pas déjà)
			if (m_RubriquesItems.find(csDivers) == m_RubriquesItems.end())
				m_RubriquesItems.insert(std::make_pair(csDivers, new IdVector_t));
		}
		else if (m_CurDisplay == effPalettes)
		{
			// on ajoute la rubrique divers avec une liste vide (si elle n'existe pas déjà)
			if (m_RubriquesPalettes.find(csDivers) == m_RubriquesPalettes.end())
				m_RubriquesPalettes.insert(std::make_pair(csDivers, new IdVector_t));
		}
		return true;
	}
	return false;
}

//=============================================================================	
// Chargement fichier rubriques
JBool COffre2Cibles::LoadFileRubriques()
//=============================================================================	
{
	// Si fichier vide, on crée la rubrique divers et on la remplit des items existants
	SetCurrentDirectory(GETPROFILESTRINGD(SECTION_DATA, KEY_USER_PATH, 0));

	// Nom du fichier rubrique
	CString FicRubrique;
	FicRubrique.Format("/%s%d.%s","Rubrique", m_CurDisplay, "lst");  
	JStringEx PathRubrique;
	PathRubrique.Append(FicRubrique);

	try
	{
		// Création 
		JArchive *pFile	= JArchive::Create(PathRubrique.AsJCharPtr());

		// Chargement entête
		LoadFileHeader(pFile);

		// Charge les rubriques
		switch (m_CurDisplay)
		{
			case effItems:		LoadRubriques(pFile, m_RubriquesItems, m_DisplayedItems); break;
			case effPalettes:	LoadRubriques(pFile, m_RubriquesPalettes, m_DisplayedPalettes); break;
		}
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

//=============================================================================	
// Chargement entete fichier rubrique
void COffre2Cibles::LoadFileHeader(JArchive *pFile)
//=============================================================================	
{
	// Version fichier
	const JInt32 CURRENT_VERSION = 1;

	CFileHeader	fileHeader;
	fileHeader.Recv(*pFile);

	// Check compagnie name
	if(fileHeader.GetCompagnie () != 0x0043464A)
		throw JInternalError::GetInstance(); //("File Header, compagnie field error");

	// Verify the magic number
	if(fileHeader.GetMagicNumber () != 0x6FD3481C)
		//("File Header, Magic Number field error");
		throw JInternalError::GetInstance(); 

	// No compression
	if(fileHeader.GetCompression () != 0)
		//("File Header, Compression field error");
		throw JInternalError::GetInstance(); 

	// No cryptage
	if(fileHeader.GetCryptage () != 0)
		//("File Header, Cryptage field error");
		throw JInternalError::GetInstance(); 

	// Must not be greater than my current version
	if(fileHeader.GetVersion () > CURRENT_VERSION)
		//("File Header, Version error");
		throw JInternalError::GetInstance(); 
}

//=============================================================================	
// Charge les rubriques
void COffre2Cibles::LoadRubriques(JArchive *pFile, RubrikMap_t & mRubriks, DisplayMap_t & mDisplay)
//=============================================================================	
{
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

		// Test si c'est une rubrique du type recherché (items ou palettes)
		if (TypeRubrique == m_CurDisplay)
		{
			// Récup nb items associés
			JUnt32 NbItems;
			pFile->Recv(NbItems);

			IdVector_t * pIdVector = new IdVector_t;

			// Et les idents des items associés
			for (JUnt32 InxItem = 0; InxItem < NbItems; InxItem++)
			{
				// Récup ident items
				JInt32 IdItem;
				pFile->Recv(IdItem);

				// ajoute l'id à la liste
				pIdVector->push_back(IdItem); 

				// Ajoute au map des items à afficher (si nécessaire)
				if (mDisplay.find(IdItem) == mDisplay.end())
					mDisplay.insert(std::make_pair(IdItem, false));
			}
			mRubriks.insert(std::make_pair(LabelRubrique.AsJCharPtr(), pIdVector));
		}
	}
}

//=============================================================================	
// libère la map et les elements qu'elle contient
void COffre2Cibles::FreeRubrikMap(const eFilterFolder folder)
//=============================================================================	
{
	if (folder == effItems)
	{
		IdVector_t * pIdVector = 0;

		///////////////////////////////////////////
		// boucle sur ts les elts de la map d'items

		RubrikIter rubrik_it = m_RubriquesItems.begin();
		while(rubrik_it != m_RubriquesItems.end())
		{
			// on garde le ptr
			pIdVector = rubrik_it->second;
			++rubrik_it;

			if (pIdVector)
				delete pIdVector;
				pIdVector = 0;
		}
		// on peut vider la map
		m_RubriquesItems.clear();
	}
	else if (folder == effPalettes)
	{
		IdVector_t * pIdVector = 0;

		///////////////////////////////////////////
		// boucle sur ts les elts de la map d'items

		RubrikIter rubrik_it = m_RubriquesPalettes.begin();
		while(rubrik_it != m_RubriquesPalettes.end())
		{
			// on garde le ptr
			pIdVector = rubrik_it->second;
			++rubrik_it;

			if (pIdVector)
				delete pIdVector;
			pIdVector = 0;
		}
		// on peut vider la map
		m_RubriquesPalettes.clear();
	}
}

//=============================================================================	
void COffre2Cibles::ReloadNewItems()
//=============================================================================	
{
	RubrikIter rubrikdivers_it, rubrik_end;
	
	switch (m_CurDisplay)
	{
		case effItems:
			rubrikdivers_it = m_RubriquesItems.find(csDivers);
			rubrik_end = m_RubriquesItems.end();
			break;
		case effPalettes:
			rubrikdivers_it = m_RubriquesPalettes.find(csDivers);
			rubrik_end = m_RubriquesPalettes.end();
			break;
	}

	// à ce point là, la rubrique 'divers' doit absolument avoir été déjà créée
	ASSERT (rubrikdivers_it != rubrik_end);

	// Récup liste des items associés à la rubrique divers
	IdVector_t * pLstDiversItems = rubrikdivers_it->second;

	// récupère items ou palettes correspondants au terrains courants de l'offre
	AssertItemsDependantOnTerrainsLoaded();

	// Chargement des items non classés si rubrique items
	if (m_CurDisplay == effItems)
	{
		// on vide la taille des items à afficher
		//m_DisplayedItems.clear();

		// on boucle sur les items chargés, on les place dans les rubriques
		IItemList_t::const_iterator item_it = m_AllItems.begin();
		while(item_it != m_AllItems.end())
		{
			// Ident de l'item
			const JInt32 IdItem = (*item_it)->GetID();

			bool bItemFound = false;

			// on boucle sur les rubriques d'items
			RubrikIter rubrik_it = m_RubriquesItems.begin();
			while(rubrik_it != m_RubriquesItems.end())
			{
				// on cherche cet id dans cette rubrique
				IsRubrikContainItem isRubrikContainItem(rubrik_it);
				if (isRubrikContainItem(IdItem))
				{
					bItemFound = true;
					break;
				}
				++rubrik_it;
			}

			// item pas trouvé, on l'ajoute dans la rubrique divers
			if (!bItemFound)
				pLstDiversItems->push_back(IdItem);

			//m_DisplayedItems[IdItem] = false;

			++item_it;
		}
	}
	// Chargement des palettes non classées si rubrique palettes
	else if (m_CurDisplay == effPalettes)
	{
		// on vide la taille des items à afficher
		//m_DisplayedPalettes.clear();

		// on boucle sur les items chargés, on les place dans les rubriques
		IItemList_t::const_iterator item_it = m_AllPalettes.begin();
		while(item_it != m_AllPalettes.end())
		{
			// Ident de l'item
			const JInt32 IdItem = (*item_it)->GetID();

			bool bItemFound = false;

			// on boucle sur les rubriques d'items
			RubrikIter rubrik_it = m_RubriquesPalettes.begin();
			while(rubrik_it != m_RubriquesPalettes.end())
			{
				// on cherche cet id dans cette rubrique
				IsRubrikContainItem isRubrikContainItem(rubrik_it);
				if (isRubrikContainItem(IdItem))
				{
					bItemFound = true;
					break;
				}
				++rubrik_it;
			}

			// item pas trouvé, on l'ajoute dans la rubrique divers
			if (!bItemFound)
				pLstDiversItems->push_back(IdItem);

			//m_DisplayedPalettes[IdItem] = false;

			++item_it;
		}
	}

	// on trie les items
	std::sort(pLstDiversItems->begin(), pLstDiversItems->end());
}

//=============================================================================	
// supprime tous les enfants d'un element
void COffre2Cibles::DeleteAllChildrenOf(HTREEITEM htParent)
//=============================================================================	
{
	HTREEITEM htItem = m_TreeCibles.GetChildItem(htParent);
	while (htItem)
	{
		HTREEITEM htNext = m_TreeCibles.GetNextSiblingItem(htItem);
		if (htItem == m_hRubriqueDivers)
			m_hRubriqueDivers = 0;
		m_TreeCibles.DeleteItem(htItem);
		htItem = htNext;
	}
}

//=============================================================================	
void COffre2Cibles::SetImageList()
//=============================================================================	
{
	switch(CATPColors::GetAppColor(m_pDlgOffre->GetAppColor()))
	{
		case CATPColors::BOTTLE_GREEN:
			m_imageList.Create(IDB_RI_BITMAP1, 15, 1, RGB(255,255,255));break;
		case CATPColors::EMERAUDE:
			m_imageList.Create(IDB_RI_BITMAP2, 15, 1, RGB(255,255,255));break;
		case CATPColors::BLEU_PASTEL:
			m_imageList.Create(IDB_RI_BITMAP3, 15, 1, RGB(255,255,255));break;
		case CATPColors::OCEAN:
			m_imageList.Create(IDB_RI_BITMAP4, 15, 1, RGB(255,255,255));break;
		case CATPColors::LILAS:
			m_imageList.Create(IDB_RI_BITMAP5, 15, 1, RGB(255,255,255));break;
		case CATPColors::SAHARA:
			m_imageList.Create(IDB_RI_BITMAP6, 15, 1, RGB(255,255,255));break;
		case CATPColors::TERRE_SIENNE:
			m_imageList.Create(IDB_RI_BITMAP7, 15, 1, RGB(255,255,255));break;
		case CATPColors::RUBIS:
			m_imageList.Create(IDB_RI_BITMAP8, 15, 1, RGB(255,255,255));break;
		case CATPColors::GREY_MOUSE:
			m_imageList.Create(IDB_RI_BITMAP9, 15, 1, RGB(255,255,255));break;
		case CATPColors::ROSE:
			m_imageList.Create(IDB_RI_BITMAP10, 15, 1, RGB(255,255,255));break;
		case CATPColors::ROSE_KAKI:
			m_imageList.Create(IDB_RI_BITMAP11, 15, 1, RGB(255,255,255));break;
		case CATPColors::ROSE_GRIS:
			m_imageList.Create(IDB_RI_BITMAP12, 15, 1, RGB(255,255,255));break;
		case CATPColors::SABLE:
			m_imageList.Create(IDB_RI_BITMAP13, 15, 1, RGB(255,255,255));break;
		case CATPColors::CAFE:
			m_imageList.Create(IDB_RI_BITMAP14, 15, 1, RGB(255,255,255));break;
		case CATPColors::NEWBLUE:
			m_imageList.Create(IDB_RI_BITMAP15, 15, 1, RGB(255,255,255));break;
	}
	m_TreeCibles.SetImageList(&m_imageList, TVSIL_STATE);
	UpdateWindow(); 
}

//=============================================================================	
// functor pour recherche un IItem ayant un Id particulier
struct IItemHasId
	: public std::unary_function<JInt32, bool>
{
	IItemHasId (const JInt32 IdtoFind) : _IdtoFind(IdtoFind) {}	
	bool operator () (const IItem * pItem)
	{
		return (pItem->GetID() == _IdtoFind);
	}
	const JInt32 _IdtoFind;
};

//=============================================================================	
void COffre2Cibles::UpdateTargetList()
//=============================================================================	
{
	m_TargetList.Lock();
	m_TargetList.ResetItems();
	m_TargetList.ResetContent();

	// on construit le filtre d'affichage des cibles
	m_Filter.idSource = m_pDlgOffre->GetSupportsSelection().IdSource;

	AssertItemsDependantOnTerrainsLoaded();

	// on passe dans la map et on crée la liste des cibles à afficher
	if (m_CurDisplay == effItems)
	{
		IItemList_t::iterator item_it = m_AllItems.begin();
		while(item_it != m_AllItems.end())
		{
			// on verifie que cet item est bien dans une rubrique qui est sélectionnée
			DisplayIter dispItem_it = m_DisplayedItems.find((*item_it)->GetID());
			if (dispItem_it != m_DisplayedItems.end())
			{
				// cet item est marqué comme 'à afficher'
				if (dispItem_it->second)
				{
					// on ajoute cet item à la liste
					m_TargetList.AddItem(*item_it);
				}
			}
			else
			{
				// TRAC : TICKET 34
				// résolution du ticket 34 (mais ne corrige pas le ticket 35)
				// pour ticket 35 --> VOIR A PARTIR de void CDlgItems::OnAccept()
				// quand 1 item est créé, le fichier User/Rubriques n'est pas mis à jour

				// item pas dans la liste d'affichage, c'est bizarre
				// on l'ajoute quand même, mais dans Divers

				RubrikIter rubrik_divers = m_RubriquesItems.find(csDivers);
#if 0
				IdVector_t * divers_items;
				if (rubrik_divers == m_RubriquesItems.end())
				{	// divers n'existe pas, on la crée
					divers_items = new IdVector_t;					
					m_RubriquesItems.insert(std::make_pair(csDivers, divers_items));
				}
				else
					divers_items = rubrik_divers->second;

				// on ajoute cet item à la rubrique 'Divers' si il n'existe pas
				if (std::find(divers_items->begin(), divers_items->end(), (*item_it)->GetID())
					 == divers_items->end())
				{
					divers_items->push_back((*item_it)->GetID());
					std::sort(divers_items->begin(), divers_items->end());					
				}

				// on ajoute cet item à la liste des items disponibles pour l'affichage
				m_DisplayedItems.insert(std::make_pair((*item_it)->GetID(), false));

				// on ajoute cet item à la liste
				m_TargetList.AddItem(*item_it);
#endif
				// TRAC : TICKET 34 FIN CODE
			}
			++item_it;
		}
		// unlock et mise à jour des cibles sur la liste d'affichage des cibles
		m_TargetList.UpdateAndUnlock(m_Filter, m_SelectionItems);
	}
	else if (m_CurDisplay == effPalettes)
	{
		IItemList_t::iterator palette_it = m_AllPalettes.begin();
		while(palette_it != m_AllPalettes.end())
		{
			// on verifie que cet item est bien dans une rubrique qui est sélectionnée
			DisplayIter dispPalet_it = m_DisplayedPalettes.find((*palette_it)->GetID());
			if (dispPalet_it != m_DisplayedPalettes.end())
			{
				// cet item est marqué comme 'à afficher'
				if (dispPalet_it->second)
				{
					// on ajoute cet item à la liste
					m_TargetList.AddItem(*palette_it);
				}
			}
			else
			{	// item pas dans la liste d'affichage, c'est bizarre
				// TRAC : TICKET 34
				// résolution du ticket 34 (mais ne corrige pas le ticket 35)
				// pour ticket 35 --> VOIR A PARTIR de void CDlgItems::OnAccept()
				// quand 1 item est créé, le fichier User/Rubriques n'est pas mis à jour

				// item pas dans la liste d'affichage, c'est bizarre
				// on l'ajoute quand même, mais dans Divers

				RubrikIter rubrik_divers = m_RubriquesPalettes.find(csDivers);
#if 0
				IdVector_t * divers_palettes;
				if (rubrik_divers == m_RubriquesPalettes.end())
				{	// divers n'existe pas, on la crée
					divers_palettes = new IdVector_t;					
					m_RubriquesPalettes.insert(std::make_pair(csDivers, divers_palettes));
				}
				else
					divers_palettes = rubrik_divers->second;

				// on ajoute cet item à la rubrique 'Divers' si il n'existe pas
				if (std::find(divers_palettes->begin(), divers_palettes->end(), (*palette_it)->GetID())
					== divers_palettes->end())
				{
					divers_palettes->push_back((*palette_it)->GetID());
					std::sort(divers_palettes->begin(), divers_palettes->end());					
				}

				// on ajoute cet item à la liste des items disponibles pour l'affichage
				m_DisplayedPalettes.insert(std::make_pair((*palette_it)->GetID(), false));
#endif
				// TRAC : TICKET 34 FIN CODE			
			}
			++palette_it;
		}
		// unlock et mise à jour des cibles sur la liste d'affichage des cibles
		m_TargetList.UpdateAndUnlock(m_Filter, m_SelectionPalettes);
	}
}

//=============================================================================	
//=============================================================================	
//							Functors de l'onglet cibles
//=============================================================================	
//=============================================================================	

#pragma region functors onglet cibles

//=============================================================================	
struct SetItemSelection
	: public std::unary_function<JInt32, void>
{
	SetItemSelection(COffre2Cibles::DisplayMap_t & displaymap, const bool bSelection)
		:	_displaymap(displaymap), _bSelection(bSelection) {}
	
	void operator () (const JInt32 id)
	{
		_map_it = _displaymap.find(id);
		if (_map_it != _displaymap.end())
			_map_it->second/*.bSelected*/ = _bSelection;
			//_map_it->second.bSelected = _bSelection;
	}


	COffre2Cibles::DisplayMap_t &			_displaymap;
	COffre2Cibles::DisplayMap_t::iterator	_map_it;
	const bool								_bSelection;
};


//=============================================================================	
const bool IsItemSelected(const int treeItemState)
{
	switch(treeItemState)
	{
	case COffre2Cibles::etisNotSelected:
		return false; break;
	case COffre2Cibles::etisSelected:
	case COffre2Cibles::etisHalfSelected:
		return true;break;
	default:
		return false; break;
	}
}

//=============================================================================	
struct IsItemSelected
	: public std::unary_function<JInt32, bool>
{
	IsItemSelected(){}


};

#pragma endregion functors onglet cibles

//=============================================================================	
void COffre2Cibles::SelectRubrik(HTREEITEM hti, eTreeItemStates state)
//=============================================================================	
{
	// on déduit l'etat de sélection de la rubrique
	bool bSelect = IsItemSelected(state);

	DWORD_PTR dwItemData = m_TreeCibles.GetItemData(hti);
	if(dwItemData != eidtrRoot)
	{
		// on (de)sélectionne une rubrique
		CString sTxtRubrik(reinterpret_cast<LPCTSTR>(dwItemData));

		if (m_CurDisplay == effItems)
		{
			// on enregistre l'état de sélection de cette rubrique
			m_RubrikItemsSelection[sTxtRubrik] = state;

			// on cherche l'elt de la map ayant le même text que le tree item passé en paramêtre
			RubrikConstIter rubrik_it = m_RubriquesItems.find(sTxtRubrik);
			if (rubrik_it != m_RubriquesItems.end())
			{
				// on sélectionne/déselectionne tous les id correspondants
				SetItemSelection SetSelectionFunc(m_DisplayedItems, bSelect);
				std::for_each(rubrik_it->second->begin(), rubrik_it->second->end(), SetSelectionFunc);
			}
		}
		else if (m_CurDisplay == effPalettes)
		{
			// on enregistre l'état de sélection de cette rubrique
			m_RubrikPalettesSelection[sTxtRubrik] = state;

			// on cherche l'elt de la map ayant le même text que le tree item passé en paramêtre
			RubrikConstIter rubrik_it = m_RubriquesPalettes.find(sTxtRubrik);
			if (rubrik_it != m_RubriquesPalettes.end())
			{
				// on sélectionne/déselectionne tous les id correspondants
				SetItemSelection SetSelectionFunc(m_DisplayedPalettes, bSelect);
				std::for_each(rubrik_it->second->begin(), rubrik_it->second->end(), SetSelectionFunc);
			}
		}
	}
}

//=============================================================================	
int COffre2Cibles::ComputeRootSelection()
//=============================================================================	
{
	if (!HasActifChildren(m_hRoot))
	{
		// aucun element actif, donc pas de sélection
		return etisNotSelected;
	}
	else
	{
		// il y a des elements actif, mais tous?
		if (HasInactifChildren(m_hRoot))
			// certains sont inactifs, donc
			return etisHalfSelected;
		else
			// aucun n'est inactif, donc tous actifs!
			return etisSelected;
	}
}

//=============================================================================	
void COffre2Cibles::OnEnChangeCiblesSearchStr()
//=============================================================================	
{
	SetTimer(IdTimer_SearchStrTargets, 500, NULL);
}

//=============================================================================	
void COffre2Cibles::OnTimer(UINT_PTR nIDEvent)
//=============================================================================	
{
	if (nIDEvent == IdTimer_SearchStrTargets) 
	{
		KillTimer(IdTimer_SearchStrTargets);
		UpdateData();

		// check de changement de la chaine de filtrage
		if (m_sLastSearchStr != m_Filter.strSearch)
		{
			// mise à jour de la liste de cibles
			m_TargetList.Lock();
			m_TargetList.ResetContent();
			if (m_CurDisplay == effItems)
				m_TargetList.UpdateAndUnlock(m_Filter, m_SelectionItems);
			else if (m_CurDisplay == effPalettes)
				m_TargetList.UpdateAndUnlock(m_Filter, m_SelectionPalettes);
			m_sLastSearchStr = m_Filter.strSearch;

			// deselectionne les items dont le seuil est rédhibitoire
			m_TargetList.DeselectItemsWithBadSeuils();

			// mise à jour de l'indicateur d'elements
			UpdateIndicator();
		}
	}
	COffre2OngletBase::OnTimer(nIDEvent);
}

//=============================================================================	
// mise à jour de l'indicateur
void COffre2Cibles::UpdateIndicator()
//=============================================================================	
{
	CString sMsg;
	const int selcount = m_TargetList.GetSelCount();

	// on a des elements sélectionnés, on affiche le nbr d'elements sélectionnés
	if (selcount > 0)
	{	
		if (selcount == 1)	sMsg = "1 élement sélectionné";
		else				sMsg.Format("%d élements sélectionnés", selcount);	
	}
	else
	{
		// sinon on affiche seulement le nombre d'elements dans la liste
		const int count = m_TargetList.GetCount();
		if (count == 0)		sMsg = "aucun élément";
		else if (count == 1)sMsg = "1 élément";
		else				sMsg.Format("%d éléments", count);
	}
	m_indicator.SetWindowText(sMsg);
}

//=============================================================================	
void COffre2Cibles::OnBnClickedToutselect()
//=============================================================================	
{
	const int nMax = m_TargetList.GetCount();

	// il y a au moins un elements, la sélection sera modifiée --> on marque le dialog offre comme modifié
	if (nMax > 0) m_pDlgOffre->SetModified();

	// on bloque la listbox
	m_TargetList.LockWindowUpdate();

	std::set<int> elements;
	GetControlSelection(elements, m_TargetList);

	// qu'a-t-on de sélectionné pour l'instant? si rien on sélectionne tout ce qui est affiché, 
	// sinon si au moins un site affiché est sélectionné, on déselectionne tout ce qui est affiché
	bool bSelectAll = elements.empty();

	// on effectue la sélection/déselection globale avant de debloquer la listbox
	m_TargetList.SelItemRange(bSelectAll?TRUE:FALSE, 0, nMax);
	m_TargetList.UnlockWindowUpdate();

	// deselectionne les items dont le seuil est rédhibitoire
	m_TargetList.DeselectItemsWithBadSeuils();

	if (m_CurDisplay == effItems)
		// on récupère tous les items sélectionnés
		GetControlSelectedData(m_SelectionItems, m_TargetList);	
	else if (m_CurDisplay == effPalettes)
		// on récupère toutes les palettes sélectionnées
		GetControlSelectedData(m_SelectionPalettes, m_TargetList);

	// on récupère la sélection
	GetControlSelection(m_lastIndexes, m_TargetList);

	// mise à jour de l'indicateur d'elements
	UpdateIndicator();

	// mise à jour du bouton OK
	m_pDlgOffre->EnableOK();
}

//=============================================================================	
void COffre2Cibles::OnLbnSelchangeLstCibles()
//=============================================================================	
{
	std::set<int> diffIndexes;

	// on récupère les indexes venant d'être ajoutés
	GetControlSelection(m_Indexes, m_TargetList);
	set_difference(m_Indexes.begin(), m_Indexes.end(),
		m_lastIndexes.begin(), m_lastIndexes.end(), inserter(diffIndexes, diffIndexes.end()));

	// on check si on a clické sur 1 seul item, et si son seuil est redhibitoire
	const size_t before_size = m_lastIndexes.size();
	const size_t after_size = m_Indexes.size();

	// on teste si un 1 seul item ou palette vient d'être sélectionné
	bool bDisplayMessage = false;
	if (after_size > before_size)
		if (after_size - 1 == before_size)
			if (includes(m_Indexes.begin(), m_Indexes.end(), m_lastIndexes.begin(), m_lastIndexes.end()))
				bDisplayMessage = true;

	// on affiche un message
	if (bDisplayMessage)
	{
		// normalement diffIndexes contient 1 seul element, celui venant d'être sélectionné
		ASSERT(diffIndexes.size() == 1);

		if (diffIndexes.size() == 1)
		{
			// index de l'element unique venant d'être sélectionné
			const int lonelySelIndex = *diffIndexes.begin();

			// on récupère l'item correspondant
			const IItem * pItem = reinterpret_cast<const IItem*>(m_TargetList.GetItemData(lonelySelIndex));
			ASSERT (pItem);
			if (pItem)
			{
				bool bUnselect = false;

				// on affiche le message et on voit si faut déselectionner cet element
				m_TargetList.DisplaySingleItemMessage(pItem, bUnselect);

				if (bUnselect)
				{
					// on supprime cet element du record des indexes
					m_Indexes.erase(lonelySelIndex);
					m_TargetList.SetSel(lonelySelIndex, FALSE);
				}
			}
		}
	}
	else
	{
		// on traite les déselections d'elements totalement retreints en groupe

		// deselectionne les items dont le seuil est rédhibitoire
		m_TargetList.DeselectItemsWithBadSeuils();
	}

	// on met à jour les listes d'elements (items/palettes) sélectionnés
	if (m_CurDisplay == effItems)
		// on récupère tous les items sélectionnés
		GetControlSelectedData(m_SelectionItems, m_TargetList);	
	else if (m_CurDisplay == effPalettes)
		// on récupère toutes les palettes sélectionnées
		GetControlSelectedData(m_SelectionPalettes, m_TargetList);

	// on récupère la sélection
	GetControlSelection(m_lastIndexes, m_TargetList);

	// on met à jour l'indicateur du nombre d'elements
	UpdateIndicator();
}

//=============================================================================	
// notifie l'onglet qu'il vient d'être sélectionné/déselectionné
//=============================================================================	
void COffre2Cibles::NotifySelectionChange(bool bSelect)
{
	// si on vient de sélectionner cet onglet
	if (bSelect)
	{
		m_bIsFirstDisplayItems = true;
		m_bIsFirstDisplayPalettes = true;

		// on recharge les terrains et la source sélectionnés
		ReloadTerrainsSelection();

		// on notifie la listbox des cibles que l'on vient d'afficher l'onglet, afin qu'elle refasse son cache pour le calcul de seuils
		m_TargetList.ReloadCacheSeuilsElements();

		// on affiche l'arbre courant (items ou palettes)
		OnLbnSelchangeDisplayChoiceLst();
	}
}

//=============================================================================	
LRESULT COffre2Cibles::OnSourceElementsChange(WPARAM wParam, LPARAM lParam)
//=============================================================================	
{
	m_TargetList.InvalidateSeuils();
	return 0;
}


//=============================================================================	
//=============================================================================	
//							Gestion des tree elements
//=============================================================================	
//=============================================================================	

#pragma region gestion des tree elements

//=============================================================================	
void COffre2Cibles::ToggleItemState(HTREEITEM hti)
//=============================================================================	
{
	int iImage = m_TreeCibles.GetItemState( hti, TVIS_STATEIMAGEMASK )>>12;
	switch(iImage)
	{
	case etisNotSelected: iImage = etisSelected; break;
	case etisSelected: iImage = etisNotSelected; break;
	case etisHalfSelected: iImage = etisSelected; break;
	}
	// on sélectionne les rubriques du côté données
	SelectRubrik(hti, static_cast<eTreeItemStates>(iImage));

	if (m_TreeCibles.ItemHasChildren(hti))
		ToggleChildItemState(hti,iImage);
	else
		m_TreeCibles.SetItemState(hti, INDEXTOSTATEIMAGEMASK(iImage), TVIS_STATEIMAGEMASK);

	ToggleParentItemState(hti,iImage);
}

//=============================================================================	
void COffre2Cibles::ToggleChildItemState(HTREEITEM hti, int iImage)
//=============================================================================	
{
	// on sélectionne les rubriques du côté données
	SelectRubrik(hti, static_cast<eTreeItemStates>(iImage));

	m_TreeCibles.SetItemState(hti, INDEXTOSTATEIMAGEMASK(iImage), TVIS_STATEIMAGEMASK);

	if (m_TreeCibles.ItemHasChildren(hti))
	{
		HTREEITEM htiChild = m_TreeCibles.GetChildItem (hti);
		if (htiChild)
			ToggleChildItemState(htiChild,iImage);
		else
			return;
		HTREEITEM htiSibling = m_TreeCibles.GetNextSiblingItem (htiChild);
		while (htiSibling)
		{
			ToggleChildItemState(htiSibling,iImage);
			htiSibling = m_TreeCibles.GetNextSiblingItem(htiSibling);
		}
	}
}

//=============================================================================	
void COffre2Cibles::ToggleParentItemState(HTREEITEM hti, int iImage)
//=============================================================================	
{
	HTREEITEM hParent = m_TreeCibles.GetParentItem(hti);
	if(hParent)
	{
		int piImage = etisNotSelected;

		if(HasActifChildren(hParent))
		{
			piImage = etisSelected;
			if(HasInactifChildren(hParent))
				piImage = etisHalfSelected;
		}
		m_TreeCibles.SetItemState(hParent, INDEXTOSTATEIMAGEMASK(piImage), TVIS_STATEIMAGEMASK);
		ToggleParentItemState(hParent,piImage);

		// on sélectionne les rubriques du côté données
		SelectRubrik(hParent, static_cast<eTreeItemStates>(piImage));
	}
}

//=============================================================================	
bool COffre2Cibles::HasInactifChildren(HTREEITEM hti)
//=============================================================================	
{
	if (m_TreeCibles.ItemHasChildren(hti))
	{
		HTREEITEM htiChild = m_TreeCibles.GetChildItem (hti);
		if (htiChild)
		{
			if((m_TreeCibles.GetItemState(htiChild, TVIS_STATEIMAGEMASK )>>12) == etisNotSelected)
				return true;
			if(HasInactifChildren(htiChild))
				return true;
		}
		else
			return false;
		HTREEITEM htiSibling = m_TreeCibles.GetNextSiblingItem (htiChild);
		while (htiSibling )
		{
			if((m_TreeCibles.GetItemState(htiSibling, TVIS_STATEIMAGEMASK )>>12) == etisNotSelected)
				return true;
			if(HasInactifChildren(htiSibling))
				return true;
			htiSibling = m_TreeCibles.GetNextSiblingItem(htiSibling);
		}
	}
	return false;
}

//=============================================================================	
bool COffre2Cibles::HasActifChildren(HTREEITEM hti)
//=============================================================================	
{
	if (m_TreeCibles.ItemHasChildren(hti))
	{
		HTREEITEM htiChild = m_TreeCibles.GetChildItem (hti);
		if (htiChild)
		{
			if((m_TreeCibles.GetItemState(htiChild, TVIS_STATEIMAGEMASK )>>12) > etisNotSelected)
				return true;
			if(HasActifChildren(htiChild))
				return true;
		}
		else
			return false;
		HTREEITEM htiSibling = m_TreeCibles.GetNextSiblingItem (htiChild);
		while (htiSibling )
		{
			if((m_TreeCibles.GetItemState(htiSibling, TVIS_STATEIMAGEMASK )>>12) > etisNotSelected)
				return true;
			if(HasActifChildren(htiSibling))
				return true;
			htiSibling = m_TreeCibles.GetNextSiblingItem(htiSibling);
		}
	}
	return false;
}

//=============================================================================	
void COffre2Cibles::OnNMClickFdTreeCibles(NMHDR *pNMHDR, LRESULT *pResult)
//=============================================================================	
{
	CPoint pt;

	// coordonnées par rapport au bord de l'écran
	if (GetCursorPos(&pt))
	{
		UINT uFlags;

		// donne les coordonnée dans la fenêtre
		::ScreenToClient(m_TreeCibles.GetSafeHwnd(), &pt);
		if (HTREEITEM hti = m_TreeCibles.HitTest(pt,&uFlags)) 
		{
			if( uFlags & (TVHT_ONITEMSTATEICON)) //|TVHT_ONITEMLABEL))
			{
				// Sélection du noeud courant
				ToggleItemState(hti);

				// Mise à jour de la liste dans dialogue items
				UpdateTargetList();

				// on met à jour l'indicateur du nombre d'elements
				UpdateIndicator();
			}		
		}
	}

	*pResult = 0;
}

#pragma endregion gestion des tree elements