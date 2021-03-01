//
// Fichier: JMTFR03FLTFiltres.cpp
// Auteur:  Sylvain SAMMURI
// Date:    05/01/2004
//

// on inclut les définitions nécessaires
#include "JMTFR03FLTFiltres.h"

////////////////////
// les constructeurs

JMTFR03FLTFiltres::JMTFR03FLTFiltres()
{
	// on ne fait rien
}

JMTFR03FLTFiltres::JMTFR03FLTFiltres(const JMTFR03FLTFiltres & Source)
{
	// on génère une exception
	throw JInvalidCall::GetInstance();
}

/////////////////////////////////////////
// l'opérateur pour recopier les éléments

JMTFR03FLTFiltres & JMTFR03FLTFiltres::operator =(const JMTFR03FLTFiltres & Source)
{
	// on génère une exception
	throw JInvalidCall::GetInstance();
}

//////////////////////////////////////////////////
// la fonction pour récupérer le nombre d'éléments

JInt32 JMTFR03FLTFiltres::GetCount() const
{
	// on renvoie le nombre d'éléments
	return (m_TBLItems.GetCount());
}

////////////////////////////////////////
// la fonction pour définir les éléments

JVoid JMTFR03FLTFiltres::SetItems(const JMTFR03KEYTerrain & KEYTerrain, const JMTFR03TBLTerrains & TBLTerrains)
{
	// on crée les éléments
	JArray<JItem> TBLItems;

	// on crée les éléments temporaires
	JMap<JRef32, JBool> TmpItems;

	// on recherche les éléments du terrain
	TBLTerrains.MoveTo(KEYTerrain); if (!TBLTerrains.IsValid()) throw JInvalidCall::GetInstance();

	// on récupère les éléments du terrain
	const JMTFR03DATTerrain & DATTerrain = TBLTerrains.GetItem();

	// on boucle sur tous les filtres d'audience
	for (DATTerrain.m_TBLFiltres.MoveFirst(); DATTerrain.m_TBLFiltres.IsValid(); DATTerrain.m_TBLFiltres.MoveNext())
	{
		// on récupère le filtre d'audience courant
		const JMTFR03KEYFiltre & KEYFiltre = DATTerrain.m_TBLFiltres.GetKey();
		const JMTFR03DATFiltre & DATFiltre = DATTerrain.m_TBLFiltres.GetItem();

		// on ajoute le filtre d'audience courant
		TmpItems.MoveTo(KEYFiltre.m_IdFiltre); if (!TmpItems.IsValid()) TmpItems.Add(KEYFiltre.m_IdFiltre) = true;
	}

	// on alloue les nouveaux éléments
	TBLItems.SetCount(TmpItems.GetCount());

	// on se place sur les premiers filtres d'audience
	JInt32 Idx = 0; TmpItems.MoveFirst();

	// on boucle sur tous les filtres d'audience
	while (TmpItems.IsValid())
	{
		// on recopie le filtre courant
		TBLItems.Item(Idx).m_IdFiltre = TmpItems.GetKey();

		// on passe aux filtres suivants
		TmpItems.MoveNext(); Idx += 1;
	}

	// on permute les éléments
	m_TBLItems.Swap(TBLItems);
}

///////////////////////////////////////////
// la fonction pour rechercher les éléments

JInt32 JMTFR03FLTFiltres::FindItem(JRef32 IdFiltre) const
{
	// on vérifie la validité de l'identifiant
	if (!IdFiltre.IsValid()) throw JInvalidCall::GetInstance();

	// on initialise l'indice
	JInt32 Idx = -1;

	// on initialise les bornes de recherche
	JInt32 Min = 0;
	JInt32 Max = m_TBLItems.GetCount() - 1;

	// on boucle tant qu'on a pas trouvé l'élément
	while (Idx < 0 && Min <= Max)
	{
		// on calcule l'indice médian
		JInt32 Med = ((Max - Min) / 2) + Min;

		// on récupère l'identifiant de l'élément
		const JRef32 & IdRef = m_TBLItems.Item(Med).m_IdFiltre;

		// on compare les identifiants
		if      (IdRef < IdFiltre) Min = Med + 1;
		else if (IdRef > IdFiltre) Max = Med - 1;
		else                       Idx = Med;
	}

	// on renvoie l'indice
	return (Idx);
}

////////////////////////////////////////////
// les fonctions pour manipuler la sélection

JVoid JMTFR03FLTFiltres::SetSelItem(JInt32 Index)
{
	// on modifie la sélection de l'élément
	if (Index != -1) m_TBLItems.Item(Index).m_Etat.SetAt(0, true);

	// on récupère le nombre d'éléments
	JInt32 Count = m_TBLItems.GetCount();

	// on réinitialise la sélection de tous les autres éléments
	for (JInt32 Idx = 0; Idx < Count; Idx += 1) if (Idx != Index) m_TBLItems.Item(Idx).m_Etat.SetAt(0, false);
}

JInt32 JMTFR03FLTFiltres::GetSelItem() const
{
	// on récupère le nombre d'éléments
	JInt32 Count = m_TBLItems.GetCount();

	// on teste la sélection de tous les éléments
	for (JInt32 Idx = 0; Idx < Count; Idx += 1) if (m_TBLItems.Item(Idx).m_Etat.GetAt(0)) return (Idx);

	// on n'a trouvé aucun élément
	return (-1);
}

//////////////////////////////////////////
// la fonction pour récupérer les éléments

const JRef32 & JMTFR03FLTFiltres::Item(JInt32 Index) const
{
	// on renvoie l'identifiant de l'élément
	return (m_TBLItems.Item(Index).m_IdFiltre);
}

/////////////////////////////////////////
// la fonction pour permuter les éléments

JVoid JMTFR03FLTFiltres::Swap(JMTFR03FLTFiltres & Source)
{
	// on permute les éléments
	m_TBLItems.Swap(Source.m_TBLItems);
}

////////////////////////////////////////
// la fonction pour libérer les éléments

JVoid JMTFR03FLTFiltres::Reset()
{
	// on libère les éléments
	m_TBLItems.Reset();
}

/////////////////
// le destructeur

JMTFR03FLTFiltres::~JMTFR03FLTFiltres()
{
	// on ne fait rien
}
