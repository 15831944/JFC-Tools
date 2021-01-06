//
// Fichier: JATPDocument.h
// Auteur:  Alain CHAMBARD
// Auteur:  Jérémy DEVILERS
// Date:    13/04/2004
// Gestion document sélection Atelier Presse

#pragma once
// on vérifie que le fichier n'a pas déjà été inclus
#ifndef JATP_DOCUMENT_H

// on définit la macro pour éviter les inclusions multiples
#define JATP_DOCUMENT_H

// on inclut les définitions nécessaires
#include "JATPOffre.h"
#include "JFCDocument.h"
#include "ItemsConstruits.h"
#include "MoteurPresse.h"

#include "KeyTarifManuel.h"
#include "Tarifs.h"

class AFX_EXT_CLASS JATPDocument : public JFCDocument
{
public:
	// le constructeur
	JATPDocument(void);
	
	// le destructeur
	~JATPDocument(void);
	
	// la fonction pour tester l'élément
	virtual JBool IsValid() const = 0;
	
	// les fonctions pour sérialiser l'élément
	virtual JVoid Send(JStream & Stream)      = 0;
	virtual JVoid Recv(JStream & Stream) 	  = 0;

protected:
	// la fonction pour synchroniser
	virtual JVoid SynchroSupports()  = 0;
	
	// la fonction pour préparer le contenu du document
	virtual BOOL OnPrepareContents() = 0;

	// la fonction pour mettre à jour le document
	virtual VOID OnUpdate(const JFCUpdate & Message) = 0;
	
	// la fonction pour mettre à jour le plateau document
	virtual JVoid UpdatePlateau() = 0; 	

private:
	///////////////////////// Chargement des éléments principaux ///////////////////////
	//
	// Chargement des Supports
	virtual JBool OnLoadSupports()	= 0;
	
	// Chargement des terrains
	virtual JBool OnLoadTerrains()	= 0;
	
	// Chargement des items cibles
	virtual JBool OnLoadItems()		= 0;

	// Chargement des formats
	virtual JVoid OnLoadFormats()	= 0;

public:

	////////////////////////  Chargement des sélections ////////////////////////////////
	//
	// Source d'Audience Courante (Id ou liste d'Id)
	virtual JUnt32 GetIdSrcAudienceSel();
	virtual void SetIdSrcAudienceSel(JUnt32 Id);

	// Source de Ciblage Courante (Id ou liste d'Id)	
	virtual JUnt32 GetIdSrcCiblageSel();
	virtual void SetIdSrcCiblageSel(JUnt32 Id);

	// Terrain Ciblage Selectionné (Id ou liste d'Id)
	virtual JUnt32 GetIdTerrainCiblageSel();
	virtual void SetIdTerrainCiblageSel(JUnt32 Id);

	// Segment d'Audience Selectionné (Id ou liste d'Id)
	virtual JUnt32 GetIdSegmentAudienceSel();
	virtual void SetIdSegmentAudienceSel(JUnt32 Id);

	// Segment de Ciblage Selectionné (Id ou liste d'Id)
	virtual JUnt32 GetIdSegmentCibleSel();
	virtual void SetIdSegmentCibleSel(JUnt32 Id);

	// Le filtre d'audience (Id ou liste d'Id)
	virtual JUnt32 GetIdFiltreAudienceSel();
	virtual void SetIdFiltreAudienceSel(JUnt32 Id);

	// List des terrains audience sélectionnés (Map ou liste de Maps)
	virtual JMap<JUnt32,JUnt32> * GetMapIdTerrainsAudienceSel();
	virtual void SetMapIdTerrainsAudienceSel(JMap<JUnt32,JUnt32> &MapId);

	// Liste de Formats Actifs  (Map ou liste de Maps)
	virtual JMap<JUnt32,JUnt32> * GetMapIdFormatsSel();
	virtual void SetMapIdFormatsSel(JMap<JUnt32,JUnt32> &MapId);

	// Objet moteur
	CMoteurPresse				m_MoteurPresse;

////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////		DATA		////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////
public:

	//***************************** SELECTIONS ****************************
	//
	/*
	// Source d'Audience Courante
	JUnt32						m_IdSrcAudienceSel;

	// Source de Ciblage Courante
	JUnt32						m_IdSrcCiblageSel;

	// Terrain Ciblage Selectionné
	JUnt32						m_IdTerrainCiblageSel;

	// Segment d'Audience Selectionné (Segment Population)
	JUnt32						m_IdSegmentAudienceSel;

	// Segment de Ciblage Selectionné
	JUnt32						m_IdSegmentCibleSel;

	// Le filtre d'audience
	JUnt32						m_IdFiltreAudienceSel;

	// List des terrains audience sélectionnés
	JMap<JUnt32,JUnt32>			m_MapIdTerrainsAudienceSel;

	// Liste de Formats Actifs  (Les formats pour insertion)
	JMap<JUnt32,JUnt32>			m_MapIdFormatsSel;
	*/

	// ***************************** L'OFFRE EN COURS ****************************
	//
	JATPOffre					m_Offre;			

	// *****************************  OPTIONS TARIF MANUEL ***********************
	// Tarifs manuels en cours (clef d'accés = id support + id format)
	JMap <CKeyTarifManuel, JFlt32>	m_MapTarifManuel; 
	
	// ******************************** OPTION CALCUL ****************************
	//
	// Prise en compte reprises en main
	JBool						m_bRepriseEnMain;
	// Privilégier audience LNM pour magazines
	JBool						m_bLNMMag;
	// Privilégier audience LDP pour quotidiens
	JBool						m_bLDPQtd;
	// Privilégier audience LDP 30 j pour les bimestriels
	JBool						m_bLDP30J;
	// Privilégier avec cumul famille (comme si c'était un plan de supports // par défaut false = calcul moyenne)
	JBool						m_CumulFamille;		
	
	// ******************************* ETAT AFFICHAGE *****************************
	// l'état d'affichage mode familles
	JBool						m_AFFModeFamilles;	

	// l'état d'affichage mode supports
	JBool						m_AFFModeSupports;	

	// l'état d'affichage des filtres d'audiences
	JBool						m_AFFFiltreAudience;	

	// l'état d'affichage des options moteur
	JBool						m_AFFOptionsMoteur;		

	// **************************  LISTE DES ELMTS DE TRAVAIL *********************
	//
	// Map des Items Construits by ID
	JMap<JUnt32 , IItem * >		m_MAPpItems;

	// Map des Items Pondérés by ID
	JMap<JUnt32 , IItem * >		m_MAPpItemsPonderes;

	// Affichage
	// Nb décimales coût
	JUnt32 m_NbDecCout;
	
};


// fin de l'inclusion conditionnelle
#endif

