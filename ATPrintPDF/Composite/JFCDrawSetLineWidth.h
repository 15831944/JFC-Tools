//==================================
// fichier: JFCDrawSetLineWidth.h
//
// date: 02/09/2001
// auteur: JB
//==================================

// on teste si la macro qui identifie le fichier est déjà définie
#ifndef _JFCDRAWSETLINEWIDTH_H_

// on définit une macro pour identifier le fichier
#define _JFCDRAWSETLINEWIDTH_H_

// on inclut les fichiers nécessaires
#include "JFCDrawComponent.h"

class DrawSetLineWidth : public DrawComponent
{
public:
	// le constructeur
	DrawSetLineWidth(long linewidth);

	// le constructeur de recopie
	DrawSetLineWidth(const DrawSetLineWidth & source);

	// la fonction pour dessiner le composant
	void Draw(JFCDraw * pDraw, long firstpage, long lastpage);

	// la fonction de clonage
	DrawComponent * Clone() const;

	// le destructeur
	~DrawSetLineWidth();
protected:
	// le paramètre
	long m_LineWidth;
};
// fin du test sur la macro
#endif