// TomyCIMBelge.cpp: implementation of the CTomyCIMBelge class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "TomyCIMBelge.h"
#include <math.h>

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

CPanelLoader CTomyCIMBelge::m_PanelLoader;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CTomyCIMBelge::CTomyCIMBelge()
{
	m_FicProba      = NULL;
	m_CoeffEx       = NULL;
	m_NbIndiv       = 0;
	m_NbHit         = NULL;
	m_PlStation     = NULL;
	m_TabSemAct     = NULL;
	m_PlanWhileZoom = NULL;
}

CTomyCIMBelge::~CTomyCIMBelge()
{
	FreeAllocIndiv();

	FreePanel();
}

void CTomyCIMBelge::SetParametres(CTomyParam * param)
{
	m_Param = *param;
}

void CTomyCIMBelge::InitialisePanel(LPCSTR panel,LPCSTR etude)
{
	ASSERT(etude=="CIMBELGE");

	// Si le panel est déjà chargé, il n'est pas necessaire de le recharger
	if(m_CodePanel == panel && m_Etude == etude) return;

	// Libération des allocation des individus
	FreeAllocIndiv();

	// Libération du panel
	FreePanel();

	// Chargement du panel
	m_CodePanel = panel;
	m_Etude     = etude;
	m_FicProba  = LoadPanel(m_CodePanel, m_Etude);

	// Allocation des vecteurs des individus
	AllocIndivPourPanel();
}

void CTomyCIMBelge::InitialiseCible(int NrCible)
{
	AfxMessageBox("Utilisation d'une fonction obsolète dans le modèle CIM Belge !");

	m_NrCible = NrCible;

	LoadCible();
	CalculPopulation();
}

void CTomyCIMBelge::InitialiseCible(long * CoeffExCible)
{
	m_NrCible = -1;
	for(int i=0;i<m_NbIndiv;i++)
	{
		m_CoeffEx[i] = CoeffExCible[i];
#ifndef OPTION_ALGERIE 
		m_CoeffEx[i] /= 10.0;
#endif
	}
	CalculPopulation();
}

void CTomyCIMBelge::InitialiseCibleVentile(int NrCible,int NrCritere1 ,int NrModalite1,int NrCritere2,int NrModalite2)
{

}

void CTomyCIMBelge::InitialiseCibleVentile(long* CoeffExCible, char* NrModaliteDuCritere, int Modalite)
{
	m_NrCible = -1;
	for(int i = 0; i < m_NbIndiv; i++)
	{
		if (NrModaliteDuCritere[i] == Modalite)
			m_CoeffEx[i] = CoeffExCible[i];
		else
			m_CoeffEx[i] = 0;
#ifndef OPTION_ALGERIE 
		m_CoeffEx[i] /= 10.0;
#endif
	}
	CalculPopulation();
}

int CTomyCIMBelge::GetNbCas()
{
	return m_NbCas;
}

int CTomyCIMBelge::GetPopulation()
{
	return (int)(m_Population+.4999);
}

void CTomyCIMBelge::InitialiseSpot()
{
	m_Plan.SetSize(0,100);
}

double CTomyCIMBelge::LanceEvalDistrib()
{
	// Initialisation
	m_GRP = m_Couverture = 0.0;

	if (m_Population <= 0)
		return 0.0;

	// on détermine  3 sous plans:
	// CalculNrSemaineParQH()
	// note ce calcul est effectué lorsque le patron d'écoute est déterminé

	// Allocation automatique du vecteur binomiale par individu
	// note: alloué au chargement du panel

	// Initialisation du vecteur hit par individu
	for(int i=0;i<m_NbIndiv;i++)
	{
		for(int sp=0;sp<NBWEEK;sp++)
		{
			for(int w=0;w<NBWEEK;w++)
			{
				for(int p=0;p<NBPATRON;p++)
				{
					m_NbHit[i][sp][w][p]=0;
				}
			}
		}
	}

	// Boucle sur les supports du plan pour comptabiliser les contacts
	for(int s=0;s<m_Plan.GetSize();s++)
	{
		if(!TrouvePatronFrequentation(m_Plan[s]))continue;
		CalculFrequentationComptage(m_Plan[s]);
		if(m_Param.m_CallBack && (s%2))m_Param.m_CallBack->OneSpotEvaluated();
	}

	m_GRP=CalculMoyenne6Distribution(m_Distribution,NBPATRON);
	m_Couverture=m_Distribution.CalculCouverture_nPlus(1);

	return (m_Couverture);
}

double CTomyCIMBelge::GetAsymptote()
{
	return 0;
}

#ifdef OPTION_DUREEECOUTE
void CTomyCIMBelge::AddSpot(short Station, int JourDate,short JourNomme,short QHeure,double GRP,short NbInsertion)
{
	ASSERT(GRP>=0);
	CElement E;
	E.m_Station=Station;
	E.m_JourDate=JourDate;
	E.m_JourNomme=JourNomme;
	E.m_QHeure=QHeure%10000;
	E.m_OptionAncrage=(QHeure/10000)>0;
	E.m_GRP=GRP;
	E.m_NbInsertion=NbInsertion;
	m_Plan.Add(E);
}

