//
// Fichier: JMTFR03KEYEchantillon.h
// Auteur:  Sylvain SAMMURI
// Date:    10/09/2003
//

// on vérifie que le fichier n'a pas déjà été inclus
#ifndef JMTFR03_KEY_ECHANTILLON_H

// on définit la macro pour éviter les inclusions multiples
#define JMTFR03_KEY_ECHANTILLON_H

// on inclut les définitions nécessaires
#include "JLib.h"

class JMTFR03KEYEchantillon
{
public:
	// les constructeurs
	JMTFR03KEYEchantillon();
	JMTFR03KEYEchantillon(const JMTFR03KEYEchantillon & Source);

	// la fonction pour tester l'élément
	JBool IsValid() const;

	// l'opérateur pour recopier l'élément
	JMTFR03KEYEchantillon & operator =(const JMTFR03KEYEchantillon & Source);

	// les opérateurs pour comparer les éléments
	JBool operator <(const JMTFR03KEYEchantillon & Reference) const;
	JBool operator >(const JMTFR03KEYEchantillon & Reference) const;

	// la fonction pour réinitialiser l'élément
	JVoid Reset();

	// le destructeur
	~JMTFR03KEYEchantillon();

public:
	JRef32 m_IdEchantillon; // l'identifiant de l'échantillon
};

// fin de l'inclusion conditionnelle
#endif
