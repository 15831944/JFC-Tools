//
// Fichier: JMTFR03TBLPoids.h
// Auteur:  Sylvain SAMMURI
// Date:    10/09/2003
//

// on v�rifie que le fichier n'a pas d�j� �t� inclus
#ifndef JMTFR03_TABLE_POIDS_H

// on d�finit la macro pour �viter les inclusions multiples
#define JMTFR03_TABLE_POIDS_H

// on inclut les d�finitions n�cessaires
#include "JMTFR03KEYPoids.h"
#include "JMTFR03DATPoids.h"

class JMTFR03TBLPoids
{
public:
	// le constructeur
	JMTFR03TBLPoids();

	// la fonction pour r�cup�rer le nombre d'�l�ments
	JInt32 GetCount() const;

	// la fonction pour ajouter les �l�ments
	JMTFR03DATPoids & Add(const JMTFR03KEYPoids & KEYPoids, JBool Move = false);

	// la fonction pour tester le curseur
	JBool IsValid() const;

	// les fonctions pour r�cup�rer l'�l�ment du curseur
	const JMTFR03KEYPoids & GetKey() const;
	const JMTFR03DATPoids & GetItem() const;

	// la fonction pour modifier l'�l�ment du curseur
	JMTFR03DATPoids & GetItem();

	// les fonctions pour d�placer le curseur
	JVoid MoveTo(const JMTFR03KEYPoids & KEYPoids, JInt32 Move = 0) const;
	JVoid MoveFirst() const;
	JVoid MoveNext() const;
	JVoid MovePrev() const;
	JVoid MoveLast() const;

	// la fonction pour supprimer l'�l�ment du curseur
	JVoid Remove(JInt32 Move = 0);

	// la fonction pour permuter les �l�ments
	JVoid Swap(JMTFR03TBLPoids & Source);

	// la fonction pour lib�rer les �l�ments
	JVoid Reset();

	// le destructeur
	~JMTFR03TBLPoids();

private:
	// l'op�rateur pour recopier les �l�ments
	JMTFR03TBLPoids & operator =(const JMTFR03TBLPoids & Source);

	// le constructeur copie
	JMTFR03TBLPoids(const JMTFR03TBLPoids & Source);

private:
	JMap<JMTFR03KEYPoids, JMTFR03DATPoids> m_TBLItems; // la table des �l�ments
};

// fin de l'inclusion conditionnelle
#endif
