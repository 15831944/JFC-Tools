//==============================
// fichier: ARMLStateHeader.h
//
// date: 06/09/2001
// auteur: JB
//==============================

// on teste si la macro qui identifie le fichier est d�j� d�finie
#ifndef _ARMLSTATEHEADER_H_

// on d�finit une macro pour identifier le fichier
#define _ARMLSTATEHEADER_H_

// on inclut les fichiers n�cessaires
#include "ARMLState.h"
#include "ARMLStateError.h"
#include "./../Composite/JFCDrawHeader.h"


class StateHeader : public State
{
public:
	// le constructeur
	StateHeader(Context * contexte);

	// la fonction pour traiter l'�tat courant
	bool Manage();

	// le destructeur
	~StateHeader();
protected:
	// la fonction pour passer � l'�tat erreur
	bool GotoError();

protected:
};
// fin du test sur la macro
#endif