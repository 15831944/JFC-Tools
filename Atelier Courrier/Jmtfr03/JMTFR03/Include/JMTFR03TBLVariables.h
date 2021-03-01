//
// Fichier: JMTFR03TBLVariables.h
// Auteur:  Sylvain SAMMURI
// Date:    10/09/2003
//

// on vérifie que le fichier n'a pas déjà été inclus
#ifndef JMTFR03_TABLE_VARIABLES_H

// on définit la macro pour éviter les inclusions multiples
#define JMTFR03_TABLE_VARIABLES_H

// on inclut les définitions nécessaires
#include "JMTFR03KEYVariable.h"
#include "JMTFR03DATVariable.h"

class JMTFR03TBLVariables
{
public:
	// le constructeur
	JMTFR03TBLVariables();

	// la fonction pour récupérer le nombre d'éléments
	JInt32 GetCount() const;

	// la fonction pour ajouter les éléments
	JMTFR03DATVariable & Add(const JMTFR03KEYVariable & KEYVariable, JBool Move = false);

	// la fonction pour tester le curseur
	JBool IsValid() const;

	// les fonctions pour récupérer l'élément du curseur
	const JMTFR03KEYVariable & GetKey() const;
	const JMTFR03DATVariable & GetItem() const;

	// la fonction pour modifier l'élément du curseur
	JMTFR03DATVariable & GetItem();

	// les fonctions pour déplacer le curseur
	JVoid MoveTo(const JMTFR03KEYVariable & KEYVariable, JInt32 Move = 0) const;
	JVoid MoveFirst() const;
	JVoid MoveNext() const;
	JVoid MovePrev() const;
	JVoid MoveLast() const;

	// la fonction pour supprimer l'élément du curseur
	JVoid Remove(JInt32 Move = 0);

	// la fonction pour permuter les éléments
	JVoid Swap(JMTFR03TBLVariables & Source);

	// la fonction pour libérer les éléments
	JVoid Reset();

	// le destructeur
	~JMTFR03TBLVariables();

private:
	// l'opérateur pour recopier les éléments
	JMTFR03TBLVariables & operator =(const JMTFR03TBLVariables & Source);

	// le constructeur copie
	JMTFR03TBLVariables(const JMTFR03TBLVariables & Source);

private:
	JMap<JMTFR03KEYVariable, JMTFR03DATVariable> m_TBLItems; // la table des éléments
};

// fin de l'inclusion conditionnelle
#endif
