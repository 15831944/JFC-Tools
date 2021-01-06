//
// Fichier: JMTFR03DATVariable.cpp
// Auteur:  Sylvain SAMMURI
// Date:    10/09/2003
//

// on inclut les d�finitions n�cessaires
#include "JMTFR03DATVariable.h"

////////////////////
// les constructeurs

JMTFR03DATVariable::JMTFR03DATVariable()
{
	// on ne fait rien
}

JMTFR03DATVariable::JMTFR03DATVariable(const JMTFR03DATVariable & Source)
{
	// on g�n�re une exception
	throw JInvalidCall::GetInstance();
}

//////////////////////////////////////
// l'op�rateur pour recopier l'�l�ment

JMTFR03DATVariable & JMTFR03DATVariable::operator =(const JMTFR03DATVariable & Source)
{
	// on g�n�re une exception
	throw JInvalidCall::GetInstance();
}

/////////////////
// le destructeur

JMTFR03DATVariable::~JMTFR03DATVariable()
{
	// on ne fait rien
}
