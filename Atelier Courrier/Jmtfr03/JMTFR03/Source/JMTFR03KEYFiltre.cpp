//
// Fichier: JMTFR03KEYFiltre.cpp
// Auteur:  Sylvain SAMMURI
// Date:    10/09/2003
//

// on inclut les définitions nécessaires
#include "JMTFR03KEYFiltre.h"

////////////////////
// les constructeurs

JMTFR03KEYFiltre::JMTFR03KEYFiltre()
{
	// on ne fait rien
}

JMTFR03KEYFiltre::JMTFR03KEYFiltre(const JMTFR03KEYFiltre & Source)
{
	// on vérifie l'existence de la source
	if (this == &Source) throw JInvalidCall::GetInstance();

	// on recopie l'élément
	m_IdFiltre = Source.m_IdFiltre;
	m_IdTitre  = Source.m_IdTitre;
}

////////////////////////////////////
// la fonction pour tester l'élément

JBool JMTFR03KEYFiltre::IsValid() const
{
	// on renvoie la validité de l'élément
	return (m_IdFiltre.IsValid() && m_IdTitre.IsValid());
}

//////////////////////////////////////
// l'opérateur pour recopier l'élément

JMTFR03KEYFiltre & JMTFR03KEYFiltre::operator =(const JMTFR03KEYFiltre & Source)
{
	// on teste si on a quelque chose à faire
	if (this != &Source)
	{
		// on recopie l'élément
		m_IdFiltre = Source.m_IdFiltre;
		m_IdTitre  = Source.m_IdTitre;
	}

	// on renvoie notre référence
	return (*this);
}

////////////////////////////////////////////
// les opérateurs pour comparer les éléments

JBool JMTFR03KEYFiltre::operator <(const JMTFR03KEYFiltre & Reference) const
{
	// on compare la première composante
	if (m_IdFiltre < Reference.m_IdFiltre) return (true);
	if (m_IdFiltre > Reference.m_IdFiltre) return (false);

	// on compare la dernière composante
	return (m_IdTitre < Reference.m_IdTitre);
}

JBool JMTFR03KEYFiltre::operator >(const JMTFR03KEYFiltre & Reference) const
{
	// on compare la première composante
	if (m_IdFiltre > Reference.m_IdFiltre) return (true);
	if (m_IdFiltre < Reference.m_IdFiltre) return (false);

	// on compare la dernière composante
	return (m_IdTitre > Reference.m_IdTitre);
}

///////////////////////////////////////////
// la fonction pour réinitialiser l'élément

JVoid JMTFR03KEYFiltre::Reset()
{
	// on réinitialise l'élément
	m_IdFiltre.Reset();
	m_IdTitre .Reset();
}

/////////////////
// le destructeur

JMTFR03KEYFiltre::~JMTFR03KEYFiltre()
{
	// on ne fait rien
}
