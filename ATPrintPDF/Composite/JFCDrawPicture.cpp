//===============================
// fichier: JFCDrawPicture.cpp
//
// date: 02/09/2001
// auteur: JB
//===============================

// on inclut les fichiers nécessaires
#include "stdafx.h"
#include "JFCDrawPicture.h"

//===============================================================
// le constructeur:
//===============================================================
DrawPicture::DrawPicture(const char * filename, long WidthMax, long HeightMax)
{
	// on récupère la longueur de la chaine
	long length = 0; while(filename[length]!=0) length+=1;
	
	// on alloue la chaine
	m_FileName = new char[length+1];

	// on recopie les caractères
	for (long i = 0; i < length;i++) m_FileName[i] = filename[i];

	// on fixe le dernier caractère
	m_FileName[length] = 0;

	// on met à jour les paramètres
	m_WidthMax = WidthMax;
	m_HeightMax = HeightMax;
}

//===============================================================
// le constructeur de recopie:
//===============================================================
DrawPicture::DrawPicture(const DrawPicture & source)
{
	// on récupère la longueur de la chaine
	long length = 0; while(source.m_FileName[length]!=0) length+=1;
	
	// on alloue la chaine
	m_FileName = new char[length+1];

	// on recopie les caractères
	for (long i = 0; i < length;i++) m_FileName[i] = source.m_FileName[i];

	// on fixe le dernier caractère
	m_FileName[length] = 0;

	// on met à jour les paramètres
	m_WidthMax = source.m_WidthMax;
	m_HeightMax = source.m_HeightMax;
}

//===============================================
// la fonction pour dessiner le composant:
//===============================================
void DrawPicture::Draw(JFCDraw * pDraw, long firstpage, long lastpage)
{
	// on lance la commande en lui passant les paramètres
	pDraw->SetPicture(m_FileName, m_WidthMax, m_HeightMax);
}

//=====================================================================
// la fonction de clonage:
//=====================================================================
DrawComponent * DrawPicture::Clone() const
{
	// on renvoie l'adresse d'une copie
	return (new DrawPicture(*this));
}

//==============================================================
// le destructeur:
//==============================================================
DrawPicture::~DrawPicture()
{
	// on libère le nom du fichier
	delete m_FileName;
}