// TomEval.cpp: implementation of the CTomEval class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "TomEval.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

//##ModelId=3D9AB7D402BC
CTomEval::CTomEval()
{

}

//##ModelId=3D9AB7D402BD
CTomEval::~CTomEval()
{

}
#include ".\newtomeval\ficlbx.h"


//##ModelId=3D9AB7D402C7
#ifdef OPTION_TELEFRANCE
bool CTomEval::LanceCalculClassique(LPCSTR NomFicTomjob,LPCSTR NomFicResTom,bool flagMarginal)
{
#ifdef OPTION_JFC
	TRACE("\n\n\nCTomEval::LanceCalculClassique\n\n\n");

	// Efface le fichier restom
	if(flagMarginal)
		m_Restom.EffaceFichier(NomFicResTom);

	// Lecture du tomjob
	m_Tomjob.LectureFichier(NomFicTomjob);


	// Etape 1
	// Initialisation des paramètres
	TRACE("*** Initialisation des paramètres ***\n");
	CTomyParam param;
	param.m_CheminData=m_Tomjob.m_CheminData;
	param.m_TypeSupport=1; // écrans
	param.m_ValideBeta=m_Tomjob.m_Beta;
	param.m_ValideMini=m_Tomjob.m_Mini;
	param.m_ValideMaxi=m_Tomjob.m_Maxi;
	param.m_NbClassesResultatDistribution=20;

//	param.m_DisableBisource=1; //pour tester les probabliltés sans GRP et MW
//	param.m_CoeffCHI=0; //pour tester
//	param.m_CoeffDuplication=0;//pour tester

	m_Tomy.SetParametres(&param);
	// __________________________________________

	// Etape 2
	// Initialisaton du panel
	TRACE("*** Initialisation du(des) panel(s) ***\n");
	bool flagMediadata=0;
	bool flagMediacabsat=0;

	// Le choix de l'étude est maintenant envisagée en amont (dans l'étude)
	if(m_Tomjob.m_CodePanelMediadata!="XXXX")
		flagMediadata=1;
	if(m_Tomjob.m_CodePanelMediaCabSat!="XXXX")
		flagMediacabsat=1;

	if(flagMediadata==1 && flagMediacabsat==1)
	{
		// Initialisation Mediadata+Mediacabsat
//		m_Tomy.InitialisePanelMediadataAvecSurEchantillonMediacabsat(m_Tomjob.m_CodePanelMediadata,m_Tomjob.m_CodePanelMediaCabSat);
		m_Tomy.InitialisePanelMediadataETMediacabsat(m_Tomjob.m_CodePanelMediadata,m_Tomjob.m_CodePanelMediaCabSat);
	}
	else if(flagMediadata==1)
	{
		// Le mediadata
		m_Tomy.InitialisePanel(m_Tomjob.m_CodePanelMediadata,"MEDIADATA");
	}
	else if(flagMediacabsat==1)
	{
		// Le Mediacabsat
		m_Tomy.InitialisePanel(m_Tomjob.m_CodePanelMediaCabSat,"MEDIACABSAT");
	}
	else
	{
		// c'est que le plan est vide ?
		return (0);
	}
	//
	// __________________________________________

	// Etape 3
	// Initialisation de la cible
	TRACE("*** Initialisation de la cible ***\n");

	int NrCibleJFC;
	NrCibleJFC=m_Tomjob.m_NumeroCible;
	m_Tomy.InitialiseCible(NrCibleJFC);

	// ou si l'on travaille avec des sous cibles
	// CTomy::InitialiseSousCible(int NrCible,int CritereFiltre,CWordArray * ModaliteFiltre)
	// __________________________________________

	// A ce point on peut obtenir des informations sur la cible du Panel
	m_Restom.SetSize(m_Tomjob.m_NbPlan);
	m_Restom.GetRestom(0)->m_NbCas.RemoveAll();
	if(flagMediadata)m_Restom.GetRestom(0)->m_NbCas.Add(m_Tomy.GetNbCasMediadata());
	if(flagMediacabsat)m_Restom.GetRestom(0)->m_NbCas.Add(m_Tomy.GetNbCasMediacabsat());
	double PopulationPanel=m_Tomy.GetPopulation();
	// Les résultats sont sur la population de référence sauf pour le CIM (2TUDE DIPTIQUE°
	m_Restom.GetRestom(0)->m_Population=m_Tomjob.m_Population;
	double deltapop=m_Restom.GetRestom(0)->m_Population/PopulationPanel; // pour calculer les ventilations
	// __________________________________________
	// nouvelle étape depuis la fusion à la vollée
	// du panel Médiadata et Médiacabsat
	if(flagMediadata==1 && flagMediacabsat==1)
		m_Tomy.CalculCoeffCorrectionOffreElargie(m_Tomjob.m_Population,m_Tomjob.m_PopulationOffreElargie);

	// Etape 4
	// Evaluation des plans
	TRACE("*** Evaluation du(des) plans(s) ***\n");
	for(int p=0;p<m_Tomjob.m_NbPlan;p++)
	{
		CRestom * Restom;
		Restom=m_Restom.GetRestom(p);
		m_Tomy.InitialiseSpot();
		for(int s=0;s<m_Tomjob.m_Plan.GetSize();s++)
		{
			if(!AddEcranDansTomy(s,p))return(0);
		}
		// __________________________________________

		// Lancement du calcul
		double couverture=m_Tomy.LanceEvalDistrib();
		//double couverture=m_Tomy.LanceEvalDistributionCroisee();
		// __________________________________________

		// pour les tests on vérifie l'Asymptote
#ifdef _DEBUG
		double A=m_Tomy.GetAsymptote();
		CString txt;
		txt.Format("Population panel=%d\nCouverture=%f\nAsymptote=%f",m_Tomy.GetPopulation(),couverture,A);
		AfxMessageBox(txt);
#endif

		// Récupération des résultats calculé par Tomy
		// Rangement des résultats dans le Restom
		Restom->UpdateResultat(&m_Tomy,m_Restom.GetRestom(0)->m_Population);

		// Courbe de réponse
		m_Restom.GetRestom(0)->m_CourbeReponse.RemoveAll();
		for(int i=1;i<=param.m_NbClassesResultatDistribution;i++)
		{
			m_Restom.GetRestom(0)->m_CourbeReponse.Add((WORD)m_Tomy.GetCourbeReponse(i));
		}

		if(m_Tomjob.m_NbVentilation>0)
		{
			TRACE("\n*** Calcul des ventilations ***\n\n");
			// Chargement des critères, car ce n'est pas le role de Tomy
			// Chargement des critères
			// Note: cette fonction n'étant utilisée que pour les ventilations,
			// La rapidité n'est alors pas critère necessaire, on lit les informations dans le fichier au fur et à meusure.
			CFicLBX FicCritere;

			CString CheminPanel;
			CheminPanel.Format("%s\\%s",m_Tomjob.m_CheminData,"MEDIADATA");
			FicCritere.LoadVague(CheminPanel,m_Tomjob.m_CodePanelMediadata);

			// Calcul des ventilations **********************************
			// La ventilation revient à calculer pour chaque modalité
			// la population,le GRP et la couverture dans la sous-cible.
			// on reprend donc le calcul à l'étape 3

			Restom->InitResultatVentile(m_Tomjob.m_NbVentilation);

			for(int v=0;v<m_Tomjob.m_NbVentilation;v++)
			{
				int NrCritere=m_Tomjob.m_Ventilation[v];
				Restom->m_LibelleCritere.Add(FicCritere.GetLibCritere(NrCritere));
				int NbModalite=FicCritere.GetNbModalite(NrCritere);
				for(int mod=0;mod<NbModalite;mod++)
				{
					// Initialisation de la sous cible
					Restom->m_LibelleModalite[v].Add(FicCritere.GetLibModalite(NrCritere,mod));
					m_Tomy.InitialiseCibleVentile(NrCibleJFC,NrCritere,mod);
					Restom->m_Population_Ventile[v].Add(m_Tomy.GetPopulation()*deltapop);
					Restom->m_NbCas_Ventile[v].Add(m_Tomy.GetNbCas());

					// Lancement du calcul
					Restom->m_Couverture_Ventile[v].Add(m_Tomy.LanceEval());
					Restom->m_GRP_Ventile[v].Add(m_Tomy.GetGRP());
					// __________________________________________
				}
			}
			// ************************************************************
			// Restitution de la cible
			m_Tomy.InitialiseCible(NrCibleJFC);

		}
	}
	// Ecrire le fichier restom
	m_Restom.EcritureFichier(NomFicResTom);
#endif

	return(1); //OK
}
#endif

