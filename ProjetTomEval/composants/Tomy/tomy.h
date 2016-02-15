// Tomy.h: interface for the CTomy class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TOMY_H__2725D420_7196_11D3_989D_9A200D61B026__INCLUDED_)
#define AFX_TOMY_H__2725D420_7196_11D3_989D_9A200D61B026__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "panelloader.h"
#include "distributioncroisee.h"


#define BASEPROBA (1000)
#define MWMAX (5)
// EN FRANCE
#define NBCLASSEDEGAU (20)
// EN ITALIE
//#define NBCLASSEDEGAU (30)


//##ModelId=3D9AB7D001B6
class CTomyCallBack{
public:
	//##ModelId=3D9AB7D001C6
	virtual void OneSpotEvaluated(){};
};

//##ModelId=3D9AB7D00299
class CTomyParam{
public:
	//##ModelId=3D9AB7D002A3
	CString m_CheminData;
	//##ModelId=3D9AB7D002FD
	int m_DisableBisource; // 1 calcul monosource
	//##ModelId=3D9AB7D00307
	int m_MethodeComptage; // 1 calcul par comptage sur le seuil de vision
	//##ModelId=3D9AB7D00308
	double m_SeuilDeVision;
	//##ModelId=3D9AB7D00311
	int m_TypeSupport; //0 quart d'heure 1:Ecran 2:Emission
	//##ModelId=3D9AB7D0031B
	int m_HoraireDebut; // Debut de journée
	//##ModelId=3D9AB7D0031C
	int m_PhasageHoraire; // Phasage horaire
	//##ModelId=3D9AB7D00325
	int m_NbClassesResultatDistribution; // 10 par défaut
	//##ModelId=3D9AB7D0032F
	int m_ValideBeta; // à renseigner pour GetCouvertureValideBeta()
	//##ModelId=3D9AB7D00330
	int m_ValideMini; // à renseigner pour GetCouvertureValideMinMax()
	//##ModelId=3D9AB7D00339
	int m_ValideMaxi; // à renseigner pour GetCouvertureValideMinMax()
	//##ModelId=3D9AB7D0033A
	double m_CoeffCHI; // Coeff de Correction Hypothèse indépendance (activé uniquement en mode écran)
	//##ModelId=3D9AB7D00343
	double m_CoeffDuplication; // Coeff de dupplication (lorsque NbInsertion>0);
	//##ModelId=3D9AB7D0034D
	int m_NbRatioConsomation; // Nombre de ratios de consomation (0 inactif)
	//##ModelId=3D9AB7D0036C
	CTomyCallBack * m_CallBack;

public:
	//##ModelId=3D9AB7D00375
	double GetCoeffDuplication();
	//##ModelId=3D9AB7D00376
	double GetCorrectionHypotheseIndependance();
	//##ModelId=3D9AB7D0037F
	CTomyParam()
	{
		m_DisableBisource=0;
		m_MethodeComptage=0;
		m_SeuilDeVision=50;
		m_TypeSupport=0;
		m_HoraireDebut=300;
		m_PhasageHoraire=0;
		m_CallBack=NULL;
		m_NbClassesResultatDistribution=10; // pour calculer de 1 à 10+
		m_ValideBeta=10;
		m_ValideMini=2;
		m_ValideMaxi=5;
		m_CoeffCHI=+0.2; // 20 % en mode écran seulement (et méthode probabiliste)
		m_CoeffDuplication=+.95; // 95% de duplication en mode écran seulement (et méthode probabiliste)
		m_NbRatioConsomation=0;
	};

