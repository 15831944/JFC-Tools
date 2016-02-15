//===============================
// fichier: JFCDrawAdress.cpp
//
// date: 02/09/2001
// auteur: JB
//===============================

// on inclut les fichiers nécessaires
#include "stdafx.h"
#include "JFCDrawAdress.h"

//=============================================
// le constructeur:
//=============================================
DrawAdress::DrawAdress(const char * adresse)
{
	// on fixe les paramètres de trait
	m_WidthTH = m_WidthTD = m_WidthTB = m_WidthTG = 0;
}

//==================================================================
// le constructeur de recopie:
//==================================================================
DrawAdress::DrawAdress(const DrawAdress & source) : DrawFrame(source)
{
	// on copie la liste de commandes
}

//==================================================================
// la fonction de dessin:
//==================================================================
void DrawAdress::Draw(JFCDraw * pDraw, long firstpage, long lastpage)
{
	// on boucle sur les enfants
	DrawComponent * pChild = this->GetChild();

	while(pChild != 0)
	{
		// on lance la commande de dessin de l'enfant
		pChild->Draw(pDraw, firstpage, lastpage);

		// on passe a l'enfant suivant
		pChild = pChild->GetNext();
	}
}

//==================================================================
// la fonction de clonage:
//==================================================================
DrawComponent * DrawAdress::Clone() const
{
	// on renvoie l'adresse d'une copie
	return (new DrawAdress(*this));
}

//==================================================================
// le destructeur:
//==================================================================
DrawAdress::~DrawAdress()
{
	// on ne fait rien
}