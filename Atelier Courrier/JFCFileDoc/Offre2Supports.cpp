// DlgOffre2_Titres.cpp : fichier d'implémentation
//
#include "stdafx.h"

//=============================================================================	
// include pour contrôles/dialogues
#include "Offre2Supports.h"
#include "Offre2MainDialog.h"
#include "Offre2SupportsSource.h"
#include "DlgSupportsInexistants.h"
#include "SupportListElt.h"
#include "SupportListFunctors.h"
#include "JFCConfig.h"
#include "Offre2SupportsOngletCtrl.h"

//=============================================================================	
// includes pour les données
#include "JSRCTables.h"
#include "JSRCPresse.h"
#include "AllTypeOfNode.h"
#include "OffreTerrainManager.h"

//=============================================================================	
// includes C++ STL
#include <algorithm>
#include <functional>
#include <numeric>
#include <iterator>
#include <string>

#include <stdext.hpp>

#include "SupportListFunctors.h"

using namespace std;

//=============================================================================	
// Implémentation privée de COffre2Supports
struct COffre2Supports::Pimpl
//=============================================================================	
{
	vector<int> indexes, lastIndexes, diffIndexes;
};

// timer mise à jour chaine de recherche
#define IdTimer_SearchStrSupports WM_USER + 200

//=============================================================================	
// Boîte de dialogue DlgOffre2_Titres
IMPLEMENT_DYNAMIC(COffre2Supports, COffre2OngletBase)

// Gestionnaires de messages de COffre2Supports
BEGIN_MESSAGE_MAP(COffre2Supports, COffre2OngletBase)
	ON_WM_CTLCOLOR()
	ON_WM_TIMER()
	ON_EN_CHANGE	(IDC_TITLE_SEARCH_STR,					&COffre2Supports::OnEnChangeTitleSearchStr)
	ON_NOTIFY		(TCN_SELCHANGE, IDC_TABCTRL_SELECTION,	&COffre2Supports::OnTcnSelchangeTabctrl)
	ON_LBN_SELCHANGE(IDC_LST_FILTRES,						&COffre2Supports::OnLbnSelchangeLstFiltres)
	ON_LBN_SELCHANGE(IDC_LST_FILTRES_ELEMENTS,				&COffre2Supports::OnLbnSelchangeLstFiltresElements)
	ON_LBN_SELCHANGE(IDC_LST_TITRES,						&COffre2Supports::OnSelchangeListTitres)
	ON_BN_CLICKED	(IDC_RAZFILTER,							&COffre2Supports::OnBnClickedRazfilter)
	ON_BN_CLICKED	(IDC_RAZSELECT,							&COffre2Supports::OnBnClickedToutselect)
	ON_BN_CLICKED	(IDC_CK_SUPPORTS_FROM_SOURCE,			&COffre2Supports::OnBnClickedCkSupportsFromSource)
	ON_REGISTERED_MESSAGE	(WMU_SOURCE_ELEMENTS_CHANGE,					  OnSourceElementsChange)
END_MESSAGE_MAP()


//=============================================================================	
void COffre2Supports::DoDataExchange(CDataExchange* pDX)
//=============================================================================	
{
	COffre2OngletBase::DoDataExchange(pDX);
	DDX_Control	(pDX, IDC_LST_FILTRES,				m_FilterFoldersList);
	DDX_Control	(pDX, IDC_LST_FILTRES_ELEMENTS,		m_FilterElementsList);
	DDX_Control	(pDX, IDC_LST_TITRES,				m_TitresList);
	DDX_Control	(pDX, IDC_TABCTRL_SELECTION,		*m_pTabCtrl);
	DDX_Text	(pDX, IDC_TITLE_SEARCH_STR,			m_Filter.strSearch);
	DDX_Control	(pDX, IDC_FD_STATIC_TTLTITRES,		m_ttlTitres);
	DDX_Control	(pDX, IDC_FD_STATIC_TTLFILTRES,		m_ttlFiltres);
	DDX_Control(pDX, IDC_RAZFILTER,					m_RAZFilterBtn);
	DDX_Control(pDX, IDC_RAZSELECT,					m_RAZSupportsSelBtn);
	DDX_Control(pDX, IDC_CK_SUPPORTS_FROM_SOURCE,	m_SupFromSrcBtn);
	DDX_Control(pDX, IDC_ELEMS_INDICATOR,			m_indicator);
}

// a evidemment laisser dans cet ordre (libellés des categories de filtres)
static const char * ppLibellesFolders[]			=	{"Ma Sélection", "Périodicités", "Titres/Couplages/Familles", '\0'}; 
static const char * ppLibellesTypesSupports[]	=	{"Titres", "Couplages", "Familles", '\0'}; 


//=============================================================================
//				Implémentation de la classe COffre2Supports
//=============================================================================	

//=============================================================================	
COffre2Supports::COffre2Supports(COffre2MainDialog * pDialogOffre)
//=============================================================================	
	:	COffre2OngletBase(IDD, pDialogOffre)
	,	m_Filter(JSRCPresse::GetInstance().m_TBLPeriodicites.GetCount())
	// Init tables des sources
	,	m_srcTables (JSRCTables::GetInstance())
	,	m_srcPresse (JSRCPresse::GetInstance())
	,	m_bDontUpdateSearchStringThisTime(false)
	,	m_TitresList(pDialogOffre->GetAppColor())
{
	// création de l'onglet sélection
	m_pTabCtrl	= new COffre2SupportsOngletCtrl(this, m_pDlgOffre);

	m_pImpl = new Pimpl;
}

//=============================================================================	
COffre2Supports::~COffre2Supports()
//=============================================================================	
{
	if (m_pTabCtrl) delete m_pTabCtrl;
	m_pTabCtrl = 0;

	if (m_pImpl) delete m_pImpl;
	m_pImpl = 0;
}

