//
// Fichier: JMTFR03KEYSegment.cpp
// Auteur:  Sylvain SAMMURI
// Date:    10/09/2003
//

// on inclut les définitions nécessaires
#include "JMTFR03KEYSegment.h"

////////////////////
// les constructeurs

JMTFR03KEYSegment::JMTFR03KEYSegment()
{
	// on ne fait rien
}

JMTFR03KEYSegment::JMTFR03KEYSegment(const JMTFR03KEYSegment & Source)
{
	// on vérifie l'existence de la source
	if (this == &Source) throw JInvalidCall::GetInstance();

	// on recopie l'élément
	m_IdSegment = Source.m_IdSegment;
}

////////////////////////////////////
// la fonction pour tester l'élément

JBool JMTFR03KEYSegment::IsValid() const
{
	// on renvoie la validité de l'élément
	return (m_IdSegment.IsValid());
}

//////////////////////////////////////
// l'opérateur pour recopier l'élément

JMTFR03KEYSegment & JMTFR03KEYSegment::operator =(const JMTFR03KEYSegment & Source)
{
	// on teste si on a quelque chose à faire
	if (this != &Source)
	{
		// on recopie l'élément
		m_IdSegment = Source.m_IdSegment;
	}

	// on renvoie notre référence
	return (*this);
}

////////////////////////////////////////////
// les opérateurs pour comparer les éléments

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
// la fonction pour réinitialiser l'élément

JVoid JMTFR03KEYSegment::Reset()
{
	// on réinitialise l'élément
	m_IdSegment.Reset();
}

/////////////////
// le destructeur

JMTFR03KEYSegment::~JMTFR03KEYSegment()
{
	// on ne fait rien
}
