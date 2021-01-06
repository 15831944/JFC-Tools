// ListTitre.cpp : fichier d'implémentation
//

#include "stdafx.h"

#include "SupportListFunctors.h"
#include "SupportList.h"
#include "SupportListElt.h"
#include "FormatStr.h"

#include "JSRCPresse.h"
#include "JTerrain.h"
#include "JSegment.h"
#include "OffreTerrainManager.h"

#include "AllTypeOfIterator.h"
#include "KeyTerrain.h"

#include "IDLimits.h"
#include "AllTypeOfNode.h"
#include "IElem.h"

// C++ STL includes
#include <algorithm>
#include <functional>
#include <iterator>
#include <map>
#include <vector>
#include <cstdlib>

// C++ STL extensions header
#include <stdext.hpp>

using namespace std;

//=============================================================================	
//							implémentation privée
//=============================================================================	
#pragma region implémentation privée

const COLORREF TITRES_TEXTCOLOR			= ::GetSysColor(COLOR_WINDOWTEXT);		//RGB(5,145,169);
const COLORREF TITRES_SELTEXTCOLOR		= ::GetSysColor(COLOR_HIGHLIGHTTEXT);	//RGB(154,237,252);

const COLORREF COUPLAGES_TEXTCOLOR		= RGB(25,151,4);
const COLORREF COUPLAGES_SELTEXTCOLOR	= RGB(165,252,150);

const COLORREF FAMILLES_TEXTCOLOR		= RGB(145,3,147);
const COLORREF FAMILLES_SELTEXTCOLOR	= RGB(251,168,252);

const COLORREF NOTFROMSOURCE_BACKCOLOR	= RGB(255,221,221);

//=============================================================================	
// foncteurs d'ajout de lignes de la listbox
//=============================================================================	

//=============================================================================	
// functeur d'insertion d'un elt à une CSupportLists, en respectant le tri des elts
struct InsertSortedLineToListBox
//=============================================================================	
{
	InsertSortedLineToListBox(CSupportList * pListbox, bool sorted = false)
		:	_pListbox	(pListbox)
		,	_sort		(sorted)
		,	_idx		(0)
		,	_pElt		(0)
	{
	}

	void operator () (const SupportListElt * pElttoInsert)
	{		
		// on trouve le template correspondant à l'elt à ajouter
		HXTemplateHandle hxtemplate;
		if (pElttoInsert->bIsTitre)
			hxtemplate = pElttoInsert->bIsSupportFromSource? _pListbox->GetXTemplateTitre() : _pListbox->GetNotFromSrcXTemplateTitre();
		else if (pElttoInsert->bIsCouplage)
			hxtemplate = pElttoInsert->bIsSupportFromSource? _pListbox->GetXTemplateCouplages() : _pListbox->GetNotFromSrcXTemplateCouplages();
		else
			hxtemplate = pElttoInsert->bIsSupportFromSource? _pListbox->GetXTemplateFamilles() : _pListbox->GetNotFromSrcXTemplateFamilles();

		// est-ce que l'on insère en triant
		if (_sort && _pListbox->GetCount() > 0)
		{
			const int nMax = _pListbox->GetCount();
			while (_idx < nMax)
			{
				_pElt = reinterpret_cast<SupportListElt*>(_pListbox->GetItemData(_idx));
				if (_sortFunctor.operator ()(pElttoInsert, _pElt))
				{
					// on insère ici à la place de l'elt _idx (en fonction de la couleur, on donne tel ou tel template)
						_idx = _pListbox->InsertString(_idx, pElttoInsert->GetLabel(), hxtemplate);

					_pListbox->SetItemData(_idx, reinterpret_cast<DWORD_PTR>(pElttoInsert));
					_pListbox->SetSel(_idx, pElttoInsert->iSelected);
					return;
				}
				++_idx;
			}
			// on est ici, on insère à la fin (en fonction de la couleur, on donne tel ou tel template)
			_idx = _pListbox->InsertString(LB_ERR, pElttoInsert->GetLabel(), hxtemplate);

			_pListbox->SetItemData(_idx, reinterpret_cast<DWORD_PTR>(pElttoInsert));
			_pListbox->SetSel(_idx, pElttoInsert->iSelected);
		}
		else
		{
			// on est ici, on insère à la fin (en fonction de la couleur, on donne tel ou tel template)
			_idx = _pListbox->AddLine(pElttoInsert->GetLabel(), hxtemplate);

			_pListbox->SetItemData(_idx, reinterpret_cast<DWORD_PTR>(pElttoInsert));
			_pListbox->SetSel(_idx, pElttoInsert->iSelected);
		}
	}

protected:

	CSupportList *	_pListbox;
	SupportListElt * _pElt;
	int				_idx;
	const bool		_sort;
	TitleLabelSort	_sortFunctor;
};

