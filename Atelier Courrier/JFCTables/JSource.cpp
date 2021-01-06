//
// Fichier: JSource.cpp
// Auteur:  Sylvain SAMMURI
// Date:    11/03/2003
//

// on inclut les d�finitions n�cessaires
#include "stdafx.h"
#include "JSource.h"

////////////////////
// les constructeurs

JSource::JSource()
{
	// on ne fait rien
}

JSource::JSource(const JSource & Source)
{
	// on recopie l'�l�ment
	m_IdSource = Source.m_IdSource;
	m_IdMedia = Source.m_IdMedia;
	m_Libelle  = Source.m_Libelle;
}

////////////////////////////////////
// la fonction pour tester l'�l�ment

JBool JSource::IsValid() const
{
	// on renvoie la validit� de l'�l�ment
	return (m_IdSource!=0 && m_IdMedia!=0 && !m_Libelle.IsEmpty());
}

//////////////////////////////////////////
// les fonctions pour s�rialiser l'�l�ment

JVoid JSource::Send(JStream & Stream) const
{
	// on s�rialise l'�l�ment
	Stream.Send(m_IdSource);
	Stream.Send(m_IdMedia);
	m_Libelle .Send(Stream);
}

JVoid JSource::Recv(JStream & Stream)
{
	// on s�rialise l'�l�ment
	JInt32 IdSource; Stream.Recv(IdSource);
	JInt32 IdMedia; Stream.Recv(IdMedia);
	JLabel Libelle;  Libelle .Recv(Stream);

	// on recopie l'�l�ment
	m_IdSource = IdSource;
	m_IdMedia = IdMedia;
	m_Libelle  = Libelle;
}

//////////////////////////////////////
// l'op�rateur pour recopier l'�l�ment

JSource & JSource::operator =(const JSource & Source)
{
	// on recopie l'�l�ment
	m_IdSource = Source.m_IdSource;
	m_IdMedia = Source.m_IdMedia;
	m_Libelle  = Source.m_Libelle;

	// on renvoie notre r�f�rence
	return (*this);
}

///////////////////////////////////////////
// la fonction pour r�initialiser l'�l�ment

JVoid JSource::Reset()
{
	// on r�initialise l'�l�ment
	//m_IdSource.Reset();
	m_IdSource = 0;
	m_IdMedia = 0;
	m_Libelle .Reset();
}

/////////////////
// le destructeur

JSource::~JSource()
{
	// on ne fait rien
}
