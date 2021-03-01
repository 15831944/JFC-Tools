//
// Fichier: JMTFR03KEYLectorat.h
// Auteur:  Sylvain SAMMURI
// Date:    02/12/2003
//

// on v�rifie que le fichier n'a pas d�j� �t� inclus
#ifndef JMTFR03_KEY_LECTORAT_H

// on d�finit la macro pour �viter les inclusions multiples
#define JMTFR03_KEY_LECTORAT_H

// on inclut les d�finitions n�cessaires
#include "JLib.h"

class JMTFR03KEYLectorat
{
public:
	// les constructeurs
	JMTFR03KEYLectorat();
	JMTFR03KEYLectorat(const JMTFR03KEYLectorat & Source);

	// la fonction pour tester l'�l�ment
	JBool IsValid() const;

	// l'op�rateur pour recopier l'�l�ment
	JMTFR03KEYLectorat & operator =(const JMTFR03KEYLectorat & Source);

	// les op�rateurs pour comparer les �l�ments
	JBool operator <(const JMTFR03KEYLectorat & Reference) const;
	JBool operator >(const JMTFR03KEYLectorat & Reference) const;

	// la fonction pour r�initialiser l'�l�ment
	JVoid Reset();

	// le destructeur
	~JMTFR03KEYLectorat();

public:
	JRef32 m_IdTitre; // l'identifiant du titre
};

// fin de l'inclusion conditionnelle
#endif
