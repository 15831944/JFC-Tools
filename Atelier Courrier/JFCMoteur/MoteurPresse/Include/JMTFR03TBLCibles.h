//
// Fichier: JMTFR03TBLCibles.h
// Auteur:  Sylvain SAMMURI
// Date:    09/12/2003
//

// on v�rifie que le fichier n'a pas d�j� �t� inclus
#ifndef JMTFR03_TABLE_CIBLES_H

// on d�finit la macro pour �viter les inclusions multiples
#define JMTFR03_TABLE_CIBLES_H

// on inclut les d�finitions n�cessaires
#include "JMTFR03KEYCible.h"
#include "JMTFR03DATCible.h"

class JMTFR03TBLCibles
{
public:
	// le constructeur
	JMTFR03TBLCibles();

	// la fonction pour r�cup�rer le nombre d'�l�ments
	JInt32 GetCount() const;

	// les fonctions pour ajouter les �l�ments
	const JMTFR03DATCible & Add(const JMTFR03KEYCible & KEYCible, const JBitArray & ItemCible, JBool Move = false);
	const JMTFR03DATCible & Add(const JMTFR03KEYCible & KEYCible, const JUnt32* pItemCible,  JInt32 NbIndividus, JBool Move = false);
	const JMTFR03DATCible & Add(const JMTFR03KEYCible & KEYCible, const JFlt64* pItemProbas, JInt32 NbIndividus, JBool Move = false);

	// la fonction pour tester le curseur
	JBool IsValid() const;

	// les fonctions pour r�cup�rer l'�l�ment du curseur
	const JMTFR03KEYCible & GetKey() const;
	const JMTFR03DATCible & GetItem() const;

	// les fonctions pour d�placer le curseur
	JVoid MoveTo(const JMTFR03KEYCible & KEYCible, JInt32 Move = 0) const;
	JVoid MoveFirst() const;
	JVoid MoveNext() const;
	JVoid MovePrev() const;
	JVoid MoveLast() const;

	// la fonction pour supprimer l'�l�ment du curseur
	JVoid Remove(JInt32 Move = 0);

	// la fonction pour permuter les �l�ments
	JVoid Swap(JMTFR03TBLCibles & Source);

	// la fonction pour lib�rer les �l�ments
	JVoid Reset();

	// le destructeur
	~JMTFR03TBLCibles();

private:
	// l'op�rateur pour recopier les �l�ments
	JMTFR03TBLCibles & operator =(const JMTFR03TBLCibles & Source);

	// le constructeur copie
	JMTFR03TBLCibles(const JMTFR03TBLCibles & Source);

private:
	JMap<JMTFR03KEYCible, JMTFR03DATCible> m_TBLCibles; // la table des cibles
};

// fin de l'inclusion conditionnelle
#endif