//=============================================================================	
BOOL COffre2Supports::OnInitDialog()
//=============================================================================	 
{
	COffre2OngletBase::OnInitDialog();

	// configuration du label 'filtres'
	m_ttlFiltres.SetFontName(CATPColors::GetFontLabelName());
	m_ttlFiltres.SetFontSize(CATPColors::GetFontLabelSize());
	m_ttlFiltres.SetBorder(false);
	m_ttlFiltres.SetBkColor(CATPColors::GetColorDark(m_pDlgOffre->GetAppColor()),m_pDlgOffre->GetAppColor(),CATPColors::DARK_DEGRAD);
	m_ttlFiltres.SetTextColor(CATPColors::GetColorSuperDark(m_pDlgOffre->GetAppColor()));

	// configuration du label 'titres'
	m_ttlTitres.SetFontName(CATPColors::GetFontLabelName());
	m_ttlTitres.SetFontSize(CATPColors::GetFontLabelSize());
	m_ttlTitres.SetBorder(false);
	m_ttlTitres.SetBkColor(CATPColors::GetColorDark(m_pDlgOffre->GetAppColor()),m_pDlgOffre->GetAppColor(),CATPColors::DARK_DEGRAD);
	m_ttlTitres.SetTextColor(CATPColors::GetColorSuperDark(m_pDlgOffre->GetAppColor()));

	// configuration des boutons Reset
	const char pszTooltip1 [] = "Permet d'annuler le filtrage de supports";
	const char pszTooltip2 [] = "Permet de sélectionner/déselectionner tous les supports affichés";
	m_RAZFilterBtn.SetTooltipText(pszTooltip1);
	m_RAZFilterBtn.SetIcon(IDI_FD_RAZ,	IDI_FD_RAZ); 
	m_RAZSupportsSelBtn.SetTooltipText(pszTooltip2);	

	m_RAZFilterBtn.SetColor(CButtonST::BTNST_COLOR_BK_IN,		CATPColors::GetColorLight(m_pDlgOffre->GetAppColor()),m_pDlgOffre->GetAppColor(),0);
	m_RAZFilterBtn.SetColor(CButtonST::BTNST_COLOR_BK_OUT,		CATPColors::GetColorMedium(m_pDlgOffre->GetAppColor()),m_pDlgOffre->GetAppColor(),0);
	m_RAZFilterBtn.SetColor(CButtonST::BTNST_COLOR_BK_FOCUS,	CATPColors::GetColorDark(m_pDlgOffre->GetAppColor()),m_pDlgOffre->GetAppColor());

	m_RAZSupportsSelBtn.SetColor(CButtonST::BTNST_COLOR_BK_IN,		CATPColors::GetColorLight(m_pDlgOffre->GetAppColor()),m_pDlgOffre->GetAppColor(),0);
	m_RAZSupportsSelBtn.SetColor(CButtonST::BTNST_COLOR_BK_OUT,		CATPColors::GetColorMedium(m_pDlgOffre->GetAppColor()),m_pDlgOffre->GetAppColor(),0);
	m_RAZSupportsSelBtn.SetColor(CButtonST::BTNST_COLOR_BK_FOCUS,	CATPColors::GetColorDark(m_pDlgOffre->GetAppColor()),m_pDlgOffre->GetAppColor());


	// configuration du bouton 'supports de la source'
	const char pszTooltip3 [] = "Permet de choisir entre les supports de la source et tous les supports";

	m_SupFromSrcBtn.SetCheck(TRUE);

	// on charge la dernière selection
	set<JUnt32> selectedIds;
	const CSupportsSelection & Selection = m_pDlgOffre->GetSupportsSelection();

	copy(Selection.SelectedSupports.begin(), Selection.SelectedSupports.end(),
		insert_iterator<set<JUnt32>>(selectedIds, selectedIds.begin()));

	// on charge les titres appariés
	m_TitresList.UpdateApparies(Selection.IdSource, Selection.SelectedTerrains, selectedIds);

	// on charge tout les supports indépendants du filtre
	m_TitresList.InitializeData(selectedIds);

	ConstructSelectionTabs();

	// initialisations
	m_pTabCtrl->InitDialogs();
	m_pTabCtrl->ActivateTabDialogs();

	m_TitresList.SetCallingApp(m_pDlgOffre->GetAppColor());

	// on charge les elements des filtres
	LoadPeriodicites();

	// reset propriétés 'à état'
	m_Filter.strSearch.Empty();
	m_sLastSearchStr.Empty();

	// on remplit la liste des libellés de filtre
	m_FilterFoldersList.ResetContent();

	// on parcourt les libelles des categories de filtres
	const char ** ppLibelle = ppLibellesFolders;
	while(*ppLibelle)
	{
		// on ajoute le libellé, et on définit sa couleur de fond
		int idx = m_FilterFoldersList.AddLine(*ppLibelle, m_pDlgOffre->GetAppColorTemplate());
		ASSERT(idx != CB_ERR);
		ppLibelle++;
	}
	ASSERT(m_FilterFoldersList.GetCount() == effNumFolders);

	// on se place sur la périodicité par defaut
	m_FilterFoldersList.SetCurSel(m_eCurFilter = effPeriodicite);

	// par defaut, pas de filtrage
	m_Filter.titre_coupl_famil = 0;

	// on force un changement de catégories filtre
	OnLbnSelchangeLstFiltres();
	
	Offre2SupportsSource* pOngSource = static_cast<Offre2SupportsSource*>(m_pTabCtrl->GetTab(COffre2SupportsOngletCtrl::SelectionSource));
	ASSERT(pOngSource);
	pOngSource->FillSources();

	// mise à jour de l'indicateur d'elements
	UpdateIndicator();

	return TRUE;
}


//=============================================================================	
void COffre2Supports::OnEnChangeTitleSearchStr()
//=============================================================================	
{
	if (!m_bDontUpdateSearchStringThisTime)
	{
		SetTimer(IdTimer_SearchStrSupports, 500, NULL);
	}
	m_bDontUpdateSearchStringThisTime = false;
}

