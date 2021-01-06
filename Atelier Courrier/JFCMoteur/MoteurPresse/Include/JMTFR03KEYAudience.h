//
// Fichier: JMTFR03KEYAudience.h
// Auteur:  Sylvain SAMMURI
// Date:    05/12/2003
//

// on vérifie que le fichier n'a pas déjà été inclus
#ifndef JMTFR03_KEY_AUDIENCE_H

// on définit la macro pour éviter les inclusions multiples
#define JMTFR03_KEY_AUDIENCE_H

// on inclut les définitions nécessaires
#include "JLib.h"

class JMTFR03KEYAudience
{
public:
	// les constructeurs
	JMTFR03KEYAudience();
	JMTFR03KEYAudience(const JMTFR03KEYAudience & Source);

	// la fonction pour tester l'élément
	JBool IsValid() const;

	// l'opérateur pour recopier l'élément
	JMTFR03KEYAudience & operator =(const JMTFR03KEYAudience & Source);

	// les opérateurs pour comparer les éléments
	JBool operator <(const JMTFR03KEYAudience & Reference) const;
	JBool operator >(const JMTFR03KEYAudience & Reference) const;

	// la fonction pour réinitialiser l'élément
	JVoid Reset();

	// le destructeur
	~JMTFR03KEYAudience();

public:
	JRef32 m_IdSource;  // l'identifiant de la source
	JRef32 m_IdTerrain; // l'identifiant du terrain
	JRef32 m_IdCible;   // l'identifiant de la cible
	JRef32 m_IdTitre;   // l'identifiant du titre
};

// fin de l'inclusion conditionnelle
#endif
