//
// Fichier: JMTFR03DATPoids.cpp
// Auteur:  Sylvain SAMMURI
// Date:    10/09/2003
//

// on inclut les d�finitions n�cessaires
#include "JMTFR03DATPoids.h"

////////////////////
// les constructeurs

JMTFR03DATPoids::JMTFR03DATPoids()
{
	// on ne fait rien
}

JMTFR03DATPoids::JMTFR03DATPoids(const JMTFR03DATPoids & Source)
{
	// on g�n�re une exception
	throw JInvalidCall::GetInstance();
}

//////////////////////////////////////
// l'op�rateur pour recopier l'�l�ment

JMTFR03DATPoids & JMTFR03DATPoids::operator =(const JMTFR03DATPoids & Source)
{
	// on g�n�re une exception
	throw JInvalidCall::GetInstance();
}

/////////////////
// le destructeur

JMTFR03DATPoids::~JMTFR03DATPoids()
{
	// on ne fait rien
}