//##ModelId=3D9AB7D4033F
bool CTomEval::AddEcranDansTomy(int ecran,int sousplan)
{
	int NbInsertion=m_Tomjob.m_Plan[ecran].m_NbInsertion[sousplan];
	if(NbInsertion<=0)return(1); //OK
	int NrStationJFC;
#ifdef OPTION_CARAT
	if(!m_LienStation.GetLien(m_Tomjob.m_Plan[ecran].m_Station,NrStationJFC))
	{
		CString msg;
		msg.Format("La station %d n'est pas trouvée dans le fichier %s",m_Tomjob.m_Plan[ecran].m_Station,m_LienStation.GetNomFichLien());
		AfxMessageBox(msg);
		return (0);
	}
#else
	NrStationJFC=m_Tomjob.m_Plan[ecran].m_Station;
#endif

#ifdef OPTION_CIMBELGE
	m_Tomy.AddSpot(NrStationJFC,
					atoi(m_Tomjob.m_Plan[ecran].m_JourD),
					m_Tomjob.m_Plan[ecran].m_JourN,
					m_Tomjob.m_Plan[ecran].m_horaire+m_Tomjob.m_Plan[ecran].m_OptionAncrage*10000,
					m_Tomjob.m_Plan[ecran].m_GRP,
					NbInsertion);
#elif OPTION_RUSSIE
	m_Tomy.AddSpot(NrStationJFC,
					atoi(m_Tomjob.m_Plan[ecran].m_JourD),
					m_Tomjob.m_Plan[ecran].m_JourN,
					m_Tomjob.m_Plan[ecran].m_horaire+m_Tomjob.m_Plan[ecran].m_OptionAncrage*10000,
					m_Tomjob.m_Plan[ecran].m_GRP,
					NbInsertion);
#else
	m_Tomy.AddSpot(NrStationJFC,
					m_Tomjob.m_Plan[ecran].m_JourN,
					m_Tomjob.m_Plan[ecran].m_horaire,
					m_Tomjob.m_Plan[ecran].m_GRP,
					NbInsertion);
#endif

	return(1); //OK
}


