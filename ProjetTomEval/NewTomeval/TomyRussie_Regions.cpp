// TomyRussie.cpp: implementation of the CTomyRussie class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Tomeval.h"
#include "TomyRussie.h"
#include "BetaBinomial.h"
#include "math.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

static const long g_NbRegions = 256;

CPanelLoader CTomyRussie::m_PanelLoader;

CTomyRussie::CTomyRussie()
{
	m_FicProba      = 0;
	m_NbIndiv       = 0;
	m_NrCible       = 0;
	m_CoeffEx       = 0;
	m_NbCas         = 0;
	m_Population    = 0.0;
	m_NbInsertions  = 0;
	m_GRP           = 0.0;
	m_Couverture    = 0.0;
	m_PlanWhileZoom = 0;
	m_AudienceDLY   = 0;
	m_AudienceWLY   = 0;
	m_AudienceMLY   = 0;
}

CFicProba* CTomyRussie::LoadPanel(LPCSTR Panel, LPCSTR Etude)
{
	TRACE("LoadPanel : Chargement du panel\n");

	CFicProba * FicProba = m_PanelLoader.LoadPanel(m_Param.m_CheminData, Panel, m_Param.m_TypeSupport);

	TRACE("LoadPanel : Chargement du panel Ok\n");

	m_NbIndiv = FicProba->GetNbIndiv();

	return(FicProba);
}

void CTomyRussie::BuildAsymptotes()
{
	// on récupère le nombre de stations
	int NbStations = m_FicProba->GetNbStation();

	// on alloue les audiences
	m_AudienceDLY = new long*[NbStations];
	m_AudienceWLY = new bool*[NbStations];
	m_AudienceMLY = new bool*[NbStations];

	// on boucle sur toutes les stations
	for(int IdxStation = 0; IdxStation < NbStations; IdxStation += 1)
	{
		// on alloue les individus
		m_AudienceDLY[IdxStation] = new long[m_NbIndiv];
		m_AudienceWLY[IdxStation] = new bool[m_NbIndiv];
		m_AudienceMLY[IdxStation] = new bool[m_NbIndiv];

		// on boucle sur tous les individus
		for (int IdxIndividu = 0; IdxIndividu < m_NbIndiv; IdxIndividu += 1)
		{
			// on initialise les audiences
			m_AudienceDLY[IdxStation][IdxIndividu] = 0;
			m_AudienceWLY[IdxStation][IdxIndividu] = false;
			m_AudienceMLY[IdxStation][IdxIndividu] = false;
		}

		// on boucle sur tous les jours nommés
		for (int IdxJrNomme = 0; IdxJrNomme < 7; IdxJrNomme += 1)
		{
			// on boucle sur tous les quarts d'heure
			for (int IdxQuartHeure = 0; IdxQuartHeure < 96; IdxQuartHeure += 1)
			{
				// on récupère les écoutes du support
				const CEcouteSupport* pEcoutesDLY = m_FicProba->GetEcoutesQH(IdxStation, IdxJrNomme, IdxQuartHeure);

				// on boucle sur toutes les écoutes
				for (int IdxEcoute = 0; IdxEcoute < pEcoutesDLY->m_NbEcoutes; IdxEcoute += 1)
				{
					// on récupère le numéro de l'individu et la durée d'écoute
					int NoIndividu = pEcoutesDLY->m_pNoIndividus[IdxEcoute];
					int ValEcoute  = pEcoutesDLY->m_pValEcoutes [IdxEcoute];

					// on incrémente les audiences de l'individu à 1 jour
					m_AudienceDLY[IdxStation][NoIndividu] += 1;
				}
			}
		}

		// on récupère les audiences à 1 semaine
		const CEcouteSupport* pEcoutesWLY = m_FicProba->GetEcoutesWLY(IdxStation);

		// on boucle sur toutes les écoutes
		for (int JdxEcoute = 0; JdxEcoute < pEcoutesWLY->m_NbEcoutes; JdxEcoute += 1)
		{
			// on récupère le numéro de l'individu et la durée d'écoute
			int NoIndividu = pEcoutesWLY->m_pNoIndividus[JdxEcoute];
			int ValEcoute  = pEcoutesWLY->m_pValEcoutes [JdxEcoute];

			// on ajoute l'individu à l'audience 1 semaine
			m_AudienceWLY[IdxStation][NoIndividu] |= true;
		}

		// on récupère les audiences à 1 mois
		const CEcouteSupport* pEcoutesMLY = m_FicProba->GetEcoutesMLY(IdxStation);

		// on boucle sur toutes les écoutes
		for (int KdxEcoute = 0; KdxEcoute < pEcoutesMLY->m_NbEcoutes; KdxEcoute += 1)
		{
			// on récupère le numéro de l'individu et la durée d'écoute
			int NoIndividu = pEcoutesMLY->m_pNoIndividus[KdxEcoute];
			int ValEcoute  = pEcoutesMLY->m_pValEcoutes [KdxEcoute];

			// on ajoute l'individu à l'audience 1 mois
			m_AudienceMLY[IdxStation][NoIndividu] |= true;
		}
	}
}

void CTomyRussie::BuildMapStation()
{
	m_MapStation.RemoveAll();
	int NbStations = m_FicProba->GetNbStation();
	for(int IdxStation = 0; IdxStation < NbStations; IdxStation += 1)
		m_MapStation.SetAt(m_FicProba->GetNrStation(IdxStation), IdxStation);
}

void CTomyRussie::AllocIndivPourPanel()
{
	m_CoeffEx = new double[m_NbIndiv];
}

void CTomyRussie::CalculPopulation()
{
	m_NbCas      = 0;
	m_Population = 0.0;
	for(int i = 0; i < m_NbIndiv; i += 1)
	{
		if(m_CoeffEx[i] > 0)
		{
			m_Population += m_CoeffEx[i];
			m_NbCas      += 1;
		}
	}
}

void CTomyRussie::FreeAllocIndiv()
{
	delete [] m_CoeffEx;

	m_CoeffEx = 0;
	m_NbIndiv = 0;
}

void CTomyRussie::FreePanel()
{
	if (m_FicProba)
	{
		if (m_AudienceDLY != 0 && m_AudienceWLY != 0 && m_AudienceMLY != 0)
		{
			int NbStations = m_FicProba->GetNbStation();
			for (int s = 0; s < NbStations; s += 1) {
				delete [] m_AudienceDLY[s];
				delete [] m_AudienceWLY[s];
				delete [] m_AudienceMLY[s];
			}

			delete [] m_AudienceDLY;
			delete [] m_AudienceWLY;
			delete [] m_AudienceMLY;
		}

		m_PanelLoader.UnloadPanel(m_FicProba);
	}

	m_MapStation.RemoveAll();

	m_CodePanel.Empty();
	m_Etude    .Empty();

	m_AudienceDLY = 0;
	m_AudienceWLY = 0;
	m_AudienceMLY = 0;
	m_FicProba    = 0;
}

void CTomyRussie::SetParametres(CTomyParam * param)
{
	m_Param = *param;
}

