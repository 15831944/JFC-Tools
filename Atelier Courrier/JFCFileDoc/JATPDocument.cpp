//
// Fichier: JATPDocument.h
// Date:    13/04/2004
// Gestion document sélection Atelier Presse

#include "StdAfx.h"
#include "resource.h"
#include "JATPDocument.h"
#include "ATPDocCSSerializer.h"

JATPDocument::JATPDocument(void)
{
	// ******************************* ETAT AFFICHAGE *****************************
	// l'état affichage mode familles
	m_AFFModeFamilles			 = false;	

	// l'état affichage mode supports (mode par défaut)
	m_AFFModeSupports			 = true;	

	// l'état d'affichage des filtres d'audiences
	m_AFFFiltreAudience			= false;	

	// l'état d'affichage des options moteur
	m_AFFOptionsMoteur			= false;

	// **************************  LISTE DES ELMTS DE TRAVAIL *********************
	//
	// Map des Items Construits by ID
	m_MAPpItems.Reset();

	// Map des Items Pondérés Construits by ID
	m_MAPpItemsPonderes.Reset();

	// Nb décimales coût
	m_NbDecCout = AfxGetApp()->GetProfileInt("Options", "NbDecimaleCout", 2);

}

JATPDocument::~JATPDocument(void)
{
	// Nb décimales coût
	AfxGetApp()->WriteProfileInt("Options","NbDecimaleCout", m_NbDecCout);
}

///////////////////////////////////////  Chargement des sélections //////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////////////////////////////////
// Source d'Audience Courante
JUnt32 JATPDocument::GetIdSrcAudienceSel()
{
	throw JInternalError::GetInstance();
}

void JATPDocument::SetIdSrcAudienceSel(JUnt32 Id)
{
	throw JInternalError::GetInstance();
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////
// Source de Ciblage Courante
JUnt32 JATPDocument::GetIdSrcCiblageSel()
{
	throw JInternalError::GetInstance();
}

void JATPDocument::SetIdSrcCiblageSel(JUnt32 Id)
{
	throw JInternalError::GetInstance();
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////
// Terrain Ciblage Selectionné (Id ou liste d'Id)
JUnt32 JATPDocument::GetIdTerrainCiblageSel()
{
	throw JInternalError::GetInstance();
}

void JATPDocument::SetIdTerrainCiblageSel(JUnt32 Id)
{
	throw JInternalError::GetInstance();
}


/////////////////////////////////////////////////////////////////////////////////////////////////////////
// Segment d'Audience Selectionné (Id ou liste d'Id)
JUnt32 JATPDocument::GetIdSegmentAudienceSel()
{
	throw JInternalError::GetInstance();
}

void JATPDocument::SetIdSegmentAudienceSel(JUnt32 Id)
{
	throw JInternalError::GetInstance();
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////
// Segment de Ciblage Selectionné (Id ou liste d'Id)
JUnt32 JATPDocument::GetIdSegmentCibleSel()
{
	throw JInternalError::GetInstance();
}

void JATPDocument::SetIdSegmentCibleSel(JUnt32 Id)
{
	throw JInternalError::GetInstance();
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////
// Le filtre d'audience (Id ou liste d'Id)
JUnt32 JATPDocument::GetIdFiltreAudienceSel()
{
	throw JInternalError::GetInstance();
}

void JATPDocument::SetIdFiltreAudienceSel(JUnt32 Id)
{
	throw JInternalError::GetInstance();
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////
// List des terrains audience sélectionnés (Map ou liste de Maps)
JMap<JUnt32,JUnt32> * JATPDocument::GetMapIdTerrainsAudienceSel()
{
	throw JInternalError::GetInstance();
}

void JATPDocument::SetMapIdTerrainsAudienceSel(JMap<JUnt32,JUnt32> &MapId)
{
	throw JInternalError::GetInstance();
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////
// Liste de Formats Actifs  (Map ou liste de Maps)
JMap<JUnt32,JUnt32> * JATPDocument::GetMapIdFormatsSel()
{
	throw JInternalError::GetInstance();
}

void JATPDocument::SetMapIdFormatsSel(JMap<JUnt32,JUnt32> &MapId)
{
	throw JInternalError::GetInstance();
}