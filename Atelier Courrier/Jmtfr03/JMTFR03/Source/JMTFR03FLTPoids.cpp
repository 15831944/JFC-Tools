//
// Fichier: JMTFR03FLTPoids.cpp
// Auteur:  Sylvain SAMMURI
// Date:    05/01/2004
//

// on inclut les d�finitions n�cessaires
#include "JMTFR03FLTPoids.h"

////////////////////
// les constructeurs

JMTFR03FLTPoids::JMTFR03FLTPoids()
{
	// on ne fait rien
}

JMTFR03FLTPoids::JMTFR03FLTPoids(const JMTFR03FLTPoids & Source)
{
	// on g�n�re une exception
	throw JInvalidCall::GetInstance();
}

/////////////////////////////////////////
// l'op�rateur pour recopier les �l�ments

JMTFR03FLTPoids & JMTFR03FLTPoids::operator =(const JMTFR03FLTPoids & Source)
{
	// on g�n�re une exception
	throw JInvalidCall::GetInstance();
}

//////////////////////////////////////////////////
// la fonction pour r�cup�rer le nombre d'�l�ments

JInt32 JMTFR03FLTPoids::GetCount() const
{
	// on renvoie le nombre d'�l�ments
	return (m_TBLItems.GetCount());
}

////////////////////////////////////////
// la fonction pour d�finir les �l�ments

JVoid JMTFR03FLTPoids::SetItems(const JMTFR03KEYTerrain & KEYTerrain, const JMTFR03TBLTerrains & TBLTerrains)
{
	// on cr�e les �l�ments
	JArray<JItem> TBLItems;

	// on cr�e les �l�ments temporaires
	JMap<JRef32, JBool> TmpItems;

	// on recherche les �l�ments du terrain
	TBLTerrains.MoveTo(KEYTerrain); if (!TBLTerrains.IsValid()) throw JInvalidCall::GetInstance();

	// on r�cup�re les �l�ments du terrain
	const JMTFR03DATTerrain & DATTerrain = TBLTerrains.GetItem();

	// on boucle sur tous les types de poids
	for (DATTerrain.m_TBLPoids.MoveFirst(); DATTerrain.m_TBLPoids.IsValid(); DATTerrain.m_TBLPoids.MoveNext())
	{
		// on r�cup�re le type de poids courant
		const JMTFR03KEYPoids & KEYPoids = DATTerrain.m_TBLPoids.GetKey();
		const JMTFR03DATPoids & DATPoids = DATTerrain.m_TBLPoids.GetItem();

		// on ajoute le type de poids courant
		TmpItems.MoveTo(KEYPoids.m_IdPoids); if (!TmpItems.IsValid()) TmpItems.Add(KEYPoids.m_IdPoids) = true;
	}

	// on alloue les nouveaux �l�ments
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
// la fonction pour rechercher les �l�ments

JInt32 JMTFR03FLTPoids::FindItem(JRef32 IdPoids) const
{
	// on v�rifie la validit� de l'identifiant
	if (!IdPoids.IsValid()) throw JInvalidCall::GetInstance();

	// on initialise l'indice
	JInt32 Idx = -1;

	// on initialise les bornes de recherche
	JInt32 Min = 0;
	JInt32 Max = m_TBLItems.GetCount() - 1;

	// on boucle tant qu'on a pas trouv� l'�l�ment
	while (Idx < 0 && Min <= Max)
	{
		// on calcule l'indice m�dian
		JInt32 Med = ((Max - Min) / 2) + Min;

		// on r�cup�re l'identifiant de l'�l�ment
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
// les fonctions pour manipuler la s�lection

JVoid JMTFR03FLTPoids::SetSelItem(JInt32 Index)
{
	// on modifie la s�lection de l'�l�ment
	if (Index != -1) m_TBLItems.Item(Index).m_Etat.SetAt(0, true);

	// on r�cup�re le nombre d'�l�ments
	JInt32 Count = m_TBLItems.GetCount();

	// on r�initialise la s�lection de tous les autres �l�ments
	for (JInt32 Idx = 0; Idx < Count; Idx += 1) if (Idx != Index) m_TBLItems.Item(Idx).m_Etat.SetAt(0, false);
}

JInt32 JMTFR03FLTPoids::GetSelItem() const
{
	// on r�cup�re le nombre d'�l�ments
	JInt32 Count = m_TBLItems.GetCount();

	// on teste la s�lection de tous les �l�ments
	for (JInt32 Idx = 0; Idx < Count; Idx += 1) if (m_TBLItems.Item(Idx).m_Etat.GetAt(0)) return (Idx);

	// on n'a trouv� aucun �l�ment
	return (-1);
}

//////////////////////////////////////////
// la fonction pour r�cup�rer les �l�ments

const JRef32 & JMTFR03FLTPoids::Item(JInt32 Index) const
{
	// on renvoie l'identifiant de l'�l�ment
	return (m_TBLItems.Item(Index).m_IdPoids);
}

/////////////////////////////////////////
// la fonction pour permuter les �l�ments

JVoid JMTFR03FLTPoids::Swap(JMTFR03FLTPoids & Source)
{
	// on permute les �l�ments
	m_TBLItems.Swap(Source.m_TBLItems);
}

////////////////////////////////////////
// la fonction pour lib�rer les �l�ments

JVoid JMTFR03FLTPoids::Reset()
{
	// on lib�re les �l�ments
	m_TBLItems.Reset();
}

/////////////////
// le destructeur

JMTFR03FLTPoids::~JMTFR03FLTPoids()
{
	// on ne fait rien
}
