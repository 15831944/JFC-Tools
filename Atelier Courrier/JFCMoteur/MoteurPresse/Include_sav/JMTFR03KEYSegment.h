//
// Fichier: JMTFR03KEYSegment.h
// Auteur:  Sylvain SAMMURI
// Date:    10/09/2003
//

// on v�rifie que le fichier n'a pas d�j� �t� inclus
#ifndef JMTFR03_KEY_SEGMENT_H

// on d�finit la macro pour �viter les inclusions multiples
#define JMTFR03_KEY_SEGMENT_H

// on inclut les d�finitions n�cessaires
#include "JLib.h"

class JMTFR03KEYSegment
{
public:
	// les constructeurs
	JMTFR03KEYSegment();
	JMTFR03KEYSegment(const JMTFR03KEYSegment & Source);

	// la fonction pour tester l'�l�ment
	JBool IsValid() const;

	// l'op�rateur pour recopier l'�l�ment
	JMTFR03KEYSegment & operator =(const JMTFR03KEYSegment & Source);

	// les op�rateurs pour comparer les �l�ments
	JBool operator <(const JMTFR03KEYSegment & Reference) const;
	JBool operator >(const JMTFR03KEYSegment & Reference) const;

	// la fonction pour r�initialiser l'�l�ment
	JVoid Reset();

	// le destructeur
	~JMTFR03KEYSegment();

public:
	JRef32 m_IdSegment; // l'identifiant du segment de population
};

// fin de l'inclusion conditionnelle
#endif
