//
// Fichier: JTrusquin.h
// Auteur:  Sylvain SAMMURI
// Date:    01/03/2005
//

// on vérifie que le fichier n'a pas déjà été inclus
#ifndef JTRUSQUIN_H

// on définit la macro pour éviter les inclusions multiples
#define JTRUSQUIN_H

// on inclut les définitions nécessaires
#include "JLib.h"

class JTrusquin
{
public:
	// le constructeur
	JTrusquin();

	// la fonction pour récupérer le nombre d'éléments
	JInt32 GetCount() const;

	// la fonction pour définir les éléments
	JVoid SetItems(JFlt64 P, JFlt64 N);

	// la fonction pour récupérer les éléments
	const JFlt64 & Item(JInt32 Index) const;

	// la fonction pour permuter les éléments
	JVoid Swap(JTrusquin & Source);

	// la fonction pour libérer les éléments
	JVoid Reset();

	// le destructeur
	~JTrusquin();

private:
	// l'opérateur pour recopier les éléments
	JTrusquin & operator =(const JTrusquin & Source);

	// le constructeur copie
	JTrusquin(const JTrusquin & Source);

private:
	JArray<JFlt64> m_TBLItems; // la table des éléments
};

// fin de l'inclusion conditionnelle
#endif
