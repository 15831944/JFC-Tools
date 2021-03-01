//
// Fichier: JMTFR03KEYHomothetie.cpp
// Auteur:  Sylvain SAMMURI
// Date:    10/09/2003
//

// on inclut les définitions nécessaires
#include "JMTFR03KEYHomothetie.h"

////////////////////
// les constructeurs

JMTFR03KEYHomothetie::JMTFR03KEYHomothetie()
{
	// on ne fait rien
}

JMTFR03KEYHomothetie::JMTFR03KEYHomothetie(const JMTFR03KEYHomothetie & Source)
{
	// on vérifie l'existence de la source
	if (this == &Source) throw JInvalidCall::GetInstance();

	// on recopie l'élément
	m_IdEchantillon = Source.m_IdEchantillon;
	m_IdUnivers     = Source.m_IdUnivers;
}

////////////////////////////////////
// la fonction pour tester l'élément

JBool JMTFR03KEYHomothetie::IsValid() const
{
	// on renvoie la validité de l'élément
	return (m_IdEchantillon.IsValid() && m_IdUnivers.IsValid());
}

//////////////////////////////////////
// l'opérateur pour recopier l'élément

JMTFR03KEYHomothetie & JMTFR03KEYHomothetie::operator =(const JMTFR03KEYHomothetie & Source)
{
	// on teste si on a quelque chose à faire
	if (this != &Source)
	{
		// on recopie l'élément
		m_IdEchantillon = Source.m_IdEchantillon;
		m_IdUnivers     = Source.m_IdUnivers;
	}

	// on renvoie notre référence
	return (*this);
}

////////////////////////////////////////////
// les opérateurs pour comparer les éléments

JBool JMTFR03KEYHomothetie::operator <(const JMTFR03KEYHomothetie & Reference) const
{
	// on compare la première composante
	if (m_IdEchantillon < Reference.m_IdEchantillon) return (true);
	if (m_IdEchantillon > Reference.m_IdEchantillon) return (false);

	// on compare la dernière composante
	return (m_IdUnivers < Reference.m_IdUnivers);
}

JBool JMTFR03KEYHomothetie::operator >(const JMTFR03KEYHomothetie & Reference) const
{
	// on compare la première composante
	if (m_IdEchantillon > Reference.m_IdEchantillon) return (true);
	if (m_IdEchantillon < Reference.m_IdEchantillon) return (false);

	// on compare la dernière composante
	return (m_IdUnivers > Reference.m_IdUnivers);
}

///////////////////////////////////////////
// la fonction pour réinitialiser l'élément

JVoid JMTFR03KEYHomothetie::Reset()
{
	// on réinitialise l'élément
	m_IdEchantillon.Reset();
	m_IdUnivers    .Reset();
}

/////////////////
// le destructeur

JMTFR03KEYHomothetie::~JMTFR03KEYHomothetie()
{
	// on ne fait rien
}
