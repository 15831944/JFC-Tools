//==============================
// fichier: ARMLStateBody.h
//
// date: 07/09/2001
// auteur: JB
//==============================

// on teste si la macro qui identifie le fichier est déjà définie
#ifndef _ARMLSTATEBODY_H_

// on définit une macro pour identifier le fichier
#define _ARMLSTATEBODY_H_

// on inclut les fichiers nécessaires
#include "ARMLState.h"
#include "ARMLStateError.h"
#include "./../Composite/JFCDrawBody.h"
#include "./../Composite/JFCDrawPage.h"

class StateBody : public State
{
public:
	// le constructeur
	StateBody(Context * contexte);

	// la fonction pour gérer l'état corps du document
	bool Manage();

	// le destructeur
	~StateBody();

protected:
	// la fonction pour passer à l'état erreur
	bool GotoError();

protected:

};
// fin du test sur la macro
#endif
