//
// Fichier: JMTFR03KEYTerrain.h
// Auteur:  Sylvain SAMMURI
// Date:    10/09/2003
//

// on vérifie que le fichier n'a pas déjà été inclus
#ifndef JMTFR03_KEY_TERRAIN_H

// on définit la macro pour éviter les inclusions multiples
#define JMTFR03_KEY_TERRAIN_H

// on inclut les définitions nécessaires
#include "JLib.h"

class JMTFR03KEYTerrain
{
public:
	// les constructeurs
	JMTFR03KEYTerrain();
	JMTFR03KEYTerrain(const JMTFR03KEYTerrain & Source);

	// la fonction pour tester l'élément
	JBool IsValid() const;

	// l'opérateur pour recopier l'élément
	JMTFR03KEYTerrain & operator =(const JMTFR03KEYTerrain & Source);

	// les opérateurs pour comparer les éléments
	JBool operator <(const JMTFR03KEYTerrain & Reference) const;
	JBool operator >(const JMTFR03KEYTerrain & Reference) const;

	// la fonction pour réinitialiser l'élément
	JVoid Reset();

	// le destructeur
	~JMTFR03KEYTerrain();

public:
	JRef32 m_IdSource;  // l'identifiant de la source
	JRef32 m_IdTerrain; // l'identifiant du terrain
};

// fin de l'inclusion conditionnelle
#endif
