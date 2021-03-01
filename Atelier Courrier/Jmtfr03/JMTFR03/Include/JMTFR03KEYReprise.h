//
// Fichier: JMTFR03KEYReprise.h
// Auteur:  Sylvain SAMMURI
// Date:    06/04/2004
//

// on vérifie que le fichier n'a pas déjà été inclus
#ifndef JMTFR03_KEY_REPRISE_H

// on définit la macro pour éviter les inclusions multiples
#define JMTFR03_KEY_REPRISE_H

// on inclut les définitions nécessaires
#include "JLib.h"

class JMTFR03KEYReprise
{
public:
	// les constructeurs
	JMTFR03KEYReprise();
	JMTFR03KEYReprise(const JMTFR03KEYReprise & Source);

	// la fonction pour tester l'élément
	JBool IsValid() const;

	// l'opérateur pour recopier l'élément
	JMTFR03KEYReprise & operator =(const JMTFR03KEYReprise & Source);

	// les opérateurs pour comparer les éléments
	JBool operator <(const JMTFR03KEYReprise & Reference) const;
	JBool operator >(const JMTFR03KEYReprise & Reference) const;

	// la fonction pour réinitialiser l'élément
	JVoid Reset();

	// le destructeur
	~JMTFR03KEYReprise();

public:
	JRef32 m_IdTitre; // l'identifiant du titre
};

// fin de l'inclusion conditionnelle
#endif