//=============================================================================	
//				functeurs pour elements de listes de supports
//=============================================================================	

//=============================================================================	
bool IsApparie(const SupportListElt * pElt)
//=============================================================================	
{
	return pElt->bIsApparie;
}

//=============================================================================	
	template <typename T>
void DeleteObject(T o)
//=============================================================================	
{
	if (o) delete o; o = 0;
}

#pragma endregion implémentation privée

//=============================================================================	
//								CSupportFilter							
//=============================================================================	
#pragma region CSupportFilter

//=============================================================================	
CSupportFilter::CSupportFilter(const int num_periodicites_)
//=============================================================================	
	:	num_periodicites(num_periodicites_)
{
	Reset();
}

//=============================================================================	
void CSupportFilter::Reset()
//=============================================================================	
{
	segments.clear();
	terrains.clear();
	periodicites.clear();				
	all_segments.clear();
	strSearch.Empty();
	idSource			= 0;
	titre_coupl_famil	= 0;
	bDirtyFlag			= true;
	bUseSegments		= false;
	onlyselected		= false;
	supports_from_source= true;
}

//=============================================================================	
bool CSupportFilter::Empty() const
//=============================================================================	
{
	CSupportFilter * This = (const_cast<CSupportFilter *>(this));
	if (idSource == 0)
		if (terrains.empty() && segments.empty() && periodicites.empty() || periodicites.size() == num_periodicites)
			if ((titre_coupl_famil == DISPLAY_ALL) || (titre_coupl_famil == 0))
				return (strSearch.IsEmpty());
	return false;
}

//=============================================================================	
// à appeler lors du changement de source d'un filtre (permet au filtre de connaître ts les ids segments posibles)
void CSupportFilter::SetAllSegments(const vector<const JSegment*> & all_segments_)
//=============================================================================	
{
	bUseSegments = (all_segments_.size() > 1);
	all_segments.clear();

	// on passe dans les segments pour crée la liste d'ids
	std::vector<const JSegment*>::const_iterator seg_it = all_segments_.begin();
	while (seg_it != all_segments_.end())
	{
		all_segments.push_back((*seg_it)->m_IdSegment);
		++seg_it;
	}
	sort(all_segments.begin(), all_segments.end());
}
#pragma endregion CSupportFilter

//=============================================================================	
//								CSupportList
//=============================================================================	

#pragma region CSupportList

// Gestionnaires de messages de CSupportList
BEGIN_MESSAGE_MAP(CSupportList, CJFCXListBox)
	ON_NOTIFY_EX_RANGE(TTN_NEEDTEXTW, 0, 0xFFFF, OnToolTipText)
	ON_NOTIFY_EX_RANGE(TTN_NEEDTEXTA, 0, 0xFFFF, OnToolTipText)
END_MESSAGE_MAP()

IMPLEMENT_DYNAMIC(CSupportList, CJFCXListBox)

//=============================================================================	
// functor utilisé pour définir la sélection d'u support si son id se trouve dans le container passé en paramêtre
template<class C>
struct SetSupportSelection 
{
	SetSupportSelection(C & container) : _container(container) {}

	void operator ()(SupportListElt * pSupportListElt)
	{
		// on cherche dans le container l'id du support courant
		_iter = _container.find(pSupportListElt->Id);
		if (_iter != _container.end())
		{
			// on a trouvé l'id du support courant
			pSupportListElt->iSelected = TRUE;

			// on peut supprimer cet id du container
			_container.erase(_iter);
		}
		else
		{
			// pas trouvé, le support n'est pas sélectionné
			pSupportListElt->iSelected = FALSE;
		}
	}

	C & _container;
	typename C::iterator _iter;
};

typedef set<SupportListElt*, less<const SupportListElt*> > SupportSet;

