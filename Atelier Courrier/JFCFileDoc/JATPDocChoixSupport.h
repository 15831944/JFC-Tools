//
// Fichier: JATPDocChoixSupport.h
// Auteur:  Alain CHAMBARD
// Date:    06/04/2004
// Gestion document s�lection Choix Support
//
#pragma once

// on v�rifie que le fichier n'a pas d�j� �t� inclus
#ifndef JATP_DOC_CHOIX_SUPPORT_H

// on d�finit la macro pour �viter les inclusions multiples
#define JATP_DOC_CHOIX_SUPPORT_H

#include "JATPDocument.h"
#include "JLib.h"
#include "JATPKEYPlateau.h"
#include "JATPOffre.h"
#include "Terrain.h"
// #include "MoteurPresse.h"

///////////////////////////////// POUR STOCKAGE CALCUL ECHELLE ///////////////////////////
// Classe pour la cl� des r�sultats �chelles
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

 	// les op�rateurs pour comparer les �l�ments
	JBool 		operator <(const CKeyResultEch & rhs) const;
	JBool 		operator >(const CKeyResultEch & rhs) const;

 	JUnt32		m_IdTerrain; 	// 	Ident Terrain via liste terrains s�lectionn�s
 	JUnt32		m_IdCible;		//	Ident Cible via liste terrains s�lectionn�s
	JUnt32      m_IdCibleAff;	//  Ident cible affinit�
	JUnt32		m_IdSegment;	//  Ident segment : combinaison des segments sur un long (32 positions possibles)
	JUnt32		m_IdSupport;	//	Ident Support via liste terrains s�lectionn�s
	JUnt32		m_IdMoteur;		//	Ident Moteur
 };

// classe r�sultats �chelles
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
		m_ResultPartiel			= false;	// par d�faut r�sultat complet	
 	}
 
	// Les diff�rents r�sultats apr�s calcul
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
	JBool	m_ResultPartiel;				// indique si r�sultat partiel (servira pour familles)
};

///////////////////////////////// POUR STOCKAGE CALCUL CUMULATIVITE ///////////////////////////
// Classe pour la cl� des r�sultats cumulativit�
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

 	// les op�rateurs pour comparer les �l�ments
	JBool 		operator <(const CKeyResultCum & rhs) const;
	JBool 		operator >(const CKeyResultCum & rhs) const;

 	JUnt32		m_IdTerrain; 	// 	Ident Terrain via liste terrains s�lectionn�s
 	JUnt32		m_IdCible;		//	Ident Cible via liste terrains s�lectionn�s
	JUnt32		m_IdSegment;	//  Ident segment : combinaison des segments sur un long (32 positions possibles)
	JUnt32		m_IdSupport;	//	Ident Support via liste terrains s�lectionn�s
	JUnt32		m_IdMoteur;		//	Ident Moteur
	JUnt32		m_NbInsert;		//  Nb insertions pour r�sultats
 };

// classe r�sultats cumulativit�
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
		m_ResultPartiel			= false;	// par d�faut r�sultat complet	
 	}
 
	// Les diff�rents r�sultats apr�s calcul
 	JFlt64  m_VolLecture;
	JFlt64  m_Audience;
	JFlt64  m_AudProba1Ins;
	JFlt64  m_Aud1Ins;
	JFlt64  m_Effectif;
	JInt32  m_NbCas;
	JUnt32  m_PtEtalonnage;					//  Nb jours par insertion 
	JBool	m_ResultPartiel;				// indique si r�sultat partiel (servira pour familles)
};


///////////////////////////////// POUR STOCKAGE CALCUL MAPPING ///////////////////////////
// Classe pour la cl� des r�sultats Mapping
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

 	// les op�rateurs pour comparer les �l�ments
	JBool 		operator <(const CKeyResultMap & rhs) const;
	JBool 		operator >(const CKeyResultMap & rhs) const;

 	JUnt32		m_IdTerrain; 	// 	Ident Terrain via liste terrains s�lectionn�s
 	JUnt32		m_IdCible;		//	Ident Cible via liste terrains s�lectionn�s
	JUnt32      m_IdCibleAff;	//  Ident cible affinit�
	JUnt32		m_IdSegment;	//  Ident segment : combinaison des segments sur un long (32 positions possibles)
	JUnt32		m_IdSupport;	//	Ident Support via liste terrains s�lectionn�s
	JUnt32		m_IdMoteur;		//	Ident Moteur
 };

