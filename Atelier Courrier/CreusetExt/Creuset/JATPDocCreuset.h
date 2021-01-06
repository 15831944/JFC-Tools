//
// Fichier: JATPDocCreuset.h
// Auteur:  Alain Chambard & Eddie Gent
// Date:    18/08/2003
//

#pragma once
// on vérifie que le fichier n'a pas déjà été inclus
#ifndef JATP_DOC_CREUSET_H

// on définit la macro pour éviter les inclusions multiples
#define JATP_DOC_CREUSET_H

// on inclut les définitions nécessaires
#include "JATPDocument.h"
#include "JATPKEYEtude.h"
#include "JATPKEYPeriodeAction.h"
#include "PlanInsertions.h"
#include "Insertion.h"
#include "PlanMasters.h"
#include "JATPEchelleSupports.h"
#include "JATPPlateauSupports.h"


// On inclut les définitions spécifiques à Creuset
#include "JITPTBLHypotheses.h"
#include "JITPTBLEchelle.h"
#include "JATPAFFResultPlan.h"
#include "JATPAFFZoomPlan.h"
#include "JATPAFFCreuset.h"
#include "JATPAFFEchelle.h"
#include "JATPAFFContrib.h"
#include "ATPAFFVentilation.h"

#include "Terrain.h"
#include "IItem.h"

#include "ATPMapAffHypo.h"

#include "MoteurPresse.h"

// #include "KeyTarifManuel.h"

class CATPIVisitor;

class JATPDocCreuset : public JATPDocument
{
public:
	// le constructeur
	JATPDocCreuset();

	// le destructeur
	~JATPDocCreuset();

	// la fonction pour tester l'élément
	virtual JBool IsValid() const;

	// les fonctions pour sérialiser l'élément
	virtual JVoid Send(JStream & Stream);
	virtual JVoid Recv(JStream & Stream);

	// la fonction pour permuter l'élément
	virtual JVoid Swap(JATPDocCreuset & Source);

private:
	// l'opérateur pour recopier l'élément
	JATPDocCreuset & operator =(const JATPDocCreuset & Source);

	// le constructeur de copie
	JATPDocCreuset(const JATPDocCreuset & Source);

public:

	// Copier Coller
	void CopierPlanClipBoard();
	void CollerPlanClipBoard();
	void AjouterPlanClipBoard();
	void AjouterSuccessifPlanClipBoard();

	// Copy Master Plan to Hypothese
	bool	CopyMasterPlanToHypothese(JUnt32 HypoIdx);
	// Replace Plan with Hypothese
	bool	ReplacePlanWithHypothese(CPlanInsertions *pDest, JUnt32 HypoIdx, JDate DateD, JDate DateF);

	// Update plateau suppors hypothese suite à nouvelle vague
	JVoid   MajPlateauHypotheseNewVague();

	// Récup ligne active (plan de test) selon mode visualisation (offre ou plateau)
	JUnt32 OnRecupLigneSuppActif(JUnt32 IdSupport);

	// Recup nb insertions d'un support pour hypothèse courante
	JUnt32 OnGetNbInsertions(JUnt32 IdSupport,JDate DateDebut, JDate DateFin, JUnt32 IdHypothese);

	JBool OnInsertionSupport(JUnt32 InxSupportPlateau,JDate DateDebut, JDate DateFin, JUnt32 IdHypothese);

	// Récup nb lignes active du plateau courant
	JUnt32 OnNbLigPlanPlateau(JBool PlanEtendu);

	// Recup nb lignes supports actifs toutes hypothèses
	JUnt32 OnNbLigPlanActif();

	JUnt32 GetIndexSuppPlateauViaIndexSuppActif(JUnt32 IndSupportActif);
	
	// Récup Index Plateau via no ligne plan
	JInt32 OnIndexPlateauViaIndexPlan(JUnt32 IndexSupportPlan);

	

	// Ajoute une nouvelle hypothèse
	void AddNewHypo();

	// Pour export des informations
	void Accept(CATPIVisitor * pVisitor);

	// la fonction pour supprimer les insertions (formats actifs / période action)
	JVoid OnSuppInsertion(JUnt32 idxHypo, bool FormatActif);

	// la fonction pour supprimer les insertions (toutes hypothèses / formats actifs / période action)
	JVoid OnSuppInsertionAllHypo();

	// Suppression d'une hypothese
	JVoid OnSuppHypothese(JUnt32 IdxHypothese);

	// Calcul apport via l'hypothèse courante
	JVoid CalculApport(const JITPTBLHypotheses & ITRHypotheses, CInsertion & Insertion, JFlt64 & Apport);

	// Ajout des nouveaux supports de l'offre
	JVoid AjouteSupportsPlateau();

public:

