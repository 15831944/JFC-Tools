// Tomy.cpp: implementation of the CTomy class.
//
/*

Tomy se veut un moteur de calcul des couvertures rapide.
Il permet le calcul rapide des couvertures.
Il permet aussi le calcul (plus long) des distributions
Il est monocible, monosource, monor�gion.
Il ne calcul pas l'apport en marginal
Il n'ex�cute pas une s�rie de ventilation, mais permet par contre le calcul d'une sous-cible
Il ne dispose pas d'un r�servoir de panel, partag� par de multiples applications

Rapidit�:
Trusquin simplifi� si MW<2
Qualit� :
Degauchisseuse des contacts si nbspot<NBCLASSEDEGAU
Cela signifie que:
On ne calculera jamais au dessus de 20
Avec 10 et mois, la distribution restitue TOUJOURS les contacts
Entre 10 et 20, la distribution restitue progressivement moins les contacts perdus,
mais il faut savoir que ceux-ci deviennent normalement minimes, puisque la
dose de contacts perdue est dut au trusquin, et � la plurifr�quentation;
associ� � la coh�rence de nombre de classe <= nombre spots !
*/
///////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Tomy.h"
#include "ficaqj.h"
#include "ficaej.h"
#include "ficAEJ_SurEchantillon.h"
#include <math.h>



#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

// Liaison avec la dll panelloader
#ifdef PANELSHARED
CPanelLoader * GetPtrPanelLoader();
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

//##ModelId=3D9AB7D201F3
static CPanelLoader GPanelLoader;
CTomy::CTomy()
{
	int p;
	m_NT=NULL;
	m_VB=NULL;
	m_NbVB=NULL;
	m_VBs=NULL;
	m_P=new double [BASEPROBA+1];
	m_Q=new double [BASEPROBA+1];
	for(p=0;p<=BASEPROBA;p++){
		m_P[p]=(double)p/(double)BASEPROBA;
		m_Q[p]=+1.0-m_P[p];
	}

	m_Trusquin=new double * [BASEPROBA+1];
	for(p=0;p<=BASEPROBA;p++){
		m_Trusquin[p]=new double [MWMAX+1];
		m_Trusquin[p][0]=0; // Non utilis�
		m_Trusquin[p][1]=m_P[p];
		m_Trusquin[p][1]=m_P[p];
		
		for(int i=0;i<=MWMAX;i++)m_Trusquin[p][i]=1-pow(m_Q[p],i);
	}

	m_CoeffEx=NULL;
	m_CoeffExCible=NULL;
	m_CoeffExSousCible=NULL;
	m_FiltreIndividus=NULL;
	m_FiltreIndividusPermanent=NULL;

//			m_Plan.GetSize()


	m_CodePanel="";
	m_FicProba=NULL;
	m_Param.m_CheminData="";

	// Connexion avec le chargeur de panel
#ifdef PANELSHARED
	// Liaison avec le panelLoader partag�
	// Utilisation de la dll PanelLoader
	// Dans ce cas les vagues qui seront charg�es seront partag�es
	m_PanelLoader=GetPtrPanelLoader();
#else
	// Utilisation du PanelLoader "en dur"
	// Dans ce cas chaque instance de tomy charge la vague du panel
	// sur lequel il travaille.
//	m_PanelLoader=new CPanelLoader;
	m_PanelLoader=&GPanelLoader;
#endif
	m_PlanWhileZoom=NULL;
}

//##ModelId=3D9AB7D201FB
CTomy::~CTomy()
{
	delete [] m_P;
	delete [] m_Q;
	for(int p=0;p<=BASEPROBA;p++){
		delete [] m_Trusquin[p];
	}
	delete [] m_Trusquin;
	
	FreeAllocIndiv();
	
	FreePanel();

}

//##ModelId=3D9AB7D201E8
void CTomy::InitialiseSpot()
{
//	m_Plan.RemoveAll();
	m_Plan.SetSize(0,100);
}

#ifdef OPTION_DUREEECOUTE
//##ModelId=3D9AB7D201C1
void CTomy::AddSpot(short Station, int JourDate,short JourNomme,short QHeure,double GRP,short NbInsertion)
{
	ASSERT(GRP>=0);
	CElement E;
	E.m_Station=Station;
	E.m_JourDate=JourDate;
	E.m_JourNomme=JourNomme;
	E.m_QHeure=QHeure;
	E.m_HoraireDebut=-1; // Evenement
	E.m_Duree=-1; // Evenement
	E.m_GRPgrp=E.m_GRP=GRP;
	E.m_NbInsertion=NbInsertion;
	E.m_Media=0; // Utilis� dans TomyFusion
	m_Plan.Add(E);
}

//##ModelId=3D9AB7D201A1
double CTomy::CalculSousGRPSpot(short Station, int JourDate,short JourNomme,short QHeure,double GRP)
{
	CElement E;
	E.m_Station=Station;
	E.m_JourDate=JourDate;
	E.m_JourNomme=JourNomme;
	E.m_QHeure=QHeure;
	E.m_GRPgrp=E.m_GRP=GRP;

	m_GRP=+0.0;
	if(m_Population<=0)return 0;

	if(m_flagSousCible || m_flagVentilation){
		if(!TrouveProba(E))return 0;
		CalculMWventile(E);
		return(m_GRP);
	}
	else {
		// Pas besoin de perdre du temps
		// on retourne directement le GRP
		return(GRP);
	}
}

//##ModelId=3D9AB7D201AE
double CTomy::CalculSousGRPEvenement(short Station, int JourDate,short JourNomme,short HoraireDebut,short Duree,double GRP)
{
	CElement E;
	E.m_Station=Station;
	E.m_JourDate=JourDate;
	E.m_JourNomme=JourNomme;
	E.m_QHeure=-1;
	E.m_HoraireDebut=HoraireDebut;
	E.m_Duree=Duree;
	E.m_GRPgrp=E.m_GRP=GRP;

	m_GRP=+0.0;
	if(m_Population<=0)return 0;

	if(m_flagSousCible || m_flagVentilation){
		if(!TrouveProba(E))return 0;
		CalculMWventile(E);
		return(m_GRP);
	}
	else {
		// Pas besoin de perdre du temps
		// on retourne directement le GRP
		return(GRP);
	}
}

//##ModelId=3D9AB7D201D5
void CTomy::AddEvenement(short Station, int JourDate,short JourNomme,short HoraireDebut,short Duree,double GRP)
{
	CElement E;
	E.m_Station=Station;
	E.m_JourDate=JourDate;
	E.m_JourNomme=JourNomme;
	E.m_QHeure=-1;
	E.m_HoraireDebut=HoraireDebut;
	E.m_Duree=Duree;
	E.m_GRPgrp=E.m_GRP=GRP;
	E.m_NbInsertion=1; // pas de gestion de la duplication pour les ev�nements
	E.m_Media=0; // Utilis� dans TomyFusion
	m_Plan.Add(E);
}

#else
void CTomy::UpdateSpot(short Station, short JourNomme,short QHeure,double GRP,short NbInsertion)
{
	// Cette fonction est d�sign�e pour un fonctionnement "marginal"
	// elle permet de mettre � jour la liste des spots du plan

	// Cette fonction ne peut pas assumer la gestion de la dupplication
	// Le nombre d'insertion repr�sente les spot sur-dupliqu�s

	CElement E;
	E.m_Station=Station;
	E.m_JourDate=0;
	E.m_JourNomme=JourNomme;
	E.m_QHeure=QHeure;
	E.m_HoraireDebut=-1; // Evenement
	E.m_Duree=-1; // Evenement
	E.m_GRPgrp=E.m_GRP=GRP;
	E.m_NbInsertion=NbInsertion;

	if(NbInsertion>0)
	{
		// Ajout
		m_Plan.Add(E);
		return;
	}
	else
	{
		// Suppression
		for(int e=m_Plan.GetSize()-1;e>=0;e--)
		{
			if(m_Plan[e].IsEquivalent(&E))
			{
				if(m_Plan[e].m_NbInsertion==-NbInsertion)
				{
					m_Plan.RemoveAt(e);
					return;
				}
			}
		}
	}
	AfxMessageBox("Erreur dans le calcul marginal: un spot � supprimer n'est pas retrouv� dans la liste des spots du calcul en cours!");
}

//fc sans dur�e d'�coute
void CTomy::AddSpot(short Station, short JourNomme,short QHeure,double GRP,short NbInsertion)
{
	ASSERT(GRP>=0);
	CElement E;
	E.m_Station=Station;
	E.m_JourDate=0;
	E.m_JourNomme=JourNomme;
	E.m_QHeure=QHeure;
	E.m_HoraireDebut=-1; // Evenement
	E.m_Duree=-1; // Evenement
	E.m_GRPgrp=E.m_GRP=GRP;
	E.m_NbInsertion=NbInsertion;
	E.m_Media=0; // Utilis� dans TomyFusion
	m_Plan.Add(E);
}
#endif

// Calcul une �valuation en couverture
// Retourne la couverture 1+
//##ModelId=3D9AB7D20184
double CTomy::LanceEval()
{
	// La d�gauchisseuse doit fonctionner pour les petits plan
	if(m_Plan.GetSize()<NBCLASSEDEGAU && m_Param.m_DisableBisource==0)
	{
		return(LanceEvalDistrib());
	}

	// Si on corrige l'hypoth�se d'ind�pendance
	// il faut obligatoirement passer par un calcul avec les distributions
	if(m_Param.GetCorrectionHypotheseIndependance())
	{
		return(LanceEvalDistrib());
	}

	// On pourrait liberer les 1Mo de la distribution � ce moment ?
	// if(param.m_FreeAutoVB)FreeVecteurBinomiale();

	m_GRP=m_Couverture=+0.0;
	
	if(m_Population<=0)return 0;

	// Initialisation du vecteur des non touch�s
	for(int i=0;i<m_NbIndiv;i++)m_NT[i]=+1.0;

	if(m_flagSousCible || m_flagVentilation)
	{
		for(int s=0;s<m_Plan.GetSize();s++)
		{
			if(!TrouveProba(m_Plan[s]))continue;
			CalculMWventile(m_Plan[s]);
			CalculApport(m_Plan[s]);
			if(m_Param.m_CallBack)m_Param.m_CallBack->OneSpotEvaluated();
		}
	}
	else {
		for(int s=0;s<m_Plan.GetSize();s++)
		{
			if(!TrouveProba(m_Plan[s]))continue;
			CalculMediaWeight(m_Plan[s]);
			CalculApport(m_Plan[s]);
			if(m_Param.m_CallBack)m_Param.m_CallBack->OneSpotEvaluated();
		}
	}

	// Une petite v�rification de la m�moire...
	ASSERT(_CrtCheckMemory());

	return (m_Couverture=CalculNonTouche());
}

