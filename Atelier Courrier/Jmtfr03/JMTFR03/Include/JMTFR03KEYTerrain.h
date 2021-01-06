//
// Fichier: JMTFR03KEYTerrain.h
// Auteur:  Sylvain SAMMURI
// Date:    10/09/2003
//

// on v�rifie que le fichier n'a pas d�j� �t� inclus
#ifndef JMTFR03_KEY_TERRAIN_H

// on d�finit la macro pour �viter les inclusions multiples
#define JMTFR03_KEY_TERRAIN_H

// on inclut les d�finitions n�cessaires
#include "JLib.h"

class JMTFR03KEYTerrain
{
public:
	// les constructeurs
	JMTFR03KEYTerrain();
	JMTFR03KEYTerrain(const JMTFR03KEYTerrain & Source);

	// la fonction pour tester l'�l�ment
	JBool IsValid() const;

	// l'op�rateur pour recopier l'�l�ment
	JMTFR03KEYTerrain & operator =(const JMTFR03KEYTerrain & Source);

	// les op�rateurs pour comparer les �l�ments
	JBool operator <(const JMTFR03KEYTerrain & Reference) const;
	JBool operator >(const JMTFR03KEYTerrain & Reference) const;

	// la fonction pour r�initialiser l'�l�ment
	JVoid Reset();

	// le destructeur
	~JMTFR03KEYTerrain();

public:
	JRef32 m_IdSource;  // l'identifiant de la source
	JRef32 m_IdTerrain; // l'identifiant du terrain
};

// fin de l'inclusion conditionnelle
#endif
