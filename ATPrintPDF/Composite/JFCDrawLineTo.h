//============================
// fichier: JFCDrawLineTo.h
//
// date: 02/09/2001
// auteur: JB
//============================

// on teste si la macro qui identifie le fichier est d�j� d�finie
#ifndef _JFCDRAWLINETO_H_

// on d�finit une macro pour identifier le fichier
#define _JFCDRAWLINETO_H_

// on inclut les fichiers n�cessaires
#include "JFCDrawComponent.h"

class DrawLineTo : public DrawComponent
{
public:
	// le constructeur
	DrawLineTo(long posx, long posy);

	// le constructeur de recopie
	DrawLineTo(const DrawLineTo & source);

	// la fonction pour dessiner le composant
	void Draw(JFCDraw * pDraw, long firstpage, long lastpage);

	// la fonction de clonage
	DrawComponent * Clone() const;

	// le destructeur
	~DrawLineTo();
protected:
	// les param�tres de position
	long m_PosX;
	long m_PosY;
};
// fin du test sur la macro
#endif