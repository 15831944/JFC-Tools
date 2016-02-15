//==============================
// fichier: ARMLStateError.h
//
// date: 06/09/2001
// auteur: JB
//==============================

// on teste si la macro qui identifie le fichier est d�j� d�finie
#ifndef _ARMLSTATEERROR_H_

// on d�finit une macro pour identifier le fichier
#define _ARMLSTATEERROR_H_

// on inclut les fichiers n�cessaires
#include "ARMLState.h"


class StateError : public State
{
public:
	// le constructeur
	StateError(Context * contexte);

	// la fonction pour g�rer l'�tat courant
	bool Manage();

	// le destructeur
	~StateError();
protected:
};
// fin du test sur la macro
#endif
