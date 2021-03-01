//
// Fichier: JMTFR03DATHomothetie.cpp
// Auteur:  Sylvain SAMMURI
// Date:    10/09/2003
//

// on inclut les définitions nécessaires
#include "JMTFR03DATHomothetie.h"

////////////////////
// les constructeurs

JMTFR03DATHomothetie::JMTFR03DATHomothetie()
{
	// on ne fait rien
}

JMTFR03DATHomothetie::JMTFR03DATHomothetie(const JMTFR03DATHomothetie & Source)
{
	// on génère une exception
	throw JInvalidCall::GetInstance();
}

//////////////////////////////////////
// l'opérateur pour recopier l'élément

JMTFR03DATHomothetie & JMTFR03DATHomothetie::operator =(const JMTFR03DATHomothetie & Source)
{
	// on génère une exception
	throw JInvalidCall::GetInstance();
}

/////////////////
// le destructeur

JMTFR03DATHomothetie::~JMTFR03DATHomothetie()
{
	// on ne fait rien
}
