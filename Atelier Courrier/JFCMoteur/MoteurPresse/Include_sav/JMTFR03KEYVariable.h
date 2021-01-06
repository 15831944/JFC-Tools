//
// Fichier: JMTFR03KEYVariable.h
// Auteur:  Sylvain SAMMURI
// Date:    10/09/2003
//

// on vérifie que le fichier n'a pas déjà été inclus
#ifndef JMTFR03_KEY_VARIABLE_H

// on définit la macro pour éviter les inclusions multiples
#define JMTFR03_KEY_VARIABLE_H

// on inclut les définitions nécessaires
#include "JLib.h"

class JMTFR03KEYVariable
{
public:
	// les constructeurs
	JMTFR03KEYVariable();
	JMTFR03KEYVariable(const JMTFR03KEYVariable & Source);

	// la fonction pour tester l'élément
	JBool IsValid() const;

	// l'opérateur pour recopier l'élément
	JMTFR03KEYVariable & operator =(const JMTFR03KEYVariable & Source);

	// les opérateurs pour comparer les éléments
	JBool operator <(const JMTFR03KEYVariable & Reference) const;
	JBool operator >(const JMTFR03KEYVariable & Reference) const;

	// la fonction pour réinitialiser l'élément
	JVoid Reset();

	// le destructeur
	~JMTFR03KEYVariable();

public:
	JRef32 m_IdTitre; // l'identifiant du titre
};

// fin de l'inclusion conditionnelle
#endif
