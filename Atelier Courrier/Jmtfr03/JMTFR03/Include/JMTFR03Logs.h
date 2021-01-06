//
// Fichier: JMTFR03Logs.h
// Auteur:  Sylvain SAMMURI
// Date:    02/12/2003
//

// on v�rifie que le fichier n'a pas d�j� �t� inclus
#ifndef JMTFR03_LOGS_H

// on d�finit la macro pour �viter les inclusions multiples
#define JMTFR03_LOGS_H

// on inclut les d�finitions n�cessaires
#include "JLib.h"

class JMTFR03Logs
{
public:
	// les constructeurs
	JMTFR03Logs();
	JMTFR03Logs(const JMTFR03Logs & Source);

	// l'op�rateur pour recopier les �l�ments
	JMTFR03Logs & operator =(const JMTFR03Logs & Source);

	// le destructeur
	virtual ~JMTFR03Logs();
};

// fin de l'inclusion conditionnelle
#endif
