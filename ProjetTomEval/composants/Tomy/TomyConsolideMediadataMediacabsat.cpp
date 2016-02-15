// TomyConsolideMediadataMediacabsat.cpp: implementation of the CTomyConsolideMediadataMediacabsat class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "TomyConsolideMediadataMediacabsat.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CTomyConsolideMediadataMediacabsat::CTomyConsolideMediadataMediacabsat()
{
	m_flagMediadata=false;
	m_flagMediacabsat=false;
	m_flagCorrectionPopulationReferenceCabsat=false;
	m_popEquipeTV=1;
	m_popEquipeCabsat=1;
}

CTomyConsolideMediadataMediacabsat::~CTomyConsolideMediadataMediacabsat()
{

}

void CTomyConsolideMediadataMediacabsat::InitialiseCible(int NrCible)
{
	CTomyConsolide::InitialiseCible(NrCible);
	if((m_flagMediacabsat==1)&&(m_flagMediadata==1))
	{

		m_TomyMediadata.CalculPopulationInitialiseEtNonInitialise();
	}

/*	if((m_flagMediacabsat==1)&&(m_flagMediadata==1))
	{
		m_TomyMediadata.SetFiltreIndividuInitialise();
		m_TomyMediacabsat.SetPopulationCouvRepresentative(m_TomyMediadata.GetPopulation());
		m_TomyMediadata.SetFiltreIndividuTous();
	}*/
}

void CTomyConsolideMediadataMediacabsat::InitialiseCibleVentile(int NrCible,int NrCritere1 ,int NrModalite1,int NrCritere2,int NrModalite2)
{
	ASSERT(m_flagMediacabsat==0);
	ASSERT(m_flagMediadata==1);
	CTomyConsolide::InitialiseCibleVentile(NrCible,NrCritere1,NrModalite1,NrCritere2,NrModalite2);
/*	
m_TomyMediadata.SetFiltreIndividuInitialise();
m_TomyMediacabsat.SetPopulationCouvRepresentative(m_TomyMediadata.GetPopulation());
m_TomyMediadata.SetFiltreIndividuTous();
	*/
}



void CTomyConsolideMediadataMediacabsat::InitialisePanel(LPCSTR panel,LPCSTR etude)
{
	if(etude=="MEDIADATA")
	{
		InitialisePanelMediadata(panel);
		return;
	}
	if (etude=="MEDIACABSAT")
	{
		InitialisePanelMediacabsat(panel);
		return;
	}

	// SINON Cette fonction ne doit pas être utilisée
	ASSERT(0);
	AfxThrowUserException();
}

double CTomyConsolideMediadataMediacabsat::LanceEvalDistrib()
{
	if((!m_flagMediadata) || (!m_flagMediacabsat))
	{
		CTomyConsolide::LanceEvalDistrib();
		return GetCouverture();
	}

	// Lorsque les deux études sont chargés, il faut
	// 1/ Faire les évaluations de chacune d'elles
	// 2/ Filtrer pour n'utiliser que les résultats des individus initialsés offre élargie sur MEDIADATA
	// 3/ Redresser la distribution croisée du MEDIACABSAT avec la distribution des individus initialisés offre élargie de MEDIADATA
	// 4/ Filtrer pour n'utiliser que les résultats des individus non initialsés offre élargie sur MEDIADATA
	// 5/ Consolider les résultats en cumulant MEDIADATA+MEDIACABSAT

	
	// 1/ Faire les évaluations de chacune d'elles
	m_TomyMediadata.SetFiltreIndividuTous();
	m_TomyMediadata.LanceEvalDistrib();
	m_TomyMediacabsat.LanceEvalDistributionCroisee();
	
	// 2/ Filtrer pour n'utiliser que les résultats des individus initialsés offre élargie sur MEDIADATA
	m_TomyMediadata.SetFiltreIndividuInitialise(1);
	//int popInitialise=m_TomyMediadata.GetPopulation();
	
	// 3/ Redresser la distribution croisée du MEDIACABSAT avec la distribution des individus initialisés offre élargie de MEDIADATA
	CDistribution DXHertzien=m_TomyMediadata.GetDistributionExactementTouche();
	if(!m_flagCorrectionPopulationReferenceCabsat)
	{
		m_TomyMediacabsat.RedresseDistributionHertzien(DXHertzien);
	}
	else
	{
		// Il faut redresser simultanément les deux cotés.
		// Les distribution du CABSAT doivent anticiper l'expression sur les initialisés
		double coeff=(m_popEquipeCabsat/m_popEquipeTV)*((double)m_TomyMediadata.GetPopulationTous()/(double)m_TomyMediadata.GetPopulationInitialise());
		CDistribution DXThematique=m_TomyMediacabsat.GetDistributionMedia1();
		DXThematique.RedresseDistribution(coeff);
		TRACE("Coeff de correction du thématique :%f\n",coeff);

		//m_TomyMediacabsat.RedresseDistributionCabsat(DXThematique);
		m_TomyMediacabsat.RedresseDistributionCroisee(DXHertzien,DXThematique);

		// Le GRP aussi doit subit le redressement
		m_TomyMediacabsat.RedresseGRPThematique(coeff);
	}
	
	m_TomyMediacabsat.SetPopulationCouvRepresentative(m_TomyMediadata.GetPopulation());
	
	// 4/ Filtrer pour n'utiliser que les résultats
	// des individus non initialsés offre élargie sur la bande MEDIADATA
	m_TomyMediadata.SetFiltreIndividuNonInitialise(1);

	
	// 5/ Consolider les résultats en cumulant MEDIADATA+MEDIACABSAT
	return GetCouverture();
}


