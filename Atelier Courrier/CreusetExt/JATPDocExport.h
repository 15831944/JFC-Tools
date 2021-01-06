#pragma once

// on inclut les d�finitions n�cessaires
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

		// Init Dur�e de parution insertion
		m_Duree		= -1;

		// Init Tarif insertion (� la date de parution)
		m_Tarif		= -1;

		// Init Nombre de contact insertion
		m_NbContact = -1;	

		// Init GRP insertion
		m_GRP		= -1;

		// Init Co�t GRP insertion
		m_CoutGRP	= -1;

		// Init Contribution
		m_Contribution = -1;

		// Nb Insertion
		m_NbInsertion = 0;
	}

	// Date de parution insertion
	JDate	m_DateParution;

	// Dur�e de parution insertion
	JUnt32	m_Duree;

	// Tarif insertion (� la date de parution)
	JInt32	m_Tarif;

	// Nombre de contact insertion
	JFlt64 m_NbContact;	

	// GRP insertion
	JFlt64 m_GRP;

	// Co�t GRP insertion
	JFlt64 m_CoutGRP;

    // Contribution Insertion
	JFlt64 m_Contribution;

	// Nb Insertion
	JInt32 m_NbInsertion;

	// Les exports
	JBool Accept(CATPIVisitor *pIVisitor);
};

/////////////////////////////////// Classe Infos Supports Hypoth�se //////////////////////////////////////
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
		m_LblAbregeFormat	= "";		// Label abr�g� du format

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
		m_LblAbregeFormat	= rhs.m_LblAbregeFormat;	// Label abr�g� du format

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
	JString		m_LblAbregeFormat;	// Label abr�g� du format

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

/////////////////////////////////////// Classe Infos Hypoth�se //////////////////////////////////////
class CInfoHypo
{
 public:
 	// Constructor
 	CInfoHypo()
 	{
 		// Initialisation de tous les elmts
		m_TInfoSupports.Reset();		// Tableau des infos supports 
		m_LabelHypo			= "";		// Libell� hypoth�se				
		m_TotInsertions		= 0;		// Total insertions
		m_BudgetBrut		= 0.0;		// Budget brut
		m_Audience			= -1;		// Nombre d'individus touch�s (en milliers) ou Audience
		m_ContactActif		= -1;		// Nombre total de contacts (en milliers)
		m_Grp				= -1;		// Grp
		m_GrpActif			= -1;		// Grp Actif
		m_Couverture		= -1;		// Couverture
		m_Repetition		= -1;		// R�p�tition
		m_RepetitionPlus	= -1;		// R�p�tition[+]
		m_CouvMemo			= -1;		// Couverture m�moris�e	
		m_CoutGRP			= -1;		// Co�t GRP
		m_CoutCouv			= -1;		// Co�t/%couv
		m_Cout1000Indiv		= -1;		// Co�t/1000 individus
		m_Cout1000Contact	= -1;		// Co�t/1000 Contacts
		m_DistriCum.Reset();			// Distribution cumul�e
		m_DistriNette.Reset();			// Distribution nette
		m_ValCouv.Reset();				// Init Valeurs graphe couverture		 
		m_ValGRP.Reset();				// Init Valeurs graphe GRP

		// String export
		m_StrExportHypo = "";	
 	}
	
	// Liste des infos supports pour l'hypoth�se
	JList  <CInfoSupport> m_TInfoSupports;

	// R�sultats hypoth�ses
	JString m_LabelHypo;				// Label Hypoth�se
	JUnt32	m_TotInsertions;			// Total insertions
	JFlt64  m_BudgetBrut;				// Budget brut
	JUnt32  m_Audience;					// Nombre d'individus touch�s (en milliers) ou Audience
	JFlt64  m_ContactActif;				// Nombre total de contacts (en milliers)
	JFlt64  m_Grp;						// Grp
	JFlt64  m_GrpActif;					// Grp Actif
	JFlt64  m_Couverture;				// Couverture
	JFlt64  m_Repetition;				// R�p�tition
	JFlt64  m_RepetitionPlus;			// R�p�tition[+]
	JFlt64  m_CouvMemo;					// Couverture m�moris�e	
	JFlt64  m_CoutGRP;					// Co�t GRP
	JFlt64  m_CoutCouv;					// Co�t/%couv
	JFlt64  m_Cout1000Indiv;			// Co�t/1000 individus
	JFlt64  m_Cout1000Contact;			// Co�t/1000 Contacts
	JArray <JFlt64> m_DistriCum;		// Distribution cumul�e
	JArray <JFlt64> m_DistriNette;		// Distribution nette
	JList  <JFlt64> m_ValCouv;			// Valeur Couverture pour graphe couverture
	JList  <JFlt64> m_ValGRP;			// Valeur GRP pour graphe GRP

