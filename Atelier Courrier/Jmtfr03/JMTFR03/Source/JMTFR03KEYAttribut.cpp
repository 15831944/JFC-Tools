//
// Fichier: JMTFR03KEYAttribut.cpp
// Auteur:  Sylvain SAMMURI
// Date:    10/09/2003
//

// on inclut les définitions nécessaires
#include "JMTFR03KEYAttribut.h"

////////////////////
// les constructeurs

JMTFR03KEYAttribut::JMTFR03KEYAttribut()
{
	// on ne fait rien
}

JMTFR03KEYAttribut::JMTFR03KEYAttribut(const JMTFR03KEYAttribut & Source)
{
	// on vérifie l'existence de la source
	if (this == &Source) throw JInvalidCall::GetInstance();

	// on recopie l'élément
	m_IdTitre = Source.m_IdTitre;
}

////////////////////////////////////
// la fonction pour tester l'élément

JBool JMTFR03KEYAttribut::IsValid() const
{
	// on renvoie la validité de l'élément
	return (m_IdTitre.IsValid());
}

//////////////////////////////////////
// l'opérateur pour recopier l'élément

JMTFR03KEYAttribut & JMTFR03KEYAttribut::operator =(const JMTFR03KEYAttribut & Source)
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
// la fonction pour réinitialiser l'élément

JVoid JMTFR03KEYAttribut::Reset()
{
	// on réinitialise l'élément
	m_IdTitre.Reset();
}

/////////////////
// le destructeur

JMTFR03KEYAttribut::~JMTFR03KEYAttribut()
{
	// on ne fait rien
}