void CTomyRussie::InitialisePanel(LPCSTR panel, LPCSTR etude)
{
	ASSERT((CString)etude == "RUSSIE");

	if(m_CodePanel == panel && m_Etude == etude) return;

	this->FreeAllocIndiv();
	this->FreePanel();

	m_CodePanel = panel;
	m_Etude     = etude;
	m_FicProba  = this->LoadPanel(m_CodePanel, m_Etude);

	this->AllocIndivPourPanel();
	this->BuildAsymptotes();
	this->BuildMapStation();
}

void CTomyRussie::InitialiseCible(int NrCible)
{
	ASSERT(FALSE);
}

void CTomyRussie::InitialiseCible(long* CoeffExCible)
{
	m_NrCible = -1;

	for(int i=0;i<m_NbIndiv;i++)
		m_CoeffEx[i] = CoeffExCible[i];

	this->CalculPopulation();
}

void CTomyRussie::InitialiseCibleVentile(int NrCible, int NrCritere1, int NrModalite1, int NrCritere2,int NrModalite2)
{
	ASSERT(FALSE);
}

int CTomyRussie::GetNbCas()
{
	return (m_NbCas);
}

int CTomyRussie::GetPopulation()
{
	return ((int)floor(m_Population + 0.5));
}

void CTomyRussie::InitialiseSpot()
{
	m_Plan.SetSize(0,100);
}

bool CTomyRussie::AddMaille(CList<CSupport, CSupport &> & Maille, int IdxStation, int IdxQuartHeure)
{
	// on crée le support
	CSupport Support;

	// on initialise le support
	Support.m_IdxStation    = IdxStation;
	Support.m_IdxQuartHeure = IdxQuartHeure;

	// on récupère la première position
	POSITION Pos = Maille.GetHeadPosition();

	// on boucle sur tous les éléments
	for (int Idx = 0; Idx < Maille.GetCount(); Idx += 1)
	{
		// on récupère l'élément courant
		const CSupport & Sup = Maille.GetNext(Pos);

		// on compare les éléments
		if (Sup.m_IdxStation == Support.m_IdxStation && Sup.m_IdxQuartHeure == Support.m_IdxQuartHeure)
			return (false);
	}

	// on ajoute le support
	Maille.AddHead(Support);

	// on renvoie l'indicateur
	return (true);
}

double CTomyRussie::ExtrapoleAudience(double VolContactsDLY, double VolContactsWLY, double VolContactsMLY, double AudienceDLY, double AudienceWLY, double AudienceMLY, double VolContacts)
{
	// on initialise l'audience
	double Audience = 0.0;

	// on teste le cas à traiter
	if (VolContacts <= VolContactsDLY)
	{
		// on calcule l'audience
		Audience = (AudienceDLY * VolContacts) / VolContactsDLY;
	}
	else if (VolContacts <= VolContactsWLY)
	{
		// on calcule l'audience
		Audience = AudienceDLY + (((AudienceWLY - AudienceDLY) * (VolContacts - VolContactsDLY)) / (VolContactsWLY - VolContactsDLY));
	}
	else if (VolContacts <= VolContactsMLY)
	{
		// on calcule l'audience
		Audience = AudienceWLY + (((AudienceMLY - AudienceWLY) * (VolContacts - VolContactsWLY)) / (VolContactsMLY - VolContactsWLY));
	}
	else
	{
		// on calcule l'audience
		Audience = AudienceMLY;
	}

	// on renvoie l'audience
	return (Audience);
}

void CTomyRussie::DoEtalonnage(CAbaque & Abaque, double Asymptote, double Audience, double Audience7Jrs, double VolContacts, double* pDistribution, long KMax)
{
	// on initialise les bornes
	long Frst = 1;
	long Last = 9;

	// on initialise le diviseur
	long Diviseur = 1;

	// on calcule le GRP unitaire
	double GRPu = VolContacts / Asymptote;

	// on calcule la probabilité
	double Proba = GRPu / (double)KMax; if (Proba >= 1.0) Proba = 0.999;

	// on crée les modèles théoriques
	CBetaBinomial BetaBinomial1Jr;
	CBetaBinomial BetaBinomial7Jrs;

	// on boucle jusqu'à obtenir une précision suffisante
	for (long ItrDiviseur = 0; ItrDiviseur < 6; ItrDiviseur += 1)
	{
		// on calcule le diviseur
		Diviseur *= 10;

		// on initialise la dispersion
		long Disp = -1;

		// on initialise la distance quadratique
		double Quadra = 0.0;

		// on boucle sur toutes les dispersions
		for (long ItrDisp = Frst; ItrDisp <= Last; ItrDisp += 1)
		{
			// on calcule les paramètres Alpha et Beta
			double Alpha = ((double)Diviseur * Proba) / (double)ItrDisp;
			double Beta  = ((double)Diviseur * (1.0 - Proba)) / (double)ItrDisp;

			// on calcule les distributions théoriques
			BetaBinomial1Jr .SetItems(Alpha, Beta, KMax);
			BetaBinomial7Jrs.SetItems(Alpha, Beta, (double)KMax * 7.0);

			// on calcule la différence entre le modèle et la valeur empirique
			double Dif1Jr  = BetaBinomial1Jr .Item(1) - (Audience     / Asymptote);
			double Dif7Jrs = BetaBinomial7Jrs.Item(1) - (Audience7Jrs / Asymptote);

			// on calcule le delta
//			double Delta = (Dif1Jr * Dif1Jr * 0.2) + (Dif7Jrs * Dif7Jrs * 0.8);
			double Delta = (Dif1Jr * Dif1Jr * 0.5) + (Dif7Jrs * Dif7Jrs * 0.5);

			// on teste si on doit garder les paramètres courants
			if (Disp < 0 || Delta < Quadra)
			{
				// on mémorise la dispersion et la distance quadratique
				Disp = ItrDisp; Quadra = Delta;

				// on mémorise les paramètres
				Abaque.m_Delta = Delta;
				Abaque.m_Alpha = Alpha;
				Abaque.m_Beta  = Beta;
				Abaque.m_Prob  = Proba;
				Abaque.m_Disp  = (double)Disp / (double)Diviseur;
			}
		}

		// on calcule les nouvelles bornes
		if (Disp == Frst) Frst = (Disp * 10) - 9; else Frst = (Disp * 10) - 5;
		if (Disp == Last) Last = (Disp * 10) + 9; else Last = (Disp * 10) + 5;
	}
}

