//
// Fichier: JATPDocCreuset.h
// Auteur:  Alain Chambard & Eddie Gent
// Date:    18/08/2003
//

#pragma once
// on v�rifie que le fichier n'a pas d�j� �t� inclus
#ifndef JATP_DOC_CREUSET_H

// on d�finit la macro pour �viter les inclusions multiples
#define JATP_DOC_CREUSET_H

// on inclut les d�finitions n�cessaires
#include "JATPDocument.h"
#include "JATPKEYEtude.h"
#include "JATPKEYPeriodeAction.h"
#include "PlanInsertions.h"
#include "Insertion.h"
#include "PlanMasters.h"
#include "JATPEchelleSupports.h"
#include "JATPPlateauSupports.h"


// On inclut les d�finitions sp�cifiques � Creuset
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

	// la fonction pour tester l'�l�ment
	virtual JBool IsValid() const;

	// les fonctions pour s�rialiser l'�l�ment
	virtual JVoid Send(JStream & Stream);
	virtual JVoid Recv(JStream & Stream);

	// la fonction pour permuter l'�l�ment
	virtual JVoid Swap(JATPDocCreuset & Source);

private:
	// l'op�rateur pour recopier l'�l�ment
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

	// Update plateau suppors hypothese suite � nouvelle vague
	JVoid   MajPlateauHypotheseNewVague();

	// R�cup ligne active (plan de test) selon mode visualisation (offre ou plateau)
	JUnt32 OnRecupLigneSuppActif(JUnt32 IdSupport);

	// Recup nb insertions d'un support pour hypoth�se courante
	JUnt32 OnGetNbInsertions(JUnt32 IdSupport,JDate DateDebut, JDate DateFin, JUnt32 IdHypothese);

	JBool OnInsertionSupport(JUnt32 InxSupportPlateau,JDate DateDebut, JDate DateFin, JUnt32 IdHypothese);

	// R�cup nb lignes active du plateau courant
	JUnt32 OnNbLigPlanPlateau(JBool PlanEtendu);

	// Recup nb lignes supports actifs toutes hypoth�ses
	JUnt32 OnNbLigPlanActif();

	JUnt32 GetIndexSuppPlateauViaIndexSuppActif(JUnt32 IndSupportActif);
	
	// R�cup Index Plateau via no ligne plan
	JInt32 OnIndexPlateauViaIndexPlan(JUnt32 IndexSupportPlan);

	

	// Ajoute une nouvelle hypoth�se
	void AddNewHypo();

	// Pour export des informations
	void Accept(CATPIVisitor * pVisitor);

	// la fonction pour supprimer les insertions (formats actifs / p�riode action)
	JVoid OnSuppInsertion(JUnt32 idxHypo, bool FormatActif);

	// la fonction pour supprimer les insertions (toutes hypoth�ses / formats actifs / p�riode action)
	JVoid OnSuppInsertionAllHypo();

	// Suppression d'une hypothese
	JVoid OnSuppHypothese(JUnt32 IdxHypothese);

	// Calcul apport via l'hypoth�se courante
	JVoid CalculApport(const JITPTBLHypotheses & ITRHypotheses, CInsertion & Insertion, JFlt64 & Apport);

	// Ajout des nouveaux supports de l'offre
	JVoid AjouteSupportsPlateau();

public:

	//--------------------------------- R�cup�ration ou positionnement des s�lections ----------------------------------
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
	
	// List des terrains audience s�lectionn�s (Map ou liste de Maps)
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

	// Flag pour le calcul des apports
	JBool						m_ApportARecalculer;

	// Flag pour indiquer si cibles pond�r�es activ�es
	JBool						m_CiblesPondsActives;

	JBool						m_ListSupportsChange;

protected:
	// la fonction pour faire le machin cad???
	virtual JVoid SynchroSupports();

	// la fonction pour pr�parer le contenu du document
	virtual BOOL OnPrepareContents();

	// la fonction pour mettre � jour le document
	virtual VOID OnUpdate(const JFCUpdate & Message);

	// la fonction pour mettre � jour le plateau
	virtual JVoid UpdatePlateau();

