//===============================
// fichier: JFCDrawSetFont.cpp
//
// date: 08/09/2001
// auteur: JB
//===============================

// on inclut les fichiers nécessaires
#include "stdafx.h"
#include "JFCDrawSetFont.h"

//===================================================
// le constructeur:
//===================================================
//DrawSetFont::DrawSetFont(FontManager font)
DrawSetFont::DrawSetFont(long font)
{
	// on initialise le paramètre de la fonte
	m_Font = font;
}

//===================================================
// le constructeur de recopie:
//===================================================
DrawSetFont::DrawSetFont(const DrawSetFont & source)
{
	// on recopie les paramètres
	m_Font = source.m_Font;
}

//===================================================
// la fonction pour lancer la commande:
//===================================================
void DrawSetFont::Draw(JFCDraw * pDraw, long firstpage, long lastpage)
{
	// on lance la commande
	pDraw->SetFont(m_Font);
}

//===================================================
// la fonction de clonage:
//===================================================
DrawComponent * DrawSetFont::Clone() const
{
	// on renvoie l'adresse d'une copie
	return (new DrawSetFont(*this));
}

//===================================================
// le destructeur:
//===================================================
DrawSetFont::~DrawSetFont()
{
	// on ne fait rien
}