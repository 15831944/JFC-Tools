//
// Fichier: JMTFR03TBLReprises.h
// Auteur:  Sylvain SAMMURI
// Date:    06/04/2004
//

// on vérifie que le fichier n'a pas déjà été inclus
#ifndef JMTFR03_TABLE_REPRISES_H

// on définit la macro pour éviter les inclusions multiples
#define JMTFR03_TABLE_REPRISES_H

// on inclut les définitions nécessaires
#include "JMTFR03KEYReprise.h"
#include "JMTFR03DATReprise.h"

class JMTFR03TBLReprises
{
public:
	// le constructeur
	JMTFR03TBLReprises();

	// la fonction pour récupérer le nombre d'éléments
	JInt32 GetCount() const;

	// la fonction pour ajouter les éléments
	JMTFR03DATReprise & Add(const JMTFR03KEYReprise & KEYReprise, JBool Move = false);

	// la fonction pour tester le curseur
	JBool IsValid() const;

	// les fonctions pour récupérer l'élément du curseur
	const JMTFR03KEYReprise & GetKey() const;
	const JMTFR03DATReprise & GetItem() const;

	// la fonction pour modifier l'élément du curseur
	JMTFR03DATReprise & GetItem();

	// les fonctions pour déplacer le curseur
	JVoid MoveTo(const JMTFR03KEYReprise & KEYReprise, JInt32 Move = 0) const;
	JVoid MoveFirst() const;
	JVoid MoveNext() const;
	JVoid MovePrev() const;
	JVoid MoveLast() const;

	// la fonction pour supprimer l'élément du curseur
	JVoid Remove(JInt32 Move = 0);

	// la fonction pour permuter les éléments
	JVoid Swap(JMTFR03TBLReprises & Source);

	// la fonction pour libérer les éléments
	JVoid Reset();

	// le destructeur
	~JMTFR03TBLReprises();

private:
	// l'opérateur pour recopier les éléments
	JMTFR03TBLReprises & operator =(const JMTFR03TBLReprises & Source);

	// le constructeur copie
	JMTFR03TBLReprises(const JMTFR03TBLReprises & Source);

private:
	JMap<JMTFR03KEYReprise, JMTFR03DATReprise> m_TBLItems; // la table des éléments
};

// fin de l'inclusion conditionnelle
#endif
