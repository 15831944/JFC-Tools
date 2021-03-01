//
// Fichier: JMTFR03KEYCible.h
// Auteur:  Sylvain SAMMURI
// Date:    09/12/2003
//

// on v�rifie que le fichier n'a pas d�j� �t� inclus
#ifndef JMTFR03_KEY_CIBLE_H

// on d�finit la macro pour �viter les inclusions multiples
#define JMTFR03_KEY_CIBLE_H

// on inclut les d�finitions n�cessaires
#include "JLib.h"

class JMTFR03KEYCible
{
public:
	// les constructeurs
	JMTFR03KEYCible();
	JMTFR03KEYCible(const JMTFR03KEYCible & Source);

	// la fonction pour tester l'�l�ment
	JBool IsValid() const;

	// l'op�rateur pour recopier l'�l�ment
	JMTFR03KEYCible & operator =(const JMTFR03KEYCible & Source);

	// les op�rateurs pour comparer les �l�ments
	JBool operator <(const JMTFR03KEYCible & Reference) const;
	JBool operator >(const JMTFR03KEYCible & Reference) const;

	// la fonction pour r�initialiser l'�l�ment
	JVoid Reset();

	// le destructeur
	~JMTFR03KEYCible();

public:
	JRef32 m_IdSource;  // l'identifiant de la source
	JRef32 m_IdTerrain; // l'identifiant du terrain
	JRef32 m_IdCible;   // l'identifiant de la cible
};

// fin de l'inclusion conditionnelle
#endif