//=============================================================================	
// pointeur vers l'implémentation privée de CSupportList
class CSupportList::Pimpl
//=============================================================================	
{
public :
	
	Pimpl() {}

	~Pimpl()
	{
		// on supprime toutes les SupportListElt
		for_each(m_AllSupports.begin(), m_AllSupports.end(), DeleteObject<SupportListElt*>);
	}

	SupportSet			m_StartSupports;	// tous les supports présents au début sans les appariés
	SupportSet			m_AllSupports;		// tous les supports (union des titres et des couplages) avec appariés courant
	SupportSet			m_CurrentApparies;	// les titres appariés pour la source et les terrains courants
};

//=============================================================================	
CSupportList::CSupportList(CATPColors::APPCOLOR appColor)
//=============================================================================	
	:	m_pRegroupements(0)
	,	m_pSrcPresse(0)
	,	m_pchTip(0)
	,	m_pwchTip(0)
{
	m_pImpl = new Pimpl;
	m_pRegroupements = CRegroupements::GetInstance();
	m_pSrcPresse = &JSRCPresse::GetInstance();

	m_callingApp = appColor;

	// on crée les templates pour cet appcolor
	// on donne des valeurs invalides pour les handle template (tant que la couleur n'est pa défini)
	m_TitresXTemplate		= CXTemplateManager::InvalidHandle;
	m_CouplagesXTemplate	= CXTemplateManager::InvalidHandle;
	m_FamillesXTemplate		= CXTemplateManager::InvalidHandle;

	m_NotFromSourceTitresXTemplate		= CXTemplateManager::InvalidHandle;
	m_NotFromSourceCouplagesXTemplate	= CXTemplateManager::InvalidHandle;
	m_NotFromSourceFamillesXTemplate	= CXTemplateManager::InvalidHandle;
}

//=============================================================================	
CSupportList::~CSupportList()
//=============================================================================	
{
	// on supprime le pimpl
	if (m_pImpl) delete m_pImpl; m_pImpl = 0;
}

//=============================================================================	
// valeurs constantes définissant la nature d'une modification
typedef enum eModificationType
//=============================================================================	
{
	emt_NOTHING			= 0,		// pas de modif
	emt_REMOVE_ELEMENTS	= 1,		// suppression d'elts
	emt_ADD_ELEMENTS	= 2,		// ajout d'elts
	emt_BOTH			= 3			// suppression et ajout d'elts
};

