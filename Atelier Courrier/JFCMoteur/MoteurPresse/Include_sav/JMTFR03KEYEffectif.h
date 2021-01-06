//
// Fichier: JMTFR03KEYEffectif.h
// Auteur:  Sylvain SAMMURI
// Date:    05/12/2003
//

// on vérifie que le fichier n'a pas déjà été inclus
#ifndef JMTFR03_KEY_EFFECTIF_H

// on définit la macro pour éviter les inclusions multiples
#define JMTFR03_KEY_EFFECTIF_H

// on inclut les définitions nécessaires
#include "JLib.h"

class JMTFR03KEYEffectif
{
public:
	// les constructeurs
	JMTFR03KEYEffectif();
	JMTFR03KEYEffectif(const JMTFR03KEYEffectif & Source);

	// la fonction pour tester l'élément
	JBool IsValid() const;

	// l'opérateur pour recopier l'élément
	JMTFR03KEYEffectif & operator =(const JMTFR03KEYEffectif & Source);

	// les opérateurs pour comparer les éléments
	JBool operator <(const JMTFR03KEYEffectif & Reference) const;
	JBool operator >(const JMTFR03KEYEffectif & Reference) const;

	// la fonction pour réinitialiser l'élément
	JVoid Reset();

	// le destructeur
	~JMTFR03KEYEffectif();

public:
	JRef32 m_IdSource;  // l'identifiant de la source
	JRef32 m_IdTerrain; // l'identifiant du terrain
	JRef32 m_IdCible;   // l'identifiant de la cible
};

// fin de l'inclusion conditionnelle
#endif