// classe r�sultats mapping
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
		m_ResultPartiel			= false;	// par d�faut r�sultat complet	
 	}
 
	// Les diffr�ents r�sultats apr�s calcul
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
	JBool	m_ResultPartiel;				// indique si r�sultat partiel (servira pour familles)
};

///////////////////////////////// POUR STOCKAGE INFO POSITIONS MAPPING ///////////////////////////
// Classe pour la cl� des positions Mapping
class AFX_EXT_CLASS CKeyInfoMapping
{
 public:
 	// Constructor
 	CKeyInfoMapping()
 	{
		m_IdTerrain			= 0;	// Ident Terrain via liste terrains s�lectionn�s
 		m_IdCible			= 0;	// Ident Cible via liste cibles s�lectionn�s
		m_IdCibleAff		= 0;	// Ident cible affinit�
		m_IdSegment			= 0;	//  Ident segment : combinaison des segments sur un long (32 positions possibles)
		m_IdSupport			= 0;	// Ident Support via liste supports s�lectionn�s
		m_IdFormat			= 0;	// Ident Format via liste formats s�lectionn�s
		m_IdMoteur			= CMoteurPresse::PROBA;	// Ident moteur
		m_NbInsertions		= 0;	// Nombre d'insertions	
		m_IdGrandeurAbs		= 0;	// Ident Grandeur abscisse mapping
		m_IdGrandeurOrd		= 0;	// Ident Grandeur ordonn�e mapping
		m_IdFiltreAudience  = 0;	// Ident Filtre d'audience	
		m_IdContexteLecture = 0;	// Ident Contexte de lecture
		m_bRepriseEnMain	= true; // Prise en compte reprises en main
		m_bLNMMag			= false;// Privil�gier audience LNM pour magazines
		m_bLDPQtd			= false;// Privil�gier audience LDP pour quotidiens
		m_bLDP30J			= false;// Privil�gier audience LDP 30 j pour les bimestriels
		m_CumulFamille		= false;// Pas de calcul avec cumul famille, mais moyenne par d�faut
 	}

 	// les op�rateurs pour comparer les �l�ments
	JBool 		operator <(const CKeyInfoMapping & rhs) const;
	JBool 		operator >(const CKeyInfoMapping & rhs) const;

	// op�rateur d'affectation
 	JUnt32		m_IdTerrain; 		// Ident Terrain via liste terrains s�lectionn�s
 	JUnt32		m_IdCible;			// Ident Cible via liste cibles s�lectionn�s
	JUnt32      m_IdCibleAff;		// Ident cible affinit�
	JUnt32		m_IdSegment;		// Ident segment : combinaison des segments sur un long (32 positions possibles)
	JUnt32		m_IdSupport;		// Ident Support via liste supports s�lectionn�s

	JUnt32		m_IdFormat;			// Ident Format via liste formats s�lectionn�s
	JUnt32		m_IdMoteur;			//	Ident Moteur
	JUnt32		m_NbInsertions;		// Nombre d'insertions	

	JUnt32		m_IdGrandeurAbs;	// Ident Grandeur abscisse mapping
	JUnt32		m_IdGrandeurOrd;	// Ident Grandeur ordonn�e mapping
	JUnt32		m_IdFiltreAudience;	// Ident Filtre d'audience	
	JUnt32		m_IdContexteLecture;// Ident Contexte de lecture
	
	JBool		m_bRepriseEnMain;	// Prise en compte reprises en main
	JBool		m_bLNMMag;			// Privil�gier audience LNM pour magazines
	JBool		m_bLDPQtd;			// Privil�gier audience LDP pour quotidiens
	JBool		m_bLDP30J;			// Privil�gier audience LDP 30 j pour les bimestriels

