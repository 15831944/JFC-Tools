//
// Fichier: JMTFR03KEYFiltre.cpp
// Auteur:  Sylvain SAMMURI
// Date:    10/09/2003
//

// on inclut les d�finitions n�cessaires
#include "JMTFR03KEYFiltre.h"

////////////////////
// les constructeurs

JMTFR03KEYFiltre::JMTFR03KEYFiltre()
{
	// on ne fait rien
}

JMTFR03KEYFiltre::JMTFR03KEYFiltre(const JMTFR03KEYFiltre & Source)
{
	// on v�rifie l'existence de la source
	if (this == &Source) throw JInvalidCall::GetInstance();

	// on recopie l'�l�ment
	m_IdFiltre = Source.m_IdFiltre;
	m_IdTitre  = Source.m_IdTitre;
}

////////////////////////////////////
// la fonction pour tester l'�l�ment

JBool JMTFR03KEYFiltre::IsValid() const
{
	// on renvoie la validit� de l'�l�ment
	return (m_IdFiltre.IsValid() && m_IdTitre.IsValid());
}

//////////////////////////////////////
// l'op�rateur pour recopier l'�l�ment

JMTFR03KEYFiltre & JMTFR03KEYFiltre::operator =(const JMTFR03KEYFiltre & Source)
{
	// on teste si on a quelque chose � faire
	if (this != &Source)
	{
		// on recopie l'�l�ment
		m_IdFiltre = Source.m_IdFiltre;
		m_IdTitre  = Source.m_IdTitre;
	}

	// on renvoie notre r�f�rence
	return (*this);
}

////////////////////////////////////////////
// les op�rateurs pour comparer les �l�ments

JBool JMTFR03KEYFiltre::operator <(const JMTFR03KEYFiltre & Reference) const
{
	// on compare la premi�re composante
	if (m_IdFiltre < Reference.m_IdFiltre) return (true);
	if (m_IdFiltre > Reference.m_IdFiltre) return (false);

	// on compare la derni�re composante
	return (m_IdTitre < Reference.m_IdTitre);
}

JBool JMTFR03KEYFiltre::operator >(const JMTFR03KEYFiltre & Reference) const
{
	// on compare la premi�re composante
	if (m_IdFiltre > Reference.m_IdFiltre) return (true);
	if (m_IdFiltre < Reference.m_IdFiltre) return (false);

	// on compare la derni�re composante
	return (m_IdTitre > Reference.m_IdTitre);
}

///////////////////////////////////////////
// la fonction pour r�initialiser l'�l�ment

JVoid JMTFR03KEYFiltre::Reset()
{
	// on r�initialise l'�l�ment
	m_IdFiltre.Reset();
	m_IdTitre .Reset();
}

/////////////////
// le destructeur

JMTFR03KEYFiltre::~JMTFR03KEYFiltre()
{
	// on ne fait rien
}
