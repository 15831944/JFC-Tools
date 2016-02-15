//============================
// fichier: JFCDrawLogo.cpp
//
// date: 04/09/2001
// auteur: JB
//============================

// on inclut les fichiers nécessaires
#include "stdafx.h"
#include "JFCDrawLogo.h"

//==========================================================
// le constructeur:
//==========================================================
DrawLogo::DrawLogo(const char * logopath)
{
	// on récupère la longeur du texte
	long length = 0; const char * txt = logopath; while(*txt++) length+=1;

	// on alloue la chaine
	m_LogoPath = new char[length+1];

	// on recopie les caractères
	for (long i = 0; i < length;i++) m_LogoPath[i] = logopath[i];

	// on fixe le dernier caractère
	m_LogoPath[length] = 0;
}

//==========================================================
// le constructeur de recopie:
//==========================================================
DrawLogo::DrawLogo(const DrawLogo & source) : DrawComposite(source)
{
	// on récupère la longueur de la chaine
	long length = 0; while(source.m_LogoPath[length]!=0) length+=1;
	
	// on alloue la chaine
	m_LogoPath = new char[length+1];

	// on recopie les caractères
	for (long i = 0; i < length;i++) m_LogoPath[i] = source.m_LogoPath[i];

	// on fixe le dernier caractère
	m_LogoPath[length] = 0;
}

//==========================================================
// la fonction de dessin:
//==========================================================
void DrawLogo::Draw(JFCDraw * pDraw, long firstpage, long lastpage)
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
DrawComponent * DrawLogo::Clone() const
{
	// on renvoie l'adresse d'une copie
	return (new DrawLogo(*this));
}

//==========================================================
// le constructeur:
//==========================================================
DrawLogo::~DrawLogo()
{
	// on libère le chemin du logo
	delete m_LogoPath;
}
