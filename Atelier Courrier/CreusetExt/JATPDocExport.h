#pragma once

// on inclut les définitions nécessaires
#include  "JATPDocCreuset.h"
#include  "ATPIVisitor.h"

/////////////////////////////////////// Classe Infos Insertions Support //////////////////////////////////////
class CInfoInsertion
{
public:
	// Constructor
	CInfoInsertion()
	{
		// Init Date de parution insertion
		m_DateParution.Reset();

		// Init Durée de parution insertion
		m_Duree		= -1;

		// Init Tarif insertion (à la date de parution)
		m_Tarif		= -1;

		// Init Nombre de contact insertion
		m_NbContact = -1;	

		// Init GRP insertion
		m_GRP		= -1;

		// Init Coût GRP insertion
		m_CoutGRP	= -1;

		// Init Contribution
		m_Contribution = -1;

		// Nb Insertion
		m_NbInsertion = 0;
	}

	// Date de parution insertion
	JDate	m_DateParution;

	// Durée de parution insertion
	JUnt32	m_Duree;

	// Tarif insertion (à la date de parution)
	JInt32	m_Tarif;

	// Nombre de contact insertion
	JFlt64 m_NbContact;	

	// GRP insertion
	JFlt64 m_GRP;

	// Coût GRP insertion
	JFlt64 m_CoutGRP;

    // Contribution Insertion
	JFlt64 m_Contribution;

	// Nb Insertion
	JInt32 m_NbInsertion;

	// Les exports
	JBool Accept(CATPIVisitor *pIVisitor);
};

/////////////////////////////////// Classe Infos Supports Hypothèse //////////////////////////////////////
class CInfoSupport
{
public:
 	// Constructor
 	CInfoSupport()
 	{
 		// Initialisation de tous les elmts
		m_IdSupport			= 0; 		// Id Support
		m_LblSupport		= "";		// Label Support
  		m_IdParentSupport	= 0;		// Id Parent du support

		// Infos format
		m_IdFormat			= 0;		// Id Format
		m_LblCompletFormat	= "";		// Label complet du format
		m_LblAbregeFormat	= "";		// Label abrégé du format

		// Infos Tarif
		m_Tarif				= 0;		// Tarif

		// Info Nb Insertions
		m_NbInsertions		= 0;		// Nombre insertions pour ce format

		// Info insertions
		m_TInfoInsertions.Reset(); 
 	}

	/// assignment operator
	CInfoSupport & operator =(const CInfoSupport & rhs)
	{
		// Infos support
		m_IdSupport			= rhs.m_IdSupport; 			// Id Support
		m_LblSupport		= rhs.m_LblSupport;			// Label Support
		m_IdParentSupport	= rhs.m_IdParentSupport;	// Id Parent du support

		// Infos format
		m_IdFormat			= rhs.m_IdFormat;			// Id Format
		m_LblCompletFormat	= rhs.m_LblCompletFormat;	// Label complet du format
		m_LblAbregeFormat	= rhs.m_LblAbregeFormat;	// Label abrégé du format

		// Infos Tarif
		m_Tarif				= rhs.m_Tarif;				// Tarif

		// Info Nb Insertions
		m_NbInsertions		= rhs.m_NbInsertions;		// Nombre insertions pour ce format

		// Info Insertions
		m_TInfoInsertions.Reset();
		for (rhs.m_TInfoInsertions.MoveFirst();
			 rhs.m_TInfoInsertions.IsValid();
			 rhs.m_TInfoInsertions.MoveNext())
		{			
			m_TInfoInsertions.AddTail() = rhs.m_TInfoInsertions.GetItem();
		}

		// String export
		m_StrExportSupport = "";	

		return (*this);
	}

	// Infos support
 	JUnt32		m_IdSupport; 		// Id Support
	JString		m_LblSupport;		// Label Support
  	JUnt32		m_IdParentSupport;	// Id Parent du support

	// Infos format
	JUnt32		m_IdFormat;			// Id Format
	JString		m_LblCompletFormat;	// Label complet du format
	JString		m_LblAbregeFormat;	// Label abrégé du format

	// Infos Tarif
	JInt32		m_Tarif;			// Tarif

	// Info Nb Insertions
	JUnt32		m_NbInsertions;		// Nombre insertions pour ce format

	// Info Insertions
	JList  <CInfoInsertion> m_TInfoInsertions;

	// Les exports
	JBool Accept(CATPIVisitor *pIVisitor);

	// String export
	CString m_StrExportSupport;
};

