//
// Fichier: JSource.cpp
// Auteur:  Sylvain SAMMURI
// Date:    11/03/2003
//

// on inclut les définitions nécessaires
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
	// on recopie l'élément
	m_IdSource = Source.m_IdSource;
	m_IdMedia = Source.m_IdMedia;
	m_Libelle  = Source.m_Libelle;
}

////////////////////////////////////
// la fonction pour tester l'élément

JBool JSource::IsValid() const
{
	// on renvoie la validité de l'élément
	return (m_IdSource!=0 && m_IdMedia!=0 && !m_Libelle.IsEmpty());
}

//////////////////////////////////////////
// les fonctions pour sérialiser l'élément

JVoid JSource::Send(JStream & Stream) const
{
	// on sérialise l'élément
	Stream.Send(m_IdSource);
	Stream.Send(m_IdMedia);
	m_Libelle .Send(Stream);
}

JVoid JSource::Recv(JStream & Stream)
{
	// on sérialise l'élément
	JInt32 IdSource; Stream.Recv(IdSource);
	JInt32 IdMedia; Stream.Recv(IdMedia);
	JLabel Libelle;  Libelle .Recv(Stream);

	// on recopie l'élément
	m_IdSource = IdSource;
	m_IdMedia = IdMedia;
	m_Libelle  = Libelle;
}

//////////////////////////////////////
// l'opérateur pour recopier l'élément

JSource & JSource::operator =(const JSource & Source)
{
	// on recopie l'élément
	m_IdSource = Source.m_IdSource;
	m_IdMedia = Source.m_IdMedia;
	m_Libelle  = Source.m_Libelle;

	// on renvoie notre référence
	return (*this);
}

///////////////////////////////////////////
// la fonction pour réinitialiser l'élément

JVoid JSource::Reset()
{
	// on réinitialise l'élément
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
