//
// Fichier: JMTFR03TBLHomotheties.h
// Auteur:  Sylvain SAMMURI
// Date:    10/09/2003
//

// on v�rifie que le fichier n'a pas d�j� �t� inclus
#ifndef JMTFR03_TABLE_HOMOTHETIES_H

// on d�finit la macro pour �viter les inclusions multiples
#define JMTFR03_TABLE_HOMOTHETIES_H

// on inclut les d�finitions n�cessaires
#include "JMTFR03KEYHomothetie.h"
#include "JMTFR03DATHomothetie.h"

class JMTFR03TBLHomotheties
{
public:
	// le constructeur
	JMTFR03TBLHomotheties();

	// la fonction pour r�cup�rer le nombre d'�l�ments
	JInt32 GetCount() const;

	// la fonction pour ajouter les �l�ments
	JMTFR03DATHomothetie & Add(const JMTFR03KEYHomothetie & KEYHomothetie, JBool Move = false);

	// la fonction pour tester le curseur
	JBool IsValid() const;

	// les fonctions pour r�cup�rer l'�l�ment du curseur
	const JMTFR03KEYHomothetie & GetKey() const;
	const JMTFR03DATHomothetie & GetItem() const;

	// la fonction pour modifier l'�l�ment du curseur
	JMTFR03DATHomothetie & GetItem();

	// les fonctions pour d�placer le curseur
	JVoid MoveTo(const JMTFR03KEYHomothetie & KEYHomothetie, JInt32 Move = 0) const;
	JVoid MoveFirst() const;
	JVoid MoveNext() const;
	JVoid MovePrev() const;
	JVoid MoveLast() const;

	// la fonction pour supprimer l'�l�ment du curseur
	JVoid Remove(JInt32 Move = 0);

	// la fonction pour permuter les �l�ments
	JVoid Swap(JMTFR03TBLHomotheties & Source);

	// la fonction pour lib�rer les �l�ments
	JVoid Reset();

	// le destructeur
	~JMTFR03TBLHomotheties();

private:
	// l'op�rateur pour recopier les �l�ments
	JMTFR03TBLHomotheties & operator =(const JMTFR03TBLHomotheties & Source);

	// le constructeur copie
	JMTFR03TBLHomotheties(const JMTFR03TBLHomotheties & Source);

private:
	JMap<JMTFR03KEYHomothetie, JMTFR03DATHomothetie> m_TBLItems; // la table des �l�ments
};

// fin de l'inclusion conditionnelle
#endif
