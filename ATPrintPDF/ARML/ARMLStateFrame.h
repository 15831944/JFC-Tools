//=============================
// fichier: ARMLStateFrame.h
// 
// date: 06/09/2001
// auteur: JB
//=============================

// on teste si la macro qui identifie le fichier est déjà définie
#ifndef _ARMLSTATEFRAME_H_

// on définit une macro pour identifier le fichier
#define _ARMLSTATEFRAME_H_

// on inclut les fichiers nécessaires
#include "ARMLState.h"
#include "ARMLStateError.h"
#include "./../Composite/JFCDrawFrame.h"

class StateFrame : public State
{
public:
	// le constructeur
	StateFrame(Context * contexte, CFrame * pframe);

	// la fonction pour gérer l'état
	bool Manage();

	// le destructeur
	~StateFrame();
protected:
	// la fonction pour passer à l'état erreur
	bool GotoError();

protected:
	// le composant frame
	CFrame * m_pFrame;
};
// fin du test sur la macro
#endif