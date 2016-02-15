// TomyMediadata.cpp: implementation of the CTomyMediadata class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "TomyMediadata.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CTomyMediadata::CTomyMediadata()
{
	m_NrCritereOffreElargie=34;
	m_FiltreIndividuInitialiseOffreElargie=NULL;
	m_FiltreIndividuNonInitialiseOffreElargie=NULL;

	m_PopulationInitialise=0;
	m_PopulationNonInitialise=0;
	m_PopulationTous=0;

	m_DernierFiltre=IndividuTous;
	m_DernierFiltreStrateConsomation=-1;
}

CTomyMediadata::~CTomyMediadata()
{
	FreeFiltre();
}

// Récupération de l'information "Offre élargie" sur le panel chargé
void CTomyMediadata::LoadCritereOffreElargie()
{
	FreeFiltre();
	char * filtre=GetCritere(m_NrCritereOffreElargie);

	m_FiltreIndividuInitialiseOffreElargie=new double [m_NbIndiv];
	m_FiltreIndividuNonInitialiseOffreElargie=new double [m_NbIndiv];

	for(int i=0;i<m_NbIndiv;i++)
	{
		if(filtre[i])
		{
			m_FiltreIndividuInitialiseOffreElargie[i]=1;
			m_FiltreIndividuNonInitialiseOffreElargie[i]=0;
		}
		else
		{
			m_FiltreIndividuInitialiseOffreElargie[i]=0;
			m_FiltreIndividuNonInitialiseOffreElargie[i]=1;
		}
	}

	delete [] filtre;
}

void CTomyMediadata::InitialisePanel(LPCSTR panel,LPCSTR etude)
{
	CTomy::InitialisePanel(panel,etude);
	LoadCritereOffreElargie();
}


void CTomyMediadata::FreePanel()
{
	FreeFiltre();
	CTomy::FreePanel();
}


void CTomyMediadata::SetFiltreIndividuInitialise(bool flagRecalculDistributionExactementTouche)
{
	SetFiltreIndividusPermanent(m_FiltreIndividuInitialiseOffreElargie);
	if(m_DernierFiltreStrateConsomation>=0)ActiveFiltreIndividusStrateConsomation(m_DernierFiltreStrateConsomation);
	if(flagRecalculDistributionExactementTouche)
	{
		RecalculGRPPlan();
		m_GRPInitialise=m_GRP;
		m_Couverture=CalculExactementTouche();	
	}
	m_DernierFiltre=IndividuInitialise;
}

void CTomyMediadata::RecalculGRPPlan()
{
	m_GRP=0;
	//Pour le moment, et pour de pas s'emerder, on recalcul les MédiaWeight
	for(int s=0;s<m_Plan.GetSize();s++)
	{
		m_GRP+=RecalculGrpElement(m_Plan[s]);
	}
	TRACE("Le GRP recalculé=%f\n",m_GRP);
}

void CTomyMediadata::SetFiltreIndividuNonInitialise(bool flagRecalculDistributionExactementTouche)
{
	SetFiltreIndividusPermanent(m_FiltreIndividuNonInitialiseOffreElargie);
	if(m_DernierFiltreStrateConsomation>=0)ActiveFiltreIndividusStrateConsomation(m_DernierFiltreStrateConsomation);
	if(flagRecalculDistributionExactementTouche)
	{
		RecalculGRPPlan();
		m_GRPNonInitialise=m_GRP;
		m_Couverture=CalculExactementTouche();	
	}
	m_DernierFiltre=IndividuNonInitialise;
}

void CTomyMediadata::SetFiltreIndividuTous(bool flagRecalculDistributionExactementTouche)
{
	SetFiltreIndividusPermanent(NULL);	
	if(m_DernierFiltreStrateConsomation>=0)ActiveFiltreIndividusStrateConsomation(m_DernierFiltreStrateConsomation);
	if(flagRecalculDistributionExactementTouche)
	{
		RecalculGRPPlan();
		m_Couverture=CalculExactementTouche();	
	}
	m_DernierFiltre=IndividuTous;
}

void CTomyMediadata::FreeFiltre()
{
	if (m_FiltreIndividuInitialiseOffreElargie)
		delete [] m_FiltreIndividuInitialiseOffreElargie;
	m_FiltreIndividuInitialiseOffreElargie=NULL;

	if (m_FiltreIndividuNonInitialiseOffreElargie)
		delete [] m_FiltreIndividuNonInitialiseOffreElargie;
	m_FiltreIndividuNonInitialiseOffreElargie=NULL;
}

