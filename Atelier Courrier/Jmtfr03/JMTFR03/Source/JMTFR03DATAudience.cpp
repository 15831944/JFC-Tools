//
// Fichier: JMTFR03DATAudience.cpp
// Auteur:  Sylvain SAMMURI
// Date:    29/08/2003
//

// on inclut les définitions nécessaires
#include "JMTFR03DATAudience.h"

////////////////////
// les constructeurs

JMTFR03DATAudience::JMTFR03DATAudience()
{
	// on ne fait rien
}

JMTFR03DATAudience::JMTFR03DATAudience(const JMTFR03DATAudience & Source)
{
	// on vérifie l'existence de la source
	if (this == &Source) throw JInvalidCall::GetInstance();

	// on recopie l'élément
	m_AudienceRef   = Source.m_AudienceRef;
	m_AudienceProba = Source.m_AudienceProba;
	m_VolLecture1Jr = Source.m_VolLecture1Jr;
	m_PtEtalonnage  = Source.m_PtEtalonnage;
}

//////////////////////////////////////
// l'opérateur pour recopier l'élément

JMTFR03DATAudience & JMTFR03DATAudience::operator =(const JMTFR03DATAudience & Source)
{
	// on teste si on a quelque chose à faire
	if (this != &Source)
	{
		// on recopie l'élément
		m_AudienceRef   = Source.m_AudienceRef;
		m_AudienceProba = Source.m_AudienceProba;
		m_VolLecture1Jr = Source.m_VolLecture1Jr;
		m_PtEtalonnage  = Source.m_PtEtalonnage;
	}

	// on renvoie notre référence
	return (*this);
}

/////////////////
// le destructeur

JMTFR03DATAudience::~JMTFR03DATAudience()
{
	// on ne fait rien
}
