//==========================
// fichier: JFCDrawBody.h
// 
// date: 03/09/2001
// auteur: JB
//==========================

// on teste si la macro qui identifie le fichier est déjà définie
#ifndef _JFCDRAWBODY_H_

// on définit une macro pour identifier le fichier
#define _JFCDRAWBODY_H_

// on inclut les fichiers nécessaires
#include "JFCDrawComposite.h"

class DrawBody : public DrawComposite
{
public:
	// le constructeur
	DrawBody();

	// le constructeur de recopie
	DrawBody(const DrawBody & source);

	// la fonction pour dessiner le corps
	virtual void Draw(JFCDraw * pDraw, long firstpage, long lastpage);

	// la fonction de clonage
	virtual DrawComponent * Clone() const;

	// le destructeur
	virtual ~DrawBody();

protected:

};
// fin du test sur la macro
#endif