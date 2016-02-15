//===============================
// fichier: JFCDrawPicture.h
//
// date: 02/09/2001
// auteur: JB
//===============================

// on teste si la macro qui identifie le fichier est  déjà définie
#ifndef _JFCDRAWPICTURE_H_

// on définit une macro pour identifier le fichier
#define _JFCDRAWPICTURE_H_

// on inclut les fichiers nécessaires
#include "JFCDrawComponent.h"

class DrawPicture : public DrawComponent
{
public:
	// le constructeur
	DrawPicture(const char * filename, long WidthMax, long HeightMax);

	// le constructeur de recopie
	DrawPicture(const DrawPicture & source);

	// la fonction pour dessiner le composant
	void Draw(JFCDraw * pDraw, long firstpage, long lastpage);

	// la fonction de clonage
	DrawComponent * Clone() const;

	// le destructeur
	~DrawPicture();
protected:
	// le chemin du fichier
	char * m_FileName;

	// la largeur maximum
	long m_WidthMax;

	// la hauteur maximum
	long m_HeightMax;
};
// fin du test sur la macro
#endif