	//##ModelId=3D9AB7D00380
	CTomyParam & operator=(const CTomyParam & Data){
		m_CheminData=Data.m_CheminData;
		m_DisableBisource=Data.m_DisableBisource;
		m_MethodeComptage=Data.m_MethodeComptage;
		m_SeuilDeVision=Data.m_SeuilDeVision;
		m_TypeSupport=Data.m_TypeSupport;
		m_HoraireDebut=Data.m_HoraireDebut;
		m_CallBack=Data.m_CallBack;
				
		if(m_NbClassesResultatDistribution>NBCLASSEDEGAU)
		{
			CString txt;
			txt.Format("m_NbClassesResultatDistribution est limité à %d",NBCLASSEDEGAU);
			AfxMessageBox(txt);
		}
		else
			m_NbClassesResultatDistribution=Data.m_NbClassesResultatDistribution;
		m_ValideBeta=Data.m_ValideBeta;
		m_ValideMini=Data.m_ValideMini;
		ASSERT(m_ValideMini<=m_NbClassesResultatDistribution);
		m_ValideMaxi=Data.m_ValideMaxi;
		ASSERT(m_ValideMaxi<=m_NbClassesResultatDistribution);
		m_NbRatioConsomation=Data.m_NbRatioConsomation;
		switch(m_TypeSupport)
		{
			case 0:
				// Le phasage est donné en nombre de quart d'heure
				m_PhasageHoraire=m_HoraireDebut-300;
				m_PhasageHoraire/=100;
				m_PhasageHoraire*=4;
			break;
			case 1:
				// Le phasage est donné en nombre d'heure * 100
				m_PhasageHoraire=m_HoraireDebut-300;
			break;
			case 2:
				m_PhasageHoraire=0;
			break;
			default: ASSERT(0);
		}

		return *this;
	};
};

class InterfaceTomy  
{
public:
	//InterfaceTomy();
	//virtual ~InterfaceTomy();
	virtual void SetParametres(CTomyParam * param)=0;
	virtual	void InitialisePanel(LPCSTR panel,LPCSTR etude,int NbIndiv=0 )=0;
	virtual	void InitialiseCible(int NrCible)=0;
	virtual void InitialiseCibleVentile(int NrCible,int NrCritere1 ,int NrModalite1,int NrCritere2=-1,int NrModalite2=-1)=0;
	virtual int GetNbCas()=0;
	virtual int GetPopulation()=0;
	virtual void InitialiseSpot()=0;
	virtual double LanceEvalDistrib()=0;
	virtual double GetAsymptote()=0;

#ifdef OPTION_DUREEECOUTE
	virtual void AddSpot(short Station, int JourDate,short JourNomme,short QHeure,double GRP,short NbInsertion)=0;
	virtual void AddEvenement(short Station, int JourDate,short JourNomme,short HoraireDebut,short Duree,double GRP)=0;
#else
	virtual	void AddSpot(short Station,short JourNomme,short QHeure,double GRP,short NbInsertion=1)=0;
	virtual	void UpdateSpot(short Station, short JourNomme,short QHeure,double GRP,short NbInsertion)=0;
	virtual void AddEvenement(short Station, short JourNomme,short HoraireDebut,short Duree,double GRP){};
#endif

	virtual	int GetNbSpot()=0;
	virtual	long GetNbInsertions()=0;
	virtual	double GetCouverture()=0;
	virtual	double GetCouvertureValideBeta()=0;
	virtual	double GetCouvertureValideMinMax()=0;
	virtual double GetGRP()=0;
	virtual int GetNbClasseResultat()=0;
	virtual double GetExactementTouche(int categorie)=0;
	virtual double GetCourbeReponse(int nbfoistouche)=0;
	virtual void ActiveFiltreIndividusStrateConsomation(int NrStrate)=0;
	virtual double LanceEval()=0;
	virtual void SavePlanBeforeZoom()=0;
	virtual void RestorePlanAfterZoom()=0;
};

