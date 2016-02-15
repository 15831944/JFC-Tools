// TomyCIMBelge.h: interface for the CTomyCIMBelge class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TOMYCIMBELGE_H__2352D08F_4C5A_4CE1_9E7D_E49986ACECAF__INCLUDED_)
#define AFX_TOMYCIMBELGE_H__2352D08F_4C5A_4CE1_9E7D_E49986ACECAF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "tomy.h"

#ifdef OPTION_REACH_BELGE
	#define NBWEEK (1)
#else
	#ifdef OPTION_ALGERIE
		#define NBWEEK (1)
	#else
		#define NBWEEK (3)
	#endif
#endif
#define NBPATRON (2)

class CTomyCIMBelge : public InterfaceTomy  
{
public:
	class CElement
	{
	public:
		short m_Station;
		int m_JourDate;
		short m_JourNomme;
		short m_NrSousPlan; //0-2
		short m_QHeure;
		double m_GRP; // GRP utilisé pour le calcul du MW
		CProbaSupport * m_PatronEcoute[2][NBWEEK]; // Pointeur vers les 3 patrons d'écoute
		bool m_OptionAncrage;
		short m_NbInsertion; // pour calculer une duplication sur les spots

		CElement ()
		{
			// On initialise le minimum
			m_PatronEcoute[0][0]=NULL;
		};

		virtual ~CElement ()
		{
		};

		bool IsEquivalent(CElement * E)
		{
			if(m_Station!=E->m_Station)return false;
			if(m_JourDate!=E->m_JourDate)return false;
			if(m_JourNomme!=E->m_JourNomme)return false;
			if(m_QHeure!=E->m_QHeure)return false;
			if(m_OptionAncrage!=E->m_OptionAncrage)return false;
			if(m_GRP!=E->m_GRP)return false;
			return true;
		};
	};

	typedef CArray<CElement,CElement&> CtabElement;

	CTomyCIMBelge();
	virtual ~CTomyCIMBelge();

	// Implémentation de l'interface Tomy
	void SetParametres(CTomyParam * param);
	void InitialisePanel(LPCSTR panel,LPCSTR etude);
	void InitialiseCible(int NrCible);
	void InitialiseCible(long * CoeffExCible);
	void InitialiseCibleVentile(long * CoeffExCible,char * NrModaliteDuCritere,int Modalite);

	void InitialiseCibleVentile(int NrCible,int NrCritere1 ,int NrModalite1,int NrCritere2=-1,int NrModalite2=-1);
	int GetNbCas();
	int GetPopulation();
	void InitialiseSpot();
	double LanceEvalDistrib();
	double GetAsymptote();
	int CalculOffre();
	int AddStationOffre(int m_Sta);
	void DelAllStationOffre();
	void CalculOffreW();
	void CalculCouvWi();
	int GetJourDate(int JourDate);
	double GetCouvertureMJ();
	double GetCouvertureWK();
	double GetCouvertureY();
	void SetNbSemMax(short Nbs);
	void SetNbSemPlan(short NbsP);
	short GetNbSemMax();
	short GetNbSemPlan();
	short *GetTabSemAct();
	void SetCouvertureWi(double *CouvWi);
	double *GetCouvertureWi();
	double fZeroFrequence(double x,double y,short z);
	double seCouverturePlan(double CouvM,double CouvW,double *CouvWi,double CouvY,short NbSem);
	double * seDistributPlan(double CouvSE,double GrpM,long NbSp);
	double seResDistributPlan(double *TabDist,long NbSpot);
	double CalculGRPDansPlan();
	long CalculNbSpotDansPlan();
 
#ifdef OPTION_DUREEECOUTE
	void AddSpot(short Station, int JourDate,short JourNomme,short QHeure,double GRP,short NbInsertion);
	void AddEvenement(short Station, int JourDate,short JourNomme,short HoraireDebut,short Duree,double GRP);
	void UpdateSpot(short Station, int JourDate, short JourNomme,short QHeure,double GRP,short NbInsertion);
#else
	void AddSpot(short Station,short JourNomme,short QHeure,double GRP,short NbInsertion=1);
	void UpdateSpot(short Station, short JourNomme,short QHeure,double GRP,short NbInsertion);
#endif

	int GetNbSpot();
	void SetNbInsertions(long NbSpot);
	long GetNbInsertions();
	double GetCouverture();
	void SetCouverture(double Couv);
	double GetCouvertureValideBeta();
	double GetCouvertureValideMinMax();
	double GetGRP();
	void SetGRP(double GRPjob);
	int GetNbClasseResultat();
	double GetExactementTouche(int categorie);
	double GetCourbeReponse(int nbfoistouche);
	void ActiveFiltreIndividusStrateConsomation(int NrStrate);
	double LanceEval();
	void SavePlanBeforeZoom();
	void RestorePlanAfterZoom();
	// ----------------------------------------

protected:
	CFicProba * LoadPanel(LPCSTR Panel,LPCSTR Etude);
	int LoadCible();
	void FreeAllocIndiv();
	void FreePanel();
	void AllocIndivPourPanel();
	void CalculPopulation();
	int TrouvePatronFrequentation(CTomyCIMBelge::CElement & E);
	void CalculFrequentationComptage(CTomyCIMBelge::CElement & E);
	double CalculMoyenne6Distribution(CDistribution &ResultatDistribution,int NbPatron); // Retourn le GRP
	double CalculMoyenne6DistributionSurPatron(CDistribution &ResultatDistribution,int patron); // Retourn le GRP

	double CalculDistribution(CDistribution &Distrib,int s1,int s2,int s3,int patron); // Retourn le GRP
	void CalculMoyenne2Distribution(CDistribution &ResultatDistribution,CDistribution &Resultat1,CDistribution &Resultat2,int NbPatron);

private:
	static CPanelLoader m_PanelLoader;

private:
	CTomyParam m_Param;
	CString m_CodePanel;
	CString m_Etude;
	CFicProba * m_FicProba;
	int m_NbIndiv;
	int m_NrCible;
	double * m_CoeffEx; //[m_NbIndiv] 
	CMap<short,short,int,int> m_IndexStation;
	int m_NbCas;
	double m_Population;
	CtabElement m_Plan;
	long m_NbInsertions;
	double m_GRP,m_Couverture;
	short **** m_NbHit; // [indiv][sous plan][semaine][patron]
	short *m_PlStation;	// Si la station du panel actif dans le plan
	CDistribution m_Distribution;
	CtabElement * m_PlanWhileZoom; // Backup du plan marginal pendant le zoom
	short m_NbSemMax,m_NbSemPlan;
	short *m_TabSemAct;
	double m_CouvertureMJ;		// Couverture cumulée apres 1 jour moyen dupanel
	double m_CouvertureWK;		// Couverture cumulée après une semaine du panel
	double m_CouvertureY;		// Couverture maximale
	double *m_CouvertureWi;
};

#endif // !defined(AFX_TOMYCIMBELGE_H__2352D08F_4C5A_4CE1_9E7D_E49986ACECAF__INCLUDED_)
