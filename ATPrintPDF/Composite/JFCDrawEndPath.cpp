//================================
// fichier: JFCDrawEndPath.cpp
//
// date: 02/09/2001
// auteur: JB
//================================

// on inclut les fichiers nécessaires
#include "stdafx.h"
#include "JFCDrawEndPath.h"

//=======================================================================
// le constructeur:
//=======================================================================
DrawEndPath::DrawEndPath()
{
	// on ne fait rien
}

//=======================================================================
// le constructeur:
//=======================================================================
DrawEndPath::DrawEndPath(const DrawEndPath & source)
{
	// on ne fait rien
}

//===============================================
// la fonction pour dessiner le composant:
//===============================================
void DrawEndPath::Draw(JFCDraw * pDraw, long firstpage, long lastpage)
{
	// on lance la commande en lui passant les paramètres
	pDraw->EndPath();
}

//=====================================================================
// la fonction de clonage:
//=====================================================================
DrawComponent * DrawEndPath::Clone() const
{
	// on renvoie l'adresse d'une copie
	return (new DrawEndPath(*this));
}

//=======================================================================
// le destructeur:
//=======================================================================
DrawEndPath::~DrawEndPath()
{
	// on ne fait rien
}