void CTomyCIMBelge::UpdateSpot(short Station, int JourDate, short JourNomme,short QHeure,double GRP,short NbInsertion)
{
	// Cette fonction est désignée pour un fonctionnement "marginal"
	// elle permet de mettre à jour la liste des spots du plan

	// Cette fonction ne peut pas assumer la gestion de la dupplication
	// Le nombre d'insertion représente les spot sur-dupliqués

	CElement E;
	E.m_Station=Station;
	E.m_JourDate=JourDate;
	E.m_JourNomme=JourNomme;
	E.m_QHeure=QHeure%10000;
	E.m_OptionAncrage=(QHeure/10000)>0;
	E.m_GRP=GRP;
	E.m_NbInsertion=NbInsertion;

	if(NbInsertion>0)
	{
		// Ajout
		for(int e=0;e<m_Plan.GetSize();e++)
		{
			if(m_Plan[e].IsEquivalent(&E))
			{
				// Ce support existe déjà.
				// Il suffit d'augmenter le nombre d'insterion
				m_Plan[e].m_NbInsertion+=NbInsertion;
				return;
			}
		}

		// Ce support est nouveau dans la liste.
		// Il faut l'ajouter
		m_Plan.Add(E);
		return;
	}
	else
	{
		// Suppression
		int nbtosuppress=-NbInsertion;
		for(int e=0;e<m_Plan.GetSize();e++)
		{
			if(m_Plan[e].IsEquivalent(&E))
			{
				// On calcul ce que l'on peut virer sur ce support
				int NbEnMoins=nbtosuppress;
				if (NbEnMoins>m_Plan[e].m_NbInsertion)
					NbEnMoins=m_Plan[e].m_NbInsertion;
				nbtosuppress-=NbEnMoins;

				// On réduit le support
				m_Plan[e].m_NbInsertion-=NbEnMoins;
				if(m_Plan[e].m_NbInsertion==0)
				{
					// On élimine le support si le nombre d'insertion est à 0
					m_Plan.RemoveAt(e);
				}
				if(nbtosuppress==0)
					return;
			}
		}
	}
	AfxMessageBox("Erreur dans le calcul marginal: un spot à supprimer n'est pas retrouvé dans la liste des spots du calcul en cours!");
}

void CTomyCIMBelge::AddEvenement(short Station, int JourDate,short JourNomme,short HoraireDebut,short Duree,double GRP)
{

}

#else
void CTomyCIMBelge::AddSpot(short Station,short JourNomme,short QHeure,double GRP,short NbInsertion)
{

}

void CTomyCIMBelge::UpdateSpot(short Station, short JourNomme,short QHeure,double GRP,short NbInsertion)
{

}
#endif

int CTomyCIMBelge::GetNbSpot()
{
	return m_Plan.GetSize();
}

long CTomyCIMBelge::GetNbInsertions()
{
	return m_NbInsertions;
}

void CTomyCIMBelge::SetNbInsertions(long NbIns)
{
	m_NbInsertions = NbIns;
}

double CTomyCIMBelge::GetCouverture()
{
	return m_Couverture;
}

void CTomyCIMBelge::SetCouverture(double Couv)
{
	m_Couverture = Couv;
}

double CTomyCIMBelge::GetCouvertureValideBeta()
{
	return 0;
}

double CTomyCIMBelge::GetCouvertureValideMinMax()
{
	return 0;
}

double CTomyCIMBelge::GetGRP()
{
	return m_GRP;
}

void CTomyCIMBelge::SetGRP(double GRP)
{
	m_GRP = GRP;
}

int CTomyCIMBelge::GetNbClasseResultat()
{
	return m_Param.m_NbClassesResultatDistribution;
}

double CTomyCIMBelge::GetExactementTouche(int categorie)
{
	return m_Distribution.GetValue(categorie);
}

double CTomyCIMBelge::GetCourbeReponse(int nbfoistouche)
{
	return 100;
}

void CTomyCIMBelge::ActiveFiltreIndividusStrateConsomation(int NrStrate)
{

}

double CTomyCIMBelge::LanceEval()
{
	return LanceEvalDistrib();
}

void CTomyCIMBelge::SavePlanBeforeZoom()
{
	m_PlanWhileZoom=new CtabElement;
	m_PlanWhileZoom->Copy(m_Plan);
}

void CTomyCIMBelge::RestorePlanAfterZoom()
{
	if(m_PlanWhileZoom==NULL)
	{
		ASSERT(0);
		return; // ???
	}
	m_Plan.Copy(*m_PlanWhileZoom);
	delete [] m_PlanWhileZoom;
	m_PlanWhileZoom=NULL;
}

CFicProba * CTomyCIMBelge::LoadPanel(LPCSTR Panel,LPCSTR Etude)
{
	// Cette fonction charge un nouveau panel
	// Cette fonction ne vérifie donc pas la necessité.
	// Si par hazard un autre panel était déjà chargé,
	// il faut commencer par l'indiquer au panelloader
	// pour qu'il puisse gerer les utilisations

	// Demande de chargement du panel à la classe panelloader.

	// NOTE: depuis le fonctionnement avec Médiacabsat, il faut indiquer un
	// sous répertoire pour trouver le panel

	// NOTE pour l'environement Radio, l'étude n'est pas utile...

	CString CheminPanel;
	CheminPanel = m_Param.m_CheminData;
	//TODO passer le nombre d individu
	CFicProba * FicProba=m_PanelLoader.LoadPanel(CheminPanel,Panel,m_Param.m_TypeSupport,0);

	TRACE("Passage normal dans PanelLoader()\n");
	AfxCheckMemory();

	// Après un chargement de panel, le nombre d'individu doit être actualisé.
	// Si il y a un sur echantillon, ce chiffre sera modifié ensuite.

	m_NbIndiv = FicProba->GetNbIndiv();

	return(FicProba);
}

