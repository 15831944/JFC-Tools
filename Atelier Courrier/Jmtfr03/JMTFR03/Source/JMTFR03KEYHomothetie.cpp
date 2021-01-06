//
// Fichier: JMTFR03KEYHomothetie.cpp
// Auteur:  Sylvain SAMMURI
// Date:    10/09/2003
//

// on inclut les d�finitions n�cessaires
#include "JMTFR03KEYHomothetie.h"

////////////////////
// les constructeurs

JMTFR03KEYHomothetie::JMTFR03KEYHomothetie()
{
	// on ne fait rien
}

JMTFR03KEYHomothetie::JMTFR03KEYHomothetie(const JMTFR03KEYHomothetie & Source)
{
	// on v�rifie l'existence de la source
	if (this == &Source) throw JInvalidCall::GetInstance();

	// on recopie l'�l�ment
	m_IdEchantillon = Source.m_IdEchantillon;
	m_IdUnivers     = Source.m_IdUnivers;
}

////////////////////////////////////
// la fonction pour tester l'�l�ment

JBool JMTFR03KEYHomothetie::IsValid() const
{
	// on renvoie la validit� de l'�l�ment
	return (m_IdEchantillon.IsValid() && m_IdUnivers.IsValid());
}

//////////////////////////////////////
// l'op�rateur pour recopier l'�l�ment

JMTFR03KEYHomothetie & JMTFR03KEYHomothetie::operator =(const JMTFR03KEYHomothetie & Source)
{
	// on teste si on a quelque chose � faire
	if (this != &Source)
	{
		// on recopie l'�l�ment
		m_IdEchantillon = Source.m_IdEchantillon;
		m_IdUnivers     = Source.m_IdUnivers;
	}

	// on renvoie notre r�f�rence
	return (*this);
}

////////////////////////////////////////////
// les op�rateurs pour comparer les �l�ments

JBool JMTFR03KEYHomothetie::operator <(const JMTFR03KEYHomothetie & Reference) const
{
	// on compare la premi�re composante
	if (m_IdEchantillon < Reference.m_IdEchantillon) return (true);
	if (m_IdEchantillon > Reference.m_IdEchantillon) return (false);

	// on compare la derni�re composante
	return (m_IdUnivers < Reference.m_IdUnivers);
}

JBool JMTFR03KEYHomothetie::operator >(const JMTFR03KEYHomothetie & Reference) const
{
	// on compare la premi�re composante
	if (m_IdEchantillon > Reference.m_IdEchantillon) return (true);
	if (m_IdEchantillon < Reference.m_IdEchantillon) return (false);

	// on compare la derni�re composante
	return (m_IdUnivers > Reference.m_IdUnivers);
}

///////////////////////////////////////////
// la fonction pour r�initialiser l'�l�ment

JVoid JMTFR03KEYHomothetie::Reset()
{
	// on r�initialise l'�l�ment
	m_IdEchantillon.Reset();
	m_IdUnivers    .Reset();
}

/////////////////
// le destructeur

JMTFR03KEYHomothetie::~JMTFR03KEYHomothetie()
{
	// on ne fait rien
}