//##ModelId=3D9AB7D20089
class CTomy : public InterfaceTomy
{
public:
	//##ModelId=3D9AB7D20093
	void RestorePlanAfterZoom();
	//##ModelId=3D9AB7D2009D
	void SavePlanBeforeZoom();
	//##ModelId=3D9AB7D2009E
	double GetExactementTouche(int categorie);
	CDistribution & GetDistributionExactementTouche(){return(m_DistributionExactementTouche);};
	//##ModelId=3D9AB7D200A7
	int GetNbSpot();
	//##ModelId=3D9AB7D200A9
	void SetCallBack(CTomyCallBack * ptr);
	//##ModelId=3D9AB7D200B2
	void FreePanel();
	long GetNbInsertions();


	//##ModelId=3D9AB7D200BC
	int GetJourPanel(int jour);
	//##ModelId=3D9AB7D200C6
	double * GetCopyIndividusNonTouches();
	//##ModelId=3D9AB7D200C7
	double * GetCopyIndividusTouches(CWordArray * TabClasses);
//	double GetCoeffCorrectionSousCibleFrequentation(CWordArray * TabClasses,int cible);
	//##ModelId=3D9AB7D200D0
	void SetFiltreIndividus(double * filtre,char flagExclusifsDupliques);
	//##ModelId=3D9AB7D200D9
	void ActiveFiltreIndividusStrateConsomation(int NrStrate);
	//##ModelId=3D9AB7D200DB
	void SetFiltreIndividusPermanent(double *filtre);
	//##ModelId=3D9AB7D200E4
	double GetGRP();
	//##ModelId=3D9AB7D200E5
	void RestoreCible();
	
	//##ModelId=3D9AB7D200ED
	void InitialiseJourReference(COleDateTime date);
	//##ModelId=3D9AB7D200EF
	void InitialisePanel(LPCSTR panel,LPCSTR etude=_T("MEDIADATA"));
	//##ModelId=3D9AB7D200F9
//	void InitialisePanelMediadataAvecSurEchantillonMediacabsat(LPCSTR panelMediadata,LPCSTR panelMediaCabSat);
	//##ModelId=3D9AB7D20103
	int GetPopulation();
	//##ModelId=3D9AB7D2010B
	int GetNbCas();
	//##ModelId=3D9AB7D2010C
	int GetNbCasPlan();
	//##ModelId=3D9AB7D2010D
	double GetAsymptote();
	//##ModelId=3D9AB7D20115
	double GetCouverture();
	//##ModelId=3D9AB7D20116
	double GetCouvertureValideBeta();
	//##ModelId=3D9AB7D20117
	void CalculValideBeta_RepetitionOptimale(int NbContact);
	//##ModelId=3D9AB7D20120
	double GetCouvertureValideMinMax();
	//##ModelId=3D9AB7D20129
	double GetCourbeReponse(int nbfoistouche);
	//##ModelId=3D9AB7D20133
	int GetNbClasseResultat(){return m_Param.m_NbClassesResultatDistribution;};


	//##ModelId=3D9AB7D20134
	void SetParametres(CTomyParam * param);
	//##ModelId=3D9AB7D2013D
	void InitialiseCible(int NrCible);
	//##ModelId=3D9AB7D2013F
	void InitialiseSousCible(int NrCible,int CritereFiltre,CWordArray * ModaliteFiltre);
	//##ModelId=3D9AB7D2014A
	void InitialiseCibleVentile(int NrCible,int NrCritere1 ,int NrModalite1,int NrCritere2=-1,int NrModalite2=-1);
	//##ModelId=3D9AB7D2015D
	void InitialiseSousCibleVentile(int NrCible,int CritereFiltre,CWordArray * ModaliteFiltre,int NrCritere1 ,int NrModalite1,int NrCritere2=-1,int NrModalite2=-1);
	//##ModelId=3D9AB7D20171
//	void CalculCoeffCorrectionMediaWeightOffreElargie(double PopulationNational,double PopulationOffreElargie);