int CTomyCIMBelge::LoadCible()
{
	CString FileName;
	FileName="CIBJOB";

	CStdioFile FicCibJob;
	CJFCFileException e;
	if(!FicCibJob.Open(FileName,CFile::modeRead  | CFile::typeBinary|CFile::shareDenyWrite,&e) )
	{
		CString MsgErr;
		MsgErr.Format("Erreur à l'ouverture du fichier %s : ",FileName);
		e.AfficheErreur(MsgErr);
		return(1); // Erreur nr 1
	}


	short *coeff=new short[m_NbIndiv];

	// FicPDX.Seek(m_NrCible*sizeof(long)*m_NbIndiv,CFile::begin);
	ASSERT(m_CoeffEx!=NULL);
	//FicCibJob.Read(m_CoeffEx,sizeof(long)*m_NbIndiv);
	FicCibJob.Read(coeff,sizeof(short)*m_NbIndiv);
	FicCibJob.Close();

	for(int i=0;i<m_NbIndiv;i++)
		m_CoeffEx[i]=(short)coeff[i];


	delete [] coeff;

	return (0); // OK, pas d'erreur
}


void CTomyCIMBelge::FreeAllocIndiv()
{
	if(!m_NbIndiv)return;
	if(m_CoeffEx)delete [] m_CoeffEx;
	m_CoeffEx=NULL;
	if(m_NbHit)
	{
		for(int i=0;i<m_NbIndiv;i++)
		{
			for(int sp=0;sp<NBWEEK;sp++)
			{
				for(int w=0;w<NBWEEK;w++)
				{
					delete [] m_NbHit[i][sp][w];
				}
				delete [] m_NbHit[i][sp];
			}
			delete [] m_NbHit[i];
		}
		delete [] m_NbHit;
	}
	m_NbHit=NULL;
	if (m_PlStation) delete [] m_PlStation;
	if (m_TabSemAct) delete [] m_TabSemAct;
//	if(m_CoeffExCible)delete [] m_CoeffExCible;
	m_NbIndiv=0;
	m_PlStation = NULL;
	m_TabSemAct = NULL;
}

void CTomyCIMBelge::FreePanel()
{
	if(m_FicProba)m_PanelLoader.UnloadPanel(m_FicProba);
	m_CodePanel="";
	m_Etude="";
	m_FicProba=NULL;
}

void CTomyCIMBelge::AllocIndivPourPanel()
{
	int NbStation;
	m_CoeffEx=new double [m_NbIndiv];
//	m_CoeffExCible=new long [m_NbIndiv];

	// Construction du lien avec les stations
	NbStation = m_FicProba->GetNbStation();
	m_PlStation = new short [NbStation];
	m_IndexStation.RemoveAll();
	for(int s=0;s<m_FicProba->GetNbStation();s++) {
		m_IndexStation.SetAt(m_FicProba->GetNrStation(s),s);
		m_PlStation[s] = 0;
	}

	m_NbHit=new short***[m_NbIndiv];
	for(int i=0;i<m_NbIndiv;i++)
	{
		m_NbHit[i]=new short**[NBWEEK];
		for(int sp=0;sp<NBWEEK;sp++)
		{
			m_NbHit[i][sp]=new short*[NBWEEK];
			for(int w=0;w<NBWEEK;w++)
			{
				m_NbHit[i][sp][w]=new short[NBPATRON];
			}
		}
	}
}

void CTomyCIMBelge::CalculPopulation()
{
	m_NbCas=0;
	m_Population=0;
	double pop=+0.0;
	double cas=+0.0;
	for(int i=0;i<m_NbIndiv;i++){
		if(m_CoeffEx[i]<=0)continue;
		m_Population+=m_CoeffEx[i];
		m_NbCas++;
	}
	//m_Population=(m_Population+5)/10;
}

