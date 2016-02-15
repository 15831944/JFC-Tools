//============================
// fichier: JFCDrawEndPath.h
// 
// date: 02/09/2001
// auteur: JB
//============================

// on teste si la macro qui identifie le fichier est d�j� d�finie
#ifndef _JFCDRAWENDPATH_H_

// on d�finit une macro pour identifier le fichier
#define _JFCDRAWENDPATH_H_

// on inclut les fichiers n�cessaires
#include "JFCDrawComponent.h"

class DrawEndPath : public DrawComponent
{
public:
	// le constructeur
	DrawEndPath();

	// le constructeur de recopie
	DrawEndPath(const DrawEndPath & source);

	// la fonction pour dessiner le composant
	void Draw(JFCDraw * pDraw, long firstpage, long lastpage);

	// la fonction de clonage
	DrawComponent * Clone() const;

	// le destructeur
	~DrawEndPath();
protected:
	// pas d'argument pour cette commande
};
// fin du test sur la macro
#endif