//
// description :	Différents Foncteur de tri de SupportListElt (structure spéciale pour la
//					lisgbox d'affichage et de sélection des supports
//
// auteur : Aurélien Rainone
//

#ifndef SUPPORTLIST_FUNCTORS_H
#define SUPPORTLIST_FUNCTORS_H

#include "KeyTerrain.h"
#include "AllTypeOfNode.h"

#include <functional>
#include <algorithm>
#include <string>
#include <list>
#include <map>

class SupportListElt;
class CSupportFilter;
class COffreTerrainManager;

// types utiles pour la gestion des titres appariés
//typedef std::multimap<JUnt32, CTitresApparies*>			MapAppariesByTerrains;
//typedef std::multimap<JUnt32, MapAppariesByTerrains>	MapAppariesBySourceTerrains;

//=============================================================================	
// Foncteur de base pour les comparaisons et tris de libellés de supports
struct TitleLabelBase
//=============================================================================	
{
protected:

	static void RemoveAccents(char & c);

	const std::string & PrepareStr(std::string & s) const;

private:

	static const std::string _l_apos, _le, _la, _les;
};

//=============================================================================	
// Foncteur de tri de libellés de supports
struct TitleLabelSort : TitleLabelBase,  std::binary_function<const char *, const char *, bool>,
										 std::binary_function<const SupportListElt *, const SupportListElt *, bool>
//=============================================================================	
{
	virtual bool operator()	(const char * str1,				const char* str2);
	virtual bool operator()	(const SupportListElt * elt1,	const SupportListElt * elt2);
};

//=============================================================================	
// Foncteur de comparaison de libellés de supports
struct TitleLabelCompare : TitleLabelBase,  std::binary_function<const char *, const char *, int>,
											std::binary_function<const SupportListElt *, const SupportListElt *, int>
//=============================================================================	
{
	virtual int operator()	(const char * str1,				const char* str2);
	virtual int operator()	(const SupportListElt * elt1,	const SupportListElt * elt2);
};

//=============================================================================	
// Interface d'un foncteur permettant de filtrer des supports
//=============================================================================	
struct ISupportFilterFunc :  public std::unary_function<SupportListElt*, bool>
{
	ISupportFilterFunc() {}

	virtual bool operator()(SupportListElt * elt) = 0;
};

//=============================================================================	
// Permet de composer des filtres de supports, basés sur des functeurs de type ISupportFilterFunc
struct SupportFilterFuncComposer : public std::unary_function<SupportListElt*, bool>
//=============================================================================	
{
	bool operator()(SupportListElt * elt) const
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
					if (!(*_func_it->first)(elt))
						return false;
				}
				else
				{
					// operator NAND
					if ((*_func_it->first)(elt))
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
	void AddFilter (ISupportFilterFunc * functor, Operator op = AND)
	{
		_functors.insert(std::make_pair(functor, op));	
	}

	// libère les filtres contenus (et appelle leur destructeur repspectif)
	void ReleaseFilters()
	{
		FunctorIterator it = _functors.begin();
		while(it != _functors.end())
		{
			ISupportFilterFunc * pfo = it->first;
			if (pfo) delete pfo; pfo = 0;
			++it;
		}
		_functors.clear();
	}

protected:

	typedef std::map<ISupportFilterFunc*, Operator>	FunctorMap;
	typedef FunctorMap::const_iterator	ConstFunctorIterator;
	typedef FunctorMap::iterator		FunctorIterator;
	FunctorMap							_functors;
	mutable ConstFunctorIterator		_func_it;
};

//=============================================================================	
// Renvoie true si le support se trouve sur au moins 1 des terrains de la liste
// renvoie true egalement si il n'y pas de terrains dans le container
struct IsSupportOnTerrains : public ISupportFilterFunc
//=============================================================================	
{
	IsSupportOnTerrains(const std::list<JUnt32> & terrains, const JUnt32 idSource/*, const MapAppariesBySourceTerrains & allApparies*/);

	virtual bool operator()(SupportListElt * elt);

protected:
	const std::list<JUnt32> &			_terrains;
	std::list<JUnt32>::const_iterator	_ter_id;
	COffreTerrainManager	*			_offreterMgr;
	CKeyTerrain							_KeyTerrain;
	//const MapAppariesBySourceTerrains & _apparies;
};

