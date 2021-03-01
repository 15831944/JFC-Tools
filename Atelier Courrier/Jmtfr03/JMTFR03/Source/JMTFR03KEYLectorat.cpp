//
// Fichier: JMTFR03KEYLectorat.cpp
// Auteur:  Sylvain SAMMURI
// Date:    02/12/2003
//

// on inclut les définitions nécessaires
#include "JMTFR03KEYLectorat.h"

////////////////////
// les constructeurs

JMTFR03KEYLectorat::JMTFR03KEYLectorat()
{
	// on ne fait rien
}

JMTFR03KEYLectorat::JMTFR03KEYLectorat(const JMTFR03KEYLectorat & Source)
{
	// on vérifie l'existence de la source
	if (this == &Source) throw JInvalidCall::GetInstance();

	// on recopie l'élément
	m_IdTitre = Source.m_IdTitre;
}

////////////////////////////////////
// la fonction pour tester l'élément

JBool JMTFR03KEYLectorat::IsValid() const
{
	// on renvoie la validité de l'élément
	return (m_IdTitre.IsValid());
}

//////////////////////////////////////
// l'opérateur pour recopier l'élément

JMTFR03KEYLectorat & JMTFR03KEYLectorat::operator =(const JMTFR03KEYLectorat & Source)
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

JBool JMTFR03KEYLectorat::operator <(const JMTFR03KEYLectorat & Reference) const
{
	// on compare la seule composante
	return (m_IdTitre < Reference.m_IdTitre);
}

JBool JMTFR03KEYLectorat::operator >(const JMTFR03KEYLectorat & Reference) const
{
	// on compare la seule composante
	return (m_IdTitre > Reference.m_IdTitre);
}

///////////////////////////////////////////
// la fonction pour réinitialiser l'élément

JVoid JMTFR03KEYLectorat::Reset()
{
	// on réinitialise l'élément
	m_IdTitre.Reset();
}

/////////////////
// le destructeur

JMTFR03KEYLectorat::~JMTFR03KEYLectorat()
{
	// on ne fait rien
}