#ifndef OPTION_CARAT
//##ModelId=3D9AB7D40399
bool CTomEvalMarginal::LanceCalculBaseMarginal()
{
#ifdef OPTION_TELEFRANCE
	return(LanceCalculClassique(m_NomFicTomjob,m_NomFicRestom,1));
#else
	return(LanceCalculClassique(m_NomFicTomjob,m_NomFicRestom,m_NomFicCibjob,1));
#endif
}

//##ModelId=3D9AB7D403A4
bool CTomEvalMarginal::UpdateEcranDansTomy(int ecran,int sousplan)
{
	int NbInsertion=m_Tomjob.m_Plan[ecran].m_NbInsertion[sousplan];
	if(NbInsertion==0)
	{
		ASSERT(0);
		return(1); //OK
	}
	int NrStationJFC;

	/*if(!m_LienStation.GetLien(m_Tomjob.m_Plan[ecran].m_Station,NrStationJFC))
	{
		CString msg;
		msg.Format("La station %d n'est pas trouvée dans le fichier %s",m_Tomjob.m_Plan[ecran].m_Station,m_LienStation.GetNomFichLien());
		AfxMessageBox(msg);
		return (0);
	}*/

//	NrStationJFC=m_Tomjob.m_Plan[ecran].m_Station; // Pour les tests
	NrStationJFC=m_Tomjob.m_Plan[ecran].m_Station;
#ifdef OPTION_CIMBELGE
	m_Tomy.UpdateSpot(	NrStationJFC,
					atoi(m_Tomjob.m_Plan[ecran].m_JourD),
					m_Tomjob.m_Plan[ecran].m_JourN,
					m_Tomjob.m_Plan[ecran].m_horaire+m_Tomjob.m_Plan[ecran].m_OptionAncrage*10000,
					m_Tomjob.m_Plan[ecran].m_GRP,
					NbInsertion);
#elif OPTION_RUSSIE
	m_Tomy.UpdateSpot(	NrStationJFC,
					atoi(m_Tomjob.m_Plan[ecran].m_JourD),
					m_Tomjob.m_Plan[ecran].m_JourN,
					m_Tomjob.m_Plan[ecran].m_horaire+m_Tomjob.m_Plan[ecran].m_OptionAncrage*10000,
					m_Tomjob.m_Plan[ecran].m_GRP,
					NbInsertion);
#else
	m_Tomy.UpdateSpot(	NrStationJFC,
					m_Tomjob.m_Plan[ecran].m_JourN,
					m_Tomjob.m_Plan[ecran].m_horaire,
					m_Tomjob.m_Plan[ecran].m_GRP,
					NbInsertion);
#endif

	return(1); //OK
}