// Calcul une �valuation en distribution
// Retourne la couverture 1+
//##ModelId=3D9AB7D2018D
double CTomy::LanceEvalDistrib()
{
	int i,s,t;
	// Calcul de la dimension de calcul: m_NbClassesCalculDistribution
	ASSERT(m_Param.m_NbClassesResultatDistribution>1);
	ASSERT(m_Param.m_NbClassesResultatDistribution<=NBCLASSEDEGAU);
	if(GetNbSpot()<NBCLASSEDEGAU && m_Param.m_DisableBisource==0)
	{
		m_NbClassesCalculDistribution=1+NBCLASSEDEGAU;
	}
	else
	{
		m_NbClassesCalculDistribution=1+m_Param.m_NbClassesResultatDistribution;
	}
	
	// Initialisation
	m_GRP=m_Couverture=+0.0;
	
	if(m_Population<=0)return 0;

	// Allocation automatique du vecteur binomiale par individu
	if(m_VB==NULL)
	{
		AllocVecteurBinomiale();
	}
	
	// Initialisation du vecteur binomiale par individu
	for(i=0;i<m_NbIndiv;i++)
	{
		m_VB[i][0]=+1.0;
		m_NbVB[i]=0;
		for (t=1;t<m_NbClassesCalculDistribution;t++)
		{
			m_VB[i][t]=+0.0;
		}
	}
	
	// Pour �viter les d�bordements dans les petits plans,
	// on limite la distribution au nombre de spot
	int nbspot=GetNbSpot();
	int nbecran=m_Plan.GetSize();
	if(m_Param.m_DisableBisource==0)
	{
		m_Cmax=min(m_NbClassesCalculDistribution,nbspot+1);
	}
	else
	{
		m_Cmax=m_NbClassesCalculDistribution;
	}
	ASSERT(m_Cmax>0);
	//m_Cmax=m_NbClassesCalculDistribution;
	

	if(m_flagSousCible || m_flagVentilation){
		for(s=0;s<nbecran;s++){
			if(!TrouveProba(m_Plan[s]))continue;
			CalculMWventile(m_Plan[s]);
			CalculDistribution(m_Plan[s]);
			if(m_Param.m_CallBack)m_Param.m_CallBack->OneSpotEvaluated();
		}
	}
	else {
		for(s=0;s<nbecran;s++){
			if(!TrouveProba(m_Plan[s]))continue;
			CalculMediaWeight(m_Plan[s]);
			CalculDistribution(m_Plan[s]);
			if(m_Param.m_CallBack)m_Param.m_CallBack->OneSpotEvaluated();
		}
	}

	// Pour le m�thode par comptage, on met � jour le vecteur binomiale de chaque individu
	if(m_Param.m_MethodeComptage==1)
	{	// METHODE PAR COMPTAGE
		for(i=0;i<m_NbIndiv;i++)
		{
			if(m_CoeffEx[i]<=0)continue;
			m_VB[i][0]=0.0;
			if(m_NbVB[i]>=m_Cmax)m_NbVB[i]=m_Cmax-1;
			m_VB[i][m_NbVB[i]]=+1.0;
		}
	}

	// On met � jour le vecteur des non-touch�s
	for(i=0;i<m_NbIndiv;i++)m_NT[i]=m_VB[i][0];
	
	// Int�gration des individus
	m_Couverture=CalculExactementTouche();

	// Une petite v�rification de la m�moire...
	ASSERT(_CrtCheckMemory());

	return (m_Couverture);
}


// Lancement de l'�valuation avec 1 ou 2 crit�res-classes
// La population de la cible ne change pas.
// pour calculer une vrai ventilation il faut utiliser InitialiseCibleVentile()
//##ModelId=3D9AB7D2018F
double CTomy::LanceEvalVentile(int NrCritere1 ,int NrModalite1,int NrCritere2,int NrModalite2)
{
	RestoreCible();
	if((NrCritere1<0 || NrModalite1<0) && (NrCritere2<0 || NrModalite2<0))return(LanceEval());
	if(NrCritere1>=0 && NrModalite1>=0)IntegreVentilation(NrCritere1,NrModalite1);
	if(NrCritere2>=0 && NrModalite2>=0)IntegreVentilation(NrCritere2,NrModalite2);

	return(LanceEval());
}

//##ModelId=3D9AB7D202CE
int CTomy::TrouveProba(CTomy::CElement & E)
{
	int i,c,s;
	int qhe=E.m_QHeure;
	int jd=E.m_JourDate;
	int jn=E.m_JourNomme;

	if(E.m_Station<0)	// CREATION D'UNE PROBABILITE SUR TOUTES LES STATIONS
	{
		CDWordArray VProba;
		VProba.SetSize(m_NbIndiv);
		for(i=0;i<m_NbIndiv;i++)VProba[i]=0;
		CElement E2;
		int NbStation=m_FicProba->GetNbStation();
		for(s=0;s<NbStation;s++)
		{
			E2=E;
			E2.m_Station=m_FicProba->GetNrStation(s);
			TrouveProba(E2);
			for(int c=0;c<E2.m_ProbaSupp->m_NbCas;c++)
			{
				VProba[E2.m_ProbaSupp->m_NrIndividu[c]]+=E2.m_ProbaSupp->m_Proba[c];
			}
		}
		c=0;
		for(i=0;i<m_NbIndiv;i++)
		{
			VProba[i]/=NbStation;
			if(VProba[i]>0)c++;
		}

		// Maintenant on cr�e ce nouveau support probabilis�
		E.m_fFreeProba=1;
		E.m_ProbaSupp=new CProbaSupport;
		E.m_ProbaSupp->SetNbCas(c);
		c=0;
		for(i=0;i<m_NbIndiv;i++)
		{
			if(VProba[i]>0)
			{
				E.m_ProbaSupp->m_NrIndividu[c]=i;
				E.m_ProbaSupp->m_Proba[c]=(unsigned short)VProba[i];
				ASSERT(E.m_ProbaSupp->m_Proba[c]>0 && E.m_ProbaSupp->m_Proba[c]<=1000);
				// Comme pour l'instant les fichiers sont faux, et qu l'on veut travailler
				// sans que cela plante....
				if(E.m_ProbaSupp->m_Proba[c]>1000)E.m_ProbaSupp->m_Proba[c]=1000;
				c++;
			}
		}
		return 1;
	}

	if(qhe<0)	// CREATION D'UNE PROBABILITE SUR UN EVENEMENT
	{
		CDWordArray VProba;
		VProba.SetSize(m_NbIndiv);
		for(i=0;i<m_NbIndiv;i++)VProba[i]=0;
		CElement E2;
		
		int HorairePassage=E.m_HoraireDebut;
		int Duree=(E.m_Duree+30)/60;
		
		if(Duree==0)Duree=1;
//		ASSERT(Duree<180);

//		ASSERT(!(E.m_Station==1 && E.m_JourDate==0 && E.m_HoraireDebut==1198));

		int QH=(HorairePassage-180)/15;
		int Poids=min(15-HorairePassage%15,Duree);
		int Sigma=Poids;
		Duree-=Poids;
		if(Duree==0)
		{
			// Ce cas signifie que l'�v�nement n'est repr�sent� que par un
			// seul QHeure. Pas besoin de predred u temps et de la m�moire...
			E.m_QHeure=QH;
			return(TrouveProba(E));
		}
		E2=E;
		E2.m_QHeure=QH;
		TrouveProba(E2);
		for(c=0;c<E2.m_ProbaSupp->m_NbCas;c++)
		{
			VProba[E2.m_ProbaSupp->m_NrIndividu[c]]+=Poids*E2.m_ProbaSupp->m_Proba[c];
		}
		while(Duree>0)
		{
			QH++;
			if(QH>=96)
			{
				QH=0;
				E2.m_JourDate++;
				E2.m_JourNomme++;
				E2.m_JourNomme%=7;
			}
			Poids=min(15,Duree);
			Sigma+=Poids;
			E2.m_QHeure=QH;
			TrouveProba(E2);
			for(c=0;c<E2.m_ProbaSupp->m_NbCas;c++)
			{
				VProba[E2.m_ProbaSupp->m_NrIndividu[c]]+=Poids*E2.m_ProbaSupp->m_Proba[c];
			}
			Duree-=Poids;
		}
		c=0;
		for(i=0;i<m_NbIndiv;i++)
		{
			VProba[i]/=Sigma;
			if(VProba[i]>0)c++;
		}

		// Maintenant on cr�e ce nouveau support probabilis�
		E.m_fFreeProba=1;
		E.m_ProbaSupp=new CProbaSupport;
		E.m_ProbaSupp->SetNbCas(c);
		c=0;
		for(i=0;i<m_NbIndiv;i++)
		{
			if(VProba[i]>0)
			{
				E.m_ProbaSupp->m_NrIndividu[c]=i;
				E.m_ProbaSupp->m_Proba[c]=(unsigned short)VProba[i];
				ASSERT(E.m_ProbaSupp->m_Proba[c]>0 && E.m_ProbaSupp->m_Proba[c]<=1000);
				// Comme pour l'instant les fichiers sont faux, et qu l'on veut travailler
				// sans que cela plante....
				if(E.m_ProbaSupp->m_Proba[c]>1000)E.m_ProbaSupp->m_Proba[c]=1000;
				c++;
			}
		}
		return 1;
	}

	// Prise en compte du d�but de journ�e
	if(m_Param.m_PhasageHoraire!=0){
		int NbSupport;
		switch(m_Param.m_TypeSupport)
		{
			case 0: // Quart d'heure 300
				NbSupport=96;
				if(m_Param.m_PhasageHoraire<0){
					qhe-=m_Param.m_PhasageHoraire;
					if(qhe>NbSupport){
						qhe-=NbSupport;
						jd++;
						jn=(jn+1)%7;
					}
				}
				else {
					qhe-=m_Param.m_PhasageHoraire;
					if(qhe<0){
						qhe+=NbSupport;
						jd--;
						jn=(jn-1)%7;
					}
				}
			break;

			case 1: // Code support 300
				NbSupport=2400;
				if(m_Param.m_PhasageHoraire<0){
					if(qhe<300)
					{
						qhe+=NbSupport;
						jd--;
						jn=(jn-1)%7;
					}
				}
				else {
					if(qhe>2700)
					{
						qhe-=NbSupport;
						jd++;
						jn=(jn+1)%7;
					}
				}
			break;

			case 2:
				ASSERT(0); // PAs de phasage horaire
			break;

			default:
				ASSERT(0);
		}
	}

	E.m_ProbaSupp=NULL;
	int idxsta;
	if(!m_IndexStation.Lookup(E.m_Station,idxsta)){
		if(m_Param.m_TypeSupport==0){
			E.m_ProbaSupp=m_FicProba->GetProbaNeutre(jn,qhe);
			return 1; // La Proba est trouv�e
		}
		CString txt;
		txt.Format("Les probabilit�s de la station %d (num�ro JFC) ne sont pas trouv�es dans le panel charg�.",E.m_Station);
		AfxMessageBox(txt);
		ASSERT(0);
		return 0;
	}

#ifdef OPTION_DUREEECOUTE
	int idxjou,jourdate;
	jourdate=jd;
	if(m_Param.m_MethodeComptage)
	{
		// On n'utilise jamais la probabilit�, mais toujours le taux d'�coute.
		// Pour ne pas avoir � utiliser un ph�nom�ne al�atoire,
		// le patron d'�coute est cal� sur les jours.
		int idxmaxjou=m_DecalagePanelJouRef+m_FicProba->GetNbJour();
		while(jourdate>=idxmaxjou)jourdate-=m_FicProba->GetNbJour();
		while(jourdate<m_DecalagePanelJouRef)jourdate+=m_FicProba->GetNbJour();
	}
	if(m_IndexJour.Lookup(jourdate,idxjou)){
		//E.m_ProbaSupp=&m_FicAQJ->m_DureeQH[idxsta][idxjou][E.m_QHeure];
		E.m_ProbaSupp=m_FicProba->GetDureeEcoute(idxsta,idxjou,qhe);

		// En mode �crans, il se peut que l'on ne trouve pas 
		// C'est �videment � v�rifier, car normalement les �crans doivent �tres
		// en phase
		// Il semble que le probl�me vienne, une fois de plus, des fichiers !
		// TODO : A VERIFIER
		if(m_Param.m_TypeSupport==1 && E.m_ProbaSupp==NULL)
		{
			E.m_ProbaSupp=m_FicProba->GetProbabilite(idxsta,jn,qhe);
		}
	}
	else {	// Le jour n'est pas trouv� dans le fichier
			// des audiences, on utilise les supports probabilis�s
		//E.m_ProbaSupp=&m_FicAQJ->m_ProbaQH[idxsta][E.m_JourNomme][E.m_QHeure];
		E.m_ProbaSupp=m_FicProba->GetProbabilite(idxsta,jn,qhe);
	}
	return 1; // La Proba est trouv�e
#else
	E.m_ProbaSupp=m_FicProba->GetProbabilite(idxsta,jn,qhe);
	return 1; // La Proba est trouv�e
#endif
}


