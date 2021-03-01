//
// Fichier: JMTFR03TBLVariables.h
// Auteur:  Sylvain SAMMURI
// Date:    10/09/2003
//

// on v�rifie que le fichier n'a pas d�j� �t� inclus
#ifndef JMTFR03_TABLE_VARIABLES_H

// on d�finit la macro pour �viter les inclusions multiples
#define JMTFR03_TABLE_VARIABLES_H

// on inclut les d�finitions n�cessaires
#include "JMTFR03KEYVariable.h"
#include "JMTFR03DATVariable.h"

class JMTFR03TBLVariables
{
public:
	// le constructeur
	JMTFR03TBLVariables();

	// la fonction pour r�cup�rer le nombre d'�l�ments
	JInt32 GetCount() const;

	// la fonction pour ajouter les �l�ments
	JMTFR03DATVariable & Add(const JMTFR03KEYVariable & KEYVariable, JBool Move = false);

	// la fonction pour tester le curseur
	JBool IsValid() const;

	// les fonctions pour r�cup�rer l'�l�ment du curseur
	const JMTFR03KEYVariable & GetKey() const;
	const JMTFR03DATVariable & GetItem() const;

	// la fonction pour modifier l'�l�ment du curseur
	JMTFR03DATVariable & GetItem();

	// les fonctions pour d�placer le curseur
	JVoid MoveTo(const JMTFR03KEYVariable & KEYVariable, JInt32 Move = 0) const;
	JVoid MoveFirst() const;
	JVoid MoveNext() const;
	JVoid MovePrev() const;
	JVoid MoveLast() const;

	// la fonction pour supprimer l'�l�ment du curseur
	JVoid Remove(JInt32 Move = 0);

	// la fonction pour permuter les �l�ments
	JVoid Swap(JMTFR03TBLVariables & Source);

	// la fonction pour lib�rer les �l�ments
	JVoid Reset();

	// le destructeur
	~JMTFR03TBLVariables();

private:
	// l'op�rateur pour recopier les �l�ments
	JMTFR03TBLVariables & operator =(const JMTFR03TBLVariables & Source);

	// le constructeur copie
	JMTFR03TBLVariables(const JMTFR03TBLVariables & Source);

private:
	JMap<JMTFR03KEYVariable, JMTFR03DATVariable> m_TBLItems; // la table des �l�ments
};

// fin de l'inclusion conditionnelle
#endif