void CTomyRussie::DoFiche(int NbCas, double Effectif, double AudienceDLY, double AudienceWLY, double AudienceMLY, double Asymptote, double Audience, double VolContacts, double* pDistribution, long KMax, const CAbaque & Abaque)
{
	// on crée les fichiers
	FILE* pFile = ::fopen("TomevalRU.txt", "wt");

	// on teste la validité du fichier
	if (pFile != 0)
	{
		// on écrit le libellé
		::fprintf(pFile, "Cible\n");
		::fprintf(pFile, "-----\n");

		// on écrit l'effectif et le nombre de cas
		::fprintf(pFile, "Effectif  : %9.f\n", Effectif);
		::fprintf(pFile, "Nb de cas : %9d\n",  NbCas);

		// on saute une ligne
		::fprintf(pFile, "\n");

		// on écrit le libellé
		::fprintf(pFile, "Variables d'etalonnage du modele\n");
		::fprintf(pFile, "--------------------------------\n");

		// on écrit l'asymptote
		::fprintf(pFile, "Asymptote          : %9.0f\n", Asymptote);
		::fprintf(pFile, "Audience           : %9.0f\n", Audience);
		::fprintf(pFile, "Volume de contacts : %9.0f\n", VolContacts);
		::fprintf(pFile, "GRP unitaire  1jr  : %9.4f, %7.2f GRP\n", (VolContacts *  1.0) / Asymptote, (VolContacts * 100.0 *  1.0) / Effectif);
		::fprintf(pFile, "GRP unitaire  7jrs : %9.4f, %7.2f GRP\n", (VolContacts *  7.0) / Asymptote, (VolContacts * 100.0 *  7.0) / Effectif);
		::fprintf(pFile, "GRP unitaire 30jrs : %9.4f, %7.2f GRP\n", (VolContacts * 30.0) / Asymptote, (VolContacts * 100.0 * 30.0) / Effectif);

		// on saute une ligne
		::fprintf(pFile, "\n");

		// on écrit les paramètres du modèle
		::fprintf(pFile, "h  = %.2f\n", Audience / AudienceDLY);
		::fprintf(pFile, "p0 = %.6f, t0 = %.6f, Delta = %.6f\n", Abaque.m_Prob, Abaque.m_Disp, Abaque.m_Delta);

		// on saute une ligne
		::fprintf(pFile, "\n");

		// on calcule le modèle
		CBetaBinomial BetaBinomial1jr;   BetaBinomial1jr  .SetItems(Abaque.m_Alpha, Abaque.m_Beta, KMax);
		CBetaBinomial BetaBinomial7jrs;  BetaBinomial7jrs .SetItems(Abaque.m_Alpha, Abaque.m_Beta, (double)KMax * 7.0);
		CBetaBinomial BetaBinomial30jrs; BetaBinomial30jrs.SetItems(Abaque.m_Alpha, Abaque.m_Beta, (double)KMax * 30.0);

		// on calcule le coefficient entre l'asymptote et l'effectif
		double CoefA = Asymptote / Effectif;

		// on écrit le tableau
		::fprintf(pFile,"---------------------------------------------------------------------\n");
		::fprintf(pFile,"|        K       | NbIndividus |  Empirique  |    Modèle   |   C%%   |\n");
		::fprintf(pFile,"---------------------------------------------------------------------\n");
		::fprintf(pFile,"|    Non touchés |   %9.0f |   %9.4f |   %9.4f | %6.2f |\n", Asymptote - Audience, (Asymptote - Audience) / Asymptote, BetaBinomial1jr.Item(0) - BetaBinomial1jr.Item(1), (BetaBinomial1jr.Item(0) - BetaBinomial1jr.Item(1)) * 100.0 * CoefA);
		::fprintf(pFile,"---------------------------------------------------------------------\n");

		// on boucle sur toutes les classes
		for (int Kdx = 1; Kdx <= KMax; Kdx += 1)
		{
			::fprintf(pFile,"|      %9d |   %9.0f |   %9.4f |   %9.4f | %6.2f |\n", Kdx, pDistribution[Kdx - 1], pDistribution[Kdx - 1] / Asymptote, BetaBinomial1jr.Item(Kdx) - BetaBinomial1jr.Item(Kdx + 1), (BetaBinomial1jr.Item(Kdx) - BetaBinomial1jr.Item(Kdx + 1)) * 100.0 * CoefA);
		}

		// on écrit le tableau
		::fprintf(pFile,"---------------------------------------------------------------------\n");
		::fprintf(pFile,"| Audience  1jr  |   %9.0f |   %9.4f |   %9.4f | %6.2f |\n", Audience, Audience / Asymptote, BetaBinomial1jr.Item(1), BetaBinomial1jr.Item(1) * 100.0 * CoefA);
		::fprintf(pFile,"| Audience  7jrs | ----------- | ----------- |   %9.4f | %6.2f |\n", BetaBinomial7jrs.Item(1), BetaBinomial7jrs.Item(1) * 100.0 * CoefA);
		::fprintf(pFile,"| Audience 30jrs | ----------- | ----------- |   %9.4f | %6.2f |\n", BetaBinomial30jrs.Item(1), BetaBinomial30jrs.Item(1) * 100.0 * CoefA);
		::fprintf(pFile,"---------------------------------------------------------------------\n");
		::fprintf(pFile,"|      Asymptote |   %9.0f |   %9.4f |   %9.4f | %6.2f |\n", Asymptote, Asymptote / Asymptote, Asymptote / Asymptote, 100.0 * CoefA);
		::fprintf(pFile,"---------------------------------------------------------------------\n");

		// on saute une ligne
		::fprintf(pFile, "\n");

		// on écrit le libellé
		::fprintf(pFile, "Maille enveloppe\n");
		::fprintf(pFile, "----------------\n");

		// on écrit les couvertures
		::fprintf(pFile, "Daily   Reach : %6.2f%%\n", (AudienceDLY * 100.0) / Effectif);
		::fprintf(pFile, "Weekly  Reach : %6.2f%%\n", (AudienceWLY * 100.0) / Effectif);
		::fprintf(pFile, "Monthly Reach : %6.2f%%\n", (AudienceMLY * 100.0) / Effectif);

		// on saute une ligne
		::fprintf(pFile, "\n");

		// on écrit le libellé
		::fprintf(pFile, "Points de cumulation\n");
		::fprintf(pFile, "--------------------\n");

		// on boucle sur toute les semaines
		for (long GRP = 0; GRP <= 550; GRP += 5)
		{
			// on calcule la valeur du lambda
			double Lambda = ((double)KMax * GRP * Effectif) / (VolContacts * 100.0);

			// on calcule la distribution
			CBetaBinomial BetaBinomial; BetaBinomial.SetItems(Abaque.m_Alpha, Abaque.m_Beta, Lambda);

			// on écrit le point
			::fprintf(pFile, "%3d\t%.4f\t%.4f\n", GRP, (Asymptote * 100.0) / Effectif, BetaBinomial.Item(1) * CoefA * 100.0);
		}

		// on ferme le fichier
		::fclose(pFile);
	}
}