// On calcul le GRP ventile en s'inspirant
// Du MW Globale, que l'on distribut au prorata des parts
// de GRP livr� par la source de couverture
void CTomy::CalculMWventile(CTomy::CElement & E)
{
	E.m_MW=+0.0;
	E.m_GRP=0;
	if(!E.m_ProbaSupp)return;
	long i,NrIndiv;
	double coeff,NbContact=0,NbContactCible=0/*,NbContactSousCible=0*/,NbCts;
	for(i=0;i<E.m_ProbaSupp->m_NbCas;i++){
		NrIndiv=E.m_ProbaSupp->m_NrIndividu[i];
		if((coeff=m_CoeffExCible[NrIndiv])<=0)continue;
		NbCts=coeff*m_P[E.m_ProbaSupp->m_Proba[i]];
		NbContactCible+=NbCts;
		//if((coeff=m_CoeffExSousCible[NrIndiv])<=0)continue;
		//NbContactSousCible+=NbCts;
		if((coeff=m_CoeffEx[NrIndiv])<=0)continue;
		NbContact+=NbCts;
	}
//	if(NbContactCible<=0)return;
//	if(NbContactSousCible<=0)return;
	if(NbContact<=0)return;

	// Le GRP est �quivalent au GRP GLOBALE pond�r� parce que repr�sente
	// le nombre de contact filtr�s de ce support par rapport � la TOTALITE
	// Avec comme d�nominateur la population redress�e sur cellec de la sous cible ventil�e
	E.m_GRP=E.m_GRPgrp*(NbContact/NbContactCible)*m_RatioPopulation;
	E.m_GRPcouv=100.0*NbContact/m_PopulationSansFiltrePermanent;
	if(m_Param.m_DisableBisource)
	{
		E.m_MW=+1.0;
		E.m_GRP=E.m_GRPcouv;
	}
	else if(E.m_GRPcouv>0)
		E.m_MW=E.m_GRP/E.m_GRPcouv;

	if(m_flagFiltreIndividus){
		double NbContactFiltre=0;
		for(i=0;i<E.m_ProbaSupp->m_NbCas;i++){
			NrIndiv=E.m_ProbaSupp->m_NrIndividu[i];
			if((coeff=m_CoeffEx[NrIndiv])<=0)continue;
			NbContactFiltre+=coeff*m_P[E.m_ProbaSupp->m_Proba[i]]*m_FiltreIndividus[NrIndiv];
		}
		E.m_GRP*=NbContactFiltre/NbContact;
	}
	m_GRP+=E.m_GRP;
}

void CTomy::CalculMediaWeight(CTomy::CElement & E)
{
	E.m_MW=+0.0;
	if(!E.m_ProbaSupp)return;
	
	long i,NrIndiv;
	double coeff,NbContact=0;
	for(i=0;i<E.m_ProbaSupp->m_NbCas;i++){
		NrIndiv=E.m_ProbaSupp->m_NrIndividu[i];
		if((coeff=m_CoeffEx[NrIndiv])<=0)continue;
		NbContact+=coeff*m_P[E.m_ProbaSupp->m_Proba[i]];
	}
	if(NbContact<=0)return;
	E.m_GRPcouv=100.0*NbContact/m_PopulationSansFiltrePermanent;
	if(m_Param.m_DisableBisource)
	{
		E.m_MW=+1.0;
		E.m_GRP=E.m_GRPcouv;
	}
	else if(E.m_GRPcouv>0)E.m_MW=E.m_GRP/E.m_GRPcouv;

/*	CString txt;
	txt.Format("MW: %.1f  cas:%d contacts: %.0f",E.m_MW,E.m_ProbaSupp->m_NbCas,NbContact);
	AfxMessageBox(txt);*/

	if(m_flagFiltreIndividus)
	{
		double NbContactFiltre=0;
		for(i=0;i<E.m_ProbaSupp->m_NbCas;i++){
			NrIndiv=E.m_ProbaSupp->m_NrIndividu[i];
			if((coeff=m_CoeffEx[NrIndiv])<=0)continue;
			NbContactFiltre+=coeff*m_P[E.m_ProbaSupp->m_Proba[i]]*m_FiltreIndividus[NrIndiv];
		}
		E.m_GRP=E.m_GRPgrp*NbContactFiltre/NbContact;
	}
	m_GRP+=E.m_GRP;
	ASSERT(m_GRP>=0);

/*	CString txt;
	txt.Format("MW=%.2f %.2f/%.2f",E.m_MW,E.m_GRP,E.m_GRPcouv);
	AfxMessageBox(txt);*/
}

double CTomy::RecalculGrpElement(CTomy::CElement & E)
{
	double result;
	if(m_flagFiltreIndividus)
	{
		double NbContactFiltre=0,coeff,NbContact=0;
		for(int i=0;i<E.m_ProbaSupp->m_NbCas;i++){
			int NrIndiv=E.m_ProbaSupp->m_NrIndividu[i];
			if((coeff=m_CoeffEx[NrIndiv])<=0)continue;
			NbContact+=coeff*m_P[E.m_ProbaSupp->m_Proba[i]];
			NbContactFiltre+=coeff*m_P[E.m_ProbaSupp->m_Proba[i]]*m_FiltreIndividus[NrIndiv];
		}
		if(NbContact>0)
			result=E.m_GRPgrp*NbContactFiltre/NbContact;
		else
			result=0;

	}
	else
	{
		result=E.m_GRPgrp;
	}
	// Le GRP doit �tre exprim� sur la base cible permanente
	result*=m_PopulationSansFiltrePermanent/m_Population;
	return(result);
}



void CTomy::CalculApport(CTomy::CElement & E)
{
	if(E.m_MW<=0)return;
	long i,NrIndiv;
	if(m_Param.m_MethodeComptage==0)
	{	// METHODE PROBABILISTE
		double CoeffDuplication=m_Param.GetCoeffDuplication();
		if(CoeffDuplication>0)
		{
			for(i=0;i<E.m_ProbaSupp->m_NbCas;i++){
				NrIndiv=E.m_ProbaSupp->m_NrIndividu[i];
				if(m_CoeffEx[NrIndiv]<=0)continue;
				// On boucle sur le nb d'insertions.
				// on aurais put utiliser l'exposant (pow)
				double q=GetTrusquinNT(E.m_ProbaSupp->m_Proba[i],E.m_MW);
				m_NT[NrIndiv]*=CalculDuplicationNT(E.m_NbInsertion,q);
			}
		}
		else
		{
			for(i=0;i<E.m_ProbaSupp->m_NbCas;i++){
				NrIndiv=E.m_ProbaSupp->m_NrIndividu[i];
				if(m_CoeffEx[NrIndiv]<=0)continue;
				// On boucle sur le nb d'insertions.
				// on aurais put utiliser l'exposant (pow)
				for(int s=0;s<E.m_NbInsertion;s++)
					m_NT[NrIndiv]*=GetTrusquinNT(E.m_ProbaSupp->m_Proba[i],E.m_MW);
			}
		}
	}
	else
	{	// METHODE PAR COMPTAGE
		double Seuil=1-m_Param.m_SeuilDeVision;
		for(i=0;i<E.m_ProbaSupp->m_NbCas;i++){
			NrIndiv=E.m_ProbaSupp->m_NrIndividu[i];
			if(m_CoeffEx[NrIndiv]<=0)continue;
			if(GetTrusquinNT(E.m_ProbaSupp->m_Proba[i],E.m_MW)<=Seuil)
			{
				m_NT[NrIndiv]=+0.0;
			}
		}
	}
}

