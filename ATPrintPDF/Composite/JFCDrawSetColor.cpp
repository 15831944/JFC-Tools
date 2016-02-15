//==============================
// fichier: JFCDrawSetColor.cpp
//
// date: 02/09/2001
// auteur: JB
//==============================


// on inclut les fichiers nécessaires
#include "stdafx.h"
#include "JFCDrawSetColor.h"

//=========================================================
// le constructeur:
//=========================================================
DrawSetColor::DrawSetColor(long color)
{
	// on initialise le paramètre
	m_Color = color;
}

//=========================================================
// le constructeur de recopie:
//=========================================================
DrawSetColor::DrawSetColor(const DrawSetColor & source)
{
	// on initialise le paramètre
	m_Color = source.m_Color;
}

//===============================================
// la fonction pour dessiner le composant:
//===============================================
void DrawSetColor::Draw(JFCDraw * pDraw, long firstpage, long lastpage)
{
	// on lance la commande en lui passant les paramètres
	pDraw->SetColor(m_Color);
}

//=====================================================================
// la fonction de clonage:
//=====================================================================
DrawComponent * DrawSetColor::Clone() const
{
	// on renvoie l'adresse d'une copie
	return (new DrawSetColor(*this));
}

//=========================================================
// le destructeur:
//=========================================================
DrawSetColor::~DrawSetColor()
{
	// on ne fait rien
}
