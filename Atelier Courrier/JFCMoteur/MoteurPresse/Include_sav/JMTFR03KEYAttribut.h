//
// Fichier: JMTFR03KEYAttribut.h
// Auteur:  Sylvain SAMMURI
// Date:    10/09/2003
//

// on vérifie que le fichier n'a pas déjà été inclus
#ifndef JMTFR03_KEY_ATTRIBUT_H

// on définit la macro pour éviter les inclusions multiples
#define JMTFR03_KEY_ATTRIBUT_H

// on inclut les définitions nécessaires
#include "JLib.h"

class JMTFR03KEYAttribut
{
public:
	// les constructeurs
	JMTFR03KEYAttribut();
	JMTFR03KEYAttribut(const JMTFR03KEYAttribut & Source);

	// la fonction pour tester l'élément
	JBool IsValid() const;

	// l'opérateur pour recopier l'élément
	JMTFR03KEYAttribut & operator =(const JMTFR03KEYAttribut & Source);

	// les opérateurs pour comparer les éléments
	JBool operator <(const JMTFR03KEYAttribut & Reference) const;
	JBool operator >(const JMTFR03KEYAttribut & Reference) const;

	// la fonction pour réinitialiser l'élément
	JVoid Reset();

	// le destructeur
	~JMTFR03KEYAttribut();

public:
	JRef32 m_IdTitre; // l'identifiant du titre
};

// fin de l'inclusion conditionnelle
#endif
