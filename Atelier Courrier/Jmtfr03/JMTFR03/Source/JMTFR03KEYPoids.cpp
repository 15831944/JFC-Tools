//
// Fichier: JMTFR03KEYPoids.cpp
// Auteur:  Sylvain SAMMURI
// Date:    10/09/2003
//

// on inclut les définitions nécessaires
#include "JMTFR03KEYPoids.h"

////////////////////
// les constructeurs

JMTFR03KEYPoids::JMTFR03KEYPoids()
{
	// on ne fait rien
}

JMTFR03KEYPoids::JMTFR03KEYPoids(const JMTFR03KEYPoids & Source)
{
	// on vérifie l'existence de la source
	if (this == &Source) throw JInvalidCall::GetInstance();

	// on recopie l'élément
	m_IdPoids   = Source.m_IdPoids;
	m_IdUnivers = Source.m_IdUnivers;
}

////////////////////////////////////
// la fonction pour tester l'élément

JBool JMTFR03KEYPoids::IsValid() const
{
	// on renvoie la validité de l'élément
	return (m_IdPoids.IsValid() && m_IdUnivers.IsValid());
}

//////////////////////////////////////
// l'opérateur pour recopier l'élément

JMTFR03KEYPoids & JMTFR03KEYPoids::operator =(const JMTFR03KEYPoids & Source)
{
	// on teste si on a quelque chose à faire
	if (this != &Source)
	{
		// on recopie l'élément
		m_IdPoids   = Source.m_IdPoids;
		m_IdUnivers = Source.m_IdUnivers;
	}

	// on renvoie notre référence
	return (*this);
}

////////////////////////////////////////////
// les opérateurs pour comparer les éléments

JBool JMTFR03KEYPoids::operator <(const JMTFR03KEYPoids & Reference) const
{
	// on compare la première composante
	if (m_IdPoids < Reference.m_IdPoids) return (true);
	if (m_IdPoids > Reference.m_IdPoids) return (false);

	// on compare la dernière composante
	return (m_IdUnivers < Reference.m_IdUnivers);
}

JBool JMTFR03KEYPoids::operator >(const JMTFR03KEYPoids & Reference) const
{
	// on compare la première composante
	if (m_IdPoids > Reference.m_IdPoids) return (true);
	if (m_IdPoids < Reference.m_IdPoids) return (false);

	// on compare la dernière composante
	return (m_IdUnivers > Reference.m_IdUnivers);
}

///////////////////////////////////////////
// la fonction pour réinitialiser l'élément

JVoid JMTFR03KEYPoids::Reset()
{
	// on réinitialise l'élément
	m_IdPoids  .Reset();
	m_IdUnivers.Reset();
}

/////////////////
// le destructeur

JMTFR03KEYPoids::~JMTFR03KEYPoids()
{
	// on ne fait rien
}
