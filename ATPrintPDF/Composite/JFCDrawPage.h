//==========================
// fichier: JFCDrawPage.h
//
// date: 08/09/2001
// auteur: JB
//==========================

// on teste si la macro qui identifie le fichier est d�j� d�finie
#ifndef _JFCDRAWPAGE_H_

// on d�finit une macro pour identifier le fichier
#define _JFCDRAWPAGE_H_

// on inclut les fichiers n�cessaires
#include "JFCDrawComposite.h"

class DrawPage : public DrawComposite
{
public:
	// le constructeur
	DrawPage();

	// le constructeur de recopie
	DrawPage(const DrawPage & source);

	// la fonction pour dessiner la page
	void Draw(JFCDraw * pDraw, long firstpage, long lastpage);

	// la fonction de clonage
	DrawComponent * Clone() const;

	// le destructeur
	~DrawPage();

protected:
};
// fin du test sur la macro
#endif