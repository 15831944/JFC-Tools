//
// Fichier: JATPDocChoixSupport.h
// Auteur:  Alain CHAMBARD
// Date:    06/04/2004
// Gestion document sélection Choix Support
//
#pragma once

// on vérifie que le fichier n'a pas déjà été inclus
#ifndef JATP_DOC_CHOIX_SUPPORT_H

// on définit la macro pour éviter les inclusions multiples
#define JATP_DOC_CHOIX_SUPPORT_H

#include "JATPDocument.h"
#include "JLib.h"
#include "JATPKEYPlateau.h"
#include "JATPOffre.h"
#include "Terrain.h"
// #include "MoteurPresse.h"

///////////////////////////////// POUR STOCKAGE CALCUL ECHELLE ///////////////////////////
// Classe pour la clé des résultats échelles
class AFX_EXT_CLASS CKeyResultEch
{
 public:
 	// Constructor
 	CKeyResultEch()
 	{
 		m_IdTerrain	 =	0;
 		m_IdCible	 =	0;
		m_IdCibleAff =  0;
		m_IdSegment  =  0;		
 		m_IdSupport	 =	0;
		m_IdMoteur	 =  0;
 	}

 	// les opérateurs pour comparer les éléments
	JBool 		operator <(const CKeyResultEch & rhs) const;
	JBool 		operator >(const CKeyResultEch & rhs) const;

 	JUnt32		m_IdTerrain; 	// 	Ident Terrain via liste terrains sélectionnés
 	JUnt32		m_IdCible;		//	Ident Cible via liste terrains sélectionnés
	JUnt32      m_IdCibleAff;	//  Ident cible affinité
	JUnt32		m_IdSegment;	//  Ident segment : combinaison des segments sur un long (32 positions possibles)
	JUnt32		m_IdSupport;	//	Ident Support via liste terrains sélectionnés
	JUnt32		m_IdMoteur;		//	Ident Moteur
 };

// classe résultats échelles
class AFX_EXT_CLASS CResultEch
{
	public:
 	// Constructor
 	CResultEch()
 	{
		m_VolLecture			= 0.0;
		m_AudienceLDP			= 0.0;
		m_AudienceProba			= 0.0;
		m_Audience				= 0.0;	
		m_Audience1Ins			= 0.0;
		m_Effectif				= 0.0;
		m_NbCas					= 0;
		m_VolAffinite			= 0.0;
		m_AudienceAffiniteLDP	= 0.0;
		m_AudienceAffiniteProba	= 0.0;
		m_AudienceAffinite		= 0.0;
		m_EffectifAffinite		= 0.0;
		m_ResultPartiel			= false;	// par défaut résultat complet	
 	}
 
	// Les différents résultats après calcul
 	JFlt64  m_VolLecture;
	JFlt64  m_AudienceLDP;
	JFlt64  m_AudienceProba;
	JFlt64  m_Audience;
	JFlt64  m_Audience1Ins;
	JFlt64  m_Effectif;
	JInt32  m_NbCas;
	JFlt64  m_VolAffinite;
	JFlt64  m_AudienceAffiniteLDP;
	JFlt64  m_AudienceAffiniteProba;
	JFlt64  m_AudienceAffinite;
	JFlt64  m_EffectifAffinite;
	JBool	m_ResultPartiel;				// indique si résultat partiel (servira pour familles)
};

///////////////////////////////// POUR STOCKAGE CALCUL CUMULATIVITE ///////////////////////////
// Classe pour la clé des résultats cumulativité
class AFX_EXT_CLASS CKeyResultCum
{
 public:
 	// Constructor
 	CKeyResultCum()
 	{
 		m_IdTerrain		=	0;
 		m_IdCible		=	0;
		m_IdSegment		=   0;		
 		m_IdSupport		=	0;
		m_IdMoteur		=	0;
		m_NbInsert		=   1; 
 	}

 	// les opérateurs pour comparer les éléments
	JBool 		operator <(const CKeyResultCum & rhs) const;
	JBool 		operator >(const CKeyResultCum & rhs) const;

 	JUnt32		m_IdTerrain; 	// 	Ident Terrain via liste terrains sélectionnés
 	JUnt32		m_IdCible;		//	Ident Cible via liste terrains sélectionnés
	JUnt32		m_IdSegment;	//  Ident segment : combinaison des segments sur un long (32 positions possibles)
	JUnt32		m_IdSupport;	//	Ident Support via liste terrains sélectionnés
	JUnt32		m_IdMoteur;		//	Ident Moteur
	JUnt32		m_NbInsert;		//  Nb insertions pour résultats
 };

