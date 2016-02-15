// PageInfo.cpp: implementation of the CPageInfo class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "PageInfo.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//====================================================================
// le constructeur:
//====================================================================
CPageInfo::CPageInfo(long HorzRes, long VertRes, long HorzSize, long VertSize) : m_HorzRes(HorzRes), m_VertRes(VertRes), m_HorzSize(HorzSize*10), m_VertSize(VertSize*10)
{
	// on initialise les paramètres de la page
	m_PageHeight = 0;
	m_PageWidth = 0;
	m_NumPage = 0;
	m_Zoom = 0;

	// on initialise l'origine du dessin dans la page
	m_OrgX = 0;
	m_OrgY = 0;

	// on initialise le pointeur sur le document
	m_pDrawDoc = 0;
}

//====================================================================
// les fonctions pour fixer la taille de la page
//====================================================================
void CPageInfo::SetPageHeight(long pageheight)
{
	// on fixe le paramètre de hauteur de la page
	m_PageHeight = pageheight;
}

void CPageInfo::SetPageWidth(long pagewidth)
{
	// on fixe le paramètre de largeur de page
	m_PageWidth = pagewidth;
}

//====================================================================
// les fonctions pour gérer la page en cours:
//====================================================================
void CPageInfo::SetCurrentPage(long numpage)
{
	// on fixe la page courante
	m_NumPage = numpage;
}

long CPageInfo::GetCurrentPage()
{
	// on récupère la page courante
	return (m_NumPage);
}

long CPageInfo::GetNbPage()
{
	// on teste le document
	if ((*m_pDrawDoc) != 0) return ((*m_pDrawDoc)->GetNbPage());
	else return (1);
}
//====================================================================
// les fonctions pour gérer le zoom:
//====================================================================
void CPageInfo::SetZoom(long zoom)
{
	// on fixe le paramètre du zoom
	m_Zoom = zoom;
}

long CPageInfo::GetZoom()
{
	// on renvoie le zoom
	return (m_Zoom);
}

//====================================================================
// les fonctions pour récupérer la taille de la page:
//====================================================================
long CPageInfo::GetSizeX_mm()
{
	// on renvoie la largeur
	//return (m_PageWidth);//*m_Zoom/100);
	return (m_PageWidth*m_Zoom/100);
}

long CPageInfo::GetSizeX_px()
{
	// on renvoie la largeur en pixels
	return ((m_PageWidth*m_Zoom*m_HorzRes)/(m_HorzSize*100));
	//return ((m_PageWidth*m_HorzRes)/(m_HorzSize));
}

long CPageInfo::GetSizeY_mm()
{
	// on renvoie la hauteur
	//return (m_PageHeight);//*m_Zoom/100);
	return (m_PageHeight*m_Zoom/100);
}

long CPageInfo::GetSizeY_px()
{
	// on renvoie la hauteur en pixels
	return ((m_PageHeight*m_VertRes*m_Zoom)/(m_VertSize*100));
	//return ((m_PageHeight*m_VertRes)/(m_VertSize));
}

//====================================================================
// les fonctions pour gérer l'origine de la zone de page à dessiner:
//====================================================================
long CPageInfo::GetOrgX_mm()
{
	// on renvoie la position de l'origine dans la page (en mm)
	return (m_OrgX);
}

long CPageInfo::GetOrgX_px()
{
	// on renvoie la position de l'origine dans la page(en pixels)
	return (m_OrgX*m_HorzRes/m_HorzSize);
}

long CPageInfo::GetOrgY_mm()
{
	// on renvoie la position de l'origine dans la page(en mm)
	return (m_OrgY);
}

long CPageInfo::GetOrgY_px()
{
	// on renvoie la position de l'origine dans la page (en pixels)
	return (m_OrgY*m_VertRes/m_VertSize);
}

void CPageInfo::SetOrgX_mm(long orgX)
{
	// on fixe le paramètre
	m_OrgX = orgX;
}

void CPageInfo::SetOrgX_px(long orgX)
{
	// on fixe le paramètre
	m_OrgX = (orgX*m_HorzSize/m_HorzRes)*100/m_Zoom;
}

void CPageInfo::SetOrgY_mm(long orgY)
{
	// on fixe le paramètre
	m_OrgY = orgY;
}

void CPageInfo::SetOrgY_px(long orgY)
{
	// on fixe le paramètre
	m_OrgY = (orgY*m_VertSize/m_VertRes)*100/m_Zoom;
}

//====================================================================
// les fonctions pour récupérer les marges:
//====================================================================
void CPageInfo::GetMarges_px(long & margeTop, long & margeBottom, long & margeLeft, long & margeRight)
{
	
}

void CPageInfo::GetMarges_mm(long & margeTop, long & margeBottom, long & margeLeft, long & margeRight)
{
	
}

//====================================================================
// la fonction pour récupérer le pointeur sur le document:
//====================================================================
DrawDocument * CPageInfo::GetDrawDocument()
{
	// on teste si la variable existe
	if (m_pDrawDoc == 0) return (0);

	// on renvoie le pointeur sur le document
	return (*m_pDrawDoc);
}

void CPageInfo::SetDrawDocument(DrawDocument ** pDrawDoc)
{
	// on initialise le pointeur sur le document
	m_pDrawDoc = pDrawDoc;
}

//====================================================================
// le destructeur:
//====================================================================
CPageInfo::~CPageInfo()
{

}