//=============================================================================	
void COffre2Supports::OnTimer(UINT_PTR nIDEvent)
//=============================================================================	
{
	if (nIDEvent == IdTimer_SearchStrSupports) 
	{
		KillTimer(IdTimer_SearchStrSupports);
		UpdateData();

		// check de changement de la chaine de filtrage
		if (m_sLastSearchStr != m_Filter.strSearch)
		{
			// modifie le cache de la liste des supports
			m_TitresList.UpdateDisplay(m_Filter);

			m_sLastSearchStr = m_Filter.strSearch;

			// mise à jour de l'indicateur d'elements
			UpdateIndicator();
		}
	}
	COffre2OngletBase::OnTimer(nIDEvent);
}


//=============================================================================	
void COffre2Supports::OnTcnSelchangeTabctrl(NMHDR *pNMHDR, LRESULT *pResult)
//=============================================================================	
{
	m_pTabCtrl->ActivateTabDialogs();

	// on met jour l'onglet courant
	m_pTabCtrl->GetActiveTab()->UpdateContent();

	*pResult = 0;
}

//=============================================================================	
void COffre2Supports::ConstructSelectionTabs()
//=============================================================================	
{
	CSupportsSelection & Selection = m_pDlgOffre->GetSupportsSelection();

	// on supprime tous les onglets autre que 'Terrains' (le 1er)
	const int count = m_pTabCtrl->GetItemCount();

	if (count == 0)
	{
		m_pTabCtrl->InsertItem(COffre2SupportsOngletCtrl::SelectionSource, "Source");
		m_pTabCtrl->m_Onglets[COffre2SupportsOngletCtrl::SelectionSource] = COffre2SupportsOngletCtrl::SelectionSource;
	}

	int idx = 1;
	for (; idx < count; ++idx)
		m_pTabCtrl->DeleteItem(idx);

	map<COffre2SupportsOngletCtrl::eOngletType, int>::iterator ong_it = m_pTabCtrl->m_Onglets.begin();
	while(ong_it != m_pTabCtrl->m_Onglets.end())
	{
		// on supprime tout après l'onglet 'terrains'
		if (ong_it->first != COffre2SupportsOngletCtrl::SelectionSource)
		{
			m_pTabCtrl->m_Onglets.erase(ong_it, m_pTabCtrl->m_Onglets.end());
			break;
		}
		++ong_it;
	}

	idx = 1;
	// on regarde le nombre de segments disponibles
	if (m_Segments.size() > 1)
	{
		m_pTabCtrl->InsertItem(idx, "Segments");
		m_pTabCtrl->m_Onglets[COffre2SupportsOngletCtrl::SelectionSegments] = idx;
		++idx;
	}
	else
		m_pTabCtrl->m_Onglets[COffre2SupportsOngletCtrl::SelectionSegments] = -1;

	// on regarde le nombre de types de poids disponibles
	if (m_TypesPoids.size() > 1)
	{
		m_pTabCtrl->InsertItem(idx, "Types de Poids");
		m_pTabCtrl->m_Onglets[COffre2SupportsOngletCtrl::SelectionTypesPoids] = idx;
		++idx;
	}
	else
		m_pTabCtrl->m_Onglets[COffre2SupportsOngletCtrl::SelectionTypesPoids] = -1;
}

//=============================================================================	
//								Gestion de la selection
//=============================================================================	
#pragma region gestion de la selection


bool COffre2Supports::ValidateSelection(bool bForceSelect /*= false*/)
{
	bool bModified = false;
	LoadTerrains();ValidateTerrains(bModified, bForceSelect);
	LoadSegments();ValidateSegments(bModified);
	LoadTypesPoids(); ValidateTypesPoids();

	return bModified;
}

//=============================================================================	
// fonction de tri par label
//=============================================================================	

// fonction de tri des périodicités par label
bool SortPeriodicitesLabel(const IElem * pPer1, const IElem * pPer2)
{
	return (_strnicmp(pPer1->GetLabel().AsJCharPtr(), pPer2->GetLabel().AsJCharPtr(),
		max(strlen(pPer1->GetLabel().AsJCharPtr()), strlen(pPer2->GetLabel().AsJCharPtr()))) < 0);
}

// fonction de tri des terrains par label
bool SortTerrainsLabel(const JTerrain * pTer1, const JTerrain * pTer2)
{
	return (_strnicmp(pTer2->m_Libelle.AsJCharPtr(), pTer1->m_Libelle.AsJCharPtr(),
		max(strlen(pTer2->m_Libelle.AsJCharPtr()), strlen(pTer1->m_Libelle.AsJCharPtr()))) < 0);
}

// fonction de tri des types de poids par label
bool SortTypesPoidsLabel(const JInt32 type1, const JInt32 type2)
{
	JChar * pLabel1 = JSRCTables::GetInstance().m_TBLPoids.GetLabelByID(type1);
	JChar * pLabel2 = JSRCTables::GetInstance().m_TBLPoids.GetLabelByID(type2);
	ASSERT (pLabel1 && pLabel2);

	return (_strnicmp(pLabel1, pLabel2,
		max(strlen(pLabel1), strlen(pLabel2))) < 0);
}

//=============================================================================	
void COffre2Supports::LoadTerrains()
//=============================================================================	
{
	m_Terrains.clear();

	// on vide les terrains de la source (du)
	m_Filter.sourceTerrains.clear();

	// on boucle sur ts les terrains existants
	for(m_srcTables.m_TBLTerrains.MoveFirst(); m_srcTables.m_TBLTerrains.IsValid(); m_srcTables.m_TBLTerrains.MoveNext())
	{
		const JTerrain & Terr = m_srcTables.m_TBLTerrains.GetItem();

		// on ajoute au vecteur uniquement ceux de la source en cours
		if(Terr.m_IdSource == m_pDlgOffre->GetSupportsSelection().IdSource)
		{
			m_Terrains.push_back(&Terr);
			m_Filter.sourceTerrains.push_back(&Terr);
		}
	}
	sort(m_Terrains.begin(), m_Terrains.end(), SortTerrainsLabel);
}