//##ModelId=3D9AB7D4039A
bool CTomEvalMarginal::LanceCalculAddSpot()
{
	LanceCalculMarginal();

	// Récupération des résultats calculé par Tomy
	// Rangement des résultats dans le Restom
	m_Restom.GetRestom(0)->UpdateResultat(&m_Tomy,m_Restom.GetRestom(0)->m_Population);

	m_Restom.EcritureFichier(m_NomFicRestom,1);

	return(1); //OK
}

//##ModelId=3D9AB7D403A3
bool CTomEvalMarginal::LanceCalculAddZoom()
{

		// ---------------------------------
		// 1- Sauvegarde le plan dans tomy
		// 2- Calcul de plan avec zoom
		// 3- Calcul du différentiel et ecrirure du restom
		// 4- Restauration du plan avant zoom
		// 5- Ecriture du restom
		// ---------------------------------

	// 1- Sauvegarde le plan dans tomy
	m_Tomy.SavePlanBeforeZoom();

	// 2- Calcul du plan avec zoom
	LanceCalculMarginal();

	// 3- Récupération des résultats avec zoom et calcul du différentiel
	CRestomZoom RestomZoom;
	RestomZoom.UpdateResultat(&m_Tomy,m_Restom.GetRestom(0),m_Restom.GetRestom(0)->m_Population);

	// 4- Restauration du plan avant zoom
	m_Tomy.RestorePlanAfterZoom();

	// 5- Ecriture du restom
	RestomZoom.EcritureFichier(m_NomFicRestom);

	return 1;
}

#ifndef OPTION_RUSSIE
//##ModelId=3D9AB7D403AF
bool CTomEvalMarginal::LanceCalculMarginal()
{
	// Efface le fichier restom
	m_Restom.EffaceFichier(m_NomFicRestom);

	// Lecture du tomjob
	m_Tomjob.LectureFichierMarginal(m_NomFicTomjob);

	// Etape 4 du process Tomy:
	// Evaluation des plans
	ASSERT(m_Tomjob.m_NbPlan==1);
	int p=0;

	// Update du plan
	for(int s=0;s<m_Tomjob.m_Plan.GetSize();s++)
	{
		if(!UpdateEcranDansTomy(s,p))return(0);
	}
	// __________________________________________
	// Lancement du calcul
#ifdef OPTION_REACH_BELGE
	if (!m_Tomy.CalculOffre()) return (0);;

	short *TabSemAct = m_Tomy.GetTabSemAct();
	short NbSem = m_Tomy.GetNbSemPlan();
		// __________________________________________
		// Lancement du calcul
//		double couverture=m_Tomy.LanceEvalDistrib();

	double CouvM = m_Tomy.GetCouvertureMJ();
	double CouvW = m_Tomy.GetCouvertureWK();
	double CouvY = m_Tomy.GetCouvertureY();
	double *CouvWi = m_Tomy.GetCouvertureWi();
	double couvertureSe = m_Tomy.seCouverturePlan(CouvM,CouvW,CouvWi,CouvY,NbSem);
	m_Tomy.SetCouverture(couvertureSe);
	double GRPjob=m_Tomy.CalculGRPDansPlan();
	m_Tomy.SetGRP(GRPjob);
	long NbSpot=m_Tomy.CalculNbSpotDansPlan();
	m_Tomy.SetNbInsertions(NbSpot);
	double *TabDistrib = m_Tomy.seDistributPlan(couvertureSe,GRPjob/NbSpot,NbSpot);
	double couverture = m_Tomy.seResDistributPlan(TabDistrib,NbSpot);


#else
	double couverture=m_Tomy.LanceEvalDistrib();
#endif
	// __________________________________________

	return(1); //OK
}
#endif

