//==============================
// fichier: JFCDrawFooter.cpp
//
// date: 02/09/2001
// auteur: JB
//==============================

// on inclut les fichiers nécessaires
#include "stdafx.h"
#include "JFCDrawFooter.h"

//=======================================================
// le constructeur:
//=======================================================
DrawFooter::DrawFooter()
{
	// on ne fait rien
}

//=======================================================
// le constructeur de recopie
//=======================================================
DrawFooter::DrawFooter(const DrawFooter & source) : DrawComposite(source)
{
	// on recopie la liste

}

//=======================================================
// la fonction pour dessiner le composite:
//=======================================================
void DrawFooter::Draw(JFCDraw * pDraw, long firstpage, long lastpage)
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

//=======================================================
// la fonction pour cloner:
//=======================================================
DrawComponent * DrawFooter::Clone() const
{
	// on renvoie l'adresse d'une copie
	return (new DrawFooter(*this));
}

//=======================================================
// le destructeur:
//=======================================================
DrawFooter::~DrawFooter()
{
	// on ne fait rien
}