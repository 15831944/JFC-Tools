//
// Fichier: JMTFR03FLTPoids.cpp
// Auteur:  Sylvain SAMMURI
// Date:    05/01/2004
//

// on inclut les définitions nécessaires
#include "JMTFR03FLTPoids.h"

////////////////////
// les constructeurs

JMTFR03FLTPoids::JMTFR03FLTPoids()
{
	// on ne fait rien
}

JMTFR03FLTPoids::JMTFR03FLTPoids(const JMTFR03FLTPoids & Source)
{
	// on génère une exception
	throw JInvalidCall::GetInstance();
}

/////////////////////////////////////////
// l'opérateur pour recopier les éléments

JMTFR03FLTPoids & JMTFR03FLTPoids::operator =(const JMTFR03FLTPoids & Source)
{
	// on génère une exception
	throw JInvalidCall::GetInstance();
}

//////////////////////////////////////////////////
// la fonction pour récupérer le nombre d'éléments

JInt32 JMTFR03FLTPoids::GetCount() const
{
	// on renvoie le nombre d'éléments
	return (m_TBLItems.GetCount());
}

////////////////////////////////////////
// la fonction pour définir les éléments

JVoid JMTFR03FLTPoids::SetItems(const JMTFR03KEYTerrain & KEYTerrain, const JMTFR03TBLTerrains & TBLTerrains)
{
	// on crée les éléments
	JArray<JItem> TBLItems;

	// on crée les éléments temporaires
	JMap<JRef32, JBool> TmpItems;

	// on recherche les éléments du terrain
	TBLTerrains.MoveTo(KEYTerrain); if (!TBLTerrains.IsValid()) throw JInvalidCall::GetInstance();

	// on récupère les éléments du terrain
	const JMTFR03DATTerrain & DATTerrain = TBLTerrains.GetItem();

	// on boucle sur tous les types de poids
	for (DATTerrain.m_TBLPoids.MoveFirst(); DATTerrain.m_TBLPoids.IsValid(); DATTerrain.m_TBLPoids.MoveNext())
	{
		// on récupère le type de poids courant
		const JMTFR03KEYPoids & KEYPoids = DATTerrain.m_TBLPoids.GetKey();
		const JMTFR03DATPoids & DATPoids = DATTerrain.m_TBLPoids.GetItem();

		// on ajoute le type de poids courant
		TmpItems.MoveTo(KEYPoids.m_IdPoids); if (!TmpItems.IsValid()) TmpItems.Add(KEYPoids.m_IdPoids) = true;
	}

	// on alloue les nouveaux éléments
	TBLItems.SetCount(TmpItems.GetCount());

	// on se place sur les premiers poids
	JInt32 Idx = 0; TmpItems.MoveFirst();

	// on boucle sur tous les poids
	while (TmpItems.IsValid())
	{
		// on recopie le poids courant
		TBLItems.Item(Idx).m_IdPoids = TmpItems.GetKey();

		// on passe aux poids suivants
		TmpItems.MoveNext(); Idx += 1;
	}

	// on permute les poids
	m_TBLItems.Swap(TBLItems);
}

///////////////////////////////////////////
// la fonction pour rechercher les éléments

JInt32 JMTFR03FLTPoids::FindItem(JRef32 IdPoids) const
{
	// on vérifie la validité de l'identifiant
	if (!IdPoids.IsValid()) throw JInvalidCall::GetInstance();

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
		const JRef32 & IdRef = m_TBLItems.Item(Med).m_IdPoids;

		// on compare les identifiants
		if      (IdRef < IdPoids) Min = Med + 1;
		else if (IdRef > IdPoids) Max = Med - 1;
		else                      Idx = Med;
	}

	// on renvoie l'indice
	return (Idx);
}

////////////////////////////////////////////
// les fonctions pour manipuler la sélection

JVoid JMTFR03FLTPoids::SetSelItem(JInt32 Index)
{
	// on modifie la sélection de l'élément
	if (Index != -1) m_TBLItems.Item(Index).m_Etat.SetAt(0, true);

	// on récupère le nombre d'éléments
	JInt32 Count = m_TBLItems.GetCount();

	// on réinitialise la sélection de tous les autres éléments
	for (JInt32 Idx = 0; Idx < Count; Idx += 1) if (Idx != Index) m_TBLItems.Item(Idx).m_Etat.SetAt(0, false);
}

JInt32 JMTFR03FLTPoids::GetSelItem() const
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

const JRef32 & JMTFR03FLTPoids::Item(JInt32 Index) const
{
	// on renvoie l'identifiant de l'élément
	return (m_TBLItems.Item(Index).m_IdPoids);
}

/////////////////////////////////////////
// la fonction pour permuter les éléments

JVoid JMTFR03FLTPoids::Swap(JMTFR03FLTPoids & Source)
{
	// on permute les éléments
	m_TBLItems.Swap(Source.m_TBLItems);
}

////////////////////////////////////////
// la fonction pour libérer les éléments

JVoid JMTFR03FLTPoids::Reset()
{
	// on libère les éléments
	m_TBLItems.Reset();
}

/////////////////
// le destructeur

JMTFR03FLTPoids::~JMTFR03FLTPoids()
{
	// on ne fait rien
}