/////////////////////////////////////// Classe Infos Hypothèse //////////////////////////////////////
class CInfoHypo
{
 public:
 	// Constructor
 	CInfoHypo()
 	{
 		// Initialisation de tous les elmts
		m_TInfoSupports.Reset();		// Tableau des infos supports 
		m_LabelHypo			= "";		// Libellé hypothèse				
		m_TotInsertions		= 0;		// Total insertions
		m_BudgetBrut		= 0.0;		// Budget brut
		m_Audience			= -1;		// Nombre d'individus touchés (en milliers) ou Audience
		m_ContactActif		= -1;		// Nombre total de contacts (en milliers)
		m_Grp				= -1;		// Grp
		m_GrpActif			= -1;		// Grp Actif
		m_Couverture		= -1;		// Couverture
		m_Repetition		= -1;		// Répétition
		m_RepetitionPlus	= -1;		// Répétition[+]
		m_CouvMemo			= -1;		// Couverture mémorisée	
		m_CoutGRP			= -1;		// Coût GRP
		m_CoutCouv			= -1;		// Coût/%couv
		m_Cout1000Indiv		= -1;		// Coût/1000 individus
		m_Cout1000Contact	= -1;		// Coût/1000 Contacts
		m_DistriCum.Reset();			// Distribution cumulée
		m_DistriNette.Reset();			// Distribution nette
		m_ValCouv.Reset();				// Init Valeurs graphe couverture		 
		m_ValGRP.Reset();				// Init Valeurs graphe GRP

		// String export
		m_StrExportHypo = "";	
 	}
	
	// Liste des infos supports pour l'hypothèse
	JList  <CInfoSupport> m_TInfoSupports;

	// Résultats hypothèses
	JString m_LabelHypo;				// Label Hypothèse
	JUnt32	m_TotInsertions;			// Total insertions
	JFlt64  m_BudgetBrut;				// Budget brut
	JUnt32  m_Audience;					// Nombre d'individus touchés (en milliers) ou Audience
	JFlt64  m_ContactActif;				// Nombre total de contacts (en milliers)
	JFlt64  m_Grp;						// Grp
	JFlt64  m_GrpActif;					// Grp Actif
	JFlt64  m_Couverture;				// Couverture
	JFlt64  m_Repetition;				// Répétition
	JFlt64  m_RepetitionPlus;			// Répétition[+]
	JFlt64  m_CouvMemo;					// Couverture mémorisée	
	JFlt64  m_CoutGRP;					// Coût GRP
	JFlt64  m_CoutCouv;					// Coût/%couv
	JFlt64  m_Cout1000Indiv;			// Coût/1000 individus
	JFlt64  m_Cout1000Contact;			// Coût/1000 Contacts
	JArray <JFlt64> m_DistriCum;		// Distribution cumulée
	JArray <JFlt64> m_DistriNette;		// Distribution nette
	JList  <JFlt64> m_ValCouv;			// Valeur Couverture pour graphe couverture
	JList  <JFlt64> m_ValGRP;			// Valeur GRP pour graphe GRP

	JITPDATHypoFormat * m_expDATHypoFormat;
	// Les exports
	JBool Accept(CATPIVisitor *pIVisitor,JUnt32 IndexHypo, JATPDocExport * pDocExport)
	{
		// Export début hypothèse
		pIVisitor->VisitBegin(this,IndexHypo, pDocExport);

		// Export corps hypothèses
		pIVisitor->Visit(this,IndexHypo, pDocExport);

		pIVisitor->VisitEnd(this,IndexHypo, pDocExport);

		return true;
	}

	// String export
	CString m_StrExportHypo;

};


class CInfoEchSupport
{
 public:
 	// Constructor
 	CInfoEchSupport()
	{
		m_IdSupport				=  0;	// Ident Support 
		m_LabelSupport			= "";	// Label Support Echelle
		m_LabelPeriodicite		= "";   // Périodicité Support
		m_LabelAbrPeriodicite	= "";	// Périodicité abrégé support
		m_Tarif					= -1;	// Tarif support début période
		m_LDPLNM				= -1;	// LDP/LNM Support
		m_GRPActifValid         = true;	// Validité du GRP actif
		m_GRPActif				= -1;	// GRP Actif Support
		m_GRP					= -1;	// GRP Support
		m_Affinite				= -1;	// Affinite Support
		m_PrctAffinite			= -1;	// % Affinité support
		m_CoutGRP				= -1;	// Coût GRP Support
		m_Reprise				= -1; 	// Reprises support
		m_Audience				= -1;	// Audience Support
		m_Contact				= -1;	// Contacts Support
		m_Apport				= -1;	// Apport Support
		m_StrExportEch			= "";	// Buffer Export Echelle
	}

	// Ident Support 
	JUnt32  m_IdSupport;

	// Label Support Echelle
	JString m_LabelSupport;

	// Périodicité Support
	JString m_LabelPeriodicite;

	// Périodicité abrégé support
	JString m_LabelAbrPeriodicite;

	// Tarif support début période
	JFlt32	m_Tarif;
	
	// LDP/LNM Support
	JFlt64 m_LDPLNM;

	// Validité du GRP Actif
	JBool m_GRPActifValid;

	// GRP Actif Support
	JFlt64 m_GRPActif;

	// GRP Support
	JFlt64 m_GRP;

