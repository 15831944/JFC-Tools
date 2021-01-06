//
// Fichier: JMTFR03DATPoids.cpp
// Auteur:  Sylvain SAMMURI
// Date:    10/09/2003
//

// on inclut les définitions nécessaires
#include "JMTFR03DATPoids.h"

////////////////////
// les constructeurs

JMTFR03DATPoids::JMTFR03DATPoids()
{
	// on ne fait rien
}

JMTFR03DATPoids::JMTFR03DATPoids(const JMTFR03DATPoids & Source)
{
	// on génère une exception
	throw JInvalidCall::GetInstance();
}

//////////////////////////////////////
// l'opérateur pour recopier l'élément

JMTFR03DATPoids & JMTFR03DATPoids::operator =(const JMTFR03DATPoids & Source)
{
	// on génère une exception
	throw JInvalidCall::GetInstance();
}

/////////////////
// le destructeur

JMTFR03DATPoids::~JMTFR03DATPoids()
{
	// on ne fait rien
}
