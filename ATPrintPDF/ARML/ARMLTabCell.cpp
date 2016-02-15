// ARMLTabCell.cpp: implementation of the CTabCell class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "./../ATPrintPDF.h"
#include "ARMLTabCell.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//========================================================================
// le constructeur:
//========================================================================
CTabCell::CTabCell()
{
	// on initialise les paramètres
	m_PosY = 0;
	m_Height = 0;
	m_PosX = 0;
	m_Width = 0;
	m_StyleTD = 0;
	m_StyleTG = 0;
	m_BkColor = 0;
	m_Justification = 'C';
	m_Bold = false;
	m_Texte = "";
}

//========================================================================
// le constructeur de recopie:
//========================================================================
CTabCell::CTabCell(const CTabCell & source)
{
	// on initialise les paramètres
	m_PosY          = source.m_PosY;
	m_Height        = source.m_Height;
	m_PosX          = source.m_PosX ;
	m_Width         = source.m_Width;
	m_StyleTD       = source.m_StyleTD;
	m_StyleTG       = source.m_StyleTG;
	m_BkColor       = source.m_BkColor;
	m_Justification = source.m_Justification;
	m_Bold          = source.m_Bold;
	m_Texte         = source.m_Texte;
}

//========================================================================
// l'opérateur d'affectation:
//========================================================================
CTabCell & CTabCell::operator=(const CTabCell & operande)
{
	// on initialise les paramètres
	m_PosY          = operande.m_PosY;
	m_Height        = operande.m_Height;
	m_PosX          = operande.m_PosX ;
	m_Width         = operande.m_Width;
	m_StyleTD       = operande.m_StyleTD;
	m_StyleTG       = operande.m_StyleTG;
	m_BkColor       = operande.m_BkColor;
	m_Justification = operande.m_Justification;
	m_Bold          = operande.m_Bold;
	m_Texte         = operande.m_Texte;

	// on renvoie la référence sur l'élément
	return (*this);
}

//========================================================================
// les fonctions pour modifier les paramètres:
//========================================================================
void CTabCell::SetPosY(long posy)
{
	// on modifie le paramètre
	m_PosY = posy;
}

void CTabCell::SetHeight(long height)
{
	// on modifie le paramètre
	m_Height = height;
}

void CTabCell::SetPosX(long posx)
{
	// on modifie le paramètre
	m_PosX = posx;
}

void CTabCell::SetWidth(long width)
{
	// on modifie le paramètre
	m_Width = width;
}

void CTabCell::SetStyleTD(long styleTD)
{
	// on modifie le paramètre
	m_StyleTD = styleTD;
}

void CTabCell::SetStyleTG(long styleTG)
{
	// on modifie le paramètre
	m_StyleTG = styleTG;
}

void CTabCell::SetBkColor(long bkcolor)
{
	// on modifie le paramètre
	m_BkColor = bkcolor;
}

void CTabCell::SetJustification(char justification)
{
	// on modifie le paramètre
	m_Justification = justification;
}

void CTabCell::SetBold(bool bold)
{
	// on modifie le paramètre
	m_Bold = bold;
}

void CTabCell::SetText(CString texte)
{
	// on modifie le paramètre
	m_Texte = texte;
}


//========================================================================
// le destructeur:
//========================================================================
CTabCell::~CTabCell()
{

}