	//##ModelId=3D9AB7D2017A
	void ChangeGRP(int NrSpot,double GRP);
	//##ModelId=3D9AB7D20184
	double LanceEval();
	//##ModelId=3D9AB7D2018D
	double LanceEvalDistrib();
	//##ModelId=3D9AB7D2018F
	double LanceEvalVentile(int NrCritere1 ,int NrModalite1,int NrCritere2=-1,int NrModalite2=-1);

#ifndef OPTION_DUREEECOUTE
	void UpdateSpot(short Station, short JourNomme,short QHeure,double GRP,short NbInsertion);
	void AddSpot(short Station,short JourNomme,short QHeure,double GRP,short NbInsertion=1);
	void AddEvenement(short Station, short JourNomme,short HoraireDebut,short Duree,double GRP);
#else
	//##ModelId=3D9AB7D201A1
	double CalculSousGRPSpot(short Station, int JourDate,short JourNomme,short QHeure,double GRP);
	//##ModelId=3D9AB7D201AE
	double CalculSousGRPEvenement(short Station, int JourDate,short JourNomme,short HoraireDebut,short Duree,double GRP);
	//##ModelId=3D9AB7D201C1
	void AddSpot(short Station, int JourDate,short JourNomme,short QHeure,double GRP,short NbInsertion=1);
	//##ModelId=3D9AB7D201D5
	void AddEvenement(short Station, int JourDate,short JourNomme,short HoraireDebut,short Duree,double GRP);
#endif
	//##ModelId=3D9AB7D201E8
	void InitialiseSpot();
	//##ModelId=3D9AB7D201F1
//	int GetNbModalite(int NrCritere);

	//##ModelId=3D9AB7D201F3
	CTomy();
	//##ModelId=3D9AB7D201FB
	virtual ~CTomy();

protected:
	void RestoreFiltrePermanent();

	void AllocVecteurBinomiale();
	//##ModelId=3D9AB7D201FE
	CPanelLoader * m_PanelLoader;
	//##ModelId=3D9AB7D20205
	CDistribution m_DistributionExactementTouche;
	//##ModelId=3D9AB7D2020F
	int m_NbClassesCalculDistribution;
	//##ModelId=3D9AB7D20219
	int m_Cmax;
	//##ModelId=3D9AB7D20223
	int m_DecalagePanelJouRef;
	//##ModelId=3D9AB7D2022D
	CString m_CodePanel;
	//##ModelId=3D9AB7D20237
	CString m_Etude;
	//##ModelId=3D9AB7D20238
	int LoadCible();
//	int LoadCibleMediadataAvecSurEchantillonMediacabsat();
	//##ModelId=3D9AB7D2024B
	CFicProba * LoadPanel(LPCSTR Panel,LPCSTR Etude);
	//##ModelId=3D9AB7D20256
	double GetTrusquinNT(short p,double MW);
	//##ModelId=3D9AB7D20260
	void GetTrusquinVB(short p, double MW);
	//##ModelId=3D9AB7D2026B
	double CalculNonTouche();
	//##ModelId=3D9AB7D20273
	double CalculExactementTouche();
	// Les classes privées ****
	
	//##ModelId=3D9AB7D302ED
	class CElement
	{
	public:
		//##ModelId=3D9AB7D302F7
		short m_Station;
		//##ModelId=3D9AB7D30301
		int m_JourDate;
		//##ModelId=3D9AB7D3030B
		short m_JourNomme;
		//##ModelId=3D9AB7D3030C
		short m_QHeure;
		//##ModelId=3D9AB7D30315
		short m_HoraireDebut;
		//##ModelId=3D9AB7D3031F
		short m_Duree;
		//##ModelId=3D9AB7D30320
		double m_GRP; // GRP utilisé pour le calcul du MW
		//##ModelId=3D9AB7D30329
		double m_GRPgrp; // GRP livré par la source de GRP
		//##ModelId=3D9AB7D30334
		CProbaSupport * m_ProbaSupp; // Pointeur vers les probas du support
		//##ModelId=3D9AB7D30338
		bool m_fFreeProba;
		//##ModelId=3D9AB7D3033D
		double m_GRPcouv; // GRP sur la source de couverture
		//##ModelId=3D9AB7D30347
		double m_MW; // MédiaWeight
		//##ModelId=3D9AB7D30348
		short m_NbInsertion; // pour calculer une duplication sur les spots
/*		CElement & operator=(const CElement& Data){
			m_NrStation=Data.m_NrStation;
			m_NrJour=Data.m_NrJour;
			m_NrQHeure=Data.m_NrQHeure;
			return *this;
		}*/
		BYTE m_Media; // Nr Media distribution croisée (0 ou 1) pour TomyFusion

