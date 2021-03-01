// TargetList.cpp : fichier d'implémentation
//

#include "stdafx.h"
#include "TargetList.h"
#include "IItem.h"
#include "Offre2MainDialog.h"
#include "Offre2Cibles.h"
#include "Offre2Selection.h"
#include "ItemsConstruitsManager.h"
#include "ItemsConstruitsBase.h"
#include "OffreTerrainManager.h"

// pour les seuils
#include "JSource.h"
#include "JSRCTables.h"
#include "CalcItemParams.h"

#include "DlgMessage.h"

#include "Resource.h"

#include <algorithm>
#include <stdext.hpp>
#include <set>
#include <string>

const char pcszRefAffinite[] = "(Ref. Affinité)";

//=============================================================================	
//=============================================================================	
//						CTargetList : Implémentation privée
//=============================================================================	
//=============================================================================	
#pragma region CTargetList : Implémentation privée

struct TooltipContentLine
{
	CString terrainName;
	JInt32	nbCas;
};

// niveaux de restriction des items (possibilité de les sélectionner)
typedef enum eRestrictionLevel
{
	eNormalItem = 0,		// item/palette utilisable	(au moins 1 element utilisable et 0 warnings)
	eWarningSeuil,			// avertissement			(au moins 1 element utilisable et au moins 1 warnings)
	eForbiddenItem,			// interdit	,inutilisable	(aucun element utilisable)
	eNumElements
};


struct ItemRestrictionInfo
{
	CString	terrain;		// libellés de terrains (sans source)
	JInt32	seuilterrain;	// seuil avertissement ou rédhibitoire (en fonction de la demande)
	JInt32	nbcasItem;		// nb cas de l'item sur le terrain
};


// définitions de types
typedef std::map<JInt32, CCalcItemParams*>	TerrainParamsMap;	// mappe, pour id 1 terrain donné, les parametres de calcul
typedef TerrainParamsMap::iterator			TerrainParamsIter;

typedef std::map<JInt32, TerrainParamsMap*> ItemParamsMap;		// mappe, par item, les 'TerrainParamsMap'
typedef ItemParamsMap::iterator				ItemParamsIter;

typedef std::map<JInt32, eRestrictionLevel> ItemRestrictMap;	// mappe, par item, le niveau d'interdiction calculé
typedef ItemRestrictMap::const_iterator		ItemRestrictIter;

typedef std::list<const JTerrain *>			TerrainList;		// liste de JTerrain
typedef TerrainList::const_iterator			TerrainListIter;

typedef std::list<const IItem *>			IItemList;			// liste d'IItem
typedef IItemList::iterator					IItemIter;

typedef std::set<const JTerrain *>			TerrainSet;
typedef TerrainSet::iterator				TerrainSetIter;

typedef std::map<JInt32, TerrainSet*>		ItemTerrainsIdMap;	// mappe, par id item, des terrains
typedef ItemTerrainsIdMap::iterator			ItemTerrainsIter;

//=============================================================================	
struct CTargetList::Pimpl
//=============================================================================	
{
	Pimpl(CTargetList & TargetList) : m_BackRef(TargetList)
	{
		m_pSource = 0;
		m_pSrcTables = (JSRCTables*)& JSRCTables::GetInstance();
		m_bReloadCacheSeuils = true;

		// on initialize les templates
		for (int idx = 0; idx < eNumElements; ++idx)
			m_hxTemplates[idx] = CXTemplateManager::InvalidHandle;
	}

	// back reference de l'objet principal
	CTargetList & m_BackRef;

	// la gestion des seuils se fait dans l'implémentation privée
	void ResetMapSeuils();
	void AddItemParam(const CItemConstruit * pItem);
	void AddToolTipTextForItem(const JInt32 ItemId, CString & sTooltipText);

	// calcul niveaux d'interdiction des items
	eRestrictionLevel GetRestrictionLevel(const IItem * pItem);
	eRestrictionLevel GetItemRestrictionLevel(const CItemConstruit * pItem);
	eRestrictionLevel GetPaletteRestrictionLevel(const CPalettes * pPalettes);

	// génération et affichage du message lorsque l'item/palette est restreint(e) de manière complète
	void DisplayWarningMessage(const CItemConstruit * pItemConstruit, bool & bUnselect);
	void DisplayWarningMessage(const CPalettes * pPalettes, bool & bUnselect);

	// génération et affichage du message lorsque l'item/palette est restreint(e) de manière partielle
	void DisplayForbidenMessage(const CItemConstruit * pItemConstruit);
	void DisplayForbidenMessage(const CPalettes * pPalettes);

	void FillRestrictionInfo(eRestrictionLevel infotype, std::vector<ItemRestrictionInfo>& infos, const CItemConstruit * pItemConstruit);

	ItemRestrictMap			m_Restrictions;		// map des niveaux d'interdiction des items (items/palettes)
	ItemParamsMap			m_ItemsParams;		// map de sauvegarde des elements calculés (items)

	ItemTerrainsIdMap		m_ForbiddenTerrains;// map des terrains interdits par item
	ItemTerrainsIdMap		m_WarningTerrains;	// map des terrains en seul d'avrtissement par item

	CItemsConstruitsBase *	m_pItemsConstruits;
	JSRCTables *			m_pSrcTables;
	TerrainList				m_ListTerrains;
	const JSource *			m_pSource;

	bool					m_bReloadCacheSeuils;	// à true : indique qu'un recalcul de la validité
													// des supports par rapport aux seuils est nécessaire
	HXTemplateHandle		m_hxTemplates[eNumElements];	// style template
	COLORREF				m_RefAffTextColor;		// couleur du texte pour la reference affinité

	// functor amis
	friend struct AddToTargetList;
};

#pragma endregion CTargetList : Implémentation privée

//=============================================================================	
//							CTargetList functors
//=============================================================================	
#pragma region CTargetList functors

//=============================================================================	
// Foncteur de tri de libellés de cibles
//=============================================================================	
bool TargetLabelSort::operator() (const IItem * pItem1, const IItem * pItem2) const
{
	return PrepareStr(std::string(pItem1->GetLabel().AsJCharPtr(), strlen(pItem1->GetLabel().AsJCharPtr())))
		< PrepareStr(std::string(pItem2->GetLabel().AsJCharPtr(), strlen(pItem2->GetLabel().AsJCharPtr())));
/*
	CString sItem1(pItem1->GetLabel().AsJCharPtr());
	CString sItem2(pItem2->GetLabel().AsJCharPtr());

	return sItem1.CompareNoCase(sItem2);
*/

	//// taille du + petit element
	//const size_t minLen = min(strlen(pItem1->GetLabel().AsJCharPtr()),strlen(pItem2->GetLabel().AsJCharPtr()));

	//// compare les chaines alphabetiquement
	//return strncmp(pItem1->GetLabel().AsJCharPtr(), pItem2->GetLabel().AsJCharPtr(), minLen) < 0;	
}


void TargetLabelSort::RemoveAccents(char & c)
{
	if		(c == 'é' || c == 'è' || c == 'ê')	c = 'e';
	if		(c == 'à' || c == 'â')				c = 'a';
}

const std::string & TargetLabelSort::PrepareStr(std::string & s) const
{
	static std::string::size_type pos;
	const size_t len = s.length();

	std::transform( s.begin(), s.end(), s.begin(), (int(*)(int))::tolower );
	int dbg = 0;

	// trim left
	pos = s.find_first_not_of(" \t\n\r");
	if (pos != std::string::npos) s = s.substr(pos);

	// trim right
	pos = s.find_last_not_of(" \t\n\r");
	if (pos != std::string::npos) s = s.substr(0, ++pos);

	std::for_each(s.begin(), s.end(), RemoveAccents);
	return s;
}


//struct AlphabeticalCompare
//{
//	bool operator () (const IItem * item1, const IItem * item2) const
//	{
//		return item1->GetLabel() < item2->GetLabel();
//	}
//};

/*
bool operator < (const IItem & Item1, const IItem & Item2)
{
	TargetLabelSort ItemCompare;
	return ItemCompare(&Item1, &Item2);
}
*/

