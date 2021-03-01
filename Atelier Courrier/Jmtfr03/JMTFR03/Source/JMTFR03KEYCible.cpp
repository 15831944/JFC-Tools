//
// Fichier: JMTFR03KEYCible.cpp
// Auteur:  Sylvain SAMMURI
// Date:    09/12/2003
//

// on inclut les définitions nécessaires
#include "JMTFR03KEYCible.h"

////////////////////
// les constructeurs

JMTFR03KEYCible::JMTFR03KEYCible()
{
	// on ne fait rien
}

JMTFR03KEYCible::JMTFR03KEYCible(const JMTFR03KEYCible & Source)
{
	// on vérifie l'existence de la source
	if (this == &Source) throw JInvalidCall::GetInstance();

	// on recopie l'élément
	m_IdSource  = Source.m_IdSource;
	m_IdTerrain = Source.m_IdTerrain;
	m_IdCible   = Source.m_IdCible;
}

////////////////////////////////////
// la fonction pour tester l'élément

JBool JMTFR03KEYCible::IsValid() const
{
	// on renvoie la validité de l'élément
	return (m_IdSource.IsValid() && m_IdTerrain.IsValid() && m_IdCible.IsValid());
}

//////////////////////////////////////
// l'opérateur pour recopier l'élément

JMTFR03KEYCible & JMTFR03KEYCible::operator =(const JMTFR03KEYCible & Source)
{
	// on teste si on a quelque chose à faire
	if (this != &Source)
	{
		// on recopie l'élément
		m_IdSource  = Source.m_IdSource;
		m_IdTerrain = Source.m_IdTerrain;
		m_IdCible   = Source.m_IdCible;
	}

	// on renvoie notre référence
	return (*this);
}

////////////////////////////////////////////
// les opérateurs pour comparer les éléments

JBool JMTFR03KEYCible::operator <(const JMTFR03KEYCible & Reference) const
{
	// on compare la première composante
	if (m_IdSource < Reference.m_IdSource) return (true);
	if (m_IdSource > Reference.m_IdSource) return (false);

	// on compare la deuxième composante
	if (m_IdTerrain < Reference.m_IdTerrain) return (true);
	if (m_IdTerrain > Reference.m_IdTerrain) return (false);

	// on compare la dernière composante
	return (m_IdCible < Reference.m_IdCible);
}

JBool JMTFR03KEYCible::operator >(const JMTFR03KEYCible & Reference) const
{
	// on compare la première composante
	if (m_IdSource > Reference.m_IdSource) return (true);
	if (m_IdSource < Reference.m_IdSource) return (false);

	// on compare la deuxième composante
	if (m_IdTerrain > Reference.m_IdTerrain) return (true);
	if (m_IdTerrain < Reference.m_IdTerrain) return (false);

	// on compare la dernière composante
	return (m_IdCible > Reference.m_IdCible);
}

///////////////////////////////////////////
// la fonction pour réinitialiser l'élément

JVoid JMTFR03KEYCible::Reset()
{
	// on réinitialise l'élément
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
