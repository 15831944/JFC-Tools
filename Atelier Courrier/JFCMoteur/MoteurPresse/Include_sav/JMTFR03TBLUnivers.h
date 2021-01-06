//
// Fichier: JMTFR03TBLUnivers.h
// Auteur:  Sylvain SAMMURI
// Date:    10/09/2003
//

// on v�rifie que le fichier n'a pas d�j� �t� inclus
#ifndef JMTFR03_TABLE_UNIVERS_H

// on d�finit la macro pour �viter les inclusions multiples
#define JMTFR03_TABLE_UNIVERS_H

// on inclut les d�finitions n�cessaires
#include "JMTFR03KEYUnivers.h"
#include "JMTFR03DATUnivers.h"

class JMTFR03TBLUnivers
{
public:
	// le constructeur
	JMTFR03TBLUnivers();

	// la fonction pour r�cup�rer le nombre d'�l�ments
	JInt32 GetCount() const;

	// la fonction pour ajouter les �l�ments
	JMTFR03DATUnivers & Add(const JMTFR03KEYUnivers & KEYUnivers, JBool Move = false);

	// la fonction pour tester le curseur
	JBool IsValid() const;

	// les fonctions pour r�cup�rer l'�l�ment du curseur
	const JMTFR03KEYUnivers & GetKey() const;
	const JMTFR03DATUnivers & GetItem() const;

	// la fonction pour modifier l'�l�ment du curseur
	JMTFR03DATUnivers & GetItem();

	// les fonctions pour d�placer le curseur
	JVoid MoveTo(const JMTFR03KEYUnivers & KEYUnivers, JInt32 Move = 0) const;
	JVoid MoveFirst() const;
	JVoid MoveNext() const;
	JVoid MovePrev() const;
	JVoid MoveLast() const;

	// la fonction pour supprimer l'�l�ment du curseur
	JVoid Remove(JInt32 Move = 0);

	// la fonction pour permuter les �l�ments
	JVoid Swap(JMTFR03TBLUnivers & Source);

	// la fonction pour lib�rer les �l�ments
	JVoid Reset();

	// le destructeur
	~JMTFR03TBLUnivers();

private:
	// l'op�rateur pour recopier les �l�ments
	JMTFR03TBLUnivers & operator =(const JMTFR03TBLUnivers & Source);

	// le constructeur copie
	JMTFR03TBLUnivers(const JMTFR03TBLUnivers & Source);

private:
	JMap<JMTFR03KEYUnivers, JMTFR03DATUnivers> m_TBLItems; // la table des �l�ments
};

// fin de l'inclusion conditionnelle
#endif