int CTomyCIMBelge::TrouvePatronFrequentation(CTomyCIMBelge::CElement & E)
{
	int p;
	if(E.m_PatronEcoute[0][0]!=NULL)return 1; //OK
	for(p=0;p<NBWEEK;p++)
	{
		E.m_PatronEcoute[0][p]=NULL;
		E.m_PatronEcoute[1][p]=NULL;
	}

	int idxsta;
	if(!m_IndexStation.Lookup(E.m_Station,idxsta)){
		CString txt;
		txt.Format("Les probabilités de la station %d (numéro JFC) ne sont pas trouvées dans le panel chargé.",E.m_Station);
		AfxMessageBox(txt);
		ASSERT(0);
		return 0;
	}

	// calcul de l'index jour dans le fichier
	COleDateTime DateLundiReference(1990,1,1,12,0,0);
	int jd=E.m_JourDate;
	int an=jd/10000;
	jd%=10000;
	int mois=jd/100;
	jd%=100;
	int jour=jd;
	COleDateTime DateQH(an,mois,jour,15,0,0);
	COleDateTimeSpan Ecart;
	Ecart=DateQH-DateLundiReference;

	int NbJour=Ecart.GetDays();

	//NbJour+=7; // pour phaser avec le plan de test

	E.m_NrSousPlan=(NbJour%(NBWEEK*7))/7;
	ASSERT(NbJour%7==E.m_JourNomme);

	// POUR TEST COMPTAGE
	//NbJour=E.m_JourNomme;
	// POUR TEST COMPTAGE


	// Pour éviter que la semaine soit décalé, il faut déduire
	// le numéro du plan dans la semine:
	NbJour-=E.m_NrSousPlan*7;

	// calcul du numéro de quart d'heure
	int qhe=(E.m_QHeure/100)*4+(E.m_QHeure%100)/15;
	qhe-=20; // Phasage à 5H00

	if(qhe<0 || qhe>=96)
	{
		CString txt;
		txt.Format("Un support décrit un quart d'heure qui n'est pas valide (de 500 à 2845): %d",E.m_QHeure);
		AfxMessageBox(txt);
		ASSERT(0);
		return 0;
	}


	// Choix du quart d'heure (numéro 1 ou 2)
	int PosQH1=0;
	int PosQH2=1;

	if(!(((E.m_QHeure/100)+((E.m_QHeure%100)/15)+1)%2)) // Si paire
	{
		PosQH2=0;// int PosQH2=0;	// Liem 24/05/04
		PosQH1=1;// int PosQH1=1;
	}


	for(p=0;p<NBWEEK;p++)
		E.m_PatronEcoute[PosQH1][p]=m_FicProba->GetDureeEcoute(idxsta,(NbJour+(p*7))%(7*NBWEEK),qhe);

	if(E.m_OptionAncrage)
	{
		// On prend le quart d'heure qui suit
		qhe++;
		if (qhe>=96)
			NbJour++;
		for(p=0;p<NBWEEK;p++)
			E.m_PatronEcoute[PosQH2][p]=m_FicProba->GetDureeEcoute(idxsta,(NbJour+(p*7))%(7*NBWEEK),qhe);
	}
	else
	{
		// C'est le même patron d'écoute
		for(p=0;p<NBWEEK;p++)
			E.m_PatronEcoute[PosQH2][p]=E.m_PatronEcoute[PosQH1][p];
	}

	return 1; // La Proba est trouvée
}

void CTomyCIMBelge::CalculFrequentationComptage(CTomyCIMBelge::CElement & E)
{
//	TRACE("\nDebut du comptage dans une cominatoire de semaine donné:\n");
	int sp=E.m_NrSousPlan;
	int nbinsert=E.m_NbInsertion;
	for(int w=0;w<NBWEEK;w++)
	{
		for(int NrAncrage=0;NrAncrage<NBPATRON;NrAncrage++)
		{
			//TRACE("\nComptage dur la semaine %d\n",w+1);
			for(int i=0;i<E.m_PatronEcoute[NrAncrage][w]->m_NbCas;i++)
			{
				// Note: inutile de filtrer les individus de la cible,
				// car cela va être plus long que l'incrément (++)
				m_NbHit[E.m_PatronEcoute[NrAncrage][w]->m_NrIndividu[i]][sp][w][NrAncrage]+=nbinsert;
				//TRACE("Individu %d\n",E.m_PatronEcoute[NrPatron][w]->m_NrIndividu[i]);
			}
		}
	}
}


double CTomyCIMBelge::CalculMoyenne6Distribution(CDistribution &ResultatDistribution,int NbMoy)
{
#ifdef OPTION_REACH_BELGE
	const int NbResultat = 1; // Reach
#elif OPTION_ALGERIE
	const int NbResultat = 1;
#else
	const int NbResultat = 6;
#endif
	CDistribution Distrib[NbResultat];
	double GRP=0;
	short Combinatoire[6][3];
	Combinatoire[0][0]=0;	Combinatoire[0][1]=1;	Combinatoire[0][2]=2;
	Combinatoire[1][0]=1;	Combinatoire[1][1]=2;	Combinatoire[1][2]=0;
	Combinatoire[2][0]=2;	Combinatoire[2][1]=0;	Combinatoire[2][2]=1;
	Combinatoire[3][0]=0;	Combinatoire[3][1]=2;	Combinatoire[3][2]=1;
	Combinatoire[4][0]=1;	Combinatoire[4][1]=0;	Combinatoire[4][2]=2;
	Combinatoire[5][0]=2;	Combinatoire[5][1]=1;	Combinatoire[5][2]=0;

	CDistribution DistribAncrage[NBPATRON];

	for(int r=0;r<NbResultat;r++)
	{
		double GRPancrage=CalculDistribution(DistribAncrage[0],Combinatoire[r][0],Combinatoire[r][1],Combinatoire[r][2],0);
		if (NbMoy == 2)
			GRPancrage+=CalculDistribution(DistribAncrage[1],Combinatoire[r][0],Combinatoire[r][1],Combinatoire[r][2],1);
		GRP+=GRPancrage/NbMoy;
		CalculMoyenne2Distribution(Distrib[r],DistribAncrage[0],DistribAncrage[1],NbMoy);
		TRACE("Couverture Evaluation %d%d%d: %f\n",Combinatoire[r][0],Combinatoire[r][1],Combinatoire[r][2],Distrib[0].CalculCouverture_nPlus(1)*100/m_Population);
	}
	GRP/=NbResultat;
	// Calcul de la moyenne des 6
	ResultatDistribution.CreateNewDistribution(m_Param.m_NbClassesResultatDistribution+1);
	for(int c=0;c<=m_Param.m_NbClassesResultatDistribution;c++)
	{
		double moyenne=0;
		for(int r=0;r<NbResultat;r++)
		{
			moyenne+=Distrib[r].GetValue(c);
		}
		moyenne/=NbResultat;
		ResultatDistribution.SetValue(c,moyenne);
	}

	ResultatDistribution*=100.0/m_Population;
	GRP*=100.0/m_Population;
	return GRP;
}


