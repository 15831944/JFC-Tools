//
// Fichier: JMTFR03KEYModulation.h
// Auteur:  Sylvain SAMMURI
// Date:    18/06/2004
//

// on v�rifie que le fichier n'a pas d�j� �t� inclus
#ifndef JMTFR03_KEY_MODULATION_H

// on d�finit la macro pour �viter les inclusions multiples
#define JMTFR03_KEY_MODULATION_H

// on inclut les d�finitions n�cessaires
#include "JLib.h"

class JMTFR03KEYModulation
{
public:
	// les constructeurs
	JMTFR03KEYModulation();
	JMTFR03KEYModulation(const JMTFR03KEYModulation & Source);

	// la fonction pour tester l'�l�ment
	JBool IsValid() const;

	// l'op�rateur pour recopier l'�l�ment
	JMTFR03KEYModulation & operator =(const JMTFR03KEYModulation & Source);

	// les op�rateurs pour comparer les �l�ments
	JBool operator <(const JMTFR03KEYModulation & Reference) const;
	JBool operator >(const JMTFR03KEYModulation & Reference) const;

	// la fonction pour r�initialiser l'�l�ment
	JVoid Reset();

	// le destructeur
	~JMTFR03KEYModulation();

public:
	JRef32 m_IdSource;  // l'identifiant de la source
	JRef32 m_IdTerrain; // l'identifiant du terrain
};

// fin de l'inclusion conditionnelle
#endif
