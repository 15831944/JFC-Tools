//
// Fichier: JFR03Terrain.cpp
// Auteur:  Sylvain SAMMURI
// Date:    11/03/2003
//

// on inclut les définitions nécessaires
#include "JFR03Terrain.h"

////////////////////
// les constructeurs

JFR03Terrain::JFR03Terrain()
{
	// on ne fait rien
}

JFR03Terrain::JFR03Terrain(const JFR03Terrain & Source)
{
	// on vérifie la validité de la source
	if (this == &Source) throw JInvalidCall::GetInstance();

	// on recopie l'élément
	m_IdTerrain = Source.m_IdTerrain;
	m_Libelle   = Source.m_Libelle;
	m_IdSource  = Source.m_IdSource;
	m_Racine    = Source.m_Racine;
}

///////////////////////////////////////////////////
// la fonction pour tester la validité de l'élément

JBool JFR03Terrain::IsValid() const
{
	// on renvoie la validité de l'élément
	return (m_IdTerrain.IsValid() && !m_Libelle.IsEmpty() && m_IdSource.IsValid() && m_Racine.IsFilename());
}

//////////////////////////////////////
// l'opérateur pour recopier l'élément

JFR03Terrain & JFR03Terrain::operator =(const JFR03Terrain & Source)
{
	// on teste si on a quelque chose à faire
	if (this != &Source)
	{
		// on recopie l'élément
		m_IdTerrain = Source.m_IdTerrain;
		m_Libelle   = Source.m_Libelle;
		m_IdSource  = Source.m_IdSource;
		m_Racine    = Source.m_Racine;
	}

	// on renvoie notre référence
	return (*this);
}

///////////////////////////////////////////
// la fonction pour réinitialiser l'élément

JVoid JFR03Terrain::Reset()
{
	// on réinitialise l'élément
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