double CTomyCIMBelge::CalculMoyenne6DistributionSurPatron(CDistribution &ResultatDistribution,int patron)
{
	const int NbResultat=6;
	CDistribution Distrib[NbResultat];
	double GRP=0;

	GRP+=CalculDistribution(Distrib[0],0,1,2,patron);
	TRACE("Couverture Evaluation 012: %f\n",Distrib[0].CalculCouverture_nPlus(1)*100/m_Population);
	GRP+=CalculDistribution(Distrib[1],1,2,0,patron);
	TRACE("Couverture Evaluation 120: %f\n",Distrib[1].CalculCouverture_nPlus(1)*100/m_Population);
	GRP+=CalculDistribution(Distrib[2],2,0,1,patron);
	TRACE("Couverture Evaluation 201: %f\n",Distrib[2].CalculCouverture_nPlus(1)*100/m_Population);
	GRP+=CalculDistribution(Distrib[3],0,2,1,patron);
	TRACE("Couverture Evaluation 021: %f\n",Distrib[3].CalculCouverture_nPlus(1)*100/m_Population);
	GRP+=CalculDistribution(Distrib[4],1,0,2,patron);
	TRACE("Couverture Evaluation 102: %f\n",Distrib[4].CalculCouverture_nPlus(1)*100/m_Population);
	GRP+=CalculDistribution(Distrib[5],2,1,0,patron);
	TRACE("Couverture Evaluation 210: %f\n",Distrib[5].CalculCouverture_nPlus(1)*100/m_Population);

	GRP/=NbResultat;

	ResultatDistribution.CreateNewDistribution(m_Param.m_NbClassesResultatDistribution+1);
	for(int c=0;c<=m_Param.m_NbClassesResultatDistribution;c++)
	{
		double moyenne=0;
		for(int r=0;r<NbResultat;r++)
		{
			moyenne+=Distrib[r].GetValue(c);
		}
		moyenne/=NbResultat;
		ResultatDistribution.SetValue(c,moyenne);
	}

	ResultatDistribution*=100.0/(double)m_Population;
	GRP*=100.0/(double)m_Population;
	return GRP;
}

void CTomyCIMBelge::CalculMoyenne2Distribution(CDistribution &ResultatDistribution,CDistribution &Resultat1,CDistribution &Resultat2,int NbMoy)
{
	ResultatDistribution.CreateNewDistribution(m_Param.m_NbClassesResultatDistribution+1);
	for(int c=0;c<=m_Param.m_NbClassesResultatDistribution;c++)
	{
		double moyenne=Resultat1.GetValue(c);
		if (NbMoy == 2) {
			moyenne = (moyenne + Resultat2.GetValue(c))/2;
		}
		ResultatDistribution.SetValue(c,moyenne);
	}
}


