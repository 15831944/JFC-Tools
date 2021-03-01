//
// Fichier: JMTFR03KEYEchantillon.h
// Auteur:  Sylvain SAMMURI
// Date:    10/09/2003
//

// on v�rifie que le fichier n'a pas d�j� �t� inclus
#ifndef JMTFR03_KEY_ECHANTILLON_H

// on d�finit la macro pour �viter les inclusions multiples
#define JMTFR03_KEY_ECHANTILLON_H

// on inclut les d�finitions n�cessaires
#include "JLib.h"

class JMTFR03KEYEchantillon
{
public:
	// les constructeurs
	JMTFR03KEYEchantillon();
	JMTFR03KEYEchantillon(const JMTFR03KEYEchantillon & Source);

	// la fonction pour tester l'�l�ment
	JBool IsValid() const;

	// l'op�rateur pour recopier l'�l�ment
	JMTFR03KEYEchantillon & operator =(const JMTFR03KEYEchantillon & Source);

	// les op�rateurs pour comparer les �l�ments
	JBool operator <(const JMTFR03KEYEchantillon & Reference) const;
	JBool operator >(const JMTFR03KEYEchantillon & Reference) const;

	// la fonction pour r�initialiser l'�l�ment
	JVoid Reset();

	// le destructeur
	~JMTFR03KEYEchantillon();

public:
	JRef32 m_IdEchantillon; // l'identifiant de l'�chantillon
};

// fin de l'inclusion conditionnelle
#endif
