//===========================
// fichier: JFCDrawHeader.h
//
// date: 03/09/2001
// auteur: JB
//===========================

// on teste si la macro qui identifie le fichier est déjà définie
#ifndef _JFCDRAWHEADER_H_

// on définit une macro pour identifier le fichier
#define _JFCDRAWHEADER_H_

// on inclut les fichiers nécessaires
#include "JFCDrawComposite.h"

class DrawHeader : public DrawComposite
{
public:
	// le constructeur
	DrawHeader();

	// le constructeur de recopie
	DrawHeader(const DrawHeader & source);

	// la fonction pour dessiner le composite
	virtual void Draw(JFCDraw * pDraw, long firstpage, long lastpage);

	// la fonction de clonage
	virtual DrawComponent * Clone() const;

	// le destructeur
	virtual ~DrawHeader();
protected:
	//

};
// fin du test sur la macro
#endif