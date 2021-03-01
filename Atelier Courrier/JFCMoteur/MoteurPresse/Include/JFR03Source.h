//
// Fichier: JFR03Source.h
// Auteur:  Sylvain SAMMURI
// Date:    11/03/2003
//

// on vérifie que le fichier n'a pas déjà été inclus
#ifndef JFR03_SOURCE_H

// on définit la macro pour éviter les inclusions multiples
#define JFR03_SOURCE_H

// on inclut les définitions nécessaires
#include "JLib.h"

class JFR03Source
{
public:
	// les constructeurs
	JFR03Source();
	JFR03Source(const JFR03Source & Source);

	// la fonction pour tester la validité de l'élément
	JBool IsValid() const;

	// l'opérateur pour recopier l'élément
	JFR03Source & operator =(const JFR03Source & Source);

	// la fonction pour réinitialiser l'élément
	JVoid Reset();

	// le destructeur
	~JFR03Source();

public:
	JRef32 m_IdSource; // l'identifiant de la source
	JLabel m_Libelle;  // le libellé de la source
	JRef32 m_IdMedia;  // l'identifiant du média
};

// fin de l'inclusion conditionnelle
#endif
