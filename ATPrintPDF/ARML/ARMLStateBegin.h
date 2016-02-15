//==============================
// fichier: ARMLStateBegin.h
//
// date: 06/09/2001
// auteur: JB
//==============================

// on teste si la macro qui identifie le fichier est déjà définie
#ifndef _ARMLSTATEBEGIN_H_

// on définit une macro pour identifier le fichier
#define _ARMLSTATEBEGIN_H_

// on inclut les fichiers nécessaires
#include "ARMLState.h"
#include "ARMLStateError.h"

class StateBegin : public State
{
public:
	// le constructeur
	StateBegin(Context * contexte);

	// la fonction pour gérer l'état
	bool Manage();

	// le destructeur
	~StateBegin();
protected:
	// la fonction pour passer à l'état erreur
	bool GotoError();
};
// fin du test sur la macro
#endif