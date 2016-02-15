//============================
// fichier : JFCDrawLogo.h
//
// date: 03/09/2001
// auteur: JB
//============================

// on teste si la macro qui identifie le fichier est déjà définie
#ifndef _JFCDRAWLOGO_H_

// on définit une macro pour identifier le fichier
#define _JFCDRAWLOGO_H_

// on inclut les fichiers nécessaires
#include "JFCDrawComposite.h"

class DrawLogo : public DrawComposite
{
public:
	// le constructeur
	DrawLogo(const char * logopath);

	// le constructeur de recopie
	DrawLogo(const DrawLogo & source);

	// la fonction pour dessiner un composant
	virtual void Draw(JFCDraw * pDraw, long firstpage, long lastpage);

	// la fonction de clonage
	virtual DrawComponent * Clone() const;

	// le destructeur
	virtual ~DrawLogo();
protected:
	// le chemin du logo
	char * m_LogoPath;
};
// fin du test sur la macro
#endif

