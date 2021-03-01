//
// Fichier: JMTFR03DATReprise.cpp
// Auteur:  Sylvain SAMMURI
// Date:    06/04/2004
//

// on inclut les d�finitions n�cessaires
#include "JMTFR03DATReprise.h"

////////////////////
// les constructeurs

JMTFR03DATReprise::JMTFR03DATReprise()
{
	// on ne fait rien
}

JMTFR03DATReprise::JMTFR03DATReprise(const JMTFR03DATReprise & Source)
{
	// on g�n�re une exception
	throw JInvalidCall::GetInstance();
}

//////////////////////////////////////
// l'op�rateur pour recopier l'�l�ment

JMTFR03DATReprise & JMTFR03DATReprise::operator =(const JMTFR03DATReprise & Source)
{
	// on g�n�re une exception
	throw JInvalidCall::GetInstance();
}

/////////////////
// le destructeur

JMTFR03DATReprise::~JMTFR03DATReprise()
{
	// on ne fait rien
}
