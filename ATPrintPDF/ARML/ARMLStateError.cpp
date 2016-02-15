//==============================
// fichier: ARMLStateError.cpp
//
// date: 06/09/2001
// auteur: JB
//==============================

// on inclut les fichiers n�cessaires
#include "stdafx.h"
#include "ARMLStateError.h"

//===========================================================
// le constructeur:
//===========================================================
StateError::StateError(Context * contexte) : State(contexte)
{
	// on ne fait rien
}

//===========================================================
// la fonction pour g�rer l'�tat courant:
//===========================================================
bool StateError::Manage()
{
	// on affiche une boite de message
	AfxMessageBox("ERREUR");

	// on lib�re l'�tat courant
	delete (this);

	// on renvoie le code d'erreur
	return (false);
}

//===========================================================
// le destructeur:
//===========================================================
StateError::~StateError()
{
	// on ne fait rien
}