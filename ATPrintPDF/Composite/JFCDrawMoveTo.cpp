//==============================
// fichier: JFCDrawMoveTo.cpp
//
// date: 02/09/2001
// auteur: JB
//==============================

// on inclut les fichiers nécessaires
#include "stdafx.h"
#include "JFCDrawMoveTo.h"

//================================================
// le constructeur:
//================================================
DrawMoveTo::DrawMoveTo(long posx, long posy)
{
	// on initialise les paramètres
	m_PosX = posx;
	m_PosY = posy;
}

//================================================
// le constructeur de recopie:
//================================================
DrawMoveTo::DrawMoveTo(const DrawMoveTo & source)
{
	// on initialise les paramètres
	m_PosX = source.m_PosX;;
	m_PosY = source.m_PosY;
}

//================================================
// la fonction pour dessiner ce composant:
//================================================
void DrawMoveTo::Draw(JFCDraw * pDraw, long firstpage, long lastpage)
{
	// on lance la commande de dessin en lui passant les paramètres
	pDraw->MoveTo(m_PosX, m_PosY);
}

//=====================================================================
// la fonction de clonage:
//=====================================================================
DrawComponent * DrawMoveTo::Clone() const
{
	// on renvoie l'adresse d'une copie
	return (new DrawMoveTo(*this));
}


//================================================
// le destructeur:
//================================================
DrawMoveTo::~DrawMoveTo()
{
	// on ne fait rien
}