//=============================================================================	
void COffre2Supports::ValidateTerrains(bool & bModified, bool bForceSelect /*= false*/)
//=============================================================================	
{
	CSupportsSelection & Selection = m_pDlgOffre->GetSupportsSelection();

	// on vérifie chaque id terrain
	list<JUnt32>::iterator id_it = Selection.SelectedTerrains.begin();
	vector<const JTerrain*>::iterator ter_it;
	while (id_it != Selection.SelectedTerrains.end())
	{
		// on boucle sur tous les terrains chargés
		ter_it = m_Terrains.begin();
		while (ter_it != m_Terrains.end())
		{
			// on teste avec l'id en cours
			if((*ter_it)->m_IdTerrain == *id_it)
				break;
			++ter_it;
		}

		// on a pas trouvé l'id courant
		if (ter_it == m_Terrains.end())
		{
			// on l'efface et on passe au suivant
			id_it = Selection.SelectedTerrains.erase(id_it);
			bModified = true;
		}
		else ++id_it;
	}

	// on check qu'il y ait bien au moins un terrain sélectionné sinon on sélectionne le + récent (donc le 1er)
	if (bForceSelect && Selection.SelectedTerrains.empty())
	{
		if (!m_Terrains.empty())
			Selection.SelectedTerrains.push_back(m_Terrains[0]->m_IdTerrain);
	}

	// on met à jour le filtre
	m_Filter.terrains.clear();
	copy(Selection.SelectedTerrains.begin(), Selection.SelectedTerrains.end(), back_inserter(m_Filter.terrains));

	// on définit le terrain de ciblage
	Selection.SetTerrainCiblage();
}

//=============================================================================	
void COffre2Supports::LoadSegments()
//=============================================================================	
{
	const CSupportsSelection & Selection = m_pDlgOffre->GetSupportsSelection();

	m_Segments.clear();

	// on prend terrain de ciblage
	CKeyTerrain k(m_Filter.idSource, Selection.IdTerrainCiblage);
	CTerrain * pterrain = m_pDlgOffre->GetOffreTerrainManager()->GetTerrain(k);
	if (pterrain)
	{
		JVector<CSegment*> vect_segment;
		if( pterrain->GetSegments(vect_segment))
		{
			vect_segment.MoveFirst();
			// on boucle sur les segments du terrain de ciblage
			while (vect_segment.IsValid())
			{
				m_Segments.push_back(m_srcTables.m_TBLSegments.GetItem(vect_segment.GetItem()->GetID()));
				vect_segment.MoveNext();
			}
		}
	}

	// on affiche et utilise les segments ssi on en a au moins 2
	m_Filter.SetUseSegments(m_Segments.size() > 1);
}


//=============================================================================	
void COffre2Supports::ValidateSegments(bool & bModified)
//=============================================================================	
{
	CSupportsSelection & Selection = m_pDlgOffre->GetSupportsSelection();

	// on vérifie chaque id segment
	list<JUnt32>::iterator id_it = Selection.SelectedSegments.begin();
	vector<const JSegment*>::iterator seg_it;
	while (id_it != Selection.SelectedSegments.end())
	{
		// on boucle sur tous les segments chargés
		seg_it = m_Segments.begin();
		while (seg_it != m_Segments.end())
		{
			// on teste avec l'id en cours
			if((*seg_it)->m_IdSegment == *id_it)
				break;
			++seg_it;
		}

		// on a pas trouvé l'id courant
		if (seg_it == m_Segments.end())
		{
			// on l'efface et on passe au suivant
			id_it = Selection.SelectedSegments.erase(id_it);
			bModified = true;
		}
		else ++id_it;
	}

	m_Filter.SetAllSegments(m_Segments);

	//ASSERT(!m_Segments.empty());
	m_Filter.SetUseSegments(m_Segments.size() > 1);

	// on check qu'il y ait bien au moins un segment sélectionné (sinon on les selectionne tous)
	if (Selection.SelectedSegments.empty())
	{
		seg_it = m_Segments.begin();
		while(seg_it != m_Segments.end())
		{
			Selection.SelectedSegments.push_back((*seg_it)->m_IdSegment);
			++seg_it;
		}
	}

	// on met à jour le filtre
	m_Filter.segments.clear();
	copy(Selection.SelectedSegments.begin(), Selection.SelectedSegments.end(),
		back_inserter(m_Filter.segments));
}

//=============================================================================	
void COffre2Supports::LoadTypesPoids()
//=============================================================================	
{
	CSupportsSelection & Selection = m_pDlgOffre->GetSupportsSelection();

	// on récupère les poids du terrain de ciblage
	CTerrain * pterrain = NULL;
	CKeyTerrain k(Selection.IdSource, Selection.IdTerrainCiblage);
	pterrain = COffreTerrainManager::GetInstance()->GetTerrain(k);
	if (!pterrain)
	{
		// il est possible qu'aucun terrain soit sélectionné
		ASSERT (Selection.SelectedTerrains.empty() || Selection.IdTerrainCiblage == 0);
		return;
	}

	JVector<JInt32> vect;
	pterrain->GetTypesPoids(vect);

	// mise à jour de la liste de poids
	m_TypesPoids.clear();
	for(vect.MoveFirst(); vect.IsValid(); vect.MoveNext())
		m_TypesPoids.push_back(vect.GetItem());

	// on classe les types de poids par label
	sort(m_TypesPoids.begin(), m_TypesPoids.end(), SortTypesPoidsLabel);
}

