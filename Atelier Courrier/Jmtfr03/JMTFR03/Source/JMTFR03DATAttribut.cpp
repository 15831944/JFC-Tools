//
// Fichier: JMTFR03DATAttribut.cpp
// Auteur:  Sylvain SAMMURI
// Date:    10/09/2003
//

// on inclut les d�finitions n�cessaires
#include "JMTFR03DATAttribut.h"

////////////////////
// les constructeurs

JMTFR03DATAttribut::JMTFR03DATAttribut()
{
	// on ne fait rien
}

JMTFR03DATAttribut::JMTFR03DATAttribut(const JMTFR03DATAttribut & Source)
{
	// on g�n�re une exception
	throw JInvalidCall::GetInstance();
}

//////////////////////////////////////
// l'op�rateur pour recopier l'�l�ment

JMTFR03DATAttribut & JMTFR03DATAttribut::operator =(const JMTFR03DATAttribut & Source)
{
	// on g�n�re une exception
	throw JInvalidCall::GetInstance();
}

/////////////////
// le destructeur

JMTFR03DATAttribut::~JMTFR03DATAttribut()
{
	// on ne fait rien
}