// Calcul de la distribution pour la combinaison donnée:
double CTomyCIMBelge::CalculDistribution(CDistribution &Distrib,int s1,int s2,int s3,int patron)
{
	Distrib.CreateNewDistribution(m_Param.m_NbClassesResultatDistribution+1);
	Distrib.Initialise();
	double GRP=0;
	for(int i=0;i<m_NbIndiv;i++)
	{
		// Cet individu est-il dans la cible ?
		double CoeffEx=m_CoeffEx[i];
		if(CoeffEx<=0)continue;
		// Combien de fois cet individu est-il touché ?
#ifdef OPTION_REACH_BELGE
		short nbhit=m_NbHit[i][0][s1][patron];//REACH
#elif OPTION_ALGERIE
		short nbhit=m_NbHit[i][0][s1][patron];//ALGERIE
#else
		short nbhit=m_NbHit[i][0][s1][patron]+m_NbHit[i][1][s2][patron]+m_NbHit[i][2][s3][patron]; // BELGIQUE
#endif
		if(nbhit==0)continue;
		GRP+=nbhit*CoeffEx;
		if(nbhit>m_Param.m_NbClassesResultatDistribution)
			nbhit=m_Param.m_NbClassesResultatDistribution;
		Distrib.AddValue(nbhit,CoeffEx);
	}
	return GRP;
}
#ifdef OPTION_REACH_BELGE
int CTomyCIMBelge::CalculOffre()
{
//	double CouvM,CouvW,*CouvWi,CouvY;
	int Jou,Jmin,Jmax,NbS,i;

	DelAllStationOffre();
	m_CouvertureWK = m_CouvertureMJ = m_CouvertureY = 0.0;
	// Chercher la période du plan et toutes stations
	Jmin = 32700;
	Jmax = 0;
	for(int s=0;s<m_Plan.GetSize();s++)
	{
		AddStationOffre(m_Plan[s].m_Station);
		Jou = GetJourDate(m_Plan[s].m_JourDate);
		if (Jou < Jmin) Jmin = Jou;
		if (Jou > Jmax) Jmax = Jou;
	}
	Jmin /= 7;		// 1ère semaine du plan
	Jmax /= 7;		// Dernière semaine du plan
	NbS = max(0,1+Jmax-Jmin);
//	if (!NbS) return 0;
	//m_Tomy.SetNbSemMax(NbS);
	m_NbSemMax = NbS;
	m_TabSemAct = new short [NbS];
	for(s=0;s<NbS;s++) {
		m_TabSemAct[s] = 0;
	}
	// Las semaines actives
	for(s=0;s<m_Plan.GetSize();s++)
	{
		Jou = GetJourDate(m_Plan[s].m_JourDate);
		m_TabSemAct[(Jou/7)-Jmin] = Jou/7;
	}
	i = 0;
	for(s=0;s<NbS;s++) {
		if (m_TabSemAct[s]) i++;
	}
	m_NbSemPlan = i;
	// Calculer les couvertures d'offres
	CalculOffreW();
	CalculCouvWi();
	return 1;
}
int CTomyCIMBelge::AddStationOffre(int m_Station)
{
	int idxsta=-1;
	if(m_IndexStation.Lookup(m_Station,idxsta)) {
			m_PlStation[idxsta] = 1;

	}
	return idxsta;
}
void  CTomyCIMBelge::DelAllStationOffre()
{
	for (int s=0;s<m_FicProba->GetNbStation();s++)
		m_PlStation[s] = 0;
	if (m_TabSemAct) delete [] m_TabSemAct;


}
int CTomyCIMBelge::GetJourDate(int JourDate)
{
	int NbJ;
	// calcul de l'index jour dans le fichier
	COleDateTime DateLundiReference(1990,1,1,0,0,0);
	int jd=JourDate;
	int an=jd/10000;
	jd%=10000;
	int mois=jd/100;
	jd%=100;
	int jour=jd;
	COleDateTime DateQH(an,mois,jour,0,0,0);
	COleDateTimeSpan Ecart;
	Ecart=DateQH-DateLundiReference;
	NbJ=Ecart.GetDays();
	return NbJ;
}


void CTomyCIMBelge::CalculOffreW()
{
	CProbaSupport * E;
	CEverReach   * Ev;
	int Ancra=0;

	m_CouvertureWK = m_CouvertureMJ = m_CouvertureY = 0.0;
	if(m_Population<=0)return;
	// Calculer la couverture cumulée après une semaine de panel
	for(int i=0;i<m_NbIndiv;i++)
	{
		for(int sp=0;sp<NBWEEK;sp++)
		{
			for(int w=0;w<NBWEEK;w++)
			{
				m_NbHit[i][sp][w][Ancra]=0;		// Ancra = NBPATRON
			}
		}
	}
	for (int s=0;s<m_FicProba->GetNbStation();s++)
	{
		if (!m_PlStation[s]) continue;
		for (int j=0;j<7;j++) {
			for (int qhe =0;qhe<96;qhe++) {
				E = m_FicProba->GetDureeEcoute(s,j,qhe);
				for(int i=0;i<E->m_NbCas;i++)
				{
					m_NbHit[E->m_NrIndividu[i]][0][0][Ancra]=1;
				}
			}
		}
	}
	m_GRP=CalculMoyenne6Distribution(m_Distribution,1);
	m_CouvertureWK=m_Distribution.CalculCouverture_nPlus(1);
	// Calculer la couverture cumulée après un jour moyen du panel
	m_CouvertureMJ = m_GRP = 0.0;
	for (int j=0;j<7;j++) {
	// Initialisation du vecteur hit par individu
		for(int i=0;i<m_NbIndiv;i++)
		{
			for(int sp=0;sp<NBWEEK;sp++)
			{
				for(int w=0;w<NBWEEK;w++)
				{
					m_NbHit[i][sp][w][Ancra]=0;		// Ancra = NBPATRON
				}
			}
		}
		for (int s=0;s<m_FicProba->GetNbStation();s++)
		{
			if (!m_PlStation[s]) continue;
			for (int qhe =0;qhe<96;qhe++) {
				E = m_FicProba->GetDureeEcoute(s,j,qhe);
				for(int i=0;i<E->m_NbCas;i++)
				{
					m_NbHit[E->m_NrIndividu[i]][0][0][Ancra]=1;
				}
			}
		}
		m_GRP += CalculMoyenne6Distribution(m_Distribution,1);
		m_CouvertureMJ += m_Distribution.CalculCouverture_nPlus(1);
	}
	m_CouvertureMJ /= 7;
	m_CouvertureY = m_GRP = 0.0;
	for(i=0;i<m_NbIndiv;i++)
	{
		for(int sp=0;sp<NBWEEK;sp++)
		{
			for(int w=0;w<NBWEEK;w++)
			{
				m_NbHit[i][sp][w][Ancra]=0;		// Ancra = NBPATRON
			}
		}
	}
	for (s=0;s<m_FicProba->GetNbStation();s++)
	{
		if (!m_PlStation[s]) continue;
		Ev = m_FicProba->GetEcouteEVR(s);
		for(i=0;i<Ev->m_NbCas;i++)
		{
			m_NbHit[Ev->m_NrIndividu[i]][0][0][Ancra]=1;
		}
	}
	m_GRP += CalculMoyenne6Distribution(m_Distribution,1);
	m_CouvertureY = m_Distribution.CalculCouverture_nPlus(1);
}
double CTomyCIMBelge::GetCouvertureMJ()
{
	return m_CouvertureMJ;
}
double CTomyCIMBelge::GetCouvertureWK()
{
	return m_CouvertureWK;
}
double CTomyCIMBelge::GetCouvertureY()
{
	return m_CouvertureY;
}
void CTomyCIMBelge::SetNbSemMax(short NbS)
{
	m_NbSemMax = NbS;
}
void CTomyCIMBelge::SetNbSemPlan(short NbSa)
{
	m_NbSemPlan = NbSa;
}
void CTomyCIMBelge::SetCouvertureWi(double *CouvWi)
{
	m_CouvertureWi = CouvWi;
}
double * CTomyCIMBelge::GetCouvertureWi()
{
	return m_CouvertureWi;
}
short CTomyCIMBelge::GetNbSemMax()
{
	return m_NbSemMax;
}
short CTomyCIMBelge::GetNbSemPlan()
{
	return m_NbSemPlan;
}

