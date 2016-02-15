//=============================
// fichier: ARMLStateFrame.h
// 
// date: 06/09/2001
// auteur: JB
//=============================

// on teste si la macro qui identifie le fichier est d�j� d�finie
#ifndef _ARMLSTATEFRAME_H_

// on d�finit une macro pour identifier le fichier
#define _ARMLSTATEFRAME_H_

// on inclut les fichiers n�cessaires
#include "ARMLState.h"
#include "ARMLStateError.h"
#include "./../Composite/JFCDrawFrame.h"

class StateFrame : public State
{
public:
	// le constructeur
	StateFrame(Context * contexte, CFrame * pframe);

	// la fonction pour g�rer l'�tat
	bool Manage();

	// le destructeur
	~StateFrame();
protected:
	// la fonction pour passer � l'�tat erreur
	bool GotoError();

protected:
	// le composant frame
	CFrame * m_pFrame;
};
// fin du test sur la macro
#endif