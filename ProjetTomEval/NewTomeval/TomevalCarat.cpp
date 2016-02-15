// TomevalCarat.cpp: implementation of the CTomevalCarat class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Tomeval.h"
#include "TomevalCarat.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CTomevalCarat::CTomevalCarat()
{

}

CTomevalCarat::~CTomevalCarat()
{

}

void CTomevalCarat::TrouveJourActif()
{
	m_TabJour.RemoveAll();
	int s,j;
	CResultatJour J;
	for(s=0;s<m_Tomjob.m_Plan.GetSize();s++)
	{
		for(j=0;j<m_TabJour.GetSize();j++)
		{
		
			if(m_Tomjob.m_Plan[s].m_JourD==m_TabJour[j].m_Libelle)break;
		}
		// on ajoute le jour si il n'est pas trouvé
		if(j>=m_TabJour.GetSize())
		{
			J.m_Libelle=m_Tomjob.m_Plan[s].m_JourD.GetBuffer(1);
			m_TabJour.Add(J);
		}
	}
}

//#include <afxdisp.h>
void CTomevalCarat::EvaluerApportSemaine()
{
	m_TabSemaine.RemoveAll();
	CString OldLib="",NewLib,LibEnJour;
	COleDateTime D;
	CResultatJour S;
	for(int j=0;j<m_TabJour.GetSize();j++)
	{
		// Ce jour fait parti de quelle semaine ?
		D.ParseDateTime(m_TabJour[j].m_Libelle,VAR_DATEVALUEONLY);
	//	D.SetTime(12,1,1);
		int NoSem=atoi(D.Format("%W"))+1;
		int Year=D.GetYear();
		NewLib.Format("Semaine %d/%d",NoSem,Year);

		// Est-ce une nouvelle semaine ?
		if(NewLib!=OldLib){
			COleDateTimeSpan Duree;
			COleDateTime Lun,Dim;

			OldLib=NewLib;
			m_TabSemaine.Add(S);
			// On en profite pour créer le libelle
			int day=D.GetDayOfWeek()-2;
			if(day<0)day=6;
			Duree.SetDateTimeSpan(day,0,0,0);
			Lun=D-Duree;
			Duree.SetDateTimeSpan(6,0,0,0);
			Dim=Lun+Duree;
			LibEnJour.Format("du %s au %s",Lun.Format("%d/%m/%y"),Dim.Format("%d/%m/%y"));
		}
		S.m_Libelle=NewLib;
		S.m_LibJour=LibEnJour;
		S.m_Individu=m_TabJour[j].m_Individu;
		m_TabSemaine[m_TabSemaine.GetSize()-1]=S;
	}
}

void CTomevalCarat::EvaluerApportSemaineGlissante()
{
	m_TabSemaineGlissante.RemoveAll();
	COleDateTimeSpan Dgliss; 
	int DayGliss;
	CString OldLib="",NewLib,LibEnJour;
	COleDateTime D;
	CResultatJour S;
	
	// On calcul le décalage entre le jour de début et le lundi
	if(m_TabJour.GetSize()<=0)return;
	D.ParseDateTime(m_TabJour[0].m_Libelle,VAR_DATEVALUEONLY);
	DayGliss=D.GetDayOfWeek()-2;
	if(DayGliss<0)DayGliss=6;
	Dgliss.SetDateTimeSpan(DayGliss,0,0,0);

	for(int j=0;j<m_TabJour.GetSize();j++)
	{
		// Ce jour fait parti de quelle semaine ?
		D.ParseDateTime(m_TabJour[j].m_Libelle,VAR_DATEVALUEONLY);
	//	D.SetTime(12,1,1);
		D=D-Dgliss;
		int NoSem=atoi(D.Format("%W"))+1;
		int Year=D.GetYear();
		NewLib.Format("Semaine %d/%d",NoSem,Year);

		// Est-ce une nouvelle semaine ?
		if(NewLib!=OldLib)
		{
			COleDateTimeSpan Duree;
			COleDateTime Lun,Dim;

			OldLib=NewLib;
			m_TabSemaineGlissante.Add(S);
			// On en profite pour créer le libelle
			int day=D.GetDayOfWeek()-2;
			if(day<0)day=6;
			day-=DayGliss;
			Duree.SetDateTimeSpan(day,0,0,0);
			Lun=D-Duree;
			Duree.SetDateTimeSpan(6,0,0,0);
			Dim=Lun+Duree;
			LibEnJour.Format("du %s au %s",Lun.Format("%d/%m/%y"),Dim.Format("%d/%m/%y"));
		}
		S.m_Libelle=NewLib;
		S.m_LibJour=LibEnJour;
		S.m_Individu=m_TabJour[j].m_Individu;
		m_TabSemaineGlissante[m_TabSemaineGlissante.GetSize()-1]=S;
	}
}