//=============================================================================	
// functor testant la restriction d'un element 
struct IfRestrictionIs
	:	public std::unary_function<const int, bool>
//=============================================================================	
{
	IfRestrictionIs(CTargetList & listbox, eRestrictionLevel restriction_to_test, ItemRestrictMap & restrictions)
		:	_listbox(listbox), _all_restrictions(restrictions), _restriction(restriction_to_test) {}

	bool operator () (const int lb_idx)
	{
		const IItem * pItem = reinterpret_cast<const IItem *>(_listbox.GetItemData(lb_idx));
		return _restriction == _all_restrictions[pItem->GetID()];
	}

	ItemRestrictMap &		_all_restrictions;	// copie de ttes les restrictions 
	const eRestrictionLevel _restriction;		// la restriction à tester
	CTargetList &			_listbox;
};

//=============================================================================	
struct CalculateSeuilsElements
	: public std::unary_function<const IItem *, void>
//=============================================================================	
{
	CalculateSeuilsElements(int eDisplay, CTargetList::Pimpl & Impl)
		:	_eDisplay(eDisplay), _Impl(Impl)
	{}

	int _eDisplay;
	CTargetList::Pimpl & _Impl;
	eRestrictionLevel	_restriction;

	void operator () (const IItem * pItem) const
	{
		// on crée tous les parametres de calcul pour cet elements, puis le niveau de restriction
		switch (_eDisplay)
		{
		case COffre2Cibles::effItems:
			_Impl.AddItemParam(pItem->AsItemConstruit());
			break;
		case COffre2Cibles::effPalettes:
			{	
				// on crée les parametres pour chq elt de la palette
				const CPalettes * palette = static_cast<const CPalettes *>(pItem->AsPalette());
				std::list<const CItemConstruit *> itemList;
				palette->GetItems(itemList);

				std::list<const CItemConstruit *> ::const_iterator itconstr_it = itemList.begin();
				while(itconstr_it != itemList.end())
				{
					_Impl.AddItemParam(*itconstr_it);
					++itconstr_it;
				}
			}
			break;
		}

		// calcul du niveau de restriction (possibilité de sélectionner l'item
		// en fonction du nbr de cas et d'un certain seuil)
		_Impl.GetRestrictionLevel(pItem);
	}
};


//=============================================================================	
// Interface d'un foncteur permettant de filtrer des cibles
//=============================================================================	
struct ITargetFilterFunc :  public std::unary_function<const IItem *, bool>
{
	ITargetFilterFunc() {}

	virtual bool operator()(const IItem *) const = 0;
};

//=============================================================================	
//=============================================================================	
//								IsTargetLabelContains
// Renvoie true si le libellé du cible contient la chaine passée au constructeur
//=============================================================================	
//=============================================================================	

//=============================================================================	
// Renvoie true si le libellé de la cible contient la chaine passée au constructeur
// NB : une chaine vide renvoie true
struct IsTargetLabelContains : public ITargetFilterFunc, TitleLabelBase
//=============================================================================	
{
	IsTargetLabelContains(const char * strToFind)
	{
		// on fait une copie locale sans accents de la chaine de filtrage
		std::string s(strToFind, strlen(strToFind));
		PrepareStr(s);	
		_strToFind = new char[s.length() + 1];
		strncpy(_strToFind, s.c_str(), s.length());
		_strToFind[s.length()] = '\0';
	}

	~IsTargetLabelContains()
	{
		// on efface la chaine de filtrage
		if (_strToFind) delete[] _strToFind; _strToFind = 0;
	}

	virtual bool operator()(const IItem * pItem) const
	{
		// supprime les accents du libellé du support avant d'y chercher la chaine de filtrage
		static std::string _sCur;
		static const char * pcszLabel;
		pcszLabel = pItem->GetLabel().AsJCharPtr();
		_sCur = PrepareStr(std::string (pcszLabel, strlen(pcszLabel)));
		return (_sCur.find(_strToFind) != std::string::npos);
	}

	protected:

		char *			_strToFind;
		TargetLabelSort	_titleLabelSort;
};

//=============================================================================	
// Permet de composer des filtres de cibles, basés sur des functeurs de type ITargetFilterFunc
struct TargetFilterFuncComposer : public std::unary_function<const IItem *, bool>
//=============================================================================	
{
	bool operator()(const IItem * pItem) const
	{
		// pas de filtrage, le support passe au travers du filtre
		if (!_functors.empty())
		{
			// on boucle sur les filtres ajoutés
			_func_it = _functors.begin();
			while (_func_it != _functors.end())
			{
				if (_func_it->second == AND)
				{
					// operator AND
					if (!(*_func_it->first)(pItem))
						return false;
				}
				else
				{
					// operator NAND
					if ((*_func_it->first)(pItem))
						return false;
				}
				++_func_it;
			}
		}
		return true;
	}

	typedef enum Operator
	{
		AND,	//		le résultat d'un functeur ajouté avec 'AND'
				//	DOIT être true pour que le filtre composé soit true
		NAND	//		le résultat d'un functeur ajouté avec 'NAND'
				//	DOIT être false pour que le filtre composé soit true
	};

	// ajoute un functeur
	void AddFilter (ITargetFilterFunc * functor, Operator op = AND)
	{
		_functors.insert(std::make_pair(functor, op));	
	}

	// libère les filtres contenus (et appelle leur destructeur repspectif)
	void ReleaseFilters()
	{
		FunctorIterator it = _functors.begin();
		while(it != _functors.end())
		{
			ITargetFilterFunc * pfo = it->first;
			if (pfo) delete pfo; pfo = 0;
			++it;
		}
		_functors.clear();
	}

protected:

	typedef std::map<ITargetFilterFunc*, Operator>	FunctorMap;
	typedef FunctorMap::const_iterator				ConstFunctorIterator;
	typedef FunctorMap::iterator					FunctorIterator;
	FunctorMap										_functors;
	mutable ConstFunctorIterator					_func_it;
};

//=============================================================================	
// functeur pour tester si une cible 'est valide' sur une source donnée
struct IsTargetOnSource
	:	public ITargetFilterFunc
{
	IsTargetOnSource(const JUnt32 idsource) : _idsource(idsource) {}

	virtual bool operator()(const IItem * pItem) const
	{
		return pItem->AsItemConstruit()->IsSouche(_idsource);
	}

	const JUnt32 _idsource;
};
//=============================================================================	

//=============================================================================	
// functeur pour ajouter une cible à une targetList (et sélectionner celles qui doivent l'être)
// retourne le nouveau top index

struct AddToTargetList
	: public std::unary_function<const IItem*, void>
{
	AddToTargetList(CTargetList & targetList, HXTemplateHandle * hxTemplates, std::multiset<const IItem*> & selection,
					const ItemRestrictMap & RestrictionsMap, const COLORREF refAffColor)
		:	_targetList(targetList),	_selection(selection),		_toplevelIdx(LB_ERR),
			_hxTemplates(hxTemplates), _refAffColor(refAffColor),	_RestrictionsMap(RestrictionsMap)
	{}

	void operator () (const IItem * pItem)
	{
		// on définit le template de l'element à inserer
		_Restr_it = _RestrictionsMap.find(pItem->GetID());
		ASSERT(_Restr_it != _RestrictionsMap.end());

		_hxCurrent = _hxTemplates[_Restr_it->second];
		_sLabel = pItem->GetLabel().AsJCharPtr();

		// est-ce la réference affinité?
		const bool bIsRefAff = (pItem == _targetList.GetReferenceAffiniteItem());

		// si oui : on ajoute la chaine " (Ref. Affinité)" au texte de l'item
		if (bIsRefAff) _sLabel.AppendFormat(" %s", pcszRefAffinite);

		// on ajoute l'item et sa data
		_idx = _targetList.AddLine(_sLabel, _hxCurrent);
		_targetList.SetItemData(_idx, reinterpret_cast<DWORD_PTR>(pItem));		

		// si ref. affinité : on modifie la couleur du texte de l'item...
		if (bIsRefAff)
		{
			// pour les états 'sélectionné' et 'non sélectionné'
			_targetList.SetTextColor(_idx, _refAffColor, true);
			_targetList.SetTextColor(_idx, _refAffColor, false);
		}

		// on cherche si cet element doit être sélectionné dans la listbox
		if (!_selection.empty())
		{
			_sel_it = std::find(_selection.begin(), _selection.end(), pItem);
			if (_sel_it != _selection.end())
			{
				// on doit le selectionner, puis l'effacer car on ne peut pas sélectionner 2 fois!
				_targetList.SetSel(_idx);

				// on définit le 1er elt sélectionné comme étant le nouveau top index
				if (_toplevelIdx == LB_ERR)
					_toplevelIdx = _idx;
				_selection.erase(_sel_it);
			}
		}
	}

	int							_toplevelIdx;

private:
	const ItemRestrictMap &		_RestrictionsMap;
	ItemRestrictIter			_Restr_it;
	HXTemplateHandle *			_hxTemplates;
	HXTemplateHandle			_hxCurrent;
	CTargetList &				_targetList;
	COLORREF					_refAffColor;
	std::multiset<const IItem*> & _selection;
	std::multiset<const IItem*>::iterator _sel_it;
	//IItemList &					_selection;
	//IItemIter					_sel_it;
	CString						_sLabel;
	int							_idx;
};