void CTomyConsolideMediadataMediacabsat::InitialisePanelMediadata(LPCSTR panel)
{
	// Il faut d'abord le mettre dans le consolidé
	// Pour que les paramètres soient bons
	InitialiseJob();
	AddJob(&m_TomyMediadata);
	m_TomyMediadata.InitialisePanel(panel,"MEDIADATA");
	m_flagMediadata=true;
	m_flagMediacabsat=false;
	m_flagCorrectionPopulationReferenceCabsat=false;
	m_popEquipeTV=1;
	m_popEquipeCabsat=1;
}

void CTomyConsolideMediadataMediacabsat::InitialisePanelMediacabsat(LPCSTR panel)
{
	InitialiseJob();
	AddJob(&m_TomyMediacabsat);
	m_TomyMediacabsat.InitialisePanel(panel,"MEDIACABSAT");
	m_flagMediadata=false;
	m_flagMediacabsat=true;
	m_flagCorrectionPopulationReferenceCabsat=false;
	m_popEquipeTV=1;
	m_popEquipeCabsat=1;
}

void CTomyConsolideMediadataMediacabsat::InitialisePanelMediadataETMediacabsat(LPCSTR panelMediadata,LPCSTR panelMediacabsat)
{
	InitialiseJob();
	AddJob(&m_TomyMediadata);
	AddJob(&m_TomyMediacabsat);
	m_TomyMediadata.InitialisePanel(panelMediadata,"MEDIADATA");
	m_TomyMediacabsat.InitialisePanel(panelMediacabsat,"MEDIACABSAT");
	m_flagMediadata=true;
	m_flagMediacabsat=true;
	m_flagCorrectionPopulationReferenceCabsat=false;
	m_popEquipeTV=1;
	m_popEquipeCabsat=1;
}

void CTomyConsolideMediadataMediacabsat::InitialiseSpot()
{
	m_NbSpot=0;
	CTomyConsolide::InitialiseSpot();
}

