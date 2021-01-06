//
// Fichier: JMTFR03KEYPoids.h
// Auteur:  Sylvain SAMMURI
// Date:    10/09/2003
//

// on vérifie que le fichier n'a pas déjà été inclus
#ifndef JMTFR03_KEY_POIDS_H

// on définit la macro pour éviter les inclusions multiples
#define JMTFR03_KEY_POIDS_H

// on inclut les définitions nécessaires
#include "JLib.h"

class JMTFR03KEYPoids
{
public:
	// les constructeurs
	JMTFR03KEYPoids();
	JMTFR03KEYPoids(const JMTFR03KEYPoids & Source);

	// la fonction pour tester l'élément
	JBool IsValid() const;

	// l'opérateur pour recopier l'élément
	JMTFR03KEYPoids & operator =(const JMTFR03KEYPoids & Source);

	// les opérateurs pour comparer les éléments
	JBool operator <(const JMTFR03KEYPoids & Reference) const;
	JBool operator >(const JMTFR03KEYPoids & Reference) const;

	// la fonction pour réinitialiser l'élément
	JVoid Reset();

	// le destructeur
	~JMTFR03KEYPoids();

public:
	JRef32 m_IdPoids;   // l'identifiant du type de poids
	JRef32 m_IdUnivers; // l'identifiant de l'univers de redressement
};

// fin de l'inclusion conditionnelle
#endif
