//=============================
// fichier: JFCDrawText.cpp
// 
// date: 02/09/2001
// auteur: JB
//=============================

// on inclut les fichiers nécessaires
#include "stdafx.h"
#include "JFCDrawText.h"

//===============================================================
// le constructeur:
//===============================================================
DrawTexte::DrawTexte(const char * text, Align HAlign, Align VAlign)
{
	// on initialise les paramètres d'alignement
	m_HAlign = HAlign;
	m_VAlign = VAlign;
	
	// on récupère la longueur de la chaine
	long length = 0; while(text[length]!=0) length+=1;
	
	// on alloue la chaine
	m_Text = new char[length+1];

	// on recopie les caractères
	for (long i = 0; i < length;i++) m_Text[i] = text[i];

	// on fixe le dernier caractère
	m_Text[length] = 0;
}

//===============================================================
// le constructeur de recopie:
//===============================================================
DrawTexte::DrawTexte(const DrawTexte & source)
{
	// on recopie les paramètres d'alignement
	m_HAlign = source.m_HAlign;
	m_VAlign = source.m_VAlign;
	
	// on récupère la longueur de la chaine
	long length = 0; while(source.m_Text[length]!=0) length+=1;
	
	// on alloue la chaine
	m_Text = new char[length+1];

	// on recopie les caractères
	for (long i = 0; i < length;i++) m_Text[i] = source.m_Text[i];

	// on fixe le dernier caractère
	m_Text[length] = 0;
}

//===============================================================
// la fonction pour dessiner le composant:
//===============================================================
void DrawTexte::Draw(JFCDraw * pDraw, long firstpage, long lastpage)
{
	// on lance la commande en lui passant les paramètres
	pDraw->Text(m_Text, m_HAlign, m_VAlign);
}

//=====================================================================
// la fonction de clonage:
//=====================================================================
DrawComponent * DrawTexte::Clone() const
{
	// on renvoie l'adresse d'une copie
	return (new DrawTexte(*this));
}

//===============================================================
// le destructeur:
//===============================================================
DrawTexte::~DrawTexte()
{
	// on libère le texte
	delete (m_Text);
}