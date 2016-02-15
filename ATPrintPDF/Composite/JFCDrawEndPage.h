//============================
// fichier: JFCDrawEndPage.h
//
// date: 02/09/2001
// auteur: JB
//============================

// on teste si la macro qui identifie le fichier est déjà définie
#ifndef _JFCDRAWENDPAGE_H_

// on définit une macro pour identifier le fichier
#define _JFCDRAWENDPAGE_H_

// on inclut les fichiers nécessaires
#include "JFCDrawComponent.h"

class DrawEndPage : public DrawComponent
{
public:
	// le constructeur
	DrawEndPage();

	// le constructeur de recopie
	DrawEndPage(const DrawEndPage & source);

	// la fonction pour dessiner le composant
	void Draw(JFCDraw * pDraw, long firstpage, long lastpage);

	// la fonction de clonage
	DrawComponent * Clone() const;

	// le destructeur
	~DrawEndPage();
protected:
	// pas d'arguments pour cette commande
};
// fin du test sur la macro
#endif