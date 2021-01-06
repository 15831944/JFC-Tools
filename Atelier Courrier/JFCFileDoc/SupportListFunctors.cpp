#include <stdafx.h>

#include "idlimits.h"

#include "SupportListFunctors.h"
#include "SupportList.h"
#include "SupportListElt.h"

#include "OffreTerrainManager.h"
#include "AllTypeOfNode.h"
#include "AllTypeOfIterator.h"
#include "IElem.h"

using namespace std;

//=============================================================================	
//=============================================================================	
//								IsSupportOnTerrains
//Renvoie true si le support se trouve sur au moins 1 des terrains de la liste
//=============================================================================	
//=============================================================================	

//=============================================================================	
// fonctions pour savoir si un IElem (support) se trouve sur un terrain
bool IsElemOnTerrain	(CTerrain * pTerrain, IElem * pElem);
bool IsTitreOnTerrain	(CTerrain * pTerrain, const JUnt32 Id);
bool IsCouplageOnTerrain(CTerrain * pTerrain, IElem * pElem);
bool IsFamilleOnTerrain	(CTerrain * pTerrain, CLot * pFamille);

IsSupportOnTerrains::IsSupportOnTerrains(const list<JUnt32> & terrains, const JUnt32 idSource/*, const MapAppariesBySourceTerrains & allApparies*/)
	:	_terrains(terrains)
	,	_offreterMgr(COffreTerrainManager::GetInstance())
{
	_KeyTerrain.SetSource(idSource);
}

bool IsSupportOnTerrains::operator()(SupportListElt * elt)
{
	CTerrain * pTerrain = 0;
	_ter_id = _terrains.begin();	
	// boucle sur le terrains sélectionnés
	while (_ter_id != _terrains.end())
	{
		_KeyTerrain.SetTerrain(*_ter_id);	
		if (pTerrain = _offreterMgr->GetTerrain(_KeyTerrain))
			if (IsElemOnTerrain(pTerrain, elt->pElem))
				return (elt->bIsSupportFromSource = true);
		++_ter_id;
	}
	return (elt->bIsSupportFromSource = false);
}

//=============================================================================	
//=============================================================================	
//								IsSupportOnSegments
// Renvoie true si le support se trouve sur au moins 1 des segemnts de la liste
// renvoie true egalement si il n'y pas de segments, ou de terrains dans le container
//=============================================================================	
//=============================================================================	

bool IsElemOnSegment(CTerrain * pTerrain, IElem * pElem, const JUnt32 segId);

//=============================================================================	
// fonctions pour savoir si un IElem (support) se trouve sur un segment
bool IsTitreOnSegment (CTerrain * pTerrain, const JUnt32 IdAudience, const JUnt32 segId)
{
	JVector<CSegment *> Segments;

	//on recupere les segments
	pTerrain->GetSegments(Segments);
	for(Segments.MoveFirst();Segments.IsValid();Segments.MoveNext())
	{
		if (Segments.GetItem()->GetID() == segId)
		{
			return pTerrain->SupportBelongsToSegment(Segments.GetItem(), IdAudience);
		}
	}
	return false;
}

//=============================================================================	
bool IsCouplageOnSegment(CTerrain * pTerrain, IElem * pElem, const JUnt32 segId)
{
	// couplages fixes
	if (pElem->IsCouplageFixe())
	{
		JCouplageFixe * cplfixe = pElem->AsCouplageFixe();
		JList<IElem*> lstElem;
		cplfixe->GetFils(lstElem);
		for (lstElem.MoveFirst(); lstElem.IsValid(); lstElem.MoveNext())
		{
			if (!IsElemOnSegment(pTerrain, lstElem.GetItem(), segId))
				return false;
		}
		return true;
	}

	// couplages libres
	if (pElem->IsCouplageLibre())
	{
		CCouplageLibre * cpllibre = pElem->AsCouplageLibre();
		IIterateur* it = cpllibre->CreateIterateur();
		CTitreCouplageLibre* tcpllibre = (CTitreCouplageLibre*)it->getHead();
		ASSERT(tcpllibre);
		bool bRet = true;
		while (tcpllibre)
		{
			if (!IsElemOnSegment(pTerrain, tcpllibre, segId))
			{
				bRet = false;
				break;
			}
			tcpllibre = (CTitreCouplageLibre*)it->getNext();
		}
		delete it;
		return bRet;
	}

	ASSERT(FALSE);
	return false;
}

