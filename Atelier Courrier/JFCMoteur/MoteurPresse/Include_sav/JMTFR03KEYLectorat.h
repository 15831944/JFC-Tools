//
// Fichier: JMTFR03KEYLectorat.h
// Auteur:  Sylvain SAMMURI
// Date:    02/12/2003
//

// on vérifie que le fichier n'a pas déjà été inclus
#ifndef JMTFR03_KEY_LECTORAT_H

// on définit la macro pour éviter les inclusions multiples
#define JMTFR03_KEY_LECTORAT_H

// on inclut les définitions nécessaires
#include "JLib.h"

class JMTFR03KEYLectorat
{
public:
	// les constructeurs
	JMTFR03KEYLectorat();
	JMTFR03KEYLectorat(const JMTFR03KEYLectorat & Source);

	// la fonction pour tester l'élément
	JBool IsValid() const;

	// l'opérateur pour recopier l'élément
	JMTFR03KEYLectorat & operator =(const JMTFR03KEYLectorat & Source);

	// les opérateurs pour comparer les éléments
	JBool operator <(const JMTFR03KEYLectorat & Reference) const;
	JBool operator >(const JMTFR03KEYLectorat & Reference) const;

	// la fonction pour réinitialiser l'élément
	JVoid Reset();

	// le destructeur
	~JMTFR03KEYLectorat();

public:
	JRef32 m_IdTitre; // l'identifiant du titre
};

// fin de l'inclusion conditionnelle
#endif
