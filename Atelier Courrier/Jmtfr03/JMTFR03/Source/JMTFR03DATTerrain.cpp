//
// Fichier: JMTFR03DATTerrain.cpp
// Auteur:  Sylvain SAMMURI
// Date:    10/09/2003
//

// on inclut les définitions nécessaires
#include "JMTFR03DATTerrain.h"

////////////////////
// les constructeurs

JMTFR03DATTerrain::JMTFR03DATTerrain()
{
	// on ne fait rien
}

JMTFR03DATTerrain::JMTFR03DATTerrain(const JMTFR03DATTerrain & Source)
{
	// on génère une exception
	throw JInvalidCall::GetInstance();
}

//////////////////////////////////////
// l'opérateur pour recopier l'élément

JMTFR03DATTerrain & JMTFR03DATTerrain::operator =(const JMTFR03DATTerrain & Source)
{
	// on génère une exception
	throw JInvalidCall::GetInstance();
}

/////////////////
// le destructeur

JMTFR03DATTerrain::~JMTFR03DATTerrain()
{
	// on ne fait rien
}
