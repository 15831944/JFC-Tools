//
// description : Classe de gestion de la sélection de l'offre (interne à l'offre)
//
// auteur : Aurélien Rainone
//

#ifndef OFFRE2_SELECTION_H
#define OFFRE2_SELECTION_H

//=============================================================================	
#include <list>
#include <set>

//=============================================================================	
// Elements de la sélection de supports (source, terrains, segments, types de poids, supports)
//=============================================================================	
struct CSupportsSelection
{
	CSupportsSelection();

	std::list<JUnt32>	SelectedTerrains;	// terrains sélectionnés
	std::list<JUnt32>	SelectedSegments;	// segments sélectionnés
	std::list<JUnt32>	SelectedSupports;	// supports sélectionnés
	JUnt32				IdTypePoids;		// types de poids sélectionnés
	JUnt32				IdTerrainCiblage;	// terrain de ciblage
	JUnt32				IdSource;			// source sélectionnée

	void Reset();

	// définition du terrain de ciblage
	void SetTerrainCiblage();
};

class IItem;

//=============================================================================	
// Elements de la sélection de cibles (items, palettes)
//=============================================================================	
struct CCiblesSelection
{
	CCiblesSelection();

	std::list<const IItem *>	SelectedTargets;	// cibles sélectionnés
	JUnt32						ReferenceAffinite;	// la reference affinite

	void Reset();
};

#endif	// !OFFRE2_SELECTION_H