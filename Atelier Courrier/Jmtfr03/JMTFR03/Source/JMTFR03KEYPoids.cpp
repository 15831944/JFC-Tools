//
// Fichier: JMTFR03KEYPoids.cpp
// Auteur:  Sylvain SAMMURI
// Date:    10/09/2003
//

// on inclut les d�finitions n�cessaires
#include "JMTFR03KEYPoids.h"

////////////////////
// les constructeurs

JMTFR03KEYPoids::JMTFR03KEYPoids()
{
	// on ne fait rien
}

JMTFR03KEYPoids::JMTFR03KEYPoids(const JMTFR03KEYPoids & Source)
{
	// on v�rifie l'existence de la source
	if (this == &Source) throw JInvalidCall::GetInstance();

	// on recopie l'�l�ment
	m_IdPoids   = Source.m_IdPoids;
	m_IdUnivers = Source.m_IdUnivers;
}

////////////////////////////////////
// la fonction pour tester l'�l�ment

JBool JMTFR03KEYPoids::IsValid() const
{
	// on renvoie la validit� de l'�l�ment
	return (m_IdPoids.IsValid() && m_IdUnivers.IsValid());
}

//////////////////////////////////////
// l'op�rateur pour recopier l'�l�ment

JMTFR03KEYPoids & JMTFR03KEYPoids::operator =(const JMTFR03KEYPoids & Source)
{
	// on teste si on a quelque chose � faire
	if (this != &Source)
	{
		// on recopie l'�l�ment
		m_IdPoids   = Source.m_IdPoids;
		m_IdUnivers = Source.m_IdUnivers;
	}

	// on renvoie notre r�f�rence
	return (*this);
}

////////////////////////////////////////////
// les op�rateurs pour comparer les �l�ments

JBool JMTFR03KEYPoids::operator <(const JMTFR03KEYPoids & Reference) const
{
	// on compare la premi�re composante
	if (m_IdPoids < Reference.m_IdPoids) return (true);
	if (m_IdPoids > Reference.m_IdPoids) return (false);

	// on compare la derni�re composante
	return (m_IdUnivers < Reference.m_IdUnivers);
}

JBool JMTFR03KEYPoids::operator >(const JMTFR03KEYPoids & Reference) const
{
	// on compare la premi�re composante
	if (m_IdPoids > Reference.m_IdPoids) return (true);
	if (m_IdPoids < Reference.m_IdPoids) return (false);

	// on compare la derni�re composante
	return (m_IdUnivers > Reference.m_IdUnivers);
}

///////////////////////////////////////////
// la fonction pour r�initialiser l'�l�ment

JVoid JMTFR03KEYPoids::Reset()
{
	// on r�initialise l'�l�ment
	m_IdPoids  .Reset();
	m_IdUnivers.Reset();
}

/////////////////
// le destructeur

JMTFR03KEYPoids::~JMTFR03KEYPoids()
{
	// on ne fait rien
}
