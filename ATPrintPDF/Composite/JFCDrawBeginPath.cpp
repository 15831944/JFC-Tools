//=====================================
// fichier: JFCDrawBeginPath.cpp
//
// date: 02/09/2001
// auteur: JB
//=====================================

// on inclut les fichiers nécessaires
#include "stdafx.h"
#include "JFCDrawBeginPath.h"

//======================================================================
// le constructeur:
//======================================================================
DrawBeginPath::DrawBeginPath()
{
	// on ne fait rien
}

//======================================================================
// le constructeur de recopie:
//======================================================================
DrawBeginPath::DrawBeginPath(const DrawBeginPath & source)
{
	// on ne fait rien
}

//===============================================
// la fonction pour dessiner le composant:
//===============================================
void DrawBeginPath::Draw(JFCDraw * pDraw, long firstpage, long lastpage)
{
	// on lance la commande en lui passant les paramètres
	pDraw->BeginPath();
}

//=====================================================================
// la fonction de clonage:
//=====================================================================
DrawComponent * DrawBeginPath::Clone() const
{
	// on renvoie l'adresse d'une copie
	return (new DrawBeginPath(*this));
}

//======================================================================
// le destructeur:
//======================================================================
DrawBeginPath::~DrawBeginPath()
{
	// on ne fait rien
}
