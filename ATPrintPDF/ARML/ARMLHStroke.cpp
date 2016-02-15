// ARMLHStroke.cpp: implementation of the CHStroke class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "./../ATPrintPDF.h"
#include "ARMLHStroke.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//=====================================================================
// le constructeur:
//=====================================================================
CHStroke::CHStroke()
{
	// on initialise les paramètres
	this->m_StartPosX = 0;
	this->m_EndPosX = 0;
	this->m_PosY = 0;
	this->m_StrokeStyle = 0;
}

//=====================================================================
// le constructeur de recopie:
//=====================================================================
CHStroke::CHStroke(const CHStroke & source)
{
	// on recopie les éléments
	m_StartPosX = source.m_StartPosX;
	m_EndPosX   = source.m_EndPosX;
	m_PosY      = source.m_PosY;
	m_StrokeStyle = source.m_StrokeStyle;
}

//=====================================================================
// l'opérateur d'égalité:
//=====================================================================
CHStroke & CHStroke::operator =(const CHStroke & operande)
{
	// on recopie les éléments
	m_StartPosX = operande.m_StartPosX;
	m_EndPosX   = operande.m_EndPosX;
	m_PosY      = operande.m_PosY;
	m_StrokeStyle = operande.m_StrokeStyle;

	// on renvoie la référence
	return (*this);
}

//=====================================================================
// les fonctions pour modifier les paramètres:
//=====================================================================
void CHStroke::SetStartPosX(long startposx)
{
	// on modifie le paramètre
	this->m_StartPosX = startposx;
}

void CHStroke::SetEndPosX(long endposx)
{
	// on modifie le paramètre
	this->m_EndPosX = endposx;
}

void CHStroke::SetPosY(long posy)
{
	// on modifie le paramètre
	this->m_PosY = posy;
}

void CHStroke::SetStrokeStyle(long strokestyle)
{
	// on modifie le paramètre
	this->m_StrokeStyle = strokestyle;
}

//=====================================================================
// le destructeur:
//=====================================================================
CHStroke::~CHStroke()
{

}
