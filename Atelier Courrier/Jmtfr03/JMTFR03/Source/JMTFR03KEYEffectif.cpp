//
// Fichier: JMTFR03KEYEffectif.cpp
// Auteur:  Sylvain SAMMURI
// Date:    05/12/2003
//

// on inclut les définitions nécessaires
#include "JMTFR03KEYEffectif.h"

////////////////////
// les constructeurs

JMTFR03KEYEffectif::JMTFR03KEYEffectif()
{
	// on ne fait rien
}

JMTFR03KEYEffectif::JMTFR03KEYEffectif(const JMTFR03KEYEffectif & Source)
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

JBool JMTFR03KEYEffectif::IsValid() const
{
	// on renvoie la validité de l'élément
	return (m_IdSource.IsValid() && m_IdTerrain.IsValid() && m_IdCible.IsValid());
}

//////////////////////////////////////
// l'opérateur pour recopier l'élément

JMTFR03KEYEffectif & JMTFR03KEYEffectif::operator =(const JMTFR03KEYEffectif & Source)
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

JBool JMTFR03KEYEffectif::operator <(const JMTFR03KEYEffectif & Reference) const
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

JBool JMTFR03KEYEffectif::operator >(const JMTFR03KEYEffectif & Reference) const
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

JVoid JMTFR03KEYEffectif::Reset()
{
	// on réinitialise l'élément
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