#pragma endregion CTargetList functors

//=============================================================================	
//=============================================================================	
//						Boîte de dialogue CTargetList
//=============================================================================	
//=============================================================================	
#pragma region Boîte de dialogue CTargetList

IMPLEMENT_DYNAMIC(CTargetList, CJFCXListBox)

//=============================================================================	
//								message map
//=============================================================================	
BEGIN_MESSAGE_MAP(CTargetList, CJFCXListBox)
	ON_WM_RBUTTONDOWN()
	ON_NOTIFY_EX_RANGE		(TTN_NEEDTEXTW, 0, 0xFFFF,				  OnToolTipText)
	ON_NOTIFY_EX_RANGE		(TTN_NEEDTEXTA, 0, 0xFFFF,				  OnToolTipText)
	ON_COMMAND				(IDM_FD_CIBLES_REFAFFINITE,	&CTargetList::OnMenuReferenceAffinite)
END_MESSAGE_MAP()

//=============================================================================	
CTargetList::CTargetList(COffre2MainDialog * pDialogOffre)
	:	m_pDialogOffre(pDialogOffre)
//=============================================================================	
{
	m_pchTip			= 0;
	m_pwchTip			= 0;
	m_pRefAffinite		= 0;
	m_appColor			= -1;
	m_pImpl				= new Pimpl(*this);
	m_eDisplay			= COffre2Cibles::effItems;
}

//=============================================================================	
CTargetList::~CTargetList()
//=============================================================================	
{
	if (m_pImpl) delete m_pImpl;
	m_pImpl = 0;
}

//=============================================================================	
void CTargetList::SetAppColor(int appColor)
//=============================================================================	
{
	// templates des cibles/palettes sans interêt particulier
	CXTemplate SystemTemplate = CXTemplateManager::GetInstance()->GetTemplate(
		CXTemplateManager::GetInstance()->GetSystemTemplate());

	// element normal (avec selection couleur du module Atelier Presse)
	m_pImpl->m_hxTemplates[eNormalItem] = CXTemplateManager::GetInstance()->
	CreateTemplate(	SystemTemplate.textcolor_, SystemTemplate.backcolor_,
		SystemTemplate.seltextcolor_, CATPColors::GetColorSelect(static_cast<CATPColors::APPCOLOR>(appColor)));

	m_pImpl->m_RefAffTextColor = RGB(255, 157, 157);

	// seuil avertissement
	m_pImpl->m_hxTemplates[eWarningSeuil] = CXTemplateManager::GetInstance()->
		CreateTemplate(	SystemTemplate.textcolor_, RGB(255, 189, 5),
		RGB(255, 189, 5), CATPColors::GetColorSelect(static_cast<CATPColors::APPCOLOR>(appColor)));

	// seuil rédhibitoire (impossible à être sélectionné)
	m_pImpl->m_hxTemplates[eForbiddenItem] = CXTemplateManager::GetInstance()->
		CreateTemplate(	SystemTemplate.textcolor_, RGB(255, 174, 174),
		RGB(255, 2, 2), CATPColors::GetColorSelect(static_cast<CATPColors::APPCOLOR>(appColor)));
}

//=============================================================================	
void CTargetList::DoDataExchange(CDataExchange* pDX)
//=============================================================================	
{
	CJFCXListBox::DoDataExchange(pDX);
}

//=============================================================================	
void CTargetList::InvalidateSeuils()
//=============================================================================	
{
	m_pImpl->m_bReloadCacheSeuils = true;
}

//=============================================================================	
void CTargetList::SetCurrentDisplay(const int eDisplay)
//=============================================================================	
{
	m_eDisplay = eDisplay;
}
#pragma endregion Boîte de dialogue CTargetList

//=============================================================================	
//						 CTargetList Gestion des elements
//=============================================================================	
#pragma region CTargetList Gestion des elements

//=============================================================================	
// ajout d'un item à la listbox
void CTargetList::AddItem(const IItem * pItem)
//=============================================================================	
{
	// on ajoute l'element
	m_Targets.insert(pItem);
}

//=============================================================================	
// suppression de tous les items
void CTargetList::ResetItems()
//=============================================================================	
{
	m_Targets.clear();
}

//=============================================================================	
void CTargetList::Lock()
//=============================================================================	
{
	LockWindowUpdate();
}

//=============================================================================	
void CTargetList::UpdateAndUnlock(const CTargetFilter & filter, IItemList_t & SelectedItems)
//=============================================================================	
{
	std::list<const IItem *> DisplayList;

	// on recopie la sélection
	m_SelectionCopy.clear();
	std::copy(SelectedItems.begin(), SelectedItems.end(),
		std::insert_iterator<std::multiset<const IItem*> > (m_SelectionCopy, m_SelectionCopy.begin()));

	int idx = LB_ERR, nTopIdx = LB_ERR;

	// on crée notre functor-filtre composé
	TargetFilterFuncComposer _functor;

	// on ajoute si besoin un filtre de libellé
	if (!filter.strSearch.IsEmpty())
		_functor.AddFilter(new IsTargetLabelContains(filter.strSearch));

	copy_if(m_Targets.begin(), m_Targets.end(),
		std::back_inserter(DisplayList), _functor);

	//DisplayList.sort(AlphabeticalCompare());
	DisplayList.sort(TargetLabelSort());

	// on calcule les seuils et les restriction de sélection d'items
	CalculateSeuilsElements calculateSeuilsElts(m_eDisplay, *m_pImpl);
	for_each(DisplayList.begin(), DisplayList.end(), calculateSeuilsElts);

	if (!DisplayList.empty())
	{
		// notre functor d'ajout de ligne dans la listbox
		AddToTargetList addLineFn(*this, m_pImpl->m_hxTemplates, m_SelectionCopy, m_pImpl->m_Restrictions, m_pImpl->m_RefAffTextColor);

		// on affiche tous les supports (et on récupère le top level idx et l'idx de la reference affinité)
		const int toplevelIdx = for_each(DisplayList.begin(), DisplayList.end(), addLineFn)._toplevelIdx;

		// on libère le filtre composé
		_functor.ReleaseFilters();

		if (GetCount() > 0)
			// on définit le top index
			SetTopIndex((toplevelIdx == LB_ERR)? 0 : toplevelIdx);
	}

	// on met à jour la listbox
	UnlockWindowUpdate();
}

#pragma endregion CTargetList Gestion des elements

//=============================================================================	
//						 CTargetList Reference Affinite
//=============================================================================	
#pragma region CTargetList Reference Affinite