//=============================================================================	
// Renvoie true si le support se trouve sur au moins 1 des segemnts de la liste
// renvoie true egalement si il n'y pas de segments, ou de terrains dans le container
struct IsSupportOnSegments : public ISupportFilterFunc
//=============================================================================	
{
	IsSupportOnSegments(const std::list<JUnt32> & terrains, const std::list<JUnt32> & segments, const JUnt32 idSource);

	virtual bool operator()(SupportListElt * elt);

protected:
	const std::list<JUnt32> &			_terrains;
	const std::list<JUnt32> &			_segments;
	std::list<JUnt32>::const_iterator	_ter_id;
	std::list<JUnt32>::const_iterator	_seg_id;
	COffreTerrainManager	*			_offreterMgr;
	CKeyTerrain							_KeyTerrain;
};


//=============================================================================	
// Renvoie true si le libellé du support contient la chaine passée au constructeur
// NB : une chaine vide renvoie true
struct IsSupportLabelContains : public ISupportFilterFunc, TitleLabelBase
//=============================================================================	
{
	IsSupportLabelContains(const char * strToFind);
	~IsSupportLabelContains();

	virtual bool operator()(SupportListElt * elt);

protected:
	char *			_strToFind;
	TitleLabelSort	_titleLabelSort;
};

//=============================================================================	
// Renvoie true si la périodicité du support est une de celles passées dans le container C
// renvoie true egalement si il n'y pas de periodicites dans le container
template <class C>
struct IsSupportPeriodicityIsOneOf : public ISupportFilterFunc
//=============================================================================	
{
	IsSupportPeriodicityIsOneOf(const C & periodicites)
		:	_periodicites(periodicites) {}

	virtual bool operator()(SupportListElt * elt)
	{
		// pas de périodicité à filtrer, on affiche donc ts ls supports
		if (_periodicites.empty()) return true;

		// pas de périodicité pour un couplage libre, ni pour une famille
		if (elt->bIsCouplageLibre || elt->bIsFamille) return true;

		return (find(_periodicites.begin(), _periodicites.end(),
			elt->pElem->AsITitre()->GetPeriodicite())
			!= _periodicites.end());
	}

protected:
	const C &	_periodicites;
};

/*

//=============================================================================	
// Renvoie true si la périodicité du support est une de celles passées dans le container C
// renvoie true egalement si il n'y pas de periodicites dans le container
template <class C>
struct IsSupportPeriodicityIsOneOf : public ISupportFilterFunc
//=============================================================================	
{
IsSupportPeriodicityIsOneOf(const C & periodicites, bool negate_ = false)
:	_periodicites(periodicites)
,	_negate(negate_) {}

virtual bool operator()(SupportListElt * elt)
{
// pas de périodicité à filtrer, on affiche donc ts ls supports
if (_periodicites.empty())return (_negate)?!false:true;

// TODO, faut-il tester la périodicité des familles comme celle de la somme des périodicités des titres qui la composentdevant être 
if (elt->bIsFamille) return (_negate)?!false:true;

static bool bRet;
bRet = (find(_periodicites.begin(), _periodicites.end(),
elt->pElem->AsITitre()->GetPeriodicite())
!= _periodicites.end());
return (_negate)?!bRet:bRet;
}

protected:
const C &	_periodicites;
const bool	_negate;
};


*/
//=============================================================================	
// Renvoie true si l'id du support est une de celles passées dans le container C
template <class C>
struct IsSupportIdIsOneOf : public ISupportFilterFunc
//=============================================================================	
{
	IsSupportIdIsOneOf(const C & ids) : _ids(ids){}

	virtual bool operator()(SupportListElt * elt)
	{
		return (_ids.end() != std::find(_ids.begin(), _ids.end(), elt->Id));
	}

protected:
	typedef typename C::const_iterator const_iterator;
	const C &				_ids;
};


//=============================================================================	
// Renvoie true si le type du support est un des types contenus dans l'entier 32 bits
// passé au constructeur et qui est une compoisiton par OU BIT A BIT (Bitwise-OR)
// de une ou plusieurs constantes
// (@see CSupportFilter::DISPLAY_TITRES, DISPLAY_COUPLAGES, DISPLAY_FAMILLES)
struct IsSupportTypeOneOf : public ISupportFilterFunc
//=============================================================================	
{
	IsSupportTypeOneOf(const int types, bool negate_ = false);

	virtual bool operator()(SupportListElt * elt);

protected:
	const int	_types;
	const bool	_negate;
};


//=============================================================================	
// Renvoie true si le support est sélectionné
struct IsSupportSelected : public ISupportFilterFunc
//=============================================================================	
{
	virtual bool operator()(SupportListElt * elt);
};


//=============================================================================	
// functeur de sélection/déselection de supports
struct SupportSelecter
{
	SupportSelecter(bool bSelect = true);

	void operator () (SupportListElt * pElttoInsert);

private:
	const bool _bSelect;
};



#endif //!SUPPORTLIST_FUNCTORS_H