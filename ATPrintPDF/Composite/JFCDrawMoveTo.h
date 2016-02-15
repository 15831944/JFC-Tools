//================================
// fichier: JFCDrawMoveTo.h
//
// date: 02/09/2001
// auteur: JB
//================================

// on teste si la macro qui identifie le fichier est d�j� d�finie
#ifndef _JFCDRAWMOVETO_H_

// on d�finit une macro pour identifier le fichier
#define _JFCDRAWMOVETO_H_

// on inclut les fichiers n�cessaires
#include "JFCDrawComponent.h"


class DrawMoveTo : public DrawComponent
{
public:
	// le constructeur
	DrawMoveTo(long posx, long posy);

	// le constructeur de recopie
	DrawMoveTo(const DrawMoveTo & source);

	// la fonction pour dessiner ce composant
	void Draw(JFCDraw * pDraw, long firstpage, long lastpage);

	// la fonction de clonage
	DrawComponent * Clone() const;

	// le destructeur
	~DrawMoveTo();

protected:
	// les positions
	long m_PosX;
	long m_PosY;
};
// fin du test sur la macro
#endif
