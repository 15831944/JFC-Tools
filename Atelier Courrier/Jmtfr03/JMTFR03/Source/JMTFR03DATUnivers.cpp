//
// Fichier: JMTFR03DATUnivers.cpp
// Auteur:  Sylvain SAMMURI
// Date:    10/09/2003
//

// on inclut les définitions nécessaires
#include "JMTFR03DATUnivers.h"

////////////////////
// les constructeurs

JMTFR03DATUnivers::JMTFR03DATUnivers()
{
	// on ne fait rien
}

JMTFR03DATUnivers::JMTFR03DATUnivers(const JMTFR03DATUnivers & Source)
{
	// on génère une exception
	throw JInvalidCall::GetInstance();
}

//////////////////////////////////////
// l'opérateur pour recopier l'élément

JMTFR03DATUnivers & JMTFR03DATUnivers::operator =(const JMTFR03DATUnivers & Source)
{
	// on génère une exception
	throw JInvalidCall::GetInstance();
}

/////////////////
// le destructeur

JMTFR03DATUnivers::~JMTFR03DATUnivers()
{
	// on ne fait rien
}
