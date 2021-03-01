//
// Fichier: JFR03Terrain.h
// Auteur:  Sylvain SAMMURI
// Date:    11/03/2003
//

// on v�rifie que le fichier n'a pas d�j� �t� inclus
#ifndef JFR03_TERRAIN_H

// on d�finit la macro pour �viter les inclusions multiples
#define JFR03_TERRAIN_H

// on inclut les d�finitions n�cessaires
#include "JLib.h"

class JFR03Terrain
{
public:
	// les constructeurs
	JFR03Terrain();
	JFR03Terrain(const JFR03Terrain & Source);

	// la fonction pour tester la validit� de l'�l�ment
	JBool IsValid() const;

	// l'op�rateur pour recopier l'�l�ment
	JFR03Terrain & operator =(const JFR03Terrain & Source);

	// la fonction pour r�initialiser l'�l�ment
	JVoid Reset();

	// le destructeur
	~JFR03Terrain();

public:
	JRef32 m_IdTerrain; // l'identifiant du terrain
	JLabel m_Libelle;   // le libell� du terrain
	JRef32 m_IdSource;  // l'identifiant de la source
	JPath  m_Racine;    // la racine du terrain
};

// fin de l'inclusion conditionnelle
#endif