//=============================================================================	
void CTargetList::OnRButtonDown(UINT nFlags, CPoint point)
//=============================================================================	
{
	// on s'assure que l'on a au moins un element
	const int nMax = GetCount();
	if (nMax > 0)
	{
		BOOL bOutside;
		// on s'assure que le clic-droit a bien lieu sur un element de la listbox
		m_RClickedItem = ItemFromPoint(point, bOutside);
		if (!bOutside && m_RClickedItem < nMax)
		{
			const IItem * pRClickedItem = reinterpret_cast<const IItem*>(GetItemData(m_RClickedItem));

			// on s'assure que l'item n'est pas restreint totalement
			if (m_pImpl->m_Restrictions[pRClickedItem->GetID()] != eForbiddenItem)
			{
				// on transforme les coordonnées client du point en coordonnées écran
				ClientToScreen(&point);

				// on récupère le menu
				CMenu menu; menu.LoadMenu(IDR_FD_CONTEXTMENU_CIBLES);

				// on appelle la fonction qui va modifier le menu
				CMenu* pPopupMenu = menu.GetSubMenu(0);

				// on affiche le menu
				if (pPopupMenu != 0)
					pPopupMenu->TrackPopupMenu(TPM_LEFTALIGN | TPM_TOPALIGN | TPM_RIGHTBUTTON, point.x, point.y, this);
			}
		}
	}
}

//=============================================================================	
int CTargetList::FindReferenceAffiniteIndex()
//=============================================================================	
{
	ASSERT(m_pRefAffinite != 0);

	int count = 0;
	const int max = GetCount();
	while(count < max)
		if (m_pRefAffinite == reinterpret_cast<const IItem*>(GetItemData(count++)))		
			return count - 1;

	// la reference affinité n'est pas affiché actuellement
	return LB_ERR;
}

//=============================================================================	
void CTargetList::OnMenuReferenceAffinite()
//=============================================================================	
{
	// on s'assure que l'on a au moins un element
	const int nMax = GetCount();
	if (nMax > 0)
	{
		if (m_RClickedItem < nMax)
		{
			if (const IItem * pNewRefAff = reinterpret_cast<const IItem*>(GetItemData(m_RClickedItem)))
			{
				LockWindowUpdate();

				// on sauve l'etat de selection de la nouvelle reference affinité
				BOOL newRefisSelected = GetSel(m_RClickedItem);

				// on check que l'item est bien sélectionnable (pas rédhibitoire)
				ASSERT (m_pImpl->m_Restrictions[pNewRefAff->GetID()] != eForbiddenItem);

				// on sauve le top index
				const int curTopIndex = GetTopIndex();

				HXTemplateHandle hxTemplate;

				// traitement de l'ancienne reference affinite
				///////////////////////////////////////////////

				const int curRefAffiniteIdx = FindReferenceAffiniteIndex();
				if (/*m_pRefAffinite != 0 &&*/ curRefAffiniteIdx != LB_ERR)
				{
					// on sauve l'etat, le template et la donnée stockée dans l'ancienne reference affinité
					DWORD_PTR oldRefData = GetItemData(curRefAffiniteIdx);
					BOOL oldRefisSelected = GetSel(curRefAffiniteIdx);
					eRestrictionLevel oldRefRestriction = m_pImpl->m_Restrictions[m_pRefAffinite->GetID()];

					// on supprime l'ancienne reference affinité
					DeleteString(curRefAffiniteIdx);

					// on ajoute l'item supprimé, mais sans le texte "Ref. Affinité"
					hxTemplate = m_pImpl->m_hxTemplates[oldRefRestriction];
					const int oldRefafIdx = InsertString(curRefAffiniteIdx, m_pRefAffinite->GetLabel().AsJCharPtr(), hxTemplate);

					// on restaure l'etat et la data
					SetItemData(oldRefafIdx, oldRefData);
					SetSel(oldRefafIdx, oldRefisSelected);
				}

				// traitement de la nouvelle reference affinite
				///////////////////////////////////////////////

				// on sauve l'etat et la donnée stockée dans la nouvelle reference affinité
				DWORD_PTR newRefData = GetItemData(m_RClickedItem);
				DeleteString(m_RClickedItem);
				eRestrictionLevel newRefRestriction = m_pImpl->m_Restrictions[pNewRefAff->GetID()];

				// on ajoute la nouvelle chaine 'ref affinité'
				CString sRefAffLine;
				sRefAffLine.Format("%s %s", pNewRefAff->GetLabel().AsJCharPtr(), pcszRefAffinite);
				const int newRefAffiniteIdx = InsertString(m_RClickedItem, sRefAffLine, m_pImpl->m_hxTemplates[newRefRestriction]);
				SetTextColor(newRefAffiniteIdx, m_pImpl->m_RefAffTextColor, true);
				SetTextColor(newRefAffiniteIdx, m_pImpl->m_RefAffTextColor, false);

				// on restaure l'etat et la data de la nouvelle reference affinité
				SetItemData(newRefAffiniteIdx, newRefData);
				SetSel(newRefAffiniteIdx, newRefisSelected);

				// on définit la couleur du texte de la texte de la réference affinité
				SetTextColor(newRefAffiniteIdx, m_pImpl->m_RefAffTextColor, true);
				SetTextColor(newRefAffiniteIdx, m_pImpl->m_RefAffTextColor, false);

				// on retient le ptr vers la nouvelle reference affinité
				m_pRefAffinite = pNewRefAff;

				// on replace le top index
				SetTopIndex(curTopIndex);

				UnlockWindowUpdate();
			}
		}
	}
}

//=============================================================================	
const JUnt32 CTargetList::GetReferenceAffiniteId() const		
//=============================================================================	
{
	return m_pRefAffinite->GetID();
}

//=============================================================================	
void CTargetList::SetReferenceAffiniteItem(const IItem * pItem)
//=============================================================================	
{
	m_pRefAffinite = pItem;
}

//=============================================================================	
const IItem * CTargetList::GetReferenceAffiniteItem() const
//=============================================================================	
{
	return m_pRefAffinite;
}

#pragma endregion CTargetList Reference Affinite

//=============================================================================	
//						 CTargetList ToolTip management
//=============================================================================	
#pragma region CTargetList ToolTip management

//=============================================================================	
void CTargetList::PreSubclassWindow()
//=============================================================================	
{
	EnableToolTips(TRUE);
	CJFCXListBox::PreSubclassWindow();
}

//=============================================================================	
int CTargetList::OnToolHitTest(CPoint point, TOOLINFO * pTI) const
//=============================================================================	
{
	int row;
	RECT cellrect;
	BOOL tmp = FALSE;
	// on détermine la ligne correspondant à la position actuelle du curseur
	row = ItemFromPoint(point,tmp);
	if (row == -1)
		return -1;

	// on remplit la structure TOOLINFO
	GetItemRect(row,&cellrect);
	pTI->rect = cellrect;
	pTI->hwnd = m_hWnd;
	pTI->uId = static_cast<UINT>(row);		// on assigne le numérode ligne au membre uId
	pTI->lpszText = LPSTR_TEXTCALLBACK; 
	return static_cast<int>(pTI->uId);
}