		//##ModelId=3D9AB7D30351
		CElement ()
		{
			// Note: pour gagner du temps, on n'initialise que
			// ces varaibles; les autres seront initialisées dans
			// AddSpot ou AddEvenement.
			m_ProbaSupp=NULL;
			m_fFreeProba=0;
		};

		//##ModelId=3D9AB7D30352
		virtual ~CElement ()
		{
			if(m_fFreeProba)
			{
				delete m_ProbaSupp;
			}
		};

		//##ModelId=3D9AB7D3035B
		bool IsEquivalent(CElement * E)
		{
			if(m_Station!=E->m_Station)return false;
			if(m_JourDate!=E->m_JourDate)return false;
			if(m_JourNomme!=E->m_JourNomme)return false;
			if(m_QHeure!=E->m_QHeure)return false;
			if(m_HoraireDebut!=E->m_HoraireDebut)return false;
			if(m_Duree!=E->m_Duree)return false;
			if(m_GRPgrp!=E->m_GRPgrp)return false;
			return true;
		};
	};

	//##ModelId=3D9AB7D30365
	typedef CArray<CElement,CElement&> CtabElement;


	// Les attributs privés ***
	//##ModelId=3D9AB7D2027D
	void CalculPopulation();
	//##ModelId=3D9AB7D20287
	/*inline*/ void CalculMediaWeight(CElement & E);
	double RecalculGrpElement(CTomy::CElement & E);

	//##ModelId=3D9AB7D20291
	/*inline*/ void CalculMWventile(CElement & E);
	//##ModelId=3D9AB7D2029B
	inline void CalculApport(CElement & E);
	//##ModelId=3D9AB7D202A6
	inline void CalculDuplicationVB(int NbInsertion);
	//##ModelId=3D9AB7D202B0
	inline double CalculDuplicationNT(int NbInsertion,double nt);
	//##ModelId=3D9AB7D202C3
	inline void CalculDistribution(CElement & E);
	//##ModelId=3D9AB7D202CD
	inline void AppliqueCorrectionHypotheseIndependance();

	//##ModelId=3D9AB7D202CE
	inline int TrouveProba(CTomy::CElement & E);
	//##ModelId=3D9AB7D202D9
	void CalculAsymptote(CTomy::CElement & E);
	//##ModelId=3D9AB7D202E3
	char * GetCritere(int NrCritere);
	//##ModelId=3D9AB7D202ED
	void IntegreVentilation(int NrCritere,int NrModalite);
	//##ModelId=3D9AB7D202F8
	void IntegreSousCible(int NrCritere,int NrModalite);
	//##ModelId=3D9AB7D20302
	virtual void FreeAllocIndiv();
	//##ModelId=3D9AB7D200A8
	void FreeVecteurBinomiale();
	//##ModelId=3D9AB7D2030A
	void AllocIndivPourPanel();



	// Les menbres privés ***
	//##ModelId=3D9AB7D20314
	double *m_P; // Probabilité d'être touché en millième
	//##ModelId=3D9AB7D2031E
	double *m_Q; // Probabilité d'être non-touché en millième
	//##ModelId=3D9AB7D20328
	double ** m_Trusquin; // the Trusquin Binomiale
	//##ModelId=3D9AB7D20333
	CtabElement m_Plan;
	//##ModelId=3D9AB7D2033D
	CtabElement * m_PlanWhileZoom; // Backup du plan marginal pendant le zoom