void CTomyConsolideMediadataMediacabsat::AddSpot(short Station,short JourNomme,short QHeure,double GRP,short NbInsertion/*=1*/)
{
	ASSERT(GRP>=0);
	m_NbSpot++;
	if(m_flagMediadata	&& m_flagMediacabsat)
	{
		// MEDIADATA+MEDIACABSAT
		double GRPBaseInitialise;
		if(m_LienChaine.IsChaineThematique(Station))
		{
			// une chaîne thématique
			// le GRP à utiliser ne doit pas utiliser le Médiadata,
			// donc on utilise le GRP étalon pour changer de base (si disponible)
			if(m_flagCorrectionPopulationReferenceCabsat)
			{
				GRPBaseInitialise=GRP*m_popEquipeTV/m_popEquipeCabsat;
			}
			else
			{
				GRPBaseInitialise=GRP*m_TomyMediadata.GetPopulationTous()/m_TomyMediadata.GetPopulationInitialise();
			}
			//AddSpotDansJob(1,Station,JourNomme,QHeure,GRPBaseInitialise,NbInsertion,1);
			m_TomyMediacabsat.AddSpotDansDimension(Station,JourNomme,QHeure,GRPBaseInitialise,NbInsertion,1);
		}
		else
		{
			// une chaîne hertzienne ; donc elle passe par les ondes et par câble/Sat
			AddSpotDansJob(0,Station,JourNomme,QHeure,GRP,NbInsertion);

			// Il faut récupérer le GRP de ce spot sur la sous-cible de individus Initialisés
			// De façon à permettre un calcul du MédiaWeight correct.
			double GRPpopInitialise=m_TomyMediadata.CalculGRP_IndividuInitialise(Station,JourNomme,QHeure,GRP);

			double GRPpopInitialiseBaseCalculInitialise=GRPpopInitialise*m_TomyMediadata.GetPopulationTous()/m_TomyMediadata.GetPopulationInitialise();

			int NrStationHertzienneDansMediacabsat=m_LienChaine.GetNumeroMediacabsat(Station);
			//AddSpotDansJob(1,NrStationHertzienneDansMediacabsat,JourNomme,QHeure,GRPpopInitialiseBaseCalculInitialise,NbInsertion);
			m_TomyMediacabsat.AddSpotDansDimension(NrStationHertzienneDansMediacabsat,JourNomme,QHeure,GRPpopInitialiseBaseCalculInitialise,NbInsertion,0);
		}
		return;
	}

	if(m_flagMediadata	&& (!m_flagMediacabsat))
	{
		// MEDIADATA seul
		if(m_LienChaine.IsChaineThematique(Station))
		{
			AfxMessageBox("Erreur dans l'opération CTomyConsolideMediadataMediacabsat::AddSpot: présence d'un spot thématique sans panel Médiacabsat chargé !");
		}
		else
		{
			// une chaîne hertzienne: ok
			AddSpotDansJob(0,Station,JourNomme,QHeure,GRP,NbInsertion);
		}
		return;
	}

	if((!m_flagMediadata)	&& m_flagMediacabsat)
	{
		// MEDIACABSAT seul
		if(m_LienChaine.IsChaineThematique(Station))
		{
			AddSpotDansJob(0,Station,JourNomme,QHeure,GRP,NbInsertion);
		}
		else
		{
			// une chaîne hertzienne ; donc elle passe par le cable.
			int NrStationHertzienneDansMediacabsat=m_LienChaine.GetNumeroMediacabsat(Station);
			AddSpotDansJob(0,NrStationHertzienneDansMediacabsat,JourNomme,QHeure,GRP,NbInsertion);
		}
		return;
	}

	AfxMessageBox("Erreur dans l'opération CTomyConsolideMediadataMediacabsat::AddSpot: aucun panel n'est sélectionné !");
}

void CTomyConsolideMediadataMediacabsat::UpdateSpot(short Station, short JourNomme,short QHeure,double GRP,short NbInsertion)
{
	m_NbSpot+=NbInsertion;
	if(m_flagMediadata	&& m_flagMediacabsat)
	{
		// MEDIADATA+MEDIACABSAT
		double GRPBaseInitialise;
		if(m_LienChaine.IsChaineThematique(Station))
		{
			// une chaîne thématique
			// le GRP à utiliser ne doit pas utiliser le Médiadata,
			// donc on utilise le GRP étalon pour changer de base (si disponible)
			if(m_flagCorrectionPopulationReferenceCabsat)
			{
				GRPBaseInitialise=GRP*m_popEquipeTV/m_popEquipeCabsat;
			}
			else
			{
				GRPBaseInitialise=GRP*m_TomyMediadata.GetPopulationTous()/m_TomyMediadata.GetPopulationInitialise();
			}
			//UpdateSpotDansJob(1,Station,JourNomme,QHeure,GRPBaseInitialise,NbInsertion,1);
			m_TomyMediacabsat.UpdateSpotDansDimension(Station,JourNomme,QHeure,GRPBaseInitialise,NbInsertion,1);
		}
		else
		{
			// une chaîne hertzienne ; donc elle passe par les ondes et par câble/Sat
			UpdateSpotDansJob(0,Station,JourNomme,QHeure,GRP,NbInsertion);

			// Il faut récupérer le GRP de ce spot sur la sous-cible de individus Initialisés
			// De façon à permettre un calcul du MédiaWeight correct.
			double GRPpopInitialise=m_TomyMediadata.CalculGRP_IndividuInitialise(Station,JourNomme,QHeure,GRP);

			double GRPpopInitialiseBaseCalculInitialise=GRPpopInitialise*m_TomyMediadata.GetPopulationTous()/m_TomyMediadata.GetPopulationInitialise();

			int NrStationHertzienneDansMediacabsat=m_LienChaine.GetNumeroMediacabsat(Station);
			//UpdateSpotDansJob(1,NrStationHertzienneDansMediacabsat,JourNomme,QHeure,GRPpopInitialiseBaseCalculInitialise,NbInsertion);
			m_TomyMediacabsat.UpdateSpotDansDimension(NrStationHertzienneDansMediacabsat,JourNomme,QHeure,GRPpopInitialiseBaseCalculInitialise,NbInsertion,0);
		}
		return;
	}

	if(m_flagMediadata	&& (!m_flagMediacabsat))
	{
		// MEDIADATA seul
		if(m_LienChaine.IsChaineThematique(Station))
		{
			AfxMessageBox("Erreur dans l'opération CTomyConsolideMediadataMediacabsat::UpdateSpot: présence d'un spot thématique sans panel Médiacabsat chargé !");
		}
		else
		{
			// une chaîne hertzienne: ok
			UpdateSpotDansJob(0,Station,JourNomme,QHeure,GRP,NbInsertion);
		}
		return;
	}

	if((!m_flagMediadata)	&& m_flagMediacabsat)
	{
		// MEDIACABSAT seul
		if(m_LienChaine.IsChaineThematique(Station))
		{
			UpdateSpotDansJob(0,Station,JourNomme,QHeure,GRP,NbInsertion);
		}
		else
		{
			// une chaîne hertzienne ; donc elle passe par le cable.
			int NrStationHertzienneDansMediacabsat=m_LienChaine.GetNumeroMediacabsat(Station);
			UpdateSpotDansJob(0,NrStationHertzienneDansMediacabsat,JourNomme,QHeure,GRP,NbInsertion);
		}
		return;
	}

	AfxMessageBox("Erreur dans l'opération CTomyConsolideMediadataMediacabsat::UpdateSpot: aucun panel n'est sélectionné !");
}


