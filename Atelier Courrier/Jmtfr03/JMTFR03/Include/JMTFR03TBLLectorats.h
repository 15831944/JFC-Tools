//
// Fichier: JMTFR03TBLLectorats.h
// Auteur:  Sylvain SAMMURI
// Date:    02/12/2003
//

// on v�rifie que le fichier n'a pas d�j� �t� inclus
#ifndef JMTFR03_TABLE_LECTORATS_H

// on d�finit la macro pour �viter les inclusions multiples
#define JMTFR03_TABLE_LECTORATS_H

// on inclut les d�finitions n�cessaires
#include "JMTFR03KEYLectorat.h"
#include "JMTFR03DATLectorat.h"

class JMTFR03TBLLectorats
{
public:
	// le constructeur
	JMTFR03TBLLectorats();

	// la fonction pour r�cup�rer le nombre d'�l�ments
	JInt32 GetCount() const;

	// la fonction pour ajouter les �l�ments
	JMTFR03DATLectorat & Add(const JMTFR03KEYLectorat & KEYLectorat, JBool Move = false);

	// la fonction pour tester le curseur
	JBool IsValid() const;

	// les fonctions pour r�cup�rer l'�l�ment du curseur
	const JMTFR03KEYLectorat & GetKey() const;
	const JMTFR03DATLectorat & GetItem() const;

	// la fonction pour modifier l'�l�ment du curseur
	JMTFR03DATLectorat & GetItem();

	// les fonctions pour d�placer le curseur
	JVoid MoveTo(const JMTFR03KEYLectorat & KEYLectorat, JInt32 Move = 0) const;
	JVoid MoveFirst() const;
	JVoid MoveNext() const;
	JVoid MovePrev() const;
	JVoid MoveLast() const;

	// la fonction pour supprimer l'�l�ment du curseur
	JVoid Remove(JInt32 Move = 0);

	// la fonction pour permuter les �l�ments
	JVoid Swap(JMTFR03TBLLectorats & Source);

	// la fonction pour lib�rer les �l�ments
	JVoid Reset();

	// le destructeur
	~JMTFR03TBLLectorats();

private:
	// l'op�rateur pour recopier les �l�ments
	JMTFR03TBLLectorats & operator =(const JMTFR03TBLLectorats & Source);

	// le constructeur copie
	JMTFR03TBLLectorats(const JMTFR03TBLLectorats & Source);

private:
	JMap<JMTFR03KEYLectorat, JMTFR03DATLectorat> m_TBLItems; // la table des �l�ments
};

// fin de l'inclusion conditionnelle
#endif
