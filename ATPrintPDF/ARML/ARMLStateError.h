//==============================
// fichier: ARMLStateError.h
//
// date: 06/09/2001
// auteur: JB
//==============================

// on teste si la macro qui identifie le fichier est déjà définie
#ifndef _ARMLSTATEERROR_H_

// on définit une macro pour identifier le fichier
#define _ARMLSTATEERROR_H_

// on inclut les fichiers nécessaires
#include "ARMLState.h"


class StateError : public State
{
public:
	// le constructeur
	StateError(Context * contexte);

	// la fonction pour gérer l'état courant
	bool Manage();

	// le destructeur
	~StateError();
protected:
};
// fin du test sur la macro
#endif