//=============================================================================	
// chargement des données et initialisation de la listbox
void CSupportList::InitializeData(set<JUnt32> & selectedIds)
//=============================================================================	
{
	// on donne des valeurs invalides pour les handle template (tant que la couleur n'est pa défini)
	m_TitresXTemplate = CXTemplateManager::GetInstance()->CreateTemplate(TITRES_TEXTCOLOR,CATPColors::GetColorWhite(),
		TITRES_SELTEXTCOLOR,CATPColors::GetColorSelect(m_callingApp));	// bleu
	m_CouplagesXTemplate = CXTemplateManager::GetInstance()->CreateTemplate(COUPLAGES_TEXTCOLOR,CATPColors::GetColorWhite(),
		COUPLAGES_SELTEXTCOLOR,CATPColors::GetColorSelect(m_callingApp));	// vert
	m_FamillesXTemplate = CXTemplateManager::GetInstance()->CreateTemplate(FAMILLES_TEXTCOLOR,CATPColors::GetColorWhite(),
		FAMILLES_SELTEXTCOLOR,CATPColors::GetColorSelect(m_callingApp));	//violet

	m_NotFromSourceTitresXTemplate = CXTemplateManager::GetInstance()->CreateTemplate(TITRES_TEXTCOLOR,NOTFROMSOURCE_BACKCOLOR,
		TITRES_SELTEXTCOLOR,CATPColors::GetColorSelect(m_callingApp));	// bleu
	m_NotFromSourceCouplagesXTemplate = CXTemplateManager::GetInstance()->CreateTemplate(COUPLAGES_TEXTCOLOR,NOTFROMSOURCE_BACKCOLOR,
		COUPLAGES_SELTEXTCOLOR,CATPColors::GetColorSelect(m_callingApp));	// vert
	m_NotFromSourceFamillesXTemplate = CXTemplateManager::GetInstance()->CreateTemplate(FAMILLES_TEXTCOLOR,NOTFROMSOURCE_BACKCOLOR,
		FAMILLES_SELTEXTCOLOR,CATPColors::GetColorSelect(m_callingApp));	//violet

	// on charge tous les supports existants
	m_pImpl->m_StartSupports.clear();

	// on boucle sur les titres et couplages des sources presses
	for( m_pSrcPresse->m_TBLTitres.MoveFirst(); m_pSrcPresse->m_TBLTitres.IsValid(); m_pSrcPresse->m_TBLTitres.MoveNext() )
	{
		SupportListElt * pSupport = 0;
		IElem * pElem = m_pSrcPresse->m_TBLTitres.GetItem(); ASSERT(pElem);

		// c'est un couplage fixe
		if (pElem->IsCouplage())
			pSupport = new SupportListElt(pElem, SupportListElt::sletCouplagefixe);
		// c'est un titre
		else
			pSupport = new SupportListElt(pElem, SupportListElt::sletTitre);

		// ce support doit il-être sélectionné?
		set<JUnt32>::iterator ids_it = selectedIds.find(pSupport->Id);
		if (ids_it != selectedIds.end())
		{
			pSupport->iSelected = TRUE;
			selectedIds.erase(ids_it);
		}

		m_pImpl->m_StartSupports.insert(pSupport);	
	}

	// on boucle sur les titres apparentés
	JNodeList * pTitresApparentes = m_pRegroupements->GetTitresApparentes();
	for(pTitresApparentes->MoveFirst(); pTitresApparentes->IsValid(); pTitresApparentes->MoveNext())
	{
		SupportListElt * pSupport = new SupportListElt(pTitresApparentes->GetItem(), SupportListElt::sletTitre);

		// ce support doit il-être sélectionné?
		set<JUnt32>::iterator ids_it = selectedIds.find(pSupport->Id);
		if (ids_it != selectedIds.end())
		{
			pSupport->iSelected = TRUE;
			selectedIds.erase(ids_it);
		}
		m_pImpl->m_StartSupports.insert(pSupport);
	}

	// on boucle sur les couplages libres
	JNodeList * pCouplagesLibres = m_pRegroupements->GetCouplagesLibres();
	for( pCouplagesLibres->MoveFirst(); pCouplagesLibres->IsValid(); pCouplagesLibres->MoveNext() )
	{
		SupportListElt * pSupport = new SupportListElt(pCouplagesLibres->GetItem(), SupportListElt::sletCouplagelibre);

		// ce support doit il-être sélectionné?
		set<JUnt32>::iterator ids_it = selectedIds.find(pSupport->Id);
		if (ids_it != selectedIds.end())
		{
			pSupport->iSelected = TRUE;
			selectedIds.erase(ids_it);
		}
		m_pImpl->m_StartSupports.insert(pSupport);	
	}

	// on boucle sur les familles
	for (m_pRegroupements->GetFamilles()->MoveFirst(); m_pRegroupements->GetFamilles()->IsValid();
		m_pRegroupements->GetFamilles()->MoveNext())
	{
		SupportListElt * pFamille = new SupportListElt(m_pRegroupements->GetFamilles()->GetItem(),
			SupportListElt::sletFamille);

		// ce support doit il-être sélectionné? (on ne la sélectionne que s'il n'est pas vide)
		set<JUnt32>::iterator ids_it = selectedIds.find(pFamille->Id);
		if (ids_it != selectedIds.end() && pFamille->pElem->IsLot())
		{
			pFamille->iSelected = TRUE;
			selectedIds.erase(ids_it);
		}
		m_pImpl->m_StartSupports.insert(pFamille);	
	}
}
//=============================================================================	
// functor utilisé pour ajouter à 'container' le pointeur vers le IElem correspondant au support
template<class C>
struct InsertSupportIElemPtr 
{
	InsertSupportIElemPtr(C & container)
		:	_back_inserter(container)
	{}

	void operator ()(const SupportListElt * pSupportListElt)
	{
		*_back_inserter = pSupportListElt->pElem;
		++_back_inserter;
	}

	back_insert_iterator<C> _back_inserter;
};

//=============================================================================	
// functor utilisé pour ajouter à 'container' l'id correspondant au support
template<class C>
struct InsertSupportId 
{
	InsertSupportId(C & container)
		:	_inserter(container, container.begin())
	{}

	void operator ()(const SupportListElt * pSupportListElt)
	{
		*_inserter = pSupportListElt->Id;
		++_inserter;
	}

	insert_iterator<C> _inserter;
};

