//
// Fichier: JMTFR03TBLAttributs.h
// Auteur:  Sylvain SAMMURI
// Date:    10/09/2003
//

// on vérifie que le fichier n'a pas déjà été inclus
#ifndef JMTFR03_TABLE_ATTRIBUTS_H

// on définit la macro pour éviter les inclusions multiples
#define JMTFR03_TABLE_ATTRIBUTS_H

// on inclut les définitions nécessaires
#include "JMTFR03KEYAttribut.h"
#include "JMTFR03DATAttribut.h"

class JMTFR03TBLAttributs
{
public:
	// le constructeur
	JMTFR03TBLAttributs();

	// la fonction pour récupérer le nombre d'éléments
	JInt32 GetCount() const;

	// la fonction pour ajouter les éléments
	JMTFR03DATAttribut & Add(const JMTFR03KEYAttribut & KEYAttribut, JBool Move = false);

	// la fonction pour tester le curseur
	JBool IsValid() const;

	// les fonctions pour récupérer l'élément du curseur
	const JMTFR03KEYAttribut & GetKey() const;
	const JMTFR03DATAttribut & GetItem() const;

	// la fonction pour modifier l'élément du curseur
	JMTFR03DATAttribut & GetItem();

	// les fonctions pour déplacer le curseur
	JVoid MoveTo(const JMTFR03KEYAttribut & KEYAttribut, JInt32 Move = 0) const;
	JVoid MoveFirst() const;
	JVoid MoveNext() const;
	JVoid MovePrev() const;
	JVoid MoveLast() const;

	// la fonction pour supprimer l'élément du curseur
	JVoid Remove(JInt32 Move = 0);

	// la fonction pour permuter les éléments
	JVoid Swap(JMTFR03TBLAttributs & Source);

	// la fonction pour libérer les éléments
	JVoid Reset();

	// le destructeur
	~JMTFR03TBLAttributs();

private:
	// l'opérateur pour recopier les éléments
	JMTFR03TBLAttributs & operator =(const JMTFR03TBLAttributs & Source);

	// le constructeur copie
	JMTFR03TBLAttributs(const JMTFR03TBLAttributs & Source);

private:
	JMap<JMTFR03KEYAttribut, JMTFR03DATAttribut> m_TBLItems; // la table des éléments
};

// fin de l'inclusion conditionnelle
#endif