// classe résultats cumulativité
class AFX_EXT_CLASS CResultCum
{
	public:
 	// Constructor
 	CResultCum()
 	{
		m_VolLecture			= 0.0;
		m_Audience				= 0.0;	
		m_AudProba1Ins			= 0.0;		
		m_Aud1Ins			    = 0.0;		
		m_Effectif				= 0.0;
		m_NbCas					= 0;
		m_PtEtalonnage			= 0;		//  Nb jours par insertion 
		m_ResultPartiel			= false;	// par défaut résultat complet	
 	}
 
	// Les différents résultats après calcul
 	JFlt64  m_VolLecture;
	JFlt64  m_Audience;
	JFlt64  m_AudProba1Ins;
	JFlt64  m_Aud1Ins;
	JFlt64  m_Effectif;
	JInt32  m_NbCas;
	JUnt32  m_PtEtalonnage;					//  Nb jours par insertion 
	JBool	m_ResultPartiel;				// indique si résultat partiel (servira pour familles)
};


///////////////////////////////// POUR STOCKAGE CALCUL MAPPING ///////////////////////////
// Classe pour la clé des résultats Mapping
class AFX_EXT_CLASS CKeyResultMap
{
 public:
 	// Constructor
 	CKeyResultMap()
 	{
 		m_IdTerrain		=	0;
 		m_IdCible		=	0;
		m_IdCibleAff	=	0;
		m_IdSegment		=   0;
 		m_IdSupport		=	0;
		m_IdMoteur		=	CMoteurPresse::PROBA;
 	}

 	// les opérateurs pour comparer les éléments
	JBool 		operator <(const CKeyResultMap & rhs) const;
	JBool 		operator >(const CKeyResultMap & rhs) const;

 	JUnt32		m_IdTerrain; 	// 	Ident Terrain via liste terrains sélectionnés
 	JUnt32		m_IdCible;		//	Ident Cible via liste terrains sélectionnés
	JUnt32      m_IdCibleAff;	//  Ident cible affinité
	JUnt32		m_IdSegment;	//  Ident segment : combinaison des segments sur un long (32 positions possibles)
	JUnt32		m_IdSupport;	//	Ident Support via liste terrains sélectionnés
	JUnt32		m_IdMoteur;		//	Ident Moteur
 };

// classe résultats mapping
class AFX_EXT_CLASS CResultMap
{
	public:
 	// Constructor
 	CResultMap()
 	{
		m_VolLecture			= 0.0;
		m_AudienceLDP			= 0.0;
		m_AudienceProba			= 0.0;
		m_Audience				= 0.0;	
		m_Audience1Ins			= 0.0;
		m_Effectif				= 0.0;
		m_NbCas					= 0;
		m_VolAffinite			= 0.0;
		m_AudienceAffiniteLDP	= 0.0;
		m_AudienceAffiniteProba	= 0.0;
		m_AudienceAffinite		= 0.0;
		m_EffectifAffinite		= 0.0;
		m_ResultPartiel			= false;	// par défaut résultat complet	
 	}
 
	// Les diffréents résultats après calcul
 	JFlt64  m_VolLecture;
	JFlt64  m_AudienceLDP;
	JFlt64  m_AudienceProba;
	JFlt64  m_Audience;
	JFlt64  m_Audience1Ins;
	JFlt64  m_Effectif;
	JInt32  m_NbCas;
	JFlt64  m_VolAffinite;
	JFlt64  m_AudienceAffiniteLDP;
	JFlt64  m_AudienceAffiniteProba;
	JFlt64  m_AudienceAffinite;
	JFlt64  m_EffectifAffinite;
	JBool	m_ResultPartiel;				// indique si résultat partiel (servira pour familles)
};

