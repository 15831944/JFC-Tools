/*
//	JFC Informatique et M�dia
//	2004
//
//	Auteur:
//	J�r�my DEVILERS
//
//
//	Classe:
//	JATPOffre
//
//	Descriptif:
//	Objet offre, contient les s�lections de l'utilisateur
//	Plateau de titre, cible, r�f�rence affinit� et terrain
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

	// Op�rateur de substitution
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

		// on renvoie notre r�f�rence
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
	// les fonctions pour s�rialiser l'�l�ment
	JVoid Send(JStream & Stream);
	JVoid Recv(JStream & Stream);
	*/

	// les fonctions pour s�rialiser l'�l�ment
	JVoid Send(JStream & Stream);
	JVoid Recv(JStream & Stream, JMap <JInt32,const IItem *> &MapCibleOffre);
	
	// la fonction pour permuter l'�l�ment
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
	// Vraisemblablement � modifier pour s�lection segment
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
	// Chargement �l�ments source en cours pour une appli
	JVoid			LoadParamSource(JString AbrNomAppli);

	// Sauvegarde �l�ments source en cours pour une appli
	JVoid			SaveParamSource(JString AbrNomAppli);

	// 1
	// List de Cibles
	JList<JUnt32>	m_TBLIdItems;

	// 2
	// Cible d'Affinit�
	JUnt32			m_IdItemAffinite;
	
	/*
	// en attente remplacera //1... to //2...
	// List des cibles s�lectionn�es dans l'offre
	JList <const CItemConstruit *> m_TBLIdItems;

	// Cible affinit� s�lectionn�e dans l'offre
	const CItemConstruit *			m_IdItemAffinite;
	*/

	//Ces tables sont l� � des fins d'information lors de l'ouverture d'un fichier pour pouvoir afficher les lbl des titres ou d'items supprim�s
	//Table de libell�s de supports
	JMap<JUnt32, JLabel> m_mapLblSupport;

	//Table de libell�s d'item
	JMap<JUnt32, JLabel> m_mapLblItem;

	// Map des autorisations des cibles sur les terrains s�lectionn�s
	JMap <JUnt32, JMap <JUnt32, JBool> > m_MapInterdictionsItems;

	// Renvoi si la cible est autoris�e
	JBool CibleAutorisee(JUnt32 IdCible, JUnt32 IdTerrain);

	// Reconstruit le map des items interdits sur les terrains
	JVoid ConstructionItemsInterdits();
};
