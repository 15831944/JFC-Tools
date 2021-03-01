/*
//	JFC Informatique et Média
//	2004
//
//	Auteur:
//	Jérémy DEVILERS
//
//
//	Classe:
//	CMoteurPresse
//
//	Descriptif:
//	Moteur d'évaluation presse, encapsule les classe de JMTFR03 (librarie moteur de Sylvain)
*/

#pragma once

#include "KeyMoteur.h"
#include "KeyTerrain.h"
#include "JFCDistribution.h"
#include "JFCMaille.h"

// Everything for the Tables
#include "JSrcPresse.h"
// Everything from Supports
#include "AllTypeOfNode.h"
// Everything from Items
#include "ItemsConstruits.h"
// Everything  for Source
#include "JMoteurTerrainManager.h"
//Offre
#include "JATPOffre.h"
//Moteur
#include ".\MoteurPresse\Include\JMTFR03.h"


class AFX_EXT_CLASS CMoteurPresse
{

public:
	enum TYPEMOTEUR{ABAQUE = 0, ABAQUE_ETAL, PROBA};
	CMoteurPresse(void);
	~CMoteurPresse(void);

	//Les méthodes d'initialisation
	
	//Permet d'initialiser le moteur en une passe
	bool Init(JATPOffre &offre, JUnt32 filtreAud, JBool bLDP, JBool bLNM, JBool bLDP30J, JBool bReprise );//ok

	//Si l'offre a changé : terrain, support, cible, affinité
	bool SetOffre(JATPOffre& offre, JUnt32 filtreAud);
	bool SetOffre(JATPOffre& offre, JUnt32 filtreAud, JMap<JUnt32,JUnt32> *MapIdSegPop);

	//si une option a changé 
	bool SetOptions(JATPOffre& offre, JBool bLDP, JBool bLNM, JBool bLDP30J, JBool bReprise);
	bool CanLDPQuot(CKeyTerrain key) const;

	bool HasFiltre(CKeyTerrain key, JInt32 idFiltre) const;

	bool SetFiltreAudience(JATPOffre& offre, JUnt32 filtreAud);


	//Change le type de moteur utilisé
	void SetType(TYPEMOTEUR type);
	int GetType() const;

	bool CanAbaque(CKeyTerrain key) const;

	//Les méthodes de calcul
	bool GetEffCas(CKeyMoteur key, JFlt64 &effectif, JInt32 &nbcas) const;
	
	JFlt64 GetEffectif(CKeyMoteur key) const; //-1.0 si non trouvé
	JInt32 GetNbCas(CKeyMoteur key) const; //-1 si non trouvé

	bool CalcAudience(CKeyMoteur key, JInt32 idSupport, JFlt64 & VolLecture, JFlt64 & AudienceRef, JFlt64 & AudienceProba, JInt32 & PtEtalonnage, bool bDuree = false) const;

	bool SetMailleTemporelle(CKeyMoteur key, JFCMaille & maille, JInt32 idSupport, JUnt32 nbInsert, JDate date) const;
	bool SetMailleEchelle(CKeyMoteur key, JFCMaille & maille, JInt32 idSupport, JUnt32 nbInsert, bool bSuperpose = false) const;
	
	bool BuildDistributions(CKeyMoteur key, JFCMaille & maille, bool bNoDistrib = false);

	static JFCDistribution GetDistribution(CMoteurPresse &presse, JFlt64 VolLecture);

	// Nouvelle fonction pour calcul des distributions(1) avec n insertions 
	// soit distri(1).item(1) avec 1 insertion
	// soit distri(2).item(1) avec 2 insertions etc jusqu'à n 
	bool BuildTabDistributions(CKeyMoteur key, JFCMaille & maille1Ins, JUnt32 &NbInsertions);
	// static GetTabDistribution(CMoteurPresse &presse, JUnt32 &NbInsert, JArray <JFCDistribution> &TDistribution); 
	void GetTabDistribution(CMoteurPresse &presse, JUnt32 &NbInsert, JArray <JFCDistribution> &TDistribution, JUnt32 &LimitNbInsert);

	bool IsReady() const{
		return m_bInit;
	};

	void GetCoefAlpha(CKeyTerrain key, JInt32 SupportId, JInt32 & dureeVie, JVector<JInt32>& vect, JInt32 & diviseur);

	//void SetCibles

	bool AddCibleVentilation(JATPOffre& offre, JList<IItem*> &lst);

	static bool CanDescribeCouplage(JUnt32 idCouplage, CKeyTerrain key);
	static bool CanDescribeFamille(JUnt32 IdFamille, CKeyTerrain key);
	static bool CanUseSupport(IElem* pElem, CKeyTerrain key, JATPOffre& offre);

