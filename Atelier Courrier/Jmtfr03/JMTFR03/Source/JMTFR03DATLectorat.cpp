//
// Fichier: JMTFR03DATLectorat.cpp
// Auteur:  Sylvain SAMMURI
// Date:    02/12/2003
//

// on inclut les définitions nécessaires
#include "JMTFR03DATLectorat.h"

////////////////////
// les constructeurs

JMTFR03DATLectorat::JMTFR03DATLectorat()
{
	// on ne fait rien
}

JMTFR03DATLectorat::JMTFR03DATLectorat(const JMTFR03DATLectorat & Source)
{
	// on génère une exception
	throw JInvalidCall::GetInstance();
}

//////////////////////////////////////
// l'opérateur pour recopier l'élément

JMTFR03DATLectorat & JMTFR03DATLectorat::operator =(const JMTFR03DATLectorat & Source)
{
	// on génère une exception
	throw JInvalidCall::GetInstance();
}

/////////////////
// le destructeur

JMTFR03DATLectorat::~JMTFR03DATLectorat()
{
	// on ne fait rien
}
