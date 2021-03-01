//
// Fichier: JMTFR03KEYUnivers.cpp
// Auteur:  Sylvain SAMMURI
// Date:    10/09/2003
//

// on inclut les d�finitions n�cessaires
#include "JMTFR03KEYUnivers.h"

////////////////////
// les constructeurs

JMTFR03KEYUnivers::JMTFR03KEYUnivers()
{
	// on ne fait rien
}

JMTFR03KEYUnivers::JMTFR03KEYUnivers(const JMTFR03KEYUnivers & Source)
{
	// on v�rifie l'existence de la source
	if (this == &Source) throw JInvalidCall::GetInstance();

	// on recopie l'�l�ment
	m_IdUnivers = Source.m_IdUnivers;
}

////////////////////////////////////
// la fonction pour tester l'�l�ment

JBool JMTFR03KEYUnivers::IsValid() const
{
	// on renvoie la validit� de l'�l�ment
	return (m_IdUnivers.IsValid());
}

//////////////////////////////////////
// l'op�rateur pour recopier l'�l�ment

JMTFR03KEYUnivers & JMTFR03KEYUnivers::operator =(const JMTFR03KEYUnivers & Source)
{
	// on teste si on a quelque chose � faire
	if (this != &Source)
	{
		// on recopie l'�l�ment
		m_IdUnivers = Source.m_IdUnivers;
	}

	// on renvoie notre r�f�rence
	return (*this);
}

////////////////////////////////////////////
// les op�rateurs pour comparer les �l�ments

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
// la fonction pour r�initialiser l'�l�ment

JVoid JMTFR03KEYUnivers::Reset()
{
	// on r�initialise l'�l�ment
	m_IdUnivers.Reset();
}

/////////////////
// le destructeur

JMTFR03KEYUnivers::~JMTFR03KEYUnivers()
{
	// on ne fait rien
}
