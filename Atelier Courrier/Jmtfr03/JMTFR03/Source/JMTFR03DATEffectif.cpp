//
// Fichier: JMTFR03DATEffectif.cpp
// Auteur:  Sylvain SAMMURI
// Date:    05/12/2003
//

// on inclut les d�finitions n�cessaires
#include "JMTFR03DATEffectif.h"

////////////////////
// les constructeurs

JMTFR03DATEffectif::JMTFR03DATEffectif()
{
	// on ne fait rien
}

JMTFR03DATEffectif::JMTFR03DATEffectif(const JMTFR03DATEffectif & Source)
{
	// on v�rifie l'existence de la source
	if (this == &Source) throw JInvalidCall::GetInstance();

	// on recopie l'�l�ment
	m_Effectif = Source.m_Effectif;
	m_NbCas    = Source.m_NbCas;	
}

//////////////////////////////////////
// l'op�rateur pour recopier l'�l�ment

JMTFR03DATEffectif & JMTFR03DATEffectif::operator =(const JMTFR03DATEffectif & Source)
{
	// on teste si on a quelque chose � faire
	if (this != &Source)
	{
		// on recopie l'�l�ment
		m_Effectif = Source.m_Effectif;
		m_NbCas    = Source.m_NbCas;
	}

	// on renvoie notre r�f�rence
	return (*this);
}

/////////////////
// le destructeur

JMTFR03DATEffectif::~JMTFR03DATEffectif()
{
	// on ne fait rien
}
