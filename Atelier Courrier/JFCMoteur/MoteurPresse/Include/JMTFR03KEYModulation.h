//
// Fichier: JMTFR03KEYModulation.h
// Auteur:  Sylvain SAMMURI
// Date:    18/06/2004
//

// on vérifie que le fichier n'a pas déjà été inclus
#ifndef JMTFR03_KEY_MODULATION_H

// on définit la macro pour éviter les inclusions multiples
#define JMTFR03_KEY_MODULATION_H

// on inclut les définitions nécessaires
#include "JLib.h"

class JMTFR03KEYModulation
{
public:
	// les constructeurs
	JMTFR03KEYModulation();
	JMTFR03KEYModulation(const JMTFR03KEYModulation & Source);

	// la fonction pour tester l'élément
	JBool IsValid() const;

	// l'opérateur pour recopier l'élément
	JMTFR03KEYModulation & operator =(const JMTFR03KEYModulation & Source);

	// les opérateurs pour comparer les éléments
	JBool operator <(const JMTFR03KEYModulation & Reference) const;
	JBool operator >(const JMTFR03KEYModulation & Reference) const;

	// la fonction pour réinitialiser l'élément
	JVoid Reset();

	// le destructeur
	~JMTFR03KEYModulation();

public:
	JRef32 m_IdSource;  // l'identifiant de la source
	JRef32 m_IdTerrain; // l'identifiant du terrain
};

// fin de l'inclusion conditionnelle
#endif
