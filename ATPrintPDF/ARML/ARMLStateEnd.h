//===========================
// fichier: ARMLStateEnd.h
// 
// date: 07/09/2001
// auteur: JB
//===========================

// on teste si la macro qui identifie le fichier est d�j� d�finie
#ifndef _ARMLSTATEEND_H_

// on d�finit une macro pour identifier le fichier
#define _ARMLSTATEEND_H_

// on inclut les fichiers n�cessaires
#include "ARMLState.h"

class StateEnd : public State
{
public:
	// le constructeur
	StateEnd(Context * contexte);

	// la fonction pour g�rer l'�tat courant
	bool Manage();

	// le destructeur
	~StateEnd();
protected:

};
// fin du test sur la macro
#endif