//=============================
// fichier: ARMLStateEnd.cpp
//
// date: 07/09/2001
// auteur: JB
//=============================

// on inclut les fichiers n�cessaires
#include "stdafx.h"
#include "ARMLStateEnd.h"

//=======================================================
// le constructeur:
//=======================================================
StateEnd::StateEnd(Context * contexte) : State(contexte)
{
	// on ne fait rien
}

//=======================================================
// la fonction pour g�rer l'�tat courant:
//=======================================================
bool StateEnd::Manage()
{
	// on lib�re l'�tat courant
	delete (this);
	
	// on est arriv� ici donc ca c'est bien pass�
	return (true);
}

//=======================================================
// le destructeur:
//=======================================================
StateEnd::~StateEnd()
{
	// on ne fait rien
}