//=============================================================================	
void COffre2Supports::ValidateTypesPoids()
//=============================================================================	
{
	CSupportsSelection & Selection = m_pDlgOffre->GetSupportsSelection();
	IElem * pSelTypePoids = m_srcTables.m_TBLPoids.GetElemByID(Selection.IdTypePoids);

	// on cherche le type de poids sélectionné parmis ceux qui sont chargés
	std::vector<const JInt32>::const_iterator found_sel = std::find(m_TypesPoids.begin(), m_TypesPoids.end(), Selection.IdTypePoids);

	if (found_sel == m_TypesPoids.end())
	{
		// problème, la sélection n'existe pas!
		// on prend le 1er type de poids de la liste
		if (m_TypesPoids.empty())
			Selection.IdTypePoids = 0;
		else
			Selection.IdTypePoids = m_TypesPoids[0];
	}
}


//=============================================================================	
void COffre2Supports::UpdateSupportsDisplay()
//=============================================================================	
{
	m_TitresList.UpdateDisplay(m_Filter);

	// on récupère la sélection
	GetControlSelection(m_pImpl->lastIndexes, m_TitresList);

	// mise à jour de l'indicateur
	UpdateIndicator();
}


//=============================================================================	
//				gestion evenements affichage, evenement fenêtres
//=============================================================================	
#pragma region gestion evenements affichage, evenement fenêtres

HBRUSH COffre2Supports::GetBackgroundBrush()
{
	return m_pDlgOffre->GetBackgroundBrush();
}

//=============================================================================	
HBRUSH COffre2Supports::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
//=============================================================================	
{
	static int ctrlid;

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
		else if (ctrlid == IDC_CK_SUPPORTS_FROM_SOURCE)
		{
			pDC->SetBkColor(CATPColors::GetColorMedium(m_pDlgOffre->GetAppColor()));
			return m_pDlgOffre->GetBackgroundBrush();
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

		ctrlid = pWnd->GetDlgCtrlID();

		// on paint l'indicateur de la couleur claire
		if (ctrlid == IDC_ELEMS_INDICATOR)
		{
			pDC->SetBkColor(CATPColors::GetColorLight(m_pDlgOffre->GetAppColor()));    // change the background
			return static_cast<HBRUSH> (m_pDlgOffre->GetBackgroundBrush());
		}
		break;
	}

	// sinon on paint comme d'haibute
	return (CDialog::OnCtlColor(pDC, pWnd, nCtlColor));
}

#pragma endregion gestion evenements affichage, evenement fenêtres

//=============================================================================	
// remplit la partie de l'offre correspondant à l'onglet (Terrains + Supports)
void COffre2Supports::FillOffre(JATPOffre * pOffre)
//=============================================================================	
{
	// la sélection précedente des supports (on copie ceux de l'offre)
	list<JUnt32> lastSupports;
	for (pOffre->m_TBLIdSupports.MoveFirst(); pOffre->m_TBLIdSupports.IsValid(); pOffre->m_TBLIdSupports.MoveNext())
		lastSupports.push_back(pOffre->m_TBLIdSupports.GetItem());
	lastSupports.sort();

	// on récupère tous les supports sélectionnés
	list<const IElem*> selectedSupports;
	m_TitresList.GetWholeSelection(selectedSupports);

	// on récupère la sélection
	CSupportsSelection & Selection = m_pDlgOffre->GetSupportsSelection();

	// on compare les supports précedemment sélectionnés et la sélection actuelle
	if ((lastSupports.empty() ^ Selection.SelectedSupports.empty())
		|| (!(lastSupports.empty() || Selection.SelectedSupports.empty())
		&& !equal(lastSupports.begin(), lastSupports.end(), Selection.SelectedSupports.begin())))
	{
		pOffre->m_Changes |= pOffre->SUPPORTS;
	}

	// on les copie tous dans l'offre (et on construit en même tps la table des libellés)
	pOffre->m_TBLIdSupports.Reset();
	pOffre->m_mapLblSupport.Reset();
	list<const IElem*>::const_iterator sel_it = selectedSupports.begin();
	while (sel_it != selectedSupports.end())
	{
		pOffre->m_TBLIdSupports.AddTail() = static_cast<JInt32>((*sel_it)->GetID());
		pOffre->m_mapLblSupport.Add((*sel_it)->GetID()) = (*sel_it)->GetLabel();
		++sel_it;
	}

	int idxOng = COffre2SupportsOngletCtrl::SelectionSource;
	for(; idxOng < COffre2SupportsOngletCtrl::NumOnglets; ++idxOng)
	{
		COffre2SupportsOngletCtrl::eOngletType eOng = static_cast<COffre2SupportsOngletCtrl::eOngletType>(idxOng);
		m_pTabCtrl->GetTab(eOng)->FillOffre(pOffre);
	}
}

#pragma region gestion des types de supports

//=============================================================================	
void COffre2Supports::SelectTypesSupports()
//=============================================================================	
{
	// on sélectionne certains types de supports en fonction de la valeur du mask dans le filtre
	m_FilterElementsList.SetSel(0, m_Filter.titre_coupl_famil & CSupportFilter::DISPLAY_TITRES);
	m_FilterElementsList.SetSel(1, m_Filter.titre_coupl_famil & CSupportFilter::DISPLAY_COUPLAGES);
	m_FilterElementsList.SetSel(2, m_Filter.titre_coupl_famil & CSupportFilter::DISPLAY_FAMILLES);
}

