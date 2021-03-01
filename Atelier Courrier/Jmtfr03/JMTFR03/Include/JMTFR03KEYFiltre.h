//
// Fichier: JMTFR03KEYFiltre.h
// Auteur:  Sylvain SAMMURI
// Date:    10/09/2003
//

// on vérifie que le fichier n'a pas déjà été inclus
#ifndef JMTFR03_KEY_FILTRE_H

// on définit la macro pour éviter les inclusions multiples
#define JMTFR03_KEY_FILTRE_H

// on inclut les définitions nécessaires
#include "JLib.h"

class JMTFR03KEYFiltre
{
public:
	// les constructeurs
	JMTFR03KEYFiltre();
	JMTFR03KEYFiltre(const JMTFR03KEYFiltre & Source);

	// la fonction pour tester l'élément
	JBool IsValid() const;

	// l'opérateur pour recopier l'élément
	JMTFR03KEYFiltre & operator =(const JMTFR03KEYFiltre & Source);

	// les opérateurs pour comparer les éléments
	JBool operator <(const JMTFR03KEYFiltre & Reference) const;
	JBool operator >(const JMTFR03KEYFiltre & Reference) const;

	// la fonction pour réinitialiser l'élément
	JVoid Reset();

	// le destructeur
	~JMTFR03KEYFiltre();

public:
	JRef32 m_IdFiltre; // l'identifiant du filtre d'audience
	JRef32 m_IdTitre;  // l'identifiant du titre
};

// fin de l'inclusion conditionnelle
#endif
