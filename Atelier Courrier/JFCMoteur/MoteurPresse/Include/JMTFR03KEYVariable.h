//
// Fichier: JMTFR03KEYVariable.h
// Auteur:  Sylvain SAMMURI
// Date:    10/09/2003
//

// on v�rifie que le fichier n'a pas d�j� �t� inclus
#ifndef JMTFR03_KEY_VARIABLE_H

// on d�finit la macro pour �viter les inclusions multiples
#define JMTFR03_KEY_VARIABLE_H

// on inclut les d�finitions n�cessaires
#include "JLib.h"

class JMTFR03KEYVariable
{
public:
	// les constructeurs
	JMTFR03KEYVariable();
	JMTFR03KEYVariable(const JMTFR03KEYVariable & Source);

	// la fonction pour tester l'�l�ment
	JBool IsValid() const;

	// l'op�rateur pour recopier l'�l�ment
	JMTFR03KEYVariable & operator =(const JMTFR03KEYVariable & Source);

	// les op�rateurs pour comparer les �l�ments
	JBool operator <(const JMTFR03KEYVariable & Reference) const;
	JBool operator >(const JMTFR03KEYVariable & Reference) const;

	// la fonction pour r�initialiser l'�l�ment
	JVoid Reset();

	// le destructeur
	~JMTFR03KEYVariable();

public:
	JRef32 m_IdTitre; // l'identifiant du titre
};

// fin de l'inclusion conditionnelle
#endif
