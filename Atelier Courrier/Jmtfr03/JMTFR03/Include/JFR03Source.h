//
// Fichier: JFR03Source.h
// Auteur:  Sylvain SAMMURI
// Date:    11/03/2003
//

// on v�rifie que le fichier n'a pas d�j� �t� inclus
#ifndef JFR03_SOURCE_H

// on d�finit la macro pour �viter les inclusions multiples
#define JFR03_SOURCE_H

// on inclut les d�finitions n�cessaires
#include "JLib.h"

class JFR03Source
{
public:
	// les constructeurs
	JFR03Source();
	JFR03Source(const JFR03Source & Source);

	// la fonction pour tester la validit� de l'�l�ment
	JBool IsValid() const;

	// l'op�rateur pour recopier l'�l�ment
	JFR03Source & operator =(const JFR03Source & Source);

	// la fonction pour r�initialiser l'�l�ment
	JVoid Reset();

	// le destructeur
	~JFR03Source();

public:
	JRef32 m_IdSource; // l'identifiant de la source
	JLabel m_Libelle;  // le libell� de la source
	JRef32 m_IdMedia;  // l'identifiant du m�dia
};

// fin de l'inclusion conditionnelle
#endif
