//===============================
// fichier: JFCDrawEndPage.cpp
// 
// date: 02/09/2001
// auteur: JB
//===============================

// on inclut les fichiers nécessaires
#include "stdafx.h"
#include "JFCDrawEndPage.h"

//=====================================================================
// le constructeur:
//=====================================================================
DrawEndPage::DrawEndPage()
{
	// on ne fait rien
}

//=====================================================================
// le constructeur de recopie:
//=====================================================================
DrawEndPage::DrawEndPage(const DrawEndPage & source)
{
	// on ne fait rien
}

//===============================================
// la fonction pour dessiner le composant:
//===============================================
void DrawEndPage::Draw(JFCDraw * pDraw, long firstpage, long lastpage)
{
	// on lance la commande en lui passant les paramètres
	pDraw->EndPage();
}

//=====================================================================
// la fonction de clonage:
//=====================================================================
DrawComponent * DrawEndPage::Clone() const
{
	// on renvoie l'adresse d'une copie
	return (new DrawEndPage(*this));
}

//=====================================================================
// le destructeur:
//=====================================================================
DrawEndPage::~DrawEndPage()
{
	// on ne fait rien
}
