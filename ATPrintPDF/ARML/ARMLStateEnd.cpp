//=============================
// fichier: ARMLStateEnd.cpp
//
// date: 07/09/2001
// auteur: JB
//=============================

// on inclut les fichiers nécessaires
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
// la fonction pour gérer l'état courant:
//=======================================================
bool StateEnd::Manage()
{
	// on libère l'état courant
	delete (this);
	
	// on est arrivé ici donc ca c'est bien passé
	return (true);
}

//=======================================================
// le destructeur:
//=======================================================
StateEnd::~StateEnd()
{
	// on ne fait rien
}