//##ModelId=3D9AB7D40385
bool CTomEvalMarginal::EcritureRestom()
{
	m_Restom.EffaceFichier(m_NomFicRestom);
	return(m_Restom.EcritureFichier(m_NomFicRestom,1));
}
#endif


#include "TablePopulationEtalon.h"
int CTomEval::GetPopulationEquipeCabSat(LPCSTR CheminData,LPCSTR Etude,LPCSTR Vague,int NrCible)
{
	CTablePopulationEtalon FichierPop;
	int pop=0;
	CString NomFic;
	NomFic.Format("%s\\%s\\%s.pop",CheminData,Etude,Vague);
	if(!FichierPop.LoadFile(NomFic))return 0;
	if(!FichierPop.GetPopulation(NrCible,pop))
	{
		CString txt;
		txt.Format("Erreur: La cible %s n'est pas trouvée dans le fichier des populations étalons %s",NrCible,NomFic);
		AfxMessageBox(txt);
		return 0;
	}
	return pop;
}

#ifdef OPTION_CIMBELGE
bool CTomEval::LanceCalculClassique(LPCSTR NomFicTomjob,LPCSTR NomFicResTom,LPCSTR NomFicCibJob,bool flagMarginal)
{
	TRACE("\n\n\nCTomEval::LanceCalculClassique _ OPTION_CIMBELGE\n\n\n");
	// Efface le fichier restom
#ifdef OPTION_JFC
	if(flagMarginal)
		m_Restom.EffaceFichier(NomFicResTom);
#else
		m_Restom.EffaceFichier(NomFicResTom);
#endif

	// Lecture du tomjob
	m_Tomjob.LectureFichier(NomFicTomjob);

	// Lecture du cibjob
	m_Cibjob.LoadFile(NomFicCibJob,m_Tomjob.m_PosFichierCIBJOB,m_Tomjob.m_NbIndiv,m_Tomjob.m_NbVentilation);

	// Etape 1
	// Initialisation des paramètres
	TRACE("*** Initialisation des paramètres ***\n");
	CTomyParam param;
	param.m_CheminData=m_Tomjob.m_CheminData;
	param.m_TypeSupport=0; // QH
	param.m_ValideBeta=m_Tomjob.m_Beta;
	param.m_ValideMini=m_Tomjob.m_Mini;
	param.m_ValideMaxi=m_Tomjob.m_Maxi;
	param.m_NbClassesResultatDistribution=20;

//	param.m_DisableBisource=1; //pour tester les probabliltés sans GRP et MW
//	param.m_CoeffCHI=0; //pour tester
//	param.m_CoeffDuplication=0;//pour tester

	m_Tomy.SetParametres(&param);
	// __________________________________________

	// Etape 2
	// Initialisaton du panel
	TRACE("*** Initialisation du(des) panel(s) ***\n");
	m_Tomy.InitialisePanel(m_Tomjob.m_CodePanelMediadata,"CIMBELGE");
	// __________________________________________

	// Etape 3
	// Initialisation de la cible
	TRACE("*** Initialisation de la cible ***\n");
	m_Tomy.InitialiseCible(m_Cibjob.m_CoeffExCible);

	// ou si l'on travaille avec des sous cibles
	// CTomy::InitialiseSousCible(int NrCible,int CritereFiltre,CWordArray * ModaliteFiltre)
	// __________________________________________

	// A ce point on peut obtenir des informations sur la cible du Panel
	m_Restom.SetSize(m_Tomjob.m_NbPlan);
	m_Restom.GetRestom(0)->m_NbCas.RemoveAll();
	m_Restom.GetRestom(0)->m_NbCas.Add(m_Tomy.GetNbCas());
	double PopulationPanel=m_Tomy.GetPopulation();
	// Les résultats sont sur la population de référence sauf pour le CIM (ETUDE DIPTIQUE°
	m_Restom.GetRestom(0)->m_Population=(int)PopulationPanel;
	// __________________________________________


	// Etape 4
	// Evaluation des plans
#ifdef OPTION_REACH_BELGE
	TRACE("*** Evaluation du(des) plans(s) ***\n");
	for(int p=0;p<m_Tomjob.m_NbPlan;p++)
	{
		CRestom * Restom;
		Restom=m_Restom.GetRestom(p);
		m_Tomy.InitialiseSpot();
		for(int sp=0;sp<m_Tomjob.m_Plan.GetSize();sp++)
		{
			if(!AddEcranDansTomy(sp,p))return(0);
		}
		if (!m_Tomy.CalculOffre()) continue;

		short *TabSemAct = m_Tomy.GetTabSemAct();
		short NbSem = m_Tomy.GetNbSemPlan();
/*
		double *CouvWii = new double[NbSem];
		short NbS = 0;
		for(int s=0;s<m_Tomy.GetNbSemMax();s++) {
			if (!TabSemAct[s]) continue;
			m_Tomy.InitialiseSpot();
			for(sp=0;sp<m_Tomjob.m_Plan.GetSize();sp++)
			{
				AddEcranSemaineDansTomy(TabSemAct[s],p,sp);
			}
			CouvWii[NbS] = m_Tomy.LanceEvalDistrib();
			NbS++;
		}
*/
		// __________________________________________
		// Lancement du calcul
//		double couverture=m_Tomy.LanceEvalDistrib();

		double CouvM = m_Tomy.GetCouvertureMJ();
		double CouvW = m_Tomy.GetCouvertureWK();
		double CouvY = m_Tomy.GetCouvertureY();
		double *CouvWi = m_Tomy.GetCouvertureWi();
		double couvertureSe = m_Tomy.seCouverturePlan(CouvM,CouvW,CouvWi,CouvY,NbSem);
		m_Tomy.SetCouverture(couvertureSe);
		double GRPjob=m_Tomy.CalculGRPDansPlan();
		m_Tomy.SetGRP(GRPjob);
		long NbSpot=m_Tomy.CalculNbSpotDansPlan();
		m_Tomy.SetNbInsertions(NbSpot);
		double *TabDistrib = m_Tomy.seDistributPlan(couvertureSe,GRPjob/NbSpot,NbSpot);
		double couverture = m_Tomy.seResDistributPlan(TabDistrib,NbSpot);
		
		// __________________________________________

		// pour les tests on vérifie l'Asymptote
		double GRPCalcule=GRPjob;//m_Tomy.GetGRP();
#ifdef _DEBUG
		CString txt;
		txt.Format("Population panel=%d\nCouverture=%f\nGRP=%f",m_Tomy.GetPopulation(),couvertureSe,GRPCalcule);
		AfxMessageBox(txt);
#endif

		// Récupération des résultats calculé par Tomy
		// Rangement des résultats dans le Restom
		Restom->UpdateResultat(&m_Tomy,m_Restom.GetRestom(0)->m_Population);

		// Courbe de réponse
		m_Restom.GetRestom(0)->m_CourbeReponse.RemoveAll();
		for(int i=1;i<=param.m_NbClassesResultatDistribution;i++)
		{
			m_Restom.GetRestom(0)->m_CourbeReponse.Add((WORD)m_Tomy.GetCourbeReponse(i));
		}
	}
	// Ecrire le fichier restom
	m_Restom.EcritureFichier(NomFicResTom,flagMarginal);
#else
	TRACE("*** Evaluation du(des) plans(s) ***\n");
	for(int p=0;p<m_Tomjob.m_NbPlan;p++)
	{
		CRestom * Restom;
		Restom=m_Restom.GetRestom(p);
		m_Tomy.InitialiseSpot();
		for(int s=0;s<m_Tomjob.m_Plan.GetSize();s++)
		{
			if(!AddEcranDansTomy(s,p))return(0);
		}
		// __________________________________________

		// Lancement du calcul
		double couverture=m_Tomy.LanceEvalDistrib();




		//double couverture=m_Tomy.LanceEvalDistributionCroisee();
		// __________________________________________

		// pour les tests on vérifie l'Asymptote
		double GRPjob=m_Tomjob.CalculGRPDansJob(p);
		double GRPCalcule=m_Tomy.GetGRP();
#ifdef _DEBUG
		CString txt;
		txt.Format("Population panel=%d\nCouverture=%f\nGRP=%f",m_Tomy.GetPopulation(),couverture,GRPCalcule);
		AfxMessageBox(txt);
#endif

		// Récupération des résultats calculé par Tomy
		// Rangement des résultats dans le Restom
		Restom->UpdateResultat(&m_Tomy,m_Restom.GetRestom(0)->m_Population);

		// Courbe de réponse
		m_Restom.GetRestom(0)->m_CourbeReponse.RemoveAll();
		for(int i=1;i<=param.m_NbClassesResultatDistribution;i++)
		{
			m_Restom.GetRestom(0)->m_CourbeReponse.Add((WORD)m_Tomy.GetCourbeReponse(i));
		}
	}
	// Ecrire le fichier restom
	m_Restom.EcritureFichier(NomFicResTom,flagMarginal);
#endif

	return(1); //OK
}
#endif



