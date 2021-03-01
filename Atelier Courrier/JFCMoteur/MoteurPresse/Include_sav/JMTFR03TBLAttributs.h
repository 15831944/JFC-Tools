//
// Fichier: JMTFR03TBLAttributs.h
// Auteur:  Sylvain SAMMURI
// Date:    10/09/2003
//

// on v�rifie que le fichier n'a pas d�j� �t� inclus
#ifndef JMTFR03_TABLE_ATTRIBUTS_H

// on d�finit la macro pour �viter les inclusions multiples
#define JMTFR03_TABLE_ATTRIBUTS_H

// on inclut les d�finitions n�cessaires
#include "JMTFR03KEYAttribut.h"
#include "JMTFR03DATAttribut.h"

class JMTFR03TBLAttributs
{
public:
	// le constructeur
	JMTFR03TBLAttributs();

	// la fonction pour r�cup�rer le nombre d'�l�ments
	JInt32 GetCount() const;

	// la fonction pour ajouter les �l�ments
	JMTFR03DATAttribut & Add(const JMTFR03KEYAttribut & KEYAttribut, JBool Move = false);

	// la fonction pour tester le curseur
	JBool IsValid() const;

	// les fonctions pour r�cup�rer l'�l�ment du curseur
	const JMTFR03KEYAttribut & GetKey() const;
	const JMTFR03DATAttribut & GetItem() const;

	// la fonction pour modifier l'�l�ment du curseur
	JMTFR03DATAttribut & GetItem();

	// les fonctions pour d�placer le curseur
	JVoid MoveTo(const JMTFR03KEYAttribut & KEYAttribut, JInt32 Move = 0) const;
	JVoid MoveFirst() const;
	JVoid MoveNext() const;
	JVoid MovePrev() const;
	JVoid MoveLast() const;

	// la fonction pour supprimer l'�l�ment du curseur
	JVoid Remove(JInt32 Move = 0);

	// la fonction pour permuter les �l�ments
	JVoid Swap(JMTFR03TBLAttributs & Source);

	// la fonction pour lib�rer les �l�ments
	JVoid Reset();

	// le destructeur
	~JMTFR03TBLAttributs();

private:
	// l'op�rateur pour recopier les �l�ments
	JMTFR03TBLAttributs & operator =(const JMTFR03TBLAttributs & Source);

	// le constructeur copie
	JMTFR03TBLAttributs(const JMTFR03TBLAttributs & Source);

private:
	JMap<JMTFR03KEYAttribut, JMTFR03DATAttribut> m_TBLItems; // la table des �l�ments
};

// fin de l'inclusion conditionnelle
#endif