//=============================================================================	
void CTargetList::Pimpl::AddToolTipTextForItem(const JInt32 ItemId, CString & sTooltipText)
//=============================================================================	
{
	if (m_pSource == 0) 
	{
		// on charge les elements de calcul des seuils
		m_BackRef.ReloadCacheSeuilsElements();
	}

	ASSERT(m_pSource);
	if (m_pSource)
	{
		// récupère les items construits
		m_pItemsConstruits = CItemsConstruitsManager::GetInstance()->GetItemsInstance();

		// on cherche si cet id d'item existe deja dans la map
		ItemParamsIter iparams_it = m_ItemsParams.find(ItemId);
		if (iparams_it != m_ItemsParams.end())
		{
			std::vector<TooltipContentLine> tipContent;
			std::vector<int> terrainNameLengths;

			// Boucle sur les terrains
			std::list<const JTerrain*>::const_iterator ter_it = m_ListTerrains.begin();
			while(ter_it != m_ListTerrains.end())
			{
				// Récup l'info terrain
				JUnt32 IdTerrain = (*ter_it)->m_IdTerrain;

				// On se positionne sur les infos items pour ce terrain
				TerrainParamsIter iterrain_it = iparams_it->second->find(IdTerrain);
				if (iterrain_it != iparams_it->second->end())
				{
					TooltipContentLine tooltipLine;	
					// on remplit la ligne de tooltip
					tooltipLine.nbCas = iterrain_it->second->GetNbCas();
					tooltipLine.terrainName.Format(" - %s %s", m_pSource->m_Libelle.AsJCharPtr(), (*ter_it)->m_Libelle.AsJCharPtr());

					// on l'ajoute au vecteur
					tipContent.push_back(tooltipLine);
					terrainNameLengths.push_back(tooltipLine.terrainName.GetLength());
				}
				++ter_it;
			}

			// on crée le tooltip pour cet item
			if (!terrainNameLengths.empty())
			{
				const int maxTerrainNameLen = 2 + *std::max_element(terrainNameLengths.begin(), terrainNameLengths.end());

				// on boucle sur toutes les lignes du tooltip
				std::vector<TooltipContentLine>::const_iterator tip_it = tipContent.begin();
				while(tip_it != tipContent.end())
				{
					// on  crée la chaine d'espaces
					std::string spaces(maxTerrainNameLen - tip_it->terrainName.GetLength(), ' ');

					// Ajoute au tooltip texte
					sTooltipText.AppendFormat("%s%s(nb. cas :%d)\n", tip_it->terrainName, spaces.c_str(),tip_it->nbCas);
					++tip_it;
				}
			}
		}
		sTooltipText.AppendChar('\0');
	}
}


//=============================================================================	
//Define OnToolTipText(). This is the handler for the TTN_NEEDTEXT notification from 
//support ansi and unicode 
BOOL CTargetList::OnToolTipText(UINT id, NMHDR * pNMHDR, LRESULT * pResult)
//=============================================================================	
{
	// need to handle both ANSI and UNICODE versions of the message
	TOOLTIPTEXTA* pTTTA = (TOOLTIPTEXTA*)pNMHDR;
	TOOLTIPTEXTW* pTTTW = (TOOLTIPTEXTW*)pNMHDR;

	UINT nID = (UINT)pNMHDR->idFrom;

	::SendMessage(pNMHDR->hwndFrom, TTM_SETMAXTIPWIDTH, 0, 300);

	// Init texte tooltip
	CString strTipText;

	// item courant
	const IItem * pItem = reinterpret_cast<const IItem*>(GetItemData(nID));
	ASSERT(pItem);

	// affichage d'items
	if(m_eDisplay == COffre2Cibles::effItems)
	{
		m_pImpl->AddToolTipTextForItem(pItem->GetID(), strTipText);
	}
	// affichage de palettes
	else if(m_eDisplay == COffre2Cibles::effPalettes)
	{
		// Est-ce une palette
		const CPalettes * palette = pItem->AsPalette();
		ASSERT(palette);

		std::list<const CItemConstruit *> itemList;
		std::list<const CItemConstruit *>::const_iterator item_it;
		palette->GetItems(itemList);

		std::vector<TooltipContentLine> tipContent;
		std::vector<int> terrainNameLengths;

		// on boucle sur tous les items de la palette
		item_it = itemList.begin();
		while(item_it != itemList.end())
		{
			// saute des lignes entre chq item du tooltip
			if (!strTipText.IsEmpty())
				strTipText.AppendChar('\n');

			CString sTipLine;
			m_pImpl->AddToolTipTextForItem((*item_it)->GetID(), sTipLine);
			strTipText.AppendFormat("%s\n%s", (*item_it)->GetLabel().AsJCharPtr(), sTipLine);

			++item_it;
		}

		// on forme le tooltip complet en multiline
		if (itemList.empty())
		{
			// palette vide
			strTipText.Format("palette %s : vide", palette->GetLabel().AsJCharPtr());
		}
		else
		{
			// on 'normalize le tooltip'
			strTipText.Trim(); strTipText.AppendChar('\0');
		}
	}

#ifndef _UNICODE
	if(pNMHDR->code == TTN_NEEDTEXTA)
	{
		if(m_pchTip)
			delete m_pchTip;

		m_pchTip = new TCHAR[strTipText.GetLength()];
		lstrcpyn(m_pchTip, strTipText, strTipText.GetLength());
		pTTTW->lpszText = (WCHAR*)m_pchTip;
	}
	if(pNMHDR->code == TTN_NEEDTEXTW)
	{
		if(m_pwchTip)
			delete m_pwchTip;

		m_pwchTip = new WCHAR[strTipText.GetLength()];
		_mbstowcsz(m_pwchTip, strTipText, strTipText.GetLength());
		pTTTW->lpszText = (WCHAR*)m_pwchTip;
	}
#else
	if(pNMHDR->code == TTN_NEEDTEXTA)
	{
		if(m_pchTip)
			delete m_pchTip;

		m_pchTip = new TCHAR[strTipText.GetLength()];
		_wcstombsz(m_pchTip, strTipText, strTipText.GetLength());
		pTTTA->lpszText = (LPTSTR)m_pchTip;
	}
	if(pNMHDR->code == TTN_NEEDTEXTW)
	{
		if(m_pwchTip)
			delete m_pwchTip;

		m_pwchTip = new WCHAR[strTipText.GetLength()];
		lstrcpyn(m_pwchTip, strTipText, strTipText.GetLength());
		pTTTA->lpszText = (LPTSTR) m_pwchTip;
	}
#endif

	*pResult = 0;

	return TRUE; 
}

#pragma endregion CTargetList ToolTip management

//=============================================================================	
//						 CTargetList::Pimpl : Gestion des Seuils
//=============================================================================	
#pragma region CTargetList::Pimpl Gestion des Seuils

//=============================================================================	
void CTargetList::ReloadCacheSeuilsElements()
//=============================================================================	
{
	// on ne refait le cache que si on a besoin de le faire
	if (m_pImpl->m_bReloadCacheSeuils)
	{
		// récupère la sélection actuelle
		const CSupportsSelection & Selection = m_pDialogOffre->GetSupportsSelection();

		// on récupère la source courante
		m_pImpl->m_pSource = m_pImpl->m_pSrcTables->m_TBLSources.GetItem(Selection.IdSource);

		// on recrée la liste des terrains sélectionnés
		m_pImpl->m_ListTerrains.clear();
		std::list<JUnt32>::const_iterator terid_it = Selection.SelectedTerrains.begin();
		while (terid_it != Selection.SelectedTerrains.end())
		{
			m_pImpl->m_ListTerrains.push_back(m_pImpl->m_pSrcTables->m_TBLTerrains.GetItem(Selection.IdSource, *terid_it));
			++terid_it;
		}

		// on supprime les seuils précedent
		m_pImpl->ResetMapSeuils();

		m_pImpl->m_bReloadCacheSeuils = false;
	}
}

//=============================================================================	
void CTargetList::Pimpl::ResetMapSeuils()
//=============================================================================	
{
	CCalcItemParams* pCalcItemParams = 0;
	TerrainParamsMap * pTerrainsParamsMap = 0;

	///////////////////////////////////////////
	// boucle sur ts les elts de la map d'items

	ItemParamsIter itparam_it = m_ItemsParams.begin();
	while(itparam_it != m_ItemsParams.end())
	{
		// boucle sur ts les elts de la map interne
		TerrainParamsIter itterrain_it = itparam_it->second->begin();
		while(itterrain_it != itparam_it->second->end())
		{
			// on garde le ptr
			pCalcItemParams = itterrain_it->second;
			++itterrain_it;

			if (pCalcItemParams)
				delete pCalcItemParams;
			pCalcItemParams = 0;
		}
		// on garde le ptr
		itparam_it->second->clear();
		++itparam_it;
	}

	// boucle sur ts les elts de la map d'items
	// obligé de detruire en 2 fois les elts de la map (sinon VS8.0SP1 plante en DEBUG)
	itparam_it = m_ItemsParams.begin();
	while(itparam_it != m_ItemsParams.end())
	{
		pTerrainsParamsMap = itparam_it->second;
		if (pTerrainsParamsMap)
			delete pTerrainsParamsMap;
		pTerrainsParamsMap = 0;
		++itparam_it;
	}

	///////////////////////////////////////////
	// boucle sur ts les elts de la map de liste de terrains

	ItemTerrainsIter idterforbid_it = m_ForbiddenTerrains.begin();
	TerrainSet * pTerrainSet = 0;
	while(idterforbid_it != m_ForbiddenTerrains.end())
	{
		// detruit le set
		if (pTerrainSet = idterforbid_it->second)
		{
			delete pTerrainSet;
			pTerrainSet = 0;
		}		

		++idterforbid_it;
	}

	///////////////////////////////////////////
	// boucle sur ts les elts de la map de liste de terrains

	ItemTerrainsIter idterwarn_it = m_WarningTerrains.begin();
	while(idterwarn_it != m_WarningTerrains.end())
	{
		// detruit le set
		if (pTerrainSet = idterwarn_it->second)
		{
			delete pTerrainSet;
			pTerrainSet = 0;
		}		

		++idterwarn_it;
	}

	// on peut vider toutes les map
	m_ItemsParams.clear();
	m_Restrictions.clear();
	m_ForbiddenTerrains.clear();
	m_WarningTerrains.clear();
}

