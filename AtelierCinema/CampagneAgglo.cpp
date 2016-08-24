// CampagneAgglo.cpp: implementation of the CCampagneAgglo class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "cinema.h"
#include "CampagneAgglo.h"
extern CCinemaApp theApp;


#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CCampagneAgglo::CCampagneAgglo()
{

}

CCampagneAgglo::~CCampagneAgglo()
{

}



#define NRINDEXCRITEREHABITAT (8)
void CCampagneAgglo::PrepareVecteurIndividusAgglo(int Cible)
{
	BYTE* Habitat=theApp.m_Cible.GetNewVecteurCritere(NRINDEXCRITEREHABITAT);
	for(int i=0;i<theApp.NbIndiv;i++)	
	{

		// Dans ce critère il y a 7 modalités.
		// Les parisiens on la modalité 6.
		// C'est donc ici que l'on filtre les individus
		// En fonction du lieu d'habitat.
		if(Habitat[i]<NumeroModaliteAgglo)
		{
			CibleUtile[Cible].Vecteur[i] = 0;	
		}
	}
	delete [] Habitat;
}

void CCampagneAgglo::CalculPerformanceAgglo(int Hypothese)
{
	CalculToutHypothese(Hypothese);
	m_ResultatAgglo=m_Resultat;


}

void CCampagneAgglo::CalculPerformanceMigre(int Hypothese)
{
	for(int iCib=0;iCib<NBCIBLECALCUL;iCib++)
	{
		if(m_NumCible[iCib]<0)continue;

		m_ResultatMigre.BudgetNet[Hypothese]=m_ResultatMigre.BudgetTot[Hypothese]=0;
		m_ResultatMigre.BasePopulation[iCib]=m_Resultat.BasePopulation[iCib]-m_ResultatAgglo.BasePopulation[iCib];
		m_ResultatMigre.NbCas[iCib]=m_Resultat.NbCas[iCib]-m_ResultatAgglo.NbCas[iCib];
		m_ResultatMigre.NbContactTot[iCib][Hypothese]=m_Resultat.NbContactTot[iCib][Hypothese]-m_ResultatAgglo.NbContactTot[iCib][Hypothese];
		m_ResultatMigre.NbIndivTouchesTot[iCib][Hypothese]=m_Resultat.NbIndivTouchesTot[iCib][Hypothese]-m_ResultatAgglo.NbIndivTouchesTot[iCib][Hypothese];
		m_ResultatMigre.NbIndivMemoTot[iCib][Hypothese]=m_Resultat.NbIndivMemoTot[iCib][Hypothese]-m_ResultatAgglo.NbIndivMemoTot[iCib][Hypothese];
	}
}

#include "dlgperformances.h"
#include "dlgperfagglo.h"
#include "dlggraphe.h"

//  HYPOTHESESPOP ou -3				(on ne charge que la première cible)
//  CIBLEUTILE						(calcul de la cible utile)
//  MULTICIBLE						(chargement des cibles et calcul du vecteur individu pour l'impression multicible.)
//  TOUTHYPOTHESEVALIDE				(calcul de l'hypothese valide)
//  TOUTHYPOTHESEACTIVE				(calcul de l'hypothese active)
//  TOUTHYPOTHESEACTIVEIFNOTVALIDE	(calcul de l'hypothese active lorsque celle-ci n'est pas valide)
//  TOUT							(calcul tout)
//  HYPOTHESESPOP					(calcul de tout le budget, toutes les mémos, toute la population)
//  HYPOTHESES						(calcul tout le budget et tout le mémo)
//  HYPOTHESEFORMATACTIF			(calcul tout le budget et tout le mémo si non fPiloteDurees )
//	BUDGETPERF						(calcul tout budget et tout mémo si boite performances ou graphique)


