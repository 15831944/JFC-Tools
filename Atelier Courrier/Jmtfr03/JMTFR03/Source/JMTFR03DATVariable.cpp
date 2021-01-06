//
// Fichier: JMTFR03DATVariable.cpp
// Auteur:  Sylvain SAMMURI
// Date:    10/09/2003
//

// on inclut les définitions nécessaires
#include "JMTFR03DATVariable.h"

////////////////////
// les constructeurs

JMTFR03DATVariable::JMTFR03DATVariable()
{
	// on ne fait rien
}

JMTFR03DATVariable::JMTFR03DATVariable(const JMTFR03DATVariable & Source)
{
	// on génère une exception
	throw JInvalidCall::GetInstance();
}

//////////////////////////////////////
// l'opérateur pour recopier l'élément

JMTFR03DATVariable & JMTFR03DATVariable::operator =(const JMTFR03DATVariable & Source)
{
	// on génère une exception
	throw JInvalidCall::GetInstance();
}

/////////////////
// le destructeur

JMTFR03DATVariable::~JMTFR03DATVariable()
{
	// on ne fait rien
}
