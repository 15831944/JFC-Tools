//==============================
// fichier: ARMLStateTable.h
//
// date: 07/09/2001
// auteur: JB
//==============================

// on teste si la macro qui identifie le fichier est déjà définie
#ifndef _ARMLSTATETABLE_H_

// on définit une macro pour identifier le fichier
#define _ARMLSTATETABLE_H_

// on inclut les fichiers nécessaires
#include "ARMLState.h"
#include "ARMLStateError.h"
#include "./../Composite/JFCDrawTableau.h"

class StateTable : public State
{
public:
	// le constructeur
	StateTable(Context * contexte, CTable * table);

	// la fonction pour gérer l'état tableau
	bool Manage();

	// le destructeur
	~StateTable();

protected:
	// la fonction pour passer à l'état erreur
	bool GotoError();

	// la fonction pour trouver la position de départ
	long GetStartPosX(long startcol);

	// la fonction pour trouver la position d'arrivée
	long GetEndPosX(long endcol);
protected:
	// le pointeur sur le tableau
	CTable * m_pTable;

	// le tableau de styles de traits
	CArray<long, long> m_ArrayStrokeStyle;

	// le tableau des justifications
	CArray<char, char> m_ArrayJustification;

	// le tableau des largeurs de colonnes
	CArray<long, long> m_ArrayColWidth;

	// la couleur utilisée
	long m_couleur;
};
// fin du test sur la macro
#endif