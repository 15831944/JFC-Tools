//
// Fichier: JMTFR03KEYAudience.cpp
// Auteur:  Sylvain SAMMURI
// Date:    05/12/2003
//

// on inclut les d�finitions n�cessaires
#include "JMTFR03KEYAudience.h"

////////////////////
// les constructeurs

JMTFR03KEYAudience::JMTFR03KEYAudience()
{
	// on ne fait rien
}

JMTFR03KEYAudience::JMTFR03KEYAudience(const JMTFR03KEYAudience & Source)
{
	// on v�rifie l'existence de la source
	if (this == &Source) throw JInvalidCall::GetInstance();

	// on recopie l'�l�ment
	m_IdSource  = Source.m_IdSource;
	m_IdTerrain = Source.m_IdTerrain;
	m_IdCible   = Source.m_IdCible;
	m_IdTitre   = Source.m_IdTitre;
}

////////////////////////////////////
// la fonction pour tester l'�l�ment

JBool JMTFR03KEYAudience::IsValid() const
{
	// on renvoie la validit� de l'�l�ment
	return (m_IdSource.IsValid() && m_IdTerrain.IsValid() && m_IdCible.IsValid() && m_IdTitre.IsValid());
}

//////////////////////////////////////
// l'op�rateur pour recopier l'�l�ment

JMTFR03KEYAudience & JMTFR03KEYAudience::operator =(const JMTFR03KEYAudience & Source)
{
	// on teste si on a quelque chose � faire
	if (this != &Source)
	{
		// on recopie l'�l�ment
		m_IdSource  = Source.m_IdSource;
		m_IdTerrain = Source.m_IdTerrain;
		m_IdCible   = Source.m_IdCible;
		m_IdTitre   = Source.m_IdTitre;
	}

	// on renvoie notre r�f�rence
	return (*this);
}

////////////////////////////////////////////
// les op�rateurs pour comparer les �l�ments

JBool JMTFR03KEYAudience::operator <(const JMTFR03KEYAudience & Reference) const
{
	// on compare la premi�re composante
	if (m_IdSource < Reference.m_IdSource) return (true);
	if (m_IdSource > Reference.m_IdSource) return (false);

	// on compare la deuxi�me composante
	if (m_IdTerrain < Reference.m_IdTerrain) return (true);
	if (m_IdTerrain > Reference.m_IdTerrain) return (false);

	// on compare la troisi�me composante
	if (m_IdCible < Reference.m_IdCible) return (true);
	if (m_IdCible > Reference.m_IdCible) return (false);

	// on compare la derni�re composante
	return (m_IdTitre < Reference.m_IdTitre);
}

JBool JMTFR03KEYAudience::operator >(const JMTFR03KEYAudience & Reference) const
{
	// on compare la premi�re composante
	if (m_IdSource > Reference.m_IdSource) return (true);
	if (m_IdSource < Reference.m_IdSource) return (false);

	// on compare la deuxi�me composante
	if (m_IdTerrain > Reference.m_IdTerrain) return (true);
	if (m_IdTerrain < Reference.m_IdTerrain) return (false);

	// on compare la troisi�me composante
	if (m_IdCible > Reference.m_IdCible) return (true);
	if (m_IdCible < Reference.m_IdCible) return (false);

	// on compare la derni�re composante
	return (m_IdTitre > Reference.m_IdTitre);
}

///////////////////////////////////////////
// la fonction pour r�initialiser l'�l�ment

JVoid JMTFR03KEYAudience::Reset()
{
	// on r�initialise l'�l�ment
	m_IdSource .Reset();
	m_IdTerrain.Reset();
	m_IdCible  .Reset();
	m_IdTitre  .Reset();
}

/////////////////
// le destructeur

JMTFR03KEYAudience::~JMTFR03KEYAudience()
{
	// on ne fait rien
}
