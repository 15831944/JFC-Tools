//==============================
// fichier: ARMLStateHeader.h
//
// date: 06/09/2001
// auteur: JB
//==============================

// on teste si la macro qui identifie le fichier est déjà définie
#ifndef _ARMLSTATEHEADER_H_

// on définit une macro pour identifier le fichier
#define _ARMLSTATEHEADER_H_

// on inclut les fichiers nécessaires
#include "ARMLState.h"
#include "ARMLStateError.h"
#include "./../Composite/JFCDrawHeader.h"


class StateHeader : public State
{
public:
	// le constructeur
	StateHeader(Context * contexte);

	// la fonction pour traiter l'état courant
	bool Manage();

	// le destructeur
	~StateHeader();
protected:
	// la fonction pour passer à l'état erreur
	bool GotoError();

protected:
};
// fin du test sur la macro
#endif