//
// Fichier: JMTFR03Couplage.h
// Auteur:  Sylvain SAMMURI
// Date:    17/02/2005
//

// on vérifie que le fichier n'a pas déjà été inclus
#ifndef JMTFR03_COUPLAGE_H

// on définit la macro pour éviter les inclusions multiples
#define JMTFR03_COUPLAGE_H

// on inclut les définitions nécessaires
#include "JLib.h"

class JMTFR03Couplage
{
public:
	// le constructeur
	JMTFR03Couplage();

	// la fonction pour récupérer le nombre d'éléments
	JInt32 GetCount() const;

	// la fonction pour ajouter les éléments
	JVoid Add(const JRef32 & IdTitre, JBool Move = false);

	// la fonction pour tester le curseur
	JBool IsValid() const;

	// la fonction pour récupérer l'élément du curseur
	const JRef32 & GetIdTitre() const;

	// les fonctions pour déplacer le curseur
	JVoid MoveTo(const JRef32 & IdTitre, JInt32 Move = 0) const;
	JVoid MoveFirst() const;
	JVoid MoveNext() const;
	JVoid MovePrev() const;
	JVoid MoveLast() const;

	// la fonction pour supprimer l'élément du curseur
	JVoid Remove(JInt32 Move = 0);

	// la fonction pour permuter les éléments
	JVoid Swap(JMTFR03Couplage & Source);

	// la fonction pour libérer les éléments
	JVoid Reset();

	// le destructeur
	~JMTFR03Couplage();

private:
	// l'opérateur pour recopier les éléments
	JMTFR03Couplage & operator =(const JMTFR03Couplage & Source);

	// le constructeur copie
	JMTFR03Couplage(const JMTFR03Couplage & Source);

private:
	JMap<JRef32, JInt32x> m_TBLItems; // la table des éléments
};

// fin de l'inclusion conditionnelle
#endif