///////////////////////////////// POUR STOCKAGE INFO POSITIONS MAPPING ///////////////////////////
// Classe pour la clé des positions Mapping
class AFX_EXT_CLASS CKeyInfoMapping
{
 public:
 	// Constructor
 	CKeyInfoMapping()
 	{
		m_IdTerrain			= 0;	// Ident Terrain via liste terrains sélectionnés
 		m_IdCible			= 0;	// Ident Cible via liste cibles sélectionnés
		m_IdCibleAff		= 0;	// Ident cible affinité
		m_IdSegment			= 0;	//  Ident segment : combinaison des segments sur un long (32 positions possibles)
		m_IdSupport			= 0;	// Ident Support via liste supports sélectionnés
		m_IdFormat			= 0;	// Ident Format via liste formats sélectionnés
		m_IdMoteur			= CMoteurPresse::PROBA;	// Ident moteur
		m_NbInsertions		= 0;	// Nombre d'insertions	
		m_IdGrandeurAbs		= 0;	// Ident Grandeur abscisse mapping
		m_IdGrandeurOrd		= 0;	// Ident Grandeur ordonnée mapping
		m_IdFiltreAudience  = 0;	// Ident Filtre d'audience	
		m_IdContexteLecture = 0;	// Ident Contexte de lecture
		m_bRepriseEnMain	= true; // Prise en compte reprises en main
		m_bLNMMag			= false;// Privilégier audience LNM pour magazines
		m_bLDPQtd			= false;// Privilégier audience LDP pour quotidiens
		m_bLDP30J			= false;// Privilégier audience LDP 30 j pour les bimestriels
		m_CumulFamille		= false;// Pas de calcul avec cumul famille, mais moyenne par défaut
 	}

 	// les opérateurs pour comparer les éléments
	JBool 		operator <(const CKeyInfoMapping & rhs) const;
	JBool 		operator >(const CKeyInfoMapping & rhs) const;

	// opérateur d'affectation
 	JUnt32		m_IdTerrain; 		// Ident Terrain via liste terrains sélectionnés
 	JUnt32		m_IdCible;			// Ident Cible via liste cibles sélectionnés
	JUnt32      m_IdCibleAff;		// Ident cible affinité
	JUnt32		m_IdSegment;		// Ident segment : combinaison des segments sur un long (32 positions possibles)
	JUnt32		m_IdSupport;		// Ident Support via liste supports sélectionnés

	JUnt32		m_IdFormat;			// Ident Format via liste formats sélectionnés
	JUnt32		m_IdMoteur;			//	Ident Moteur
	JUnt32		m_NbInsertions;		// Nombre d'insertions	

	JUnt32		m_IdGrandeurAbs;	// Ident Grandeur abscisse mapping
	JUnt32		m_IdGrandeurOrd;	// Ident Grandeur ordonnée mapping
	JUnt32		m_IdFiltreAudience;	// Ident Filtre d'audience	
	JUnt32		m_IdContexteLecture;// Ident Contexte de lecture
	
	JBool		m_bRepriseEnMain;	// Prise en compte reprises en main
	JBool		m_bLNMMag;			// Privilégier audience LNM pour magazines
	JBool		m_bLDPQtd;			// Privilégier audience LDP pour quotidiens
	JBool		m_bLDP30J;			// Privilégier audience LDP 30 j pour les bimestriels

	JBool		m_CumulFamille;		// Calcul avec cumul famille (comme si c'était un plan de supports)
									// par défaut false = calcul moyenne
 };

class AFX_EXT_CLASS CPointLibelle
{
public:
	double  m_XLib;			// la position en x			 du libellé
	double  m_YLib;			// la position en y			 du libellé
	// constructeur
	CPointLibelle()
	{
		m_XLib = 0.05;
		m_YLib = 0.05;
	}
};


///////////////////////////////// POUR STOCKAGE CALCUL DUPLICATION ///////////////////////////
// Classe pour la clé des résultats duplication
class AFX_EXT_CLASS CKeyResultDup
{
 public:
 	// Constructor
 	CKeyResultDup()
 	{
 		m_IdTerrain		=	0;
 		m_IdCible		=	0;
		m_IdSegment		=   0;
 		m_IdSupportRow	=	0;
		m_IdSupportCol	=	0;
		m_IdMoteur		=   0;
 	}

 	// les opérateurs pour comparer les éléments
	JBool 		operator <(const CKeyResultDup & rhs) const;
	JBool 		operator >(const CKeyResultDup & rhs) const;

 	JUnt32	m_IdTerrain; 	// 	Ident Terrain via liste terrains sélectionnés
 	JUnt32	m_IdCible;		//	Ident Cible via liste terrains sélectionnés
	JUnt32	m_IdSegment;	//  Ident segment : combinaison des segments sur un long (32 positions possibles)  
	JUnt32	m_IdSupportRow;	//	Ident Support en ligne
	JUnt32	m_IdSupportCol;	//	Ident Support en colonne
	JUnt32	m_IdMoteur;		//	Ident Moteur
 };

