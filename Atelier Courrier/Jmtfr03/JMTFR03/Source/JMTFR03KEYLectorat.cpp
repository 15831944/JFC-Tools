//
// Fichier: JMTFR03KEYLectorat.cpp
// Auteur:  Sylvain SAMMURI
// Date:    02/12/2003
//

// on inclut les d�finitions n�cessaires
#include "JMTFR03KEYLectorat.h"

////////////////////
// les constructeurs

JMTFR03KEYLectorat::JMTFR03KEYLectorat()
{
	// on ne fait rien
}

JMTFR03KEYLectorat::JMTFR03KEYLectorat(const JMTFR03KEYLectorat & Source)
{
	// on v�rifie l'existence de la source
	if (this == &Source) throw JInvalidCall::GetInstance();

	// on recopie l'�l�ment
	m_IdTitre = Source.m_IdTitre;
}

////////////////////////////////////
// la fonction pour tester l'�l�ment

JBool JMTFR03KEYLectorat::IsValid() const
{
	// on renvoie la validit� de l'�l�ment
	return (m_IdTitre.IsValid());
}

//////////////////////////////////////
// l'op�rateur pour recopier l'�l�ment

JMTFR03KEYLectorat & JMTFR03KEYLectorat::operator =(const JMTFR03KEYLectorat & Source)
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
// la fonction pour r�initialiser l'�l�ment

JVoid JMTFR03KEYLectorat::Reset()
{
	// on r�initialise l'�l�ment
	m_IdTitre.Reset();
}

/////////////////
// le destructeur

JMTFR03KEYLectorat::~JMTFR03KEYLectorat()
{
	// on ne fait rien
}
