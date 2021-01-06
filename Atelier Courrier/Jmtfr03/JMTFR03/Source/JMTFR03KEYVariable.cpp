//
// Fichier: JMTFR03KEYVariable.cpp
// Auteur:  Sylvain SAMMURI
// Date:    10/09/2003
//

// on inclut les d�finitions n�cessaires
#include "JMTFR03KEYVariable.h"

////////////////////
// les constructeurs

JMTFR03KEYVariable::JMTFR03KEYVariable()
{
	// on ne fait rien
}

JMTFR03KEYVariable::JMTFR03KEYVariable(const JMTFR03KEYVariable & Source)
{
	// on v�rifie l'existence de la source
	if (this == &Source) throw JInvalidCall::GetInstance();

	// on recopie l'�l�ment
	m_IdTitre = Source.m_IdTitre;
}

////////////////////////////////////
// la fonction pour tester l'�l�ment

JBool JMTFR03KEYVariable::IsValid() const
{
	// on renvoie la validit� de l'�l�ment
	return (m_IdTitre.IsValid());
}

//////////////////////////////////////
// l'op�rateur pour recopier l'�l�ment

JMTFR03KEYVariable & JMTFR03KEYVariable::operator =(const JMTFR03KEYVariable & Source)
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

JBool JMTFR03KEYVariable::operator <(const JMTFR03KEYVariable & Reference) const
{
	// on compare la seule composante
	return (m_IdTitre < Reference.m_IdTitre);
}

JBool JMTFR03KEYVariable::operator >(const JMTFR03KEYVariable & Reference) const
{
	// on compare la seule composante
	return (m_IdTitre > Reference.m_IdTitre);
}

///////////////////////////////////////////
// la fonction pour r�initialiser l'�l�ment

JVoid JMTFR03KEYVariable::Reset()
{
	// on r�initialise l'�l�ment
	m_IdTitre.Reset();
}

/////////////////
// le destructeur

JMTFR03KEYVariable::~JMTFR03KEYVariable()
{
	// on ne fait rien
}