// classe résultats duplication
class AFX_EXT_CLASS CResultDup
{
	public:
 	// Constructor
 	CResultDup()
 	{
		AudienceDup		= 0.0;
		Penetration		= 0.0;
		CentrageH		= 0.0;			
		CentrageV		= 0.0;			
		CoeffYule		= 0.0;
		m_ResultPartiel	= false;	// par défaut résultat complet	
 	}
 
	// Les différents résultats après calcul
	JFlt64 AudienceDup;
	JFlt64 Penetration;
	JFlt64 CentrageH;			
	JFlt64 CentrageV;			
	JFlt64 CoeffYule;
	JBool  m_ResultPartiel;				// indique si résultat partiel (servira pour familles)
};

// classe informations tri
class AFX_EXT_CLASS CInfoTriCol
{
	public:
 	// Constructor
 	CInfoTriCol()
 	{
		NoCol			= 1;
		TriCroissant	= true;
 	}

	// Opérateur d'affectation
	CInfoTriCol & operator =(const CInfoTriCol & rhs)
	{
		NoCol		 = rhs.NoCol;
		TriCroissant = rhs.TriCroissant;
		return *this;
	}
 
	// Les infos états tri colonne
	JUnt32 NoCol;
	bool   TriCroissant;
};

class AFX_EXT_CLASS JATPDocChoixSupport : public JATPDocument
{
public:
	JATPDocChoixSupport(void);
	~JATPDocChoixSupport(void);

	// Validité du document choix supports
	virtual JBool IsValid() const;

	// les fonctions pour sérialiser l'élément
	virtual JVoid Send(JStream & Stream);
	virtual JVoid Recv(JStream & Stream);

	// Désarchivage élmt No Version Choix Support
	virtual JVoid RecvNoVersion(JStream & Stream, JUnt32 & NoVersion);

	// la fonction pour permuter l'élément
	virtual JVoid Swap(JATPDocChoixSupport & Source);

	//la fonction de recopie
	JATPDocChoixSupport & operator =(const JATPDocChoixSupport & Source);

	// Sélection valide
	JBool SelectionValid();

public:

	// Le plateau
	JATPKEYPlateau				m_KEYPlateau;

	// Récup index module courant
	void	SetModuleCourant(JUnt32 InxModule);

	// Positionne index module courant
	JUnt32  GetModuleCourant();

	// Récup paramètrage par défaut
	JBool RecupParamDef();

	// *********************** FONCTIONS RECUPERATION LISTES SELECTION ***************************
	//
	// Source d'Audience Courante
	virtual JUnt32 GetIdSrcAudienceSel();
	virtual void SetIdSrcAudienceSel(JUnt32 Id);

	// Source de Ciblage Courante
	virtual JUnt32 GetIdSrcCiblageSel();
	virtual void SetIdSrcCiblageSel(JUnt32 Id);

	// Terrain Ciblage Selectionné
	virtual JUnt32 GetIdTerrainCiblageSel();
	virtual void SetIdTerrainCiblageSel(JUnt32 Id);

	// Segment d'Audience Selectionné
	virtual JUnt32 GetIdSegmentAudienceSel();
	virtual void SetIdSegmentAudienceSel(JUnt32 Id);

	// Segment de Ciblage Selectionné
	virtual JUnt32 GetIdSegmentCibleSel();
	virtual void SetIdSegmentCibleSel(JUnt32 Id);

	// Le filtre d'audience
	virtual JUnt32 GetIdFiltreAudienceSel();
	virtual void SetIdFiltreAudienceSel(JUnt32 Id);

	// Map des terrains audience sélectionnés
	virtual JMap<JUnt32,JUnt32> * GetMapIdTerrainsAudienceSel();
	virtual void SetMapIdTerrainsAudienceSel(JMap<JUnt32,JUnt32> &MapId);

	// Map des cibles audience sélectionnés
	virtual JMap<JUnt32,JUnt32> * GetMapIdCiblesAudienceSel();
	virtual void SetMapIdCiblesAudienceSel(JMap<JUnt32,JUnt32> &MapId);

	// Map des segments audience sélectionnés
	virtual JMap<JUnt32,JUnt32> * GetMapIdSegmentsAudienceSel();
	virtual JMap<JUnt32,JUnt32> * GetMapIdSegmentsAudienceSel(JUnt32 IndexModule);
	virtual void SetMapIdSegmentsAudienceSel(JMap<JUnt32,JUnt32> &MapId);

	// Indique si support appartient au(x) segment(x) sélectionnés
	virtual JVoid GetMapSupportValideSegTerrain(JMap<JUnt32,JUnt32> &MapSuppSegTerrain);

