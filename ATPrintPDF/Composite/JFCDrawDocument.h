//=============================
// fichier: JFCDrawDocument.h
//
// date: 02/09/2001
// auteur: JB
//=============================

// on teste si la macro qui identifie le fichier est déjà définie
#ifndef _JFCDRAWDOCUMENT_H_

// on définit une macro pour identifier le fichier
#define _JFCDRAWDOCUMENT_H_

// on inclut les fichiers nécessaires
#include "JFCDrawComposite.h"

class DrawDocument : public DrawComposite
{
public:
	// le constructeur
	DrawDocument();

	// le constructeur de recopie
	DrawDocument(const DrawDocument & source);

	// la fonction pour dessiner le composite
	virtual void Draw(JFCDraw * pDraw, long firstpage, long lastpage);

	// la fonction de clonage
	virtual DrawComponent * Clone() const;

	// les fonctions pour fixer les dimensions de la page
	void SetPageHeight(long pageheight);
	void SetPageWidth(long pagewidth);

	// les fonctions pour récupérer les dimensions de la page
	long GetPageHeight();
	long GetPageWidth();

	// la fonction pour fixer le nombre de pages
	long GetNbPage();
	void SetNbPage(long nbpage);

	// le destructeur
	virtual ~DrawDocument();
protected:
	// le nombre de page
	long m_NbPage;

	// la hauteur de page
	long m_PageHeight;
	// la largeur de page
	long m_PageWidth;
};
// fin du test sur la macro
#endif