//=============================================================================	
void COffre2Supports::DisplayTypesSupports()
//=============================================================================	
{
	m_FilterElementsList.ResetContent();

	// on ajoute le libellé, et on définit sa couleur de fond
	int item_idx = m_FilterElementsList.AddLine(ppLibellesTypesSupports[0], m_TitresList.GetXTemplateTitre());
	m_FilterElementsList.SetItemData(item_idx, static_cast<DWORD_PTR>(CSupportFilter::DISPLAY_TITRES));

	item_idx = m_FilterElementsList.AddLine(ppLibellesTypesSupports[1], m_TitresList.GetXTemplateCouplages());
	m_FilterElementsList.SetItemData(item_idx, static_cast<DWORD_PTR>(CSupportFilter::DISPLAY_COUPLAGES));

	item_idx = m_FilterElementsList.AddLine(ppLibellesTypesSupports[2], m_TitresList.GetXTemplateFamilles());
	m_FilterElementsList.SetItemData(item_idx, static_cast<DWORD_PTR>(CSupportFilter::DISPLAY_FAMILLES));

	ASSERT(m_FilterFoldersList.GetCount() == effNumFolders);
}

#pragma endregion gestion des types de supports
#pragma region gestion des periodicites

//=============================================================================	
void COffre2Supports::LoadPeriodicites()
//=============================================================================	
{
	m_Periodicites.clear();

	// on boucle sur ts les periodicites existantes
	m_srcPresse.m_TBLPeriodicites.MoveFirst();
	while (m_srcPresse.m_TBLPeriodicites.IsValid())
	{
		const IElem * pPerioElem = m_srcPresse.m_TBLPeriodicites.GetItem();
		// volontairement, on n'affiche pas les 'Hors-Serie', id 8 dans Periodicites.tbl
		if (pPerioElem->GetID() != 8)
			m_Periodicites.push_back(pPerioElem);
		m_srcPresse.m_TBLPeriodicites.MoveNext();
	}

	sort(m_Periodicites.begin(), m_Periodicites.end(), SortPeriodicitesLabel);
}

//=============================================================================	
void COffre2Supports::DisplayPeriodicites()
//=============================================================================	
{
	m_FilterElementsList.ResetContent();

	// on boucle sur ts les periodicites de la liste
	std::vector<const IElem*>::iterator per_it = m_Periodicites.begin();
	while (per_it != m_Periodicites.end())
	{
		const IElem * pPeriodicite = *per_it;
		// ajout de l'id periodicité à la listbox
		int idx = m_FilterElementsList.AddLine(pPeriodicite->GetLabel().AsJCharPtr(), m_pDlgOffre->GetAppColorTemplate());
		m_FilterElementsList.SetItemData(idx, static_cast<DWORD_PTR>(pPeriodicite->GetID()));
		++per_it;
	}
}

//=============================================================================	
void COffre2Supports::SelectPeriodicites()
//=============================================================================	
{
	// on check qu'on est bien sur le folder 'periodicites'
	ASSERT(m_eCurFilter == effPeriodicite);

	list<JUnt32>::iterator per_it = m_Filter.periodicites.begin();
	while(per_it != m_Filter.periodicites.end())
	{
		// on cherche la périodicité dont l'id vaut *per_it
		const size_t nCount = m_Periodicites.size();
		for(size_t idx = 0; idx < nCount; ++idx)
		{
			if (m_Periodicites[idx]->GetID() == (*per_it))
			{
				m_FilterElementsList.SetSel(static_cast<int>(idx), TRUE);
				break;
			}
		}
		++per_it;
	}
	// on check le nbr d'elts sélectionnés
	ASSERT(m_FilterElementsList.GetSelCount() == static_cast<int>(m_Filter.periodicites.size()));
}

#pragma endregion gestion des periodicites

//=============================================================================	
void COffre2Supports::OnBnClickedRazfilter()
//=============================================================================	
{
	// on reset les elements réels du filtre (ceux modifiables par l'utilisateur)
	m_Filter.periodicites.clear();
	m_Filter.titre_coupl_famil = 0;
	m_Filter.strSearch.Empty();

	// on supprime le texte de la chaine de recherche
	GetDlgItem(IDC_TITLE_SEARCH_STR)->SetWindowText("");

	// on indique au timer de modif de la chaine de recherche ne ne pas s'activer sur cette modif
	m_bDontUpdateSearchStringThisTime = true;

	// on bloque les listes
	m_FilterFoldersList.LockWindowUpdate();
	m_FilterElementsList.LockWindowUpdate();

	// on déselectionne les elements de filtre qui sont sélectionnés
	m_FilterElementsList.SelItemRange(FALSE, 0, m_FilterElementsList.GetCount());

	const int curSel = m_FilterFoldersList.GetCurSel();

	// on dégrise les fond des listes de catégroies de filtre dans lesquelles des elements sont sélectionnés
	m_FilterFoldersList.SetBackgroundColor(effPeriodicite, CATPColors::GetColorWhite());
	m_FilterFoldersList.SetBackgroundColor(effTitres_Couplages_Familles, CATPColors::GetColorWhite());

	// on bloque les listes
	m_FilterElementsList.UnlockWindowUpdate();
	m_FilterFoldersList.UnlockWindowUpdate();

	m_FilterFoldersList.SetCurSel(curSel);

	// on reaffiche les elements après avoir supprimé le filtre
	m_TitresList.UpdateDisplay(m_Filter);

	// on récupère la sélection
	GetControlSelection(m_pImpl->lastIndexes, m_TitresList);

	// mise à jour de l'indicateur d'elements
	UpdateIndicator();
}


//=============================================================================	
void COffre2Supports::OnBnClickedToutselect()
//=============================================================================	
{
	// si la sélection a été modifiée
	if (m_TitresList.GetCount())
		m_pDlgOffre->SetModified();

	m_TitresList.OnClickOnSelectTout(m_eCurFilter == effSelection);

	// on récupère la sélection
	GetControlSelection(m_pImpl->lastIndexes, m_TitresList);

	// mise à jour de l'indicateur d'elements
	UpdateIndicator();

	// mise à jour du bouton OK
	m_pDlgOffre->EnableOK();
}

