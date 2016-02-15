//=============================
// fichier: JFCDrawText.cpp
// 
// date: 02/09/2001
// auteur: JB
//=============================

// on inclut les fichiers n�cessaires
#include "stdafx.h"
#include "JFCDrawText.h"

//===============================================================
// le constructeur:
//===============================================================
DrawTexte::DrawTexte(const char * text, Align HAlign, Align VAlign)
{
	// on initialise les param�tres d'alignement
	m_HAlign = HAlign;
	m_VAlign = VAlign;
	
	// on r�cup�re la longueur de la chaine
	long length = 0; while(text[length]!=0) length+=1;
	
	// on alloue la chaine
	m_Text = new char[length+1];

	// on recopie les caract�res
	for (long i = 0; i < length;i++) m_Text[i] = text[i];

	// on fixe le dernier caract�re
	m_Text[length] = 0;
}

//===============================================================
// le constructeur de recopie:
//===============================================================
DrawTexte::DrawTexte(const DrawTexte & source)
{
	// on recopie les param�tres d'alignement
	m_HAlign = source.m_HAlign;
	m_VAlign = source.m_VAlign;
	
	// on r�cup�re la longueur de la chaine
	long length = 0; while(source.m_Text[length]!=0) length+=1;
	
	// on alloue la chaine
	m_Text = new char[length+1];

	// on recopie les caract�res
	for (long i = 0; i < length;i++) m_Text[i] = source.m_Text[i];

	// on fixe le dernier caract�re
	m_Text[length] = 0;
}

//===============================================================
// la fonction pour dessiner le composant:
//===============================================================
void DrawTexte::Draw(JFCDraw * pDraw, long firstpage, long lastpage)
{
	// on lance la commande en lui passant les param�tres
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
	// on lib�re le texte
	delete (m_Text);
}