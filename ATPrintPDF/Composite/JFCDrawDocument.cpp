//=================================
// fichier: JFCDrawDocument.cpp
//
// date: 08/09/2001
// auteur: JB
//=================================

// on inclut les fichiers nécessaires
#include "stdafx.h"
#include "JFCDrawDocument.h"

//=========================================================
// le constructeur:
//=========================================================
DrawDocument::DrawDocument()
{
	// on fixe le nombre de pages
	m_NbPage = 1;

	// on fixe la hauteur de ligne
	this->m_PageWidth = 0;
	// on fixe la largeur de colonne
	this->m_PageHeight = 0;
}

//=========================================================
// le constructeur de recopie:
//=========================================================
DrawDocument::DrawDocument(const DrawDocument & source) : DrawComposite(source)
{

}


//========================================================
// la fonction pour dessiner un composant:
//========================================================
void DrawDocument::Draw(JFCDraw * pDraw, long firstpage, long lastpage)
{
	// on boucle sur les enfants
	DrawComponent * pChild = this->GetChild();

	long numpage = 1;

	while(pChild != 0)
	{
		// on lance la commande de dessin de l'enfant
		if (numpage >= firstpage && numpage <= lastpage)
		{
			pChild->Draw(pDraw, firstpage, lastpage);
		}
		// on passe a l'enfant suivant
		pChild = pChild->GetNext();
		
		// on incrémente le numéro de page
		numpage+=1;
	}
}

//========================================================
// la fonction de clonage:
//========================================================
DrawComponent * DrawDocument::Clone() const
{
	// on renvoie l'adresse d'une copie
	return (new DrawDocument(*this));
}

//========================================================
// les fonctions pour fixer les dimensions de la page:
//========================================================
void DrawDocument::SetPageHeight(long pageheight)
{
	// on renvoie la hauteur de ligne
	m_PageHeight = pageheight;
}

void DrawDocument::SetPageWidth(long pagewidth)
{
	// on renvoie la largeur de colonne
	m_PageWidth = pagewidth;
}

//========================================================
// les fonctions pour récupérer les dimensions de la page:
//========================================================
long DrawDocument::GetPageHeight()
{
	// on renvoie la hauteur de la page
	return (m_PageHeight);
}

long DrawDocument::GetPageWidth()
{
	// on renvoie la largeur de la page
	return (m_PageWidth);
}

//========================================================
// les fonctions pour gérer le nombre de pages:
//========================================================
void DrawDocument::SetNbPage(long nbpage)
{
	// on fixe le nombre de pages
	m_NbPage = nbpage;
}

long DrawDocument::GetNbPage()
{
	// on renvoie le nombre de page
	return (m_NbPage);
}

//========================================================
// le destructeur:
//========================================================
DrawDocument::~DrawDocument()
{
	// on ne fait rien
}