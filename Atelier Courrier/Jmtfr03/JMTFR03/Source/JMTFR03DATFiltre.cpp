//
// Fichier: JMTFR03DATFiltre.cpp
// Auteur:  Sylvain SAMMURI
// Date:    10/09/2003
//

// on inclut les d�finitions n�cessaires
#include "JMTFR03DATFiltre.h"

////////////////////
// les constructeurs

JMTFR03DATFiltre::JMTFR03DATFiltre()
{
	// on ne fait rien
}

JMTFR03DATFiltre::JMTFR03DATFiltre(const JMTFR03DATFiltre & Source)
{
	// on g�n�re une exception
	throw JInvalidCall::GetInstance();
}

//////////////////////////////////////
// l'op�rateur pour recopier l'�l�ment

JMTFR03DATFiltre & JMTFR03DATFiltre::operator =(const JMTFR03DATFiltre & Source)
{
	// on g�n�re une exception
	throw JInvalidCall::GetInstance();
}

/////////////////
// le destructeur

JMTFR03DATFiltre::~JMTFR03DATFiltre()
{
	// on ne fait rien
}
