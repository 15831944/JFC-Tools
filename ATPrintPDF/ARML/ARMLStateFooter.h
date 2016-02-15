//===============================
// fichier: ARMLStateFooter.h
//
// date: 07/09/2001
// auteur: JB
//===============================

// on teste si la macro qui identifie le fichier est d�j� d�finie
#ifndef _ARMLSTATEFOOTER_H_

// on d�finit une macro pour identifier le fichier
#define _ARMLSTATEFOOTER_H_

// on inclut les fichiers n�cessaires
#include "ARMLState.h"
#include "ARMLStateError.h"
#include "./../Composite/JFCDrawFooter.h"

class StateFooter : public State
{
public:
	// le constructeur
	StateFooter(Context * contexte);

	// la fonction pour g�rer l'�tat pied de page
	bool Manage();

	// le destructeur
	~StateFooter();

protected:
	// la fonction pour passer � l'�tat erreur
	bool GotoError();

protected:
};
// fin du test sur la macro
#endif