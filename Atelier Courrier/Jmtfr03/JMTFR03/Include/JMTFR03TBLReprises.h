//
// Fichier: JMTFR03TBLReprises.h
// Auteur:  Sylvain SAMMURI
// Date:    06/04/2004
//

// on v�rifie que le fichier n'a pas d�j� �t� inclus
#ifndef JMTFR03_TABLE_REPRISES_H

// on d�finit la macro pour �viter les inclusions multiples
#define JMTFR03_TABLE_REPRISES_H

// on inclut les d�finitions n�cessaires
#include "JMTFR03KEYReprise.h"
#include "JMTFR03DATReprise.h"

class JMTFR03TBLReprises
{
public:
	// le constructeur
	JMTFR03TBLReprises();

	// la fonction pour r�cup�rer le nombre d'�l�ments
	JInt32 GetCount() const;

	// la fonction pour ajouter les �l�ments
	JMTFR03DATReprise & Add(const JMTFR03KEYReprise & KEYReprise, JBool Move = false);

	// la fonction pour tester le curseur
	JBool IsValid() const;

	// les fonctions pour r�cup�rer l'�l�ment du curseur
	const JMTFR03KEYReprise & GetKey() const;
	const JMTFR03DATReprise & GetItem() const;

	// la fonction pour modifier l'�l�ment du curseur
	JMTFR03DATReprise & GetItem();

	// les fonctions pour d�placer le curseur
	JVoid MoveTo(const JMTFR03KEYReprise & KEYReprise, JInt32 Move = 0) const;
	JVoid MoveFirst() const;
	JVoid MoveNext() const;
	JVoid MovePrev() const;
	JVoid MoveLast() const;

	// la fonction pour supprimer l'�l�ment du curseur
	JVoid Remove(JInt32 Move = 0);

	// la fonction pour permuter les �l�ments
	JVoid Swap(JMTFR03TBLReprises & Source);

	// la fonction pour lib�rer les �l�ments
	JVoid Reset();

	// le destructeur
	~JMTFR03TBLReprises();

private:
	// l'op�rateur pour recopier les �l�ments
	JMTFR03TBLReprises & operator =(const JMTFR03TBLReprises & Source);

	// le constructeur copie
	JMTFR03TBLReprises(const JMTFR03TBLReprises & Source);

private:
	JMap<JMTFR03KEYReprise, JMTFR03DATReprise> m_TBLItems; // la table des �l�ments
};

// fin de l'inclusion conditionnelle
#endif
