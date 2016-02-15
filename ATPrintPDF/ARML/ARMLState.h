//=========================
// fichier: ARMLState.h
//
// date: 06/09/2001
// auteur: JB
//=========================

// on teste si la macro qui identifie le fichier est déjà définie
#ifndef _ARMLSTATE_H_

// on définit une macro pour identifier le fichier
#define _ARMLSTATE_H_

// on inclut les fichiers nécessaires
#include "ARMLContext.h"
#include "Afxtempl.h "
class Context;

class State
{
public:
	// le constructeur
	State(Context * contexte);

	// la fonction pour gérer l'état
	virtual bool Manage() = 0;

	// le destructeur
	virtual ~State();
protected:
	// le lien avec le contexte
	Context * m_Context;
};
// fin du test sur la macro
#endif