double CTomyCIMBelge::fZeroFrequence(double x,double y,short z)
{
	double f;
	short k;
	f = 1;
	for (k=1 ; k<=z ; k++) {
		f = (f*(1 - y)*(x + k - 1))/(x + (k - 1)*(1 - y));
	}
	return f;
}

double CTomyCIMBelge::seCouverturePlan(double CouvM,double CouvW,double *CouvWi,double CouvY,short NbSem)
{
	// n = number of weeks in the schedule	(NbSem)
	// m = average all-time % daily reach  (CouvM)
	// w = all-time % weekly reach (CouvW)
	// y  =  % ever read (ever listened) (CouvY)
	// sw(i) = schedule % weekly reach for week i of the schedule (CouvWi)
	short n    = NbSem;
	double m   = CouvM;
	double w   = CouvW;
  	double *Wi = CouvWi;
  	double y   = CouvY;
	double t,se;
	double b,l,u,e,c,a,x,k,A;
	short d;

	if (!NbSem) return 0.0;
	m = m/y;								// Convert to
	w = w/y;								// proportions of
	for (int i = 0 ;i < n;i++) {	// ever read
		Wi[i] = Wi[i]/y;				// universe
	}
	y /= 100.0;
	if (w != 0) {								//  if (w = 0) go to 200 	Special case
		if (w < (1 - pow((1.0 - m),7.0))) {		//  if (w >= (1 - (1 - m)**7)) go to 110 	Special case
			t = 1 - w;							// 				Zero frequency
			b = 1;								// 				Initial value for
													// 				growth parameter
			if ((fabs (fZeroFrequence (b, m, 7) - t) >= 0.0001)) {		// if (abs (f (b, m, 7) - t) < 0.0001)) go to 100
																							// b = 1 might be
																							// the solution

				l = b;																	//	Find lower and
				u = b;																	//	upper limits for b
				if (fZeroFrequence (b, m, 7) > t) {
					while (fZeroFrequence (u, m, 7) >= t)										// do until	f(u,m,7) < t
						u = u +  u/10;
				}
				else {
					while (fZeroFrequence (l, m, 7) <= t)										// do until	f(u,m,7) > t
						l = l - 0.1;
				}

				b = u;																// Iterate to find
				while (fabs(fZeroFrequence (b, m,7) - t ) > 0.0001) {					// value for b
					b = (u + l) / 2;
					if (fZeroFrequence (b, m, 7) > t)
						l = b;
					else
						u = b;
				}
			}
// 		100
			d = 21;								// 	Days in schedule
			e = 1.0 - fZeroFrequence (b,m,d);	// 	Extended all
			// go to 120								// time reach
		}
//		110
		else {
			e = 1 - pow((1.0 - w),3);	//	Binomial default
		}

// 	120
		c = n+1;		// éviter owerflot
		if (w != 1 && e != 1) 	c = log(1-e)/log(1-w);

		if (c < 3)	{	// if c>=3 goto 300
			a = 1;
// 130
			do {
				x = a;
				a = pow((1-w),log(1+3*a)/log(1-e))-1;
			}
			while(fabs(x-a) > 0.0001 );
			k = log(1-e)/log(1+3*a);

			A = 0;
			for (i=0;i<n;i++)
				A = A+pow((1-Wi[i]),(1/k));	// 		-1;
			A = A-1;
			se = 100*(1-pow((1+A),k));
			se = se*y;
			return se;
		}
// 300
		else {
			A = 0;
			for (i=0;i<n;i++) {
				A = A-log(1-Wi[i]);
			}
			se = 100*(1-exp(-A));
			se = se*y;
			return se;
		}
	}
// 200
	else
		se = 0;		// Special case

	return se;	//	end
}

