//
// Fichier: JATPDocument.h
// Auteur:  Alain CHAMBARD
// Auteur:  J�r�my DEVILERS
// Date:    13/04/2004
// Gestion document s�lection Atelier Presse

#pragma once
// on v�rifie que le fichier n'a pas d�j� �t� inclus
#ifndef JATP_DOCUMENT_H

// on d�finit la macro pour �viter les inclusions multiples
#define JATP_DOCUMENT_H

// on inclut les d�finitions n�cessaires
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
	
	// la fonction pour tester l'�l�ment
	virtual JBool IsValid() const = 0;
	
	// les fonctions pour s�rialiser l'�l�ment
	virtual JVoid Send(JStream & Stream)      = 0;
	virtual JVoid Recv(JStream & Stream) 	  = 0;

protected:
	// la fonction pour synchroniser
	virtual JVoid SynchroSupports()  = 0;
	
	// la fonction pour pr�parer le contenu du document
	virtual BOOL OnPrepareContents() = 0;

	// la fonction pour mettre � jour le document
	virtual VOID OnUpdate(const JFCUpdate & Message) = 0;
	
	// la fonction pour mettre � jour le plateau document
	virtual JVoid UpdatePlateau() = 0; 	

private:
	///////////////////////// Chargement des �l�ments principaux ///////////////////////
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

	////////////////////////  Chargement des s�lections ////////////////////////////////
	//
	// Source d'Audience Courante (Id ou liste d'Id)
	virtual JUnt32 GetIdSrcAudienceSel();
	virtual void SetIdSrcAudienceSel(JUnt32 Id);

	// Source de Ciblage Courante (Id ou liste d'Id)	
	virtual JUnt32 GetIdSrcCiblageSel();
	virtual void SetIdSrcCiblageSel(JUnt32 Id);

	// Terrain Ciblage Selectionn� (Id ou liste d'Id)
	virtual JUnt32 GetIdTerrainCiblageSel();
	virtual void SetIdTerrainCiblageSel(JUnt32 Id);

	// Segment d'Audience Selectionn� (Id ou liste d'Id)
	virtual JUnt32 GetIdSegmentAudienceSel();
	virtual void SetIdSegmentAudienceSel(JUnt32 Id);

	// Segment de Ciblage Selectionn� (Id ou liste d'Id)
	virtual JUnt32 GetIdSegmentCibleSel();
	virtual void SetIdSegmentCibleSel(JUnt32 Id);

	// Le filtre d'audience (Id ou liste d'Id)
	virtual JUnt32 GetIdFiltreAudienceSel();
	virtual void SetIdFiltreAudienceSel(JUnt32 Id);

	// List des terrains audience s�lectionn�s (Map ou liste de Maps)
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

	// Terrain Ciblage Selectionn�
	JUnt32						m_IdTerrainCiblageSel;

	// Segment d'Audience Selectionn� (Segment Population)
	JUnt32						m_IdSegmentAudienceSel;

	// Segment de Ciblage Selectionn�
	JUnt32						m_IdSegmentCibleSel;

	// Le filtre d'audience
	JUnt32						m_IdFiltreAudienceSel;

	// List des terrains audience s�lectionn�s
	JMap<JUnt32,JUnt32>			m_MapIdTerrainsAudienceSel;

	// Liste de Formats Actifs  (Les formats pour insertion)
	JMap<JUnt32,JUnt32>			m_MapIdFormatsSel;
	*/

	// ***************************** L'OFFRE EN COURS ****************************
	//
	JATPOffre					m_Offre;			

	// *****************************  OPTIONS TARIF MANUEL ***********************
	// Tarifs manuels en cours (clef d'acc�s = id support + id format)
	JMap <CKeyTarifManuel, JFlt32>	m_MapTarifManuel; 
	
	// ******************************** OPTION CALCUL ****************************
	//
	// Prise en compte reprises en main
	JBool						m_bRepriseEnMain;
	// Privil�gier audience LNM pour magazines
	JBool						m_bLNMMag;
	// Privil�gier audience LDP pour quotidiens
	JBool						m_bLDPQtd;
	// Privil�gier audience LDP 30 j pour les bimestriels
	JBool						m_bLDP30J;
	// Privil�gier avec cumul famille (comme si c'�tait un plan de supports // par d�faut false = calcul moyenne)
	JBool						m_CumulFamille;		
	
	// ******************************* ETAT AFFICHAGE *****************************
	// l'�tat d'affichage mode familles
	JBool						m_AFFModeFamilles;	

	// l'�tat d'affichage mode supports
	JBool						m_AFFModeSupports;	

	// l'�tat d'affichage des filtres d'audiences
	JBool						m_AFFFiltreAudience;	

	// l'�tat d'affichage des options moteur
	JBool						m_AFFOptionsMoteur;		

	// **************************  LISTE DES ELMTS DE TRAVAIL *********************
	//
	// Map des Items Construits by ID
	JMap<JUnt32 , IItem * >		m_MAPpItems;

	// Map des Items Pond�r�s by ID
	JMap<JUnt32 , IItem * >		m_MAPpItemsPonderes;

	// Affichage
	// Nb d�cimales co�t
	JUnt32 m_NbDecCout;
	
};


// fin de l'inclusion conditionnelle
#endif

