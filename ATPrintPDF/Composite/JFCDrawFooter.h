//===========================
// fichier: JFCDrawFooter.h
//
// date: 03/09/2001
// auteur: JB
//===========================

// on teste si la macro qui identifie le fichier est d�j� d�finie
#ifndef _JFCDRAWFOOTER_H_

// on d�finit une macro pour identifier le fichier
#define _JFCDRAWFOOTER_H_

// on inclut les fichiers n�cessaires
#include "JFCDrawComposite.h"

class DrawFooter : public DrawComposite
{
public:
	// le constructeur
	DrawFooter();

	// le constructeur de recopie
	DrawFooter(const DrawFooter & source);

	// la fonction pour dessiner le header
	virtual void Draw(JFCDraw * pDraw, long firstpage, long lastpage);

	// la fonction de clonage
	virtual DrawComponent * Clone() const;

	// le destructeur
	virtual ~DrawFooter();
protected:

};
// fin du test sur la macro
#endif