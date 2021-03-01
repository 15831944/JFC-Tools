//
// Fichier: JMTFR03TBLFiltres.h
// Auteur:  Sylvain SAMMURI
// Date:    10/09/2003
//

// on v�rifie que le fichier n'a pas d�j� �t� inclus
#ifndef JMTFR03_TABLE_FILTRES_H

// on d�finit la macro pour �viter les inclusions multiples
#define JMTFR03_TABLE_FILTRES_H

// on inclut les d�finitions n�cessaires
#include "JMTFR03KEYFiltre.h"
#include "JMTFR03DATFiltre.h"

class JMTFR03TBLFiltres
{
public:
	// le constructeur
	JMTFR03TBLFiltres();

	// la fonction pour r�cup�rer le nombre d'�l�ments
	JInt32 GetCount() const;

	// la fonction pour ajouter les �l�ments
	JMTFR03DATFiltre & Add(const JMTFR03KEYFiltre & KEYFiltre, JBool Move = false);

	// la fonction pour tester le curseur
	JBool IsValid() const;

	// les fonctions pour r�cup�rer l'�l�ment du curseur
	const JMTFR03KEYFiltre & GetKey() const;
	const JMTFR03DATFiltre & GetItem() const;

	// la fonction pour modifier l'�l�ment du curseur
	JMTFR03DATFiltre & GetItem();

	// les fonctions pour d�placer le curseur
	JVoid MoveTo(const JMTFR03KEYFiltre & KEYFiltre, JInt32 Move = 0) const;
	JVoid MoveFirst() const;
	JVoid MoveNext() const;
	JVoid MovePrev() const;
	JVoid MoveLast() const;

	// la fonction pour supprimer l'�l�ment du curseur
	JVoid Remove(JInt32 Move = 0);

	// la fonction pour permuter les �l�ments
	JVoid Swap(JMTFR03TBLFiltres & Source);

	// la fonction pour lib�rer les �l�ments
	JVoid Reset();

	// le destructeur
	~JMTFR03TBLFiltres();

private:
	// l'op�rateur pour recopier les �l�ments
	JMTFR03TBLFiltres & operator =(const JMTFR03TBLFiltres & Source);

	// le constructeur copie
	JMTFR03TBLFiltres(const JMTFR03TBLFiltres & Source);

private:
	JMap<JMTFR03KEYFiltre, JMTFR03DATFiltre> m_TBLItems; // la table des �l�ments
};

// fin de l'inclusion conditionnelle
#endif
