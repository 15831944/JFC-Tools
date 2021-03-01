//
// Fichier: JMTFR03Couplage.h
// Auteur:  Sylvain SAMMURI
// Date:    17/02/2005
//

// on v�rifie que le fichier n'a pas d�j� �t� inclus
#ifndef JMTFR03_COUPLAGE_H

// on d�finit la macro pour �viter les inclusions multiples
#define JMTFR03_COUPLAGE_H

// on inclut les d�finitions n�cessaires
#include "JLib.h"

class JMTFR03Couplage
{
public:
	// le constructeur
	JMTFR03Couplage();

	// la fonction pour r�cup�rer le nombre d'�l�ments
	JInt32 GetCount() const;

	// la fonction pour ajouter les �l�ments
	JVoid Add(const JRef32 & IdTitre, JBool Move = false);

	// la fonction pour tester le curseur
	JBool IsValid() const;

	// la fonction pour r�cup�rer l'�l�ment du curseur
	const JRef32 & GetIdTitre() const;

	// les fonctions pour d�placer le curseur
	JVoid MoveTo(const JRef32 & IdTitre, JInt32 Move = 0) const;
	JVoid MoveFirst() const;
	JVoid MoveNext() const;
	JVoid MovePrev() const;
	JVoid MoveLast() const;

	// la fonction pour supprimer l'�l�ment du curseur
	JVoid Remove(JInt32 Move = 0);

	// la fonction pour permuter les �l�ments
	JVoid Swap(JMTFR03Couplage & Source);

	// la fonction pour lib�rer les �l�ments
	JVoid Reset();

	// le destructeur
	~JMTFR03Couplage();

private:
	// l'op�rateur pour recopier les �l�ments
	JMTFR03Couplage & operator =(const JMTFR03Couplage & Source);

	// le constructeur copie
	JMTFR03Couplage(const JMTFR03Couplage & Source);

private:
	JMap<JRef32, JInt32x> m_TBLItems; // la table des �l�ments
};

// fin de l'inclusion conditionnelle
#endif
