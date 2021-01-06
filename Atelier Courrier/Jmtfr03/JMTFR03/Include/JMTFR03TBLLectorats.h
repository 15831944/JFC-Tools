//
// Fichier: JMTFR03TBLLectorats.h
// Auteur:  Sylvain SAMMURI
// Date:    02/12/2003
//

// on vérifie que le fichier n'a pas déjà été inclus
#ifndef JMTFR03_TABLE_LECTORATS_H

// on définit la macro pour éviter les inclusions multiples
#define JMTFR03_TABLE_LECTORATS_H

// on inclut les définitions nécessaires
#include "JMTFR03KEYLectorat.h"
#include "JMTFR03DATLectorat.h"

class JMTFR03TBLLectorats
{
public:
	// le constructeur
	JMTFR03TBLLectorats();

	// la fonction pour récupérer le nombre d'éléments
	JInt32 GetCount() const;

	// la fonction pour ajouter les éléments
	JMTFR03DATLectorat & Add(const JMTFR03KEYLectorat & KEYLectorat, JBool Move = false);

	// la fonction pour tester le curseur
	JBool IsValid() const;

	// les fonctions pour récupérer l'élément du curseur
	const JMTFR03KEYLectorat & GetKey() const;
	const JMTFR03DATLectorat & GetItem() const;

	// la fonction pour modifier l'élément du curseur
	JMTFR03DATLectorat & GetItem();

	// les fonctions pour déplacer le curseur
	JVoid MoveTo(const JMTFR03KEYLectorat & KEYLectorat, JInt32 Move = 0) const;
	JVoid MoveFirst() const;
	JVoid MoveNext() const;
	JVoid MovePrev() const;
	JVoid MoveLast() const;

	// la fonction pour supprimer l'élément du curseur
	JVoid Remove(JInt32 Move = 0);

	// la fonction pour permuter les éléments
	JVoid Swap(JMTFR03TBLLectorats & Source);

	// la fonction pour libérer les éléments
	JVoid Reset();

	// le destructeur
	~JMTFR03TBLLectorats();

private:
	// l'opérateur pour recopier les éléments
	JMTFR03TBLLectorats & operator =(const JMTFR03TBLLectorats & Source);

	// le constructeur copie
	JMTFR03TBLLectorats(const JMTFR03TBLLectorats & Source);

private:
	JMap<JMTFR03KEYLectorat, JMTFR03DATLectorat> m_TBLItems; // la table des éléments
};

// fin de l'inclusion conditionnelle
#endif