	JBool		m_CumulFamille;		// Calcul avec cumul famille (comme si c'�tait un plan de supports)
									// par d�faut false = calcul moyenne
 };

class AFX_EXT_CLASS CPointLibelle
{
public:
	double  m_XLib;			// la position en x			 du libell�
	double  m_YLib;			// la position en y			 du libell�
	// constructeur
	CPointLibelle()
	{
		m_XLib = 0.05;
		m_YLib = 0.05;
	}
};


///////////////////////////////// POUR STOCKAGE CALCUL DUPLICATION ///////////////////////////
// Classe pour la cl� des r�sultats duplication
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

 	// les op�rateurs pour comparer les �l�ments
	JBool 		operator <(const CKeyResultDup & rhs) const;
	JBool 		operator >(const CKeyResultDup & rhs) const;

 	JUnt32	m_IdTerrain; 	// 	Ident Terrain via liste terrains s�lectionn�s
 	JUnt32	m_IdCible;		//	Ident Cible via liste terrains s�lectionn�s
	JUnt32	m_IdSegment;	//  Ident segment : combinaison des segments sur un long (32 positions possibles)  
	JUnt32	m_IdSupportRow;	//	Ident Support en ligne
	JUnt32	m_IdSupportCol;	//	Ident Support en colonne
	JUnt32	m_IdMoteur;		//	Ident Moteur
 };

// classe r�sultats duplication
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
		m_ResultPartiel	= false;	// par d�faut r�sultat complet	
 	}
 
	// Les diff�rents r�sultats apr�s calcul
	JFlt64 AudienceDup;
	JFlt64 Penetration;
	JFlt64 CentrageH;			
	JFlt64 CentrageV;			
	JFlt64 CoeffYule;
	JBool  m_ResultPartiel;				// indique si r�sultat partiel (servira pour familles)
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

	// Op�rateur d'affectation
	CInfoTriCol & operator =(const CInfoTriCol & rhs)
	{
		NoCol		 = rhs.NoCol;
		TriCroissant = rhs.TriCroissant;
		return *this;
	}
 
	// Les infos �tats tri colonne
	JUnt32 NoCol;
	bool   TriCroissant;
};

class AFX_EXT_CLASS JATPDocChoixSupport : public JATPDocument
{
public:
	JATPDocChoixSupport(void);
	~JATPDocChoixSupport(void);

	// Validit� du document choix supports
	virtual JBool IsValid() const;

	// les fonctions pour s�rialiser l'�l�ment
	virtual JVoid Send(JStream & Stream);
	virtual JVoid Recv(JStream & Stream);

	// D�sarchivage �lmt No Version Choix Support
	virtual JVoid RecvNoVersion(JStream & Stream, JUnt32 & NoVersion);

	// la fonction pour permuter l'�l�ment
	virtual JVoid Swap(JATPDocChoixSupport & Source);

	//la fonction de recopie
	JATPDocChoixSupport & operator =(const JATPDocChoixSupport & Source);

	// S�lection valide
	JBool SelectionValid();

public:

	// Le plateau
	JATPKEYPlateau				m_KEYPlateau;

	// R�cup index module courant
	void	SetModuleCourant(JUnt32 InxModule);

	// Positionne index module courant
	JUnt32  GetModuleCourant();

	// R�cup param�trage par d�faut
	JBool RecupParamDef();

	// *********************** FONCTIONS RECUPERATION LISTES SELECTION ***************************
	//
	// Source d'Audience Courante
	virtual JUnt32 GetIdSrcAudienceSel();
	virtual void SetIdSrcAudienceSel(JUnt32 Id);

	// Source de Ciblage Courante
	virtual JUnt32 GetIdSrcCiblageSel();
	virtual void SetIdSrcCiblageSel(JUnt32 Id);

	// Terrain Ciblage Selectionn�
	virtual JUnt32 GetIdTerrainCiblageSel();
	virtual void SetIdTerrainCiblageSel(JUnt32 Id);

	// Segment d'Audience Selectionn�
	virtual JUnt32 GetIdSegmentAudienceSel();
	virtual void SetIdSegmentAudienceSel(JUnt32 Id);

