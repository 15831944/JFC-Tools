//==========================
// fichier: JFCDraw.cpp
//
// date: 26/07/2001
// auteur: JB
//==========================

// on inclus les fichiers nécessaires
#include "stdafx.h"
#include "JFCDraw.h"

long JFCDraw::m_CodePage = 1252;

//==============================================================================
// le constructeur:
//==============================================================================
JFCDraw::JFCDraw(long orgX, long orgY)
{
	// on initialise le curseur
	m_CurseurX = 0;
	m_CurseurY = 0;

	// on initialise les variables de contrôle
	m_PageStarted = false;
	m_NoPage = 0;
	m_PosXMin = 0;
	m_PosXMax = 0;
	m_PosYMin = 0;
	m_PosYMax = 0;

	// on fixe l'origine
	m_OrgX = orgX;
	m_OrgY = orgY;
}


void JFCDraw::SetCodePage(long codePage)
{
	m_CodePage = codePage;
}

//==============================================================================
// le destructeur:
//==============================================================================
JFCDraw::~JFCDraw()
{
	// on ne fait rien
}