	//--------------------------------- Récupération ou positionnement des sélections ----------------------------------
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
	
	// List des terrains audience sélectionnés (Map ou liste de Maps)
	virtual JMap<JUnt32,JUnt32> * GetMapIdTerrainsAudienceSel();
	virtual void SetMapIdTerrainsAudienceSel(JMap<JUnt32,JUnt32> &MapId);
	
	// Liste de Formats Actifs  (Map ou liste de Maps)
	virtual void GetMapIdFormatsSel(JMap<JUnt32,JUnt32> &MapId);
	virtual void SetMapIdFormatsSel(JMap<JUnt32,JUnt32> &MapId);

public :  // >> mettre en private
	//***************************** SELECTIONS ****************************
	//
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

	// Flag pour le calcul des apports
	JBool						m_ApportARecalculer;

	// Flag pour indiquer si cibles pondérées activées
	JBool						m_CiblesPondsActives;

	JBool						m_ListSupportsChange;

protected:
	// la fonction pour faire le machin cad???
	virtual JVoid SynchroSupports();

	// la fonction pour préparer le contenu du document
	virtual BOOL OnPrepareContents();

	// la fonction pour mettre à jour le document
	virtual VOID OnUpdate(const JFCUpdate & Message);

	// la fonction pour mettre à jour le plateau
	virtual JVoid UpdatePlateau();

private:
	// les fonctions pour vérifier le document
	JBool OnCheckEtude(const JATPKEYEtude & KEYEtude) const;
	JBool OnCheckPeriodeAction(const JATPKEYEtude & KEYEtude, const JATPKEYPeriodeAction & KEYPeriodeAction) const;
	JBool OnCheckHypotheses(const JATPKEYEtude & KEYEtude, const JATPEchelleSupports & EchelleSupports, const JATPPlateauSupports & PlateauSupports, LIST_HYPOTHESES & PLNHypotheses);

	// Chargement des terrains
	virtual JBool OnLoadTerrains();
	JBool OnChangeTerrains();

	// Chargement des Supports
	virtual JBool OnLoadSupports();
	virtual JBool OnUpdateSupports();

	// Mise à jour du plateau supports selon option Plan Etendu ou pas
	JVoid OnPlateauSupport(JBool OptionPlanEtendu);

	// Chargement des items cibles
	virtual JBool OnLoadItems();

	// Chargement des items cibles pondérées
	virtual JBool OnLoadItemsPonderes();

	// Chargement des formats
	virtual JVoid OnLoadFormats();

	//Initialise le moteur avec les terrains et les cibles
	virtual void InitMoteur();  

	// les fonctions pour calculer les interrogations de l'étude
	JVoid OnMakeHypotheses(const JUnt32 & IdHypothese, const JATPKEYPeriodeAction & KEYPeriodeAction, const JATPKEYPeriodeAction & KEYPeriodeTravail, JUnt32 IdItem, LIST_HYPOTHESES & PLNHypotheses, JITPTBLHypotheses & ITRHypotheses, JBool bCompute) ;
	JVoid OnMakeEchelles(const JATPEchelleSupports & EchelleSupports, const JATPKEYEtude & KEYEtude, const JATPKEYPeriodeAction & KEYPeriodeAction, JUnt32 IdItem, JITPTBLEchelle & ITREchelle, JITPTBLHypotheses & ITRHypotheses, JBool RecalculApport = false);
	// Avant JVoid OnMakeEchelles(const JATPEchelleSupports & EchelleSupports, const JATPKEYEtude & KEYEtude, const JATPKEYPeriodeAction & KEYPeriodeAction, JUnt32 IdItem, JITPTBLEchelle & ITREchelle) const;
	
	//permet de recalculer les tarif des insertions
	JVoid UpdateTarifs();
	bool UpdateTarifsPlan(CPlanInsertions & plan);

	// la fonction pour trier les échelles
	JVoid OnSortEchelles(const JATPAFFEchelle & AFFEchelle, const JITPTBLEchelle & ITREchelle, JATPEchelleSupports & EchelleSupports) const;

////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////		DATA		////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////

public:
    // l'étude
	JATPKEYEtude				m_KEYEtude;

	// la période d'action courante (utilisée pour le master plan)
	JATPKEYPeriodeAction		m_KEYPeriodeAction; 

	// la période d'action des hypothèses
	JATPKEYPeriodeAction		m_KEYPeriodeActionHyp; 

	// la période d'action du tablier
	JATPKEYPeriodeAction		m_KEYPeriodeActionTablier; 


	JBool OffreCurSelTerrainCurSelCibleChange();


	//***************************** SELECTIONS ****************************
	//
	// Item Selectionné
	JUnt32						m_IdItemSel;

