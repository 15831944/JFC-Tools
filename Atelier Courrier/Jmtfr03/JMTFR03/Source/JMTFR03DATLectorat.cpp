//
// Fichier: JMTFR03DATLectorat.cpp
// Auteur:  Sylvain SAMMURI
// Date:    02/12/2003
//

// on inclut les d�finitions n�cessaires
#include "JMTFR03DATLectorat.h"

////////////////////
// les constructeurs

JMTFR03DATLectorat::JMTFR03DATLectorat()
{
	// on ne fait rien
}

JMTFR03DATLectorat::JMTFR03DATLectorat(const JMTFR03DATLectorat & Source)
{
	// on g�n�re une exception
	throw JInvalidCall::GetInstance();
}

//////////////////////////////////////
// l'op�rateur pour recopier l'�l�ment

JMTFR03DATLectorat & JMTFR03DATLectorat::operator =(const JMTFR03DATLectorat & Source)
{
	// on g�n�re une exception
	throw JInvalidCall::GetInstance();
}

/////////////////
// le destructeur

JMTFR03DATLectorat::~JMTFR03DATLectorat()
{
	// on ne fait rien
}
