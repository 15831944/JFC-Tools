//==============================
// fichier: ARMLStateError.cpp
//
// date: 06/09/2001
// auteur: JB
//==============================

// on inclut les fichiers nécessaires
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
// la fonction pour gérer l'état courant:
//===========================================================
bool StateError::Manage()
{
	// on affiche une boite de message
	AfxMessageBox("ERREUR");

	// on libère l'état courant
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