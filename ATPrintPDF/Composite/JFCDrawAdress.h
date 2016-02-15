//=============================
// fichier: JFCDrawAdress.h
//
// date: 03/09/2001
// auteur: JB
//=============================

// on teste si la macro qui identifie le fichier est déjà définie
#ifndef _JFCDRAWADRESS_H_

// on définit une macro pour identifier le fichier
#define _JFCDRAWADRESS_H_

// on inclut les fichiers nécessaires
#include "JFCDrawFrame.h"

class DrawAdress : public DrawFrame
{
public:
	// le constructeur
	DrawAdress(const char * adresse);

	// le constructeur de recopie
	DrawAdress(const DrawAdress & source);

	// la fonction pour dessiner le composite
	virtual void Draw(JFCDraw * pDraw, long firstpage, long lastpage);

	// la fonction de clonage
	virtual DrawComponent * Clone() const;

	// le destructeur
	virtual ~DrawAdress();
protected:
	//
};
// fin du test sur la macro
#endif