void CTomy::CalculDistribution(CTomy::CElement & E)
{
	if(E.m_MW<=0)return;
	int  i,NrIndiv;
	if(m_Param.m_MethodeComptage==0)
	{	// METHODE PROBABILISTE
		double CoeffDuplication=m_Param.GetCoeffDuplication();
		m_VB0=new double [1+NBCLASSEDEGAU];
		m_VBt=new double [1+NBCLASSEDEGAU];
		m_sizeVB=sizeof(double)*(1+NBCLASSEDEGAU);
		for(int c=0;c<1+NBCLASSEDEGAU;c++)m_VB0[c]=+0.0;
		if(CoeffDuplication>0)
		{
			for(i=0;i<E.m_ProbaSupp->m_NbCas;i++)
			{
				NrIndiv=E.m_ProbaSupp->m_NrIndividu[i];
				if(m_CoeffEx[NrIndiv]<=0)continue;
				GetTrusquinVB(E.m_ProbaSupp->m_Proba[i],E.m_MW);
				AppliqueCorrectionHypotheseIndependance();
				CalculDuplicationVB(E.m_NbInsertion);

				CalculApportDansVecteurBinomial(NrIndiv);

			}
		}
		else // Calcul sans les duplications
		{
			for(i=0;i<E.m_ProbaSupp->m_NbCas;i++)
			{
				NrIndiv=E.m_ProbaSupp->m_NrIndividu[i];
				if(m_CoeffEx[NrIndiv]<=0)continue;
				GetTrusquinVB(E.m_ProbaSupp->m_Proba[i],E.m_MW);
				AppliqueCorrectionHypotheseIndependance();
				// On boucle sur les spots comme si ils �taient ind�pendants
				for(int spot=0;spot<E.m_NbInsertion;spot++)
				{
					CalculApportDansVecteurBinomial(NrIndiv);
				}
			}
		}

	
		delete [] m_VB0;
		delete [] m_VBt;
	}
	else
	{	// METHODE PAR COMPTAGE
		double Seuil=1-m_Param.m_SeuilDeVision;
		for(i=0;i<E.m_ProbaSupp->m_NbCas;i++){
			NrIndiv=E.m_ProbaSupp->m_NrIndividu[i];
			if(m_CoeffEx[NrIndiv]<=0)continue;
			if(GetTrusquinNT(E.m_ProbaSupp->m_Proba[i],E.m_MW)<=Seuil)
			{
				// Pour des raisons de rapidit�, on ne met pas � jour
				// le vcteur binomiale m_VB[NrIndiv], mais uniquement
				// le maxi, qui correspont au nombre de fois touch�s
				// pour un individu donn�.
				m_NbVB[NrIndiv]++;
			}
		}
	}
}

//##ModelId=3D9AB7D202D9
void CTomy::CalculAsymptote(CTomy::CElement & E)
{
	if(!E.m_ProbaSupp)return;
	long i;
	for(i=0;i<E.m_ProbaSupp->m_NbCas;i++){
		m_NT[E.m_ProbaSupp->m_NrIndividu[i]]=+0.0;
	}
}

//##ModelId=3D9AB7D2027D
void CTomy::CalculPopulation()
{
	m_NbCas=m_Population=0;
	m_PopulationCibleEntiere=0;
	m_PopulationSansFiltrePermanent=0;
//	m_PopulationOffreElargieSansFiltrePermanent=0;
	double pop=+0.0;
	double cas=+0.0;
	for(int i=0;i<m_NbIndiv;i++){
		m_PopulationCibleEntiere+=m_CoeffExCible[i];
		//if(m_CoeffExSousCible[i]<=0)continue;
		//m_Population+=m_CoeffExSousCible[i];
		if(m_CoeffEx[i]<=0)continue;
		if(m_FiltreIndividusPermanent)
		{
			pop+=m_CoeffEx[i]*m_FiltreIndividusPermanent[i];
			cas+=m_FiltreIndividusPermanent[i];
		}
		else
		{
			m_Population+=m_CoeffEx[i];
			m_NbCas++;
		}
		m_PopulationSansFiltrePermanent+=m_CoeffEx[i];
//		if(m_FiltreOffreElargie && m_FiltreOffreElargie[i])
//			m_PopulationOffreElargieSansFiltrePermanent+=m_CoeffEx[i];
	}
	if(m_FiltreIndividusPermanent)
	{
		// Ce sont des entiers, donc on utilise des r�els le temps du calcul
		m_Population=(int)pop;
		m_NbCas=(int)cas;
	}

	if(m_Population>0)m_RatioPopulation=((double)m_PopulationCibleEntiere)/((double)m_Population);
	else m_RatioPopulation=0;
}


//##ModelId=3D9AB7D2026B
double CTomy::CalculNonTouche()
{
	double coeff;
	double result=+0.0;
	if(!m_flagFiltreIndividus){
		for(int i=0;i<m_NbIndiv;i++){
			if((coeff=m_CoeffEx[i])<=0)continue;
			result+=(+1.0-m_NT[i])*coeff;
		}
	}
	else {
		for(int i=0;i<m_NbIndiv;i++){
			if((coeff=m_CoeffEx[i])<=0)continue;
			result+=(+1.0-m_NT[i])*coeff*m_FiltreIndividus[i];
		}
	}

	result=100.0*result/m_Population;

	/*tring txt;
	txt.Format("Population= %ld c%%=%.2f",m_Population,result);
	AfxMessageBox(txt);*/
	
	return(result);
}

//##ModelId=3D9AB7D20273
double CTomy::CalculExactementTouche()
{
	ASSERT(m_VB);
	double GRPdistribue=+0.0;
	m_DistributionExactementTouche.CreateNewDistribution(m_NbClassesCalculDistribution);
	for(int c=1;c<m_NbClassesCalculDistribution;c++)
	{
		double coeff;
		double result=+0.0;
		if(!m_flagFiltreIndividus){
			for(int i=0;i<m_NbIndiv;i++){
				if((coeff=m_CoeffEx[i])<=0)continue;
				result+=m_VB[i][c]*coeff;
			}
		}
		else {
			for(int i=0;i<m_NbIndiv;i++){
				if((coeff=m_CoeffEx[i])<=0)continue;
				result+=m_VB[i][c]*coeff*m_FiltreIndividus[i];
			}
		}
		result=100.0*result/m_Population;
		m_DistributionExactementTouche.SetValue(c,result);
		GRPdistribue+=result*c;
	}

	m_DistributionExactementTouche.RecalculNonTouche();

	/*tring txt;
	txt.Format("Population= %ld c%%=%.2f",m_Population,result);
	AfxMessageBox(txt);*/


	// Correction avec la d�gauchisseuse, si besoin est
	int nbspot=GetNbSpot();
	int nbecran=m_Plan.GetSize();
	if(nbspot<NBCLASSEDEGAU && m_Param.m_DisableBisource==0)
	{
		// Des GRP sont perdu, soit parce que le m�diaweight est >5
		// soit parce que le nombre de spot est petit (exemple 1) et que
		// le m�dia weight est d�j� grand (exemple 2)
		// du coup tous les touch�s en pluri-fr�quentation ne sont
		// plus perdus dans la masse, mais sont �limin�s de la distribution
		// des contacts.
		// Puisque cela est flagrand, on utilise le GRP distribu�,
		// et on calcul le manque � gagner dans la distribution.
		// Ce manque � gagner est redistribu� �quitablement aux classes

		double GRPperdu=m_GRP-GRPdistribue;
		if(GRPperdu>=0.001)
		{
			double VBcoeff=-1;
			if(GRPdistribue>0)
			{
				VBcoeff=m_GRP/GRPdistribue;
				ASSERT(VBcoeff>+1.0);
			}

//			**************************************
//			**************************************
//			DEGAUCHISSEUSE DES CONTACTS DISTRIBUES
//			**************************************
//			**************************************

			TRACE("Utilisation de la d�gauchisseuse dans Tomy\n");
			GRPperdu/=nbspot;
			// Le seuil de d�gressif 
			int seuil=NBCLASSEDEGAU-10;
			if(nbspot>seuil)
			{
				double degressif=((double)(nbspot-seuil))/10.0;
				ASSERT(degressif<+1.0);
				GRPperdu=GRPperdu-GRPperdu*degressif;
			}
			
			// Distribution des contacts perdus,
			// et recalcul des non-touch�s
			m_DistributionExactementTouche.SetValue(0,100.0);
			for(int c=1;c<=nbspot;c++)
			{
				m_DistributionExactementTouche.AddValue(c,GRPperdu/c);
				m_DistributionExactementTouche.AddValue(0,-m_DistributionExactementTouche.GetValue(c));
			}

			//if(flagDiffuseDegauIndividu && VBcoeff>1.0)
			//{
				// Ce flag est utilis� lorsque le plan qui en en cours d'�valuation
				// va servir de filtre (permanent== sous-cible d'exposition).
				// De cette fa�on on reproduit l'effet de la d�gauchisseuse sur les
				// vecteurs binomiale des individus. Dans ce cas exeptionnel, 
				// le sigma peut d�pass� 1. On n'a peu de risque de d�passer les asymptotes,
				// donc de d�passer 100, car cette fonction ne joue que sur des petits plans
/*
				double Nbcontactperdudanslesindividus=0.0;
				for(int i=0;i<m_NbIndiv;i++)
				{
					if(m_CoeffEx[i]<=0)continue;
					double touche=1.0-m_VB[i][0];
					double coeffindif=VBcoeff;
					double nouveautouche=touche*coeffindif;
					if(nouveautouche>1.0)
					{
						coeffindif=+1.0/touche;
						Nbcontactperdudanslesindividus+=nouveautouche-1.0;
						nouveautouche=+1.0;
					}
					m_VB[i][0]=+1.0-nouveautouche;
					for(int c=1;c<m_NbClassesCalculDistribution;c++)
					{
						double coeffend=1+(coeffindif-1)/c;
						m_VB[i][c]*=coeffend;
					}
				}

/*
				STOPPE PAR JFC..... dommage !

				La formule ci-dessus m_VB[i][c]*=VBcoeff est fausse,
				puisque que pour 2 contacts, il faut que l'augmentation soit /2
				et ainsi de suite.....

				Et dans tous les cas, il faut recalculer les non-touch�s, da fa�on 
				� ce que le compl�ment donne un r�sultat juste (ce qui n'est pas le cas aujourd'hui

				donc, soit cette m�thode n'est pas encore suffisante,
				soit il faut accepter des individus 0 avec un proportion <0 !

				il reste donc encore du travail
				}*/
			//}
		}
	}


	return(m_DistributionExactementTouche.CalculCouverture_nPlus(1));
}


