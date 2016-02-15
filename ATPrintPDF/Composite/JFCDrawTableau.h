//===========================
// fichier: JFCDrawTable.h
//
// date: 03/09/2001
// auteur: JB
//===========================

// on teste si la macro qui identifie le fichier est déjà définie
#ifndef _JFCDRAWTABLE_H_

// on définit une macro pour identifier le fichier
#define _JFCDRAWTABLE_H_

// on inclut les fichiers nécessaires
#include "Afxtempl.h"
#include "JFCDrawComposite.h"

class DrawTable : public DrawComposite
{
public:
public:
	// le constructeur
	DrawTable();

	// le constructeur de recopie
	DrawTable(const DrawTable & source);

	// la fonction pour dessiner le composite
	virtual void Draw(JFCDraw * pDraw, long firstpage, long lastpage);

	// la fonction de clonage
	virtual DrawComponent * Clone() const;

	// le destructeur
	virtual ~DrawTable();
};
// fin du test sur la macro
#endif