//=============================================================================	
bool IsFamilleOnSegment	(CTerrain * pTerrain, CLot * pFamille, const JUnt32 segId)
{
	ASSERT(pFamille && pTerrain);

	IIterateur* fam_it = pFamille->CreateIterateur();
	CTitreLot* pTitreLot = (CTitreLot*)fam_it->getHead();
	bool bRet = true;
	while (pTitreLot)
	{
		if ( !IsElemOnTerrain(pTerrain, pTitreLot) )
		{
			bRet = false;
			break;
		}
		//if ( !IsTitreOnTerrain(pTerrain, pTitreLot->GetID()) )
		//{
		//	bRet = false;
		//	break;
		//}
		pTitreLot = (CTitreLot*)fam_it->getNext();
	}
	delete fam_it;
	return bRet;
}

//=============================================================================	
bool IsElemOnSegment(CTerrain * pTerrain, IElem * pElem, const JUnt32 segId)
{
	ASSERT(pElem);

	if (pTerrain->IsLoaded())
	{
		if (IsElemOnTerrain(pTerrain, pElem))
		{
			if(pElem->IsCouplage())
				return IsCouplageOnSegment(pTerrain, pElem, segId);

			if (pElem->IsTitre())
				return IsTitreOnSegment(pTerrain, pElem->AsITitre()->GetIDAudience(), segId);

			// c'est forcément une famille, mais IsLot n'est pas forcément 'true'
			if (pElem->IsLot())
				return (IsFamilleOnSegment(pTerrain, static_cast<CLot*>(pElem->AsLot()), segId));
		}
	}
	return false;
}

//=============================================================================	
IsSupportOnSegments::IsSupportOnSegments(const std::list<JUnt32> & terrains,
										 const std::list<JUnt32> & segments,
										 const JUnt32 idSource)
	 :	_terrains(terrains)
	 ,	_segments(segments)
	 ,	_offreterMgr(COffreTerrainManager::GetInstance())
{
	_KeyTerrain.SetSource(idSource);
}

bool IsSupportOnSegments::operator()(SupportListElt * elt)
{
	// pas de terrain ou de segments à filtrer, pas de filtre donc tous les elts passent
	if (_terrains.empty() || _segments.empty()) return true;

	CTerrain * pTerrain = 0;
	_ter_id = _terrains.begin();
	// boucle sur le terrains sélectionnés
	while (_ter_id != _terrains.end())
	{
		_KeyTerrain.SetTerrain(*_ter_id);	
		if ((pTerrain = _offreterMgr->GetTerrain(_KeyTerrain)))
		{
			_seg_id = _segments.begin();
			// boucle sur le segments sélectionnés
			while(_seg_id != _segments.end())
			{
				if (IsElemOnSegment(pTerrain, elt->pElem, *_seg_id))
					return true;
				++_seg_id;
			}
		}
		++_ter_id;
	}
	return false;
}


//=============================================================================	
//=============================================================================	
//								IsSupportLabelContains
// Renvoie true si le libellé du support contient la chaine passée au constructeur
//=============================================================================	
//=============================================================================	

IsSupportLabelContains::IsSupportLabelContains(const char * strToFind)
{
	// on fait une copie locale sans accents de la chaine de filtrage
	string s(strToFind, strlen(strToFind));
	PrepareStr(s);	
	_strToFind = new char[s.length() + 1];
	strncpy(_strToFind, s.c_str(), s.length());
	_strToFind[s.length()] = '\0';
}

IsSupportLabelContains::~IsSupportLabelContains()
{
	// on efface la chaine de filtrage
	if (_strToFind) delete[] _strToFind; _strToFind = 0;
}

bool IsSupportLabelContains::operator()(SupportListElt * elt)
{
	// supprime les accents du libellé du support avant d'y chercher la chaine de filtrage
	static string _sCur;
	_sCur = PrepareStr(string (elt->GetLabel(), strlen(elt->GetLabel())));
	return (_sCur.find(_strToFind) != string::npos);
}

