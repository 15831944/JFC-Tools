//
// Fichier: JMTFR03KEYReprise.cpp
// Auteur:  Sylvain SAMMURI
// Date:    06/04/2004
//

// on inclut les définitions nécessaires
#include "JMTFR03KEYReprise.h"

////////////////////
// les constructeurs

JMTFR03KEYReprise::JMTFR03KEYReprise()
{
	// on ne fait rien
}

JMTFR03KEYReprise::JMTFR03KEYReprise(const JMTFR03KEYReprise & Source)
{
	// on vérifie l'existence de la source
	if (this == &Source) throw JInvalidCall::GetInstance();

	// on recopie l'élément
	m_IdTitre = Source.m_IdTitre;
}

////////////////////////////////////
// la fonction pour tester l'élément

JBool JMTFR03KEYReprise::IsValid() const
{
	// on renvoie la validité de l'élément
	return (m_IdTitre.IsValid());
}

//////////////////////////////////////
// l'opérateur pour recopier l'élément

JMTFR03KEYReprise & JMTFR03KEYReprise::operator =(const JMTFR03KEYReprise & Source)
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

JBool JMTFR03KEYReprise::operator <(const JMTFR03KEYReprise & Reference) const
{
	// on compare la seule composante
	return (m_IdTitre < Reference.m_IdTitre);
}

JBool JMTFR03KEYReprise::operator >(const JMTFR03KEYReprise & Reference) const
{
	// on compare la seule composante
	return (m_IdTitre > Reference.m_IdTitre);
}

///////////////////////////////////////////
// la fonction pour réinitialiser l'élément

JVoid JMTFR03KEYReprise::Reset()
{
	// on réinitialise l'élément
	m_IdTitre.Reset();
}

/////////////////
// le destructeur

JMTFR03KEYReprise::~JMTFR03KEYReprise()
{
	// on ne fait rien
}