//=============================================================================	
void CTargetList::Pimpl::AddItemParam(const CItemConstruit * pItem)
//=============================================================================	
{
	// récupère la sélection actuelle de l'offre
	const CSupportsSelection & Selection = m_BackRef.m_pDialogOffre->GetSupportsSelection();

	// on vérifie que l'id de l'item n'existe pas deja
	ItemParamsIter iparams_it = m_ItemsParams.find(pItem->GetID());
	if (iparams_it == m_ItemsParams.end())
	{
		// l'elt n'existe pas, on l'ajoute
		m_ItemsParams.insert(std::make_pair(pItem->GetID(), new TerrainParamsMap));
		iparams_it = m_ItemsParams.find(pItem->GetID());

		JInt32 IdTerrain;

		// Boucle sur les terrains
		TerrainListIter ter_it = m_ListTerrains.begin();
		while(ter_it != m_ListTerrains.end())
		{
			IdTerrain = (*ter_it)->m_IdTerrain;
			CTerrain * pTerrain = COffreTerrainManager::GetInstance()->GetTerrain(m_pSource->m_IdSource, IdTerrain);
			if (pTerrain)
			{
				// Créer nouvelle instance paramètrages pour calcul effectif cible 
				CCalcItemParams * Params = new CCalcItemParams();

				// Init des paramètrages
				Params->SetTerrain(pTerrain);
				Params->SetEchantillonID(1);
				Params->SetUniversID(pTerrain->GetDefaultUnivers());
				Params->SetTypePoidsID( Selection.IdTypePoids);

				// Tenir compte de la population segment et positionner le paramètre
				CBitArray bArrPopSeg;
				JUnt32 contingent =  0xFFFF;
				pTerrain->GetDefaultPopSegment(bArrPopSeg, contingent, true);    // en tenant compte des multi-segments
				Params->SetPopulationSegment(bArrPopSeg);

				// Set param segments
				Params->SetSegmentsIDs(Selection.SelectedSegments);

				// Calcul de l'effectif pour cet item
				pItem->CalculateEffectif(Params);
				
				// On se positionne sur les infos params du terrain
				(*iparams_it->second)[IdTerrain] = Params;
			}
			++ter_it;
		}
	}
}

//=============================================================================	
eRestrictionLevel CTargetList::Pimpl::GetRestrictionLevel(const IItem * pItem)
//=============================================================================	
{
	eRestrictionLevel restriction = eNormalItem;

	// la restriction a-t-elle deja été calculée
	if (m_Restrictions.find(pItem->GetID()) != m_Restrictions.end())
		restriction = m_Restrictions[pItem->GetID()];
	else
	{
		// non... on le fait!
		if (pItem->IsPalette())
			restriction = GetPaletteRestrictionLevel(pItem->AsPalette());
		else
			restriction = GetItemRestrictionLevel(pItem->AsItemConstruit());

		// on l'insère pour la prochaine fois...
		m_Restrictions[pItem->GetID()] = restriction;
	}
	
	return restriction;
}

//=============================================================================	
eRestrictionLevel  CTargetList::Pimpl::GetPaletteRestrictionLevel(const CPalettes * pPalettes)
//=============================================================================	
{
	// Récupère la palette
	if(pPalettes)
	{
		// Init le nombre d'interdictions partielles trouvées et le nombre d'interdictions totales
		size_t numForbidden	= 0;
		size_t numWarning	= 0;

		// on compte le nombre d'interdictions des elements composant cette palette
		std::list<const CItemConstruit*> itemList;
		pPalettes->GetItems(itemList);
		std::list<const CItemConstruit*>::const_iterator item_it = itemList.begin();
		while (item_it != itemList.end())
		{
			// on calcule la restriction des elements fils
			eRestrictionLevel restriction = GetItemRestrictionLevel(*item_it);
			if (restriction == eWarningSeuil)
				// item avec interdiction partielle
				numWarning++;
			else if (restriction == eForbiddenItem)
				// item avec interdiction totale
				numForbidden++; 
			++item_it;
		}

		// on détermine la restriction de cette palette
		if (numForbidden == itemList.size())
			// tous les items ont une restriction totale --> palette interdite
			return eForbiddenItem;
		else if (numWarning > 0)
			// au moins 1 item est en avertissement --> palette en avertissement
			return eWarningSeuil;
	}

	// renvoie le niveau de restriction de la palette
	return eNormalItem;
}

//=============================================================================	
eRestrictionLevel  CTargetList::Pimpl::GetItemRestrictionLevel(const CItemConstruit * pItem)
//=============================================================================	
{
	const JInt32 IdItem = pItem->GetID();

	// par defaut : pas de restriction
	eRestrictionLevel restriction = eNormalItem;

	// on cherche l'item dans la map de parametres
	ItemParamsIter iparams_it = m_ItemsParams.find(IdItem);

	// on cherche l'id dans la map des terrains interdits
	ItemTerrainsIter idterforbid_it = m_ForbiddenTerrains.find(IdItem);

	// on cherche l'id dans la map des terrains interdits
	ItemTerrainsIter idterwarn_it = m_WarningTerrains.find(IdItem);

	// si le nombre de cas de l'item n'a pas encore été calculé, on le fait maintenant
	if (iparams_it == m_ItemsParams.end())
	{
		AddItemParam(pItem);

		// on s'assure que l'elt existe
		iparams_it = m_ItemsParams.find(IdItem);
		ASSERT(iparams_it != m_ItemsParams.end());
	}

	// on ajoute les liste d'enregistrement des terrains par restriction si elles n'existent pas
	if (idterforbid_it == m_ForbiddenTerrains.end())
	{
		m_ForbiddenTerrains.insert(std::make_pair(IdItem, new TerrainSet));		
		idterforbid_it = m_ForbiddenTerrains.find(IdItem);
		ASSERT(idterforbid_it != m_ForbiddenTerrains.end());
	}
	if (idterwarn_it == m_WarningTerrains.end())
	{
		m_WarningTerrains.insert(std::make_pair(IdItem, new TerrainSet));		
		idterwarn_it = m_WarningTerrains.find(IdItem);
		ASSERT(idterwarn_it != m_WarningTerrains.end());
	}

	if (iparams_it != m_ItemsParams.end() && idterforbid_it != m_ForbiddenTerrains.end()
		&& idterwarn_it != m_WarningTerrains.end())
	{
		// on récupère le set de terrains interdits
		TerrainSet * pForbiddenTerrainsSet = idterforbid_it->second;
 		pForbiddenTerrainsSet->clear();

		// on récupère le set de terrains en warning
		TerrainSet * pWarningTerrainsSet = idterwarn_it->second;
		pWarningTerrainsSet->clear();

		//bool bHasAtLeastOneWarningSeuil = false;

		// Boucle sur les terrains
		std::list<const JTerrain *>::const_iterator jter_it = m_ListTerrains.begin();
		while(jter_it != m_ListTerrains.end())
		{
			// On se positionne sur les infos items pour ce terrain
			TerrainParamsIter terparam_it = iparams_it->second->find((*jter_it)->m_IdTerrain); 
			if (terparam_it != iparams_it->second->end())
			{
				// Test nombre de cas item via les seuils du terrain
				const JInt32 NbCasItem = terparam_it->second->GetNbCas();
				if (NbCasItem < (*jter_it)->m_SeuilRedhibitoire)
				{
					// on ajoute ce terrain à l'ensemble (set) des terrains interdits pour cet item
					pForbiddenTerrainsSet->insert(*jter_it);
				}
				else if (NbCasItem < (*jter_it)->m_SeuilAvertissement)
				{
					// on ajoute ce terrain à l'ensemble (set) des terrains interdits pour cet item
					pWarningTerrainsSet->insert(*jter_it);
				}
			}
			++jter_it;
		}

		// si on a au moins 1 terrain avec un seuil d'avertissement
		if (!pWarningTerrainsSet->empty())
		{
			// l'item est en warning
			restriction = eWarningSeuil;
		}
		// si on a au moins 1 terrain avec un seuil rédhibitoire
		else if(!pForbiddenTerrainsSet->empty())
		{
			// tous les terrains ont des seuils rédhibitoires
			if (pForbiddenTerrainsSet->size() == m_ListTerrains.size())
				// tous les items ont un seuli redhibitoire
				restriction = eForbiddenItem;			
		}
	}
	else
	{
		// ce n'est pas normal, car si l'item n'existait pas dans la map, on l'y aurait ajouté!!
		ASSERT(FALSE);
	}

	// renvoie le niveau de restriction de l'item
	m_Restrictions[IdItem] = restriction;
	return restriction;
}

