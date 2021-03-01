//
// Fichier: JMTFR03KEYEffectif.cpp
// Auteur:  Sylvain SAMMURI
// Date:    05/12/2003
//

// on inclut les d�finitions n�cessaires
#include "JMTFR03KEYEffectif.h"

////////////////////
// les constructeurs

JMTFR03KEYEffectif::JMTFR03KEYEffectif()
{
	// on ne fait rien
}

JMTFR03KEYEffectif::JMTFR03KEYEffectif(const JMTFR03KEYEffectif & Source)
{
	// on v�rifie l'existence de la source
	if (this == &Source) throw JInvalidCall::GetInstance();

	// on recopie l'�l�ment
	m_IdSource  = Source.m_IdSource;
	m_IdTerrain = Source.m_IdTerrain;
	m_IdCible   = Source.m_IdCible;
}

////////////////////////////////////
// la fonction pour tester l'�l�ment

JBool JMTFR03KEYEffectif::IsValid() const
{
	// on renvoie la validit� de l'�l�ment
	return (m_IdSource.IsValid() && m_IdTerrain.IsValid() && m_IdCible.IsValid());
}

//////////////////////////////////////
// l'op�rateur pour recopier l'�l�ment

JMTFR03KEYEffectif & JMTFR03KEYEffectif::operator =(const JMTFR03KEYEffectif & Source)
{
	// on teste si on a quelque chose � faire
	if (this != &Source)
	{
		// on recopie l'�l�ment
		m_IdSource  = Source.m_IdSource;
		m_IdTerrain = Source.m_IdTerrain;
		m_IdCible   = Source.m_IdCible;
	}

	// on renvoie notre r�f�rence
	return (*this);
}

////////////////////////////////////////////
// les op�rateurs pour comparer les �l�ments

JBool JMTFR03KEYEffectif::operator <(const JMTFR03KEYEffectif & Reference) const
{
	// on compare la premi�re composante
	if (m_IdSource < Reference.m_IdSource) return (true);
	if (m_IdSource > Reference.m_IdSource) return (false);

	// on compare la deuxi�me composante
	if (m_IdTerrain < Reference.m_IdTerrain) return (true);
	if (m_IdTerrain > Reference.m_IdTerrain) return (false);

	// on compare la derni�re composante
	return (m_IdCible < Reference.m_IdCible);
}

JBool JMTFR03KEYEffectif::operator >(const JMTFR03KEYEffectif & Reference) const
{
	// on compare la premi�re composante
	if (m_IdSource > Reference.m_IdSource) return (true);
	if (m_IdSource < Reference.m_IdSource) return (false);

	// on compare la deuxi�me composante
	if (m_IdTerrain > Reference.m_IdTerrain) return (true);
	if (m_IdTerrain < Reference.m_IdTerrain) return (false);

	// on compare la derni�re composante
	return (m_IdCible > Reference.m_IdCible);
}

///////////////////////////////////////////
// la fonction pour r�initialiser l'�l�ment

JVoid JMTFR03KEYEffectif::Reset()
{
	// on r�initialise l'�l�ment
	m_IdSource .Reset();
	m_IdTerrain.Reset();
	m_IdCible  .Reset();
}

/////////////////
// le destructeur

JMTFR03KEYEffectif::~JMTFR03KEYEffectif()
{
	// on ne fait rien
}
