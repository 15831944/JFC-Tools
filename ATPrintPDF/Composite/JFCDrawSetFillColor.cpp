//==================================
// fichier: JFCDrawSetFillColor.cpp
// 
// date: 02/09/2001
// auteur: JB
//==================================

// on inclut les fichiers n�cessaires
#include "stdafx.h"
#include "JFCDrawSetFillColor.h"

//===========================================================
// le constructeur:
//===========================================================
DrawSetFillColor::DrawSetFillColor(long fillcolor)
{
	// on fixe le param�tres
	m_FillColor = fillcolor;
}

//===========================================================
// le constructeur:
//===========================================================
DrawSetFillColor::DrawSetFillColor(const DrawSetFillColor & source)
{
	// on fixe le param�tres
	m_FillColor = source.m_FillColor;
}

//===============================================
// la fonction pour dessiner le composant:
//===============================================
void DrawSetFillColor::Draw(JFCDraw * pDraw, long firstpage, long lastpage)
{
	// on lance la commande en lui passant les param�tres
	pDraw->SetFillColor(m_FillColor);
}

//=====================================================================
// la fonction de clonage:
//=====================================================================
DrawComponent * DrawSetFillColor::Clone() const
{
	// on renvoie l'adresse d'une copie
	return (new DrawSetFillColor(*this));
}

//===========================================================
// le destructeur:
//===========================================================
DrawSetFillColor::~DrawSetFillColor()
{
	// on ne fait rien
}
