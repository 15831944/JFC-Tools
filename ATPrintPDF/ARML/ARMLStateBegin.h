//==============================
// fichier: ARMLStateBegin.h
//
// date: 06/09/2001
// auteur: JB
//==============================

// on teste si la macro qui identifie le fichier est d�j� d�finie
#ifndef _ARMLSTATEBEGIN_H_

// on d�finit une macro pour identifier le fichier
#define _ARMLSTATEBEGIN_H_

// on inclut les fichiers n�cessaires
#include "ARMLState.h"
#include "ARMLStateError.h"

class StateBegin : public State
{
public:
	// le constructeur
	StateBegin(Context * contexte);

	// la fonction pour g�rer l'�tat
	bool Manage();

	// le destructeur
	~StateBegin();
protected:
	// la fonction pour passer � l'�tat erreur
	bool GotoError();
};
// fin du test sur la macro
#endif