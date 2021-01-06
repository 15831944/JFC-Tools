//
// Fichier: JMTFR03TBLModulations.h
// Auteur:  Sylvain SAMMURI
// Date:    18/06/2004
//

// on v�rifie que le fichier n'a pas d�j� �t� inclus
#ifndef JMTFR03_TABLE_MODULATIONS_H

// on d�finit la macro pour �viter les inclusions multiples
#define JMTFR03_TABLE_MODULATIONS_H

// on inclut les d�finitions n�cessaires
#include "JMTFR03KEYModulation.h"
#include "JMTFR03DATModulation.h"

// on inclut les d�finitions n�cessaires
#include "JMTFR03TBLTerrains.h"

class JMTFR03TBLModulations
{
public:
	// le constructeur
	JMTFR03TBLModulations();

	// la fonction pour r�cup�rer le nombre d'�l�ments
	JInt32 GetCount() const;

	// la fonction pour ajouter les �l�ments
	JMTFR03DATModulation & Add(const JMTFR03KEYModulation & KEYModulation, const JMTFR03TBLTerrains & TBLTerrains, JBool Move = false);

	// la fonction pour tester le curseur
	JBool IsValid() const;

	// les fonctions pour r�cup�rer l'�l�ment du curseur
	const JMTFR03KEYModulation & GetKey() const;
	const JMTFR03DATModulation & GetItem() const;

	// la fonction pour modifier l'�l�ment du curseur
	JMTFR03DATModulation & GetItem();

	// les fonctions pour d�placer le curseur
	JVoid MoveTo(const JMTFR03KEYModulation & KEYModulation, JInt32 Move = 0) const;
	JVoid MoveFirst() const;
	JVoid MoveNext() const;
	JVoid MovePrev() const;
	JVoid MoveLast() const;

	// la fonction pour supprimer l'�l�ment courant
	JVoid Remove(JInt32 Move = 0);

	// la fonction pour permuter les �l�ments
	JVoid Swap(JMTFR03TBLModulations & Source);

	// la fonction pour lib�rer les �l�ments
	JVoid Reset();

	// le destructeur
	~JMTFR03TBLModulations();

private:
	// l'op�rateur pour recopier les �l�ments
	JMTFR03TBLModulations & operator =(const JMTFR03TBLModulations & Source);

	// le constructeur copie
	JMTFR03TBLModulations(const JMTFR03TBLModulations & Source);

private:
	JMap<JMTFR03KEYModulation, JMTFR03DATModulation> m_TBLModulations; // la table des �l�ments
};

// fin de l'inclusion conditionnelle
#endif
