//=============================
// fichier: JFCDrawBody.cpp
//
// date: 04/09/2001
// auteur: JB
//=============================

// on inclut les fichiers nécessaires
#include "stdafx.h"
#include "JFCDrawBody.h"

//========================================================
// le constructeur:
//========================================================
DrawBody::DrawBody()
{
	// on ne fait rien
}

//========================================================
// le constructeur de recopie:
//========================================================
DrawBody::DrawBody(const DrawBody & source) : DrawComposite(source)
{
		
}

//========================================================
// la fonction pour dessiner un composant:
//========================================================
void DrawBody::Draw(JFCDraw * pDraw, long firstpage, long lastpage)
{
	// on ne dessine que les enfants désignés par les numéros de page
	// sachant que l'enfant d'un body doit etre une page
	DrawComponent * pChild = this->GetChild();

	// on se déplace sur la première page demandée
	for (long i = 1; i < firstpage; i++) pChild = pChild->GetNext();

	// on dessine les pages demandées
	for(long i = 0; i <= (lastpage-firstpage); i++)
	{
		// on dessine l'enfant
		pChild->Draw(pDraw, firstpage, lastpage);

		// on passe à l'enfant suivant
		pChild = pChild->GetNext();
	}
		
}

//========================================================
// la fonction de clonage:
//========================================================
DrawComponent * DrawBody::Clone() const
{
	// on renvoie l'adresse d'une copie
	return (new DrawBody(*this));
}

//========================================================
// le destructeur:
//========================================================
DrawBody::~DrawBody()
{
	// on ne fait rien
}