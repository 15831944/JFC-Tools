//
// Fichier: JMTFR03DATSegment.cpp
// Auteur:  Sylvain SAMMURI
// Date:    10/09/2003
//

// on inclut les d�finitions n�cessaires
#include "JMTFR03DATSegment.h"

////////////////////
// les constructeurs

JMTFR03DATSegment::JMTFR03DATSegment()
{
	// on ne fait rien
}

JMTFR03DATSegment::JMTFR03DATSegment(const JMTFR03DATSegment & Source)
{
	// on g�n�re une exception
	throw JInvalidCall::GetInstance();
}

//////////////////////////////////////
// l'op�rateur pour recopier l'�l�ment

JMTFR03DATSegment & JMTFR03DATSegment::operator =(const JMTFR03DATSegment & Source)
{
	// on g�n�re une exception
	throw JInvalidCall::GetInstance();
}

/////////////////
// le destructeur

JMTFR03DATSegment::~JMTFR03DATSegment()
{
	// on ne fait rien
}
