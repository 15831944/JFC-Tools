//
// Fichier: JMTFR03DATUnivers.h
// Auteur:  Sylvain SAMMURI
// Date:    10/09/2003
//

// on vérifie que le fichier n'a pas déjà été inclus
#ifndef JMTFR03_DAT_UNIVERS_H

// on définit la macro pour éviter les inclusions multiples
#define JMTFR03_DAT_UNIVERS_H

// on inclut les définitions nécessaires
#include "JLib.h"

class JMTFR03DATUnivers
{
public:
	// le constructeur
	JMTFR03DATUnivers();

	// le destructeur
	~JMTFR03DATUnivers();

private:
	// l'opérateur pour recopier l'élément
	JMTFR03DATUnivers & operator =(const JMTFR03DATUnivers & Source);

	// le constructeur copie
	JMTFR03DATUnivers(const JMTFR03DATUnivers & Source);

public:
	JBitArray m_ItemIndividus; // le vecteur des individus
	JMsk32    m_Etat;          // l'état de l'univers
};

// fin de l'inclusion conditionnelle
#endif
