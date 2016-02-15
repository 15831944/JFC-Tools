//=============================
// fichier: JFCDrawTable.cpp
// 
// date: 04/09/2001
// auteur: JB
//=============================

// on inclut les fichiers nécessaires
#include "stdafx.h"
#include "JFCDrawTableau.h"


//===================================================================================
// le constructeur:
//===================================================================================
DrawTable::DrawTable()
{
	// on 

}

//===================================================================================
// le constructeur de recopie:
//===================================================================================
DrawTable::DrawTable(const DrawTable & source) : DrawComposite(source)
{
	// on recopie les paramètres du tableau

}

//===================================================================================
// la fonction pour dessiner le tableau:
//===================================================================================
void DrawTable::Draw(JFCDraw * pDraw, long firstpage, long lastpage)
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

//===================================================================================
// la fonction de clonage:
//===================================================================================
DrawComponent * DrawTable::Clone() const
{
	// on renvoie l'adresse d'une copie
	return(new DrawTable(*this));
}

//===================================================================================
// le destructeur:
//===================================================================================
DrawTable::~DrawTable()
{
	// on ne fait rien
}