//
// Fichier: JMTFR03TBLPoids.h
// Auteur:  Sylvain SAMMURI
// Date:    10/09/2003
//

// on vérifie que le fichier n'a pas déjà été inclus
#ifndef JMTFR03_TABLE_POIDS_H

// on définit la macro pour éviter les inclusions multiples
#define JMTFR03_TABLE_POIDS_H

// on inclut les définitions nécessaires
#include "JMTFR03KEYPoids.h"
#include "JMTFR03DATPoids.h"

class JMTFR03TBLPoids
{
public:
	// le constructeur
	JMTFR03TBLPoids();

	// la fonction pour récupérer le nombre d'éléments
	JInt32 GetCount() const;

	// la fonction pour ajouter les éléments
	JMTFR03DATPoids & Add(const JMTFR03KEYPoids & KEYPoids, JBool Move = false);

	// la fonction pour tester le curseur
	JBool IsValid() const;

	// les fonctions pour récupérer l'élément du curseur
	const JMTFR03KEYPoids & GetKey() const;
	const JMTFR03DATPoids & GetItem() const;

	// la fonction pour modifier l'élément du curseur
	JMTFR03DATPoids & GetItem();

	// les fonctions pour déplacer le curseur
	JVoid MoveTo(const JMTFR03KEYPoids & KEYPoids, JInt32 Move = 0) const;
	JVoid MoveFirst() const;
	JVoid MoveNext() const;
	JVoid MovePrev() const;
	JVoid MoveLast() const;

	// la fonction pour supprimer l'élément du curseur
	JVoid Remove(JInt32 Move = 0);

	// la fonction pour permuter les éléments
	JVoid Swap(JMTFR03TBLPoids & Source);

	// la fonction pour libérer les éléments
	JVoid Reset();

	// le destructeur
	~JMTFR03TBLPoids();

private:
	// l'opérateur pour recopier les éléments
	JMTFR03TBLPoids & operator =(const JMTFR03TBLPoids & Source);

	// le constructeur copie
	JMTFR03TBLPoids(const JMTFR03TBLPoids & Source);

private:
	JMap<JMTFR03KEYPoids, JMTFR03DATPoids> m_TBLItems; // la table des éléments
};

// fin de l'inclusion conditionnelle
#endif
