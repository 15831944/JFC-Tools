//=====================================
// fichier: JFCDrawSetLineWidth.cpp
//
// date: 02/09/2001
// auteur: JB
//=====================================

// on inclut les fichiers nécessaires
#include "stdafx.h"
#include "JFCDrawSetLineWidth.h"

//======================================================
// le constructeur:
//======================================================
DrawSetLineWidth::DrawSetLineWidth(long linewidth)
{
	// on initialise le paramètre
	m_LineWidth = linewidth;
}

//======================================================
// le constructeur de recopie:
//======================================================
DrawSetLineWidth::DrawSetLineWidth(const DrawSetLineWidth & source)
{
	// on initialise le paramètre
	m_LineWidth = source.m_LineWidth;
}

//======================================================
// la fonction pour dessiner le composant:
//======================================================
void DrawSetLineWidth::Draw(JFCDraw * pDraw, long firstpage, long lastpage)
{
	// on lance la commande
	pDraw->SetLineWidth(m_LineWidth);
}

//=====================================================================
// la fonction de clonage:
//=====================================================================
DrawComponent * DrawSetLineWidth::Clone() const
{
	// on renvoie l'adresse d'une copie
	return (new DrawSetLineWidth(*this));
}

//======================================================
// le destructeur:
//======================================================
DrawSetLineWidth::~DrawSetLineWidth()
{
	// on ne fait rien
}