//=============================================================================	
void COffre2Supports::OnSelchangeListTitres()
//=============================================================================	
{
	int idx = 0;
	const int nmax = m_TitresList.GetCount();
	while (idx < nmax)
	{
		SupportListElt * pElem = reinterpret_cast<SupportListElt*>(m_TitresList.GetItemData(idx));
		pElem->iSelected = FALSE;
		++idx;
	}

	// on récupère les indexes venant d'être ajoutés
	GetControlSelection(m_pImpl->indexes, m_TitresList);
	m_pImpl->diffIndexes.clear();
	set_difference(m_pImpl->indexes.begin(), m_pImpl->indexes.end(),
		m_pImpl->lastIndexes.begin(), m_pImpl->lastIndexes.end(), back_inserter(m_pImpl->diffIndexes));


	bool bDisplayFamilleVideMsg = false;

	// on affiche le message 'impossible de sélectionner une famille vide que lorsqu'on vient de 
	// sélectionner une seule famille vide, sinon le msg apparaitrait trop souvent'
	if (m_pImpl->diffIndexes.size() == 1)
	{
		SupportListElt* pNewElem = reinterpret_cast<SupportListElt*>(m_TitresList.GetItemData(*m_pImpl->diffIndexes.begin()));
		if (pNewElem->bIsFamille && !pNewElem->pElem->IsLot())
		{
			// on ne peut pas sélectionner une famille vide
			bDisplayFamilleVideMsg = true;
			// on déselectionne l'element
			m_TitresList.SetSel(*m_pImpl->diffIndexes.begin(), FALSE);
			m_pImpl->diffIndexes.clear();
		}
	}

	// on affiche éventuellement le message
	if (bDisplayFamilleVideMsg)
		AfxMessageBox("Impossible de sélectionner une famille vide!");

	// on s'assure de n'avoir aucune famille vide sélectionné (visuellement et dans l'offre)
	vector<int>::iterator idx_it = m_pImpl->indexes.begin();
	while(idx_it != m_pImpl->indexes.end())
	{		
		SupportListElt* pElem = reinterpret_cast<SupportListElt*>(m_TitresList.GetItemData(*idx_it));
		if (pElem->bIsFamille && !pElem->pElem->IsLot())
		{
			// on déselectionne l'element
			m_TitresList.SetSel(*idx_it, FALSE);
			idx_it = m_pImpl->indexes.erase(idx_it);
		}
		else
			++idx_it;
	}

	// on récupère les elements sélectionnés
	static set<SupportListElt*> elements;
	elements.clear();
	GetControlSelectedData(elements, m_TitresList);

	set<SupportListElt*>::iterator selelt_it = elements.begin();
	while(selelt_it != elements.end())
	{
		// si ce n'est pas une famille ou que la famille n'est pas vide, on peut selectionner
		if (!(*selelt_it)->bIsFamille || (*selelt_it)->pElem->IsLot())
			(*selelt_it)->iSelected = TRUE;
		++selelt_it;
	}

	// on indique au dialogue parent une modif
	m_pDlgOffre->SetModified();

	// mise à jour du bouton OK de la boite principale de l'Offre
	m_pDlgOffre->EnableOK();

	// mise à jour de l'indicateur d'elements
	UpdateIndicator();

	// on copie la sélection
	m_pImpl->lastIndexes.clear();
	copy(m_pImpl->indexes.begin(), m_pImpl->indexes.end(), back_inserter(m_pImpl->lastIndexes));
}

//=============================================================================	
void COffre2Supports::OnLbnSelchangeLstFiltres()
//=============================================================================	
{
	static eFilterFolder lastFolder = effNumFolders;

	// on récupère la nouvelle sélection de filtre
	m_eCurFilter = static_cast<eFilterFolder>(m_FilterFoldersList.GetCurSel());

	// on empêche de n'avoir aucun filtre sélectionné
	if (m_eCurFilter == LB_ERR)
	{
		ASSERT(lastFolder != LB_ERR);
		m_FilterFoldersList.SetCurSel(lastFolder);
	}

	// on n'etait en 'Ma Sélection' et on n'y est plus, on reaffiche les supports filtrés
	if (lastFolder == effSelection)
	{
		m_Filter.onlyselected = false;

		// on réactive le bouton 'supports de la source'
		m_SupFromSrcBtn.EnableWindow();

		set<JUnt32> selectedIds;
		m_TitresList.GetWholeSelection(selectedIds);

		m_TitresList.UpdateDisplay(m_Filter);
	}

	// on check la catégorie de filtre ayant été modifiée
	switch (m_eCurFilter)
	{
	case effSelection:
		
		m_Filter.onlyselected = true;

		// on desactive le bouton 'supports de la source'
		m_SupFromSrcBtn.EnableWindow(FALSE);

		// on supprime le contenu des elements de filtre
		m_FilterElementsList.ResetContent();

		m_TitresList.UpdateDisplay(m_Filter);
				
		break;
	case effPeriodicite:
		m_Filter.onlyselected = false;
		DisplayPeriodicites();
		SelectPeriodicites();
		break;
	case effTitres_Couplages_Familles:
		m_Filter.onlyselected = false;
		DisplayTypesSupports();
		SelectTypesSupports();
		break;
	}

	// on backup le dernier folder sélectionné
	lastFolder = m_eCurFilter;

	// on récupère la sélection
	GetControlSelection(m_pImpl->lastIndexes, m_TitresList);

	// mise à jour de l'indicateur d'elements
	UpdateIndicator();
}

