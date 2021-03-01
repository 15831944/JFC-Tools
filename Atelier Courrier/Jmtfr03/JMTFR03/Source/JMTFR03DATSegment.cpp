//
// Fichier: JMTFR03DATSegment.cpp
// Auteur:  Sylvain SAMMURI
// Date:    10/09/2003
//

// on inclut les définitions nécessaires
#include "JMTFR03DATSegment.h"

////////////////////
// les constructeurs

JMTFR03DATSegment::JMTFR03DATSegment()
{
	// on ne fait rien
}

JMTFR03DATSegment::JMTFR03DATSegment(const JMTFR03DATSegment & Source)
{
	// on génère une exception
	throw JInvalidCall::GetInstance();
}

//////////////////////////////////////
// l'opérateur pour recopier l'élément

JMTFR03DATSegment & JMTFR03DATSegment::operator =(const JMTFR03DATSegment & Source)
{
	// on génère une exception
	throw JInvalidCall::GetInstance();
}

/////////////////
// le destructeur

JMTFR03DATSegment::~JMTFR03DATSegment()
{
	// on ne fait rien
}
