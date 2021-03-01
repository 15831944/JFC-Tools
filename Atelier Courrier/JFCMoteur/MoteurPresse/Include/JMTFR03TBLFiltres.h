//
// Fichier: JMTFR03TBLFiltres.h
// Auteur:  Sylvain SAMMURI
// Date:    10/09/2003
//

// on vérifie que le fichier n'a pas déjà été inclus
#ifndef JMTFR03_TABLE_FILTRES_H

// on définit la macro pour éviter les inclusions multiples
#define JMTFR03_TABLE_FILTRES_H

// on inclut les définitions nécessaires
#include "JMTFR03KEYFiltre.h"
#include "JMTFR03DATFiltre.h"

class JMTFR03TBLFiltres
{
public:
	// le constructeur
	JMTFR03TBLFiltres();

	// la fonction pour récupérer le nombre d'éléments
	JInt32 GetCount() const;

	// la fonction pour ajouter les éléments
	JMTFR03DATFiltre & Add(const JMTFR03KEYFiltre & KEYFiltre, JBool Move = false);

	// la fonction pour tester le curseur
	JBool IsValid() const;

	// les fonctions pour récupérer l'élément du curseur
	const JMTFR03KEYFiltre & GetKey() const;
	const JMTFR03DATFiltre & GetItem() const;

	// la fonction pour modifier l'élément du curseur
	JMTFR03DATFiltre & GetItem();

	// les fonctions pour déplacer le curseur
	JVoid MoveTo(const JMTFR03KEYFiltre & KEYFiltre, JInt32 Move = 0) const;
	JVoid MoveFirst() const;
	JVoid MoveNext() const;
	JVoid MovePrev() const;
	JVoid MoveLast() const;

	// la fonction pour supprimer l'élément du curseur
	JVoid Remove(JInt32 Move = 0);

	// la fonction pour permuter les éléments
	JVoid Swap(JMTFR03TBLFiltres & Source);

	// la fonction pour libérer les éléments
	JVoid Reset();

	// le destructeur
	~JMTFR03TBLFiltres();

private:
	// l'opérateur pour recopier les éléments
	JMTFR03TBLFiltres & operator =(const JMTFR03TBLFiltres & Source);

	// le constructeur copie
	JMTFR03TBLFiltres(const JMTFR03TBLFiltres & Source);

private:
	JMap<JMTFR03KEYFiltre, JMTFR03DATFiltre> m_TBLItems; // la table des éléments
};

// fin de l'inclusion conditionnelle
#endif