//=============================================================================	
// récupère la sélection complète (les supports eux-mêmes)
void CSupportList::GetWholeSelection (list<const IElem *> & selectedElems) const
//=============================================================================	
{
	//// on copie les elements sélectionnés dans 'selectedElems'
	for_each_if(m_pImpl->m_AllSupports.begin(), m_pImpl->m_AllSupports.end(), IsSupportSelected(), InsertSupportIElemPtr<list<const IElem *> >(selectedElems));
}

//=============================================================================	
// récupère la sélection complète (uniquement les id des supports)
void CSupportList::GetWholeSelection (set<JUnt32> & selectedIds) const
//=============================================================================	
{
	// on supprime tous les élements
	selectedIds.clear();

	set<SupportListElt*> eltdata;
	GetControlSelectedData(eltdata, *this);

	set<SupportListElt*>::iterator eltdata_it = eltdata.begin();
	while (eltdata_it != eltdata.end())
	{
		selectedIds.insert((*eltdata_it)->Id);
		++eltdata_it;
	}
}

//=============================================================================	
// simple function définiaant un element comme n'etant pas un suport de la source
void SetAsNotFromSource (SupportListElt * pElt)
//=============================================================================	
{
	pElt->bIsSupportFromSource = false;
}

//=============================================================================	
// mise à jour des support affichés
void CSupportList::UpdateDisplay(CSupportFilter & newfilter)
//=============================================================================	
{
	const int iTopIdx = GetTopIndex();
	int iNewTopIdx = LB_ERR;

	set<JUnt32> emptyVec;

	// on freeze l'affichage
	CWaitCursor Wait;
	LockWindowUpdate();

	static list<SupportListElt*> displayList;
	displayList.clear();

	ResetContent();

	// on crée notre functor-filtre composé
	SupportFilterFuncComposer _functor;

	// on ajoute si besoin un filtre de libellé
	if (!newfilter.strSearch.IsEmpty())
		_functor.AddFilter(new IsSupportLabelContains(newfilter.strSearch));

	bool bSetAllAsNotFromSource = newfilter.terrains.empty();
	// on ajoute le filtre terrains
	if (!newfilter.terrains.empty() && newfilter.supports_from_source)
		_functor.AddFilter( new IsSupportOnTerrains(newfilter.terrains, newfilter.idSource/*, m_pImpl->m_AllApparies*/));

	// on ajoute le filtre segments si nécessaire
	if (newfilter.segments.size() != newfilter.all_segments.size())
		_functor.AddFilter( new IsSupportOnSegments(newfilter.terrains, newfilter.segments, newfilter.idSource));

	// on ajoute le filtre periodicites
	if (!newfilter.periodicites.empty() && (newfilter.periodicites.size() < static_cast<size_t>(newfilter.num_periodicites)))
		_functor.AddFilter( new IsSupportPeriodicityIsOneOf<list<JUnt32> >(newfilter.periodicites));

	// on ajoute si besoin un filtre de type de supports
	if (!(newfilter.titre_coupl_famil == CSupportFilter::DISPLAY_ALL || newfilter.titre_coupl_famil == 0)) 
		_functor.AddFilter( new IsSupportTypeOneOf(newfilter.titre_coupl_famil));

	if (newfilter.onlyselected)
		_functor.AddFilter( new IsSupportSelected());

	copy_if(m_pImpl->m_AllSupports.begin(), m_pImpl->m_AllSupports.end(), back_inserter(displayList), _functor);

	// notre functor d'ajout de ligne dans la listbox
	InsertSortedLineToListBox addLineFn(this, false);

	// on definit eventuellement tous les supports comme n'etant pas de la source
	if (bSetAllAsNotFromSource)
		for_each(displayList.begin(), displayList.end(), &SetAsNotFromSource);

	displayList.sort(TitleLabelSort());

	// on affiche tous les supports
	for_each(displayList.begin(), displayList.end(), addLineFn);
	_functor.ReleaseFilters();

	list<int> selectedIdx;
	GetControlSelection(selectedIdx, *this);
	if (selectedIdx.empty())
	{
		// si rien n'est séletionné, on redefinit le top index
		iNewTopIdx = (iTopIdx >= GetCount()) ? 0 : iTopIdx;
	}
	else
	{
		// on définit comme topidx le 1er item sélectionné
		iNewTopIdx = *min_element(selectedIdx.begin(), selectedIdx.end());
	}
	SetTopIndex(iNewTopIdx);

	// on unfreeze l'affichage
	UnlockWindowUpdate();
}

//=============================================================================	
//							CSupportList ToolTip management
//=============================================================================	
#pragma region CSupportList ToolTip management

