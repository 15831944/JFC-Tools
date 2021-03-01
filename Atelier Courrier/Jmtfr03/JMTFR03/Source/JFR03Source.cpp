//
// Fichier: JFR03Source.cpp
// Auteur:  Sylvain SAMMURI
// Date:    11/03/2003
//

// on inclut les d�finitions n�cessaires
#include "JFR03Source.h"

////////////////////
// les constructeurs

JFR03Source::JFR03Source()
{
	// on ne fait rien
}

JFR03Source::JFR03Source(const JFR03Source & Source)
{
	// on v�rifie la validit� de la source
	if (this == &Source) throw JInvalidCall::GetInstance();

	// on recopie l'�l�ment
	m_IdSource = Source.m_IdSource;
	m_Libelle  = Source.m_Libelle;
	m_IdMedia  = Source.m_IdMedia;
}

///////////////////////////////////////////////////
// la fonction pour tester la validit� de l'�l�ment

JBool JFR03Source::IsValid() const
{
	// on renvoie la validit� de l'�l�ment
	return (m_IdSource.IsValid() && !m_Libelle.IsEmpty() && m_IdMedia.IsValid());
}

//////////////////////////////////////
// l'op�rateur pour recopier l'�l�ment

JFR03Source & JFR03Source::operator =(const JFR03Source & Source)
{
	// on teste si on a quelque chose � faire
	if (this != &Source)
	{
		// on recopie l'�l�ment
		m_IdSource = Source.m_IdSource;
		m_Libelle  = Source.m_Libelle;
		m_IdMedia  = Source.m_IdMedia;
	}

	// on renvoie notre r�f�rence
	return (*this);
}

//////////////////////////////////////////////
// la fonction pour r�initialiser les �l�ments

JVoid JFR03Source::Reset()
{
	// on r�initialise l'�l�ment
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