	// Affinite Support
	JFlt64 m_Affinite;

	// % Affinité support
	JFlt64 m_PrctAffinite;

	// Coût GRP Support
	JFlt64 m_CoutGRP;

	// Reprises support
	JFlt64 m_Reprise;

	// Audience Support
	JFlt64 m_Audience;

	// Contacts Support
	JFlt64 m_Contact;

	// Apport Support
	JFlt64 m_Apport;

	// Les exports échelle
	JBool Accept(CATPIVisitor *pIVisitor)
	{
		// Export début hypothèse
		pIVisitor->VisitBegin(this);

		// Export corps hypothèses
		pIVisitor->Visit(this);

		pIVisitor->VisitEnd(this);

		return true;
	}

	// Buffer export échelle
	CString m_StrExportEch;
};

class JATPDocExport
{
public:

	// le constructeur
	JATPDocExport(JATPDocCreuset & ATPDocCreuset);

	// le destructeur
	~JATPDocExport(void);

	// Type d'export
	enum ExportATP{TESTPLAN, PLANCALENDAIRE};

	// Les exports creuset
	virtual JBool Accept(CATPIVisitor *pIVisitor);

	// L'export échelle
	virtual JBool AcceptEch(CATPIVisitor *pIVisitor);

	// String Export
	CString m_StrExport;

public:
	// Les informations nécessaires à l'export Plans calendaires et Plans hypothèses
	//
	// le document
	JATPDocCreuset & m_ATPDocCreuset;

	// Nom de l'étude
	JString m_LabelEtude;

	// Nom groupe associé
	JString m_LabelGroupe;

	// Code groupe
	JString m_CodeGroupe;

	// Nom annonceur associé
	JString m_LabelAnnonceur;

	// Code annonceur
	JString m_CodeAnnonceur;

	// Nom produit associé
	JString m_LabelProduit;

	// Code produit
	JString m_CodeProduit;

	// Période globale
	JDate	m_DateDebPeriode;
	JDate   m_DateFinPeriode;

	// Période d'action
	JDate	m_DateDebPeriodeAction;
	JDate   m_DateFinPeriodeAction;

	// Nom de la cible courante
	JString m_LabelCible;

	// Effectif et échantillon cible courante
	JUnt32  m_EffectifCible;
	JUnt32  m_EchantillonCible;
	JBool   m_GRPActifValid;

	// Nom référence affinité
	JString m_LabelRefAffinite;

	// Nom Filtre d'audience
	JString m_LabelFiltreAudience;

	// Nom Source
	JString m_LabelSource;

	// Nom Terrain
	JString m_LabelTerrain;

	// Nom Segment
	JString m_LabelSegment;

	// Nom type de poids
	JString m_LabelTypePoids;

	// Nom méthode de calculs couverture
	JString m_LabelMethodeCalcCouv;

	// Liste des formats actifs
	JList <JUnt32> m_ListFormatActif;

	// Les infos hypothèses
	JArray <CInfoHypo> m_TInfoHypos;

	// les infos échelles
	JList <CInfoEchSupport> m_InfoEchelle;

private:

	// Récupération des informations générales
	JBool RecupInfoGene();

	// Récup Lable Etude
	JVoid GetInfoEtude();

	// Nom et id groupe associé
	JVoid GetInfoGroupe();

	// Nom et id annonceur associé
	JVoid GetInfoAnnonceur();

	// Nom produit associé
	JVoid GetInfoProduit();

	// Période globale et période d'action
	JVoid GetInfoPeriodes();
	
	// Nom de la cible
	JVoid GetInfoCible();
	
	// Nom référence affinité
	JVoid GetInfoRefAffinite();

	// Nom Filtre d'audience
	JVoid GetInfoFiltreAudience();

	// Nom Source et Terrain
	JVoid GetInfoSourceTerrain();

	// Nom Segment
	JVoid GetInfoSegment();
	
	// Nom type de poids
	JVoid GetInfoTypePoids();
	
	// Nom méthode de calculs couverture
	JVoid GetInfoMethodeCalcul();

	// Récupération des formats actifs
	JVoid GetFormatActif();

	// Récupération des informations hypothèses
	JVoid GetInfoHypotheses();

	// Récupération des infos échelle
	JVoid GetInfoEchelle();

	// Récupération Valeurs Graphe Couverture
	JVoid RecupValGrapheCouv(JUnt32 NoHypo);

	// Récupération Valeurs Graphe GRP
	JVoid RecupValGrapheGRP(JUnt32 NoHypo);

	// Nombre d'insertions active pour une hypothèse
	JInt32 OnGetNbInsertionsHyp(JUnt32 IdHypothese) const;

	// Calcul résultat pour 1 insertion
	JBool CalculResultatInsertion(CInsertion *pInsertion, JFlt64 &Contact, JFlt64 &GRP);

	JSRCTables* m_srcTables;
	JSRCPresse* m_srcPresse;

};
