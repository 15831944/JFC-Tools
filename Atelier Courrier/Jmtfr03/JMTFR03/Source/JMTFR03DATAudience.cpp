//
// Fichier: JMTFR03DATAudience.cpp
// Auteur:  Sylvain SAMMURI
// Date:    29/08/2003
//

// on inclut les d�finitions n�cessaires
#include "JMTFR03DATAudience.h"

////////////////////
// les constructeurs

JMTFR03DATAudience::JMTFR03DATAudience()
{
	// on ne fait rien
}

JMTFR03DATAudience::JMTFR03DATAudience(const JMTFR03DATAudience & Source)
{
	// on v�rifie l'existence de la source
	if (this == &Source) throw JInvalidCall::GetInstance();

	// on recopie l'�l�ment
	m_AudienceRef   = Source.m_AudienceRef;
	m_AudienceProba = Source.m_AudienceProba;
	m_VolLecture1Jr = Source.m_VolLecture1Jr;
	m_PtEtalonnage  = Source.m_PtEtalonnage;
}

//////////////////////////////////////
// l'op�rateur pour recopier l'�l�ment

JMTFR03DATAudience & JMTFR03DATAudience::operator =(const JMTFR03DATAudience & Source)
{
	// on teste si on a quelque chose � faire
	if (this != &Source)
	{
		// on recopie l'�l�ment
		m_AudienceRef   = Source.m_AudienceRef;
		m_AudienceProba = Source.m_AudienceProba;
		m_VolLecture1Jr = Source.m_VolLecture1Jr;
		m_PtEtalonnage  = Source.m_PtEtalonnage;
	}

	// on renvoie notre r�f�rence
	return (*this);
}

/////////////////
// le destructeur

JMTFR03DATAudience::~JMTFR03DATAudience()
{
	// on ne fait rien
}