//=============================================================================	
//=============================================================================	
//								IsSupportLabelContains
// Renvoie true si le type du support est un des types contenus dans l'entier 32 bits
// passé au constructeur et qui est une compoisiton par OU BIT A BIT (Bitwise-OR)
// de une ou plusieurs constantes
// (@see CSupportFilter::DISPLAY_TITRES, DISPLAY_COUPLAGES, DISPLAY_FAMILLES)
//=============================================================================	
//=============================================================================	


IsSupportTypeOneOf::IsSupportTypeOneOf(const int types, bool negate_)
	: _types(types), _negate(negate_)
{

}

bool IsSupportTypeOneOf::operator()(SupportListElt * elt)
{
	static bool bRet;

	if (_types)
	{
		// un titre ?
		if (elt->bIsTitre)
			bRet =  ((_types & CSupportFilter::DISPLAY_TITRES) == CSupportFilter::DISPLAY_TITRES);
		// un couplage ?
		else if (elt->bIsCouplage)
			bRet =  ((_types & CSupportFilter::DISPLAY_COUPLAGES) == CSupportFilter::DISPLAY_COUPLAGES);
		// une famille ?
		else
			bRet =  ((_types & CSupportFilter::DISPLAY_FAMILLES) == CSupportFilter::DISPLAY_FAMILLES);

		return (_negate)?!bRet:bRet;
	}
	else return (_negate?false:true);
}


//=============================================================================	
//=============================================================================	
//								IsSupportSelected
//					Renvoie true si le support est sélectionné
//=============================================================================	
//=============================================================================	


//=============================================================================	
bool IsSupportSelected::operator()(SupportListElt * elt)
//=============================================================================	
{
	return (elt->iSelected == TRUE)?true:false;
}


//=============================================================================	
//=============================================================================	
//								TitleLabelBase
// Foncteur de base pour les comparaisons et tris de libellés de supports
//=============================================================================	
//=============================================================================	

const string TitleLabelBase::_l_apos("l'");
const string TitleLabelBase::_le	("le ");
const string TitleLabelBase::_la	("la ");
const string TitleLabelBase::_les	("les ");

void TitleLabelBase::RemoveAccents(char & c)
{
	if		(c == 'é' || c == 'è' || c == 'ê')	c = 'e';
	if		(c == 'à' || c == 'â')				c = 'a';
}

const string & TitleLabelBase::PrepareStr(string & s) const
{
	static string::size_type pos;
	const size_t len = s.length();

	transform( s.begin(), s.end(), s.begin(), (int(*)(int))::tolower );
	int dbg = 0;

	// trim left
	pos = s.find_first_not_of(" \t\n\r");
	if (pos != string::npos) s = s.substr(pos);

	// trim right
	pos = s.find_last_not_of(" \t\n\r");
	if (pos != string::npos) s = s.substr(0, ++pos);

	for_each(s.begin(), s.end(), RemoveAccents);

	while (s[0] == 'l')
	{
		pos = s.find(_l_apos);
		if (pos == 0)
		{
			s.erase(0, 2);
			break;
		}
		pos = s.find(_le);
		if (pos == 0)
		{
			s.erase(0, 3);
			break;
		}
		pos = s.find(_la);
		if (pos == 0)
		{
			s.erase(0, 3);
			break;
		}
		pos = s.find(_les);
		if (pos == 0)
		{
			s.erase(0, 4);
			break;
		}
		break;
	}
	return s;
}

//=============================================================================	
// Foncteur de tri de libellés de supports
//=============================================================================	


bool TitleLabelSort::operator () (const char * str1, const char* str2)
{
	return PrepareStr(string(str1, strlen(str1))) < PrepareStr(string(str2, strlen(str2)));
}

bool TitleLabelSort::operator()(const SupportListElt * elt1, const SupportListElt * elt2)
{
	return PrepareStr(string(elt1->GetLabel(), strlen(elt1->GetLabel())))
		< PrepareStr(string(elt2->GetLabel(), strlen(elt2->GetLabel())));
}


//=============================================================================	
// Foncteur de comparaison de libellés de supports
//=============================================================================	

