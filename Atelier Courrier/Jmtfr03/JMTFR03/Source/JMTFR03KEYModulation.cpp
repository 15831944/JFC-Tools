//
// Fichier: JMTFR03KEYModulation.cpp
// Auteur:  Sylvain SAMMURI
// Date:    18/06/2004
//

// on inclut les définitions nécessaires
#include "JMTFR03KEYModulation.h"

////////////////////
// les constructeurs

JMTFR03KEYModulation::JMTFR03KEYModulation()
{
	// on ne fait rien
}

JMTFR03KEYModulation::JMTFR03KEYModulation(const JMTFR03KEYModulation & Source)
{
	// on vérifie l'existence de la source
	if (this == &Source) throw JInvalidCall::GetInstance();

	// on recopie l'élément
	m_IdSource  = Source.m_IdSource;
	m_IdTerrain = Source.m_IdTerrain;
}

////////////////////////////////////
// la fonction pour tester l'élément

JBool JMTFR03KEYModulation::IsValid() const
{
	// on renvoie la validité de l'élément
	return (m_IdSource.IsValid() && m_IdTerrain.IsValid());
}

//////////////////////////////////////
// l'opérateur pour recopier l'élément

JMTFR03KEYModulation & JMTFR03KEYModulation::operator =(const JMTFR03KEYModulation & Source)
{
	// on teste si on a quelque chose à faire
	if (this != &Source)
	{
		// on recopie l'élément
		m_IdSource  = Source.m_IdSource;
		m_IdTerrain = Source.m_IdTerrain;
	}

	// on renvoie notre référence
	return (*this);
}

////////////////////////////////////////////
// les opérateurs pour comparer les éléments

JBool JMTFR03KEYModulation::operator <(const JMTFR03KEYModulation & Reference) const
{
	// on compare la première composante
	if (m_IdSource < Reference.m_IdSource) return (true);
	if (m_IdSource > Reference.m_IdSource) return (false);

	// on compare la dernière composante
	return (m_IdTerrain < Reference.m_IdTerrain);
}

JBool JMTFR03KEYModulation::operator >(const JMTFR03KEYModulation & Reference) const
{
	// on compare la première composante
	if (m_IdSource > Reference.m_IdSource) return (true);
	if (m_IdSource < Reference.m_IdSource) return (false);

	// on compare la dernière composante
	return (m_IdTerrain > Reference.m_IdTerrain);
}

///////////////////////////////////////////
// la fonction pour réinitialiser l'élément

JVoid JMTFR03KEYModulation::Reset()
{
	// on réinitialise l'élément
	m_IdSource .Reset();
	m_IdTerrain.Reset();
}

/////////////////
// le destructeur

JMTFR03KEYModulation::~JMTFR03KEYModulation()
{
	// on ne fait rien
}
