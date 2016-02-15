//===============================
// fichier: ARMLStateFooter.h
//
// date: 07/09/2001
// auteur: JB
//===============================

// on teste si la macro qui identifie le fichier est déjà définie
#ifndef _ARMLSTATEFOOTER_H_

// on définit une macro pour identifier le fichier
#define _ARMLSTATEFOOTER_H_

// on inclut les fichiers nécessaires
#include "ARMLState.h"
#include "ARMLStateError.h"
#include "./../Composite/JFCDrawFooter.h"

class StateFooter : public State
{
public:
	// le constructeur
	StateFooter(Context * contexte);

	// la fonction pour gérer l'état pied de page
	bool Manage();

	// le destructeur
	~StateFooter();

protected:
	// la fonction pour passer à l'état erreur
	bool GotoError();

protected:
};
// fin du test sur la macro
#endif