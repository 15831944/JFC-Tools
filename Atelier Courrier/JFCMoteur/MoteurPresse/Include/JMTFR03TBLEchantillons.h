//
// Fichier: JMTFR03TBLEchantillons.h
// Auteur:  Sylvain SAMMURI
// Date:    10/09/2003
//

// on v�rifie que le fichier n'a pas d�j� �t� inclus
#ifndef JMTFR03_TABLE_ECHANTILLONS_H

// on d�finit la macro pour �viter les inclusions multiples
#define JMTFR03_TABLE_ECHANTILLONS_H

// on inclut les d�finitions n�cessaires
#include "JMTFR03KEYEchantillon.h"
#include "JMTFR03DATEchantillon.h"

class JMTFR03TBLEchantillons
{
public:
	// le constructeur
	JMTFR03TBLEchantillons();

	// la fonction pour r�cup�rer le nombre d'�l�ments
	JInt32 GetCount() const;

	// la fonction pour ajouter les �l�ments
	JMTFR03DATEchantillon & Add(const JMTFR03KEYEchantillon & KEYEchantillon, JBool Move = false);

	// la fonction pour tester le curseur
	JBool IsValid() const;

	// les fonctions pour r�cup�rer l'�l�ment du curseur
	const JMTFR03KEYEchantillon & GetKey() const;
	const JMTFR03DATEchantillon & GetItem() const;

	// la fonction pour modifier l'�l�ment du curseur
	JMTFR03DATEchantillon & GetItem();

	// les fonctions pour d�placer le curseur
	JVoid MoveTo(const JMTFR03KEYEchantillon & KEYEchantillon, JInt32 Move = 0) const;
	JVoid MoveFirst() const;
	JVoid MoveNext() const;
	JVoid MovePrev() const;
	JVoid MoveLast() const;

	// la fonction pour supprimer l'�l�ment du curseur
	JVoid Remove(JInt32 Move = 0);

	// la fonction pour permuter les �l�ments
	JVoid Swap(JMTFR03TBLEchantillons & Source);

	// la fonction pour lib�rer les �l�ments
	JVoid Reset();

	// le destructeur
	~JMTFR03TBLEchantillons();

private:
	// l'op�rateur pour recopier les �l�ments
	JMTFR03TBLEchantillons & operator =(const JMTFR03TBLEchantillons & Source);

	// le constructeur copie
	JMTFR03TBLEchantillons(const JMTFR03TBLEchantillons & Source);

private:
	JMap<JMTFR03KEYEchantillon, JMTFR03DATEchantillon> m_TBLItems; // la table des �l�ments
};

// fin de l'inclusion conditionnelle
#endif