	// Segment de Ciblage Selectionn�
	virtual JUnt32 GetIdSegmentCibleSel();
	virtual void SetIdSegmentCibleSel(JUnt32 Id);

	// Le filtre d'audience
	virtual JUnt32 GetIdFiltreAudienceSel();
	virtual void SetIdFiltreAudienceSel(JUnt32 Id);

	// Map des terrains audience s�lectionn�s
	virtual JMap<JUnt32,JUnt32> * GetMapIdTerrainsAudienceSel();
	virtual void SetMapIdTerrainsAudienceSel(JMap<JUnt32,JUnt32> &MapId);

	// Map des cibles audience s�lectionn�s
	virtual JMap<JUnt32,JUnt32> * GetMapIdCiblesAudienceSel();
	virtual void SetMapIdCiblesAudienceSel(JMap<JUnt32,JUnt32> &MapId);

	// Map des segments audience s�lectionn�s
	virtual JMap<JUnt32,JUnt32> * GetMapIdSegmentsAudienceSel();
	virtual JMap<JUnt32,JUnt32> * GetMapIdSegmentsAudienceSel(JUnt32 IndexModule);
	virtual void SetMapIdSegmentsAudienceSel(JMap<JUnt32,JUnt32> &MapId);

	// Indique si support appartient au(x) segment(x) s�lectionn�s
	virtual JVoid GetMapSupportValideSegTerrain(JMap<JUnt32,JUnt32> &MapSuppSegTerrain);

	// Validit� support via les terrains s�lectionn�s et segments
	JBool SupportValid(IElem* pelem, JUnt32 &IdTerrain, JUnt32 &IdSegment);

	// Ident 1er cible s�lectionn�e
	virtual JUnt32 GetIdCibleSel();

	// Map de Formats Actifs
	JMap<JUnt32,JUnt32> * GetMapIdFormatsSel();	
	virtual void SetMapIdFormatsSel(JMap<JUnt32,JUnt32> &MapId);	

	// Gestion Map Visibilit� supports
	//
	// Positionne visibilit� d'un support
    virtual void  SetSupportVisible(JUnt32 &IdSupport, JBool Visible);
	
	// Positionne tous les supports visibles
	virtual void SetAllSupportVisible(JBool Visible);

	// Seulement les supports s�lectionn�s visibles
	virtual void SetAllSupportSelVisible(JBool Visible);

	// Seulement les supports non s�lectionn�s visibles
	virtual void SetAllSupportNonSelVisible(JBool Visible);

	// S�lection ou d�selection d'un support 
	virtual void SetSelSupport(JUnt32 &IdSupport, JBool Selection);

	// R�cup�re visibilit� d'un support
	virtual JBool GetSupportVisible(JUnt32 &IdSupport);

	// Nombre de supports visibles
	virtual JUnt32 GetNbSuppVisible();

	// Filtre des supports via segment(s) s�lectionn�s
	void FiltreSupportsSegments();

	// Rend visible ou non visible les supports viable sur le ou les segments
	void SetSupportsSegmentsVisible();

	// R�cup�ration validation support sur environnement terrain/segment s�lectionn�
	JBool ValideSupportSegments(JUnt32 IdSupport);

	// Visibilit� mode Supports et/ou familles
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

	// les supports s�lectionn�es avec leur �tat
	JMap<JUnt32,JBool>			m_MapIdSupportsSel;

	// Support visible ou pas
	JMap<JUnt32,JBool>			m_MapIdSupportsVisible;

	// les couleurs supports pour dessin courbe
	JMap<JUnt32,JUnt32>			m_MapIdColorSupports;

	// Mode de s�lection des couleurs	(0 >> s�par�es 1 couleur repr�sente 1 support sur le graphe)
	//									(1 >> group�es 1 couleur peut repr�senter plusieurs supports sur le graphe)
	JUnt32						m_ModeColorGraphe;	

	// Liste des Grandeurs s�lectionn�es pour �chelle
	JMap<JUnt32,JUnt32> 		m_MapIdGrandEchSel;
	