bool CTomevalCarat::LanceCalculClassique(LPCSTR NomFicTomjob,LPCSTR NomFicResTom,bool flagMarginal)
{

	TRACE("\n\n\nCTomEval::LanceCalculClassique\n\n\n");


	// Efface le fichier restom
	m_Restom.EffaceFichier(NomFicResTom);

	// Lecture du tomjob
	m_Tomjob.LectureFichier(NomFicTomjob);

	// NOTE: Carat ne travail toujours que sur UN plan
	ASSERT(m_Tomjob.m_NbPlan==1);

	// Lecture de la table des liens des stations
	CString NomFic;
	NomFic.Format("%s\\Liens_Carat.sta",m_Tomjob.m_CheminData);
	m_LienStation.LoadFile(NomFic);
	
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

	param.m_NbRatioConsomation=3;
	m_Tomy.SetParametres(&param);
	// __________________________________________

	// Etape 2
	// Initialisaton du panel
	TRACE("*** Initialisation du(des) panel(s) ***\n");
	bool flagMediadata=0;
	bool flagMediacabsat=0;
	flagMediadata=1;
	if(m_Tomjob.m_CodePanelMediaCabSat!="----")
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
	NomFic.Format("%s\\Liens_Carat.cib",m_Tomjob.m_CheminData);
	m_LienCible.LoadFile(NomFic);
	if(!m_LienCible.GetLien(m_Tomjob.m_NumeroCible,NrCibleJFC))
	{
		CString msg;
		msg.Format("La cible %d n'est pas trouvée dans le fichier %s",m_Tomjob.m_NumeroCible,NomFic);
		AfxMessageBox(msg);
		return (0);
	}
//	NrCibleJFC=m_Tomjob.m_NumeroCible; // Pour les tests

	m_Tomy.InitialiseCible(NrCibleJFC);

	// ou si l'on travaille avec des sous cibles
	// CTomy::InitialiseSousCible(int NrCible,int CritereFiltre,CWordArray * ModaliteFiltre)
	// __________________________________________

	// A ce point on peut obtenir des informations sur la cible du Panel
//	m_Restom.m_NbCas=m_Tomy.GetNbCas();
	m_Restom.m_NbCas.RemoveAll();
//	m_Restom.m_NbCas=m_Tomy.GetNbCas();
	if(flagMediadata)m_Restom.m_NbCas.Add(m_Tomy.GetNbCasMediadata());
	if(flagMediacabsat)m_Restom.m_NbCas.Add(m_Tomy.GetNbCasMediacabsat());

	double PopulationPanel=m_Tomy.GetPopulation();
	// Les résultats sont sur la population de référence sauf pour le CIM (2TUDE DIPTIQUE°
	m_Restom.m_Population=m_Tomjob.m_Population;
	double deltapop=m_Restom.m_Population/PopulationPanel; // pour calculer les ventilations
	// __________________________________________


	// nouvelle étape depuis la fusion à la vollée 
	// du panel Médiadata et Médiacabsat
	// NB: Carat ne nous donne pas le pop élargie pour le moment
	if(flagMediadata==1 && flagMediacabsat==1)
	{
		m_Tomy.CalculCoeffCorrectionOffreElargie(m_Tomjob.m_Population,GetPopulationEquipeCabSat(m_Tomjob.m_CheminData,"MEDIACABSATagrege",m_Tomjob.m_CodePanelMediaCabSat,NrCibleJFC));
	}

	
	// Etape 4
	// Evaluation des plans
	TRACE("*** Evaluation du(des) plans(s) ***\n");

	for(int p=0;p<m_Tomjob.m_NbPlan;p++)
	{
		CRestomCarat * Restom;
		Restom=&m_Restom;
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
		Restom->UpdateResultat(&m_Tomy,m_Restom.m_Population);

		// Courbe de réponse
		m_Restom.m_CourbeReponse.RemoveAll();
		for(int i=1;i<=param.m_NbClassesResultatDistribution;i++)
		{
			m_Restom.m_CourbeReponse.Add((WORD)m_Tomy.GetCourbeReponse(i));
		}

		// Calcul des PMG +++++++++++++++++++++++++++++++++++++++++
		TRACE("\n*** Calcul des PMG ***\n\n");
		const int NbPMG=3;
		for(int c=0;c<NbPMG;c++)
		{
			m_Tomy.ActiveFiltreIndividusStrateConsomation(c);
			m_Restom.m_CouverturePMG.Add(m_Tomy.LanceEval());
			if(flagMediadata==1 && flagMediacabsat==1)
				m_Tomy.EtalonneSousGRPCabsatAvecLesSousGRPHertzien();
			m_Restom.m_GRPPMG.Add(m_Tomy.GetGRP());
		}

		// Calcul de l'apport par jour +++++++++++++++++++++++++++++++
		TRACE("\n*** Calcul de l'apport par jour ***\n\n");
		TrouveJourActif();
		// on reprend depuis l'étape 3:
		m_Tomy.InitialiseCible(NrCibleJFC);
		m_Tomy.InitialiseSpot();
		for(int j=0;j<m_TabJour.GetSize();j++)
		{
			for(int s=0;s<m_Tomjob.m_Plan.GetSize();s++)
			{
				if(m_TabJour[j].m_Libelle==m_Tomjob.m_Plan[s].m_JourD)
				if(!AddEcranDansTomy(s,p))return(0);
			}
			// __________________________________________

			// Lancement du calcul
			m_TabJour[j].m_Individu=m_Tomy.LanceEval()*m_Restom.m_Population/100000.0;
			// __________________________________________
		}
		// On transmet l'info à la classe CRestom
		m_Restom.m_TabJour=&m_TabJour;


		// Calcul de l'apport par semaine
		EvaluerApportSemaine();

		// On transmet l'info à la classe CRestom
		m_Restom.m_TabSemaine=&m_TabSemaine;

		// Calcul de l'apport par semaine glissante
		EvaluerApportSemaineGlissante();

		// On transmet l'info à la classe CRestom
		m_Restom.m_TabSemaineGlissante=&m_TabSemaineGlissante;

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
		}	
	}
	// Ecrire le fichier restom
	m_Restom.EcritureFichier(NomFicResTom);

	return(1); //OK
}



