//
// Fichier: JMTFR03DATCible.h
// Auteur:  Sylvain SAMMURI
// Date:    09/12/2003
//

// on vérifie que le fichier n'a pas déjà été inclus
#ifndef JMTFR03_DAT_CIBLE_H

// on définit la macro pour éviter les inclusions multiples
#define JMTFR03_DAT_CIBLE_H

// on inclut les définitions nécessaires
#include "JLib.h"

class JMTFR03DATCible
{
public:
	// le constructeur
	JMTFR03DATCible();

	// le destructeur
	~JMTFR03DATCible();

private:
	// l'opérateur pour recopier l'élément
	JMTFR03DATCible & operator =(const JMTFR03DATCible & Source);

	// le constructeur copie
	JMTFR03DATCible(const JMTFR03DATCible & Source);

public:
	JArray<JFlt64x> m_TBLProbas; // la table des éléments
};

// fin de l'inclusion conditionnelle
#endif
