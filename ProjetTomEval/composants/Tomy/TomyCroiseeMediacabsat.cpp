// TomyCroiseeMediacabsat.cpp: implementation of the CTomyCroiseeMediacabsat class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "TomyCroiseeMediacabsat.h"
#include "DistributionCroiseeEx.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CTomyCroiseeMediacabsat::CTomyCroiseeMediacabsat()
{
	m_PopulationRepresentative=0;
}

CTomyCroiseeMediacabsat::~CTomyCroiseeMediacabsat()
{

}


#ifdef OPTION_DUREEECOUTE
#error La fonction addspot n'est disponique que SANS l'option durée d'écoute 
#endif

void CTomyCroiseeMediacabsat::AddSpot(short Station, short JourNomme,short QHeure,double GRP,short NbInsertion)
{
	ASSERT(GRP>=0);
	// Par défaut, si on ajoute des spots, il sont thématique
	AddSpotDansDimension(Station,JourNomme,QHeure,GRP,NbInsertion,1);
}

void CTomyCroiseeMediacabsat::UpdateSpot(short Station, short JourNomme,short QHeure,double GRP,short NbInsertion)
{
	// Par défaut, si on ajoute des spots, il sont thématique
	UpdateSpotDansDimension(Station,JourNomme,QHeure,GRP,NbInsertion,1);
}


void CTomyCroiseeMediacabsat::AddSpotDansDimension(short Station, short JourNomme,short QHeure,double GRP,short NbInsertion,BYTE DimensionMedia)
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

	E.m_Media=DimensionMedia;
	if(E.m_Media==0)
	{
		// Chaîne hertzienne
		// E.m_Media=0;
		m_GRPHertzien+=GRP;
	}
	else
	{
		// Chaîne thématique
		// E.m_Media=1;
		m_GRPThematique+=GRP;
	}
	m_Plan.Add(E);
}

