//
// Fichier: JMTFR03DATCible.h
// Auteur:  Sylvain SAMMURI
// Date:    09/12/2003
//

// on v�rifie que le fichier n'a pas d�j� �t� inclus
#ifndef JMTFR03_DAT_CIBLE_H

// on d�finit la macro pour �viter les inclusions multiples
#define JMTFR03_DAT_CIBLE_H

// on inclut les d�finitions n�cessaires
#include "JLib.h"

class JMTFR03DATCible
{
public:
	// le constructeur
	JMTFR03DATCible();

	// le destructeur
	~JMTFR03DATCible();

private:
	// l'op�rateur pour recopier l'�l�ment
	JMTFR03DATCible & operator =(const JMTFR03DATCible & Source);

	// le constructeur copie
	JMTFR03DATCible(const JMTFR03DATCible & Source);

public:
	JArray<JFlt64x> m_TBLProbas; // la table des �l�ments
};

// fin de l'inclusion conditionnelle
#endif