//##ModelId=3D9AB7D2009E
double CTomy::GetExactementTouche(int categorie)
{
	ASSERT(categorie>=0 && categorie<m_NbClassesCalculDistribution);
	if(categorie==m_Param.m_NbClassesResultatDistribution)
	{
		return(m_DistributionExactementTouche.CalculCouverture_nPlus(categorie));
	}
	else
	{
		return(m_DistributionExactementTouche.GetValue(categorie));
	}
}

//##ModelId=3D9AB7D20256
double CTomy::GetTrusquinNT(short p, double MW)
{
	ASSERT(p>0 && p<=1000);
	ASSERT(MW>+0.0);
	// Si il y a une correction, elle doit �tre faite dans GetTrusquinVB
	ASSERT(m_Param.GetCorrectionHypotheseIndependance()==0);
	double Proba;
	
	if(MW<=2){
		Proba=m_P[p]*MW;
		if(Proba>m_Trusquin[p][2])Proba=m_Trusquin[p][2];
	}
	else {
		if(MW<=3){
			MW-=2;
			Proba=m_Trusquin[p][2]+MW*(m_Trusquin[p][3]-m_Trusquin[p][2]);
		}
		else {
			if(MW<=4){
				MW-=3;
				Proba=m_Trusquin[p][3]+MW*(m_Trusquin[p][4]-m_Trusquin[p][3]);
			}
			else {
				if(MW>MWMAX)MW=MWMAX;
				MW-=4;
				Proba=m_Trusquin[p][4]+MW*(m_Trusquin[p][5]-m_Trusquin[p][4]);
			}
		}
	}

	if(Proba<0 || Proba>1){
		AfxMessageBox("Bug dans CTomy::GetTrusquinNT");
		Proba=0;
	}

	return(+1.0-Proba);
}

//##ModelId=3D9AB7D20260
void CTomy::GetTrusquinVB(short p, double MW)
{
	ASSERT(p>0 && p<=1000);
	ASSERT(MW>+0.0);
	double Contact=m_P[p]*MW;
//	double Contact1=Contact;// pour v�rification
	int nb,c;
	double * VB=m_VBs;

	if(MW<=2){
		if(Contact<=m_Trusquin[p][2])
		{
			// C'est le cas qui doit gazer...
			VB[1]=Contact;
			VB[0]=+1.0-Contact;
			m_NbVBs=1;
			return;
		}
		VB[1]=m_Trusquin[p][2];
		nb=2;
	}
	else // Interpolation entre 2 points du trusquin
	{
		if(MW<=3)
		{
			nb=3;
			MW-=2;
		}
		else 
		{
			if(MW<=4)
			{
				nb=4;
				MW-=3;
			}
			else 
			{
				if(MW>MWMAX)MW=MWMAX;
				MW-=4;
				nb=5;
			}
		}
		int o=nb-1;
		VB[1]=m_Trusquin[p][o]+MW*(m_Trusquin[p][nb]-m_Trusquin[p][o]);
	}

	if(VB[1]>=Contact)
	{
		// Cas rapide
		VB[1]=Contact;
		VB[0]=+1.0-Contact;
		m_NbVBs=1;
		return;
	}

	// Distribution des contacts le long des points du trusquin
	Contact-=VB[1];
	int pos=2;
	while(Contact>0)
	{
		double apport=m_Trusquin[p][nb-pos+1];
		if(apport>=Contact)
		{
			VB[pos]=Contact;
			nb=pos;
			break;
		}
		VB[pos]=apport;
		Contact-=apport;
		if(pos>=nb)
		{
//			ASSERT(0); // Perte de contacts autoris�
			break;
		}
		pos++;
	}

	// Transformation de VB en exactement touch�
	for(c=1;c<nb;c++)VB[c]-=VB[c+1];

	// Calcul du VB[0] (les non touch�s)
	VB[0]=+1.0;
	for(c=1;c<=nb;c++)VB[0]-=VB[c];

	ASSERT(VB[0]>=0);
	// Pour v�rifier la coh�rence des deux trusquins
	// double NT=GetTrusquinNT(p,MW);
	// ASSERT(VB[0]>NT-0.000001 && VB[0]<NT+0.000001);
	// Pour v�rifier que l'on ne perd pas de contacts
	// D�griser Contact1 au d�but de cette fonction
	//double Contact2=+0.0;
	//for(c=1;c<=nb;c++)Contact2+=VB[c]*c;
	//ASSERT(Contact1<Contact2+0.00001);
	//ASSERT(Contact1>Contact2-0.00001);
	// ***********************************************

	m_NbVBs=nb;
	
	return;
}

//##ModelId=3D9AB7D2017A
void CTomy::ChangeGRP(int NrSpot,double GRP)
{
	ASSERT(NrSpot<m_Plan.GetSize());
	m_Plan[NrSpot].m_GRP=GRP;
}

//##ModelId=3D9AB7D2013D
void CTomy::InitialiseCible(int NrCible)
{
	m_NrCible=NrCible;
	/*if(m_Etude=="SURECHANTILLON")LoadCibleMediadataAvecSurEchantillonMediacabsat();
	else*/ LoadCible();
	memcpy(m_CoeffExCible,m_CoeffEx,sizeof(long)*m_NbIndiv);
	memcpy(m_CoeffExSousCible,m_CoeffEx,sizeof(long)*m_NbIndiv);
	m_flagSousCible=0;
	m_flagVentilation=0;
	m_flagFiltreIndividus=0;
	m_FiltreIndividusPermanent=NULL;
	CalculPopulation();
}

//##ModelId=3D9AB7D2013F
void CTomy::InitialiseSousCible(int NrCible,int CritereFiltre,CWordArray * ModaliteFiltre)
{
	InitialiseCible(NrCible);
	if(CritereFiltre<0)return;
	memset(m_CoeffExSousCible,0,sizeof(long)*m_NbIndiv);
	for(int i=0;i<ModaliteFiltre->GetSize();i++)IntegreSousCible(CritereFiltre,ModaliteFiltre->GetAt(i));
	memcpy(m_CoeffEx,m_CoeffExSousCible,sizeof(long)*m_NbIndiv);
	CalculPopulation();
}


//##ModelId=3D9AB7D2014A
void CTomy::InitialiseCibleVentile(int NrCible,int NrCritere1 ,int NrModalite1,int NrCritere2,int NrModalite2)
{
	InitialiseCible(NrCible);
	if((NrCritere1<0 || NrModalite1<0) && (NrCritere2<0 || NrModalite2<0))return;
	if(NrCritere1>=0 && NrModalite1>=0)IntegreVentilation(NrCritere1,NrModalite1);
	if(NrCritere2>=0 && NrModalite2>=0)IntegreVentilation(NrCritere2,NrModalite2);
	CalculPopulation();
}

//##ModelId=3D9AB7D2015D
void CTomy::InitialiseSousCibleVentile(int NrCible,int CritereFiltre,CWordArray * ModaliteFiltre,int NrCritere1 ,int NrModalite1,int NrCritere2,int NrModalite2)
{
	InitialiseSousCible(NrCible,CritereFiltre,ModaliteFiltre);
	if((NrCritere1<0 || NrModalite1<0) && (NrCritere2<0 || NrModalite2<0))return;
	if(NrCritere1>=0 && NrModalite1>=0)IntegreVentilation(NrCritere1,NrModalite1);
	if(NrCritere2>=0 && NrModalite2>=0)IntegreVentilation(NrCritere2,NrModalite2);
	CalculPopulation();
}

//##ModelId=3D9AB7D20171
/*
void CTomy::CalculCoeffCorrectionMediaWeightOffreElargie(double PopulationNational,double PopulationOffreElargie)
{
	// Cette fonction est necessaire pour activer un correction
	// sur le M�diaWeight des supports de l'offre �largie.

	// Cette correction est n�cesaire car le chagnement de base de calcul
	// National -> Offre �largie provoque une modification des m�dia Weight.

	// Normalement, le M�dia Weight d'un support est identique quelque soit la base
	// si il n'y a pas de d�bordement de contacts en dehors de la base.

	// Le fait de modifier la base modifie aussi la population de la cible.
	// Le probl�me vient du fait que sur deux source de donn�e diff�rente 
	// le rapport entre la population National et Offre �largie n'est pas le m�me.

	// Un cas similaire existe pour la belgique avec ses r�gion filles.

	double denominateur=PopulationOffreElargie*m_PopulationSansFiltrePermanent;
	if(denominateur!=0)
		m_CoeffCoerrectionMediaWeightOffreElargie=(PopulationNational*m_PopulationOffreElargieSansFiltrePermanent)/denominateur;
	else
		m_CoeffCoerrectionMediaWeightOffreElargie=+1.0;
	TRACE("Coefficient de correction du M�diaWeight des supports CABSAT=%f",m_CoeffCoerrectionMediaWeightOffreElargie);
}
*/


//##ModelId=3D9AB7D20302
void CTomy::FreeAllocIndiv()
{
	if(m_CoeffEx)delete [] m_CoeffEx;
	if(m_CoeffExCible)delete [] m_CoeffExCible;
	if(m_CoeffExSousCible)delete [] m_CoeffExSousCible;
	if(m_NT)delete [] m_NT;
	if(m_FiltreIndividus)delete [] m_FiltreIndividus;
	m_FiltreIndividusPermanent=NULL;
	FreeVecteurBinomiale(); // Avant de changer de nb d'indiv
	m_NbIndiv=0;
}

// Alloue pour m_NbIndiv
// les variable necesaire lorsque le panel est charg�.
// C'est aussi cette fonction qui prend en charge le chargement
// Et le calcul des strates de consomation des individus du panel
// lorsqu'un calcul sur les consomation a la "voll�e" est demand�e
//##ModelId=3D9AB7D2030A
void CTomy::AllocIndivPourPanel()
{
	m_CoeffEx=new long [m_NbIndiv];
	m_CoeffExCible=new long [m_NbIndiv];
	m_CoeffExSousCible=new long [m_NbIndiv];
	m_NT=new double [m_NbIndiv];
	m_FiltreIndividus=new double [m_NbIndiv];

	// Construction du lien avec les stations
	m_IndexStation.RemoveAll();
	for(int s=0;s<m_FicProba->GetNbStation();s++)
		m_IndexStation.SetAt(m_FicProba->GetNrStation(s),s);

	// Calcul des consommations
	if(m_Param.m_NbRatioConsomation>0)
	{
		ASSERT(m_Param.m_TypeSupport==1);
		m_FicProba->CalculRatioConsomation(m_Param.m_NbRatioConsomation);
	}
}


