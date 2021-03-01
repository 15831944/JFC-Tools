//
// Fichier: JMTFR03TBLUnivers.h
// Auteur:  Sylvain SAMMURI
// Date:    10/09/2003
//

// on vérifie que le fichier n'a pas déjà été inclus
#ifndef JMTFR03_TABLE_UNIVERS_H

// on définit la macro pour éviter les inclusions multiples
#define JMTFR03_TABLE_UNIVERS_H

// on inclut les définitions nécessaires
#include "JMTFR03KEYUnivers.h"
#include "JMTFR03DATUnivers.h"

class JMTFR03TBLUnivers
{
public:
	// le constructeur
	JMTFR03TBLUnivers();

	// la fonction pour récupérer le nombre d'éléments
	JInt32 GetCount() const;

	// la fonction pour ajouter les éléments
	JMTFR03DATUnivers & Add(const JMTFR03KEYUnivers & KEYUnivers, JBool Move = false);

	// la fonction pour tester le curseur
	JBool IsValid() const;

	// les fonctions pour récupérer l'élément du curseur
	const JMTFR03KEYUnivers & GetKey() const;
	const JMTFR03DATUnivers & GetItem() const;

	// la fonction pour modifier l'élément du curseur
	JMTFR03DATUnivers & GetItem();

	// les fonctions pour déplacer le curseur
	JVoid MoveTo(const JMTFR03KEYUnivers & KEYUnivers, JInt32 Move = 0) const;
	JVoid MoveFirst() const;
	JVoid MoveNext() const;
	JVoid MovePrev() const;
	JVoid MoveLast() const;

	// la fonction pour supprimer l'élément du curseur
	JVoid Remove(JInt32 Move = 0);

	// la fonction pour permuter les éléments
	JVoid Swap(JMTFR03TBLUnivers & Source);

	// la fonction pour libérer les éléments
	JVoid Reset();

	// le destructeur
	~JMTFR03TBLUnivers();

private:
	// l'opérateur pour recopier les éléments
	JMTFR03TBLUnivers & operator =(const JMTFR03TBLUnivers & Source);

	// le constructeur copie
	JMTFR03TBLUnivers(const JMTFR03TBLUnivers & Source);

private:
	JMap<JMTFR03KEYUnivers, JMTFR03DATUnivers> m_TBLItems; // la table des éléments
};

// fin de l'inclusion conditionnelle
#endif
