//
// Fichier: JMTFR03TBLSegments.h
// Auteur:  Sylvain SAMMURI
// Date:    10/09/2003
//

// on v�rifie que le fichier n'a pas d�j� �t� inclus
#ifndef JMTFR03_TABLE_SEGMENTS_H

// on d�finit la macro pour �viter les inclusions multiples
#define JMTFR03_TABLE_SEGMENTS_H

// on inclut les d�finitions n�cessaires
#include "JMTFR03KEYSegment.h"
#include "JMTFR03DATSegment.h"

class JMTFR03TBLSegments
{
public:
	// le constructeur
	JMTFR03TBLSegments();

	// la fonction pour r�cup�rer le nombre d'�l�ments
	JInt32 GetCount() const;

	// la fonction pour ajouter les �l�ments
	JMTFR03DATSegment & Add(const JMTFR03KEYSegment & KEYSegment, JBool Move = false);

	// la fonction pour tester le curseur
	JBool IsValid() const;

	// les fonctions pour r�cup�rer l'�l�ment du curseur
	const JMTFR03KEYSegment & GetKey() const;
	const JMTFR03DATSegment & GetItem() const;

	// la fonction pour modifier l'�l�ment du curseur
	JMTFR03DATSegment & GetItem();

	// les fonctions pour d�placer le curseur
	JVoid MoveTo(const JMTFR03KEYSegment & KEYSegment, JInt32 Move = 0) const;
	JVoid MoveFirst() const;
	JVoid MoveNext() const;
	JVoid MovePrev() const;
	JVoid MoveLast() const;

	// la fonction pour supprimer l'�l�ment du curseur
	JVoid Remove(JInt32 Move = 0);

	// la fonction pour permuter les �l�ments
	JVoid Swap(JMTFR03TBLSegments & Source);

	// la fonction pour lib�rer les �l�ments
	JVoid Reset();

	// le destructeur
	~JMTFR03TBLSegments();

private:
	// l'op�rateur pour recopier les �l�ments
	JMTFR03TBLSegments & operator =(const JMTFR03TBLSegments & Source);

	// le constructeur copie
	JMTFR03TBLSegments(const JMTFR03TBLSegments & Source);

private:
	JMap<JMTFR03KEYSegment, JMTFR03DATSegment> m_TBLItems; // la table des �l�ments
};

// fin de l'inclusion conditionnelle
#endif
