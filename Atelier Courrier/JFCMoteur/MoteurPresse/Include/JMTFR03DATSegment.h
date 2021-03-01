//
// Fichier: JMTFR03DATSegment.h
// Auteur:  Sylvain SAMMURI
// Date:    10/09/2003
//

// on vérifie que le fichier n'a pas déjà été inclus
#ifndef JMTFR03_DAT_SEGMENT_H

// on définit la macro pour éviter les inclusions multiples
#define JMTFR03_DAT_SEGMENT_H

// on inclut les définitions nécessaires
#include "JLib.h"

class JMTFR03DATSegment
{
public:
	// le constructeur
	JMTFR03DATSegment();

	// le destructeur
	~JMTFR03DATSegment();

private:
	// l'opérateur pour recopier l'élément
	JMTFR03DATSegment & operator =(const JMTFR03DATSegment & Source);

	// le constructeur copie
	JMTFR03DATSegment(const JMTFR03DATSegment & Source);

public:
	JBitArray m_ItemIndividus; // le vecteur des individus
	JMsk32    m_Masque;        // le masque de positionnement
};

// fin de l'inclusion conditionnelle
#endif
