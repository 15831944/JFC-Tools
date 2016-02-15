//===========================
// fichier: ARMLStateEnd.h
// 
// date: 07/09/2001
// auteur: JB
//===========================

// on teste si la macro qui identifie le fichier est déjà définie
#ifndef _ARMLSTATEEND_H_

// on définit une macro pour identifier le fichier
#define _ARMLSTATEEND_H_

// on inclut les fichiers nécessaires
#include "ARMLState.h"

class StateEnd : public State
{
public:
	// le constructeur
	StateEnd(Context * contexte);

	// la fonction pour gérer l'état courant
	bool Manage();

	// le destructeur
	~StateEnd();
protected:

};
// fin du test sur la macro
#endif