//=============================================================================	
void CSupportList::PreSubclassWindow()
//=============================================================================	
{
	EnableToolTips(TRUE);
	CJFCXListBox::PreSubclassWindow();
}

//=============================================================================	
int CSupportList::OnToolHitTest(CPoint point, TOOLINFO * pTI) const
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
//Define OnToolTipText(). This is the handler for the TTN_NEEDTEXT notification from 
//support ansi and unicode 
BOOL CSupportList::OnToolTipText( UINT id, NMHDR * pNMHDR, LRESULT * pResult )
//=============================================================================	
{
	// on prend en compte les 2 versions différentes de ce messgae, ANSI et UNICODE
	TOOLTIPTEXTA* pTTTA = reinterpret_cast<TOOLTIPTEXTA*>(pNMHDR);
	TOOLTIPTEXTW* pTTTW = reinterpret_cast<TOOLTIPTEXTW*>(pNMHDR);
	CString strTipText;
	UINT nID = static_cast<UINT>(pNMHDR->idFrom);

	::SendMessage(pNMHDR->hwndFrom, TTM_SETMAXTIPWIDTH, 0, 300);
	SupportListElt * pSupport =	reinterpret_cast<SupportListElt*>(GetItemData(nID));
	if (pSupport)
	{
		if (pSupport->bIsCouplage)
		{
			IElem * pCouplElem = pSupport->pElem;
			bool bcouplagelibre = pCouplElem->IsCouplageLibre();
			bool bcouplagefixe = pCouplElem->IsCouplageFixe();

			strTipText.Format("     Couplage \'%s\' :\n", pCouplElem->GetLabel().AsJCharPtr());

			// on concatène tous les noms des supports contenus dans ce couplage
			if (bcouplagefixe)
			{
				JCouplageFixe * cplfixe = pCouplElem->AsCouplageFixe();
				JList<IElem*> listFils;
				cplfixe->GetFils(listFils);
				listFils.MoveFirst();
				while(listFils.IsValid())
				{
					strTipText.AppendFormat("%s\r\n", listFils.GetItem()->GetLabel().AsJCharPtr());				
					listFils.MoveNext();
				}
			}
			else if (bcouplagelibre)
			{
				CCouplageLibre * cpllibre = pCouplElem->AsCouplageLibre();
				IIterateur* it = cpllibre->CreateIterateur();
				CTitreCouplageLibre* tcpllibre = (CTitreCouplageLibre*)it->getHead();
				while (tcpllibre)
				{
					const JUnt32 cplId = tcpllibre->GetID();
					strTipText.AppendFormat("%s\r\n", m_pSrcPresse->m_TBLTitres.GetLabelByID(cplId));
					tcpllibre = (CTitreCouplageLibre*)it->getNext();
				}
				delete it;
			}
		}
		else if (pSupport->bIsTitre)
		{
			strTipText.Append(pSupport->GetLabel());
		}
		else if (pSupport->bIsFamille)
		{
			strTipText.Format("     Famille \'%s\' :\n", pSupport->GetLabel());
			// on concatène tous les noms des supports contenus dans cette famille
			IIterateur * fam_it = pSupport->pElem->AsLot()->CreateIterateur();
			CTitreLot * pTitreLot = (CTitreLot*)fam_it->getHead();
			bool bRet = false;
			while (pTitreLot)
			{
				strTipText.AppendFormat("%s\r\n", pTitreLot->GetLabel().AsJCharPtr());
				pTitreLot = (CTitreLot*)fam_it->getNext();
			}
			delete fam_it;
		}
	}
	strTipText.TrimRight();
	strTipText.AppendChar('\0');

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

#pragma endregion CSupportList ToolTip management

#pragma endregion CSupportList


//=============================================================================	
// déselection de tous les supports
void CSupportList::ResetSelection()
//=============================================================================	
{
	LockWindowUpdate();

	// on déselectionne graphiquement les elements
	const int nMax = GetCount();
	SelItemRange(FALSE, 0, nMax);

	int idx = 0;
	while (idx < nMax)
	{
		SupportListElt * pElem = reinterpret_cast<SupportListElt*>(GetItemData(idx));
		pElem->iSelected = FALSE;
		++idx;
	}

	UnlockWindowUpdate();
}

//=============================================================================	
// gestion de l'evenement : on a clické sur le bouton 'T' de la sélection
bool CSupportList::OnClickOnSelectTout(const bool bIsMaSelection)
//=============================================================================	
{
	LockWindowUpdate();

	set<int> elements;
	const int nMax = GetCount();
	GetControlSelection(elements, *this);

	// qu'a-t-on de sélectionné pour l'instant, si rien on sélectionne tout ce qui est affiché, 
	// sinon si au moins un site affiché est sélectionné, on déselectionne tout ce qui est affiché
	bool bSelectAll = elements.empty();

	//int idx = 0;
	//while (idx < nMax)
	//{
	//	SupportListElt * pElem = reinterpret_cast<SupportListElt*>(GetItemData(idx));
	//	if (bSelectAll && !pElem->bIsFamille || pElem->pElem->IsLot())
	//		// on n'ajoute pas les familles vides
	//		pElem->iSelected = TRUE;
	//	else
	//	{
	//		pElem->iSelected = FALSE;
	//		// on déselectionné le listitem
	//		SetSel(idx, FALSE);
	//	}
	//	++idx;
	//}

	int idx = 0;
	while (idx < nMax)
	{
		SupportListElt * pElem = reinterpret_cast<SupportListElt*>(GetItemData(idx));
		if (bSelectAll)
		{
			if (!pElem->bIsFamille || pElem->pElem->IsLot())
			{
				// on sélectionne tout...
				pElem->iSelected = TRUE;
				SetSel(idx, TRUE);
			}
			else
			{
				// ...sauf les familles vides
				pElem->iSelected = FALSE;
				SetSel(idx, FALSE);
			}
		}
		else
		{
			pElem->iSelected = FALSE;
			// on déselectionné le listitem
			SetSel(idx, FALSE);
		}
		++idx;
	}




	// si on est sur 'Ma Sélection' et que l'on déselectionne tout, on a plus de supports à afficher
	if (!bSelectAll && bIsMaSelection)
		ResetContent();

	UnlockWindowUpdate();

	// si il y avait quelquechose dans la liste, on indique une modif
	return (bSelectAll);
}

//=============================================================================	
// y a-t-il au moins 1 support sélectionné (pas forcément affiché car peut-être filtré)
bool CSupportList::HasAtLeastOneSupportSelected() const
//=============================================================================	
{
	// on cherche si il existe 1 element sélectionné
	SupportSet::const_iterator it = 
		find_if(m_pImpl->m_AllSupports.begin(), m_pImpl->m_AllSupports.end(), IsSupportSelected());

	return (it != m_pImpl->m_AllSupports.end());
}

//=============================================================================	
void CSupportList::UpdateApparies(const JUnt32 IdSource, const std::list<JUnt32> & terrains, const std::set<JUnt32> & selectedIds)
//=============================================================================	
{
	CTitresApparies * pTitresApparies = 0;

	// on detruit les appariés courants
	for_each(m_pImpl->m_CurrentApparies.begin(), m_pImpl->m_CurrentApparies.end(), DeleteObject<SupportListElt*>);

	// on vide le set
	m_pImpl->m_CurrentApparies.clear();

	std::list<JUnt32>::const_iterator ter_it = terrains.begin();
	while (ter_it != terrains.end())
	{                                                                                     
		if (pTitresApparies = m_pRegroupements->GetTitresApparies(IdSource, *ter_it))    
		{
			pTitresApparies->MoveFirst();
			while(pTitresApparies->IsValid())
			{
				// on crée un nouveau support
				SupportListElt * pSupport = new SupportListElt(pTitresApparies->GetItem(), SupportListElt::sletTitre, true);

				// ce titre apparié est-il dans la liste des supports à sélectionner?
				if (selectedIds.find(pSupport->Id) != selectedIds.end())
					pSupport->iSelected = TRUE;

				// on l'ajoute au container de resultat
				m_pImpl->m_CurrentApparies.insert(pSupport);		
				pTitresApparies->MoveNext();
			}
		}
		++ter_it;
	}

	// on vide l'union de tous les supports 
	m_pImpl->m_AllSupports.clear();

	// on la recrée avec les nouveaux appariés
	set_union(m_pImpl->m_CurrentApparies.begin(), m_pImpl->m_CurrentApparies.end(),
		m_pImpl->m_StartSupports.begin(), m_pImpl->m_StartSupports.end(),
		insert_iterator<SupportSet>(m_pImpl->m_AllSupports, m_pImpl->m_AllSupports.begin()));
}