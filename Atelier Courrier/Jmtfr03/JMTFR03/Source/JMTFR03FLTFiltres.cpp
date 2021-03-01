//
// Fichier: JMTFR03FLTFiltres.cpp
// Auteur:  Sylvain SAMMURI
// Date:    05/01/2004
//

// on inclut les d�finitions n�cessaires
#include "JMTFR03FLTFiltres.h"

////////////////////
// les constructeurs

JMTFR03FLTFiltres::JMTFR03FLTFiltres()
{
	// on ne fait rien
}

JMTFR03FLTFiltres::JMTFR03FLTFiltres(const JMTFR03FLTFiltres & Source)
{
	// on g�n�re une exception
	throw JInvalidCall::GetInstance();
}

/////////////////////////////////////////
// l'op�rateur pour recopier les �l�ments

JMTFR03FLTFiltres & JMTFR03FLTFiltres::operator =(const JMTFR03FLTFiltres & Source)
{
	// on g�n�re une exception
	throw JInvalidCall::GetInstance();
}

//////////////////////////////////////////////////
// la fonction pour r�cup�rer le nombre d'�l�ments

JInt32 JMTFR03FLTFiltres::GetCount() const
{
	// on renvoie le nombre d'�l�ments
	return (m_TBLItems.GetCount());
}

////////////////////////////////////////
// la fonction pour d�finir les �l�ments

JVoid JMTFR03FLTFiltres::SetItems(const JMTFR03KEYTerrain & KEYTerrain, const JMTFR03TBLTerrains & TBLTerrains)
{
	// on cr�e les �l�ments
	JArray<JItem> TBLItems;

	// on cr�e les �l�ments temporaires
	JMap<JRef32, JBool> TmpItems;

	// on recherche les �l�ments du terrain
	TBLTerrains.MoveTo(KEYTerrain); if (!TBLTerrains.IsValid()) throw JInvalidCall::GetInstance();

	// on r�cup�re les �l�ments du terrain
	const JMTFR03DATTerrain & DATTerrain = TBLTerrains.GetItem();

	// on boucle sur tous les filtres d'audience
	for (DATTerrain.m_TBLFiltres.MoveFirst(); DATTerrain.m_TBLFiltres.IsValid(); DATTerrain.m_TBLFiltres.MoveNext())
	{
		// on r�cup�re le filtre d'audience courant
		const JMTFR03KEYFiltre & KEYFiltre = DATTerrain.m_TBLFiltres.GetKey();
		const JMTFR03DATFiltre & DATFiltre = DATTerrain.m_TBLFiltres.GetItem();

		// on ajoute le filtre d'audience courant
		TmpItems.MoveTo(KEYFiltre.m_IdFiltre); if (!TmpItems.IsValid()) TmpItems.Add(KEYFiltre.m_IdFiltre) = true;
	}

	// on alloue les nouveaux �l�ments
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

	// on permute les �l�ments
	m_TBLItems.Swap(TBLItems);
}

///////////////////////////////////////////
// la fonction pour rechercher les �l�ments

JInt32 JMTFR03FLTFiltres::FindItem(JRef32 IdFiltre) const
{
	// on v�rifie la validit� de l'identifiant
	if (!IdFiltre.IsValid()) throw JInvalidCall::GetInstance();

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
// les fonctions pour manipuler la s�lection

JVoid JMTFR03FLTFiltres::SetSelItem(JInt32 Index)
{
	// on modifie la s�lection de l'�l�ment
	if (Index != -1) m_TBLItems.Item(Index).m_Etat.SetAt(0, true);

	// on r�cup�re le nombre d'�l�ments
	JInt32 Count = m_TBLItems.GetCount();

	// on r�initialise la s�lection de tous les autres �l�ments
	for (JInt32 Idx = 0; Idx < Count; Idx += 1) if (Idx != Index) m_TBLItems.Item(Idx).m_Etat.SetAt(0, false);
}

JInt32 JMTFR03FLTFiltres::GetSelItem() const
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

const JRef32 & JMTFR03FLTFiltres::Item(JInt32 Index) const
{
	// on renvoie l'identifiant de l'�l�ment
	return (m_TBLItems.Item(Index).m_IdFiltre);
}

/////////////////////////////////////////
// la fonction pour permuter les �l�ments

JVoid JMTFR03FLTFiltres::Swap(JMTFR03FLTFiltres & Source)
{
	// on permute les �l�ments
	m_TBLItems.Swap(Source.m_TBLItems);
}

////////////////////////////////////////
// la fonction pour lib�rer les �l�ments

JVoid JMTFR03FLTFiltres::Reset()
{
	// on lib�re les �l�ments
	m_TBLItems.Reset();
}

/////////////////
// le destructeur

JMTFR03FLTFiltres::~JMTFR03FLTFiltres()
{
	// on ne fait rien
}