void CCampagneAgglo::LanceCalcul(int Choix)
{
	if(Choix==HYPOTHESESPOP || Choix==-3)
	{
		// On ne charge que la première cible
		if(m_NumCible[0]>=0) ChargeCible(m_NumCible[0],0);
	}

	CDlgPerformances * dlg1=(CDlgPerformances * )m_Perf;
	int hDlgPerf=dlg1->IsWindowVisible();

	CDlgPerfAgglo * dlgAgglo=(CDlgPerfAgglo * )m_PerfAgglo;
	int hDlgPerfAgglo=dlgAgglo->IsWindowVisible();

	CDlgGraphe * dlg2=(CDlgGraphe * )m_Graphe;
	int hDlgGraph=dlg2->IsWindowVisible();
	int hDlgPanorama=0;

	if(hDlgPerfAgglo)
	{
		// Si les agglo sont présente, il faut calculer en premier
		// Les performances sur les agglomérations...
		PrepareVecteurIndividus(0);
		PrepareVecteurIndividusAgglo(0);
		CalculPopCibleUtile(0);
		CalculPerformanceAgglo(m_HypotheseActive);

		// il faur recharger les bonnes cibles
		PrepareVecteurIndividus(0);
		CalculPopCibleUtile(0);

		// Et recalculer
	}
	
	
	// chargement de la cible pour calcul
	if(Choix==CIBLEUTILE)
	{
		PrepareVecteurIndividus(0);
		CalculPopCibleUtile(0);
	}


	// chargement des cibles pour impression multicible
	bool flagCalculPerfMigre=false;
	if (m_PrintPerfAgglo)
	{
		flagCalculPerfMigre=1;
		// Si les agglo sont présente, il faut calculer en premier
		// Les performances sur les agglomérations...
		for(int cib=0;cib<NBCIBLECALCUL;cib++)
		{
			if(m_IndexCibleImprime[cib]!=-1)
			{
				// Chargement des cibles sur les agglomérations
				ChargeCible(m_IndexCibleImprime[cib],cib);
				PrepareVecteurIndividus(cib);
				PrepareVecteurIndividusAgglo(cib);
				CalculPopCibleUtile(cib);
			}
		}
		// Calcul des perf sur les agglomérations
		CalculPerformanceAgglo(m_HypotheseActive);
		for(cib=0;cib<NBCIBLECALCUL;cib++)
		{
			if(m_IndexCibleImprime[cib]!=-1)
			{
				//ChargeCible(m_IndexCibleImprime[cib],cib);
				PrepareVecteurIndividus(cib);
				CalculPopCibleUtile(cib);
			}
		}
	}

	
	if(Choix==MULTICIBLE)
	{
		// Chargement "classique" des cibles
		for(int cib=0;cib<NBCIBLECALCUL;cib++)
		{
			if(m_IndexCibleImprime[cib]!=-1)
			{
				ChargeCible(m_IndexCibleImprime[cib],cib);
				PrepareVecteurIndividus(cib);
				CalculPopCibleUtile(cib);
			}
		}
	}

	// fonctions de calcul seules
	if(Choix==TOUTHYPOTHESEVALIDE){
		ASSERT(AfxCheckMemory());
		CalculToutHypothese(m_HypotheseValide);
		return;
	}
	if(Choix==TOUTHYPOTHESEACTIVE || hDlgPerfAgglo)
	{
		CalculToutHypothese(m_HypotheseActive);
		goto dessin;
		//return; //Modifier le 18/04/02 par goto dessin
	}
	if(Choix==TOUTHYPOTHESEACTIVEIFNOTVALIDE){
		if(m_HypotheseActive!=m_HypotheseValide)
			CalculToutHypothese(m_HypotheseActive);
		return;
	}
	if(Choix==TOUT){
		CalculTout();
		return;
	}

	if(!(hDlgGraph || hDlgPanorama || hDlgPerf|| hDlgPerfAgglo))return;


	if(Choix==HYPOTHESESPOP)
	{
		if(hDlgPerf)CalculToutBudget();
		CalculBasePopulation();
		CalculToutMemo();
		goto dessin;
	}
	if(Choix==HYPOTHESES)
	{
		if(hDlgPerf)CalculToutBudget();
		CalculToutMemo();
		goto dessin;
	}

	if(Choix==HYPOTHESEFORMATACTIF)
	{
		if(!fPiloteDurees)
		{
			if(hDlgPerf)CalculToutBudget();
			CalculToutMemo();
			goto dessin;
		}
	}

	if(Choix==BUDGETPERF)
	{
		if(hDlgPerf||hDlgGraph)
		{
			CalculToutBudget();
			CalculMemo(m_HypotheseActive);
			CalculTotalMemo(m_HypotheseActive);
		}
	}


	dessin: // on DESSINE ...
	if(hDlgPerfAgglo||flagCalculPerfMigre)
	{
		CalculPerformanceMigre(m_HypotheseActive);
	}

	if(hDlgPerf)dlg1->MajAffichagePerf();
	if(hDlgPerfAgglo)dlgAgglo->MajAffichagePerf();
	if(hDlgGraph)dlg2->MajAffichageACCOGRAP();
	m_pDoc->SetModifiedFlag();
}
