//
// Fichier: JMTFR03KEYVariable.cpp
// Auteur:  Sylvain SAMMURI
// Date:    10/09/2003
//

// on inclut les définitions nécessaires
#include "JMTFR03KEYVariable.h"

////////////////////
// les constructeurs

JMTFR03KEYVariable::JMTFR03KEYVariable()
{
	// on ne fait rien
}

JMTFR03KEYVariable::JMTFR03KEYVariable(const JMTFR03KEYVariable & Source)
{
	// on vérifie l'existence de la source
	if (this == &Source) throw JInvalidCall::GetInstance();

	// on recopie l'élément
	m_IdTitre = Source.m_IdTitre;
}

////////////////////////////////////
// la fonction pour tester l'élément

JBool JMTFR03KEYVariable::IsValid() const
{
	// on renvoie la validité de l'élément
	return (m_IdTitre.IsValid());
}

//////////////////////////////////////
// l'opérateur pour recopier l'élément

JMTFR03KEYVariable & JMTFR03KEYVariable::operator =(const JMTFR03KEYVariable & Source)
{
	// on teste si on a quelque chose à faire
	if (this != &Source)
	{
		// on recopie l'élément
		m_IdTitre = Source.m_IdTitre;
	}

	// on renvoie notre référence
	return (*this);
}

////////////////////////////////////////////
// les opérateurs pour comparer les éléments

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
// la fonction pour réinitialiser l'élément

JVoid JMTFR03KEYVariable::Reset()
{
	// on réinitialise l'élément
	m_IdTitre.Reset();
}

/////////////////
// le destructeur

JMTFR03KEYVariable::~JMTFR03KEYVariable()
{
	// on ne fait rien
}