//##ModelId=3D9AB7D2024B
CFicProba * CTomy::LoadPanel(LPCSTR Panel,LPCSTR Etude)
{
	AfxCheckMemory();

	// Cette fonction charge un nouveau panel
	// Cette fonction ne v�rifie donc pas la necessit�.
	// Si par hazard un autre panel �tait d�j� charg�,
	// il faut commencer par l'indiquer au panelloader
	// pour qu'il puisse gerer les utilisations

	// Demande de chargement du panel � la classe panelloader.

	// NOTE: depuis le fonctionnement avec M�diacabsat, il faut indiquer un
	// sous r�pertoire pour trouver le panel

	CString CheminPanel;
	CheminPanel.Format("%s\\%s",m_Param.m_CheminData,Etude);
	//TODO: passer le nombdre dindividus 
	CFicProba * FicProba=m_PanelLoader->LoadPanel(CheminPanel,Panel,m_Param.m_TypeSupport,0);

	TRACE("Passage normal dans PanelLoader()\n");
	AfxCheckMemory();

	// Apr�s un chargement de panel, le nombre d'individu doit �tre actualis�.
	// Si il y a un sur echantillon, ce chiffre sera modifi� ensuite.

	m_NbIndiv=FicProba->GetNbIndiv();

	return(FicProba);
}


//##ModelId=3D9AB7D20238
int CTomy::LoadCible()
{
	CString FileName;
	FileName.Format("%s\\%s\\%s_%04d.pdx",m_Param.m_CheminData,m_Etude,m_CodePanel,m_NrCible);

	CStdioFile FicPDX;
	CJFCFileException e;
	if(!FicPDX.Open(FileName,CFile::modeRead  | CFile::typeBinary|CFile::shareDenyWrite,&e) )
	{
		CString MsgErr;
		MsgErr.Format("Erreur � l'ouverture du fichier %s : ",FileName);
		e.AfficheErreur(MsgErr);
		return(1);
	}

	// FicPDX.Seek(m_NrCible*sizeof(long)*m_NbIndiv,CFile::begin);
	ASSERT(m_CoeffEx!=NULL);
	FicPDX.Read(m_CoeffEx,sizeof(long)*m_NbIndiv);
	FicPDX.Close();

/*	if(m_NrCible==1){
		2 octets: Nb foyers
			2 octets: NrFoyer
			2 octets: NbIndiv
			2 octets: NrIndiv


		FileName.Format("%s\\%s.foy",m_Param.m_CheminData,m_CodePanel);
		
		// Cas de la cible foyer
		CStdioFile FicFOY;
		if(!FicFOY.Open(FileName,CFile::modeRead  | CFile::typeBinary|CFile::shareDenyWrite,&e) )
		{
			CString MsgErr;
			MsgErr.Format("Erreur � l'ouverture du fichier %s : ",FileName);
			e.AfficheErreur(MsgErr);
			return(1);
		}
		FicFOY.Read(m_NbFoyer,sizeof()*m_NbIndiv);
//		FicFOY.Read(m_CoeffEx,sizeof()*m_NbIndiv);
		FicFOY.Close();
	}*/

	return (0);
}

/*
int CTomy::LoadCibleMediadataAvecSurEchantillonMediacabsat()
{

	CFicAEJ * pFicProbaBase=(CFicAEJ *)m_FicProba1;
	CFicAEJ * pFicProbaSurEchantillon=(CFicAEJ *)m_FicProba2;
	CFicAEJ_SurEchantillon * pFicProbaFusion=(CFicAEJ_SurEchantillon *)m_FicProba;
	
	
	// On charge la cible sur le Mediadata
	CString FileName;
	CString panel;
	panel=m_CodePanel.Left(4);
	FileName.Format("%s\\%s\\%s_%04d.pdx",m_Param.m_CheminData,"MEDIADATA",panel,m_NrCible);

	CStdioFile FicPDX;
	CJFCFileException e;
	if(!FicPDX.Open(FileName,CFile::modeRead  | CFile::typeBinary|CFile::shareDenyWrite,&e) )
	{
		CString MsgErr;
		MsgErr.Format("Erreur � l'ouverture du fichier %s : ",FileName);
		e.AfficheErreur(MsgErr);
		return(1);
	}

	int nbindivMediadata=pFicProbaBase->GetNbIndiv();
	long * CoeffExMediadata=new long[nbindivMediadata];
	FicPDX.Read(CoeffExMediadata,sizeof(long)*nbindivMediadata);
	FicPDX.Close();

	// On charge la cible sur le Mediacabsat
	panel=m_CodePanel.Right(4);
	FileName.Format("%s\\%s\\%s_%04d.pdx",m_Param.m_CheminData,"MEDIACABSAT",panel,m_NrCible);
	if(!FicPDX.Open(FileName,CFile::modeRead  | CFile::typeBinary|CFile::shareDenyWrite,&e) )
	{
		CString MsgErr;
		MsgErr.Format("Erreur � l'ouverture du fichier %s : ",FileName);
		e.AfficheErreur(MsgErr);
		return(1);
	}

	int nbindivMediacabsat=pFicProbaSurEchantillon->GetNbIndiv();
	long * CoeffExMediacabsat=new long[nbindivMediacabsat];
	FicPDX.Read(CoeffExMediacabsat,sizeof(long)*nbindivMediacabsat);
	FicPDX.Close();


	// On fusionne	...
	ASSERT(m_CoeffEx!=NULL);
	// Tous les individus non offre �largie
	for(int i=0;i<nbindivMediadata;i++)
	{
		short nr=pFicProbaFusion->GetNrIndivBase(i);
		if(nr>=0)m_CoeffEx[nr]=CoeffExMediadata[i];
	}

	// Tous les individus du sur-�chantillon
	for(i=0;i<nbindivMediacabsat;i++)
	{
		m_CoeffEx[pFicProbaFusion->GetNrIndivSurEchantillon(i)]=CoeffExMediacabsat[i];
	}


	delete [] CoeffExMediadata;
	delete [] CoeffExMediacabsat;

	return (0);
}
*/

//##ModelId=3D9AB7D20134
void CTomy::SetParametres(CTomyParam * param)
{
	m_Param=*param;
}

//##ModelId=3D9AB7D20115
double CTomy::GetCouverture()
{
	return(m_Couverture);
}


//##ModelId=3D9AB7D20129
double CTomy::GetCourbeReponse(int nbfoistouche)
{
	if(m_Param.m_ValideBeta<0)return(0);
	ASSERT(nbfoistouche>0 && nbfoistouche<=m_Param.m_NbClassesResultatDistribution);
	double beta=1-pow(1-((double)m_Param.m_ValideBeta/100.0),nbfoistouche);
	return(100.0*beta);
}


//##ModelId=3D9AB7D20116
double CTomy::GetCouvertureValideBeta()
{
	double couverture=0;

	for(int i=1;i<=m_Param.m_NbClassesResultatDistribution;i++)
	{
		couverture+=m_DistributionExactementTouche.GetValue(i)*GetCourbeReponse(i)/100.0;
	}
	return(couverture);
}


// Cette fc d�termine le b�ta "id�ale" pour une r�p�tition
// optimale "optimis�e" � n contacts
//##ModelId=3D9AB7D20117
void CTomy::CalculValideBeta_RepetitionOptimale(int NbContact)
{
	int resultat0a20[]={100 , 86 , 63 , 48 , 39 , 32 , 28 , 24 , 22 , 20 , 18 , 16 , 15 , 14 , 13 , 12 , 12 , 11 , 10 , 10 , 9};
	if(NbContact<=0)m_Param.m_ValideBeta=resultat0a20[NbContact];
	else if(NbContact>=20)m_Param.m_ValideBeta=resultat0a20[NbContact];
	else m_Param.m_ValideBeta=resultat0a20[NbContact];
}


//##ModelId=3D9AB7D20120
double CTomy::GetCouvertureValideMinMax()
{
	if(m_Param.m_ValideMini<0 || m_Param.m_ValideMaxi<0)return(0);

	double couverture=0;

	for(int i=m_Param.m_ValideMini;i<=m_Param.m_ValideMaxi;i++)
	{
		couverture+=m_DistributionExactementTouche.GetValue(i);
	}
	return(couverture);
}


//##ModelId=3D9AB7D2010D
double CTomy::GetAsymptote()
{
	m_Asymptote=+0.0;
	if(m_Population<=0)return 0;

	for(int i=0;i<m_NbIndiv;i++)m_NT[i]=+1.0;
	for(int s=0;s<m_Plan.GetSize();s++){
		CalculAsymptote(m_Plan[s]);
	}
	return (m_Asymptote=CalculNonTouche());
}

//##ModelId=3D9AB7D2010B
int CTomy::GetNbCas()
{
	return(m_NbCas);
}

// Retourne le nombre de cas des touch�s par le plan
//##ModelId=3D9AB7D2010C
int CTomy::GetNbCasPlan()
{
	/*
	JFC m'a encore fais changer...
	int Nb=0;
	for(int i=0;i<m_NbIndiv;i++){
		if(m_NT[i]<+1.0)Nb++;
	}
	return(Nb);
	*/
	double nbcas=+0.0;
	for(int i=0;i<m_NbIndiv;i++)
	{
		nbcas+=1-m_NT[i];
	}
	return((int)nbcas);
}

//##ModelId=3D9AB7D20103
int CTomy::GetPopulation()
{
	return(m_Population);
}


//##ModelId=3D9AB7D200EF
void CTomy::InitialisePanel(LPCSTR panel,LPCSTR etude)
{
	ASSERT(etude=="MEDIADATA" || etude=="MEDIACABSAT" /*|| etude=="SURECHANTILLON"*/);
	// Si le panel est d�j� charg�, il n'est pas necessaire de le recharger
	if(m_CodePanel==panel && m_Etude==etude)return;

	// Lib�ration des allocation des individus
	FreeAllocIndiv();
	
	// Lib�ration du panel
	FreePanel();

	// Chargement du panel
	m_CodePanel=panel;
	m_Etude=etude;
	m_FicProba=LoadPanel(m_CodePanel,m_Etude);

	// Allocation des vecteurs des individus
	AllocIndivPourPanel();
}