	// Validité support via les terrains sélectionnés et segments
	JBool SupportValid(IElem* pelem, JUnt32 &IdTerrain, JUnt32 &IdSegment);

	// Ident 1er cible sélectionnée
	virtual JUnt32 GetIdCibleSel();

	// Map de Formats Actifs
	JMap<JUnt32,JUnt32> * GetMapIdFormatsSel();	
	virtual void SetMapIdFormatsSel(JMap<JUnt32,JUnt32> &MapId);	

	// Gestion Map Visibilité supports
	//
	// Positionne visibilité d'un support
    virtual void  SetSupportVisible(JUnt32 &IdSupport, JBool Visible);
	
	// Positionne tous les supports visibles
	virtual void SetAllSupportVisible(JBool Visible);

	// Seulement les supports sélectionnés visibles
	virtual void SetAllSupportSelVisible(JBool Visible);

	// Seulement les supports non sélectionnés visibles
	virtual void SetAllSupportNonSelVisible(JBool Visible);

	// Sélection ou déselection d'un support 
	virtual void SetSelSupport(JUnt32 &IdSupport, JBool Selection);

	// Récupère visibilité d'un support
	virtual JBool GetSupportVisible(JUnt32 &IdSupport);

	// Nombre de supports visibles
	virtual JUnt32 GetNbSuppVisible();

	// Filtre des supports via segment(s) sélectionnés
	void FiltreSupportsSegments();

	// Rend visible ou non visible les supports viable sur le ou les segments
	void SetSupportsSegmentsVisible();

	// Récupèration validation support sur environnement terrain/segment sélectionné
	JBool ValideSupportSegments(JUnt32 IdSupport);

	// Visibilité mode Supports et/ou familles
	JBool VisibleSupportMode(JUnt32 IdSupport);

	// Repositionnement moteur via nouvelle offre
	virtual JVoid UpdateOffreMoteur();

	// Calcul clef segment selon module
	JUnt32	ClefSegment(JUnt32 InxModule);

	// Nb segment au total sur le ou les terrains
	JUnt32		m_NbTotSegDispo;
	
	//***************************** SELECTIONS ****************************
	//
	// Date Tarifaire
	JDate 						m_DateTarifaire;

	// les supports sélectionnées avec leur état
	JMap<JUnt32,JBool>			m_MapIdSupportsSel;

	// Support visible ou pas
	JMap<JUnt32,JBool>			m_MapIdSupportsVisible;

	// les couleurs supports pour dessin courbe
	JMap<JUnt32,JUnt32>			m_MapIdColorSupports;

	// Mode de sélection des couleurs	(0 >> séparées 1 couleur représente 1 support sur le graphe)
	//									(1 >> groupées 1 couleur peut représenter plusieurs supports sur le graphe)
	JUnt32						m_ModeColorGraphe;	

	// Liste des Grandeurs sélectionnées pour échelle
	JMap<JUnt32,JUnt32> 		m_MapIdGrandEchSel;
	
	// Nb insertions sélectionné pour calcul
	JUnt32 						m_NbInsertions;

	// Nb insertions sélectionné pour calcul
	JUnt32 						m_NbInsertionsCum;

	// Ordre des grandeurs affichées dans tableau final
	JList <JUnt32>				m_LstOrdoGrandeurEch;

	// Grandeur sélectionnée pour abscisse mapping
	JInt32 						m_IdGrandMapSelAbs;

	// Grandeur sélectionnée pour ordonnée mapping
	JInt32 						m_IdGrandMapSelOrd;

	// Liste des Grandeurs sélectionnées pour cumulativité
	JMap<JUnt32,JUnt32> 		m_MapIdGrandCumSel;

	// Liste des Grandeurs sélectionnées pour duplication
	JMap<JUnt32,JUnt32> 		m_MapIdGrandDupSel;

	/////////////////////////////// Maps des formats spécifiques par application /////////////////////////////
	JMap<JUnt32,JUnt32> 		m_MapIdFormatSpecifEch;
	JMap<JUnt32,JUnt32> 		m_MapIdFormatSpecifMap;
	JMap<JUnt32,JUnt32> 		m_MapIdFormatSpecifCum;
	JMap<JUnt32,JUnt32> 		m_MapIdFormatSpecifDup;

	// Map des positions libellés mapping
	JMap<CKeyInfoMapping,CPointLibelle> m_MapPointMapping;
	
	// énumération des différents modules de Choix Support
	enum {ModuleEch = 0, ModuleMap, ModuleCum, ModuleDup};