double CTomyConsolideMediadataMediacabsat::GetGRP()
{
/*	if(m_flagMediadata	&& m_flagMediacabsat)
	{
		double popNonInitialiseMediadata=m_TomyMediadata.GetPopulation();
		double popInitialiseMediadata=m_TomyMediacabsat.GetPopulation();
		double coeff=popInitialiseMediadata/(popInitialiseMediadata+popNonInitialiseMediadata);
		double GRP=m_TomyMediadata.GetGRP()+m_TomyMediacabsat.GetGRP()*coeff;
		return GRP;
	}
	*/
	return CTomyConsolide::GetGRP();
}

double CTomyConsolideMediadataMediacabsat::GetGRPOffreElargie()
{
	if(!m_flagMediacabsat)return 0;
	return m_TomyMediacabsat.GetGRP();
}

double CTomyConsolideMediadataMediacabsat::GetGRPOffreRestreinte()
{
	if(!m_flagMediacabsat)return 0;
	return m_TomyMediadata.GetGRP();
}

double CTomyConsolideMediadataMediacabsat::GetCouvertureOffreElargie()
{
	if(!m_flagMediacabsat)
	{
		m_TomyMediadata.SetFiltreIndividuInitialise(1);
		double couv=m_TomyMediadata.GetCouverture();
		m_TomyMediadata.SetFiltreIndividuTous(1);
		return couv;
	}
	return m_TomyMediacabsat.GetCouverture();
}

double CTomyConsolideMediadataMediacabsat::GetCouvertureOffreRestreinte()
{
	if(!m_flagMediacabsat)
	{
		m_TomyMediadata.SetFiltreIndividuNonInitialise(1);
		double couv=m_TomyMediadata.GetCouverture();
		m_TomyMediadata.SetFiltreIndividuTous(1);
		return couv;
	}
	return m_TomyMediadata.GetCouverture();
}

double CTomyConsolideMediadataMediacabsat::GetCouvertureValideMinMaxOffreElargie()
{
	if(!m_flagMediacabsat)
	{
		m_TomyMediadata.SetFiltreIndividuInitialise(1);
		double couv=m_TomyMediadata.GetCouvertureValideMinMax();
		m_TomyMediadata.SetFiltreIndividuTous(1);
		return couv;
	}
	return m_TomyMediacabsat.GetCouvertureValideMinMax();
}

double CTomyConsolideMediadataMediacabsat::GetCouvertureValideMinMaxOffreRestreinte()
{
	if(!m_flagMediacabsat)
	{
		m_TomyMediadata.SetFiltreIndividuNonInitialise(1);
		double couv=m_TomyMediadata.GetCouvertureValideMinMax();
		m_TomyMediadata.SetFiltreIndividuTous(1);
		return couv;
	}
	return m_TomyMediadata.GetCouvertureValideMinMax();
}

