//
// Fichier: JMTFR03DATPoids.h
// Auteur:  Sylvain SAMMURI
// Date:    10/09/2003
//

// on vérifie que le fichier n'a pas déjà été inclus
#ifndef JMTFR03_DAT_POIDS_H

// on définit la macro pour éviter les inclusions multiples
#define JMTFR03_DAT_POIDS_H

// on inclut les définitions nécessaires
#include "JLib.h"

class JMTFR03DATPoids
{
public:
	// le constructeur
	JMTFR03DATPoids();

	// le destructeur
	~JMTFR03DATPoids();

private:
	// l'opérateur pour recopier l'élément
	JMTFR03DATPoids & operator =(const JMTFR03DATPoids & Source);

	// le constructeur copie
	JMTFR03DATPoids(const JMTFR03DATPoids & Source);

public:
	JArray<JInt32x> m_ItemPoids; // le vecteur des poids des individus
	JInt32x         m_Diviseur;  // le diviseur
};

// fin de l'inclusion conditionnelle
#endif
