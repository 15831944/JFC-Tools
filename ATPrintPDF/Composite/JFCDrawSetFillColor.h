//==================================
// fichier: JFCDrawSetFillColor.h
// 
// date: 02/09/2001
// auteur: JB
//==================================

// on teste si la macro qui identifie le fichier est déjà définie
#ifndef _JFCDRAWSETFILLCOLOR_H_

// on définit une macro pour identifier le fichier
#define _JFCDRAWSETFILLCOLOR_H_

// on inclut les fichiers nécessaires
#include "JFCDrawComponent.h"

class DrawSetFillColor : public DrawComponent
{
public:
	// le constructeur
	DrawSetFillColor(long fillcolor);

	// le constructeur de recopie
	DrawSetFillColor(const DrawSetFillColor & source);

	// la fonction pour dessiner le composant
	void Draw(JFCDraw * pDraw, long firstpage, long lastpage);

	// la fonction de clonage
	DrawComponent * Clone() const;

	// le destructeur
	~DrawSetFillColor();
protected:
	// la couleur de fond
	long m_FillColor;
};
// fin du test sur la macro
#endif