void CTomyConsolideMediadataMediacabsat::CalculCoeffCorrectionOffreElargie(int popEquipeTV, int popEquipeCabsat)
{
	m_flagCorrectionPopulationReferenceCabsat=1;
	m_popEquipeTV=popEquipeTV;
	m_popEquipeCabsat=popEquipeCabsat;
}

void CTomyConsolideMediadataMediacabsat::ActiveFiltreIndividusStrateConsomation(int NrStrate)
{
	if(m_flagMediacabsat && (!m_flagMediadata))
	{
		AfxMessageBox("Le calcul des PMG sur la bande Médicabsat ne peut être calculée que si l'on dispose d'une bande Médiadata de façon à disposer d'une écoute GLOBALE de la télévision.");
		return;
	}

	// Si on est dans une offre simple (Médiadata seul) cela est suffisant
	if(!m_flagMediacabsat)
	{
		m_TomyMediadata.ActiveFiltreIndividusStrateConsomation(NrStrate);
		return;
	}

	// Il faut calculer sur le Médiadata les proportions de PMG
	// dans la sous-cible des individus Initialisés

	// Pour éviter de le faire 3 fois on sait que la première requète commence 
	// Par le groupe 0. Donc on précalcul et on mémorise

	if(NrStrate==0)
	{
		m_ProportionRatioConsoDansOffreElargie.RemoveAll();
		for(int r=0;r<m_Param->m_NbRatioConsomation;r++)
		{
			m_TomyMediadata.ActiveFiltreIndividusStrateConsomation(r);
			m_ProportionRatioConsoDansOffreElargie.Add(m_TomyMediadata.CalculProportionPMGDansOffreElargie());
			TRACE("Proportion du groupe %d dans Offre élargie: %.2f%%\n",r,m_ProportionRatioConsoDansOffreElargie[r]*100);
		}
	}


	// On calcul le quota début-fin de population que la proportion
	// représente dans le Médiacabsat
	double proportiondebut=0;
	for(int r=0;r<NrStrate;r++)
		proportiondebut+=m_ProportionRatioConsoDansOffreElargie[r];
	double proportionfin;
	if(r<m_Param->m_NbRatioConsomation)
		proportionfin=proportiondebut+m_ProportionRatioConsoDansOffreElargie[r];
	else
		proportionfin=1.0;

	// ESSAI
	proportiondebut=NrStrate/3.0;
	proportionfin=(1+NrStrate)/3.0;

	// On se positionne sur le bon groupe de consomation sur les 2 tomy
	m_TomyMediadata.ActiveFiltreIndividusStrateConsomation(NrStrate);
	m_TomyMediacabsat.ActiveFiltreIndividusStrateConsomation(proportiondebut,proportionfin);
}


void CTomyConsolideMediadataMediacabsat::EtalonneSousGRPCabsatAvecLesSousGRPHertzien()
{
	/*
	Cette fonction doit être utilisée pour redresser le GRP du plan Hertzien sur 
	la bande Médiacabsat. En effet, bien que les GRP soit étallonés, les sous GRP
	calculé sur les initialisé dé Médiadata n'est pas égual aux sous GRP correspondant 
	de la bande Médiacabsat. Idéalement il faudrais refaire l'étalonage spot à spot,
	mais le manque de temps fait que l'on fait l'impasse sur cette "finition"

	La couverture étant déjà redressé par l'intermédiaire de la méthode dichotomique,
	seul le GRP reste faux. Si l'on rentre dans le détail de cette impasse,
	on peut interpreter que la distribution croisée calculée initialement 
	ne tiend pas compte d'un sous-GRP étalonné.
	*/
	
	double GRPHertzien=m_TomyMediadata.GetLastGRPindividuInitialise();
	m_TomyMediacabsat.EtalonneGRPHertzien(GRPHertzien);
	
}

int CTomyConsolideMediadataMediacabsat::GetNbCas()
{
	AfxMessageBox("Cette fonction  est obsolète dans le cas particulier de la consolidation Médiadata-Médiacabsat.");
	return 0;
}

int CTomyConsolideMediadataMediacabsat::GetNbCasMediadata()
{
	return m_TomyMediadata.GetNbCas();
}

int CTomyConsolideMediadataMediacabsat::GetNbCasMediacabsat()
{
	return m_TomyMediacabsat.GetNbCas();
}

void CTomyConsolideMediadataMediacabsat::SetParametres(CTomyParam * param)
{
	CString Nomfic;
	Nomfic.Format("%s\\MediadataToCabsat.Sta",param->m_CheminData);
	m_LienChaine.LoadFile(Nomfic);
	CTomyConsolide::SetParametres(param);
}


