//==============================
// fichier: ARMLStateBody.h
//
// date: 07/09/2001
// auteur: JB
//==============================

// on teste si la macro qui identifie le fichier est d�j� d�finie
#ifndef _ARMLSTATEBODY_H_

// on d�finit une macro pour identifier le fichier
#define _ARMLSTATEBODY_H_

// on inclut les fichiers n�cessaires
#include "ARMLState.h"
#include "ARMLStateError.h"
#include "./../Composite/JFCDrawBody.h"
#include "./../Composite/JFCDrawPage.h"

class StateBody : public State
{
public:
	// le constructeur
	StateBody(Context * contexte);

	// la fonction pour g�rer l'�tat corps du document
	bool Manage();

	// le destructeur
	~StateBody();

protected:
	// la fonction pour passer � l'�tat erreur
	bool GotoError();

protected:

};
// fin du test sur la macro
#endif
