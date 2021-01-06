//
// Fichier: JMTFR03KEYReprise.cpp
// Auteur:  Sylvain SAMMURI
// Date:    06/04/2004
//

// on inclut les d�finitions n�cessaires
#include "JMTFR03KEYReprise.h"

////////////////////
// les constructeurs

JMTFR03KEYReprise::JMTFR03KEYReprise()
{
	// on ne fait rien
}

JMTFR03KEYReprise::JMTFR03KEYReprise(const JMTFR03KEYReprise & Source)
{
	// on v�rifie l'existence de la source
	if (this == &Source) throw JInvalidCall::GetInstance();

	// on recopie l'�l�ment
	m_IdTitre = Source.m_IdTitre;
}

////////////////////////////////////
// la fonction pour tester l'�l�ment

JBool JMTFR03KEYReprise::IsValid() const
{
	// on renvoie la validit� de l'�l�ment
	return (m_IdTitre.IsValid());
}

//////////////////////////////////////
// l'op�rateur pour recopier l'�l�ment

JMTFR03KEYReprise & JMTFR03KEYReprise::operator =(const JMTFR03KEYReprise & Source)
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
// la fonction pour r�initialiser l'�l�ment

JVoid JMTFR03KEYReprise::Reset()
{
	// on r�initialise l'�l�ment
	m_IdTitre.Reset();
}

/////////////////
// le destructeur

JMTFR03KEYReprise::~JMTFR03KEYReprise()
{
	// on ne fait rien
}