	// Positionne les pondérations selon la cible active (à 1 si cible de l'offre)
	void SetPonderations(JUnt32 IdCible, JUnt32 IdSource, JUnt32 IdTerrain);

	// Vérifie si la cible est valide sur une liste de terrains
	bool CiblePondAutorise(JUnt32 IdCiblePond, JUnt32 IdSource, JMap <JUnt32, JUnt32> *pMapIdSelTerrain);

	// Indique au moteur la cible courante
	void SetItemCourant(JUnt32 IdCible) {m_ItemCourant = IdCible; };

private:
	bool CanUseSupportInt(IElem* pElem, CKeyTerrain key) const;

	bool AddTerrain(JUnt32 IDSource, JUnt32 IDTerrain);//ok

	bool MakeCibles(JATPOffre& offre, JUnt32 IDSource, JUnt32 IDTerrain);

	// Ajout d'une cible pour le calcul du moteur
	bool AddCible(JMTFR03KEYCible kCible, CItemConstruit* item, JBool ItemAutorise = true);

	// Fabrique les cibles pondérées existantes
	bool MakeCiblesPonderes(JList <JUnt32> & LstItemsPonderes, JUnt32 IDSource, JUnt32 IDTerrain);

	// Ajout de la cible pour les calculs
	bool AddCiblePonderee(JMTFR03KEYCible KeyCiblePond, CItemPondere * pItemPond);

	bool AddVentil(CCalcItemParams &Param1, JMTFR03KEYCible kCible, CItemConstruit* item);

	// Appellé après un changement d'option
	bool MakeValues(JATPOffre& offre, JUnt32 IdSource, JUnt32 IdTerrain, JBool CalculPonderation = true); // = MakeEffectifs + MakeContacts

	bool MakeEffectifs(JUnt32 IdSource, JUnt32 IdTerrain);//ok
	bool AddEffectif(CKeyMoteur key, JBool CalculPonderation = true);//ok

	bool MakeContacts(JATPOffre& offre, JUnt32 IdSource, JUnt32 IdTerrain);//ok
	bool CalcContacts(JATPOffre& offre, CKeyMoteur key);
	bool AddContact(CKeyMoteur key, JUnt32 IDSupport);

	////
	bool CalcAudienceCouplage(JMTFR03KEYAudience kAudience, IElem* pElem, JFlt64 & VolLecture, JFlt64 & AudienceRef, JFlt64 & AudienceProba, JInt32 & PtEtalonnage, bool bDuree = false) const;
	bool CalcAudienceFamille(JMTFR03KEYAudience kAudience, IElem* pElem, JFlt64 & VolLecture, JFlt64 & AudienceRef, JFlt64 & AudienceProba, JInt32 & PtEtalonnage, bool bDuree) const;
	bool CalcAudienceMediaWeight(JMTFR03KEYAudience kAudience, IElem* pElem, JFlt64 & VolLecture, JFlt64 & AudienceRef, JFlt64 & AudienceProba, JInt32 & PtEtalonnage, bool bDuree = false) const;
	bool CalcAudienceTitre(JMTFR03KEYAudience kAudience, IElem* pElem, JFlt64 & VolLecture, JFlt64 & AudienceRef, JFlt64 & AudienceProba, JInt32 & PtEtalonnage, bool bDuree = false) const;

private:

	JMTFR03TBLCibles		m_TblCibles;		// La table des cibles
	JMTFR03TBLEffectifs		m_TblEffectifs;		// La table des effectifs
	JMTFR03TBLAudiences		m_TblAudiences;		// La table des audiences

	JMTFR03TBLModulations	m_TblModulations;	// La table des filtres (Contextes, Filtres audience, Segments, Types poids)

	JMTFR03Options m_Options;					// les options du moteur
	TYPEMOTEUR m_type;							// le type de moteur

	JMTFR03BetaBinomial m_BetaBinomial;
	JMTFR03FullBinomial m_FullBinomial;

	JMTFR03FullBinomialCum m_FullBinomialCum;

	// Pour test si moteur initialisé
	bool m_bInit;

	bool ctxSelected;

	JMoteurTerrainManager * m_TerrainManager;

	// Identifiant de la cible en cours de sélection
	JUnt32 m_ItemCourant;

public:
	// Voir avec Jeremy -- Récupération des différentes tables
	const JMTFR03TBLTerrains		& GetTblTerrains() const;
	const JMTFR03TBLCibles			& GetTblCibles() const  ;
	const JMTFR03TBLModulations		& GetTblModulations() const;
	const JMTFR03Options			& GetOptions() const;

	JMTFR03TBLTerrains		& GetTblTerrains();

};
