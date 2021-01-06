//
// Fichier: JMTFR03TBLEchantillons.h
// Auteur:  Sylvain SAMMURI
// Date:    10/09/2003
//

// on vérifie que le fichier n'a pas déjà été inclus
#ifndef JMTFR03_TABLE_ECHANTILLONS_H

// on définit la macro pour éviter les inclusions multiples
#define JMTFR03_TABLE_ECHANTILLONS_H

// on inclut les définitions nécessaires
#include "JMTFR03KEYEchantillon.h"
#include "JMTFR03DATEchantillon.h"

class JMTFR03TBLEchantillons
{
public:
	// le constructeur
	JMTFR03TBLEchantillons();

	// la fonction pour récupérer le nombre d'éléments
	JInt32 GetCount() const;

	// la fonction pour ajouter les éléments
	JMTFR03DATEchantillon & Add(const JMTFR03KEYEchantillon & KEYEchantillon, JBool Move = false);

	// la fonction pour tester le curseur
	JBool IsValid() const;

	// les fonctions pour récupérer l'élément du curseur
	const JMTFR03KEYEchantillon & GetKey() const;
	const JMTFR03DATEchantillon & GetItem() const;

	// la fonction pour modifier l'élément du curseur
	JMTFR03DATEchantillon & GetItem();

	// les fonctions pour déplacer le curseur
	JVoid MoveTo(const JMTFR03KEYEchantillon & KEYEchantillon, JInt32 Move = 0) const;
	JVoid MoveFirst() const;
	JVoid MoveNext() const;
	JVoid MovePrev() const;
	JVoid MoveLast() const;

	// la fonction pour supprimer l'élément du curseur
	JVoid Remove(JInt32 Move = 0);

	// la fonction pour permuter les éléments
	JVoid Swap(JMTFR03TBLEchantillons & Source);

	// la fonction pour libérer les éléments
	JVoid Reset();

	// le destructeur
	~JMTFR03TBLEchantillons();

private:
	// l'opérateur pour recopier les éléments
	JMTFR03TBLEchantillons & operator =(const JMTFR03TBLEchantillons & Source);

	// le constructeur copie
	JMTFR03TBLEchantillons(const JMTFR03TBLEchantillons & Source);

private:
	JMap<JMTFR03KEYEchantillon, JMTFR03DATEchantillon> m_TBLItems; // la table des éléments
};

// fin de l'inclusion conditionnelle
#endif
