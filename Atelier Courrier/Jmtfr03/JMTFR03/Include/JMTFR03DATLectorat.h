//
// Fichier: JMTFR03DATLectorat.h
// Auteur:  Sylvain SAMMURI
// Date:    02/12/2003
//

// on v�rifie que le fichier n'a pas d�j� �t� inclus
#ifndef JMTFR03_DAT_LECTORAT_H

// on d�finit la macro pour �viter les inclusions multiples
#define JMTFR03_DAT_LECTORAT_H

// on inclut les d�finitions n�cessaires
#include "JLib.h"

class JMTFR03DATLectorat
{
public:
	// le constructeur
	JMTFR03DATLectorat();

	// le destructeur
	~JMTFR03DATLectorat();

private:
	// l'op�rateur pour recopier l'�l�ment
	JMTFR03DATLectorat & operator =(const JMTFR03DATLectorat & Source);

	// le constructeur copie
	JMTFR03DATLectorat(const JMTFR03DATLectorat & Source);

public:
	JRef32    m_IdUnivers;     // l'identifiant de l'univers
	JBitArray m_ItemIndividus; // le vecteur des individus
};

// fin de l'inclusion conditionnelle
#endif