double CTomyRussie::LanceEvalDistrib()
{
	// on crée l'indicateur
	bool IsAbaqueValid = false;

	// on initialise le GRP et la couverture
	m_GRP = m_Couverture = 0.0;

	// on teste si on a quelque chose à faire
	if (m_Population > 0.0 && m_NbIndiv > 0)
	{
		/////////////////////////////////////////////////////////////////////
		//                           INITIALISATION
		/////////////////////////////////////////////////////////////////////

		// on crée le map des régions
		bool MapRegions[g_NbRegions];

		// on crée les tribus et les régions des individus
		long* pTribus  = new long[m_NbIndiv];
		long* pRegions = new long[m_NbIndiv];

		// on crée les audiences des stations
		long* pAudiencesDLY = new long[m_NbIndiv];
		bool* pAudiencesWLY = new bool[m_NbIndiv];
		bool* pAudiencesMLY = new bool[m_NbIndiv];

		// on boucle sur toutes les régions
		for (int Rdx = 0; Rdx < g_NbRegions; Rdx += 1)
		{
			// on initialise l'indicateur de la région
			MapRegions[Rdx] = false;
		}

		// on boucle sur tous les individus
		for (int Idx = 0; Idx < m_NbIndiv; Idx += 1)
		{
			// on récupère la région de l'individu
			pRegions[Idx] = (long)m_FicProba->GetRegionsIDV(Idx); if (pRegions[Idx] < 0) pRegions[Idx] += 256;

			// on positionne l'indicateur de la région
			if (m_CoeffEx[Idx] > 0.0) MapRegions[pRegions[Idx]] = true;

			// on initialise les audiences
			pAudiencesDLY[Idx] = 0;
			pAudiencesWLY[Idx] = false;
			pAudiencesMLY[Idx] = false;

			// on initialise le tribu
			pTribus[Idx] = 0;
		}

		// on récupère le nombre de stations
		int NbStations = m_MapStation.GetCount();

		// on crée l'état des stations
		bool* pStations = new bool[NbStations];

		// on boucle sur toutes les stations
		for (int Jdx = 0; Jdx < NbStations; Jdx += 1)
		{
			// on initialise la station
			pStations[Jdx] = false;
		}

		/////////////////////////////////////////////////////////////////////
		//                         PHASE DE COMPTAGE 
		/////////////////////////////////////////////////////////////////////

		// on crée la maille des supports
		CList<CSupport, CSupport &> Maille;

		// on initialise le nombre de spots de la maille
		int NbSpotsMaille = 0;

		// on initialise le nombre de spots et le GRP du plan
		long   NbSpots_Plan = 0;
		double GRP_Plan     = 0.0;

		// on récupère le nombre de spots du plan
		int NbSpots = m_Plan.GetSize();

		// on boucle sur tous les spots
		for(int IdxSpot = 0; IdxSpot < NbSpots; IdxSpot += 1)
		{
			// on récupère le spot courant
			const CElement & Spot = m_Plan[IdxSpot];

			// on teste si le spot génère du GRP
			if (Spot.m_GRP > 0.0 && Spot.m_NbInsertion > 0)
			{
				// on incrémente le nombre de spots et le GRP du plan
				NbSpots_Plan += (long)Spot.m_NbInsertion;
				GRP_Plan     += Spot.m_GRP * (double)Spot.m_NbInsertion;

				/////////////////////////////////////////////////////////////////////

				// on recherche les indices des stations dans le panel
				int IdxStation = -1; if (FALSE == m_MapStation.Lookup(Spot.m_NoStationJFC, IdxStation)) IdxStation = -1;

				// on calcule l'indice du quart d'heure
				int IdxQuartHeure = ((((Spot.m_QHeure / 100) * 2) + ((Spot.m_QHeure % 100) / 30)) - 10) * 2;

				// on calcule l'indice du jour nommé
				int IdxJrNomme = Spot.m_JourNomme;

				// on teste si on a trouvé la station
				if (IdxStation < 0) {
					CString txt;
					txt.Format("Les probabilités de la station %d (numéro JFC) ne sont pas trouvées dans le panel.", Spot.m_NoStationJFC);
					::AfxMessageBox(txt);
					ASSERT(0);
					return (0.0);
				}

				// on teste la validité du quart d'heure
				if (IdxQuartHeure < 0 || IdxQuartHeure > 94) {
					CString txt;
					txt.Format("Le quart d'heure %d est invalide.", Spot.m_QHeure);
					::AfxMessageBox(txt);
					ASSERT(0);
					return (0.0);
				}

				// on teste la validité du jour nommé
				if (IdxJrNomme < 0 || IdxJrNomme > 6) {
					CString txt;
					txt.Format("Le jour nommé %d est invalide.", Spot.m_JourNomme);
					::AfxMessageBox(txt);
					ASSERT(0);
					return (0.0);
				}

				/////////////////////////////////////////////////////////////////////

				// on teste si on a déjà traiter la station
				if (!pStations[IdxStation])
				{
					// on boucle sur tous les individus
					for (int IdxIndividu = 0; IdxIndividu < m_NbIndiv; IdxIndividu += 1)
					{
						// on incrémente les audiences
						if      (m_AudienceDLY[IdxStation][IdxIndividu] > 0) { pAudiencesMLY[IdxIndividu] |= true; pAudiencesWLY[IdxIndividu] |= true; pAudiencesDLY[IdxIndividu] += m_AudienceDLY[IdxStation][IdxIndividu];  }
						else if (m_AudienceWLY[IdxStation][IdxIndividu])     { pAudiencesMLY[IdxIndividu] |= true; pAudiencesWLY[IdxIndividu] |= true; }
						else if (m_AudienceMLY[IdxStation][IdxIndividu])     { pAudiencesMLY[IdxIndividu] |= true; }
					}

					// on met à jour l'état de la station
					pStations[IdxStation] |= true;
				}

				/////////////////////////////////////////////////////////////////////

				// on teste si on doit traiter le support
				if (this->AddMaille(Maille, IdxStation, IdxQuartHeure))
				{
					// on incrémente le nombre de spots de la maille
					NbSpotsMaille += 1;

					// on boucle sur tous les jours nommés
					for (int JdxJrNomme = 0; JdxJrNomme < 7; JdxJrNomme += 1)
					{
						// on initialise le quart d'heure
						int JdxQuartHeure = IdxQuartHeure;

						// on fait deux tours, pour les deux quarts d'heure
						for (int NoTour = 0; NoTour < 2; NoTour += 1)
						{
							// on récupère les écoutes du support
							const CEcouteSupport* pEcoutes = m_FicProba->GetEcoutesQH(IdxStation, JdxJrNomme, JdxQuartHeure);

							// on boucle sur toutes les écoutes
							for (int IdxEcoute = 0; IdxEcoute < pEcoutes->m_NbEcoutes; IdxEcoute += 1)
							{
								// on récupère le numéro de l'individu
								int NoIndividu = pEcoutes->m_pNoIndividus[IdxEcoute];

								// on incrémente le tribus de l'individu
								pTribus[NoIndividu] += 1;
							}

							// on passe au quart d'heure suivant
							JdxQuartHeure += 1;
						}
					}
				}
			}
		}

		/////////////////////////////////////////////////////////////////////
		//                         PHASE D'AGREGATION
		/////////////////////////////////////////////////////////////////////

		// on initialise le tribu maximum
		int TribuMax = 0;

		// on initialise l'effectif et le volume de contacts
		double EffectifT    = 0.0;
		double VolContactsT = 0.0;

		// on boucle sur tous les individus
		for (int KdxIndividu = 0; KdxIndividu < m_NbIndiv; KdxIndividu += 1)
		{
			// on teste le poids de l'individu
			if (m_CoeffEx[KdxIndividu] > 0.0)
			{
				// on incrémente l'effectif et le volume de contacts
				EffectifT    += m_CoeffEx[KdxIndividu];
				VolContactsT += m_CoeffEx[KdxIndividu] * ((double)pTribus[KdxIndividu] / 2.0);
			}

			// on met à jour le tribu maximum
			if (pTribus[KdxIndividu] > TribuMax) TribuMax = pTribus[KdxIndividu];
		}

		// on calcule le coefficient entre le GRP d'étalonnage et le GRP d'exploitation
		double CoeffG = 0.0; if (EffectifT > 0.0 && VolContactsT > 0.0) CoeffG = (GRP_Plan * EffectifT) / (VolContactsT * 100);

		/////////////////////////////////////////////////////////////////////
		//                   PHASE D'AGREGATION PAR REGION
		/////////////////////////////////////////////////////////////////////

		// on calcule la classe maximum empirique
		int KMax = (TribuMax + 1) / 2; if (KMax < NbSpotsMaille) KMax = NbSpotsMaille;

		// on teste si on a quelque chose à étalonner
		if (KMax > 0 && EffectifT > 0.0 && CoeffG > 0.0 && GRP_Plan > 0.0)
		{
			// on initialise la couverture
			double Couverture = 0.0;

			// on alloue les résultats
			double* pResultats = new double[m_Param.m_NbClassesResultatDistribution + 1];

			// on initialise les résultats
			for (int IdxResultat = 0; IdxResultat <= m_Param.m_NbClassesResultatDistribution; IdxResultat += 1) pResultats[IdxResultat] = 0.0;

			// on alloue la distribution
			double* pDistribution = new double[KMax];

			// on boucle sur toutes les régions
			for (int Qdx = 0; Qdx < g_NbRegions; Qdx += 1)
			{
				// on teste si la région est active
				if (MapRegions[Qdx])
				{
					// on initialise l'effectif
					double Effectif = 0.0;

					// on initialise les audiences
					double AudienceDLY = 0.0;
					double AudienceWLY = 0.0;
					double AudienceMLY = 0.0;

					// on initialise le volume de contacts
					double VolContactsDLY = 0.0;

					// on initialise l'audience le volumes de contacts de la maille
					double Audience    = 0.0;
					double VolContacts = 0.0;

					// on initialise la distribution de contacts de la maille
					for (int Kdx = 0; Kdx < KMax; Kdx += 1) pDistribution[Kdx] = 0.0;

					// on boucle sur tous les individus
					for (int JdxIndividu = 0; JdxIndividu < m_NbIndiv; JdxIndividu += 1)
					{
						// on teste la région et le poids de l'individu
						if (pRegions[JdxIndividu] == Qdx && m_CoeffEx[JdxIndividu] > 0.0)
						{
							// on incrémente l'effectif
							Effectif += m_CoeffEx[JdxIndividu];

							// on calcule le demi-poids de l'individu
							double Poids = m_CoeffEx[JdxIndividu] / 2.0;

							// on teste si l'individu a un tribu
							if (pTribus[JdxIndividu] > 0)
							{
								// on calcule les deux classes de contacts
								long KFistM = (pTribus[JdxIndividu])     / 2;
								long KLastM = (pTribus[JdxIndividu] + 1) / 2;

								// on incrémente les audiences, les volumes de contacts et les distributions de contacts
								if (KFistM > 0) { Audience += Poids; VolContacts += Poids * (double)KFistM; pDistribution[KFistM - 1] += Poids; }
								if (KLastM > 0) { Audience += Poids; VolContacts += Poids * (double)KLastM; pDistribution[KLastM - 1] += Poids; }
							}

							// on incrémente les audiences
							if (pAudiencesDLY[JdxIndividu] > 0) { AudienceDLY += m_CoeffEx[JdxIndividu]; VolContactsDLY += (m_CoeffEx[JdxIndividu] * (double)pAudiencesDLY[JdxIndividu]) / 2.0; }
							if (pAudiencesWLY[JdxIndividu])     { AudienceWLY += m_CoeffEx[JdxIndividu]; }
							if (pAudiencesMLY[JdxIndividu])     { AudienceMLY += m_CoeffEx[JdxIndividu]; }
						}
					}

					// on teste si on a quoi étalonner un abaque
					if (Effectif > 0.0 && AudienceDLY > 0.0 && AudienceWLY > 0.0 && AudienceMLY > 0.0 && VolContactsDLY > 0.0 && Audience > 0.0 && VolContacts > 0.0)
					{
						/////////////////////////////////////////////////////////////////////
						//                    CALCUL DE L'ASYMPTOTE ENVELOPPE
						/////////////////////////////////////////////////////////////////////

						// on estime empiriquement l'asymptote de l'enveloppe
						double AsymptoteMLY = Effectif * (1.0 - pow(1.0 - (AudienceMLY / Effectif), 1.05));

						/////////////////////////////////////////////////////////////////////
						//                    CALCUL DE L'AUDIENCE ENVELOPPE
						/////////////////////////////////////////////////////////////////////

						// on calcule les volumes de contacts à 7 et à 30 jours
						double VolContactsWLY = VolContactsDLY *  7.0;
						double VolContactsMLY = VolContactsDLY * 30.0;

						// on extrapole l'audience enveloppe à 30 fois le grp de la maille
						double AudienceEx = this->ExtrapoleAudience(VolContactsDLY, VolContactsWLY, VolContactsMLY, AudienceDLY, AudienceWLY, AudienceMLY, VolContacts * 30.0);

						/////////////////////////////////////////////////////////////////////
						//               CALCUL DE L'AUDIENCE 7 JOURS DE LA MAILLE
						/////////////////////////////////////////////////////////////////////

						// on estime empiriquement l'asymptote de la maille
						double Asymptote = AsymptoteMLY * (1.0 - pow(1.0 - (AudienceEx / AsymptoteMLY), 1.05));

						// on calcule l'audience à 7 jours de la maille
						double Audience7Jrs = (Audience * AudienceWLY) / AudienceDLY; if (Audience7Jrs > Asymptote) throw "Error";

						/////////////////////////////////////////////////////////////////////
						//                               ETALONNAGE
						/////////////////////////////////////////////////////////////////////

						// on crée l'abaque
						CAbaque Abaque;

						// on effectue l'étalonnage
						this->DoEtalonnage(Abaque, Asymptote, Audience, Audience7Jrs, VolContacts, pDistribution, KMax);

						// on écrit la fiche
//						this->DoFiche(m_NbCas, Effectif, AudienceDLY, AudienceWLY, AudienceMLY, Asymptote, Audience, VolContacts, pDistribution, KMax, Abaque);

						// on crée le modèle théorique
						CBetaBinomial BetaBinomial;

						// on calcule la valeur du lambda
						double Lambda = (double)KMax * CoeffG;

						// on calcule la distribution
						BetaBinomial.SetItems(Abaque.m_Alpha, Abaque.m_Beta, Lambda);

						// on met à jour les résultats
						for (int JdxResultat = 0; JdxResultat < m_Param.m_NbClassesResultatDistribution; JdxResultat += 1)
							pResultats[JdxResultat] += (BetaBinomial.Item(JdxResultat) - BetaBinomial.Item(JdxResultat + 1)) * Asymptote;
						pResultats[m_Param.m_NbClassesResultatDistribution] += BetaBinomial.Item(m_Param.m_NbClassesResultatDistribution) * Asymptote;

						// on incrémente la couverture
						Couverture += BetaBinomial.Item(1) * Asymptote;
					}
				}
			}

			// on alloue la distribution de contacts
			m_Distribution.CreateNewDistribution(m_Param.m_NbClassesResultatDistribution + 1);

			// on remplit la distribution de contacts
			for(int c = 0; c <= m_Param.m_NbClassesResultatDistribution; c += 1)
				m_Distribution.SetValue(c, (pResultats[c] * 100.0) / EffectifT);

			// on initialise le couverture et le GRP
			m_Couverture = (Couverture * 100.0) / EffectifT;
			m_GRP        = GRP_Plan;

			// on positionne l'indicateur
			IsAbaqueValid = true;

			// on libère la distribution
			delete [] pDistribution;

			// on libère les résultats
			delete [] pResultats;
		}

		/////////////////////////////////////////////////////////////////////
		//                      LIBERATION
		/////////////////////////////////////////////////////////////////////

		// on libère les stations
		delete [] pStations;

		// on libère les audiences
		delete [] pAudiencesMLY;
		delete [] pAudiencesWLY;
		delete [] pAudiencesDLY;

		// on libère les régions et les tribus
		delete [] pRegions;
		delete [] pTribus;
	}

	// on teste la validité de l'abaque
	if (!IsAbaqueValid)
	{
		// on alloue la distribution de contacts
		m_Distribution.CreateNewDistribution(m_Param.m_NbClassesResultatDistribution + 1);

		// on initialise la distribution de contacts
		for(int c = 0; c <= m_Param.m_NbClassesResultatDistribution; c += 1)
			m_Distribution.SetValue(c, 0.0);
	}

	// on renvoie la couverture
	return (m_Couverture);
}
/*
double CTomyRussie::LanceEvalDistrib()
{
	// on crée l'indicateur
	bool IsAbaqueValid = false;

	// on initialise le GRP et la couverture
	m_GRP = m_Couverture = 0.0;

	// on teste si on a quelque chose à faire
	if (m_Population > 0.0 && m_NbIndiv > 0)
	{
		/////////////////////////////////////////////////////////////////////
		//                           INITIALISATION
		/////////////////////////////////////////////////////////////////////

		// on crée l'effectif
		double Effectif = 0.0;

		// on crée les tribus des individus
		long* pTribus = new long[m_NbIndiv];

		// on crée les audiences des stations
		long* pAudiencesDLY = new long[m_NbIndiv];
		bool* pAudiencesWLY = new bool[m_NbIndiv];
		bool* pAudiencesMLY = new bool[m_NbIndiv];

		// on boucle sur tous les individus
		for (int Idx = 0; Idx < m_NbIndiv; Idx += 1)
		{
			// on incrémente l'effectif
			Effectif += m_CoeffEx[Idx];

			// on initialise les audiences
			pAudiencesDLY[Idx] = 0;
			pAudiencesWLY[Idx] = false;
			pAudiencesMLY[Idx] = false;

			// on initialise le tribu
			pTribus[Idx] = 0;
		}

		// on récupère le nombre de stations
		int NbStations = m_MapStation.GetCount();

		// on crée l'état des stations
		bool* pStations = new bool[NbStations];

		// on boucle sur toutes les stations
		for (int Jdx = 0; Jdx < NbStations; Jdx += 1)
		{
			// on initialise la station
			pStations[Jdx] = false;
		}

		/////////////////////////////////////////////////////////////////////
		//                         PHASE DE COMPTAGE 
		/////////////////////////////////////////////////////////////////////

		// on crée la maille des supports
		CList<CSupport, CSupport &> Maille;

		// on initialise le nombre de spots et le GRP du plan
		long   NbSpots_Plan = 0;
		double GRP_Plan     = 0.0;

		// on initialise le tribu maximal
		int TribuMax = 0;

		// on initialise le nombre de spots et nombre de stations actifs
		int NbSpotsActifs     = 0;
		int NbStationsActives = 0;

		// on récupère le nombre de spots du plan
		int NbSpots = m_Plan.GetSize();

		// on boucle sur tous les spots
		for(int IdxSpot = 0; IdxSpot < NbSpots; IdxSpot += 1)
		{
			// on récupère le spot courant
			const CElement & Spot = m_Plan[IdxSpot];

			// on teste si le spot génère du GRP
			if (Spot.m_GRP > 0.0 && Spot.m_NbInsertion > 0)
			{
				// on incrémente le nombre de spots et le GRP du plan
				NbSpots_Plan += (long)Spot.m_NbInsertion;
				GRP_Plan     += Spot.m_GRP * (double)Spot.m_NbInsertion;

				/////////////////////////////////////////////////////////////////////

				// on recherche les indices des stations dans le panel
				int IdxStation = -1; if (FALSE == m_MapStation.Lookup(Spot.m_NoStationJFC, IdxStation)) IdxStation = -1;

				// on calcule l'indice du quart d'heure
				int IdxQuartHeure = ((((Spot.m_QHeure / 100) * 2) + ((Spot.m_QHeure % 100) / 30)) - 10) * 2;

				// on calcule l'indice du jour nommé
				int IdxJrNomme = Spot.m_JourNomme;

				// on teste si on a trouvé la station
				if (IdxStation < 0) {
					CString txt;
					txt.Format("Les probabilités de la station %d (numéro JFC) ne sont pas trouvées dans le panel.", Spot.m_NoStationJFC);
					::AfxMessageBox(txt);
					ASSERT(0);
					return (0.0);
				}

				// on teste la validité du quart d'heure
				if (IdxQuartHeure < 0 || IdxQuartHeure > 94) {
					CString txt;
					txt.Format("Le quart d'heure %d est invalide.", Spot.m_QHeure);
					::AfxMessageBox(txt);
					ASSERT(0);
					return (0.0);
				}

				// on teste la validité du jour nommé
				if (IdxJrNomme < 0 || IdxJrNomme > 6) {
					CString txt;
					txt.Format("Le jour nommé %d est invalide.", Spot.m_JourNomme);
					::AfxMessageBox(txt);
					ASSERT(0);
					return (0.0);
				}

				/////////////////////////////////////////////////////////////////////

				// on teste si on a déjà traiter la station
				if (!pStations[IdxStation])
				{
					// on boucle sur tous les individus
					for (int IdxIndividu = 0; IdxIndividu < m_NbIndiv; IdxIndividu += 1)
					{
						// on incrémente les audiences
						if      (m_AudienceDLY[IdxStation][IdxIndividu] > 0) { pAudiencesMLY[IdxIndividu] |= true; pAudiencesWLY[IdxIndividu] |= true; pAudiencesDLY[IdxIndividu] += m_AudienceDLY[IdxStation][IdxIndividu];  }
						else if (m_AudienceWLY[IdxStation][IdxIndividu])     { pAudiencesMLY[IdxIndividu] |= true; pAudiencesWLY[IdxIndividu] |= true; }
						else if (m_AudienceMLY[IdxStation][IdxIndividu])     { pAudiencesMLY[IdxIndividu] |= true; }
					}

					// on met à jour l'état de la station
					pStations[IdxStation] |= true;

					// on incrémente le nombre de stations
					NbStationsActives += 1;
				}

				/////////////////////////////////////////////////////////////////////

				// on teste si on doit traiter le support
				if (this->AddMaille(Maille, IdxStation, IdxQuartHeure))
				{
					// on incrémente le nombre de spots actifs
					NbSpotsActifs += 1;

					// on boucle sur tous les jours nommés
					for (int JdxJrNomme = 0; JdxJrNomme < 7; JdxJrNomme += 1)
					{
						// on initialise le quart d'heure
						int JdxQuartHeure = IdxQuartHeure;

						// on fait deux tours, pour les deux quarts d'heure
						for (int NoTour = 0; NoTour < 2; NoTour += 1)
						{
							// on récupère les écoutes du support
							const CEcouteSupport* pEcoutes = m_FicProba->GetEcoutesQH(IdxStation, JdxJrNomme, JdxQuartHeure);

							// on boucle sur toutes les écoutes
							for (int IdxEcoute = 0; IdxEcoute < pEcoutes->m_NbEcoutes; IdxEcoute += 1)
							{
								// on récupère le numéro de l'individu
								int NoIndividu = pEcoutes->m_pNoIndividus[IdxEcoute];

								// on incrémente le tribus de l'individu
								pTribus[NoIndividu] += 1; if (pTribus[NoIndividu] > TribuMax) TribuMax = pTribus[NoIndividu];
							}

							// on passe au quart d'heure suivant
							JdxQuartHeure += 1;
						}
					}
				}
			}
		}

		/////////////////////////////////////////////////////////////////////
		//                        PHASE D'AGREGATION
		/////////////////////////////////////////////////////////////////////

		// on calcule la classe maximum empirique
		int KMax = (TribuMax + 1) / 2; if (KMax < NbSpotsActifs) KMax = NbSpotsActifs;

		// on teste si on a quelque chose à étalonner
		if (KMax > 0 && GRP_Plan > 0.0 && Effectif > 0.0)
		{
			// on initialise les audiences
			double AudienceDLY = 0.0;
			double AudienceWLY = 0.0;
			double AudienceMLY = 0.0;

			// on initialise le volume de contacts
			double VolContactsDLY = 0.0;

			// on initialise l'audience le volumes de contacts de la maille
			double Audience    = 0.0;
			double VolContacts = 0.0;

			// on initialise la distribution de contacts de la maille
			double* pDistribution = new double[KMax]; for (int Kdx = 0; Kdx < KMax; Kdx += 1) pDistribution[Kdx] = 0.0;

			// on boucle sur tous les individus
			for (int JdxIndividu = 0; JdxIndividu < m_NbIndiv; JdxIndividu += 1)
			{
				// on teste le poids de l'individu
				if (m_CoeffEx[JdxIndividu] > 0.0)
				{
					// on calcule le demi-poids de l'individu
					double Poids = m_CoeffEx[JdxIndividu] / 2.0;

					// on teste si l'individu a un tribu
					if (pTribus[JdxIndividu] > 0)
					{
						// on calcule les deux classes de contacts
						long KFistM = (pTribus[JdxIndividu])     / 2;
						long KLastM = (pTribus[JdxIndividu] + 1) / 2;

						// on incrémente les audiences, les volumes de contacts et les distributions de contacts
						if (KFistM > 0) { Audience += Poids; VolContacts += Poids * (double)KFistM; pDistribution[KFistM - 1] += Poids; }
						if (KLastM > 0) { Audience += Poids; VolContacts += Poids * (double)KLastM; pDistribution[KLastM - 1] += Poids; }
					}

					// on incrémente les audiences
					if (pAudiencesDLY[JdxIndividu] > 0) { AudienceDLY += m_CoeffEx[JdxIndividu]; VolContactsDLY += (m_CoeffEx[JdxIndividu] * (double)pAudiencesDLY[JdxIndividu]) / 2.0; }
					if (pAudiencesWLY[JdxIndividu])     { AudienceWLY += m_CoeffEx[JdxIndividu]; }
					if (pAudiencesMLY[JdxIndividu])     { AudienceMLY += m_CoeffEx[JdxIndividu]; }
				}
			}

			// on teste si on a quoi étalonner un abaque
			if (AudienceDLY > 0.0 && AudienceWLY > 0.0 && AudienceMLY > 0.0 && VolContactsDLY > 0.0 && Audience > 0.0 && VolContacts > 0.0)
			{
				/////////////////////////////////////////////////////////////////////
				//                    CALCUL DE L'ASYMPTOTE ENVELOPPE
				/////////////////////////////////////////////////////////////////////

				// on estime empiriquement l'asymptote de l'enveloppe
				double AsymptoteMLY = Effectif * (1.0 - pow(1.0 - (AudienceMLY / Effectif), 1.05));

				/////////////////////////////////////////////////////////////////////
				//                    CALCUL DE L'AUDIENCE ENVELOPPE
				/////////////////////////////////////////////////////////////////////

				// on calcule les volumes de contacts à 7 et à 30 jours
				double VolContactsWLY = VolContactsDLY *  7.0;
				double VolContactsMLY = VolContactsDLY * 30.0;

				// on extrapole l'audience enveloppe à 30 fois le grp de la maille
				double AudienceEx = this->ExtrapoleAudience(VolContactsDLY, VolContactsWLY, VolContactsMLY, AudienceDLY, AudienceWLY, AudienceMLY, VolContacts * 30.0);

				/////////////////////////////////////////////////////////////////////
				//               CALCUL DE L'AUDIENCE 7 JOURS DE LA MAILLE
				/////////////////////////////////////////////////////////////////////

				// on estime empiriquement l'asymptote de la maille
				double Asymptote = AsymptoteMLY * (1.0 - pow(1.0 - (AudienceEx / AsymptoteMLY), 1.05));

				// on calcule l'audience à 7 jours de la maille
				double Audience7Jrs = (Audience * AudienceWLY) / AudienceDLY; if (Audience7Jrs > Asymptote) throw "Error";

				/////////////////////////////////////////////////////////////////////
				//                               ETALONNAGE
				/////////////////////////////////////////////////////////////////////

				// on crée l'abaque
				CAbaque Abaque;

				// on effectue l'étalonnage
				this->DoEtalonnage(Abaque, Asymptote, Audience, Audience7Jrs, VolContacts, pDistribution, KMax);

				// on écrit la fiche
//				this->DoFiche(m_NbCas, Effectif, AudienceDLY, AudienceWLY, AudienceMLY, Asymptote, Audience, VolContacts, pDistribution, KMax, Abaque);

				// on crée le modèle théorique
				CBetaBinomial BetaBinomial;

				// on calcule la valeur du lambda
				double Lambda = ((double)KMax * GRP_Plan * Effectif) / (VolContacts * 100.0);

				// on calcule la distribution
				BetaBinomial.SetItems(Abaque.m_Alpha, Abaque.m_Beta, Lambda);

				// on calcule le coefficient entre l'asymptote et l'effectif
				double CoefA = Asymptote / Effectif;

				// on alloue la distribution de contacts
				m_Distribution.CreateNewDistribution(m_Param.m_NbClassesResultatDistribution + 1);

				// on remplit la distribution de contacts
				for(int c = 0; c < m_Param.m_NbClassesResultatDistribution; c += 1)
					m_Distribution.SetValue(c, (BetaBinomial.Item(c) - BetaBinomial.Item(c + 1)) * CoefA * 100.0);
				m_Distribution.SetValue(m_Param.m_NbClassesResultatDistribution, BetaBinomial.Item(m_Param.m_NbClassesResultatDistribution) * CoefA * 100.0);

				// on initialise le couverture et le GRP
				m_Couverture = BetaBinomial.Item(1) * CoefA * 100.0;
				m_GRP        = GRP_Plan;

				// on positionne l'indicateur
				IsAbaqueValid = true;
			}

			// on libère la distribution
			delete [] pDistribution;
		}

		/////////////////////////////////////////////////////////////////////
		//                      LIBERATION
		/////////////////////////////////////////////////////////////////////

		// on libère les stations
		delete [] pStations;

		// on libère les audiences
		delete [] pAudiencesMLY;
		delete [] pAudiencesWLY;
		delete [] pAudiencesDLY;

		// on libère les tribus
		delete [] pTribus;
	}

	// on teste la validité de l'abaque
	if (!IsAbaqueValid)
	{
		// on alloue la distribution de contacts
		m_Distribution.CreateNewDistribution(m_Param.m_NbClassesResultatDistribution + 1);

		// on initialise la distribution de contacts
		for(int c = 0; c <= m_Param.m_NbClassesResultatDistribution; c += 1)
			m_Distribution.SetValue(c, 0.0);
	}

	// on renvoie la couverture
	return (m_Couverture);
}
*/
double CTomyRussie::GetAsymptote()
{
	return (0.0);
}

