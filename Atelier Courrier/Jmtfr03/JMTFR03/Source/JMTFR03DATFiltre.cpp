//
// Fichier: JMTFR03DATFiltre.cpp
// Auteur:  Sylvain SAMMURI
// Date:    10/09/2003
//

// on inclut les définitions nécessaires
#include "JMTFR03DATFiltre.h"

////////////////////
// les constructeurs

JMTFR03DATFiltre::JMTFR03DATFiltre()
{
	// on ne fait rien
}

JMTFR03DATFiltre::JMTFR03DATFiltre(const JMTFR03DATFiltre & Source)
{
	// on génère une exception
	throw JInvalidCall::GetInstance();
}

//////////////////////////////////////
// l'opérateur pour recopier l'élément

JMTFR03DATFiltre & JMTFR03DATFiltre::operator =(const JMTFR03DATFiltre & Source)
{
	// on génère une exception
	throw JInvalidCall::GetInstance();
}

/////////////////
// le destructeur

JMTFR03DATFiltre::~JMTFR03DATFiltre()
{
	// on ne fait rien
}