//=============================================================================	
void COffre2Supports::OnLbnSelchangeLstFiltresElements()
//=============================================================================	
{
	// on check la catégorie de filtre ayant été modifiée
	switch (m_eCurFilter)
	{
		// on récupère le(s) terrain(s) sélectionné(s), on met à jour le filtre
	case effSelection:
		{
			m_FilterElementsList.ResetContent();
		}break;

		// on récupère la(les) périodicité(s) sélectionnée(s), on met à jour le filtre
	case effPeriodicite:
		{
			// on nettoie les périodicités du filtre
			m_Filter.periodicites.clear();

			// on récupère les data des elements sélectionnés de la liste
			GetControlSelectedData(m_Filter.periodicites, m_FilterElementsList);

			// modifie le cache de la liste des supports
			m_TitresList.UpdateDisplay(m_Filter);

			// on affiche la catégorie de filtre en gris clair si on a 1 sélection
			if (!m_Filter.periodicites.empty())
				m_FilterFoldersList.SetBackgroundColor(effPeriodicite, CATPColors::GetColorGrisClair());
			else
				m_FilterFoldersList.SetBackgroundColor(effPeriodicite, CATPColors::GetColorWhite());

			m_FilterFoldersList.SetCurSel(effPeriodicite);

		} break;

		// on récupère le(s) type(s) de supports sélectionné(s), on met à jour le filtre
	case effTitres_Couplages_Familles:
		{
			// on récupère un vecteur de data contenant les types de supports selectionnés
			vector<JUnt32> selection;
			if (GetControlSelectedData(selection, m_FilterElementsList))
			{
				m_Filter.titre_coupl_famil = accumulate(selection.begin(), selection.end(), 0, bitwise_or<JUnt32>());
				ASSERT(m_Filter.titre_coupl_famil >= 0 && m_Filter.titre_coupl_famil <= CSupportFilter::DISPLAY_ALL);
			}
			else m_Filter.titre_coupl_famil = 0;

			// modifie le cache de la liste des supports
			m_TitresList.UpdateDisplay(m_Filter);

			// on affiche la catégorie de filtre en gris clair si on a 1 sélection
			if (m_Filter.titre_coupl_famil > 0)
				m_FilterFoldersList.SetBackgroundColor(effTitres_Couplages_Familles, CATPColors::GetColorGrisClair());
			else
				m_FilterFoldersList.SetBackgroundColor(effTitres_Couplages_Familles, CATPColors::GetColorWhite());

			m_FilterFoldersList.SetCurSel(effTitres_Couplages_Familles);
		}
		break;
	}

	// on récupère la sélection
	GetControlSelection(m_pImpl->lastIndexes, m_TitresList);

	// mise à jour de l'indicateur d'elements
	UpdateIndicator();
}

//=============================================================================	
void COffre2Supports::OnBnClickedCkSupportsFromSource()
//=============================================================================	
{
	// on récupère la valeur du check
	m_Filter.supports_from_source = m_SupFromSrcBtn.GetCheck() != FALSE ? true : false;

	// on modifie l'affichage
	m_TitresList.UpdateDisplay(m_Filter);

	// on récupère la sélection
	GetControlSelection(m_pImpl->lastIndexes, m_TitresList);

	// mise à jour de l'indicateur d'elements
	UpdateIndicator();
}

//=============================================================================	
// retourne true si l'état de la sélection est correct
bool COffre2Supports::IsSelectionOk() const
//=============================================================================	
{
	// il faut au moins 1 support sélectionné, au moins 1 terrain, 1 segment et 1 type de poids
	if (m_TitresList.HasAtLeastOneSupportSelected())
		if (m_pTabCtrl->GetTab(COffre2SupportsOngletCtrl::SelectionSource)->IsSelectionOk())
			if (m_pTabCtrl->GetTab(COffre2SupportsOngletCtrl::SelectionSegments)->IsSelectionOk())
				if (m_pTabCtrl->GetTab(COffre2SupportsOngletCtrl::SelectionTypesPoids)->IsSelectionOk())
					return true;
	return false;
}

//=============================================================================	
// mise à jour de l'indicateur
void COffre2Supports::UpdateIndicator()
//=============================================================================	
{
	CString sMsg;
	const int selcount = m_TitresList.GetSelCount();

	// on a des elements sélectionnés, on affiche le nbr d'elements sélectionnés
	if (selcount > 0)
	{	
		if (selcount == 1)	sMsg = "1 élement sélectionné";
		else				sMsg.Format("%d élements sélectionnés", selcount);	
	}
	else
	{
		// sinon on affiche seulement le nombre d'elements dans la liste
		const int count = m_TitresList.GetCount();
		if (count == 0)		sMsg = "aucun élément";
		else if (count == 1)sMsg = "1 élément";
		else				sMsg.Format("%d éléments", count);
	}
	m_indicator.SetWindowText(sMsg);
}

//=============================================================================	
// callback appelé lors du changement d'un element source (source-terrain)
LRESULT COffre2Supports:: OnSourceElementsChange(WPARAM wParam, LPARAM lParam)
//=============================================================================	
{
	int elementMask = static_cast<int>(wParam);

	// si on a un changement de source
	// if (elementMask | COffre2MainDialog::SOURCE_CHANGE) régle problème sélection support Track n° 58
	if (elementMask & COffre2MainDialog::SOURCE_CHANGE)
	{
		// on supprime l'entière sélection de supports
		m_TitresList.ResetSelection();
	}

	// si on a un changement de terrain
	// if (elementMask | COffre2MainDialog::TERRAINS_CHANGE) régle problème sélection support Track n° 58
	if (elementMask & COffre2MainDialog::TERRAINS_CHANGE)
	{
		// on crée un set des ids des supports sélectionnés
		set<JUnt32> selectedIds;

		// CSupportsSelection &SupSel = m_pDlgOffre->GetSupportsSelection();

		copy(m_pDlgOffre->GetSupportsSelection().SelectedSupports.begin(), m_pDlgOffre->GetSupportsSelection().SelectedSupports.end(),
			insert_iterator<set<JUnt32>>(selectedIds, selectedIds.begin()));



		// mise à jour des titres appariés
		m_TitresList.UpdateApparies(m_pDlgOffre->GetSupportsSelection().IdSource,
									m_pDlgOffre->GetSupportsSelection().SelectedTerrains,
									selectedIds);
	}

	UpdateIndicator();
	return 0;
}
