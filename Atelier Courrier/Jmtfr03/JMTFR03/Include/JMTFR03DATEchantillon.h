//
// Fichier: JMTFR03DATEchantillon.h
// Auteur:  Sylvain SAMMURI
// Date:    10/09/2003
//

// on v�rifie que le fichier n'a pas d�j� �t� inclus
#ifndef JMTFR03_DAT_ECHANTILLON_H

// on d�finit la macro pour �viter les inclusions multiples
#define JMTFR03_DAT_ECHANTILLON_H

// on inclut les d�finitions n�cessaires
#include "JLib.h"

class JMTFR03DATEchantillon
{
public:
	// le constructeur
	JMTFR03DATEchantillon();

	// le destructeur
	~JMTFR03DATEchantillon();

private:
	// l'op�rateur pour recopier l'�l�ment
	JMTFR03DATEchantillon & operator =(const JMTFR03DATEchantillon & Source);

	// le constructeur copie
	JMTFR03DATEchantillon(const JMTFR03DATEchantillon & Source);

public:
	JBitArray m_ItemIndividus; // le vecteur des individus
	JMsk32    m_Masque;        // le masque de positionnement
};

// fin de l'inclusion conditionnelle
#endif