//##ModelId=3D9AB7D200F9
/*
void CTomy::InitialisePanelMediadataAvecSurEchantillonMediacabsat(LPCSTR panelMediadata,LPCSTR panelMediaCabSat)
{
	// Cette fonction ne doit �tre utilis�e que pour le mode �cran
	if(m_Param.m_TypeSupport!=1)
	{
		AfxMessageBox("Utilisation incorrecte de la fonction !");
		ASSERT(0);
	}

	// Si le panel est d�j� charg�, il n'est pas necessaire de le recharger
	CString panel;
	panel.Format("%s%s",panelMediadata,panelMediaCabSat);
	if(m_CodePanel==panel && m_Etude=="SURECHANTILLON")return;

	// Lib�ration des allocation des individus
	FreeAllocIndiv();

	// Lib�ration du panel
	FreePanel();

	// Chargement du panel

	// Existe-il dans le panel loader ?
	// Chargement du Panel Mediadata avec le sur-�chantillon M�diacabsat
	m_FicProba=m_PanelLoader->LoadPanelSurEchntillonOffreElargie(m_CodePanel);

	if(m_FicProba==NULL)
	{
		// il faut construire ce panel
		// Chargement de la vague M�diadata
		m_FicProba1=LoadPanel(panelMediadata,"MEDIADATA");

		// R�cup�ration de l'information "Offre �largie"
		int NrCritere = 34-1;
		m_CodePanel=panelMediadata;
		m_Etude="MEDIADATA";
		m_FiltreOffreElargie=GetCritere(NrCritere);

		// Chargement de la vague Mediacabsat
		m_FicProba2=LoadPanel(panelMediaCabSat,"MEDIACABSAT");


		CFicAEJ_SurEchantillon * panelfusion=new CFicAEJ_SurEchantillon;
		m_FicProba=panelfusion;
		panelfusion->BindPanelSurEchntillonOffreElargie(panel,m_FicProba1,m_FicProba2,m_FiltreOffreElargie);

		// Et on le m�morise dans le panel loader
		m_PanelLoader->StoreNewPanel(m_FicProba);

		// Depuis la version (2 juillet 2002) on vire les panels originaux
		// de la m�moire car le m�diacabsat consome 75Mo
		//FreePanelOriginaux();
		// note IMPOSSIBLE car les pointeur des proba sont partag�s !
	}
	else
	{
		// c'est bon, ce panel existe...
	}

	m_CodePanel=panel;
	m_Etude="SURECHANTILLON";

	m_NbIndiv=m_FicProba->GetNbIndiv();

	// Allocation des vecteurs des individus
	AllocIndivPourPanel();
}
*/


//##ModelId=3D9AB7D200ED
void CTomy::InitialiseJourReference(COleDateTime date)
{
	m_DateDebut=date;

	// Construction du lien avec les jours
	COleDateTime m_DateDebutPanel;
	m_DateDebutPanel.SetDate(m_FicProba->GetDateDebut()/10000,(m_FicProba->GetDateDebut()/100)%100,m_FicProba->GetDateDebut()%100);
	COleDateTimeSpan diffjour=m_DateDebutPanel-m_DateDebut;
	m_DecalagePanelJouRef=diffjour.GetDays();
//	Decalage=0; // Pour les tests

	m_IndexJour.RemoveAll();
	for(int j=0;j<m_FicProba->GetNbJour();j++)
	{
		m_IndexJour.SetAt(j+m_DecalagePanelJouRef,j);
	}
	ASSERT((m_FicProba->GetNbJour()%7)==0);
}

// Retourne les modalit�es de ce crit�re
// !! la lib�ration est � la charge du demandeur !!
//##ModelId=3D9AB7D202E3
char * CTomy::GetCritere(int NrCritere)
{

	// Nb le crit�re est de 1 � n
	// donc le le remet en origine 0
	NrCritere--;


	CString NomFic;
	char * Modalite=new char [m_NbIndiv];
	CStdioFile f;
	CJFCFileException e;

	NomFic.Format("%s\\%s\\%s.crc",m_Param.m_CheminData,m_Etude,m_CodePanel);
	if(!f.Open(NomFic,CFile::modeRead  | CFile::typeBinary|CFile::shareDenyWrite,&e)){
		CString err;
		err.Format("Erreur � l'ouverture de %s : ",NomFic);
		e.AfficheErreur(err);
		memset(Modalite,0,m_NbIndiv);
		return(Modalite);
	}
	f.Seek(m_NbIndiv*NrCritere,CFile::begin);
	f.Read(Modalite,m_NbIndiv);
	for(int i=0;i<m_NbIndiv;i++)Modalite[i]--;

	f.Close();
	return(Modalite);
}


// C'est un OU qui est fait entre les crit�res
//##ModelId=3D9AB7D202F8
void CTomy::IntegreSousCible(int NrCritere,int NrModalite)
{
	m_flagSousCible=1;

	// On filtre avec la modalit�
	char * Modalite=GetCritere(NrCritere);

	for(int i=0;i<m_NbIndiv;i++){
		if(Modalite[i]==NrModalite)m_CoeffExSousCible[i]=m_CoeffExCible[i];
	}

	delete [] Modalite;
}	

// C'est un ET qui est fait entre les crit�res
//##ModelId=3D9AB7D202ED
void CTomy::IntegreVentilation(int NrCritere,int NrModalite)
{
	m_flagVentilation=1;

	// On filtre avec la modalit�
	char * Modalite=GetCritere(NrCritere);

	for(int i=0;i<m_NbIndiv;i++){
		if(Modalite[i]!=NrModalite)m_CoeffEx[i]=0;
	}

	delete [] Modalite;
}	

//##ModelId=3D9AB7D200E5
void CTomy::RestoreCible()
{
	memcpy(m_CoeffEx,m_CoeffExSousCible,sizeof(long)*m_NbIndiv);
	m_flagVentilation=0;
}

//##ModelId=3D9AB7D200E4
double CTomy::GetGRP()
{
	return(m_GRP);
}


// Ce filtre ne modifie pas la cible
// Ce filtre se substitue au filtre Exclusifs/Dupliqu�s
//##ModelId=3D9AB7D200D9
void CTomy::ActiveFiltreIndividusStrateConsomation(int NrStrate)
{
	int i;

	m_flagFiltreIndividus=1;

	for(i=0;i<m_NbIndiv;i++)
	{
		if(((CFicAEJ*)m_FicProba)->m_NrRatioConso[i]==NrStrate)
			m_FiltreIndividus[i]=+1.0;
		else
			m_FiltreIndividus[i]=+0.0;
	}
	// Si il existe un filtre permanent on le remet en place
	RestoreFiltrePermanent();
}

void CTomy::RestoreFiltrePermanent()
{
	// Si il existe un filtre permanent on le remet en place
	if(m_FiltreIndividusPermanent)
	{
		for(int i=0;i<m_NbIndiv;i++)m_FiltreIndividus[i]*=m_FiltreIndividusPermanent[i];
	}
}


//##ModelId=3D9AB7D200D0
void CTomy::SetFiltreIndividus(double *filtre,char flagExclusifsDupliques)
{
	int i;

	m_flagFiltreIndividus=1;

	if(!flagExclusifsDupliques)
	{
		/////////////// EXCLUSIFS //////////////
		if(filtre==NULL)
		{
			for(i=0;i<m_NbIndiv;i++)m_FiltreIndividus[i]=+1.0;
		}
		else
		{
			for(i=0;i<m_NbIndiv;i++)m_FiltreIndividus[i]=filtre[i];
		}
	}
	else 
	{
		/////////////// DUPLIQUES //////////////
		if(filtre==NULL)
		{
			for(i=0;i<m_NbIndiv;i++)m_FiltreIndividus[i]=+0.0;
		}
		else
		{
			for(i=0;i<m_NbIndiv;i++)m_FiltreIndividus[i]=+1.0-filtre[i];
		}
	}


	// Si il existe un filtre permanent on le remet en place
	if(m_FiltreIndividusPermanent)
	{
		for(i=0;i<m_NbIndiv;i++)m_FiltreIndividus[i]*=m_FiltreIndividusPermanent[i];
	}
}

//##ModelId=3D9AB7D200DB
void CTomy::SetFiltreIndividusPermanent(double *filtre)
{
	int i;

	if(filtre!=NULL)
	{
		m_flagFiltreIndividus=1;

		// Il est possible d'utiliser ce filtre seul;
		// Donc pour l'instant on l'active
		for(i=0;i<m_NbIndiv;i++)
		{
			m_FiltreIndividus[i]=filtre[i];
		}
	}
	else
	{
		// Les autres filtres sont perdus ?!?!!
		m_flagFiltreIndividus=0;
		for(i=0;i<m_NbIndiv;i++)
		{
			m_FiltreIndividus[i]=0;
		}
	}

	// On conserve ce filtre permanent,
	// pour l'appliquer �ventuelement sur un segond filtre
	m_FiltreIndividusPermanent=filtre;


	// Ce filtre (contrairement � l'autre) modifie la population de la cible
	CalculPopulation();
}

//##ModelId=3D9AB7D200C6
double * CTomy::GetCopyIndividusNonTouches()
{
	int i;
	double * V=new double [m_NbIndiv];
	for(i=0;i<m_NbIndiv;i++){
		V[i]=m_NT[i];
	}
	return(V);
}

// Cette fonction travaille sur le vecteur binomiale
// Il faut donc qu'une distribution ai �t� pr�c�dement calcul�e
//##ModelId=3D9AB7D200C7
double * CTomy::GetCopyIndividusTouches(CWordArray * TabClasses)
{
	if(m_VB==NULL)
	{
		ASSERT(0);
		return(NULL);
	}
	int i;
	double * V=new double [m_NbIndiv];
	for(i=0;i<m_NbIndiv;i++){
		V[i]=+0.0;
		for(int c=0;c<TabClasses->GetSize();c++)
		{
			int cat=TabClasses->GetAt(c);
			if(cat==m_Param.m_NbClassesResultatDistribution)
			{
				// Calcul de la classe n et +
				for( ;cat<m_NbClassesCalculDistribution;cat++)
				{
					V[i]+=m_VB[i][cat];
				}
			}
			else
			{
				ASSERT(cat<m_NbClassesCalculDistribution);
				V[i]+=m_VB[i][cat];
			}
		}
	}
	return(V);
}

double GetCoeffCorrectionSousCibleFrequentation(CWordArray * TabClasses,int cible)
{
/*	if(m_VB==NULL)
	{
		ASSERT(0);
		return(+1.0);
	}
	if(m_Plan.GetSize()>=NBCLASSEDEGAU || m_Param.m_DisableBisource==0)
	{
		return(+1.0);
	}
*/

return 1.0;
}



