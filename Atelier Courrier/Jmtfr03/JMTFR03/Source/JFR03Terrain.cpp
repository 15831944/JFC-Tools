//
// Fichier: JFR03Terrain.cpp
// Auteur:  Sylvain SAMMURI
// Date:    11/03/2003
//

// on inclut les d�finitions n�cessaires
#include "JFR03Terrain.h"

////////////////////
// les constructeurs

JFR03Terrain::JFR03Terrain()
{
	// on ne fait rien
}

JFR03Terrain::JFR03Terrain(const JFR03Terrain & Source)
{
	// on v�rifie la validit� de la source
	if (this == &Source) throw JInvalidCall::GetInstance();

	// on recopie l'�l�ment
	m_IdTerrain = Source.m_IdTerrain;
	m_Libelle   = Source.m_Libelle;
	m_IdSource  = Source.m_IdSource;
	m_Racine    = Source.m_Racine;
}

///////////////////////////////////////////////////
// la fonction pour tester la validit� de l'�l�ment

JBool JFR03Terrain::IsValid() const
{
	// on renvoie la validit� de l'�l�ment
	return (m_IdTerrain.IsValid() && !m_Libelle.IsEmpty() && m_IdSource.IsValid() && m_Racine.IsFilename());
}

//////////////////////////////////////
// l'op�rateur pour recopier l'�l�ment

JFR03Terrain & JFR03Terrain::operator =(const JFR03Terrain & Source)
{
	// on teste si on a quelque chose � faire
	if (this != &Source)
	{
		// on recopie l'�l�ment
		m_IdTerrain = Source.m_IdTerrain;
		m_Libelle   = Source.m_Libelle;
		m_IdSource  = Source.m_IdSource;
		m_Racine    = Source.m_Racine;
	}

	// on renvoie notre r�f�rence
	return (*this);
}

///////////////////////////////////////////
// la fonction pour r�initialiser l'�l�ment

JVoid JFR03Terrain::Reset()
{
	// on r�initialise l'�l�ment
	m_IdTerrain.Reset();
	m_Libelle  .Reset();
	m_IdSource .Reset();
	m_Racine   .Reset();
}

/////////////////
// le destructeur

JFR03Terrain::~JFR03Terrain()
{
	// on ne fait rien
}
