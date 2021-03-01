/*
//	JFC Informatique et Média
//	2004
//
//	Auteur:
//	Jérémy DEVILERS
//
//
//	Classe:
//	JATPOffre
//
//	Descriptif:
//	Objet offre, contient les sélections de l'utilisateur
//	Plateau de titre, cible, référence affinité et terrain
*/

#pragma once

#include "JLib.h"

// classe info familles
class AFX_EXT_CLASS CInfoFamille
{
	public:
 	// Constructor
 	CInfoFamille()
 	{
		// Init Nom famille
		m_NomFamille = "";

		// Init Liste des supports composant cette famille
		m_LstIdSupport.Reset();
 	}

	// Opérateur de substitution
	CInfoFamille & operator =(const CInfoFamille & rhs)
	{
		// Nom de la famille
		m_NomFamille = rhs.m_NomFamille;

		// Liste des composants
		m_LstIdSupport.Reset();
		for (rhs.m_LstIdSupport.MoveFirst(); rhs.m_LstIdSupport.IsValid(); rhs.m_LstIdSupport.MoveNext())  
		{
			m_LstIdSupport.AddTail() = rhs.m_LstIdSupport.GetItem();  
		}

		// on renvoie notre référence
		return (*this);
	}
 
	// Nom Famille
 	JString  m_NomFamille;
	
	// Liste des supports composant cette famille
	JList <JUnt32> m_LstIdSupport;
};

class AFX_EXT_CLASS JATPOffre
{
public:
	JATPOffre(void);
	virtual ~JATPOffre(void);

	enum CHANGES
	{
		SRC_AUDIENCE	=	0x0001,
		TER_AUDIENCE	=	0x0004,
		TER_CIBLAGE		=	0x0008,
		TYP_POIDS		=	0x000F,
		SEG_AUDIENCE	=	0x0010,
		SUPPORTS		=	0x0040,
		CIBLES			=	0x0080,
		CIBLE_AFFINITE	=	0x00F0,
		FORMATS			=	0x0100,
		GRANDEURS		=	0x0200
	};

	/* Avant modif correspondance items construits
	// les fonctions pour sérialiser l'élément
	JVoid Send(JStream & Stream);
	JVoid Recv(JStream & Stream);
	*/

	// les fonctions pour sérialiser l'élément
	JVoid Send(JStream & Stream);
	JVoid Recv(JStream & Stream, JMap <JInt32,const IItem *> &MapCibleOffre);
	
	// la fonction pour permuter l'élément
	JVoid Swap(JATPOffre & Source);

	//la fonction de recopie
	JATPOffre & operator =(const JATPOffre & Source);

	// Changes
	JUnt32			m_Changes;

	// Source d'Audience
	JUnt32			m_IdSourceAudience;

	// Terrains d'Audiences
	JList<JUnt32>	m_TBLIdTerrainsAudience;

	// Liste de Segments d'Audience
	// Vraisemblablement à modifier pour sélection segment
	JList<JUnt32>	m_TBLIdSegmentsAudience;

	// Type de Poids
	JUnt32			m_IdTypePoids;

	// List de Formats
	JList<JUnt32>	m_TBLIdFormats;

	// List de Supports
	JList<JUnt32>	m_TBLIdSupports;

	// Map des familles
	JMap<JUnt32, CInfoFamille> m_MapSelFamilles;

	// Terrain de Ciblage
	JUnt32			m_IdTerrainCiblage;

	/////////////////////////////// Traitement partie Source de l'offre courante /////////////////////////
	// Chargement éléments source en cours pour une appli
	JVoid			LoadParamSource(JString AbrNomAppli);

	// Sauvegarde éléments source en cours pour une appli
	JVoid			SaveParamSource(JString AbrNomAppli);

	// 1
	// List de Cibles
	JList<JUnt32>	m_TBLIdItems;

	// 2
	// Cible d'Affinité
	JUnt32			m_IdItemAffinite;
	
	/*
	// en attente remplacera //1... to //2...
	// List des cibles sélectionnées dans l'offre
	JList <const CItemConstruit *> m_TBLIdItems;

	// Cible affinité sélectionnée dans l'offre
	const CItemConstruit *			m_IdItemAffinite;
	*/

	//Ces tables sont là à des fins d'information lors de l'ouverture d'un fichier pour pouvoir afficher les lbl des titres ou d'items supprimés
	//Table de libellés de supports
	JMap<JUnt32, JLabel> m_mapLblSupport;

	//Table de libellés d'item
	JMap<JUnt32, JLabel> m_mapLblItem;

	// Map des autorisations des cibles sur les terrains sélectionnés
	JMap <JUnt32, JMap <JUnt32, JBool> > m_MapInterdictionsItems;

	// Renvoi si la cible est autorisée
	JBool CibleAutorisee(JUnt32 IdCible, JUnt32 IdTerrain);

	// Reconstruit le map des items interdits sur les terrains
	JVoid ConstructionItemsInterdits();
};