	// les supports d'échelle
	JATPEchelleSupports			m_EchelleSupports;

	// Reporter dans Doc General // Tarifs manuels en cours (clef d'accés = id support + id format)
	// JMap <CKeyTarifManuel, JFlt32>	m_MapTarifManuel; 
	
	// les supports selectionnés via echelle
	JATPPlateauSupports			m_PlateauSupports;

	// les supports plateau sur toute la durée de l'étude
	JATPPlateauSupports			m_PlateauSupportsPlanEtendu;

	//***************************** INFO HYPOTHESES ****************************
	//
	// l'hypothese Actif / Button Down
	JUnt32						m_IdxHypotheseActif;	

	// l'hypothèse Courant = Period Action
	JUnt32						m_IdxHypotheseCourant;	

	// Map des hypothèses Affichées
	CATPMapAffHypo				m_MapAffHypothese;


	// Les Plans insertions Hypotheses
	LIST_HYPOTHESES				m_TblHypotheses;		

	// Le Master Plan
	CPlanInsertions				m_MasterPlan;			

	// Les plans archivés
	LIST_PLANMASTERS			m_TblPlanArchive;		

	// Nombre maximum d'hypotheses
	const JUnt32				m_MaxHypothese;			

	

	//***************************** AFFICHAGE ****************************
	//
	JATPAFFResultPlan		m_AFFResultPlan;		// Le result plan + Graph
	// JBool					m_AFFCtxLecture;		// l'état d'affichage des contextes de lecture
	JATPAFFCreuset			m_AFFCreuset;			// le creuset
	JATPAFFEchelle			m_AFFEchelle;			// Affichage Echelle
	JBool					m_AFFPlanEtendu;		// Affichage du Plan Période
	JBool					m_AFFPlanMaitre;		// Affichage du Plan maitre
	JBool					m_AFFChromie;			// Affichage de la chromie sur les insertions
	JBool					m_AFFStatutInsert;		// Affichage du statut des insertions
	JBool					m_AFFCourbeReponse;		// l'état d'affichage des courbes de réponse

	// Element onglets sous résultats hypothèses
	JBool					m_AFFDistributionN;		// Affichage Distribution nette
	JBool					m_AFFDistributionC;		// Affichage Distribution cumulée
	JATPAFFVentilation		m_AFFVentilation;		// Affichage Ventilation
	JBool					m_AFFMemoDememo;		// Affichage Memo-Dememo

	JBool					m_AFFMargeErreur;		// Affichage Marge d'erreur

	JBool					m_AFFColonneTotal;		// Affichage Colonne Total

	JATPAFFZoomPlan			m_AFFZoomPlan;			// le zoom Plan
	JATPAFFContrib			m_AFFContribution;		// Affichage Contribution
	JATPAFFContrib_New		m_AFFContribution_New;	// Affichage Contribution

	JBool					m_AFFPlateauGrille;		// Option plateau creuset plan
	
	JBool					m_AFFOffreHypos;		// Option Offre creuset hypo
	JBool					m_AFFPlateauHypos;		// Option Plateau creuset hypo

	JInt32					m_ZoomGrille;			//Largeur des cellules de la grille calendaire

	// Gestion liste des formats disponibles
	JInt32					m_DecalAffichFormat;	// indice de décalage pour affichage format

	// L'etat de l'affichage des couplage (Plateau)
	JMap<JUnt32,JBool>		m_MapIdCouplageExpanded;

	//###MOTEUR###///

	JITPTBLHypotheses		m_ITRHypotheses;	// les données des hypothèses (Map de DATHypo)
	JITPTBLEchelle			m_ITREchelle;		// les données de l'échelle

	struct Progress{
		int m_Range;
		int m_Step;
	} m_Progress;

	JBool m_bCalculsDiff;

	JFlt64 m_PrctApport;

	CPlanInsertions m_PlanClipBoard;
	JBool m_bSomethingToPaste;

	JMap<JInt32,JInt32> m_mapSubstFormat;

	JList<JUnt32> m_listFormatsQP;

	bool SubstitutFormat(JInt32 idSupport, JInt32 & idFormat);
    void SetSubstitutFormat(JInt32 idSupport, JInt32 idFormat = 0);
};

// Rappel sélection
// Etat 0  : non sélectionné
// Etat 1  : sélectionné pour mode insertion
// Etat 2  : affichage uniquement
// Etat 3  : mode insertion et affiché
#define ETAT_FORMAT_NOSEL	0x0000
#define ETAT_FORMAT_SEL		0x0001
#define ETAT_FORMAT_AFF		0x0002
#define ETAT_FORMAT_AFFSEL	0x0003




// fin de l'inclusion conditionnelle
#endif
