//
// Fichier: JMTFR03KEYReprise.h
// Auteur:  Sylvain SAMMURI
// Date:    06/04/2004
//

// on v�rifie que le fichier n'a pas d�j� �t� inclus
#ifndef JMTFR03_KEY_REPRISE_H

// on d�finit la macro pour �viter les inclusions multiples
#define JMTFR03_KEY_REPRISE_H

// on inclut les d�finitions n�cessaires
#include "JLib.h"

class JMTFR03KEYReprise
{
public:
	// les constructeurs
	JMTFR03KEYReprise();
	JMTFR03KEYReprise(const JMTFR03KEYReprise & Source);

	// la fonction pour tester l'�l�ment
	JBool IsValid() const;

	// l'op�rateur pour recopier l'�l�ment
	JMTFR03KEYReprise & operator =(const JMTFR03KEYReprise & Source);

	// les op�rateurs pour comparer les �l�ments
	JBool operator <(const JMTFR03KEYReprise & Reference) const;
	JBool operator >(const JMTFR03KEYReprise & Reference) const;

	// la fonction pour r�initialiser l'�l�ment
	JVoid Reset();

	// le destructeur
	~JMTFR03KEYReprise();

public:
	JRef32 m_IdTitre; // l'identifiant du titre
};

// fin de l'inclusion conditionnelle
#endif
