//===============================
// fichier: PoliceManager.cpp
//
// date: 12/09/2001
// auteur: JB
//===============================

// on inclut le fichier nécessaires
#include "stdafx.h"
#include "PoliceManager.h"

//========================================================
// le mode d'incorporation des fontes
int FontManager::m_Embedding = 0;

//========================================================
// les différentes fontes
/*
const FontManager FontManager::COURRIER_JFC_10     ("Courier New", 7, false, false);
const FontManager FontManager::COURRIER_JFC_10_BOLD("Courier New", 7, true,  false);
const FontManager FontManager::COURRIER_JFC_11     ("Courier New", 8, false, false);
const FontManager FontManager::COURRIER_JFC_11_BOLD("Courier New", 8, true,  false);
*/
const FontManager FontManager::COURRIER_JFC_10     ("Arial", 7, false, false);
const FontManager FontManager::COURRIER_JFC_10_BOLD("Arial", 7, true,  false);
const FontManager FontManager::COURRIER_JFC_11     ("Arial", 8, false, false);
const FontManager FontManager::COURRIER_JFC_11_BOLD("Arial", 8, true,  false);

//========================================================
// les constructeurs:
//========================================================

FontManager::FontManager(const char * fontname,long taille, bool bold, bool italic)
{
	// on initialise les paramètres
	m_FontName = fontname;
	m_Size = taille;
	m_Bold = bold;
	m_Italic = italic;
}

//========================================================
// le constructeur de recopie:
//========================================================
FontManager::FontManager(const FontManager & source)
{
	// on recopie les paramètres
	m_FontName = source.m_FontName;
	m_Size = source.m_Size;
	m_Bold = source.m_Bold;
	m_Italic = source.m_Italic;

}

//========================================================
// l'opérateur d'égalité:
//========================================================
FontManager & FontManager::operator =(const FontManager & operande)
{
	// on recopie les paramètres
	m_FontName = operande.m_FontName;
	m_Size = operande.m_Size;
	m_Bold = operande.m_Bold;
	m_Italic = operande.m_Italic;

	// on renvoie la référence sur l'objet
	return (*this);
}

//=====================================================
// les fonctions pour manipuler le mode d'incorporation
//=====================================================

void FontManager::SetEmbedding(int mode)
{
	if (mode > 0) FontManager::m_Embedding = 1;
	else          FontManager::m_Embedding = 0;
}

int FontManager::GetEmbedding()
{
	return (FontManager::m_Embedding);
}

//=========================================================
// les fonctions pour récupérer les paramètres d'une fonte:
//=========================================================
const char * FontManager::GetFontName() const
{
	// on renvoie le nom de la fonte
	return (m_FontName);
}

long FontManager::GetFontSize() const
{
	return (m_Size);
}

bool FontManager::IsBold() const
{
	return (m_Bold);
}

bool FontManager::IsItalic() const
{
	return (m_Italic);
}

// ///////////////////////////////////////////////////////////////////////////
// fonction d'ajout de fontes au lancement de l'application
// ///////////////////////////////////////////////////////////////////////////

const FontManager & FontManager::GetFont(long indice)
{
	switch (indice)
	{
	case 0:
		return (COURRIER_JFC_11);
	case 1:
		return (COURRIER_JFC_11_BOLD);
	case 2 :
		return (COURRIER_JFC_10);
	case 3:
		return (COURRIER_JFC_10_BOLD);
	}
	return (COURRIER_JFC_11);
}

//=========================================================
// le destructeur:
//=========================================================
FontManager::~FontManager()
{
	// rien a faire
}
