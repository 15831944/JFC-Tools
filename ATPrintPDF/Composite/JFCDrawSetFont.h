//=============================
// fichier: JFCDrawSetFont.h 
//
// date: 08/09/2001
// auteur: JB
//=============================

// on teste si la macro qui identifie le fichier est déjà définie
#ifndef _JFCDRAWSETFONT_H_

// on définit une macro pour identifier le fichier
#define _JFCDRAWSETFONT_H_

// on inclut les fichiers nécessaires
#include "JFCDrawComponent.h"

class DrawSetFont : public DrawComponent
{
public:
	// le constructeur
	//DrawSetFont(FontManager font);
	DrawSetFont(long font);

	// le constructeur de recopie
	DrawSetFont(const DrawSetFont & source);

	// la fonction pour dessiner le composant
	void Draw(JFCDraw * pDraw, long firstpage, long lastpage);

	// la fonction de clonage
	DrawComponent * Clone() const;

	// le destructeur
	~DrawSetFont();
protected:
	// l'identifiant de la fonte
	// FontManager m_Font;
	// l'identifiant de l'indice de la fonte
	long m_Font;
};
// fin du test sur la macro
#endif