void CTomyCroiseeMediacabsat::UpdateSpotDansDimension(short Station, short JourNomme,short QHeure,double GRP,short NbInsertion,BYTE DimensionMedia)
{
	// Cette fonction est désignée pour un fonctionnement "marginal"
	// elle permet de mettre à jour la liste des spots du plan

	// Cette fonction ne peut pas assumer la gestion de la dupplication
	// Le nombre d'insertion représente les spot sur-dupliqués

	CElement E;
	E.m_Station=Station;
	E.m_JourDate=0;
	E.m_JourNomme=JourNomme;
	E.m_QHeure=QHeure;
	E.m_HoraireDebut=-1; // Evenement
	E.m_Duree=-1; // Evenement
	E.m_GRPgrp=E.m_GRP=GRP;
	E.m_NbInsertion=NbInsertion;
	E.m_Media=DimensionMedia;

	if(E.m_Media==0)
	{
		// Chaîne hertzienne
		// E.m_Media=0;
		m_GRPHertzien+=GRP;
	}
	else
	{
		// Chaîne thématique
		// E.m_Media=1;
		m_GRPThematique+=GRP;
	}


	if(NbInsertion>0)
	{
		// Ajout
		m_Plan.Add(E);
		return;
	}
	else
	{
		// Suppression
		for(int e=0;e<m_Plan.GetSize();e++)
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
	AfxMessageBox("Erreur dans le calcul marginal: un spot à supprimer n'est pas retrouvé dans la liste des spots du calcul en cours!");
}

void CTomyCroiseeMediacabsat::RecalculGRPPlan()
{
	m_GRP=0;
	m_GRPHertzien=0;
	m_GRPThematique=0;
	//Pour le moment, et pour de pas s'emerder, on recalcul les MédiaWeight
	for(int s=0;s<m_Plan.GetSize();s++)
	{
		if(m_Plan[s].m_Media!=0)continue;
		m_GRPHertzien+=RecalculGrpElement(m_Plan[s]);
	}
	for(s=0;s<m_Plan.GetSize();s++)
	{
		if(m_Plan[s].m_Media!=1)continue;
		m_GRPThematique+=RecalculGrpElement(m_Plan[s]);
	}
	// En attendant la correction...
	m_GRP=m_GRPHertzien+m_GRPThematique;
	TRACE("Le GRP recalculé=%f\n",m_GRP);
}

void CTomyCroiseeMediacabsat::InitialiseSpot()
{
	m_GRPThematique=0;
	m_GRPHertzien=0;
	CTomy::InitialiseSpot();
}

void CTomyCroiseeMediacabsat::InitialiseCible(int NrCible)
{
	CTomy::InitialiseCible(NrCible);
	m_PopulationRepresentative=m_Population;
}


void CTomyCroiseeMediacabsat::RedresseDistributionHertzien(CDistribution &DistribHertzien)
{
	CDistributionCroiseeEx MX=m_DistributionCroisee;

#ifdef _DEBUG
	TRACE("Distribution croisée avant redressement :\n");
	MX.TraceContent();
#endif

	MX.RedresseDistributionMarginale0(DistribHertzien);
#ifdef _DEBUG
	TRACE("Distribution croisée après redressement :\n");
	MX.TraceContent();
#endif
	m_DistributionCroisee=MX;
	CalculExactementToucheViaDistributionCroisee();
}

void CTomyCroiseeMediacabsat::RedresseDistributionCabsat(CDistribution &DistribCabsat)
{
	CDistributionCroiseeEx MX=m_DistributionCroisee;
#ifdef _DEBUG
	TRACE("Distribution croisée avant redressement :\n");
	MX.TraceContent();
#endif
	MX.RedresseDistributionMarginale1(DistribCabsat);
#ifdef _DEBUG
	TRACE("Distribution croisée après redressement :\n");
	MX.TraceContent();
#endif
	m_DistributionCroisee=MX;
	CalculExactementToucheViaDistributionCroisee();
}

void CTomyCroiseeMediacabsat::RedresseDistributionCroisee(CDistribution &DistribHertzien,CDistribution &DistribCabsat)
{
	CDistributionCroiseeEx MX=m_DistributionCroisee;
#ifdef _DEBUG
	TRACE("Distribution croisée avant redressement :\n");
	MX.TraceContent();
#endif
	MX.RedresseDistributionMarginale(DistribHertzien,DistribCabsat);
#ifdef _DEBUG
	TRACE("Distribution croisée après redressement :\n");
	MX.TraceContent();
#endif
	m_DistributionCroisee=MX;
	CalculExactementToucheViaDistributionCroisee();
}

void CTomyCroiseeMediacabsat::SetPopulationCouvRepresentative(int pop)
{
	m_PopulationRepresentative=pop;
}




void CTomyCroiseeMediacabsat::RedresseGRPThematique(double coeffThematique)
{
	m_GRP=m_GRPHertzien+m_GRPThematique*coeffThematique;
}

void CTomyCroiseeMediacabsat::EtalonneGRPHertzien(double GRPHertzien)
{
	// on essaie de mettre à jour le GRP globale en douce, car le GRP
	// Thématique a peut être été redressé
	
	TRACE("Etalonnage du GRP Hertzien dans Médiacabsat: %.2f devient %.2f\n",m_GRPHertzien,GRPHertzien);

	// On enlève donc au GRP globale l'ancien GRP hertzien
	m_GRP-=m_GRPHertzien;

	m_GRPHertzien=GRPHertzien;

	// On ajoute maintenant le nouveau GRP Hertzien
	m_GRP+=m_GRPHertzien;
};





void CTomyCroiseeMediacabsat::ActiveFiltreIndividusStrateConsomation(double ProportionDebut, double ProportionFin)
{
	// On boucle sur les individus de la cible, 
	// et on indentifie les individus qui sont dans la fourchette donnée.
	// Les autres on les ingnore

	m_flagFiltreIndividus=1;

	m_TotalTV.LoadFile(m_Param.m_CheminData,m_Etude,m_CodePanel);
//	m_TotalTV.CalculFiltreStrateConsoCible(m_CoeffEx,m_FiltreIndividus,NrStrate,m_Param.m_NbRatioConsomation);

	double comptage=0;
	double pop=GetPopulationSourceCouverture();
	for(int i=0;i<m_NbIndiv;i++)
	{
		int NrI=m_TotalTV.GetPlusPetitConsomateur(i);
		m_FiltreIndividus[NrI]=0;
		if(m_CoeffEx[NrI]<=0)continue;
		if((comptage>=ProportionDebut) && (comptage<ProportionFin))
		{
			m_FiltreIndividus[NrI]=1;
		}
		comptage+=m_CoeffEx[NrI]/pop;
	}
	ASSERT(comptage>0.99999 && comptage<1.000001);

#ifdef _DEBUG
	int nbcasselected=0;
	int populationselected=0;
	double conso=0;
	for(i=0;i<m_NbIndiv;i++)
	{
		if(m_CoeffEx[i]<=0)continue;
		if(m_FiltreIndividus[i]<=0)continue;
		populationselected+=m_CoeffEx[i];
		nbcasselected++;
		conso+=m_TotalTV.GetTEI(i)*m_CoeffEx[i];
	}
	conso/=populationselected;
	TRACE("Calcul du groupe strate de conso sur Médiacabsat (%f-%f):%d cas, %d individus ex. soit %.2f%%\n",ProportionDebut,ProportionFin,nbcasselected,populationselected,100.0*(double)populationselected/pop);
	TRACE("Consomation (TEI) du groupe=%.2f%%\n",conso);
#endif

	// Si il existe un filtre permanent on le remet en place
	RestoreFiltrePermanent();

	RecalculGRPPlan();

}