//=============================================================================	
struct DeselectIndex
	:	public std::unary_function<JInt32, void>
//=============================================================================	
{
	DeselectIndex(CTargetList & listbox) : _listbox(listbox) {}

	void operator() (const JInt32 idx)
	{
		_listbox.SetSel(idx, FALSE);
	}
	CTargetList & _listbox;
};

//=============================================================================	
void CTargetList::DeselectItemsWithBadSeuils()
//=============================================================================	
{
	std::set<int> selection;
	if (GetControlSelection(selection, *this))
	{
		// on passe dans chaque
		for_each_if(selection.begin(), selection.end(), 
			IfRestrictionIs(*this, eForbiddenItem, m_pImpl->m_Restrictions), DeselectIndex(*this));
	}
}

//=============================================================================	
void CTargetList::DisplaySingleItemMessage(const IItem * pItem, bool & bUnselect)
//=============================================================================	
{
	ItemRestrictIter iter = m_pImpl->m_Restrictions.find(pItem->GetID());
	ASSERT(iter != m_pImpl->m_Restrictions.end());

	bool bIsPalette = pItem->IsPalette();

	if (iter->second == eForbiddenItem)
	{
		bUnselect = true;

		// restriction complète
		if (bIsPalette)
			m_pImpl->DisplayForbidenMessage(pItem->AsPalette());
		else
			m_pImpl->DisplayForbidenMessage(pItem->AsItemConstruit());
	}
	else if (iter->second == eWarningSeuil)
	{
		// restriction partielle
		if (bIsPalette)
			m_pImpl->DisplayWarningMessage(pItem->AsPalette(), bUnselect);
		else
			m_pImpl->DisplayWarningMessage(pItem->AsItemConstruit(), bUnselect);
	}
}

//=============================================================================	
void CTargetList::Pimpl::FillRestrictionInfo(eRestrictionLevel infotype, std::vector<ItemRestrictionInfo>& infos, const CItemConstruit * pItemConstruit)
//=============================================================================	
{
	const JInt32 IdItem = pItemConstruit->GetID();

	TerrainParamsMap * pTerrainParamsMap = m_ItemsParams[IdItem];
	TerrainSet * pTerrainsSet = 0;

	// en fonction du type d'info, travaille sur...
	switch (infotype)
	{
		case eWarningSeuil:
			// ...la map des terrains en warning, par item...
			pTerrainsSet = m_WarningTerrains[IdItem];
			break;
		case eForbiddenItem:
			// ... ou la map des terrains interdits, toujours par item
			pTerrainsSet = m_ForbiddenTerrains[IdItem];
			break;
	}

	ASSERT(pTerrainsSet && pTerrainParamsMap);

	infos.clear();

	// on remplit la structure d'info sur les terrains en warning de cet item
	TerrainSetIter ter_it = pTerrainsSet->begin();
	while(ter_it != pTerrainsSet->end())
	{
		ItemRestrictionInfo info;
		info.terrain = (*ter_it)->m_Libelle.AsJCharPtr();
		switch (infotype)
		{
			case eWarningSeuil:
				info.seuilterrain = (*ter_it)->m_SeuilAvertissement;
				break;
			case eForbiddenItem:
				info.seuilterrain = (*ter_it)->m_SeuilRedhibitoire;
				break;
		}
		// nbr de cas de l'item sur le terrain courant
		info.nbcasItem = (*pTerrainParamsMap)[(*ter_it)->m_IdTerrain]->GetNbCas();

		infos.push_back(info);

		++ter_it;
	}
}

//=============================================================================	
void CTargetList::Pimpl::DisplayWarningMessage(const CItemConstruit * pItemConstruit, bool & bUnselect)
//=============================================================================	
{
	// on récupère les infos de restrictions
	std::vector<ItemRestrictionInfo> infos;
	FillRestrictionInfo(eWarningSeuil, infos, pItemConstruit);

	// message pour terrains en dessous des seuils d'avertissement

	CString msg("  Sur le(s) terrain(s) suivant(s), cette cible possède un nombre de cas inférieur "
				"au seuil d'avertissement\n\n");

	// on remplit la chaine à afficher avec les infos de seuil
	for(std::vector<ItemRestrictionInfo>::const_iterator info = infos.begin(); info != infos.end(); ++info)
	{
		msg.AppendFormat("  - %s%s (nb cas : %d, seuil d'avertissement : %d)\n",
												m_pSource->m_Libelle.AsJCharPtr(),
												info->terrain,
												info->nbcasItem,
												info->seuilterrain);
	}
	
	// message pour terrains en dessous des seuils rédhibitoires (et donc non utilisables)
	FillRestrictionInfo(eForbiddenItem, infos, pItemConstruit);
	if (!infos.empty())
	{
		msg.AppendFormat(	"\n\n  En revanche, cette cible ne sera pas utilisable sur le(s) terrain(s) suivant(s). "
							"Le nombre de cas étant trop faible\n\n");

		// on remplit la chaine à afficher avec les infos de seuil
		for(std::vector<ItemRestrictionInfo>::const_iterator info = infos.begin(); info != infos.end(); ++info)
		{
			msg.AppendFormat("  - %s%s (nb cas : %d, seuil rédhibitoire : %d)\n",
				m_pSource->m_Libelle.AsJCharPtr(),
				info->terrain,
				info->nbcasItem,
				info->seuilterrain);
		}
	}

	msg.Append("\n\n                         Souhaitez-vous tout de même sélectionner cette cible?");

	int mbYesNo = ::MessageBox(m_BackRef.m_pDialogOffre->GetSafeHwnd(), msg, "Atelier Presse - Avertissement Seuils", MB_YESNO);
	if (mbYesNo == IDYES)
		bUnselect = false;
	else if (mbYesNo == IDNO)
		bUnselect = true;
}

