//==============================
// fichier: JFCDrawHeader.cpp
// 
// date: 04/09/2001
// auteur: JB
//==============================

// on inclut les fichiers nécessaires
#include "stdafx.h"
#include "JFCDrawHeader.h"

//==========================================================
// le constructeur:
//==========================================================
DrawHeader::DrawHeader()
{
	// on ne fait rien
}

//==========================================================
// le constructeur de recopie:
//==========================================================
DrawHeader::DrawHeader(const DrawHeader & source) : DrawComposite(source)
{
	// on recopie la liste source

}

//==========================================================
// la fonction pour dessiner les composants de ce composite:
//==========================================================
void DrawHeader::Draw(JFCDraw * pDraw, long firstpage, long lastpage)
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

//==========================================================
// la fonction de clonage:
//==========================================================
DrawComponent * DrawHeader::Clone() const
{
	// on renvoie l'adresse d'une copie
	return (new DrawHeader(*this));
}

//==========================================================
// le destructeur:
//==========================================================
DrawHeader::~DrawHeader()
{
	// on ne fait rien
}