//=========================
// fichier: ARMLState.cpp
//
// date: 06/09/2001
// auteur: JB
//=========================

// on inclut les fichiers n�cessaires
#include "stdafx.h"
#include "ARMLState.h"


//=================================================================
// le constructeur:
//=================================================================
State::State(Context * contexte)
{
	// on initialise les param�tres
	m_Context = contexte;

	// on signale l'�tat courant au contexte
	m_Context->SetState(this);
}

//==================================================================
// le destructeur:
//==================================================================
State::~State()
{
	// on ne fait rien
}