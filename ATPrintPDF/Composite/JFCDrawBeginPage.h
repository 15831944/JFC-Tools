//==============================
// fichier: JFCDrawBeginPage.h
//
// date: 02/09/2001
// auteur: JB
//==============================

// on teste si la macro qui identifie le fichier est déjà définie
#ifndef _JFCDRAWBEGINPAGE_H_

// on définit une macro pour identifier le fichier
#define _JFCDRAWBEGINPAGE_H_

// on inclut les fichiers nécessaires
#include "JFCDrawComponent.h"

class DrawBeginPage : public DrawComponent
{
public:
	// le constructeur
	DrawBeginPage();

	// le constructeur de recopie
	DrawBeginPage(const DrawBeginPage & source);

	// la fonction pour dessiner le composant
	void Draw(JFCDraw * pDraw, long firstpage, long lastpage);

	// la fonction de clonage
	DrawComponent * Clone() const;

	// le destructeur
	~DrawBeginPage();
protected:
	// pas d'argument pour cette commande
};
// fin du test sur la macro
#endif