//
// Fichier: JMTFR03DATModulation.cpp
// Auteur:  Sylvain SAMMURI
// Date:    18/06/2004
//

// on inclut les d�finitions n�cessaires
#include "JMTFR03DATModulation.h"

////////////////////
// les constructeurs

JMTFR03DATModulation::JMTFR03DATModulation()
{
	// on ne fait rien
}

JMTFR03DATModulation::JMTFR03DATModulation(const JMTFR03DATModulation & Source)
{
	// on g�n�re une exception
	throw JInvalidCall::GetInstance();
}

//////////////////////////////////////
// l'op�rateur pour recopier l'�l�ment

JMTFR03DATModulation & JMTFR03DATModulation::operator =(const JMTFR03DATModulation & Source)
{
	// on g�n�re une exception
	throw JInvalidCall::GetInstance();
}

/////////////////
// le destructeur

JMTFR03DATModulation::~JMTFR03DATModulation()
{
	// on ne fait rien
}
