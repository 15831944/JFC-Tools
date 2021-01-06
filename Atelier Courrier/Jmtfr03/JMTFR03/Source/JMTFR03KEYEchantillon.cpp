//
// Fichier: JMTFR03KEYEchantillon.cpp
// Auteur:  Sylvain SAMMURI
// Date:    10/09/2003
//

// on inclut les d�finitions n�cessaires
#include "JMTFR03KEYEchantillon.h"

////////////////////
// les constructeurs

JMTFR03KEYEchantillon::JMTFR03KEYEchantillon()
{
	// on ne fait rien
}

JMTFR03KEYEchantillon::JMTFR03KEYEchantillon(const JMTFR03KEYEchantillon & Source)
{
	// on v�rifie l'existence de la source
	if (this == &Source) throw JInvalidCall::GetInstance();

	// on recopie l'�l�ment
	m_IdEchantillon = Source.m_IdEchantillon;
}

////////////////////////////////////
// la fonction pour tester l'�l�ment

JBool JMTFR03KEYEchantillon::IsValid() const
{
	// on renvoie la validit� de l'�l�ment
	return (m_IdEchantillon.IsValid());
}

//////////////////////////////////////
// l'op�rateur pour recopier l'�l�ment

JMTFR03KEYEchantillon & JMTFR03KEYEchantillon::operator =(const JMTFR03KEYEchantillon & Source)
{
	// on teste si on a quelque chose � faire
	if (this != &Source)
	{
		// on recopie l'�l�ment
		m_IdEchantillon = Source.m_IdEchantillon;
	}

	// on renvoie notre r�f�rence
	return (*this);
}

////////////////////////////////////////////
// les op�rateurs pour comparer les �l�ments

JBool JMTFR03KEYEchantillon::operator <(const JMTFR03KEYEchantillon & Reference) const
{
	// on compare la seule composante
	return (m_IdEchantillon < Reference.m_IdEchantillon);
}

JBool JMTFR03KEYEchantillon::operator >(const JMTFR03KEYEchantillon & Reference) const
{
	// on compare la seule composante
	return (m_IdEchantillon > Reference.m_IdEchantillon);
}

///////////////////////////////////////////
// la fonction pour r�initialiser l'�l�ment

JVoid JMTFR03KEYEchantillon::Reset()
{
	// on r�initialise l'�l�ment
	m_IdEchantillon.Reset();
}

/////////////////
// le destructeur

JMTFR03KEYEchantillon::~JMTFR03KEYEchantillon()
{
	// on ne fait rien
}