	// Nb insertions s�lectionn� pour calcul
	JUnt32 						m_NbInsertions;

	// Nb insertions s�lectionn� pour calcul
	JUnt32 						m_NbInsertionsCum;

	// Ordre des grandeurs affich�es dans tableau final
	JList <JUnt32>				m_LstOrdoGrandeurEch;

	// Grandeur s�lectionn�e pour abscisse mapping
	JInt32 						m_IdGrandMapSelAbs;

	// Grandeur s�lectionn�e pour ordonn�e mapping
	JInt32 						m_IdGrandMapSelOrd;

	// Liste des Grandeurs s�lectionn�es pour cumulativit�
	JMap<JUnt32,JUnt32> 		m_MapIdGrandCumSel;

	// Liste des Grandeurs s�lectionn�es pour duplication
	JMap<JUnt32,JUnt32> 		m_MapIdGrandDupSel;

	/////////////////////////////// Maps des formats sp�cifiques par application /////////////////////////////
	JMap<JUnt32,JUnt32> 		m_MapIdFormatSpecifEch;
	JMap<JUnt32,JUnt32> 		m_MapIdFormatSpecifMap;
	JMap<JUnt32,JUnt32> 		m_MapIdFormatSpecifCum;
	JMap<JUnt32,JUnt32> 		m_MapIdFormatSpecifDup;

	// Map des positions libell�s mapping
	JMap<CKeyInfoMapping,CPointLibelle> m_MapPointMapping;
	
	// �num�ration des diff�rents modules de Choix Support
	enum {ModuleEch = 0, ModuleMap, ModuleCum, ModuleDup};

	// Tableau des r�sultats courants pour �chelle
	JMap <CKeyResultEch,CResultEch>		m_MapResultatEch; 

	// Tableau des r�sultats courants pour cumulativit�
	JMap <CKeyResultCum,CResultCum>		m_MapResultatCum; 

	// A FAIRE / Tableau des r�sultats courants pour duplication
	JMap <CKeyResultMap,CResultMap>		m_MapResultatMap; 

	// A FAIRE / Tableau des r�sultats courants pour duplication
	JMap <CKeyResultDup,CResultDup>		m_MapResultatDup; 

	// Flags modification pour r�sultat Echelle / Mapping / Cumulativit� / duplication
	JBool m_ParamEchModif;
	JBool m_ParamMapModif;
	JBool m_ParamCumModif;
	JBool m_ParamDupModif;

	// Ordre des headers dans �chelles (1 / Terrain 2/ Cibles 3/Grandeurs
	CArray <JUnt32> m_OrdreEnteteEch;

	//Indice couleur courbe cumulativit� en cours
	JUnt32 m_IdColorCum;

	// Stockage info tri colonnes sur les diff�rents modules
	CArray <CInfoTriCol,CInfoTriCol&> m_TabInfoTriCol;

	// Matrice r�sultats courbe cumulativit� pour chaque support
	JMap <JUnt32, JArray<JFlt64> > m_MapResultCrbCum;

	// Matrice info p�riodicit� courbe cumulativit� pour chaque support
	JMap <JUnt32, JArray<bool> > m_MapResultEtalCrbCum;

	// Flag affichage des rangs r�sultats
	JBool m_AFFRangResultatEch;

	// Flag affichage moyenne r�sultat sur une grandeur
	JBool m_AFFMoyenneResultatEch;

	// Liste des id grandeurs moyennes par module (utilis�es pour la calcul colonne moyenne)
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

	// Terrain Ciblage Selectionn�
	JList <JUnt32>					m_LstIdTerrainCiblageSel;

	// Segment d'Audience Selectionn� (Segment Population)
	JList <JUnt32>					m_LstIdSegmentAudienceSel;

	// Segment de Ciblage Selectionn�
	JList <JUnt32>					m_LstIdSegmentCibleSel;

	// Le filtre d'audience
	JList <JUnt32>					m_LstIdFiltreAudienceSel;

	// List des map terrains audience s�lectionn�s
	JList <JMap<JUnt32,JUnt32> >	m_LstMapIdTerrainsAudienceSel;

