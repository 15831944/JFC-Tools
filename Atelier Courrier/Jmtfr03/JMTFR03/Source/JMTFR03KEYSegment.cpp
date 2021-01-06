//
// Fichier: JMTFR03KEYSegment.cpp
// Auteur:  Sylvain SAMMURI
// Date:    10/09/2003
//

// on inclut les d�finitions n�cessaires
#include "JMTFR03KEYSegment.h"

////////////////////
// les constructeurs

JMTFR03KEYSegment::JMTFR03KEYSegment()
{
	// on ne fait rien
}

JMTFR03KEYSegment::JMTFR03KEYSegment(const JMTFR03KEYSegment & Source)
{
	// on v�rifie l'existence de la source
	if (this == &Source) throw JInvalidCall::GetInstance();

	// on recopie l'�l�ment
	m_IdSegment = Source.m_IdSegment;
}

////////////////////////////////////
// la fonction pour tester l'�l�ment

JBool JMTFR03KEYSegment::IsValid() const
{
	// on renvoie la validit� de l'�l�ment
	return (m_IdSegment.IsValid());
}

//////////////////////////////////////
// l'op�rateur pour recopier l'�l�ment

JMTFR03KEYSegment & JMTFR03KEYSegment::operator =(const JMTFR03KEYSegment & Source)
{
	// on teste si on a quelque chose � faire
	if (this != &Source)
	{
		// on recopie l'�l�ment
		m_IdSegment = Source.m_IdSegment;
	}

	// on renvoie notre r�f�rence
	return (*this);
}

////////////////////////////////////////////
// les op�rateurs pour comparer les �l�ments

JBool JMTFR03KEYSegment::operator <(const JMTFR03KEYSegment & Reference) const
{
	// on compare la seule composante
	return (m_IdSegment < Reference.m_IdSegment);
}

JBool JMTFR03KEYSegment::operator >(const JMTFR03KEYSegment & Reference) const
{
	// on compare la seule composante
	return (m_IdSegment > Reference.m_IdSegment);
}

///////////////////////////////////////////
// la fonction pour r�initialiser l'�l�ment

JVoid JMTFR03KEYSegment::Reset()
{
	// on r�initialise l'�l�ment
	m_IdSegment.Reset();
}

/////////////////
// le destructeur

JMTFR03KEYSegment::~JMTFR03KEYSegment()
{
	// on ne fait rien
}
