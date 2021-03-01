//
// Fichier: JMTFR03KEYUnivers.h
// Auteur:  Sylvain SAMMURI
// Date:    10/09/2003
//

// on vérifie que le fichier n'a pas déjà été inclus
#ifndef JMTFR03_KEY_UNIVERS_H

// on définit la macro pour éviter les inclusions multiples
#define JMTFR03_KEY_UNIVERS_H

// on inclut les définitions nécessaires
#include "JLib.h"

class JMTFR03KEYUnivers
{
public:
	// les constructeurs
	JMTFR03KEYUnivers();
	JMTFR03KEYUnivers(const JMTFR03KEYUnivers & Source);

	// la fonction pour tester l'élément
	JBool IsValid() const;

	// l'opérateur pour recopier l'élément
	JMTFR03KEYUnivers & operator =(const JMTFR03KEYUnivers & Source);

	// les opérateurs pour comparer les éléments
	JBool operator <(const JMTFR03KEYUnivers & Reference) const;
	JBool operator >(const JMTFR03KEYUnivers & Reference) const;

	// la fonction pour réinitialiser l'élément
	JVoid Reset();

	// le destructeur
	~JMTFR03KEYUnivers();

public:
	JRef32 m_IdUnivers; // l'identifiant de l'univers de redressement
};

// fin de l'inclusion conditionnelle
#endif
