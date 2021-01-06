//
// description : Classe de gestion de la s�lection de l'offre (interne � l'offre)
//
// auteur : Aur�lien Rainone
//

#ifndef OFFRE2_SELECTION_H
#define OFFRE2_SELECTION_H

//=============================================================================	
#include <list>
#include <set>

//=============================================================================	
// Elements de la s�lection de supports (source, terrains, segments, types de poids, supports)
//=============================================================================	
struct CSupportsSelection
{
	CSupportsSelection();

	std::list<JUnt32>	SelectedTerrains;	// terrains s�lectionn�s
	std::list<JUnt32>	SelectedSegments;	// segments s�lectionn�s
	std::list<JUnt32>	SelectedSupports;	// supports s�lectionn�s
	JUnt32				IdTypePoids;		// types de poids s�lectionn�s
	JUnt32				IdTerrainCiblage;	// terrain de ciblage
	JUnt32				IdSource;			// source s�lectionn�e

	void Reset();

	// d�finition du terrain de ciblage
	void SetTerrainCiblage();
};

class IItem;

//=============================================================================	
// Elements de la s�lection de cibles (items, palettes)
//=============================================================================	
struct CCiblesSelection
{
	CCiblesSelection();

	std::list<const IItem *>	SelectedTargets;	// cibles s�lectionn�s
	JUnt32						ReferenceAffinite;	// la reference affinite

	void Reset();
};

#endif	// !OFFRE2_SELECTION_H