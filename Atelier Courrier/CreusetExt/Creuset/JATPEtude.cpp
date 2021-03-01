//
// Fichier: JATPEtude.cpp
// Auteur:  Alain Chambard & Eddie Gent
// Date:    18/08/2003
//

// on inclut les définitions nécessaires
#include "stdafx.h"
#include "JATPEtude.h"

////////////////////
// les constructeurs

JATPEtude::JATPEtude()
{
	// A priori non archivée
	m_Archive = false;
}

JATPEtude::JATPEtude(const JATPEtude & Source)
{
	// on recopie l'élément
	m_IdEtude = Source.m_IdEtude;
	m_Libelle = Source.m_Libelle;
	m_Archive = Source.m_Archive; 
}

///////////////////////////////////////////////////
// la fonction pour tester la validité de l'élément

JBool JATPEtude::IsValid() const
{
	// on renvoie la validité de l'élément
	return (m_IdEtude);
}

//////////////////////////////////////////
// les fonctions pour sérialiser l'élément

JVoid JATPEtude::Send(JStream & Stream) const
{
	// Sérialise Identifiacteur Etude
	Stream.Send(m_IdEtude);

	// Sérialise Libelle Etude
	m_Libelle.Send(Stream);

	// Sérialise Flag Archivage Etude
	Stream.Send(m_Archive);
}

JVoid JATPEtude::Recv(JStream & Stream)
{
	// on sérialise l'élément
	JUnt32 IdEtude; Stream.Recv(IdEtude);
	JLabel Libelle; Libelle.Recv(Stream);
	JBool  Archive; Stream.Recv(Archive);

	// on recopie l'élément
	m_IdEtude = IdEtude;
	m_Libelle = Libelle;
	m_Archive = Archive;
}

//////////////////////////////////////
// l'opérateur pour recopier l'élément

JATPEtude & JATPEtude::operator =(const JATPEtude & Source)
{
	// on recopie l'élément
	m_IdEtude = Source.m_IdEtude;
	m_Libelle = Source.m_Libelle;
	m_Archive = Source.m_Archive; 

	// on renvoie notre référence
	return (*this);
}

///////////////////////////////////////////
// la fonction pour réinitialiser l'élément

JVoid JATPEtude::Reset()
{
	// on réinitialise l'élément
	m_IdEtude = 0;
	m_Libelle.Reset();
	m_Archive = false;
}

/////////////////
// le destructeur

JATPEtude::~JATPEtude()
{
	// on ne fait rien
}
