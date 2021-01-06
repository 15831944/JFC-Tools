//
// Fichier: JMTFR03Logs.h
// Auteur:  Sylvain SAMMURI
// Date:    02/12/2003
//

// on vérifie que le fichier n'a pas déjà été inclus
#ifndef JMTFR03_LOGS_H

// on définit la macro pour éviter les inclusions multiples
#define JMTFR03_LOGS_H

// on inclut les définitions nécessaires
#include "JLib.h"

class JMTFR03Logs
{
public:
	// les constructeurs
	JMTFR03Logs();
	JMTFR03Logs(const JMTFR03Logs & Source);

	// l'opérateur pour recopier les éléments
	JMTFR03Logs & operator =(const JMTFR03Logs & Source);

	// le destructeur
	virtual ~JMTFR03Logs();
};

// fin de l'inclusion conditionnelle
#endif
