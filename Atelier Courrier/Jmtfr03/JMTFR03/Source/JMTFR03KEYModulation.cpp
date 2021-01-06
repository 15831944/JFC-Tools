//
// Fichier: JMTFR03KEYModulation.cpp
// Auteur:  Sylvain SAMMURI
// Date:    18/06/2004
//

// on inclut les d�finitions n�cessaires
#include "JMTFR03KEYModulation.h"

////////////////////
// les constructeurs

JMTFR03KEYModulation::JMTFR03KEYModulation()
{
	// on ne fait rien
}

JMTFR03KEYModulation::JMTFR03KEYModulation(const JMTFR03KEYModulation & Source)
{
	// on v�rifie l'existence de la source
	if (this == &Source) throw JInvalidCall::GetInstance();

	// on recopie l'�l�ment
	m_IdSource  = Source.m_IdSource;
	m_IdTerrain = Source.m_IdTerrain;
}

////////////////////////////////////
// la fonction pour tester l'�l�ment

JBool JMTFR03KEYModulation::IsValid() const
{
	// on renvoie la validit� de l'�l�ment
	return (m_IdSource.IsValid() && m_IdTerrain.IsValid());
}

//////////////////////////////////////
// l'op�rateur pour recopier l'�l�ment

JMTFR03KEYModulation & JMTFR03KEYModulation::operator =(const JMTFR03KEYModulation & Source)
{
	// on teste si on a quelque chose � faire
	if (this != &Source)
	{
		// on recopie l'�l�ment
		m_IdSource  = Source.m_IdSource;
		m_IdTerrain = Source.m_IdTerrain;
	}

	// on renvoie notre r�f�rence
	return (*this);
}

////////////////////////////////////////////
// les op�rateurs pour comparer les �l�ments

JBool JMTFR03KEYModulation::operator <(const JMTFR03KEYModulation & Reference) const
{
	// on compare la premi�re composante
	if (m_IdSource < Reference.m_IdSource) return (true);
	if (m_IdSource > Reference.m_IdSource) return (false);

	// on compare la derni�re composante
	return (m_IdTerrain < Reference.m_IdTerrain);
}

JBool JMTFR03KEYModulation::operator >(const JMTFR03KEYModulation & Reference) const
{
	// on compare la premi�re composante
	if (m_IdSource > Reference.m_IdSource) return (true);
	if (m_IdSource < Reference.m_IdSource) return (false);

	// on compare la derni�re composante
	return (m_IdTerrain > Reference.m_IdTerrain);
}

///////////////////////////////////////////
// la fonction pour r�initialiser l'�l�ment

JVoid JMTFR03KEYModulation::Reset()
{
	// on r�initialise l'�l�ment
	m_IdSource .Reset();
	m_IdTerrain.Reset();
}

/////////////////
// le destructeur

JMTFR03KEYModulation::~JMTFR03KEYModulation()
{
	// on ne fait rien
}
