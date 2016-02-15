// ARMLLogo.cpp: implementation of the CLogo class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "./../ATPrintPDF.h"
#include "ARMLLogo.h"
#include "./../composite/JFCDrawPicture.h"
#include "./../composite/JFCDrawMoveTo.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//=================================================================================
// le constructeur:
//=================================================================================
CLogo::CLogo()
{
	// on initialise les paramètres
	m_LogoPath = "";
	m_StartPosX = 0;
	m_EndPosX = 0;
	m_StartPosY = 0;
	m_EndPosY = 0;
}

//=================================================================================
// le constructeur de recopie:
//=================================================================================
CLogo::CLogo(const CLogo & source)
{
	// on recopie les parmètres
	m_LogoPath = source.m_LogoPath;
	m_StartPosX = source.m_StartPosX;
	m_EndPosX = source.m_EndPosX;
	m_StartPosY = source.m_StartPosY;
	m_EndPosY = source.m_EndPosY;
}

//=================================================================================
// l'operateur d'égalité:
//=================================================================================
CLogo & CLogo::operator = (const CLogo & operande)
{
	// on recopie les parmètres
	m_LogoPath = operande.m_LogoPath;
	m_LogoPath = operande.m_LogoPath;
	m_StartPosX = operande.m_StartPosX;
	m_EndPosX = operande.m_EndPosX;
	m_StartPosY = operande.m_StartPosY;
	m_EndPosY = operande.m_EndPosY;
	
	// on renvoie la référence sur l'élément
	return (*this);
}

//=================================================================================
// la fonction pour fixer le chemin du logo
//=================================================================================
void CLogo::SetPath(const char * logopath)
{
	// on fixe le paramètre du chemin
	m_LogoPath = logopath;
}

//=================================================================================
// les fonctions pour fixer les coordonnées
//=================================================================================
void CLogo::SetStartPosX(long startposX)
{
	// la fonction pour fixer le nombre de lignes
	m_StartPosX = startposX;
}

void CLogo::SetEndPosX(long endposX)
{
	// on fixe le numéro de la ligne de départ
	m_EndPosX = endposX;
}

void CLogo::SetStartPosY(long startposY)
{
	// la fonction pour fixer le nombre de lignes
	m_StartPosY = startposY;
}

void CLogo::SetEndPosY(long endposY)
{
	// on fixe le numéro de la ligne de départ
	m_EndPosY = endposY;
}

//=================================================================================
// la fonction pour créer le logo:
//=================================================================================
void CLogo::CreateLogo(DrawComposite * composite)
{
	// on crée la commande de déplacement du curseur
	if (m_LogoPath=="")return;
	composite->AddChild(DrawMoveTo(m_StartPosX, m_StartPosY));

	// on crée la commande de dessin
	composite->AddChild(DrawPicture(m_LogoPath, (m_EndPosX - m_StartPosX), (m_EndPosY - m_StartPosY)));
}

//=================================================================================
// le destructeur:
//=================================================================================
CLogo::~CLogo()
{
	// on ne fait rien
}
