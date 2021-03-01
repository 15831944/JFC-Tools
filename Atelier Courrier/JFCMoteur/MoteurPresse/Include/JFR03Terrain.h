//
// Fichier: JFR03Terrain.h
// Auteur:  Sylvain SAMMURI
// Date:    11/03/2003
//

// on vérifie que le fichier n'a pas déjà été inclus
#ifndef JFR03_TERRAIN_H

// on définit la macro pour éviter les inclusions multiples
#define JFR03_TERRAIN_H

// on inclut les définitions nécessaires
#include "JLib.h"

class JFR03Terrain
{
public:
	// les constructeurs
	JFR03Terrain();
	JFR03Terrain(const JFR03Terrain & Source);

	// la fonction pour tester la validité de l'élément
	JBool IsValid() const;

	// l'opérateur pour recopier l'élément
	JFR03Terrain & operator =(const JFR03Terrain & Source);

	// la fonction pour réinitialiser l'élément
	JVoid Reset();

	// le destructeur
	~JFR03Terrain();

public:
	JRef32 m_IdTerrain; // l'identifiant du terrain
	JLabel m_Libelle;   // le libellé du terrain
	JRef32 m_IdSource;  // l'identifiant de la source
	JPath  m_Racine;    // la racine du terrain
};

// fin de l'inclusion conditionnelle
#endif