	// Tableau des résultats courants pour échelle
	JMap <CKeyResultEch,CResultEch>		m_MapResultatEch; 

	// Tableau des résultats courants pour cumulativité
	JMap <CKeyResultCum,CResultCum>		m_MapResultatCum; 

	// A FAIRE / Tableau des résultats courants pour duplication
	JMap <CKeyResultMap,CResultMap>		m_MapResultatMap; 

	// A FAIRE / Tableau des résultats courants pour duplication
	JMap <CKeyResultDup,CResultDup>		m_MapResultatDup; 

	// Flags modification pour résultat Echelle / Mapping / Cumulativité / duplication
	JBool m_ParamEchModif;
	JBool m_ParamMapModif;
	JBool m_ParamCumModif;
	JBool m_ParamDupModif;

	// Ordre des headers dans échelles (1 / Terrain 2/ Cibles 3/Grandeurs
	CArray <JUnt32> m_OrdreEnteteEch;

	//Indice couleur courbe cumulativité en cours
	JUnt32 m_IdColorCum;

	// Stockage info tri colonnes sur les différents modules
	CArray <CInfoTriCol,CInfoTriCol&> m_TabInfoTriCol;

	// Matrice résultats courbe cumulativité pour chaque support
	JMap <JUnt32, JArray<JFlt64> > m_MapResultCrbCum;

	// Matrice info périodicité courbe cumulativité pour chaque support
	JMap <JUnt32, JArray<bool> > m_MapResultEtalCrbCum;

	// Flag affichage des rangs résultats
	JBool m_AFFRangResultatEch;

	// Flag affichage moyenne résultat sur une grandeur
	JBool m_AFFMoyenneResultatEch;

	// Liste des id grandeurs moyennes par module (utilisées pour la calcul colonne moyenne)
	JArray <JInt32> m_TblIdGrandeurMoyenne; 

private:

	friend class CATPDocCSSerializer;

	// Index du module courant
	JUnt32							m_InxModuleCur;

	//***************************** LISTE SELECTIONS SELON MODULE ****************************
	//
	// Source d'Audience Courante
	JList <JUnt32>					m_LstIdSrcAudienceSel;

	// Source de Ciblage Courante
	JList <JUnt32>					m_LstIdSrcCiblageSel;

	// Terrain Ciblage Selectionné
	JList <JUnt32>					m_LstIdTerrainCiblageSel;

	// Segment d'Audience Selectionné (Segment Population)
	JList <JUnt32>					m_LstIdSegmentAudienceSel;

	// Segment de Ciblage Selectionné
	JList <JUnt32>					m_LstIdSegmentCibleSel;

	// Le filtre d'audience
	JList <JUnt32>					m_LstIdFiltreAudienceSel;

	// List des map terrains audience sélectionnés
	JList <JMap<JUnt32,JUnt32> >	m_LstMapIdTerrainsAudienceSel;

	// Liste des cibles audience sélectionnés par module
	JList <JMap<JUnt32,JUnt32> >	m_LstMapIdCiblesAudienceSel;

	// Liste des segments audience sélectionnés par module
	JList <JMap<JUnt32,JUnt32> >	m_LstMapIdSegmentsAudienceSel;

	// Liste des map des Formats Actifs  (Les formats pour insertion)
	JList <JMap<JUnt32,JUnt32> >	m_LstMapIdFormatsSel;

	// Map état affichage rangs résultats
	JMap <JUnt32,JBool> m_MapEtatRangResultEch;

public :
	// Nb supports sélectionnés
	JUnt32	GetNbSupportsSel();

	/////////////////////////////////////////////////////////////////////////////////////////////////////
	// Nb supports visibles
	JUnt32 GetNbSupportsVisible();

	// Nb supports 
	JUnt32 GetNbSupports();

	// Récupére nb grandeurs disponibles	
	JUnt32 GetNbGrandeurs(JUnt32 TypeModule);

	// Récupére libellé grandeur
	CString GetGrandeur(JUnt32 TypeModule, JUnt32 IndexGrandeur);

	// Récupére libellé abrégé grandeur
	CString GetAbrGrandeur(JUnt32 TypeModule, JUnt32 IndexGrandeur);

	// Récupération périodicité support
	void GetPeriodicite(JUnt32 IdSupport, JString &TxtPeriod, JString &TxtAbrPeriod);

	// Récupération périodicité support
	JUnt32 GetMaxInsertions(JUnt32 IdSupport);