#ifdef OPTION_DUREEECOUTE
void CTomyRussie::AddSpot(short Station, int JourDate, short JourNomme, short QHeure, double GRP, short NbInsertion)
{
	ASSERT(GRP>=0);

	CElement E;

	E.m_NoStationJFC  = Station;
	E.m_JourDate      = JourDate;
	E.m_JourNomme     = JourNomme;
	E.m_QHeure        = (QHeure % 10000);
	E.m_OptionAncrage = (QHeure / 10000) > 0;
	E.m_GRP           = GRP;
	E.m_NbInsertion   = NbInsertion;

	m_Plan.Add(E);
}

void CTomyRussie::AddEvenement(short Station, int JourDate, short JourNomme, short HoraireDebut, short Duree, double GRP)
{
}

void CTomyRussie::UpdateSpot(short Station, int JourDate, short JourNomme, short QHeure, double GRP, short NbInsertion)
{
	ASSERT(GRP>=0);

	CElement E;

	E.m_NoStationJFC  = Station;
	E.m_JourDate      = JourDate;
	E.m_JourNomme     = JourNomme;
	E.m_QHeure        = (QHeure % 10000);
	E.m_OptionAncrage = (QHeure / 10000) > 0;
	E.m_GRP           = GRP;
	E.m_NbInsertion   = NbInsertion;

	if (NbInsertion > 0)
	{
		for (int e = 0; e < m_Plan.GetSize(); e += 1)
		{
			if (m_Plan[e].IsEquivalent(&E))
			{
				m_Plan[e].m_NbInsertion += NbInsertion;
				return;
			}
		}
		m_Plan.Add(E);
		return;
	}
	else
	{
		int nbtosuppress = -NbInsertion;
		for(int e = 0; e < m_Plan.GetSize(); e += 1)
		{
			if (m_Plan[e].IsEquivalent(&E))
			{
				int NbEnMoins = nbtosuppress;
				if (NbEnMoins > m_Plan[e].m_NbInsertion) 
					NbEnMoins = m_Plan[e].m_NbInsertion;
				nbtosuppress -= NbEnMoins;
				m_Plan[e].m_NbInsertion -= NbEnMoins;
				if(m_Plan[e].m_NbInsertion == 0)
				{
					m_Plan.RemoveAt(e);
				}
				if(nbtosuppress == 0)
					return;
			}
		}
	}
	AfxMessageBox("Erreur dans le calcul marginal: un spot à supprimer n'est pas retrouvé dans la liste des spots du calcul en cours!");
}
#else
void CTomyRussie::AddSpot(short Station,short JourNomme,short QHeure,double GRP,short NbInsertion)
{
}

