//
// Fichier: JMTFR03KEYSegment.h
// Auteur:  Sylvain SAMMURI
// Date:    10/09/2003
//

// on vérifie que le fichier n'a pas déjà été inclus
#ifndef JMTFR03_KEY_SEGMENT_H

// on définit la macro pour éviter les inclusions multiples
#define JMTFR03_KEY_SEGMENT_H

// on inclut les définitions nécessaires
#include "JLib.h"

class JMTFR03KEYSegment
{
public:
	// les constructeurs
	JMTFR03KEYSegment();
	JMTFR03KEYSegment(const JMTFR03KEYSegment & Source);

	// la fonction pour tester l'élément
	JBool IsValid() const;

	// l'opérateur pour recopier l'élément
	JMTFR03KEYSegment & operator =(const JMTFR03KEYSegment & Source);

	// les opérateurs pour comparer les éléments
	JBool operator <(const JMTFR03KEYSegment & Reference) const;
	JBool operator >(const JMTFR03KEYSegment & Reference) const;

	// la fonction pour réinitialiser l'élément
	JVoid Reset();

	// le destructeur
	~JMTFR03KEYSegment();

public:
	JRef32 m_IdSegment; // l'identifiant du segment de population
};

// fin de l'inclusion conditionnelle
#endif
