//
// Fichier: JFR03Source.cpp
// Auteur:  Sylvain SAMMURI
// Date:    11/03/2003
//

// on inclut les définitions nécessaires
#include "JFR03Source.h"

////////////////////
// les constructeurs

JFR03Source::JFR03Source()
{
	// on ne fait rien
}

JFR03Source::JFR03Source(const JFR03Source & Source)
{
	// on vérifie la validité de la source
	if (this == &Source) throw JInvalidCall::GetInstance();

	// on recopie l'élément
	m_IdSource = Source.m_IdSource;
	m_Libelle  = Source.m_Libelle;
	m_IdMedia  = Source.m_IdMedia;
}

///////////////////////////////////////////////////
// la fonction pour tester la validité de l'élément

JBool JFR03Source::IsValid() const
{
	// on renvoie la validité de l'élément
	return (m_IdSource.IsValid() && !m_Libelle.IsEmpty() && m_IdMedia.IsValid());
}

//////////////////////////////////////
// l'opérateur pour recopier l'élément

JFR03Source & JFR03Source::operator =(const JFR03Source & Source)
{
	// on teste si on a quelque chose à faire
	if (this != &Source)
	{
		// on recopie l'élément
		m_IdSource = Source.m_IdSource;
		m_Libelle  = Source.m_Libelle;
		m_IdMedia  = Source.m_IdMedia;
	}

	// on renvoie notre référence
	return (*this);
}

//////////////////////////////////////////////
// la fonction pour réinitialiser les éléments

JVoid JFR03Source::Reset()
{
	// on réinitialise l'élément
	m_IdSource.Reset();
	m_Libelle .Reset();
	m_IdMedia .Reset();
}

/////////////////
// le destructeur

JFR03Source::~JFR03Source()
{
	// on ne fait rien
}