	// Liste des cibles audience s�lectionn�s par module
	JList <JMap<JUnt32,JUnt32> >	m_LstMapIdCiblesAudienceSel;

	// Liste des segments audience s�lectionn�s par module
	JList <JMap<JUnt32,JUnt32> >	m_LstMapIdSegmentsAudienceSel;

	// Liste des map des Formats Actifs  (Les formats pour insertion)
	JList <JMap<JUnt32,JUnt32> >	m_LstMapIdFormatsSel;

	// Map �tat affichage rangs r�sultats
	JMap <JUnt32,JBool> m_MapEtatRangResultEch;

public :
	// Nb supports s�lectionn�s
	JUnt32	GetNbSupportsSel();

	/////////////////////////////////////////////////////////////////////////////////////////////////////
	// Nb supports visibles
	JUnt32 GetNbSupportsVisible();

	// Nb supports 
	JUnt32 GetNbSupports();

	// R�cup�re nb grandeurs disponibles	
	JUnt32 GetNbGrandeurs(JUnt32 TypeModule);

	// R�cup�re libell� grandeur
	CString GetGrandeur(JUnt32 TypeModule, JUnt32 IndexGrandeur);

	// R�cup�re libell� abr�g� grandeur
	CString GetAbrGrandeur(JUnt32 TypeModule, JUnt32 IndexGrandeur);

	// R�cup�ration p�riodicit� support
	void GetPeriodicite(JUnt32 IdSupport, JString &TxtPeriod, JString &TxtAbrPeriod);

	// R�cup�ration p�riodicit� support
	JUnt32 GetMaxInsertions(JUnt32 IdSupport);

	// R�cup�ration libell� format courant support et liste des formats support		
	void GetFormatsSupport(JUnt32 IdSupport, long &InxSelFormat, CString &ListFormatSupp, JUnt32 TypeModule);

	// R�cup�ration libell� format courant famille et liste des formats famille
	void GetFormatsFamille(JUnt32 IdFamille, long &InxSelFormat, CString &ListFormatFamille, JUnt32 TypeModule);

	/////////////////////////////// Gestion des formats pour les diff�rents modules ///////////////////////////////
	//
	// R�cup�ration du libell� format par d�faut
	CString GetFormatPrincipal(bool Abrege);

	// R�cup�ration de l'ident format par d�faut
	JUnt32 GetIdFormatPrincipal();
    
	// R�cup�ration du libell� format (abr�g� ou complet)
	CString GetFormat(JUnt32 IdFormat, bool Abrege);

	// R�cup�ration de l'ident format via libell�
	JUnt32 GetIdFormat(CString LibFormat, bool Abrege);

	// R�cup�ration des idents libell�s format possible pour un support
	void GetListIdFormats(JUnt32 IdSupport, JMap <JUnt32,JUnt32> *pLstIdFormats);

	/////////////////////////////////////// Formatage des cellules ////////////////////////////////////////////////
	//
	enum {FMT_TEXT, FMT_TEXT_MILLIERS, FMT_INT, FMT_FLOAT_1DEC, FMT_FLOAT_2C};

	// D�coupage chaine par milliers
	void FormatString(CString &Txt, JUnt32 PasDecoup);

	// Compl�te chaine num�rique avec des caract�res devant (pour palier au probl�me tri)
	void CompleteString(CString &Txt, JUnt32 NbTotCar);

	// Recompose la chaine selon le formatage demand�e
	void RebuildString(CString &Txt, JUnt32 ModeRebuild, JUnt32 NbDecimale);

	// Sp�cifit�s des �lmts des grilles
	// Les fonts
	JUnt32 m_SizeFontHeader;
	JUnt32 m_SizeFontCell;

	// Les styles
	JBool  m_BoldFontHeader;
	JBool  m_BoldFontCell;
	JBool  m_ItalicResultIncomplet;
	JBool  m_ItalicResultComplet;

	// R�initialise r�sultats de tous les modules
	void ResetResultat(JUnt32 InxModule);

	// R�initialise r�sultats d'un module
	void ResetAllResultat();

