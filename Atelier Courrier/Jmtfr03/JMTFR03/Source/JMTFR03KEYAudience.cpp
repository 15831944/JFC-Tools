//
// Fichier: JMTFR03KEYAudience.cpp
// Auteur:  Sylvain SAMMURI
// Date:    05/12/2003
//

// on inclut les définitions nécessaires
#include "JMTFR03KEYAudience.h"

////////////////////
// les constructeurs

JMTFR03KEYAudience::JMTFR03KEYAudience()
{
	// on ne fait rien
}

JMTFR03KEYAudience::JMTFR03KEYAudience(const JMTFR03KEYAudience & Source)
{
	// on vérifie l'existence de la source
	if (this == &Source) throw JInvalidCall::GetInstance();

	// on recopie l'élément
	m_IdSource  = Source.m_IdSource;
	m_IdTerrain = Source.m_IdTerrain;
	m_IdCible   = Source.m_IdCible;
	m_IdTitre   = Source.m_IdTitre;
}

////////////////////////////////////
// la fonction pour tester l'élément

JBool JMTFR03KEYAudience::IsValid() const
{
	// on renvoie la validité de l'élément
	return (m_IdSource.IsValid() && m_IdTerrain.IsValid() && m_IdCible.IsValid() && m_IdTitre.IsValid());
}

//////////////////////////////////////
// l'opérateur pour recopier l'élément

JMTFR03KEYAudience & JMTFR03KEYAudience::operator =(const JMTFR03KEYAudience & Source)
{
	// on teste si on a quelque chose à faire
	if (this != &Source)
	{
		// on recopie l'élément
		m_IdSource  = Source.m_IdSource;
		m_IdTerrain = Source.m_IdTerrain;
		m_IdCible   = Source.m_IdCible;
		m_IdTitre   = Source.m_IdTitre;
	}

	// on renvoie notre référence
	return (*this);
}

////////////////////////////////////////////
// les opérateurs pour comparer les éléments

JBool JMTFR03KEYAudience::operator <(const JMTFR03KEYAudience & Reference) const
{
	// on compare la première composante
	if (m_IdSource < Reference.m_IdSource) return (true);
	if (m_IdSource > Reference.m_IdSource) return (false);

	// on compare la deuxième composante
	if (m_IdTerrain < Reference.m_IdTerrain) return (true);
	if (m_IdTerrain > Reference.m_IdTerrain) return (false);

	// on compare la troisième composante
	if (m_IdCible < Reference.m_IdCible) return (true);
	if (m_IdCible > Reference.m_IdCible) return (false);

	// on compare la dernière composante
	return (m_IdTitre < Reference.m_IdTitre);
}

JBool JMTFR03KEYAudience::operator >(const JMTFR03KEYAudience & Reference) const
{
	// on compare la première composante
	if (m_IdSource > Reference.m_IdSource) return (true);
	if (m_IdSource < Reference.m_IdSource) return (false);

	// on compare la deuxième composante
	if (m_IdTerrain > Reference.m_IdTerrain) return (true);
	if (m_IdTerrain < Reference.m_IdTerrain) return (false);

	// on compare la troisième composante
	if (m_IdCible > Reference.m_IdCible) return (true);
	if (m_IdCible < Reference.m_IdCible) return (false);

	// on compare la dernière composante
	return (m_IdTitre > Reference.m_IdTitre);
}

///////////////////////////////////////////
// la fonction pour réinitialiser l'élément

JVoid JMTFR03KEYAudience::Reset()
{
	// on réinitialise l'élément
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
