//
// Fichier: JMTFR03DATFiltre.h
// Auteur:  Sylvain SAMMURI
// Date:    10/09/2003
//

// on vérifie que le fichier n'a pas déjà été inclus
#ifndef JMTFR03_DAT_FILTRE_H

// on définit la macro pour éviter les inclusions multiples
#define JMTFR03_DAT_FILTRE_H

// on inclut les définitions nécessaires
#include "JLib.h"

class JMTFR03DATFiltre
{
public:
	// le constructeur
	JMTFR03DATFiltre();

	// le destructeur
	~JMTFR03DATFiltre();

private:
	// l'opérateur pour recopier l'élément
	JMTFR03DATFiltre & operator =(const JMTFR03DATFiltre & Source);

	// le constructeur copie
	JMTFR03DATFiltre(const JMTFR03DATFiltre & Source);

public:
	JBitArray m_ItemIndividus; // le vecteur des individus
};

// fin de l'inclusion conditionnelle
#endif
