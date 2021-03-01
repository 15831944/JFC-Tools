//
// Fichier: JTrusquin.h
// Auteur:  Sylvain SAMMURI
// Date:    01/03/2005
//

// on v�rifie que le fichier n'a pas d�j� �t� inclus
#ifndef JTRUSQUIN_H

// on d�finit la macro pour �viter les inclusions multiples
#define JTRUSQUIN_H

// on inclut les d�finitions n�cessaires
#include "JLib.h"

class JTrusquin
{
public:
	// le constructeur
	JTrusquin();

	// la fonction pour r�cup�rer le nombre d'�l�ments
	JInt32 GetCount() const;

	// la fonction pour d�finir les �l�ments
	JVoid SetItems(JFlt64 P, JFlt64 N);

	// la fonction pour r�cup�rer les �l�ments
	const JFlt64 & Item(JInt32 Index) const;

	// la fonction pour permuter les �l�ments
	JVoid Swap(JTrusquin & Source);

	// la fonction pour lib�rer les �l�ments
	JVoid Reset();

	// le destructeur
	~JTrusquin();

private:
	// l'op�rateur pour recopier les �l�ments
	JTrusquin & operator =(const JTrusquin & Source);

	// le constructeur copie
	JTrusquin(const JTrusquin & Source);

private:
	JArray<JFlt64> m_TBLItems; // la table des �l�ments
};

// fin de l'inclusion conditionnelle
#endif
