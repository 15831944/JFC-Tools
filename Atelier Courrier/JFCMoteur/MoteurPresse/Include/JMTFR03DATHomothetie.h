//
// Fichier: JMTFR03DATHomothetie.h
// Auteur:  Sylvain SAMMURI
// Date:    10/09/2003
//

// on v�rifie que le fichier n'a pas d�j� �t� inclus
#ifndef JMTFR03_DAT_HOMOTHETIE_H

// on d�finit la macro pour �viter les inclusions multiples
#define JMTFR03_DAT_HOMOTHETIE_H

// on inclut les d�finitions n�cessaires
#include "JLib.h"

class JMTFR03DATHomothetie
{
public:
	// le constructeur
	JMTFR03DATHomothetie();

	// le destructeur
	~JMTFR03DATHomothetie();

private:
	// l'op�rateur pour recopier l'�l�ment
	JMTFR03DATHomothetie & operator =(const JMTFR03DATHomothetie & Source);

	// le constructeur copie
	JMTFR03DATHomothetie(const JMTFR03DATHomothetie & Source);

public:
	JInt32x m_Homothetie; // le facteur d'homoth�tie
	JInt32x m_Diviseur;   // le diviseur
};

// fin de l'inclusion conditionnelle
#endif
