//===========================
// fichier: JFCDrawText.h
//
// date: 02/09/2001
// auteur: JB
//===========================

// on teste si la macro qui identifie le fichier est déjà définie
#ifndef _JFCDRAWTEXT_H_

// on définit une macro pour identifier le fichier
#define _JFCDRAWTEXT_H_

// on inclut les fichiers nécessaires
#include "JFCDrawComponent.h"

class DrawTexte : public DrawComponent
{
public:
	// le constructeur
	DrawTexte(const char * text, Align HAlign, Align VAlign);

	// le constructeur de recopie
	DrawTexte(const DrawTexte & source);

	// la fonction pour dessiner le composant
	void Draw(JFCDraw * pDraw, long firstpage, long lastpage);

	// la fonction de clonage
	DrawComponent * Clone() const;

	// le destructeur
	~DrawTexte();
protected:
	// le texte
	char * m_Text;

	// l'alignement horizontal
	Align m_HAlign;

	// l'alignement vertical
	Align m_VAlign;
};
// fin du test sur la macro
#endif