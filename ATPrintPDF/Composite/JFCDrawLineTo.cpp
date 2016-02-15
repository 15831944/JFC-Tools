//==============================
// fichier: JFCDrawLineTo.cpp
//
// date: 02/09/2001
// auteur: JB
//==============================

// on inclut les fichiers nécessaires
#include "stdafx.h"
#include "JFCDrawLineTo.h"


//===============================================
// le constructeur:
//===============================================
DrawLineTo::DrawLineTo(long posx, long posy)
{
	// on initialise les paramètres
	m_PosX = posx;
	m_PosY = posy;
}

//===============================================
// le constructeur de recopie:
//===============================================
DrawLineTo::DrawLineTo(const DrawLineTo & source)
{
	// on initialise les paramètres
	m_PosX = source.m_PosX;
	m_PosY = source.m_PosY;
}

//===============================================
// la fonction pour dessiner le composant:
//===============================================
void DrawLineTo::Draw(JFCDraw * pDraw, long firstpage, long lastpage)
{
	// on lance la commande en lui passant les paramètres
	pDraw->LineTo(m_PosX, m_PosY);
}

//=====================================================================
// la fonction de clonage:
//=====================================================================
DrawComponent * DrawLineTo::Clone() const
{
	// on renvoie l'adresse d'une copie
	return (new DrawLineTo(*this));
}

//===============================================
// le destructeur:
//===============================================
DrawLineTo::~DrawLineTo()
{
	// on ne fait rien
}