	// Récupération libellé format courant support et liste des formats support		
	void GetFormatsSupport(JUnt32 IdSupport, long &InxSelFormat, CString &ListFormatSupp, JUnt32 TypeModule);

	// Récupération libellé format courant famille et liste des formats famille
	void GetFormatsFamille(JUnt32 IdFamille, long &InxSelFormat, CString &ListFormatFamille, JUnt32 TypeModule);

	/////////////////////////////// Gestion des formats pour les différents modules ///////////////////////////////
	//
	// Récupération du libellé format par défaut
	CString GetFormatPrincipal(bool Abrege);

	// Récupération de l'ident format par défaut
	JUnt32 GetIdFormatPrincipal();
    
	// Récupération du libellé format (abrégé ou complet)
	CString GetFormat(JUnt32 IdFormat, bool Abrege);

	// Récupération de l'ident format via libellé
	JUnt32 GetIdFormat(CString LibFormat, bool Abrege);

	// Récupération des idents libellés format possible pour un support
	void GetListIdFormats(JUnt32 IdSupport, JMap <JUnt32,JUnt32> *pLstIdFormats);

	/////////////////////////////////////// Formatage des cellules ////////////////////////////////////////////////
	//
	enum {FMT_TEXT, FMT_TEXT_MILLIERS, FMT_INT, FMT_FLOAT_1DEC, FMT_FLOAT_2C};

	// Découpage chaine par milliers
	void FormatString(CString &Txt, JUnt32 PasDecoup);

	// Complète chaine numérique avec des caractères devant (pour palier au problème tri)
	void CompleteString(CString &Txt, JUnt32 NbTotCar);

	// Recompose la chaine selon le formatage demandée
	void RebuildString(CString &Txt, JUnt32 ModeRebuild, JUnt32 NbDecimale);

	// Spécifités des élmts des grilles
	// Les fonts
	JUnt32 m_SizeFontHeader;
	JUnt32 m_SizeFontCell;

	// Les styles
	JBool  m_BoldFontHeader;
	JBool  m_BoldFontCell;
	JBool  m_ItalicResultIncomplet;
	JBool  m_ItalicResultComplet;

	// Réinitialise résultats de tous les modules
	void ResetResultat(JUnt32 InxModule);

	// Réinitialise résultats d'un module
	void ResetAllResultat();

	// Précalcul Graphe Cum selon le mode de traçage (0/ Traçage séparé 1/ Traçage regroupé par couleur)
	void PrepareMatriceTraceCum();

	// Récupération résultat cumulativité
	JFlt64 RecupResultatCum(JUnt32 IdSupport, JUnt32 IdTerrain, JUnt32 IdCible, JUnt32 NbInsertions, JUnt32 &NbJourPtEtal);

	// Résultat via infos résultats généraux cumulativité
	JFlt64 ResultGrandeurCum(JFlt64 Tarif, JFlt64 VolLecture, JFlt64 Audience, JFlt64 Audience1Ins,
 					         JFlt64 Effectif,   JFlt64 NbCas, JInt32 NbInsertions);

	// Calcul du nombres de jours à prendre en compte pour la courbe cumulativité
	JUnt32 MaxJourCourbeCum(JUnt32 IdTerrain, JUnt32 IdCible);

	// Positionnement Id grandeur destinée à la moyenne
	JVoid SetIdGrandeurMoyenne(JUnt32 TypeModule,JUnt32 IdGrandeur);

	// Récupére Id grandeur destinée à la moyenne
	JInt32 GetIdGrandeurMoyenne(JUnt32 TypeModule);

	// Validité grandeur destinée à la moyenne
	JBool IdGrandeurMoyenneValid(JUnt32 TypeModule,JUnt32 IdGrandeur);

	// Init état des rangs à afficher dans Echelle
	JVoid SetRangVisible(JUnt32 TypeModule, JBool EtatAffichage, JInt32 IdGrandeur = -1);

	// Récup état des rangs afficher dans Echelle
	JBool GetRangVisible(JUnt32 TypeModule, JInt32 IdGrandeur);

	// Récupére tarif associé au support
	JInt32 GetTarifSupport(JUnt32 IdFamille, JUnt32 IdFormat, JBool &TarifManuel);

	// Récupére le tarif associé à une famille (on passe le vecteur des supports valides)
	JInt32 GetTarifFamille(JUnt32 IdFamille, JUnt32 IdFormat, JDate DateTarifaire, 
		                   JDate &DateExact, JUnt32 &Duree,   JMap <JUnt32,JUnt32> &MapSuppSegTerr,
						   JBool TarifManuel);

