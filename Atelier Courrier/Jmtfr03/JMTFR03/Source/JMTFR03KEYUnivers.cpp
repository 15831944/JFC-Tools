//
// Fichier: JMTFR03KEYUnivers.cpp
// Auteur:  Sylvain SAMMURI
// Date:    10/09/2003
//

// on inclut les définitions nécessaires
#include "JMTFR03KEYUnivers.h"

////////////////////
// les constructeurs

JMTFR03KEYUnivers::JMTFR03KEYUnivers()
{
	// on ne fait rien
}

JMTFR03KEYUnivers::JMTFR03KEYUnivers(const JMTFR03KEYUnivers & Source)
{
	// on vérifie l'existence de la source
	if (this == &Source) throw JInvalidCall::GetInstance();

	// on recopie l'élément
	m_IdUnivers = Source.m_IdUnivers;
}

////////////////////////////////////
// la fonction pour tester l'élément

JBool JMTFR03KEYUnivers::IsValid() const
{
	// on renvoie la validité de l'élément
	return (m_IdUnivers.IsValid());
}

//////////////////////////////////////
// l'opérateur pour recopier l'élément

JMTFR03KEYUnivers & JMTFR03KEYUnivers::operator =(const JMTFR03KEYUnivers & Source)
{
	// on teste si on a quelque chose à faire
	if (this != &Source)
	{
		// on recopie l'élément
		m_IdUnivers = Source.m_IdUnivers;
	}

	// on renvoie notre référence
	return (*this);
}

////////////////////////////////////////////
// les opérateurs pour comparer les éléments

JBool JMTFR03KEYUnivers::operator <(const JMTFR03KEYUnivers & Reference) const
{
	// on compare la seule composante
	return (m_IdUnivers < Reference.m_IdUnivers);
}

JBool JMTFR03KEYUnivers::operator >(const JMTFR03KEYUnivers & Reference) const
{
	// on compare la seule composante
	return (m_IdUnivers > Reference.m_IdUnivers);
}

///////////////////////////////////////////
// la fonction pour réinitialiser l'élément

JVoid JMTFR03KEYUnivers::Reset()
{
	// on réinitialise l'élément
	m_IdUnivers.Reset();
}

/////////////////
// le destructeur

JMTFR03KEYUnivers::~JMTFR03KEYUnivers()
{
	// on ne fait rien
}
