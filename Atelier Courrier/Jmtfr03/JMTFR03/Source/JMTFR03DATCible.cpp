//
// Fichier: JMTFR03DATCible.cpp
// Auteur:  Sylvain SAMMURI
// Date:    09/12/2003
//

// on inclut les définitions nécessaires
#include "JMTFR03DATCible.h"

////////////////////
// les constructeurs

JMTFR03DATCible::JMTFR03DATCible()
{
	// on ne fait rien
}

JMTFR03DATCible::JMTFR03DATCible(const JMTFR03DATCible & Source)
{
	// on génère une exception
	throw JInvalidCall::GetInstance();
}

//////////////////////////////////////
// l'opérateur pour recopier l'élément

JMTFR03DATCible & JMTFR03DATCible::operator =(const JMTFR03DATCible & Source)
{
	// on génère une exception
	throw JInvalidCall::GetInstance();
}

/////////////////
// le destructeur

JMTFR03DATCible::~JMTFR03DATCible()
{
	// on ne fait rien
}
