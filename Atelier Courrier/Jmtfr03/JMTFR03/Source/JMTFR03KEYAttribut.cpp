//
// Fichier: JMTFR03KEYAttribut.cpp
// Auteur:  Sylvain SAMMURI
// Date:    10/09/2003
//

// on inclut les d�finitions n�cessaires
#include "JMTFR03KEYAttribut.h"

////////////////////
// les constructeurs

JMTFR03KEYAttribut::JMTFR03KEYAttribut()
{
	// on ne fait rien
}

JMTFR03KEYAttribut::JMTFR03KEYAttribut(const JMTFR03KEYAttribut & Source)
{
	// on v�rifie l'existence de la source
	if (this == &Source) throw JInvalidCall::GetInstance();

	// on recopie l'�l�ment
	m_IdTitre = Source.m_IdTitre;
}

////////////////////////////////////
// la fonction pour tester l'�l�ment

JBool JMTFR03KEYAttribut::IsValid() const
{
	// on renvoie la validit� de l'�l�ment
	return (m_IdTitre.IsValid());
}

//////////////////////////////////////
// l'op�rateur pour recopier l'�l�ment

JMTFR03KEYAttribut & JMTFR03KEYAttribut::operator =(const JMTFR03KEYAttribut & Source)
{
	// on teste si on a quelque chose � faire
	if (this != &Source)
	{
		// on recopie l'�l�ment
		m_IdTitre = Source.m_IdTitre;
	}

	// on renvoie notre r�f�rence
	return (*this);
}

////////////////////////////////////////////
// les op�rateurs pour comparer les �l�ments

JBool JMTFR03KEYAttribut::operator <(const JMTFR03KEYAttribut & Reference) const
{
	// on compare la seule composante
	return (m_IdTitre < Reference.m_IdTitre);
}

JBool JMTFR03KEYAttribut::operator >(const JMTFR03KEYAttribut & Reference) const
{
	// on compare la seule composante
	return (m_IdTitre > Reference.m_IdTitre);
}

///////////////////////////////////////////
// la fonction pour r�initialiser l'�l�ment

JVoid JMTFR03KEYAttribut::Reset()
{
	// on r�initialise l'�l�ment
	m_IdTitre.Reset();
}

/////////////////
// le destructeur

JMTFR03KEYAttribut::~JMTFR03KEYAttribut()
{
	// on ne fait rien
}
