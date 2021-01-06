//
// Fichier: JMTFR03FLTSegments.cpp
// Auteur:  Sylvain SAMMURI
// Date:    05/01/2004
//

// on inclut les d�finitions n�cessaires
#include "JMTFR03FLTSegments.h"

////////////////////
// les constructeurs

JMTFR03FLTSegments::JMTFR03FLTSegments()
{
	// on ne fait rien
}

JMTFR03FLTSegments::JMTFR03FLTSegments(const JMTFR03FLTSegments & Source)
{
	// on g�n�re une exception
	throw JInvalidCall::GetInstance();
}

/////////////////////////////////////////
// l'op�rateur pour recopier les �l�ments

JMTFR03FLTSegments & JMTFR03FLTSegments::operator =(const JMTFR03FLTSegments & Source)
{
	// on g�n�re une exception
	throw JInvalidCall::GetInstance();
}

//////////////////////////////////////////////////
// la fonction pour r�cup�rer le nombre d'�l�ments

JInt32 JMTFR03FLTSegments::GetCount() const
{
	// on renvoie le nombre d'�l�ments
	return (m_TBLItems.GetCount());
}

////////////////////////////////////////
// la fonction pour d�finir les �l�ments

JVoid JMTFR03FLTSegments::SetItems(const JMTFR03KEYTerrain & KEYTerrain, const JMTFR03TBLTerrains & TBLTerrains)
{
	// on cr�e les �l�ments
	JArray<JItem> TBLItems;

	// on recherche les �l�ments du terrain
	TBLTerrains.MoveTo(KEYTerrain); if (!TBLTerrains.IsValid()) throw JInvalidCall::GetInstance();

	// on r�cup�re les �l�ments du terrain
	const JMTFR03DATTerrain & DATTerrain = TBLTerrains.GetItem();

	// on alloue les nouveaux �l�ments
	TBLItems.SetCount(DATTerrain.m_TBLSegments.GetCount());

	// on se place sur les premiers segments
	JInt32 Idx = 0; DATTerrain.m_TBLSegments.MoveFirst();

	// on boucle sur tous les segments
	while (DATTerrain.m_TBLSegments.IsValid())
	{
		// on r�cup�re le segment courant
		const JMTFR03KEYSegment & KEYSegment = DATTerrain.m_TBLSegments.GetKey();
		const JMTFR03DATSegment & DATSegment = DATTerrain.m_TBLSegments.GetItem();

		// on recopie le segment courant
		TBLItems.Item(Idx).m_IdSegment = KEYSegment.m_IdSegment;

		// on passe aux segments suivants
		DATTerrain.m_TBLSegments.MoveNext(); Idx += 1;
	}

	// on permute les �l�ments
	m_TBLItems.Swap(TBLItems);
}

///////////////////////////////////////////
// la fonction pour rechercher les �l�ments

JInt32 JMTFR03FLTSegments::FindItem(JRef32 IdSegment) const
{
	// on v�rifie la validit� de l'identifiant
	if (!IdSegment.IsValid()) throw JInvalidCall::GetInstance();

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
		const JRef32 & IdRef = m_TBLItems.Item(Med).m_IdSegment;

		// on compare les identifiants
		if      (IdRef < IdSegment) Min = Med + 1;
		else if (IdRef > IdSegment) Max = Med - 1;
		else                        Idx = Med;
	}

	// on renvoie l'indice
	return (Idx);
}

//////////////////////////////////////////////
// les fonctions pour manipuler les s�lections

JVoid JMTFR03FLTSegments::SetSelItem(JInt32 Index, JBool Selection)
{
	// on modifie la s�lection de l'�l�ment
	m_TBLItems.Item(Index).m_Etat.SetAt(0, Selection);
}

JBool JMTFR03FLTSegments::GetSelItem(JInt32 Index) const
{
	// on renvoie la s�lection de l'�l�ment
	return (m_TBLItems.Item(Index).m_Etat.GetAt(0));
}

//////////////////////////////////////////
// la fonction pour r�cup�rer les �l�ments

const JRef32 & JMTFR03FLTSegments::Item(JInt32 Index) const
{
	// on renvoie l'identifiant de l'�l�ment
	return (m_TBLItems.Item(Index).m_IdSegment);
}

/////////////////////////////////////////
// la fonction pour permuter les �l�ments

JVoid JMTFR03FLTSegments::Swap(JMTFR03FLTSegments & Source)
{
	// on permute les �l�ments
	m_TBLItems.Swap(Source.m_TBLItems);
}

////////////////////////////////////////
// la fonction pour lib�rer les �l�ments

JVoid JMTFR03FLTSegments::Reset()
{
	// on lib�re les �l�ments
	m_TBLItems.Reset();
}

/////////////////
// le destructeur

JMTFR03FLTSegments::~JMTFR03FLTSegments()
{
	// on ne fait rien
}