	// Récupère la périodicité de la famille
	JString GetPeriodiciteFamille(JUnt32 IdFamille);

protected:
	// la fonction pour faire le machin cad???
	virtual JVoid SynchroSupports();

	// la fonction pour préparer le contenu du document
	virtual BOOL OnPrepareContents();

	// la fonction pour mettre à jour le document
	virtual VOID OnUpdate(const JFCUpdate & Message);

	// la fonction pour mettre à jour le plateau
	virtual JVoid UpdatePlateau();


public :
	// Map des grandeurs utilisables pour échelle (pour liste sélection, et abrégé pour tableur)
	JMap <JUnt32,CString> m_MapGrandEch;
	JMap <JUnt32,CString> m_MapAbrGrandEch;

	// Map des grandeurs utilisables pour mapping (pour liste sélection, et abrégé pour tableur)
	JMap <JUnt32,CString> m_MapGrandMap;
	JMap <JUnt32,CString> m_MapAbrGrandMap;

private :
	// Tab des grandeurs utilisables pour cumulativité (pour liste sélection, et abrégé pour tableur)
	CArray <CString> m_TabGrandCum;
	CArray <CString> m_TabAbrGrandCum;

	// Tab des grandeurs utilisables pour duplication (pour liste sélection, et abrégé pour tableur)
	CArray <CString> m_TabGrandDup;
	CArray <CString> m_TabAbrGrandDup;

	// Chargement des terrains
	virtual JBool OnLoadTerrains();
	virtual JBool OnChangeTerrains();

	// Chargement des Supports
	virtual JBool OnLoadSupports();

	// Chargement des items cibles
	virtual JBool OnLoadItems();

	// Chargement des segmennts population
	virtual JVoid OnLoadSegPop();

	// Chargement des formats
	virtual JVoid OnLoadFormats();

	// Init Liste d'identicateur
	void InitListFen(JList <JUnt32> &Lst, JUnt32 ValiInit);

	// No de la grandeur destiné à la moyenne
	JUnt32 m_IdGrandeurMoyenne;

public :
	
	// Les grandeurs analyse disponibles pour l'échelle
	enum {GrdEch_AudienceLDP = 0, 
		  GrdEch_Audience, 
		  GrdEch_Audience000, 
		  GrdEch_NbContact000,
		  GrdEch_GRP,
		  GrdEch_XGRP,
		  GrdEch_CouvCum,
		  GrdEch_Repetition,
		  GrdEch_XRepetition,
		  GrdEch_IndAffinite,
		  GrdEch_PrctAff,
		  GrdEch_Cumulativite,
		  GrdEch_CoutGRP,
		  GrdEch_CoutCouv,
		  GrdEch_Cout000Ind,
		  GrdEch_Cout000Cts}; 

	// Les autres grandeurs type exogènes seront ajoutées selon les datas exogènes existantes
	// Ils commenceront par ident 1000 ou 2000 etc...

	// Les grandeurs analyse disponibles pour le mapping
	enum {GrdMap_AudienceLDP = 0, 
		  GrdMap_Audience, 
		  GrdMap_Audience000, 
		  GrdMap_NbContact000,
		  GrdMap_GRP,
		  GrdMap_XGRP,
		  GrdMap_CouvCum,
		  GrdMap_Repetition,
		  GrdMap_XRepetition,
		  GrdMap_IndAffinite,
		  GrdMap_PrctAff,
		  GrdMap_Cumulativite,
		  GrdMap_CoutGRP,
		  GrdMap_CoutCouv,
		  GrdMap_Cout000Ind,
		  GrdMap_Cout000Cts}; 

	// Les grandeurs analyse disponibles pour la cumulativité
	enum {GrdCum_Audience = 0, 
		  GrdCum_Audience000, 
		  GrdCum_NbContact000,
		  GrdCum_GRP,
		  GrdCum_XGRP,
		  GrdCum_CouvCum,
		  GrdCum_Repetition,
		  GrdCum_XRepetition,
		  GrdCum_Cumulativite,
		  GrdCum_CoutCouv}; 

	// Les grandeurs analyse disponibles pour la duplication
	enum {GrdDup_Audience_LDPLNM = 0, 
		  GrdDup_Audience_LDPLNM_000, 
		  GrdDup_Penetration, 
		  GrdDup_CentrageH,
		  GrdDup_CentrageV,
		  GrdDup_CoeffYule};

};

// fin de l'inclusion conditionnelle
#endif
