//
// Fichier: JMTFR03KEYHomothetie.h
// Auteur:  Sylvain SAMMURI
// Date:    10/09/2003
//

// on vérifie que le fichier n'a pas déjà été inclus
#ifndef JMTFR03_KEY_HOMOHETIE_H

// on définit la macro pour éviter les inclusions multiples
#define JMTFR03_KEY_HOMOHETIE_H

// on inclut les définitions nécessaires
#include "JLib.h"

class JMTFR03KEYHomothetie
{
public:
	// les constructeurs
	JMTFR03KEYHomothetie();
	JMTFR03KEYHomothetie(const JMTFR03KEYHomothetie & Source);

	// la fonction pour tester l'élément
	JBool IsValid() const;

	// l'opérateur pour recopier l'élément
	JMTFR03KEYHomothetie & operator =(const JMTFR03KEYHomothetie & Source);

	// les opérateurs pour comparer les éléments
	JBool operator <(const JMTFR03KEYHomothetie & Reference) const;
	JBool operator >(const JMTFR03KEYHomothetie & Reference) const;

	// la fonction pour réinitialiser l'élément
	JVoid Reset();

	// le destructeur
	~JMTFR03KEYHomothetie();

public:
	JRef32 m_IdEchantillon; // l'identifiant de l'échantillon
	JRef32 m_IdUnivers;     // l'identifiant de l'univers de redressement
};

// fin de l'inclusion conditionnelle
#endif
