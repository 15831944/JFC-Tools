//
// Fichier: JMTFR03KEYCible.cpp
// Auteur:  Sylvain SAMMURI
// Date:    09/12/2003
//

// on inclut les d�finitions n�cessaires
#include "JMTFR03KEYCible.h"

////////////////////
// les constructeurs

JMTFR03KEYCible::JMTFR03KEYCible()
{
	// on ne fait rien
}

JMTFR03KEYCible::JMTFR03KEYCible(const JMTFR03KEYCible & Source)
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

JBool JMTFR03KEYCible::IsValid() const
{
	// on renvoie la validit� de l'�l�ment
	return (m_IdSource.IsValid() && m_IdTerrain.IsValid() && m_IdCible.IsValid());
}

//////////////////////////////////////
// l'op�rateur pour recopier l'�l�ment

JMTFR03KEYCible & JMTFR03KEYCible::operator =(const JMTFR03KEYCible & Source)
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

JBool JMTFR03KEYCible::operator <(const JMTFR03KEYCible & Reference) const
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

JBool JMTFR03KEYCible::operator >(const JMTFR03KEYCible & Reference) const
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

JVoid JMTFR03KEYCible::Reset()
{
	// on r�initialise l'�l�ment
	m_IdSource .Reset();
	m_IdTerrain.Reset();
	m_IdCible  .Reset();
}

/////////////////
// le destructeur

JMTFR03KEYCible::~JMTFR03KEYCible()
{
	// on ne fait rien
}
