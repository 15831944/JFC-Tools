//
// Fichier: JMTFR03TBLSegments.h
// Auteur:  Sylvain SAMMURI
// Date:    10/09/2003
//

// on vérifie que le fichier n'a pas déjà été inclus
#ifndef JMTFR03_TABLE_SEGMENTS_H

// on définit la macro pour éviter les inclusions multiples
#define JMTFR03_TABLE_SEGMENTS_H

// on inclut les définitions nécessaires
#include "JMTFR03KEYSegment.h"
#include "JMTFR03DATSegment.h"

class JMTFR03TBLSegments
{
public:
	// le constructeur
	JMTFR03TBLSegments();

	// la fonction pour récupérer le nombre d'éléments
	JInt32 GetCount() const;

	// la fonction pour ajouter les éléments
	JMTFR03DATSegment & Add(const JMTFR03KEYSegment & KEYSegment, JBool Move = false);

	// la fonction pour tester le curseur
	JBool IsValid() const;

	// les fonctions pour récupérer l'élément du curseur
	const JMTFR03KEYSegment & GetKey() const;
	const JMTFR03DATSegment & GetItem() const;

	// la fonction pour modifier l'élément du curseur
	JMTFR03DATSegment & GetItem();

	// les fonctions pour déplacer le curseur
	JVoid MoveTo(const JMTFR03KEYSegment & KEYSegment, JInt32 Move = 0) const;
	JVoid MoveFirst() const;
	JVoid MoveNext() const;
	JVoid MovePrev() const;
	JVoid MoveLast() const;

	// la fonction pour supprimer l'élément du curseur
	JVoid Remove(JInt32 Move = 0);

	// la fonction pour permuter les éléments
	JVoid Swap(JMTFR03TBLSegments & Source);

	// la fonction pour libérer les éléments
	JVoid Reset();

	// le destructeur
	~JMTFR03TBLSegments();

private:
	// l'opérateur pour recopier les éléments
	JMTFR03TBLSegments & operator =(const JMTFR03TBLSegments & Source);

	// le constructeur copie
	JMTFR03TBLSegments(const JMTFR03TBLSegments & Source);

private:
	JMap<JMTFR03KEYSegment, JMTFR03DATSegment> m_TBLItems; // la table des éléments
};

// fin de l'inclusion conditionnelle
#endif