// Retourne 1 si c'est un jour du panel
//##ModelId=3D9AB7D200BC
int CTomy::GetJourPanel(int jour)
{
	int idxjou;
	if(m_IndexJour.Lookup(jour,idxjou))return(1);
	return(0);
}

//##ModelId=3D9AB7D200B2
void CTomy::FreePanel()
{
	if(m_FicProba)m_PanelLoader->UnloadPanel(m_FicProba);
//	if(m_FiltreOffreElargie) delete [] m_FiltreOffreElargie;
	m_CodePanel="";
	m_Etude="";
	m_FicProba=NULL;
}


//##ModelId=3D9AB7D200A9
void CTomy::SetCallBack(CTomyCallBack *ptr)
{
	m_Param.m_CallBack=ptr;
}

//##ModelId=3D9AB7D200A8
void CTomy::FreeVecteurBinomiale()
{
	if(m_VB==NULL)return;
	for(int i=0;i<m_NbIndiv;i++)
	{
		delete [] m_VB[i];
	}
	delete [] m_VB;
	delete [] m_VBs;
	delete [] m_NbVB;


	m_VB=NULL;
	m_VBs=NULL;
	m_NbVB=NULL;
}

//##ModelId=3D9AB7D200A7
int CTomy::GetNbSpot()
{
	int nb=0;
	for(int e=0;e<m_Plan.GetSize();e++)nb+=m_Plan[e].m_NbInsertion;
	return(nb);
}


//##ModelId=3D9AB7D00376
double CTomyParam::GetCorrectionHypotheseIndependance()
{
	if(m_MethodeComptage)return +0.0;
	if(m_TypeSupport!=1)return +0.0;
	return(m_CoeffCHI);
}

//##ModelId=3D9AB7D202CD
void CTomy::AppliqueCorrectionHypotheseIndependance()
{
	// Correction de l'hypoth�se de relation d'ind�pendance++++++
	double CoeffCHI=m_Param.GetCorrectionHypotheseIndependance();
	if(CoeffCHI>0 && m_NbVBs>=1)
	{
		if(m_NbVBs==1)
		{
			m_NbVBs=2;
			m_VBs[2]=0;
		}
		double PointGlisse;
		PointGlisse=m_VBs[1]*CoeffCHI;
		if(PointGlisse>+0.0)
		{
			m_VBs[1]-=PointGlisse;
			PointGlisse/=2.0;
			m_VBs[2]+=PointGlisse;
			m_VBs[0]+=PointGlisse;
		}
	}
	// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
}

// Le calcul des duplications est utilis� pour les �crans lorsque le plan est compos� de 
// Plusieurs insertions (plusieur spots) dans le m�me �cran
// Ce calcul agit sur m_VBs et m_NBVBs
//##ModelId=3D9AB7D202A6
void CTomy::CalculDuplicationVB(int NbInsertion)
{
	// On boucle autant de fois qu'il existe des insertions >1
	int NbBoucle=NbInsertion-1;
	if(NbBoucle<1)return;

	// une petite v�rif ...
#ifdef _DEBUG
	{
		double s=+0.0;
		for(int i=0;i<=m_NbVBs;i++)s+=m_VBs[i];
		if(s<.9999999 || s>1.00000001)ASSERT(0);
	}
#endif

	double CoeffDupp=m_Param.m_CoeffDuplication;

	// La dupplication ne doit pas permettre � des individus qui ont
	// des probabilit� pauvres d'augmenter de fa�on anormale la couverture 1+
	// On consid�re donc que la duplication peut �tre effectu�e normalement si
	// le nombre d'entrant n'est pas sup�rieur � ceux qui �tait d�j� entr�.
	// Pour �viter de perturber, si on constate que le nombre de nouveaux entrant est trop fort,
	// on consid�re un dupplication minimum ce qui doit permetre d'augmenter la duplication et 
	// donc de diminuer les entrants

	// Calcul du coefficient de dupplication minimum
	double CoeffDuppMin=1-((1-m_VBs[0])/m_VBs[0]);
	if(CoeffDupp<CoeffDuppMin)
	{
		CoeffDupp=CoeffDuppMin;
	}
	double CoeffNouveau=1-CoeffDupp;

	// On v�rifie que ce nouveau coefficient permet d'agir correctement
	ASSERT(m_VBs[0]*CoeffNouveau<=(1-m_VBs[0])+.000001);

	// On boucle autant de fois qu'il existe des insertions >1
	for(int spot=0;spot<NbBoucle;spot++)
	{
		ASSERT(m_NbVBs<=m_Param.m_NbClassesResultatDistribution);

		int Debut=m_NbVBs;
		if(Debut==m_Param.m_NbClassesResultatDistribution)Debut--;
		else 
		{
			m_NbVBs++;
			m_VBs[m_NbVBs]=0;
		}

		// Les contacts qui glissent
		double PointGlisse;
		for(int i=Debut;i>0;i--)
		{
			PointGlisse=m_VBs[i]*CoeffDupp;
			m_VBs[i]-=PointGlisse;
			m_VBs[i+1]+=PointGlisse;
		}

		// Les nouveaux
		PointGlisse=m_VBs[0]*CoeffNouveau;
		m_VBs[0]-=PointGlisse;
		m_VBs[1]+=PointGlisse;
	}

	
	// une petite v�rif ...
#ifdef _DEBUG
	{
		double s=+0.0;
		for(int i=0;i<=m_NbVBs;i++)s+=m_VBs[i];
		if(s<.9999999 || s>1.00000001)ASSERT(0);
	}
#endif


}

// Le calcul des duplications est utilis� pour les �crans lorsque le plan est compos� de 
// Plusieurs insertions (plusieur spots) dans le m�me �cran
// Ce calcul agit sur m_VBs et m_NBVBs

//##ModelId=3D9AB7D202B0
double CTomy::CalculDuplicationNT(int NbInsertion,double nt)
{
	// On boucle autant de fois qu'il existe des insertions >1
	int NbBoucle=NbInsertion-1;
	if(NbBoucle==1)return nt;
	if(NbBoucle<=0)return 0;


	double CoeffDupp=m_Param.m_CoeffDuplication;

	// La dupplication ne doit pas permettre � des individus qui ont
	// des probabilit� pauvres d'augmenter de fa�on anormale la couverture 1+
	// On consid�re donc que la duplication peut �tre effectu�e normalement si
	// le nombre d'entrant n'est pas sup�rieur � ceux qui �tait d�j� entr�.
	// Pour �viter de perturber, si on constate que le nombre de nouveaux entrant est trop fort,
	// on consid�re un dupplication minimum ce qui doit permetre d'augmenter la duplication et 
	// donc de diminuer les entrants

	// Calcul du coefficient de dupplication minimum
	double CoeffDuppMin=1-((1-m_VBs[0])/m_VBs[0]);
	if(CoeffDupp<CoeffDuppMin)
	{
		CoeffDupp=CoeffDuppMin;
	}
	double CoeffNouveau=1-CoeffDupp;

	// On v�rifie que ce nouveau coefficient permet d'agir correctement
	ASSERT(m_VBs[0]*CoeffNouveau<=(1-m_VBs[0])+.000001);

	// On boucle autant de fois qu'il existe des insertions >1
	for(int spot=0;spot<NbBoucle;spot++)
	{
		nt-=nt*CoeffNouveau;
	}

	return nt;
}


//##ModelId=3D9AB7D00375
double CTomyParam::GetCoeffDuplication()
{
	if(m_MethodeComptage)return +0.0;
	if(m_TypeSupport!=1)return +0.0;
	return(m_CoeffDuplication);
}


// Sauvegarde du plan avant le lancement d'un calcul de type zoom
//##ModelId=3D9AB7D2009D
void CTomy::SavePlanBeforeZoom()
{
	m_PlanWhileZoom=new CtabElement;
	m_PlanWhileZoom->Copy(m_Plan);
}


// Restauration du plan apr�s l'utilisation d'un calcul de type zoom
//##ModelId=3D9AB7D20093
void CTomy::RestorePlanAfterZoom()
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

inline void CTomy::CalculApportDansVecteurBinomial(int &NrIndiv)
{
	int maxi,NbVB;
	maxi=NbVB=m_NbVB[NrIndiv];
	double * VBi=m_VB[NrIndiv];

	memcpy(m_VBt,m_VB0,m_sizeVB);

	int a,b,c;
	for(a=0;a<=m_NbVBs;a++)for(b=0;b<=NbVB;b++)
	{
		c=a+b;
		if(c>=m_Cmax)c=m_Cmax-1;
		if(maxi<c)maxi=c;
		m_VBt[c]+=(m_VBs[a])*(VBi[b]);
	}

	// On met � jour les vecteurs
	m_VB[NrIndiv]=m_VBt;
	m_VBt=VBi;
	m_NbVB[NrIndiv]=maxi;

	// Un petit test pour v�rifier que le vecteur binomiale est juste
#ifdef _DEBUG
	double sigma=+0.0;
	for(c=0;c<=m_NbVB[NrIndiv];c++)sigma+=m_VB[NrIndiv][c];
	ASSERT(sigma>0.999999 && sigma<1.000001);
#endif
}

void CTomy::AllocVecteurBinomiale()
{
	m_VB=new double * [m_NbIndiv];
	for(int i=0;i<m_NbIndiv;i++)m_VB[i]=new double [1+NBCLASSEDEGAU];

	if(!m_VBs)
	{
		if(m_Param.GetCoeffDuplication()>0)
		{
			// Si le calcul des duplicaions est actif il est possoble d'avoir
			// un Vecteur Binomiale tr�s gros
			m_VBs=new double [m_Param.m_NbClassesResultatDistribution+1];
		}
		else 
		{
			// C'est que c'est limit� au M�diaWeight maximum
			m_VBs=new double [MWMAX+1];
		}
	}
	m_NbVB=new int [m_NbIndiv];
}

long CTomy::GetNbInsertions()
{
	return 0;
}


/*
#include "c:\dev\newtomeval\ficlbx.h"
int CTomy::GetNbModalite(int NrCritere)
{
	// Chargement des crit�res
	// Note: cette fonction n'�tant utilis�e que pour les ventilations,
	// La rapidit� n'est alors pas crit�re necessaire, on lit les informations dans le fichier au fur et � meusure.
	CFicLBX FicCritere;

	CString CheminPanel;
	CheminPanel.Format("%s\\%s",m_Param.m_CheminData,m_Etude);

	FicCritere.LoadVague(CheminPanel,m_CodePanel);

	return FicCritere.GetNbModalite(NrCritere);
}*/