//=============================================================================	
void CTargetList::Pimpl::DisplayWarningMessage(const CPalettes * pPalettes, bool & bUnselect)
//=============================================================================	
{
	// La boite cibles pondérées
	CDlgMessage DlgMessage;

	// Debut message
	DlgMessage.AddMessage("Une ou plusieurs cibles constituant cette palette ont un nombre de cas inférieur au seuil d'avertissement");
	DlgMessage.AddMessage(" ");

	// on récupère les items la palette
	std::list<const CItemConstruit *> items;
	pPalettes->GetItems(items);

	// on récupère les infos à propos de chaque item
	std::vector<ItemRestrictionInfo> infos;
	std::list<const CItemConstruit *>::const_iterator item_it = items.begin();
	while(item_it != items.end())
	{
		// message pour terrains en dessous des seuils rédhibitoires (et donc non utilisables)
		FillRestrictionInfo(eWarningSeuil, infos, *item_it);

		if (!infos.empty())
		{
			// on ajoute les infos pour cette cible
			CString Txt;
			Txt.Format(" - cible \'%s\', sur les terrains suivants :", (*item_it)->GetLabel().AsJCharPtr());
			DlgMessage.AddMessage(Txt);

			for(std::vector<ItemRestrictionInfo>::const_iterator info = infos.begin(); info != infos.end(); ++info)
			{
				Txt.Format("           - %s%s",m_pSource->m_Libelle.AsJCharPtr(), info->terrain);
				DlgMessage.AddMessage(Txt);
				DlgMessage.AddMessage(" ");
			}
		}
		++item_it;
	}

	// avant d'afficher le message à propos des cibles ayant un seuil inférieur rédhibitoire,
	// on compte si il y au moins une cible dans ce cas
	bool bHasAtLeastOne = false;
	item_it = items.begin();
	while(item_it != items.end())
	{
		if (!(m_ForbiddenTerrains[(*item_it)->GetID()])->empty())
			bHasAtLeastOne = true;
		++item_it;
	}

	if (bHasAtLeastOne)
	{
		// Ajoute entete message rédhibitoire
		DlgMessage.AddMessage("Une ou plusieurs cibles constituant cette palette ont un nombre de cas inférieur au seuil rédhibitoire :");
		DlgMessage.AddMessage(" ");

		// on récupère les infos à propos de chaque item
		item_it = items.begin();
		while(item_it != items.end())
		{
			// message pour terrains en dessous des seuils rédhibitoires (et donc non utilisables)
			FillRestrictionInfo(eForbiddenItem, infos, *item_it);

			if (!infos.empty())
			{
				// on ajoute les infos pour cette cible
				CString Txt;
				Txt.Format(" - cible \'%s\', sur les terrains suivants :",(*item_it)->GetLabel().AsJCharPtr());
				DlgMessage.AddMessage(Txt);

				for(std::vector<ItemRestrictionInfo>::const_iterator info = infos.begin(); info != infos.end(); ++info)
				{
					Txt.Format("           - %s%s",m_pSource->m_Libelle.AsJCharPtr(), info->terrain);
					DlgMessage.AddMessage(Txt);
					DlgMessage.AddMessage(" ");
				}
			}
			++item_it;
		}
	}

	// Affiche titre de la fenêtre de messages
	DlgMessage.SetTitre("Atelier Presse - Avertissement Seuils");

	// Affiche la question
	DlgMessage.SetTexteQuestion("Souhaitez-vous tout de même sélectionner cette palette ?");

	// Affichage message seuils
	if(DlgMessage.DoModal()== IDOK)
		bUnselect = false;
	else
		bUnselect = true;
	
}

/*
//=============================================================================	
void CTargetList::Pimpl::DisplayWarningMessage(const CPalettes * pPalettes, bool & bUnselect)
//=============================================================================	
{
	CString msg("  Une ou plusieurs cibles constituant cette palette ont un nombre de cas inférieur "
		"au seuil d'avertissement : \n\n");

	// on récupère les items la palette
	std::list<const CItemConstruit *> items;
	pPalettes->GetItems(items);

	// on récupère les infos à propos de chaque item
	std::vector<ItemRestrictionInfo> infos;
	std::list<const CItemConstruit *>::const_iterator item_it = items.begin();
	while(item_it != items.end())
	{
		// message pour terrains en dessous des seuils rédhibitoires (et donc non utilisables)
		FillRestrictionInfo(eWarningSeuil, infos, *item_it);

		if (!infos.empty())
		{
			// on ajoute les infos pour cette cible
			msg.AppendFormat(" - cible \'%s\', sur les terrains suivants :\n", (*item_it)->GetLabel().AsJCharPtr());

			for(std::vector<ItemRestrictionInfo>::const_iterator info = infos.begin(); info != infos.end(); ++info)
			{
				msg.AppendFormat("           - %s%s\n", m_pSource->m_Libelle.AsJCharPtr(), info->terrain);
			}
		}
		++item_it;
	}

	// avant d'afficher le message à propos des cibles ayant un seuil inférieur rédhibitoire,
	// on compte si il y au moins une cible dans ce cas
	bool bHasAtLeastOne = false;
	item_it = items.begin();
	while(item_it != items.end())
	{
		if (!(m_ForbiddenTerrains[(*item_it)->GetID()])->empty())
			bHasAtLeastOne = true;
		++item_it;
	}

	if (bHasAtLeastOne)
	{

		msg.Append(	"\n\n  Une ou plusieurs cibles constituant cette palette ont un nombre de cas inférieur "
			"au seuil rédhibitoire : \n\n");

		// on récupère les infos à propos de chaque item
		item_it = items.begin();
		while(item_it != items.end())
		{
			// message pour terrains en dessous des seuils rédhibitoires (et donc non utilisables)
			FillRestrictionInfo(eForbiddenItem, infos, *item_it);

			if (!infos.empty())
			{
				// on ajoute les infos pour cette cible
				msg.AppendFormat(" - cible \'%s\', sur les terrains suivants :\n", (*item_it)->GetLabel().AsJCharPtr());

				for(std::vector<ItemRestrictionInfo>::const_iterator info = infos.begin(); info != infos.end(); ++info)
				{
					msg.AppendFormat("           - %s%s\n", m_pSource->m_Libelle.AsJCharPtr(), info->terrain);
				}
			}
			++item_it;
		}
	}

	msg.Append("\n\n                         Souhaitez-vous tout de même sélectionner cette palette?");

	int mbYesNo = ::MessageBox(m_BackRef.m_pDialogOffre->GetSafeHwnd(), msg, "Atelier Presse - Avertissement Seuils", MB_YESNO);

	if (mbYesNo == IDYES)
		bUnselect = false;
	else if (mbYesNo == IDNO)
		bUnselect = true;
}
*/

//=============================================================================	
void CTargetList::Pimpl::DisplayForbidenMessage(const CItemConstruit * pItemConstruit)
//=============================================================================	
{
	// message pour tous les terrains en dessous du seuil rédhibitoire sur cette cible

	CString msg("  Le nombre de cas de cette cible est inférieur au seuil rédhibitoire, et ce sur tous les terrains sélectionnés. "
				"Vous ne pouvez donc pas utiliser cette cible.\n\n");

	// on récupère les infos de restrictions
	std::vector<ItemRestrictionInfo> infos;
	FillRestrictionInfo(eForbiddenItem, infos, pItemConstruit);

	// on remplit la chaine à afficher avec les infos de seuil
	for(std::vector<ItemRestrictionInfo>::const_iterator info = infos.begin(); info != infos.end(); ++info)
	{
		msg.AppendFormat("  - %s%s (nb cas : %d, seuil rédhibitoire : %d)\n",
			m_pSource->m_Libelle.AsJCharPtr(),
			info->terrain,
			info->nbcasItem,
			info->seuilterrain);
	}

	::MessageBox(m_BackRef.m_pDialogOffre->GetSafeHwnd(), msg, "Atelier Presse - Information Seuils", MB_OK);
}

//=============================================================================	
void CTargetList::Pimpl::DisplayForbidenMessage(const CPalettes * pPalettes)
//=============================================================================	
{
	// message pour toutes les cibles non utilisables sur cette palette

	CString msg("  Aucune des cibles constituant cette palette n'est utilisable sur les terrains sélectionnés. "
				"Vous ne pouvez donc pas utiliser cette palette.\n");

	::MessageBox(m_BackRef.m_pDialogOffre->GetSafeHwnd(), msg, "Atelier Presse - Information Seuils", MB_OK);
}

#pragma endregion CTargetList::Pimpl Gestion des Seuils