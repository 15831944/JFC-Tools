//
// Fichier: JMTFR03DATFiltre.h
// Auteur:  Sylvain SAMMURI
// Date:    10/09/2003
//

// on v�rifie que le fichier n'a pas d�j� �t� inclus
#ifndef JMTFR03_DAT_FILTRE_H

// on d�finit la macro pour �viter les inclusions multiples
#define JMTFR03_DAT_FILTRE_H

// on inclut les d�finitions n�cessaires
#include "JLib.h"

class JMTFR03DATFiltre
{
public:
	// le constructeur
	JMTFR03DATFiltre();

	// le destructeur
	~JMTFR03DATFiltre();

private:
	// l'op�rateur pour recopier l'�l�ment
	JMTFR03DATFiltre & operator =(const JMTFR03DATFiltre & Source);

	// le constructeur copie
	JMTFR03DATFiltre(const JMTFR03DATFiltre & Source);

public:
	JBitArray m_ItemIndividus; // le vecteur des individus
};

// fin de l'inclusion conditionnelle
#endif
