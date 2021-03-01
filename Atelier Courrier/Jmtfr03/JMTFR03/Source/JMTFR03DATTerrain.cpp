//
// Fichier: JMTFR03DATTerrain.cpp
// Auteur:  Sylvain SAMMURI
// Date:    10/09/2003
//

// on inclut les d�finitions n�cessaires
#include "JMTFR03DATTerrain.h"

////////////////////
// les constructeurs

JMTFR03DATTerrain::JMTFR03DATTerrain()
{
	// on ne fait rien
}

JMTFR03DATTerrain::JMTFR03DATTerrain(const JMTFR03DATTerrain & Source)
{
	// on g�n�re une exception
	throw JInvalidCall::GetInstance();
}

//////////////////////////////////////
// l'op�rateur pour recopier l'�l�ment

JMTFR03DATTerrain & JMTFR03DATTerrain::operator =(const JMTFR03DATTerrain & Source)
{
	// on g�n�re une exception
	throw JInvalidCall::GetInstance();
}

/////////////////
// le destructeur

JMTFR03DATTerrain::~JMTFR03DATTerrain()
{
	// on ne fait rien
}