	// Pr�calcul Graphe Cum selon le mode de tra�age (0/ Tra�age s�par� 1/ Tra�age regroup� par couleur)
	void PrepareMatriceTraceCum();

	// R�cup�ration r�sultat cumulativit�
	JFlt64 RecupResultatCum(JUnt32 IdSupport, JUnt32 IdTerrain, JUnt32 IdCible, JUnt32 NbInsertions, JUnt32 &NbJourPtEtal);

	// R�sultat via infos r�sultats g�n�raux cumulativit�
	JFlt64 ResultGrandeurCum(JFlt64 Tarif, JFlt64 VolLecture, JFlt64 Audience, JFlt64 Audience1Ins,
 					         JFlt64 Effectif,   JFlt64 NbCas, JInt32 NbInsertions);

	// Calcul du nombres de jours � prendre en compte pour la courbe cumulativit�
	JUnt32 MaxJourCourbeCum(JUnt32 IdTerrain, JUnt32 IdCible);

	// Positionnement Id grandeur destin�e � la moyenne
	JVoid SetIdGrandeurMoyenne(JUnt32 TypeModule,JUnt32 IdGrandeur);

	// R�cup�re Id grandeur destin�e � la moyenne
	JInt32 GetIdGrandeurMoyenne(JUnt32 TypeModule);

	// Validit� grandeur destin�e � la moyenne
	JBool IdGrandeurMoyenneValid(JUnt32 TypeModule,JUnt32 IdGrandeur);

	// Init �tat des rangs � afficher dans Echelle
	JVoid SetRangVisible(JUnt32 TypeModule, JBool EtatAffichage, JInt32 IdGrandeur = -1);

	// R�cup �tat des rangs afficher dans Echelle
	JBool GetRangVisible(JUnt32 TypeModule, JInt32 IdGrandeur);

	// R�cup�re tarif associ� au support
	JInt32 GetTarifSupport(JUnt32 IdFamille, JUnt32 IdFormat, JBool &TarifManuel);

	// R�cup�re le tarif associ� � une famille (on passe le vecteur des supports valides)
	JInt32 GetTarifFamille(JUnt32 IdFamille, JUnt32 IdFormat, JDate DateTarifaire, 
		                   JDate &DateExact, JUnt32 &Duree,   JMap <JUnt32,JUnt32> &MapSuppSegTerr,
						   JBool TarifManuel);

	// R�cup�re la p�riodicit� de la famille
	JString GetPeriodiciteFamille(JUnt32 IdFamille);

protected:
	// la fonction pour faire le machin cad???
	virtual JVoid SynchroSupports();

	// la fonction pour pr�parer le contenu du document
	virtual BOOL OnPrepareContents();

	// la fonction pour mettre � jour le document
	virtual VOID OnUpdate(const JFCUpdate & Message);

	// la fonction pour mettre � jour le plateau
	virtual JVoid UpdatePlateau();


public :
	// Map des grandeurs utilisables pour �chelle (pour liste s�lection, et abr�g� pour tableur)
	JMap <JUnt32,CString> m_MapGrandEch;
	JMap <JUnt32,CString> m_MapAbrGrandEch;

	// Map des grandeurs utilisables pour mapping (pour liste s�lection, et abr�g� pour tableur)
	JMap <JUnt32,CString> m_MapGrandMap;
	JMap <JUnt32,CString> m_MapAbrGrandMap;

private :
	// Tab des grandeurs utilisables pour cumulativit� (pour liste s�lection, et abr�g� pour tableur)
	CArray <CString> m_TabGrandCum;
	CArray <CString> m_TabAbrGrandCum;

	// Tab des grandeurs utilisables pour duplication (pour liste s�lection, et abr�g� pour tableur)
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

	// No de la grandeur destin� � la moyenne
	JUnt32 m_IdGrandeurMoyenne;

public :
	
	// Les grandeurs analyse disponibles pour l'�chelle
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

	// Les autres grandeurs type exog�nes seront ajout�es selon les datas exog�nes existantes
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

	// Les grandeurs analyse disponibles pour la cumulativit�
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