int TitleLabelCompare::operator () (const char * str1, const char* str2)
{
	//ASSERT(str1 && str2);
	static string s1, s2;
	s1 = string(str1, strlen(str1));
	s2 = string(str2, strlen(str2));
	return (strcmp(PrepareStr(s1).c_str(), PrepareStr(s2).c_str()));
}

int TitleLabelCompare::operator()(const SupportListElt * elt1, const SupportListElt * elt2)
{
	//ASSERT(elt1 && elt2);	
	static string s1, s2;
	s1 = string(elt1->GetLabel(), strlen(elt1->GetLabel()));
	s2 = string(elt2->GetLabel(), strlen(elt2->GetLabel()));
	return (strcmp(PrepareStr(s1).c_str(), PrepareStr(s2).c_str()));
}


//=============================================================================	
//=============================================================================	
//						Test d'appartenance pour supports
//=============================================================================	
//=============================================================================	

//=============================================================================	
bool IsCouplageOnTerrain(CTerrain * pTerrain, IElem * pElem)
//=============================================================================	
{
	// couplages fixes
	if (pElem->IsCouplageFixe())
	{
		bool bRet = true;
		JCouplageFixe * cplfixe = pElem->AsCouplageFixe();
		JList<IElem*> lstElem;
		cplfixe->GetFils(lstElem);
		ASSERT(lstElem.GetCount() != 0);
		for (lstElem.MoveFirst(); lstElem.IsValid(); lstElem.MoveNext())
		{
			if (!IsElemOnTerrain(pTerrain, lstElem.GetItem()))
			{
				return false;
				break;
			}
		}
		return true;		
	}

	// couplages libres
	if (pElem->IsCouplageLibre())
	{
		bool bRet = true;
		CCouplageLibre * cpllibre = pElem->AsCouplageLibre();
		IIterateur* it = cpllibre->CreateIterateur();
		CTitreCouplageLibre* tcpllibre = (CTitreCouplageLibre*)it->getHead();
		ASSERT(tcpllibre);
		while (tcpllibre)
		{
			if (!IsElemOnTerrain(pTerrain, tcpllibre))
			{
				bRet = false;
				break;
			}
			tcpllibre = (CTitreCouplageLibre*)it->getNext();
		}
		delete it;
		return bRet;
	}

	return false;
}

//=============================================================================	
bool IsElemOnTerrain(CTerrain * pTerrain, IElem * pElem)
//=============================================================================	
{
	ASSERT(pElem);

	if (pTerrain->IsLoaded())
	{
		if(pElem->IsCouplage())
			return IsCouplageOnTerrain(pTerrain, pElem);

		if (pElem->IsTitre())
			return (pTerrain->HasSupport(pElem->AsITitre()->GetIDAudience()));

		// c'est forcément une famille
		ASSERT(pElem->AsLot());
		return (IsFamilleOnTerrain(pTerrain, static_cast<CLot*>(pElem->AsLot())));
	}
	return false;
}

//=============================================================================	
// si au moins un des elements de la famille est sur le terrain spécifié,
// on considère que la famille se trouve sur ce terrain
bool IsFamilleOnTerrain(CTerrain * pTerrain, CLot * pFamille)
//=============================================================================	
{
	ASSERT(pFamille && pTerrain);

	bool bRet = false;
	IIterateur* fam_it = pFamille->CreateIterateur();
	CTitreLot* pTitreLot = (CTitreLot*)fam_it->getHead();
	while (pTitreLot)
	{
		if (IsElemOnTerrain(pTerrain, pTitreLot/*->GetID()*/))
		{
			bRet = true;
			break;
		}
		pTitreLot = (CTitreLot*)fam_it->getNext();
	}
	delete fam_it;
	return bRet;
}

//=============================================================================	
//=============================================================================	
//								SupportSelecter
//				functeur de sélection/déselection de supports
//=============================================================================	
//=============================================================================	

SupportSelecter::SupportSelecter(bool bSelect /*= true*/) : _bSelect(bSelect) {}

void SupportSelecter::operator () (SupportListElt * pElttoInsert)
{
	pElttoInsert->iSelected = _bSelect;
}