#ifdef OPTION_REACH_BELGE
bool CTomEval::AddEcranSemaineDansTomy(int Semaine,int sousplan,int ecran)
{
	int JouD = m_Tomy.GetJourDate(atoi(m_Tomjob.m_Plan[ecran].m_JourD));
	if (Semaine != JouD/7) return 0;

	int NbInsertion=m_Tomjob.m_Plan[ecran].m_NbInsertion[sousplan];
	if(NbInsertion<=0)return 0; //OK
	int NrStationJFC = m_Tomjob.m_Plan[ecran].m_Station;
	m_Tomy.AddSpot(NrStationJFC,
					atoi(m_Tomjob.m_Plan[ecran].m_JourD),
					m_Tomjob.m_Plan[ecran].m_JourN,
					m_Tomjob.m_Plan[ecran].m_horaire+m_Tomjob.m_Plan[ecran].m_OptionAncrage*10000,
					m_Tomjob.m_Plan[ecran].m_GRP,
					NbInsertion);
	return 1;
}
#endif

#ifdef OPTION_RUSSIE
bool CTomEvalMarginal::LanceCalculMarginal()
{
	// Efface le fichier restom
	m_Restom.EffaceFichier(m_NomFicRestom);

	// Lecture du tomjob
	m_Tomjob.LectureFichierMarginal(m_NomFicTomjob);

	// Etape 4 du process Tomy:
	// Evaluation des plans
	ASSERT(m_Tomjob.m_NbPlan==1);
	int p=0;

	// Update du plan
	for(int s=0;s<m_Tomjob.m_Plan.GetSize();s++)
	{
		if(!UpdateEcranDansTomy(s,p))return(0);
	}
	// __________________________________________
	// Lancement du calcul

	double couverture=m_Tomy.LanceEvalDistrib();

	// __________________________________________

	return(1); //OK
}