double *CTomyCIMBelge::seDistributPlan(double CouvSE,double GrpM,long NbSp)
{
	/*
	n  = number of spots in schedule
	se = extended schedule % reach
	sr = schedule average spot rating
	*/
	double se,sr,t;
	double b,l,u,a,p;
	int n,i;
	double *f;

	f = NULL;
	se = CouvSE/100;		// Convert to
	sr = GrpM/100;			// proportions
	n = NbSp;
	if (se != 0) {			// if se = 0 then go to 200	Special case
		if (se < (1 - pow((1 - sr),n))) {	// if se >= (1 - (1 - sr)**n)go to 300				Special case
			t = 1 - se;						// 															Zero frequency

			b = 1;							// 															Initial value for
												// 															growth parameter

			if (fabs (fZeroFrequence(b,sr,n)-t) >= 0.0001)	{// if (abs (f(b,sr,n)-t)<0.0001) then go to 100

				l = b;						// 								Find lower and
				u = b;						// 								upper limits for b
				if (fZeroFrequence(b,sr,n)>t) {
					while(fZeroFrequence(u,sr,n)>=t) {		// do until f(u,sr,n)<t
						u = u + u/10;
					}									// loop
				}
				else {
					while(fZeroFrequence(l,sr,n) <=t) { 		// do until f(l,sr,n)>t
						l = l - 0.1;
					}									// loop
				}

				b = u;								// Iterate to find
				while (fabs (fZeroFrequence(b,sr,n)-t)>0.0001) {	// 				value for b
					b = (u + l)/2;
					if (fZeroFrequence(b,sr,n)>t)
						l = b;
					else
						u = b;
				}		// loop
			}
//	100
			a = b*sr/(1 - sr);						// Scale parameter
			f = new double [n+1];

			f[0] = 100*(1 - se);						// Zero frequency
			for (i = 1 ;i <= n; i++) {				// to n
				f[i] = (a + i - 1)*(n - i + 1)*f[i - 1]/((b + n - i)*i);		// Frequency i spots
			}		// next i
			return f;	// end
		}

//	300
		else {
			f = new double [n+1];
			p = 1 - pow((1 - se),(1.0/n));		// 					Binomial default
			f[0] = 100*(1 - se);
			for (i = 1;i <= n;i++ ){		//  to n
				f[i] = (p/(1 - p))*((double)(n - i +1) / (double)i )*f[i - 1];
			}		// next i
			return f;		// end
		}
	}
//	200
	else {
		f = new double [n+1];
		f[0] = 100.0;					// 				Special case
		for (int i = 1; i<= n;i++) {	// 				to n
			f[i] = 0.0;
		} 		// next i
		return f;	// 	end
	}

}
double CTomyCIMBelge::seResDistributPlan(double *TabDist,long NbSpot)
{
	int Nb;
	double GRP=0.0;
	double Couv = TabDist[0];

	m_Distribution.CreateNewDistribution(m_Param.m_NbClassesResultatDistribution+1);
	m_Distribution.Initialise();

	for(int c=1;c<=NbSpot;c++)
	{
		GRP += c*TabDist[c];
		Nb = c;
		if(Nb>m_Param.m_NbClassesResultatDistribution)
			Nb=m_Param.m_NbClassesResultatDistribution;
		m_Distribution.AddValue(Nb,TabDist[c]);// m_Distribution.SetValue(Nb,TabDist[c]);
	}
	GRP = (double)m_Population*GRP/100;
	return Couv;
}




short * CTomyCIMBelge::GetTabSemAct()
{
	return m_TabSemAct;
}




void CTomyCIMBelge::CalculCouvWi()
{
	if(m_Population<=0)return;

	
	double *CouvWi = new double [m_NbSemPlan];
	// Couverture cumulée de la semaine sem du plan : m_CouvertureWi
	int NbS=0;
	for (int Sem=0;Sem<m_NbSemMax;Sem++) {
		if (!m_TabSemAct[Sem]) continue;
		m_GRP=m_Couverture=+0.0;
		// Initialisation du vecteur hit par individu
		for(int i=0;i<m_NbIndiv;i++)
		{
			for(int sp=0;sp<NBWEEK;sp++)
			{
				for(int w=0;w<NBWEEK;w++)
				{	
					for(int p=0;p<NBPATRON;p++)
					{
						m_NbHit[i][sp][w][p]=0;
					}
				}
			}
		}
		// Boucle sur les supports du plan pour comptabiliser les contacts
		for(int s=0;s<m_Plan.GetSize();s++)
		{	
			int JouD = GetJourDate(m_Plan[s].m_JourDate);
			if (m_TabSemAct[Sem] != JouD/7) continue;
			if(!TrouvePatronFrequentation(m_Plan[s]))continue;
			CalculFrequentationComptage(m_Plan[s]);
			if(m_Param.m_CallBack && (s%2))m_Param.m_CallBack->OneSpotEvaluated();
		}

		m_GRP=CalculMoyenne6Distribution(m_Distribution,NBPATRON);
		CouvWi[NbS] = m_Distribution.CalculCouverture_nPlus(1);
		NbS++;
	}
	m_CouvertureWi = CouvWi;
	// Une petite vérification de la mémoire...
	ASSERT(_CrtCheckMemory());
}

double CTomyCIMBelge::CalculGRPDansPlan()
{
	double GRP=0.0;
	for(int i=0;i<m_Plan.GetSize();i++)
	{
		GRP+=m_Plan[i].m_GRP*(short)m_Plan[i].m_NbInsertion;
	}
	TRACE("GRP dans le job=%f\n",GRP);
	return GRP;
}
long CTomyCIMBelge::CalculNbSpotDansPlan()
{
	long NbSp=0;
	for(int i=0;i<m_Plan.GetSize();i++)
	{
		NbSp +=(short)m_Plan[i].m_NbInsertion;
	}
	TRACE("NbSpot dans le job=%f\n",NbSp);
	return NbSp;
}
#endif
