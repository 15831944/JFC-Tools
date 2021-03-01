//
// Fichier: JMTFR03KEYAttribut.h
// Auteur:  Sylvain SAMMURI
// Date:    10/09/2003
//

// on v�rifie que le fichier n'a pas d�j� �t� inclus
#ifndef JMTFR03_KEY_ATTRIBUT_H

// on d�finit la macro pour �viter les inclusions multiples
#define JMTFR03_KEY_ATTRIBUT_H

// on inclut les d�finitions n�cessaires
#include "JLib.h"

class JMTFR03KEYAttribut
{
public:
	// les constructeurs
	JMTFR03KEYAttribut();
	JMTFR03KEYAttribut(const JMTFR03KEYAttribut & Source);

	// la fonction pour tester l'�l�ment
	JBool IsValid() const;

	// l'op�rateur pour recopier l'�l�ment
	JMTFR03KEYAttribut & operator =(const JMTFR03KEYAttribut & Source);

	// les op�rateurs pour comparer les �l�ments
	JBool operator <(const JMTFR03KEYAttribut & Reference) const;
	JBool operator >(const JMTFR03KEYAttribut & Reference) const;

	// la fonction pour r�initialiser l'�l�ment
	JVoid Reset();

	// le destructeur
	~JMTFR03KEYAttribut();

public:
	JRef32 m_IdTitre; // l'identifiant du titre
};

// fin de l'inclusion conditionnelle
#endif
