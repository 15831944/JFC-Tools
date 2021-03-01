//
// Fichier: JMTFR03KEYPoids.h
// Auteur:  Sylvain SAMMURI
// Date:    10/09/2003
//

// on v�rifie que le fichier n'a pas d�j� �t� inclus
#ifndef JMTFR03_KEY_POIDS_H

// on d�finit la macro pour �viter les inclusions multiples
#define JMTFR03_KEY_POIDS_H

// on inclut les d�finitions n�cessaires
#include "JLib.h"

class JMTFR03KEYPoids
{
public:
	// les constructeurs
	JMTFR03KEYPoids();
	JMTFR03KEYPoids(const JMTFR03KEYPoids & Source);

	// la fonction pour tester l'�l�ment
	JBool IsValid() const;

	// l'op�rateur pour recopier l'�l�ment
	JMTFR03KEYPoids & operator =(const JMTFR03KEYPoids & Source);

	// les op�rateurs pour comparer les �l�ments
	JBool operator <(const JMTFR03KEYPoids & Reference) const;
	JBool operator >(const JMTFR03KEYPoids & Reference) const;

	// la fonction pour r�initialiser l'�l�ment
	JVoid Reset();

	// le destructeur
	~JMTFR03KEYPoids();

public:
	JRef32 m_IdPoids;   // l'identifiant du type de poids
	JRef32 m_IdUnivers; // l'identifiant de l'univers de redressement
};

// fin de l'inclusion conditionnelle
#endif