private:
	// les fonctions pour v�rifier le document
	JBool OnCheckEtude(const JATPKEYEtude & KEYEtude) const;
	JBool OnCheckPeriodeAction(const JATPKEYEtude & KEYEtude, const JATPKEYPeriodeAction & KEYPeriodeAction) const;
	JBool OnCheckHypotheses(const JATPKEYEtude & KEYEtude, const JATPEchelleSupports & EchelleSupports, const JATPPlateauSupports & PlateauSupports, LIST_HYPOTHESES & PLNHypotheses);

	// Chargement des terrains
	virtual JBool OnLoadTerrains();
	JBool OnChangeTerrains();

	// Chargement des Supports
	virtual JBool OnLoadSupports();
	virtual JBool OnUpdateSupports();

	// Mise � jour du plateau supports selon option Plan Etendu ou pas
	JVoid OnPlateauSupport(JBool OptionPlanEtendu);

	// Chargement des items cibles
	virtual JBool OnLoadItems();

	// Chargement des items cibles pond�r�es
	virtual JBool OnLoadItemsPonderes();

	// Chargement des formats
	virtual JVoid OnLoadFormats();

	//Initialise le moteur avec les terrains et les cibles
	virtual void InitMoteur();  

	// les fonctions pour calculer les interrogations de l'�tude
	JVoid OnMakeHypotheses(const JUnt32 & IdHypothese, const JATPKEYPeriodeAction & KEYPeriodeAction, const JATPKEYPeriodeAction & KEYPeriodeTravail, JUnt32 IdItem, LIST_HYPOTHESES & PLNHypotheses, JITPTBLHypotheses & ITRHypotheses, JBool bCompute) ;
	JVoid OnMakeEchelles(const JATPEchelleSupports & EchelleSupports, const JATPKEYEtude & KEYEtude, const JATPKEYPeriodeAction & KEYPeriodeAction, JUnt32 IdItem, JITPTBLEchelle & ITREchelle, JITPTBLHypotheses & ITRHypotheses, JBool RecalculApport = false);
	// Avant JVoid OnMakeEchelles(const JATPEchelleSupports & EchelleSupports, const JATPKEYEtude & KEYEtude, const JATPKEYPeriodeAction & KEYPeriodeAction, JUnt32 IdItem, JITPTBLEchelle & ITREchelle) const;
	
	//permet de recalculer les tarif des insertions
	JVoid UpdateTarifs();
	bool UpdateTarifsPlan(CPlanInsertions & plan);

	// la fonction pour trier les �chelles
	JVoid OnSortEchelles(const JATPAFFEchelle & AFFEchelle, const JITPTBLEchelle & ITREchelle, JATPEchelleSupports & EchelleSupports) const;

////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////		DATA		////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////

public:
    // l'�tude
	JATPKEYEtude				m_KEYEtude;

	// la p�riode d'action courante (utilis�e pour le master plan)
	JATPKEYPeriodeAction		m_KEYPeriodeAction; 

	// la p�riode d'action des hypoth�ses
	JATPKEYPeriodeAction		m_KEYPeriodeActionHyp; 

	// la p�riode d'action du tablier
	JATPKEYPeriodeAction		m_KEYPeriodeActionTablier; 


	JBool OffreCurSelTerrainCurSelCibleChange();


	//***************************** SELECTIONS ****************************
	//
	// Item Selectionn�
	JUnt32						m_IdItemSel;

	// les supports d'�chelle
	JATPEchelleSupports			m_EchelleSupports;

	// Reporter dans Doc General // Tarifs manuels en cours (clef d'acc�s = id support + id format)
	// JMap <CKeyTarifManuel, JFlt32>	m_MapTarifManuel; 
	
	// les supports selectionn�s via echelle
	JATPPlateauSupports			m_PlateauSupports;

	// les supports plateau sur toute la dur�e de l'�tude
	JATPPlateauSupports			m_PlateauSupportsPlanEtendu;

	//***************************** INFO HYPOTHESES ****************************
	//
	// l'hypothese Actif / Button Down
	JUnt32						m_IdxHypotheseActif;	

	// l'hypoth�se Courant = Period Action
	JUnt32						m_IdxHypotheseCourant;	

	// Map des hypoth�ses Affich�es
	CATPMapAffHypo				m_MapAffHypothese;


	// Les Plans insertions Hypotheses
	LIST_HYPOTHESES				m_TblHypotheses;		

	// Le Master Plan
	CPlanInsertions				m_MasterPlan;			

	// Les plans archiv�s
	LIST_PLANMASTERS			m_TblPlanArchive;		

	// Nombre maximum d'hypotheses
	const JUnt32				m_MaxHypothese;			

	

	//***************************** AFFICHAGE ****************************
	//
	JATPAFFResultPlan		m_AFFResultPlan;		// Le result plan + Graph
	// JBool					m_AFFCtxLecture;		// l'�tat d'affichage des contextes de lecture
	JATPAFFCreuset			m_AFFCreuset;			// le creuset
	JATPAFFEchelle			m_AFFEchelle;			// Affichage Echelle
	JBool					m_AFFPlanEtendu;		// Affichage du Plan P�riode
	JBool					m_AFFPlanMaitre;		// Affichage du Plan maitre
	JBool					m_AFFChromie;			// Affichage de la chromie sur les insertions
	JBool					m_AFFStatutInsert;		// Affichage du statut des insertions
	JBool					m_AFFCourbeReponse;		// l'�tat d'affichage des courbes de r�ponse

	// Element onglets sous r�sultats hypoth�ses
	JBool					m_AFFDistributionN;		// Affichage Distribution nette
	JBool					m_AFFDistributionC;		// Affichage Distribution cumul�e
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
	JInt32					m_DecalAffichFormat;	// indice de d�calage pour affichage format

	// L'etat de l'affichage des couplage (Plateau)
	JMap<JUnt32,JBool>		m_MapIdCouplageExpanded;

	//###MOTEUR###///

	JITPTBLHypotheses		m_ITRHypotheses;	// les donn�es des hypoth�ses (Map de DATHypo)
	JITPTBLEchelle			m_ITREchelle;		// les donn�es de l'�chelle

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

// Rappel s�lection
// Etat 0  : non s�lectionn�
// Etat 1  : s�lectionn� pour mode insertion
// Etat 2  : affichage uniquement
// Etat 3  : mode insertion et affich�
#define ETAT_FORMAT_NOSEL	0x0000
#define ETAT_FORMAT_SEL		0x0001
#define ETAT_FORMAT_AFF		0x0002
#define ETAT_FORMAT_AFFSEL	0x0003




// fin de l'inclusion conditionnelle
#endif
