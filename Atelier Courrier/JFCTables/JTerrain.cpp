//
// Fichier: JTerrain.cpp
// Auteur:  Sylvain SAMMURI
// Date:    11/03/2003
//

// on inclut les définitions nécessaires
#include "stdafx.h"
#include "JTerrain.h"

////////////////////
// les constructeurs

JTerrain::JTerrain()
{
	// uniquement initilasation des seuils d'interdiction et d'avertissement
	m_SeuilRedhibitoire  = 0;
	m_SeuilAvertissement = 0;

}

JTerrain::JTerrain(const JTerrain & Source)
{
	// on recopie l'élément
	m_IdTerrain			 = Source.m_IdTerrain;
	m_Libelle			 = Source.m_Libelle;
	m_IdSource			 = Source.m_IdSource;
	m_Chemin			 = Source.m_Chemin;
	m_SeuilRedhibitoire  = Source.m_SeuilRedhibitoire;
	m_SeuilAvertissement = Source.m_SeuilAvertissement;
}	



////////////////////////////////////
// la fonction pour tester l'élément

JBool JTerrain::IsValid() const
{
	// on renvoie la validité du terrain
	// return (m_IdTerrain!=0 && !m_Libelle.IsEmpty() && m_IdSource!=0 && !m_Chemin.IsEmpty());

	// désormais en + on teste la cohérence des seuils
	return (m_IdTerrain!=0 && !m_Libelle.IsEmpty() && m_IdSource!=0 && !m_Chemin.IsEmpty() && m_SeuilRedhibitoire <= m_SeuilAvertissement);
}

//////////////////////////////////////////
// les fonctions pour sérialiser l'élément

JVoid JTerrain::Send(JStream & Stream) const
{
	// on sérialise l'élément
	Stream.Send(m_IdTerrain);
	m_Libelle  .Send(Stream);
	Stream.Send(m_IdSource);
	m_Chemin   .Send(Stream);

	// sérialisation des seuils
	Stream.Send(m_SeuilRedhibitoire);
	Stream.Send(m_SeuilAvertissement);
}

JVoid JTerrain::Recv(JStream & Stream)
{
	// on sérialise l'élément
	JInt32 IdTerrain;			Stream.Recv(IdTerrain);
	JLabel Libelle;				Libelle.Recv(Stream);
	JInt32 IdSource;			Stream.Recv(IdSource);
	JLabel Chemin;				Chemin.Recv(Stream);

	// sérialisation des seuils
	JInt32 SeuilRedhibitoire;	Stream.Recv(SeuilRedhibitoire);
	JInt32 SeuilAvertissement;	Stream.Recv(SeuilAvertissement);

	// on recopie l'élément
	m_IdTerrain				= IdTerrain;
	m_Libelle				= Libelle;
	m_IdSource				= IdSource;
	m_Chemin				= Chemin;
	m_SeuilRedhibitoire		= SeuilRedhibitoire;
	m_SeuilAvertissement	= SeuilAvertissement;

}

////////////////////////////
// l'opérateur d'affectation

JTerrain & JTerrain::operator =(const JTerrain & Source)
{
	// on recopie l'élément
	m_IdTerrain				= Source.m_IdTerrain;
	m_Libelle				= Source.m_Libelle;
	m_IdSource				= Source.m_IdSource;
	m_Chemin				= Source.m_Chemin;
	m_SeuilRedhibitoire		= Source.m_SeuilRedhibitoire; 
	m_SeuilAvertissement	= Source.m_SeuilAvertissement;
	
	// on renvoie notre référence
	return (*this);
}

//////////////////////////////////////////////
// la fonction pour réinitialiser les éléments

JVoid JTerrain::Reset()
{
	// on réinitialise les éléments
	m_IdTerrain = 0;
	m_Libelle  .Reset();
	m_IdSource = 0;
	m_Chemin   .Reset();

	// Init seuils
	m_SeuilRedhibitoire		= 0; 
	m_SeuilAvertissement	= 0; 
}

/////////////////
// le destructeur

JTerrain::~JTerrain()
{
	// on ne fait rien
}
