//
// Fichier: JMTFR03DATLectorat.h
// Auteur:  Sylvain SAMMURI
// Date:    02/12/2003
//

// on vérifie que le fichier n'a pas déjà été inclus
#ifndef JMTFR03_DAT_LECTORAT_H

// on définit la macro pour éviter les inclusions multiples
#define JMTFR03_DAT_LECTORAT_H

// on inclut les définitions nécessaires
#include "JLib.h"

class JMTFR03DATLectorat
{
public:
	// le constructeur
	JMTFR03DATLectorat();

	// le destructeur
	~JMTFR03DATLectorat();

private:
	// l'opérateur pour recopier l'élément
	JMTFR03DATLectorat & operator =(const JMTFR03DATLectorat & Source);

	// le constructeur copie
	JMTFR03DATLectorat(const JMTFR03DATLectorat & Source);

public:
	JRef32    m_IdUnivers;     // l'identifiant de l'univers
	JBitArray m_ItemIndividus; // le vecteur des individus
};

// fin de l'inclusion conditionnelle
#endif