void CTomyRussie::UpdateSpot(short Station, short JourNomme,short QHeure,double GRP,short NbInsertion)
{
}
#endif

int CTomyRussie::GetNbSpot()
{
	return (m_Plan.GetSize());
}

long CTomyRussie::GetNbInsertions()
{
	return (m_NbInsertions);
}

double CTomyRussie::GetCouverture()
{
	return (m_Couverture);
}

double CTomyRussie::GetCouvertureValideBeta()
{
	return (0.0);
}

double CTomyRussie::GetCouvertureValideMinMax()
{
	return (0.0);
}

double CTomyRussie::GetGRP()
{
	return (m_GRP);
}

int CTomyRussie::GetNbClasseResultat()
{
	return (m_Param.m_NbClassesResultatDistribution);
}

double CTomyRussie::GetExactementTouche(int categorie)
{
	return (m_Distribution.GetValue(categorie));
}

double CTomyRussie::GetCourbeReponse(int nbfoistouche)
{
	return (100.0);
}

void CTomyRussie::ActiveFiltreIndividusStrateConsomation(int NrStrate)
{
	ASSERT(FALSE);
}

double CTomyRussie::LanceEval()
{
	return (LanceEvalDistrib());
}

void CTomyRussie::SavePlanBeforeZoom()
{
	if (m_PlanWhileZoom == 0) m_PlanWhileZoom = new CTabElement;

	m_PlanWhileZoom->Copy(m_Plan);
}

void CTomyRussie::RestorePlanAfterZoom()
{
	if (m_PlanWhileZoom != 0) m_Plan.Copy(*m_PlanWhileZoom);

	delete m_PlanWhileZoom;

	m_PlanWhileZoom = 0;
}

CTomyRussie::~CTomyRussie()
{
	delete m_PlanWhileZoom;

	this->FreeAllocIndiv();
	this->FreePanel();
}
