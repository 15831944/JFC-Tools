//
// Fichier: JMTFR03TBLModulations.h
// Auteur:  Sylvain SAMMURI
// Date:    18/06/2004
//

// on vérifie que le fichier n'a pas déjà été inclus
#ifndef JMTFR03_TABLE_MODULATIONS_H

// on définit la macro pour éviter les inclusions multiples
#define JMTFR03_TABLE_MODULATIONS_H

// on inclut les définitions nécessaires
#include "JMTFR03KEYModulation.h"
#include "JMTFR03DATModulation.h"

// on inclut les définitions nécessaires
#include "JMTFR03TBLTerrains.h"

class JMTFR03TBLModulations
{
public:
	// le constructeur
	JMTFR03TBLModulations();

	// la fonction pour récupérer le nombre d'éléments
	JInt32 GetCount() const;

	// la fonction pour ajouter les éléments
	JMTFR03DATModulation & Add(const JMTFR03KEYModulation & KEYModulation, const JMTFR03TBLTerrains & TBLTerrains, JBool Move = false);

	// la fonction pour tester le curseur
	JBool IsValid() const;

	// les fonctions pour récupérer l'élément du curseur
	const JMTFR03KEYModulation & GetKey() const;
	const JMTFR03DATModulation & GetItem() const;

	// la fonction pour modifier l'élément du curseur
	JMTFR03DATModulation & GetItem();

	// les fonctions pour déplacer le curseur
	JVoid MoveTo(const JMTFR03KEYModulation & KEYModulation, JInt32 Move = 0) const;
	JVoid MoveFirst() const;
	JVoid MoveNext() const;
	JVoid MovePrev() const;
	JVoid MoveLast() const;

	// la fonction pour supprimer l'élément courant
	JVoid Remove(JInt32 Move = 0);

	// la fonction pour permuter les éléments
	JVoid Swap(JMTFR03TBLModulations & Source);

	// la fonction pour libérer les éléments
	JVoid Reset();

	// le destructeur
	~JMTFR03TBLModulations();

private:
	// l'opérateur pour recopier les éléments
	JMTFR03TBLModulations & operator =(const JMTFR03TBLModulations & Source);

	// le constructeur copie
	JMTFR03TBLModulations(const JMTFR03TBLModulations & Source);

private:
	JMap<JMTFR03KEYModulation, JMTFR03DATModulation> m_TBLModulations; // la table des éléments
};

// fin de l'inclusion conditionnelle
#endif