double CTomyMediadata::CalculGRP_IndividuInitialise(short Station,short JourNomme,short QHeure,double GRP)
{
	CElement E;
	E.m_Station=Station;
	E.m_JourDate=0;
	E.m_JourNomme=JourNomme;
	E.m_QHeure=QHeure;
	E.m_HoraireDebut=-1; // Evenement
	E.m_Duree=-1; // Evenement
	E.m_GRPgrp=E.m_GRP=GRP;
	E.m_NbInsertion=1;
	E.m_Media=0; // Utilisé dans TomyFusion

	if(!TrouveProba(E))
	{
		ASSERT(0);
		return 0;
	}

	long i,NrIndiv;
	double coeff,NbContactNat=0,NbContactInit=0;
	for(i=0;i<E.m_ProbaSupp->m_NbCas;i++)
	{
		NrIndiv=E.m_ProbaSupp->m_NrIndividu[i];
		if((coeff=m_CoeffEx[NrIndiv])<=0)continue;
		NbContactNat+=coeff*m_P[E.m_ProbaSupp->m_Proba[i]];
		if(m_FiltreIndividuInitialiseOffreElargie[NrIndiv]==0)continue;
		NbContactInit+=coeff*m_P[E.m_ProbaSupp->m_Proba[i]];
	}

	if(NbContactNat==0)
	{
		TRACE("Calcul du GRP initialisé impossible ; pas de contact dans le panel Médiadata\n");
		return 0;
	}
	else
	{
		double PartdeGRPDansPopInitialise=PartdeGRPDansPopInitialise=NbContactInit/NbContactNat;
		return(GRP*PartdeGRPDansPopInitialise);
	}
}



void CTomyMediadata::CalculPopulationInitialiseEtNonInitialise()
{
	SetFiltreIndividuInitialise();
	m_PopulationInitialise=GetPopulation();
	SetFiltreIndividuNonInitialise();
	m_PopulationNonInitialise=GetPopulation();
	SetFiltreIndividuTous();
	m_PopulationTous=GetPopulation();
}

void CTomyMediadata::ActiveFiltreIndividusStrateConsomation(int NrStrate)
{
	// Il ne faut pas activer ce filtre si l'on n'est pas sur tous les individus
//	ASSERT(m_DernierFiltre==IndividuTous);

	m_TotalTV.LoadFile(m_Param.m_CheminData,m_Etude,m_CodePanel);
	m_TotalTV.CalculFiltreStrateConsoCible(m_CoeffEx,m_FiltreIndividus,NrStrate,m_Param.m_NbRatioConsomation);
	m_flagFiltreIndividus=1;

	// On calcul de suite la proportion de ce groupe sur l'offre élargie
	double popGroupe=0;
	for(int i=0;i<m_NbIndiv;i++)
	{
		if(m_FiltreIndividuInitialiseOffreElargie[i]>0 && m_FiltreIndividus[i]>0)
			popGroupe+=m_CoeffEx[i];
	}
	m_ProportionDernierGroupePMGsurOffreElargie=popGroupe/m_PopulationInitialise;

#ifdef _DEBUG
	double Conso=0;
	for(i=0;i<m_NbIndiv;i++)
	{
		if(m_FiltreIndividuInitialiseOffreElargie[i]>0 && m_FiltreIndividus[i]>0)
			Conso+=m_TotalTV.GetTEI(i)*m_CoeffEx[i];
	}
	Conso/=popGroupe;
	TRACE("Groupe %d sur les individus initialisés: %.2f%% de la pop avec un TEI=%.2f%%\n",NrStrate,m_ProportionDernierGroupePMGsurOffreElargie*100,Conso);
#endif

	// Si il existe un filtre permanent on le remet en place
	RestoreFiltrePermanent();

	m_DernierFiltreStrateConsomation=NrStrate;
}

double CTomyMediadata::CalculProportionPMGDansOffreElargie()
{
	return(m_ProportionDernierGroupePMGsurOffreElargie);

}

void CTomyMediadata::InitialiseCible(int NrCible)
{
	m_DernierFiltreStrateConsomation=-1;
	CTomy::InitialiseCible(NrCible);
}

void CTomyMediadata::InitialiseCibleVentile(int NrCible,int NrCritere1 ,int NrModalite1,int NrCritere2,int NrModalite2)
{
	m_DernierFiltreStrateConsomation=-1;
	CTomy::InitialiseCibleVentile(NrCible,NrCritere1,NrModalite1,NrCritere2,NrModalite2);
}
