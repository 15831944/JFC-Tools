//
// Fichier: JMTFR03KEYUnivers.h
// Auteur:  Sylvain SAMMURI
// Date:    10/09/2003
//

// on v�rifie que le fichier n'a pas d�j� �t� inclus
#ifndef JMTFR03_KEY_UNIVERS_H

// on d�finit la macro pour �viter les inclusions multiples
#define JMTFR03_KEY_UNIVERS_H

// on inclut les d�finitions n�cessaires
#include "JLib.h"

class JMTFR03KEYUnivers
{
public:
	// les constructeurs
	JMTFR03KEYUnivers();
	JMTFR03KEYUnivers(const JMTFR03KEYUnivers & Source);

	// la fonction pour tester l'�l�ment
	JBool IsValid() const;

	// l'op�rateur pour recopier l'�l�ment
	JMTFR03KEYUnivers & operator =(const JMTFR03KEYUnivers & Source);

	// les op�rateurs pour comparer les �l�ments
	JBool operator <(const JMTFR03KEYUnivers & Reference) const;
	JBool operator >(const JMTFR03KEYUnivers & Reference) const;

	// la fonction pour r�initialiser l'�l�ment
	JVoid Reset();

	// le destructeur
	~JMTFR03KEYUnivers();

public:
	JRef32 m_IdUnivers; // l'identifiant de l'univers de redressement
};

// fin de l'inclusion conditionnelle
#endif
