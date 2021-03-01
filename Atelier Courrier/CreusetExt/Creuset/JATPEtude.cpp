//
// Fichier: JATPEtude.cpp
// Auteur:  Alain Chambard & Eddie Gent
// Date:    18/08/2003
//

// on inclut les d�finitions n�cessaires
#include "stdafx.h"
#include "JATPEtude.h"

////////////////////
// les constructeurs

JATPEtude::JATPEtude()
{
	// A priori non archiv�e
	m_Archive = false;
}

JATPEtude::JATPEtude(const JATPEtude & Source)
{
	// on recopie l'�l�ment
	m_IdEtude = Source.m_IdEtude;
	m_Libelle = Source.m_Libelle;
	m_Archive = Source.m_Archive; 
}

///////////////////////////////////////////////////
// la fonction pour tester la validit� de l'�l�ment

JBool JATPEtude::IsValid() const
{
	// on renvoie la validit� de l'�l�ment
	return (m_IdEtude);
}

//////////////////////////////////////////
// les fonctions pour s�rialiser l'�l�ment

JVoid JATPEtude::Send(JStream & Stream) const
{
	// S�rialise Identifiacteur Etude
	Stream.Send(m_IdEtude);

	// S�rialise Libelle Etude
	m_Libelle.Send(Stream);

	// S�rialise Flag Archivage Etude
	Stream.Send(m_Archive);
}

JVoid JATPEtude::Recv(JStream & Stream)
{
	// on s�rialise l'�l�ment
	JUnt32 IdEtude; Stream.Recv(IdEtude);
	JLabel Libelle; Libelle.Recv(Stream);
	JBool  Archive; Stream.Recv(Archive);

	// on recopie l'�l�ment
	m_IdEtude = IdEtude;
	m_Libelle = Libelle;
	m_Archive = Archive;
}

//////////////////////////////////////
// l'op�rateur pour recopier l'�l�ment

JATPEtude & JATPEtude::operator =(const JATPEtude & Source)
{
	// on recopie l'�l�ment
	m_IdEtude = Source.m_IdEtude;
	m_Libelle = Source.m_Libelle;
	m_Archive = Source.m_Archive; 

	// on renvoie notre r�f�rence
	return (*this);
}

///////////////////////////////////////////
// la fonction pour r�initialiser l'�l�ment

JVoid JATPEtude::Reset()
{
	// on r�initialise l'�l�ment
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
