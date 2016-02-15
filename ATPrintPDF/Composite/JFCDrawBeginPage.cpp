//================================
// fichier: JFCDrawBeginPage.cpp
//
// date: 02/09/2001
// auteur: JB
//================================

// on inclut les fichiers nécessaires
#include "stdafx.h"
#include "JFCDrawBeginPage.h"

//====================================================================
// le constructeur:
//====================================================================
DrawBeginPage::DrawBeginPage()
{
	// on ne fait rien
}

//====================================================================
// le constructeur de recopie :
//====================================================================
DrawBeginPage::DrawBeginPage(const DrawBeginPage & source)
{
	// on ne fait rien
}

//=====================================================================
// la fonction pour dessiner le composant:
//=====================================================================
void DrawBeginPage::Draw(JFCDraw * pDraw, long firstpage, long lastpage)
{
	// on lance la commande en lui passant les paramètres
	pDraw->StartPage();
}

//=====================================================================
// la fonction de clonage:
//=====================================================================
DrawComponent * DrawBeginPage::Clone() const
{
	// on renvoie l'adresse d'une copie
	return (new DrawBeginPage(*this));
}

//====================================================================
// le destructeur:
//====================================================================
DrawBeginPage::~DrawBeginPage()
{
	// on ne fait rien
}
