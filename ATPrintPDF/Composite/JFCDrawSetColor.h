//==============================
// fichier: JFCDrawSetColor.h
//
// date: 02/09/2001
// auteur: JB
//==============================

// on teste si la macro qui identifie le fichier est déjà définie
#ifndef _JFCDRAWSETCOLOR_H_

// on définit une macro pour identifier le fichier
#define _JFCDRAWSETCOLOR_H_

// on inclut les fichiers nécessaires
#include "JFCDrawComponent.h"

class DrawSetColor : public DrawComponent
{
public:
	// le constructeur
	DrawSetColor(long color);

	// le constructeur de recopie
	DrawSetColor(const DrawSetColor & source);

	// la fonction pour dessiner le composant
	void Draw(JFCDraw * pDraw, long firstpage, long lastpage);

	// la fonction de clonage
	DrawComponent * Clone() const;

	// le destructeur
	~DrawSetColor();
protected:
	// la couleur
	long m_Color;
};
// fin du test sur la macro
#endif