//
// Fichier: JMTFR03TBLModulations.cpp
// Auteur:  Sylvain SAMMURI
// Date:    18/06/2004
//

// on inclut les définitions nécessaires
#include "JMTFR03TBLModulations.h"

////////////////////
// les constructeurs

JMTFR03TBLModulations::JMTFR03TBLModulations()
{
	// on ne fait rien
}

JMTFR03TBLModulations::JMTFR03TBLModulations(const JMTFR03TBLModulations & Source)
{
	// on génère une exception
	throw JInvalidCall::GetInstance();
}

/////////////////////////////////////////
// l'opérateur pour recopier les éléments

JMTFR03TBLModulations & JMTFR03TBLModulations::operator =(const JMTFR03TBLModulations & Source)
{
	// on génère une exception
	throw JInvalidCall::GetInstance();
}

//////////////////////////////////////////////////
// la fonction pour récupérer le nombre d'éléments

JInt32 JMTFR03TBLModulations::GetCount() const
{
	// on renvoie le nombre d'éléments
	return (m_TBLModulations.GetCount());
}

////////////////////////////////////////
// la fonction pour ajouter les éléments

JMTFR03DATModulation & JMTFR03TBLModulations::Add(const JMTFR03KEYModulation & KEYModulation, const JMTFR03TBLTerrains & TBLTerrains, JBool Move)
{
	// on vérifie la validité de la clé
	if (!KEYModulation.IsValid()) throw JInvalidCall::GetInstance();

	// on crée la clé du terrain
	JMTFR03KEYTerrain KEYTerrain;

	// on initialise la clé du terrain
	KEYTerrain.m_IdSource  = KEYModulation.m_IdSource;
	KEYTerrain.m_IdTerrain = KEYModulation.m_IdTerrain;

	// on crée l'ensemble des filtres
	JMTFR03FLTSegments FLTSegments; FLTSegments.SetItems(KEYTerrain, TBLTerrains);
	JMTFR03FLTFiltres  FLTFiltres;  FLTFiltres .SetItems(KEYTerrain, TBLTerrains);
	JMTFR03FLTPoids    FLTPoids;    FLTPoids   .SetItems(KEYTerrain, TBLTerrains);
	JMTFR03FLTTitres   FLTTitres;   FLTTitres  .SetItems(KEYTerrain, TBLTerrains);

	// on ajoute le nouvel élément
	JMTFR03DATModulation & DATModulation = m_TBLModulations.Add(KEYModulation, Move);

	// on initialise l'élément
	DATModulation.m_FLTSegments.Swap(FLTSegments);
	DATModulation.m_FLTFiltres .Swap(FLTFiltres);
	DATModulation.m_FLTPoids   .Swap(FLTPoids);
	DATModulation.m_FLTTitres  .Swap(FLTTitres);

	// on renvoie l'élément
	return (DATModulation);
}

/////////////////////////////////////
// la fonction pour tester le curseur

JBool JMTFR03TBLModulations::IsValid() const
{
	// on renvoie l'état du curseur
	return (m_TBLModulations.IsValid());
}

////////////////////////////////////////////////////
// les fonctions pour récupérer l'élément du curseur

const JMTFR03KEYModulation & JMTFR03TBLModulations::GetKey() const
{
	// on renvoie l'élément du curseur
	return (m_TBLModulations.GetKey());
}

const JMTFR03DATModulation & JMTFR03TBLModulations::GetItem() const
{
	// on renvoie l'élément du curseur
	return (m_TBLModulations.GetItem());
}

/////////////////////////////////////////////////
// la fonction pour modifier l'élément du curseur

JMTFR03DATModulation & JMTFR03TBLModulations::GetItem()
{
	// on renvoie l'élément du curseur
	return (m_TBLModulations.GetItem());
}

/////////////////////////////////////////
// les fonctions pour déplacer le curseur

JVoid JMTFR03TBLModulations::MoveTo(const JMTFR03KEYModulation & KEYModulation, JInt32 Move) const
{
	// on vérifie la validité de la clé
	if (!KEYModulation.IsValid()) throw JInvalidCall::GetInstance();

	// on déplace le curseur
	m_TBLModulations.MoveTo(KEYModulation, Move);
}

JVoid JMTFR03TBLModulations::MoveFirst() const
{
	// on déplace le curseur
	m_TBLModulations.MoveFirst();
}

JVoid JMTFR03TBLModulations::MoveNext() const
{
	// on déplace le curseur
	m_TBLModulations.MoveNext();
}

JVoid JMTFR03TBLModulations::MovePrev() const
{
	// on déplace le curseur
	m_TBLModulations.MovePrev();
}

JVoid JMTFR03TBLModulations::MoveLast() const
{
	// on déplace le curseur
	m_TBLModulations.MoveLast();
}

///////////////////////////////////////////////
// la fonction pour supprimer l'élément courant

JVoid JMTFR03TBLModulations::Remove(JInt32 Move)
{
	// on supprime l'élément courant
	m_TBLModulations.Remove(Move);
}

/////////////////////////////////////////
// la fonction pour permuter les éléments

JVoid JMTFR03TBLModulations::Swap(JMTFR03TBLModulations & Source)
{
	// on permute les éléments
	m_TBLModulations.Swap(Source.m_TBLModulations);
}

////////////////////////////////////////
// la fonction pour libérer les éléments

JVoid JMTFR03TBLModulations::Reset()
{
	// on réinitialise les éléments
	m_TBLModulations.Reset();
}

/////////////////
// le destructeur

JMTFR03TBLModulations::~JMTFR03TBLModulations()
{
	// on ne fait rien
}
