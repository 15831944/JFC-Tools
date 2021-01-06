//
// Fichier: JMTFR03DATReprise.h
// Auteur:  Sylvain SAMMURI
// Date:    06/04/2004
//

// on vérifie que le fichier n'a pas déjà été inclus
#ifndef JMTFR03_DAT_REPRISE_H

// on définit la macro pour éviter les inclusions multiples
#define JMTFR03_DAT_REPRISE_H

// on inclut les définitions nécessaires
#include "JLib.h"

class JMTFR03DATReprise
{
public:
	// le constructeur
	JMTFR03DATReprise();

	// le destructeur
	~JMTFR03DATReprise();

private:
	// l'opérateur pour recopier l'élément
	JMTFR03DATReprise & operator =(const JMTFR03DATReprise & Source);

	// le constructeur copie
	JMTFR03DATReprise(const JMTFR03DATReprise & Source);

public:
	JRef32          m_IdUnivers;     // l'identifiant de l'univers
	JBitArray       m_ItemIndividus; // le vecteur des individus
	JArray<JInt32x> m_ItemReprises;  // le vecteur des reprises
	JInt32x         m_Diviseur;      // le diviseur
};

// fin de l'inclusion conditionnelle
#endif
