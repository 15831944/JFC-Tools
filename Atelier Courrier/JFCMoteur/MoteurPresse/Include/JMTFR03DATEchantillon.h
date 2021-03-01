//
// Fichier: JMTFR03DATEchantillon.h
// Auteur:  Sylvain SAMMURI
// Date:    10/09/2003
//

// on vérifie que le fichier n'a pas déjà été inclus
#ifndef JMTFR03_DAT_ECHANTILLON_H

// on définit la macro pour éviter les inclusions multiples
#define JMTFR03_DAT_ECHANTILLON_H

// on inclut les définitions nécessaires
#include "JLib.h"

class JMTFR03DATEchantillon
{
public:
	// le constructeur
	JMTFR03DATEchantillon();

	// le destructeur
	~JMTFR03DATEchantillon();

private:
	// l'opérateur pour recopier l'élément
	JMTFR03DATEchantillon & operator =(const JMTFR03DATEchantillon & Source);

	// le constructeur copie
	JMTFR03DATEchantillon(const JMTFR03DATEchantillon & Source);

public:
	JBitArray m_ItemIndividus; // le vecteur des individus
	JMsk32    m_Masque;        // le masque de positionnement
};

// fin de l'inclusion conditionnelle
#endif
