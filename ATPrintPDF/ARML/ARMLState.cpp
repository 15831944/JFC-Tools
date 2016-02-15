//=========================
// fichier: ARMLState.cpp
//
// date: 06/09/2001
// auteur: JB
//=========================

// on inclut les fichiers nécessaires
#include "stdafx.h"
#include "ARMLState.h"


//=================================================================
// le constructeur:
//=================================================================
State::State(Context * contexte)
{
	// on initialise les paramètres
	m_Context = contexte;

	// on signale l'état courant au contexte
	m_Context->SetState(this);
}

//==================================================================
// le destructeur:
//==================================================================
State::~State()
{
	// on ne fait rien
}