	JITPDATHypoFormat * m_expDATHypoFormat;
	// Les exports
	JBool Accept(CATPIVisitor *pIVisitor,JUnt32 IndexHypo, JATPDocExport * pDocExport)
	{
		// Export d�but hypoth�se
		pIVisitor->VisitBegin(this,IndexHypo, pDocExport);

		// Export corps hypoth�ses
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
		m_LabelPeriodicite		= "";   // P�riodicit� Support
		m_LabelAbrPeriodicite	= "";	// P�riodicit� abr�g� support
		m_Tarif					= -1;	// Tarif support d�but p�riode
		m_LDPLNM				= -1;	// LDP/LNM Support
		m_GRPActifValid         = true;	// Validit� du GRP actif
		m_GRPActif				= -1;	// GRP Actif Support
		m_GRP					= -1;	// GRP Support
		m_Affinite				= -1;	// Affinite Support
		m_PrctAffinite			= -1;	// % Affinit� support
		m_CoutGRP				= -1;	// Co�t GRP Support
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

	// P�riodicit� Support
	JString m_LabelPeriodicite;

	// P�riodicit� abr�g� support
	JString m_LabelAbrPeriodicite;

	// Tarif support d�but p�riode
	JFlt32	m_Tarif;
	
	// LDP/LNM Support
	JFlt64 m_LDPLNM;

	// Validit� du GRP Actif
	JBool m_GRPActifValid;

	// GRP Actif Support
	JFlt64 m_GRPActif;

	// GRP Support
	JFlt64 m_GRP;

	// Affinite Support
	JFlt64 m_Affinite;

	// % Affinit� support
	JFlt64 m_PrctAffinite;

	// Co�t GRP Support
	JFlt64 m_CoutGRP;

	// Reprises support
	JFlt64 m_Reprise;

	// Audience Support
	JFlt64 m_Audience;

	// Contacts Support
	JFlt64 m_Contact;

	// Apport Support
	JFlt64 m_Apport;

	// Les exports �chelle
	JBool Accept(CATPIVisitor *pIVisitor)
	{
		// Export d�but hypoth�se
		pIVisitor->VisitBegin(this);

		// Export corps hypoth�ses
		pIVisitor->Visit(this);

		pIVisitor->VisitEnd(this);

		return true;
	}

	// Buffer export �chelle
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

	// L'export �chelle
	virtual JBool AcceptEch(CATPIVisitor *pIVisitor);

	// String Export
	CString m_StrExport;

public:
	// Les informations n�cessaires � l'export Plans calendaires et Plans hypoth�ses
	//
	// le document
	JATPDocCreuset & m_ATPDocCreuset;

	// Nom de l'�tude
	JString m_LabelEtude;

	// Nom groupe associ�
	JString m_LabelGroupe;

	// Code groupe
	JString m_CodeGroupe;

	// Nom annonceur associ�
	JString m_LabelAnnonceur;

	// Code annonceur
	JString m_CodeAnnonceur;

	// Nom produit associ�
	JString m_LabelProduit;

	// Code produit
	JString m_CodeProduit;

	// P�riode globale
	JDate	m_DateDebPeriode;
	JDate   m_DateFinPeriode;

	// P�riode d'action
	JDate	m_DateDebPeriodeAction;
	JDate   m_DateFinPeriodeAction;

	// Nom de la cible courante
	JString m_LabelCible;

	// Effectif et �chantillon cible courante
	JUnt32  m_EffectifCible;
	JUnt32  m_EchantillonCible;
	JBool   m_GRPActifValid;

	// Nom r�f�rence affinit�
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

	// Nom m�thode de calculs couverture
	JString m_LabelMethodeCalcCouv;

	// Liste des formats actifs
	JList <JUnt32> m_ListFormatActif;

	// Les infos hypoth�ses
	JArray <CInfoHypo> m_TInfoHypos;

	// les infos �chelles
	JList <CInfoEchSupport> m_InfoEchelle;

private:

	// R�cup�ration des informations g�n�rales
	JBool RecupInfoGene();

	// R�cup Lable Etude
	JVoid GetInfoEtude();

	// Nom et id groupe associ�
	JVoid GetInfoGroupe();

	// Nom et id annonceur associ�
	JVoid GetInfoAnnonceur();

	// Nom produit associ�
	JVoid GetInfoProduit();

	// P�riode globale et p�riode d'action
	JVoid GetInfoPeriodes();
	
	// Nom de la cible
	JVoid GetInfoCible();
	
	// Nom r�f�rence affinit�
	JVoid GetInfoRefAffinite();

	// Nom Filtre d'audience
	JVoid GetInfoFiltreAudience();

	// Nom Source et Terrain
	JVoid GetInfoSourceTerrain();

	// Nom Segment
	JVoid GetInfoSegment();
	
	// Nom type de poids
	JVoid GetInfoTypePoids();
	
	// Nom m�thode de calculs couverture
	JVoid GetInfoMethodeCalcul();

	// R�cup�ration des formats actifs
	JVoid GetFormatActif();

	// R�cup�ration des informations hypoth�ses
	JVoid GetInfoHypotheses();

	// R�cup�ration des infos �chelle
	JVoid GetInfoEchelle();

	// R�cup�ration Valeurs Graphe Couverture
	JVoid RecupValGrapheCouv(JUnt32 NoHypo);

	// R�cup�ration Valeurs Graphe GRP
	JVoid RecupValGrapheGRP(JUnt32 NoHypo);

	// Nombre d'insertions active pour une hypoth�se
	JInt32 OnGetNbInsertionsHyp(JUnt32 IdHypothese) const;

	// Calcul r�sultat pour 1 insertion
	JBool CalculResultatInsertion(CInsertion *pInsertion, JFlt64 &Contact, JFlt64 &GRP);

	JSRCTables* m_srcTables;
	JSRCPresse* m_srcPresse;

};
