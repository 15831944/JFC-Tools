//============================
// fichier: JFCDrawPage.cpp
//
// date: 08/09/2001
// auteur: JB
//============================

// on inclut les fichiers nécessaires
#include "stdafx.h"
#include "JFCDrawPage.h"

//=====================================================
// le constructeur:
//=====================================================
DrawPage::DrawPage()
{
	// on ne fait rien
}

//=====================================================
// le constructeur de recopie:
//=====================================================
DrawPage::DrawPage(const DrawPage & source) : DrawComposite(source)
{
	// on ne fait rien
}

//=====================================================
// la fonction pour dessiner un composant:
//=====================================================
void DrawPage::Draw(JFCDraw * pDraw, long firstpage, long lastpage)
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

//=====================================================
// la fonction de clonage:
//=====================================================
DrawComponent * DrawPage::Clone() const
{
	// on renvoie l'adresse d'une copie
	return (new DrawPage(*this));
}

//=====================================================
// le destructeur:
//=====================================================
DrawPage::~DrawPage()
{
	// on ne fait rien
}