	//##ModelId=3D9AB7D20347
	COleDateTime m_DateDebut;
	//##ModelId=3D9AB7D20350
	int m_NrCible;
	//##ModelId=3D9AB7D2035A
	int m_NrPanel;
	//##ModelId=3D9AB7D20364
	int m_NbIndiv;
	//##ModelId=3D9AB7D2036E
	long * m_CoeffEx; //[m_NbIndividu] 
	//##ModelId=3D9AB7D20382
	long * m_CoeffExCible; // Tampon dans lequel est copié m_CoeffEx pendant une ventilation
	//##ModelId=3D9AB7D2038C
	long * m_CoeffExSousCible; // Sous cible=Cible + filtre modalite


	//##ModelId=3D9AB7D20396
	int m_NbCas;
	//##ModelId=3D9AB7D203A0
	int m_Population;
	//##ModelId=3D9AB7D203AA
	int m_PopulationCibleEntiere; //Utilisé lorsqu'il y a une sous-cible de définie.
	//##ModelId=3D9AB7D203B4
	double m_PopulationSansFiltrePermanent; //Utilisé pour calculer le MédiaWeight
	//##ModelId=3D9AB7D203BE
	//int m_PopulationOffreElargieSansFiltrePermanent; // Utilisé pour calculer le Média Weight d'un support offre elargie en base National

	//##ModelId=3D9AB7D203C8
	double m_RatioPopulation;
	//##ModelId=3D9AB7D203D2
	double * m_NT; // Vecteur par individu des non touchés

	//##ModelId=3D9AB7D203E6
	double ** m_VB; // Vecteur Binomiale par individu
	//##ModelId=3D9AB7D30008
	double * m_VBs; // Vecteur Binomiale du trusquin
	//##ModelId=3D9AB7D30012
	int m_NbVBs; // Plurifréquentation du trusquin
	//##ModelId=3D9AB7D3001C
	int * m_NbVB; // Plurifréquentation des individus

	//##ModelId=3D9AB7D30026
	double * m_FiltreIndividus; // Filtre actif sur les individus
	//##ModelId=3D9AB7D30030
	double * m_FiltreIndividusPermanent; // Filtre permanent sur les individus; fait partie intégrante de la cible de base
	//##ModelId=3D9AB7D3003A
	double m_GRP;
	//##ModelId=3D9AB7D30044
	double m_Couverture;
	//##ModelId=3D9AB7D3004E
	double m_Asymptote;
	//##ModelId=3D9AB7D30063
	CTomyParam m_Param;
	//##ModelId=3D9AB7D3006D
	CFicProba * m_FicProba;

	//##ModelId=3D9AB7D30244
	CMap<short,short,int,int> m_IndexStation;
	//##ModelId=3D9AB7D30263
	CMap<int,int,int,int> m_IndexJour;

	// --------------------------------
	// filtre globale
	//##ModelId=3D9AB7D3026B
	bool m_flagSousCible; // 1 lorsque le calcul utilise les sous-cibles
	//##ModelId=3D9AB7D30275
	bool m_flagVentilation; // 1 lorsque le calcul utilise les ventilations
	//##ModelId=3D9AB7D30289
	bool m_flagFiltreIndividus; // 1 lorsque le calcul utilise le filtre sur les individus
	// --------------------------------

	// Foyer 
	//##ModelId=3D9AB7D30293
	unsigned short m_NbFoyer;

private:
	// Variable intermédiaire pendant le calcul du vecteur binomial
	int m_sizeVB;
	double * m_VB0;
	double * m_VBt;
	inline void CalculApportDansVecteurBinomial(int & NrIndiv);
	// -------------------------------------------------------------
};


#endif // !defined(AFX_TOMY_H__2725D420_7196_11D3_989D_9A200D61B026__INCLUDED_)
