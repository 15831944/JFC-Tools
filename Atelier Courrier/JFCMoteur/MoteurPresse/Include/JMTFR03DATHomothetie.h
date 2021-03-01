//
// Fichier: JMTFR03DATHomothetie.h
// Auteur:  Sylvain SAMMURI
// Date:    10/09/2003
//

// on vérifie que le fichier n'a pas déjà été inclus
#ifndef JMTFR03_DAT_HOMOTHETIE_H

// on définit la macro pour éviter les inclusions multiples
#define JMTFR03_DAT_HOMOTHETIE_H

// on inclut les définitions nécessaires
#include "JLib.h"

class JMTFR03DATHomothetie
{
public:
	// le constructeur
	JMTFR03DATHomothetie();

	// le destructeur
	~JMTFR03DATHomothetie();

private:
	// l'opérateur pour recopier l'élément
	JMTFR03DATHomothetie & operator =(const JMTFR03DATHomothetie & Source);

	// le constructeur copie
	JMTFR03DATHomothetie(const JMTFR03DATHomothetie & Source);

public:
	JInt32x m_Homothetie; // le facteur d'homothétie
	JInt32x m_Diviseur;   // le diviseur
};

// fin de l'inclusion conditionnelle
#endif
