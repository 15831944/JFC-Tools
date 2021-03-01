//
// Fichier: JMTFR03DATEchantillon.cpp
// Auteur:  Sylvain SAMMURI
// Date:    10/09/2003
//

// on inclut les définitions nécessaires
#include "JMTFR03DATEchantillon.h"

////////////////////
// les constructeurs

JMTFR03DATEchantillon::JMTFR03DATEchantillon()
{
	// on ne fait rien
}

JMTFR03DATEchantillon::JMTFR03DATEchantillon(const JMTFR03DATEchantillon & Source)
{
	// on génère une exception
	throw JInvalidCall::GetInstance();
}

//////////////////////////////////////
// l'opérateur pour recopier l'élément

JMTFR03DATEchantillon & JMTFR03DATEchantillon::operator =(const JMTFR03DATEchantillon & Source)
{
	// on génère une exception
	throw JInvalidCall::GetInstance();
}

/////////////////
// le destructeur

JMTFR03DATEchantillon::~JMTFR03DATEchantillon()
{
	// on ne fait rien
}