bool CTomEval::LanceCalculClassique(LPCSTR NomFicTomjob,LPCSTR NomFicResTom,LPCSTR NomFicCibJob,bool flagMarginal)
{
	TRACE("\n\n\nCTomEval::LanceCalculClassique _OPTION_RUSSIE\n\n\n");
	// Efface le fichier restom
#ifdef OPTION_JFC
	if(flagMarginal)
		m_Restom.EffaceFichier(NomFicResTom);
#else
		m_Restom.EffaceFichier(NomFicResTom);
#endif

	// Lecture du tomjob
	m_Tomjob.LectureFichier(NomFicTomjob);

	// Lecture du cibjob
	m_Cibjob.LoadFile(NomFicCibJob,m_Tomjob.m_PosFichierCIBJOB,m_Tomjob.m_NbIndiv,m_Tomjob.m_NbVentilation);

	// Etape 1
	// Initialisation des paramètres
	TRACE("*** Initialisation des paramètres ***\n");
	CTomyParam param;
	param.m_CheminData=m_Tomjob.m_CheminData;
	param.m_TypeSupport=0; // QH
	param.m_ValideBeta=m_Tomjob.m_Beta;
	param.m_ValideMini=m_Tomjob.m_Mini;
	param.m_ValideMaxi=m_Tomjob.m_Maxi;
	param.m_NbClassesResultatDistribution=20;

//	param.m_DisableBisource=1; //pour tester les probabliltés sans GRP et MW
//	param.m_CoeffCHI=0; //pour tester
//	param.m_CoeffDuplication=0;//pour tester

	m_Tomy.SetParametres(&param);
	// __________________________________________

	// Etape 2
	// Initialisaton du panel
	TRACE("*** Initialisation du(des) panel(s) ***\n");
	m_Tomy.InitialisePanel(m_Tomjob.m_CodePanelMediadata,"RUSSIE",m_Tomjob.m_NbIndiv);
	// __________________________________________

	// Etape 3
	// Initialisation de la cible
	TRACE("*** Initialisation de la cible ***\n");
	m_Tomy.InitialiseCible(m_Cibjob.m_CoeffExCible);

	// ou si l'on travaille avec des sous cibles
	// CTomy::InitialiseSousCible(int NrCible,int CritereFiltre,CWordArray * ModaliteFiltre)
	// __________________________________________

	// A ce point on peut obtenir des informations sur la cible du Panel
	m_Restom.SetSize(m_Tomjob.m_NbPlan);
	m_Restom.GetRestom(0)->m_NbCas.RemoveAll();
	m_Restom.GetRestom(0)->m_NbCas.Add(m_Tomy.GetNbCas());
	double PopulationPanel=m_Tomy.GetPopulation();
	// Les résultats sont sur la population de référence sauf pour le CIM (ETUDE DIPTIQUE°
	m_Restom.GetRestom(0)->m_Population=(int)PopulationPanel;
	// __________________________________________

	// Etape 4
	// Evaluation des plans
	TRACE("*** Evaluation du(des) plans(s) ***\n");
	for(int p=0;p<m_Tomjob.m_NbPlan;p++)
	{
		CRestom * Restom;
		Restom=m_Restom.GetRestom(p);
		m_Tomy.InitialiseSpot();
		for(int s=0;s<m_Tomjob.m_Plan.GetSize();s++)
		{
			if(!AddEcranDansTomy(s,p))return(0);
		}
		// __________________________________________

		// Lancement du calcul
		double couverture=m_Tomy.LanceEvalDistrib();

		//double couverture=m_Tomy.LanceEvalDistributionCroisee();
		// __________________________________________

		// pour les tests on vérifie l'Asymptote
		double GRPjob=m_Tomjob.CalculGRPDansJob(p);
		double GRPCalcule=m_Tomy.GetGRP();
#ifdef _DEBUG
		CString txt;
		txt.Format("Population panel=%d\nCouverture=%f\nGRP=%f",m_Tomy.GetPopulation(),couverture,GRPCalcule);
		AfxMessageBox(txt);
#endif

		// Récupération des résultats calculé par Tomy
		// Rangement des résultats dans le Restom
#ifdef OPTION_RUSSIE
		Restom->UpdateResultat(&m_Tomy,m_Tomjob.m_Population);
#elif
		Restom->UpdateResultat(&m_Tomy,m_Restom.GetRestom(0)->m_Population);
#endif

		// Courbe de réponse
		m_Restom.GetRestom(0)->m_CourbeReponse.RemoveAll();
		for(int i=1;i<=param.m_NbClassesResultatDistribution;i++)
		{
			m_Restom.GetRestom(0)->m_CourbeReponse.Add((WORD)m_Tomy.GetCourbeReponse(i));
		}
	}
	// Ecrire le fichier restom
	m_Restom.EcritureFichier(NomFicResTom,flagMarginal);

	return(1); //OK
}
#endif
