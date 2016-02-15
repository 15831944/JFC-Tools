//============================
// fichier: JFCDecimilToUnit.cpp
//
// date: 29/08/2001
// auteur: JB
//============================

// on inclus les fichiers nécessaires
#include "stdafx.h"
#include "JFCDecimilToUnit.h"

//===========================================================================
// le constructeur:
//===========================================================================
DecimilToUnit::DecimilToUnit(long widthunit, long heightunit, long widthmm, long heightmm, long zoom, long offsetX = 0, long offsetY = 0)
{
	// on fixe les paramètres
	m_Scale   = zoom;
	m_OffsetX = offsetX;
	m_OffsetY = offsetY;

	// on récupère les paramètres liés à windows
	m_MMWidth  = widthmm;
	m_MMHeight = heightmm;
	m_UnitWidth  = widthunit;
	m_UnitHeight = heightunit;
}

//===========================================================================
// le constructeur de recopie:
//===========================================================================
DecimilToUnit::DecimilToUnit(const DecimilToUnit & source)
{
	// on recopie les paramètres
	m_Scale = source.m_Scale;
	m_OffsetX = source.m_OffsetX;
	m_OffsetY = source.m_OffsetY;

	// on récupère les paramètres liés à windows
	m_MMWidth  = source.m_MMWidth;
	m_MMHeight = source.m_MMHeight;
	m_UnitWidth  = source.m_UnitWidth;
	m_UnitHeight = source.m_UnitHeight;
}

//===========================================================================
// l'opérateur d'égalité:
//===========================================================================
DecimilToUnit & DecimilToUnit::operator =(const DecimilToUnit & operande)
{
	// on recopie les paramètres
	m_Scale = operande.m_Scale;
	m_OffsetX = operande.m_OffsetX;
	m_OffsetY = operande.m_OffsetY;

	// on renvoie la référence
	return (*this);
}

//===========================================================================
// les fonctions pour modifier les paramètres:
//===========================================================================
void DecimilToUnit::SetScale(long scaleratio)
{
	// on modifie le paramètre de zoom
	m_Scale = scaleratio;
}

void DecimilToUnit::SetOffsetX(long offsetX)
{
	// on modifie le paramètres de translation
	m_OffsetX = offsetX;
}

void DecimilToUnit::SetOffsetY(long offsetY)
{
	// on modifie le paramètre de translation
	m_OffsetY = offsetY;
}

//===========================================================================
// les fonctions pour récupérer les coordonnées en pixel:
//===========================================================================
long DecimilToUnit::GetUnitPosX(long coordXmm)
{
	// on calcule la taille en pixels
	return((m_UnitWidth * ((coordXmm-m_OffsetX)*m_Scale/100) + (m_MMWidth/2))/(m_MMWidth));
	
}

long DecimilToUnit::GetUnitPosY(long coordYmm)
{
	// on renvoie la position en pixel
	return ((m_UnitHeight * ((coordYmm-m_OffsetY)*m_Scale/100) + (m_MMHeight/2))/(m_MMHeight));
}

//===========================================================================
// la fonction pour récupérer une dimension en pixels:
//===========================================================================
long DecimilToUnit::GetDimX(long dim)
{
	// on calcule la taille en pixels
	return((m_UnitWidth * ((dim)*m_Scale/100) + (m_MMWidth/2))/(m_MMWidth));
}

long DecimilToUnit::GetDimY(long dim)
{
	// on calcule la taille en pixels
	return((m_UnitHeight * ((dim)*m_Scale/100) + (m_MMHeight/2))/(m_MMHeight));
}


//===========================================================================
// le destructeur:
//===========================================================================
DecimilToUnit::~DecimilToUnit()
{
	// on ne fait rien
}
