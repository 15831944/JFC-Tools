//
// Fichier: JMTFR03DATUnivers.h
// Auteur:  Sylvain SAMMURI
// Date:    10/09/2003
//

// on v�rifie que le fichier n'a pas d�j� �t� inclus
#ifndef JMTFR03_DAT_UNIVERS_H

// on d�finit la macro pour �viter les inclusions multiples
#define JMTFR03_DAT_UNIVERS_H

// on inclut les d�finitions n�cessaires
#include "JLib.h"

class JMTFR03DATUnivers
{
public:
	// le constructeur
	JMTFR03DATUnivers();

	// le destructeur
	~JMTFR03DATUnivers();

private:
	// l'op�rateur pour recopier l'�l�ment
	JMTFR03DATUnivers & operator =(const JMTFR03DATUnivers & Source);

	// le constructeur copie
	JMTFR03DATUnivers(const JMTFR03DATUnivers & Source);

public:
	JBitArray m_ItemIndividus; // le vecteur des individus
	JMsk32    m_Etat;          // l'�tat de l'univers
};

// fin de l'inclusion conditionnelle
#endif
