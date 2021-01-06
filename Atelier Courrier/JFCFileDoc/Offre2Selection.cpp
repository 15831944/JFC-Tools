//
// description :	Classe de gestion de la sélection de l'offre
//
// auteur : Aurélien Rainone
//

#include "StdAfx.h"
#include "Offre2Selection.h"
#include "SupportList.h"
#include "Offre2Supports.h"
#include "JFCZipIniFile.h"
#include <sstream>

using namespace std;

//=============================================================================	
// Elements de la sélection de supports (source, terrains, segments, types de poids, supports)
//=============================================================================	

CSupportsSelection::CSupportsSelection() : IdTypePoids(0), IdTerrainCiblage(0), IdSource(0) {}

void CSupportsSelection::Reset()
{
	SelectedTerrains.clear();
	SelectedSegments.clear();
	SelectedSupports.clear();

	IdTypePoids			= 0;
	IdTerrainCiblage	= 0;
	IdSource			= 0;
}

// définition du terrain de ciblage
void CSupportsSelection::SetTerrainCiblage()
{
	// on set le terrain de ciblage (pour savoir sur quel segment on travaille)
	list<JUnt32>::const_iterator id_it = max_element(SelectedTerrains.begin(), SelectedTerrains.end());
	if (id_it != SelectedTerrains.end())
		IdTerrainCiblage = *id_it;
	else
		IdTerrainCiblage = 0;
}

#include "IItem.h"

//=============================================================================	
// Elements de la sélection de cibles (items, palettes)
//=============================================================================	
CCiblesSelection::CCiblesSelection()
{
	ReferenceAffinite = 0;
}

void CCiblesSelection::Reset()
{
	// on efface tous les id
	SelectedTargets.clear();
	ReferenceAffinite = 0;
}