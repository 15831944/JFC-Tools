//
// Fichier: JMTFR03KEYEchantillon.cpp
// Auteur:  Sylvain SAMMURI
// Date:    10/09/2003
//

// on inclut les définitions nécessaires
#include "JMTFR03KEYEchantillon.h"

////////////////////
// les constructeurs

JMTFR03KEYEchantillon::JMTFR03KEYEchantillon()
{
	// on ne fait rien
}

JMTFR03KEYEchantillon::JMTFR03KEYEchantillon(const JMTFR03KEYEchantillon & Source)
{
	// on vérifie l'existence de la source
	if (this == &Source) throw JInvalidCall::GetInstance();

	// on recopie l'élément
	m_IdEchantillon = Source.m_IdEchantillon;
}

////////////////////////////////////
// la fonction pour tester l'élément

JBool JMTFR03KEYEchantillon::IsValid() const
{
	// on renvoie la validité de l'élément
	return (m_IdEchantillon.IsValid());
}

//////////////////////////////////////
// l'opérateur pour recopier l'élément

JMTFR03KEYEchantillon & JMTFR03KEYEchantillon::operator =(const JMTFR03KEYEchantillon & Source)
{
	// on teste si on a quelque chose à faire
	if (this != &Source)
	{
		// on recopie l'élément
		m_IdEchantillon = Source.m_IdEchantillon;
	}

	// on renvoie notre référence
	return (*this);
}

////////////////////////////////////////////
// les opérateurs pour comparer les éléments

JBool JMTFR03KEYEchantillon::operator <(const JMTFR03KEYEchantillon & Reference) const
{
	// on compare la seule composante
	return (m_IdEchantillon < Reference.m_IdEchantillon);
}

JBool JMTFR03KEYEchantillon::operator >(const JMTFR03KEYEchantillon & Reference) const
{
	// on compare la seule composante
	return (m_IdEchantillon > Reference.m_IdEchantillon);
}

///////////////////////////////////////////
// la fonction pour réinitialiser l'élément

JVoid JMTFR03KEYEchantillon::Reset()
{
	// on réinitialise l'élément
	m_IdEchantillon.Reset();
}

/////////////////
// le destructeur

JMTFR03KEYEchantillon::~JMTFR03KEYEchantillon()
{
	// on ne fait rien
}
