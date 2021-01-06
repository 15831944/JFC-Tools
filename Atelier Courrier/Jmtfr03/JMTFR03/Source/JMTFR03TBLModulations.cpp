//
// Fichier: JMTFR03TBLModulations.cpp
// Auteur:  Sylvain SAMMURI
// Date:    18/06/2004
//

// on inclut les d�finitions n�cessaires
#include "JMTFR03TBLModulations.h"

////////////////////
// les constructeurs

JMTFR03TBLModulations::JMTFR03TBLModulations()
{
	// on ne fait rien
}

JMTFR03TBLModulations::JMTFR03TBLModulations(const JMTFR03TBLModulations & Source)
{
	// on g�n�re une exception
	throw JInvalidCall::GetInstance();
}

/////////////////////////////////////////
// l'op�rateur pour recopier les �l�ments

JMTFR03TBLModulations & JMTFR03TBLModulations::operator =(const JMTFR03TBLModulations & Source)
{
	// on g�n�re une exception
	throw JInvalidCall::GetInstance();
}

//////////////////////////////////////////////////
// la fonction pour r�cup�rer le nombre d'�l�ments

JInt32 JMTFR03TBLModulations::GetCount() const
{
	// on renvoie le nombre d'�l�ments
	return (m_TBLModulations.GetCount());
}

////////////////////////////////////////
// la fonction pour ajouter les �l�ments

JMTFR03DATModulation & JMTFR03TBLModulations::Add(const JMTFR03KEYModulation & KEYModulation, const JMTFR03TBLTerrains & TBLTerrains, JBool Move)
{
	// on v�rifie la validit� de la cl�
	if (!KEYModulation.IsValid()) throw JInvalidCall::GetInstance();

	// on cr�e la cl� du terrain
	JMTFR03KEYTerrain KEYTerrain;

	// on initialise la cl� du terrain
	KEYTerrain.m_IdSource  = KEYModulation.m_IdSource;
	KEYTerrain.m_IdTerrain = KEYModulation.m_IdTerrain;

	// on cr�e l'ensemble des filtres
	JMTFR03FLTSegments FLTSegments; FLTSegments.SetItems(KEYTerrain, TBLTerrains);
	JMTFR03FLTFiltres  FLTFiltres;  FLTFiltres .SetItems(KEYTerrain, TBLTerrains);
	JMTFR03FLTPoids    FLTPoids;    FLTPoids   .SetItems(KEYTerrain, TBLTerrains);
	JMTFR03FLTTitres   FLTTitres;   FLTTitres  .SetItems(KEYTerrain, TBLTerrains);

	// on ajoute le nouvel �l�ment
	JMTFR03DATModulation & DATModulation = m_TBLModulations.Add(KEYModulation, Move);

	// on initialise l'�l�ment
	DATModulation.m_FLTSegments.Swap(FLTSegments);
	DATModulation.m_FLTFiltres .Swap(FLTFiltres);
	DATModulation.m_FLTPoids   .Swap(FLTPoids);
	DATModulation.m_FLTTitres  .Swap(FLTTitres);

	// on renvoie l'�l�ment
	return (DATModulation);
}

/////////////////////////////////////
// la fonction pour tester le curseur

JBool JMTFR03TBLModulations::IsValid() const
{
	// on renvoie l'�tat du curseur
	return (m_TBLModulations.IsValid());
}

////////////////////////////////////////////////////
// les fonctions pour r�cup�rer l'�l�ment du curseur

const JMTFR03KEYModulation & JMTFR03TBLModulations::GetKey() const
{
	// on renvoie l'�l�ment du curseur
	return (m_TBLModulations.GetKey());
}

const JMTFR03DATModulation & JMTFR03TBLModulations::GetItem() const
{
	// on renvoie l'�l�ment du curseur
	return (m_TBLModulations.GetItem());
}

/////////////////////////////////////////////////
// la fonction pour modifier l'�l�ment du curseur

JMTFR03DATModulation & JMTFR03TBLModulations::GetItem()
{
	// on renvoie l'�l�ment du curseur
	return (m_TBLModulations.GetItem());
}

/////////////////////////////////////////
// les fonctions pour d�placer le curseur

JVoid JMTFR03TBLModulations::MoveTo(const JMTFR03KEYModulation & KEYModulation, JInt32 Move) const
{
	// on v�rifie la validit� de la cl�
	if (!KEYModulation.IsValid()) throw JInvalidCall::GetInstance();

	// on d�place le curseur
	m_TBLModulations.MoveTo(KEYModulation, Move);
}

JVoid JMTFR03TBLModulations::MoveFirst() const
{
	// on d�place le curseur
	m_TBLModulations.MoveFirst();
}

JVoid JMTFR03TBLModulations::MoveNext() const
{
	// on d�place le curseur
	m_TBLModulations.MoveNext();
}

JVoid JMTFR03TBLModulations::MovePrev() const
{
	// on d�place le curseur
	m_TBLModulations.MovePrev();
}

JVoid JMTFR03TBLModulations::MoveLast() const
{
	// on d�place le curseur
	m_TBLModulations.MoveLast();
}

///////////////////////////////////////////////
// la fonction pour supprimer l'�l�ment courant

JVoid JMTFR03TBLModulations::Remove(JInt32 Move)
{
	// on supprime l'�l�ment courant
	m_TBLModulations.Remove(Move);
}

/////////////////////////////////////////
// la fonction pour permuter les �l�ments

JVoid JMTFR03TBLModulations::Swap(JMTFR03TBLModulations & Source)
{
	// on permute les �l�ments
	m_TBLModulations.Swap(Source.m_TBLModulations);
}

////////////////////////////////////////
// la fonction pour lib�rer les �l�ments

JVoid JMTFR03TBLModulations::Reset()
{
	// on r�initialise les �l�ments
	m_TBLModulations.Reset();
}

/////////////////
// le destructeur

JMTFR03TBLModulations::~JMTFR03TBLModulations()
{
	// on ne fait rien
}
