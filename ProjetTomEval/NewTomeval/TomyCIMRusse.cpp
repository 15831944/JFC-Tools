 // TomyCIMRusse.cpp: implementation of the CTomyCIMRusse class.
 //
 // ////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Tomeval.h"
#include "TomyCIMRusse.h"
#include "BetaBinomial.h"
#include "math.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

CPanelLoader CTomyCIMRusse::m_PanelLoader;

CTomyCIMRusse::CTomyCIMRusse()
{
   m_FicProba = 0;
   m_NbIndiv = 0;
   m_NrCible = 0;
   m_CoeffEx = 0;
   m_NbCas = 0;
   m_Population = 0.0;
   m_NbInsertions = 0;
   m_GRP = 0.0;
   m_Couverture = 0.0;
   m_PlanWhileZoom = 0;
   m_AudienceWLY = 0;
#ifdef OPTION_NOVA_RUSSE
   m_AudienceJRS = 0;
#endif
}

CFicProba *CTomyCIMRusse::LoadPanel(LPCSTR Panel, LPCSTR Etude,int NbIndividu)
{
   TRACE("LoadPanel : Chargement du panel\n");

   CFicProba *FicProba = m_PanelLoader.LoadPanel(m_Param.m_CheminData, Panel, m_Param.m_TypeSupport,NbIndividu);

   TRACE("LoadPanel : Chargement du panel Ok\n");

   m_NbIndiv = FicProba->GetNbIndiv();

   return (FicProba);
}
#ifdef OPTION_NOVA_RUSSE
void CTomyCIMRusse::BuildAsymptotes()
{
   // on récupère le nombre de stations
   int NbStations = m_FicProba->GetNbStation();

   // on alloue les audiences
   m_AudienceWLY = new bool *[NbStations];

   // on boucle sur toutes les stations
   for (int IdxStation = 0; IdxStation < NbStations; IdxStation += 1) {
      // on alloue les individus
      m_AudienceWLY[IdxStation] = new bool[m_NbIndiv];

      // on boucle sur tous les individus
      for (int IdxIndividu = 0; IdxIndividu < m_NbIndiv; IdxIndividu += 1) {
         // on initialise les audiences
         m_AudienceWLY[IdxStation][IdxIndividu] = false;
      }
      // on récupère les audiences à 1 semaine
      const CEcouteSupport *pEcoutesWLY = m_FicProba->GetEcoutesWLY(IdxStation);

      // on boucle sur toutes les écoutes
      for (int JdxEcoute = 0; JdxEcoute < pEcoutesWLY->m_NbEcoutes; JdxEcoute += 1) {
         // on récupère le numéro de l'individu et la durée d'écoute
         int NoIndividu = pEcoutesWLY->m_pNoIndividus[JdxEcoute];
         int ValEcoute = pEcoutesWLY->m_pValEcoutes[JdxEcoute];

         // on ajoute l'individu à l'audience 1 semaine
         m_AudienceWLY[IdxStation][NoIndividu] |= true;
      }

   }
   m_AudienceJRS = new unsigned char[m_NbIndiv];

  unsigned char *  pEcJr;

  pEcJr = m_FicProba->GetEcoutesJRS();
   for (int IdxIndividu = 0; IdxIndividu < m_NbIndiv; IdxIndividu += 1) {
	   m_AudienceJRS[IdxIndividu] = pEcJr[IdxIndividu];
   }

}
#endif
void CTomyCIMRusse::BuildMapStation()
{
   m_MapStation.RemoveAll();
   int NbStations = m_FicProba->GetNbStation();
   for (int IdxStation = 0; IdxStation < NbStations; IdxStation += 1)
      m_MapStation.SetAt(m_FicProba->GetNrStation(IdxStation), IdxStation);
}

void CTomyCIMRusse::AllocIndivPourPanel()
{
   m_CoeffEx = new double[m_NbIndiv];
}

void CTomyCIMRusse::CalculPopulation()
{
   m_NbCas = 0;
   m_Population = 0.0;
   for (int i = 0; i < m_NbIndiv; i += 1) {
      if (m_CoeffEx[i] > 0) {
         m_Population += m_CoeffEx[i];
         m_NbCas += 1;
      }
   }
}

void CTomyCIMRusse::FreeAllocIndiv()
{
   delete[] m_CoeffEx;
   m_CoeffEx = 0;
   m_NbIndiv = 0;
}

void CTomyCIMRusse::FreePanel()
{
	if (m_FicProba) {
		if (m_AudienceWLY) {
			int NbStations = m_FicProba->GetNbStation();
			for (int s=0;s < NbStations;s++) {
				delete [] m_AudienceWLY[s];
			}
		}
#ifdef OPTION_NOVA_RUSSE
		if (m_AudienceJRS) delete [] m_AudienceJRS;
#endif
      m_PanelLoader.UnloadPanel(m_FicProba);
	}
   m_MapStation.RemoveAll();

   m_CodePanel.Empty();
   m_Etude.Empty();

   m_FicProba = 0;
}

void CTomyCIMRusse::SetParametres(CTomyParam * param)
{
   m_Param = *param;
}

void CTomyCIMRusse::InitialisePanel(LPCSTR panel, LPCSTR etude)
{
   ASSERT((CString) etude == "RUSSIE");

   if (m_CodePanel == panel && m_Etude == etude)
      return;

   this->FreeAllocIndiv();
   this->FreePanel();

   m_CodePanel = panel;
   m_Etude = etude;
   //TODO: passer le nombdre dindividus 
   m_FicProba = this->LoadPanel(m_CodePanel, m_Etude,0);

   this->AllocIndivPourPanel();
#ifdef OPTION_NOVA_RUSSE
   this->BuildAsymptotes();
#endif
   this->BuildMapStation();
}

void CTomyCIMRusse::InitialiseCible(int NrCible)
{
   ASSERT(FALSE);
}

void CTomyCIMRusse::InitialiseCible(long *CoeffExCible)
{
   m_NrCible = -1;

   for (int i = 0; i < m_NbIndiv; i++)
      m_CoeffEx[i] = CoeffExCible[i];

   this->CalculPopulation();
}

void CTomyCIMRusse::InitialiseCibleVentile(int NrCible, int NrCritere1, int NrModalite1, int NrCritere2, int NrModalite2)
{
   ASSERT(FALSE);
}

int CTomyCIMRusse::GetNbCas()
{
   return (m_NbCas);
}

int CTomyCIMRusse::GetPopulation()
{
   return ((int) floor(m_Population + 0.5));
}

void CTomyCIMRusse::InitialiseSpot()
{
   m_Plan.SetSize(0, 100);
}

int CTomyCIMRusse::GetNoSemaine(int JourDate)
{
   // on calcule le date de référence et la date du spot
   COleDateTime DateRef(1816, 1, 1, 0, 0, 0);
   COleDateTime DateSpt(JourDate / 10000, (JourDate % 10000) / 100, JourDate % 100, 0, 0, 0);

   // on calcule le nombre de jours entre les dates
   COleDateTimeSpan Span = DateSpt - DateRef;

   // on renvoie le numéro de la semaine
   return (Span.GetDays() / 7);
}

int *CTomyCIMRusse::CalculerSemaines(int &NbActives)
{
   // on initialise le nombre de semaines actives
   NbActives = 0;

   // on récupère le nombre de spots du plan
   int NbSpots = m_Plan.GetSize();
   if (NbSpots == 0)
      return (0);

   // on alloue les affectations
   int *pAffectations = new int[NbSpots];

   // on initialise les bornes
   int Min = -1;
   int Max = -1;

   // on boucle sur tous les spots
   for (int IdxSpot = 0; IdxSpot < NbSpots; IdxSpot += 1) {
      // on récupère le spot courant
      const CElement & Spot = m_Plan[IdxSpot];

      // on teste si le spot génère du GRP
      if (Spot.m_GRP > 0.0 && Spot.m_NbInsertion > 0) {
         // on calcule le numéro de la semaine
         int NoSemaine = this->GetNoSemaine(Spot.m_JourDate);

         // on met à jour les bornes
         if (Min < 0 || Min > NoSemaine)
            Min = NoSemaine;
         if (Max < 0 || Max < NoSemaine)
            Max = NoSemaine;

         // on initialise l'affectation
         pAffectations[IdxSpot] = NoSemaine;
      }
      else {
         // on initialise l'affectation
         pAffectations[IdxSpot] = -1;
      }
   }

   // on teste si on a au moins une semaine
   if (Min > 0 && Max > 0) {
      // on calcule le nombre de semaines
      int NbSemaines = (Max - Min) + 1;

      // on alloue le vecteur des semaines
      int *pSemaines = new int[NbSemaines];

      // on initialise le vecteur des semaines
      for (int idx = 0; idx < NbSemaines; idx += 1)
         pSemaines[idx] = -1;

      // on boucle sur toutes les affectations
      for (int JdxSpot = 0; JdxSpot < NbSpots; JdxSpot += 1) {
         // on met à jour la semaine
         if (pAffectations[JdxSpot] >= 0)
            pSemaines[pAffectations[JdxSpot] - Min] = 0;
      }

      // on affecte les numéros de semaine
      for (int jdx = 0; jdx < NbSemaines; jdx += 1) {
         // on affecte le numéro de la semaine
         if (pSemaines[jdx] >= 0) {
            pSemaines[jdx] = NbActives;
            NbActives += 1;
         }
      }

      // on boucle sur toutes les affectations
      for (int KdxSpot = 0; KdxSpot < NbSpots; KdxSpot += 1) {
         // on met à jour la semaine
         if (pAffectations[KdxSpot] >= 0)
            pAffectations[KdxSpot] = pSemaines[pAffectations[KdxSpot] - Min];
      }

      // on libère les semaines
      delete[] pSemaines;
   }

   // on renvoie les affectations
   return (pAffectations);
}

double CTomyCIMRusse::CalculerCouverture(int N, double M, double W, double *SW)
{
   int i;
   double a, b, c, k, se;

   if (W == 0.0)
      goto case50;  // Special case

   c = M / log(1.0 - W);
   if (c > -1.0)
      goto case60;  // Special case

   a = -2.0 * (1.0 + c);    // Estimate NBD parameters A and K

case30:
   b = a;
   a = c * (a - (1.0 + a) * log(1.0 + a)) / (1.0 + a + c);
   if (fabs(b - a) < 0.0001)
      goto case40;
   goto case30;

case40:
   k = -M / a;
   a = 0;   // Scale to schedule weekly reach for each week and accumulate

   for (i = 0; i < N; i++) {
      a = a + (pow(1.0 - SW[i], 1.0 / k) - 1.0);
   }
   se = 1.0 - pow(1.0 + a, k);
   return se;

case50:
   return 0.0;

case60:
   a = 0;   // Poisson default

   for (i = 0; i < N; i++) {
      a = a - log(1.0 - SW[i]);
   }
   se = 1.0 - exp(-a);
   return se;
}

double CTomyCIMRusse::fZeroFrequence(double x, double y, short z)
{
   short k;
   double f;
   f = 1;
   for (k = 1; k <= z; k++) {
      f = (f * (1.0 - y) * (x + (double) k - 1.0)) / (x + (double) (k - 1) * (1 - y));
   }
   return f;
}

double *CTomyCIMRusse::CalculerDistribution(int N, double SE, double SR)
{
   /* n  = number of spots in schedule se = extended schedule % reach sr =
    schedule average spot rating */
   double t;
   double b, l, u, a, p;
   int i;
   double *f;

   f = 0;
   if (SE > 0.0) {  // if se = 0 then go to 200 Special case

      if (SE < (1.0 - pow((1.0 - SR), N))) {    // if se >= (1 - (1 - sr)**n) go to 300 Special case

         t = 1.0 - SE;  // Zero frequency

         b = 1.0;   // Initial value for growth parameter

         if (fabs(fZeroFrequence(b, SR, N) - t) >= 0.0001) {    // if (abs (f(b,sr,n)-t)<0.0001) then go to 100

            l = b;  // Find lower and

            u = b;  // upper limits for b

            if (fZeroFrequence(b, SR, N) > t) {
               while (fZeroFrequence(u, SR, N) >= t) {  // do until f(u,sr,n)<t

                  u = u + u / 10.0;
               }
            }
            else {
               while (fZeroFrequence(l, SR, N) <= t) {  // do until f(l,sr,n)>t

                  l = l - 0.1;
               }
            }

            b = u;
            while (fabs(fZeroFrequence(b, SR, N) - t) > 0.0001) {   // Iterate to find value for b

               b = (u + l) / 2.0;
               if (fZeroFrequence(b, SR, N) > t)
                  l = b;
               else
                  u = b;
            }
         }
         // 100
         a = b * SR / (1.0 - SR);   // Scale parameter

         f = new double[N + 1];
         f[0] = 1.0 - SE;   // Zero frequency

         for (i = 1; i <= N; i++) {
            f[i] = (a + (double) i - 1.0) * (double) (N - i + 1.0) * f[i - 1] / ((b + (double) N - (double) i) * (double) i);
         }
         return f;
      }

      // 300
      else {
         f = new double[N + 1];
         p = 1.0 - pow((1.0 - SE), (1.0 / (double) N)); // Binomial default

         f[0] = 1.0 - SE;
         for (i = 1; i <= N; i++) {
            f[i] = (p / (1.0 - p)) * ((double) (N - i + 1) / (double) i) * f[i - 1];
         }
         return f;
      }
   }
   // 200
   else {
      f = new double[N + 1];
      f[0] = 1.0;   // Special case

      for (i = 1; i <= N; i++) {
         f[i] = 0.0;
      }
      return f;
   }
}

#ifdef OPTION_NOVA_RUSSE
double CTomyCIMRusse::LanceEvalDistrib()
{
   // on crée l'indicateur
   bool IsAbaqueValid = false;
   // on initialise le GRP et la couverture
   m_GRP = m_Couverture = 0.0;

   // on teste si on a quelque chose à faire
   if (m_Population > 0.0 && m_NbIndiv > 0) {
	   double SR;
         // on initialise les paramètres du modèle
	   int N;
	   double E = 0.0;
	   double M = 0.0;
		double W = 0.0;
      // on initialise le nombre de spots et le GRP du plan
      long NbSpots_Plan = 0;
      double GRP_Plan = 0.0;

      // ///////////////////////////////////////////////////////////////////////////////////////////
      // on initialise le nombre de semaines actives
      int NbActives = 0;
      // on calcule les affectations de semaine dans les spots
      int *pAffectations = this->CalculerSemaines(NbActives);
      // on teste le nombre de semaines actives
      if (NbActives > 0) {
         // on récupère le nombre de stations
         int NbStations = m_MapStation.GetCount();
         bool *pMAPStations = new bool[NbStations];
         bool 				*pMAPJourStations[7]; // on crée le map des stations - Jours
         unsigned char  *pPTREcoutesJour[7]; 		// Audiences max
         unsigned char  *pPTREcoutesPlan[7];    // Audiences du plan
			double EJ[7];
			double E=0.0;
         for (int sdx = 0; sdx < NbStations; sdx += 1)
				pMAPStations[sdx] = false;
         for (int jdx = 0; jdx < 7; jdx += 1) {
            pMAPJourStations[jdx] = new bool[NbStations];
            pPTREcoutesJour[jdx] = new unsigned char[m_NbIndiv];;
            pPTREcoutesPlan[jdx] = new unsigned char[m_NbIndiv];
				EJ[jdx] = 0.0;
         }
         for (int IdxIndividu = 0; IdxIndividu < m_NbIndiv; IdxIndividu += 1) {
               // on récupère le poids de l'individu
	         double Poids = m_CoeffEx[IdxIndividu];
			unsigned char jdx = m_AudienceJRS[IdxIndividu];
            E  += Poids;
				EJ[jdx] += Poids;
			}

         double *SW = new double[NbActives];
         for (int wdx = 0; wdx < NbActives; wdx += 1)
            SW[wdx] = 0.0;


         // ///////////////////////////////////////////////////////////////////////////////////////////
         // on récupère le nombre de spots du plan
         int NbSpots = m_Plan.GetSize();


         for (int wdx = 0; wdx < NbActives; wdx += 1) {
            // on initialise le map des stations
            for (int jdx = 0; jdx < 7; jdx += 1) {
               for (int sdx = 0; sdx < NbStations; sdx += 1)
                  pMAPJourStations[jdx][sdx] = false;

               // on boucle sur tous les individus
               for (int kdx = 0; kdx < m_NbIndiv; kdx += 1) {
                  pPTREcoutesPlan[jdx][kdx] = 0;		// Audience du Plan
                  pPTREcoutesJour[jdx][kdx] = 0;			// Audience semaine (7jours)
               }
            }

            // on boucle sur tous les spots puis traiter la semains Sdx
            for (int IdxSpot = 0; IdxSpot < NbSpots; IdxSpot += 1) {
               if (pAffectations[IdxSpot] != wdx) continue;  // on teste l'affectation du spot

               // on récupère le spot courant
               const CElement & Spot = m_Plan[IdxSpot];

               // on incrémente le nombre de spots et le GRP du plan
               NbSpots_Plan += (long) Spot.m_NbInsertion;
               GRP_Plan += Spot.m_GRP * (double) Spot.m_NbInsertion;
                  // on calcule l'indice du jour nommé
               int IdxJrNomme = Spot.m_JourNomme;
                 // on teste la validité du jour nommé
               if (IdxJrNomme < 0 || IdxJrNomme > 6) {
                     CString txt;
                     txt.Format("Le jour nommé %d est invalide.", Spot.m_JourNomme);
							::AfxMessageBox(txt);
                     ASSERT(0);
                     return (0.0);
               }
               // ///////////////////////////////////////////////////////////////////////////////////////////
               // on recherche la station du spot
               int IdxStation = -1;
               if (FALSE == m_MapStation.Lookup(Spot.m_NoStationJFC, IdxStation)) IdxStation = -1;

                  // on vérifie la validité de la station
               if (IdxStation < 0 || IdxStation >= NbStations) {
               	CString txt;
                  txt.Format("Les probabilités de la station %d ne sont pas trouvées dans le panel.", Spot.m_NoStationJFC);
						::AfxMessageBox(txt);
                  ASSERT(0);
                  return (0.0);
               }

                  // ///////////////////////////////////////////////////////////////////////////////////////////

                  // on teste si on déjà traité la station-jour
               if (!pMAPJourStations[IdxJrNomme][IdxStation]) {
                     // on boucle sur tous les jours nommés
                     // on initialise le masque
               	unsigned char Mask = 0x01;

                     // on boucle sur tous les quarts d'heure
                  for (int IdxQuartHeure = 0; IdxQuartHeure < 96; IdxQuartHeure += 1) {
                        // on récupère les écoutes du support
                     const CEcouteSupport *pEcoutes = m_FicProba->GetEcoutesQH(IdxStation, IdxJrNomme, IdxQuartHeure);

                     // on boucle sur toutes les écoutes
                     for (int IdxEcoute = 0; IdxEcoute < pEcoutes->m_NbEcoutes; IdxEcoute += 1) {
                           // on récupère le numéro de l'individu et son écoute
                        int NoIndividu = pEcoutes->m_pNoIndividus[IdxEcoute];
                        int ValEcoute = pEcoutes->m_pValEcoutes[IdxEcoute];
								if (m_AudienceJRS[NoIndividu] != IdxJrNomme) {
									CString txt;
									txt.Format("L'appartenant de jour de l'individu %d n'est pas correct",NoIndividu);
									::AfxMessageBox(txt);

								}
                           // on teste le poids de l'individu
                        if (m_CoeffEx[NoIndividu] > 0.0) {
                           // on incrémente le patron d'écoute
                           pPTREcoutesJour[IdxJrNomme][NoIndividu] |= Mask;
                        }
                     }

                        // on permute le masque
                     if (Mask == 0x01)
                        Mask = 0x02;
                     else
                        Mask = 0x01;
                  }		// IdxQuartHeure < 96

                     // on met à jour le map des stations
                  pMAPJourStations[IdxJrNomme][IdxStation] |= true;
						pMAPStations[IdxStation]  |= true;
               }

               // ///////////////////////////////////////////////////////////////////////////////////////////

                  // on calcule l'indice du quart d'heure
               int JdxQuartHeure = ((((Spot.m_QHeure / 100) * 2) + ((Spot.m_QHeure % 100) / 30)) - 10) * 2;			// HeureDebut 500 (5h00)

                  // on teste la validité du quart d'heure
               if (JdxQuartHeure < 0 || JdxQuartHeure > 94) {
                  CString txt;
                  txt.Format("Le quart d'heure %d est invalide.", Spot.m_QHeure);
							:: AfxMessageBox(txt);
                  ASSERT(0);
                  return (0.0);
               }
                  // on initialise le masque
               unsigned char Mask = 0x01;

                  // on initialise le quart d'heure
               int KdxQuartHeure = JdxQuartHeure;

                  // on fait deux tours, pour les deux quarts d'heure
               for (int NoTour = 0; NoTour < 2; NoTour += 1) {
                     // on récupère les écoutes du support
                  const CEcouteSupport *pEcoutes = m_FicProba->GetEcoutesQH(IdxStation, IdxJrNomme, KdxQuartHeure);

                     // on boucle sur toutes les écoutes
                  for (int JdxEcoute = 0; JdxEcoute < pEcoutes->m_NbEcoutes; JdxEcoute += 1) {
                        // on récupère le numéro de l'individu et la durée d'écoute
                     int NoIndividu = pEcoutes->m_pNoIndividus[JdxEcoute];
                     int ValEcoute = pEcoutes->m_pValEcoutes[JdxEcoute];

							if (m_AudienceJRS[NoIndividu] != IdxJrNomme) {
								CString txt;
								txt.Format("L'appartenant de jour de l'individu %d n'est pas correct",NoIndividu);
								::AfxMessageBox(txt);

							}
                        // on teste le poids de l'individu
                     if (m_CoeffEx[NoIndividu] > 0.0) {
                           // on incrémente les audiences de la semaines
                        pPTREcoutesPlan[IdxJrNomme][NoIndividu] |= Mask;
                     }
                  }

                     // on permute le masque
                  if (Mask == 0x01)
                     Mask = 0x02;
                  else
                     Mask = 0x01;

                  // on incrémente le quart d'heure
                  KdxQuartHeure += 1;
               }
            }		// IdxSpot < NbSpots
				// Calculer l'audience de la semaine Sdx
            double AudienceS[7];
            double AudienceC[7];
            for (int jdx = 0; jdx < 7; jdx++) {
               AudienceS[jdx] = 0.0;
               AudienceC[jdx] = 0.0;
            }
            for (int IdxIndividu = 0; IdxIndividu < m_NbIndiv; IdxIndividu += 1) {
               // on récupère le poids de l'individu
               double Poids = m_CoeffEx[IdxIndividu];

               // on teste le poids de l'individu
               if (Poids > 0.0) {
                  // on calcule le demi-poids de l'individu
                  double DemiPoids = Poids / 2.0;
						unsigned char jdx = m_AudienceJRS[IdxIndividu];

                  // for (int jdx = 0; jdx < 7; jdx++) {
                     if ((pPTREcoutesJour[jdx][IdxIndividu] & 0x01) > 0)
                        AudienceS[jdx] += DemiPoids;
                     if ((pPTREcoutesJour[jdx][IdxIndividu] & 0x02) > 0)
                        AudienceS[jdx] += DemiPoids;
                     if ((pPTREcoutesPlan[jdx][IdxIndividu] & 0x01) > 0)
                        AudienceC[jdx] += DemiPoids;
                     if ((pPTREcoutesPlan[jdx][IdxIndividu] & 0x02) > 0)
                        AudienceC[jdx] += DemiPoids;
                  // }
               }
            }
            SR = 0.0;
            for (int sdx = 0; sdx < NbStations; sdx++) {
				int jdx;
					if (!pMAPStations[sdx]) continue;
	            for (jdx = 0; jdx < 7; jdx++)
						if (pMAPJourStations[jdx][sdx]) break;
					if (jdx >= 7) continue;
               for (int IdxIndividu = 0; IdxIndividu < m_NbIndiv; IdxIndividu += 1) {
               	if (m_AudienceWLY[sdx][IdxIndividu]) SR += m_CoeffEx[IdxIndividu];
               }
            }
            SR = SR / E;
            double RCR = 1.0;
            double RSR = 1.0;
            double Cmax = 0.0;
            double Smax = 0.0;
            double CR;
            for (int jdx = 0; jdx < 7; jdx++) {
               double C = AudienceC[jdx] / EJ[jdx];
               double S = AudienceS[jdx] / EJ[jdx];
               if (Cmax < C)
                  Cmax = C;
               if (Smax < S)
                  Smax = S;
               if (AudienceS[jdx])
                  RSR = RSR * (1 - S);
               if (AudienceC[jdx])
                  RCR = RCR * (1 - C);

            }
            RCR = 1 - RCR;
            RSR = 1 - RSR;
            CR = SR * RCR / RSR;

            if (CR <= Cmax) {
               double RCRx,RSRx;
               RCRx = (SR - Smax) * (RCR - Cmax);
               RSRx = RSR - Smax;
               CR = Cmax + RCRx / RSRx;
            }
            SW[wdx] = CR;

         }  // wdx <NbActives

         // ///////////////////////////////////////////////////////////////////////////////////////////

         // on libère les audiences par semaines
         for (int Zdx = 0; Zdx < 7; Zdx += 1) {
            delete[] pPTREcoutesPlan[Zdx];
				delete[] pPTREcoutesJour[Zdx];
            delete[] pMAPJourStations[Zdx];
         }
         unsigned char  *pPTREcoutesSta[7]; 		// Audiences max

         for (int jx = 0; jx < 7; jx += 1) {
            pPTREcoutesSta[jx] = new unsigned char[m_NbIndiv];;
               // on boucle sur tous les individus
            for (int kdx = 0; kdx < m_NbIndiv; kdx += 1) {
               // on initialise les patrons d'écoutes
               pPTREcoutesSta[jx][kdx] = 0;
            }
         }


			for (int IdxStation=0;IdxStation<NbStations;IdxStation++) {
            if (!pMAPStations[IdxStation]) continue;
	         for (int jdx = 0; jdx < 7; jdx += 1) {
	            unsigned char Mask = 0x01;
                     // on boucle sur tous les quarts d'heure
               for (int IdxQuartHeure = 0; IdxQuartHeure < 96; IdxQuartHeure += 1) {
                        // on récupère les écoutes du support
	               const CEcouteSupport *pEcoutes = m_FicProba->GetEcoutesQH(IdxStation, jdx, IdxQuartHeure);

                        // on boucle sur toutes les écoutes
   	            for (int IdxEcoute = 0; IdxEcoute < pEcoutes->m_NbEcoutes; IdxEcoute += 1) {
                           // on récupère le numéro de l'individu et son écoute
   	           		int NoIndividu = pEcoutes->m_pNoIndividus[IdxEcoute];
   	           		int ValEcoute = pEcoutes->m_pValEcoutes[IdxEcoute];

							if (m_AudienceJRS[NoIndividu] != jdx) {
								CString txt;
								txt.Format("L'appartenant de jour de l'individu %d n'est pas correct",NoIndividu);
								::AfxMessageBox(txt);

							}

                           // on teste le poids de l'individu
   	            	if (m_CoeffEx[NoIndividu] > 0.0) {
                              // on incrémente le patron d'écoute
   	               	pPTREcoutesSta[jdx][NoIndividu] |= Mask;
   	              	}
   	            }
   	               // on permute le masque
   	            if (Mask == 0x01)
   	              	Mask = 0x02;
   	            else
   	               Mask = 0x01;
   	         }
				}
			}
			double MJ[7];
         for (int jdx = 0; jdx < 7; jdx += 1) {
				MJ[jdx] = 0.0;
			}

         // on boucle sur tous les individus
         for (int IdxIndividu = 0; IdxIndividu < m_NbIndiv; IdxIndividu += 1) {
            // on récupère le poids de l'individu
            double Poids = m_CoeffEx[IdxIndividu];

            // on teste le poids de l'individu
            if (Poids > 0.0) {
               // on calcule le demi-poids de l'individu
               double DemiPoids = Poids / 2.0;
               unsigned char MaskEcoute = 0x00;

               // ///////////////////////////////////////////////////////////////////////////////////////////
					unsigned char jdx = m_AudienceJRS[IdxIndividu];
               //for (int jdx = 0; jdx < 7; jdx++) {
	               if ((pPTREcoutesSta[jdx][IdxIndividu] & 0x01) > 0)
					   MJ[jdx] += DemiPoids;
	               if ((pPTREcoutesSta[jdx][IdxIndividu] & 0x02) > 0)
					   MJ[jdx] += DemiPoids;
               //}
            }
         }
         for (int jdx = 0; jdx < 7; jdx += 1) {
            delete[] pPTREcoutesSta[jdx];
         }

		   N = NbActives;
         W = 0.0;
         for (int sdx = 0; sdx < NbStations; sdx++) {
				if (!pMAPStations[sdx]) continue;
            for (int IdxIndividu = 0; IdxIndividu < m_NbIndiv; IdxIndividu += 1) {
            	if (m_AudienceWLY[sdx][IdxIndividu]) W += m_CoeffEx[IdxIndividu];
            }
         }


			M = 0.0;
         // on teste l'effectif de la cible
         if (E > 0.0) {
				for (int jdx=0;jdx<7;jdx++) {
					if (EJ[jdx]) M += MJ[jdx]/EJ[jdx];
				}
				W /= E;

            // on calcule la couverture
            double Couv = this->CalculerCouverture(N, M, W, SW);

            // on calcule la distribution
            double *pDistribution = this->CalculerDistribution(NbSpots_Plan, Couv, GRP_Plan / ((double) NbSpots_Plan * 100.0));

            // on alloue la distribution de contacts
            m_Distribution.CreateNewDistribution(m_Param.m_NbClassesResultatDistribution + 1);
            m_Distribution.Initialise();

            // on boucle sur toutes les classes
            for (int c = 0; c <= NbSpots_Plan; c += 1) {
               // on initialise la classe de contacts
               if (c <= m_Param.m_NbClassesResultatDistribution)
                  m_Distribution.SetValue(c, pDistribution[c] * 100.0);
               else
                  m_Distribution.AddValue(m_Param.m_NbClassesResultatDistribution, pDistribution[c] * 100.0);
            }

            // on initialise la couverture et le GRP
            m_Couverture = Couv * 100.0;
            m_GRP = GRP_Plan;

            // on libère la distribution
            delete[] pDistribution;

            // on positionne l'indicateur
            IsAbaqueValid = true;
         }

         // ///////////////////////////////////////////////////////////////////////////////////////////

         // on libère les audiences
         delete[] SW;

			// on libère le map des stations
			delete [] pMAPStations;

      }
			// on libère les affectations
		delete [] pAffectations;
   }

   // on teste la validité de l'abaque
   if (!IsAbaqueValid) {
      // on alloue la distribution de contacts
      m_Distribution.CreateNewDistribution(m_Param.m_NbClassesResultatDistribution + 1);

      // on initialise la distribution de contacts
      for (int c = 0; c <= m_Param.m_NbClassesResultatDistribution; c += 1)
         m_Distribution.SetValue(c, 0.0);
   }

   // on renvoie la couverture
   return (m_Couverture);
}
#else
double CTomyCIMRusse::LanceEvalDistrib()
{
	// on crée l'indicateur
	bool IsAbaqueValid = false;

	// on initialise le GRP et la couverture
	m_GRP = m_Couverture = 0.0;

	// on teste si on a quelque chose à faire
	if (m_Population > 0.0 && m_NbIndiv > 0)
	{
		// on initialise le nombre de spots et le GRP du plan
		long   NbSpots_Plan = 0;
		double GRP_Plan     = 0.0;

		/////////////////////////////////////////////////////////////////////////////////////////////

		// on récupère le nombre de stations
		int NbStations = m_MapStation.GetCount();

		// on crée le map des stations
		bool* pMAPStations = new bool[NbStations];

		// on initialise le map des stations
		for (int idx = 0; idx < NbStations; idx += 1) pMAPStations[idx] = false;

		/////////////////////////////////////////////////////////////////////////////////////////////

		// on initialise le nombre de semaines actives
		int NbActives = 0;

		// on calcule les affectations des spots
		int* pAffectations = this->CalculerSemaines(NbActives);

		/////////////////////////////////////////////////////////////////////////////////////////////

		// on teste le nombre de semaines actives
		if (NbActives > 0)
		{
			// on crée les audiences par semaines
			unsigned char** pPTRSemaines = new unsigned char*[NbActives];

			// on alloue les individus
			for (int jdx = 0; jdx < NbActives; jdx += 1) pPTRSemaines[jdx] = new unsigned char[m_NbIndiv];

			/////////////////////////////////////////////////////////////////////////////////////////////

			// on crée les patrons d'écoutes
			unsigned char* pPTREcoutes[7];

			// on alloue les patrons d'écoute
			pPTREcoutes[0] = new unsigned char[m_NbIndiv]; pPTREcoutes[1] = new unsigned char[m_NbIndiv];
			pPTREcoutes[2] = new unsigned char[m_NbIndiv]; pPTREcoutes[3] = new unsigned char[m_NbIndiv];
			pPTREcoutes[4] = new unsigned char[m_NbIndiv]; pPTREcoutes[5] = new unsigned char[m_NbIndiv];
			pPTREcoutes[6] = new unsigned char[m_NbIndiv];

			/////////////////////////////////////////////////////////////////////////////////////////////

			// on boucle sur tous les individus
			for (int kdx = 0; kdx < m_NbIndiv; kdx += 1)
			{
				// on initialise les audiences par semaine
				for (int Ldx = 0; Ldx < NbActives; Ldx += 1) pPTRSemaines[Ldx][kdx] = 0;

				// on initialise les patrons d'écoutes
				pPTREcoutes[0][kdx] = 0; pPTREcoutes[1][kdx] = 0;
				pPTREcoutes[2][kdx] = 0; pPTREcoutes[3][kdx] = 0;
				pPTREcoutes[4][kdx] = 0; pPTREcoutes[5][kdx] = 0;
				pPTREcoutes[6][kdx] = 0;
			}

			/////////////////////////////////////////////////////////////////////////////////////////////

			// on récupère le nombre de spots du plan
			int NbSpots = m_Plan.GetSize();

			// on boucle sur tous les spots
			for(int IdxSpot = 0; IdxSpot < NbSpots; IdxSpot += 1)
			{
				// on récupère le spot courant
				const CElement & Spot = m_Plan[IdxSpot];

				// on teste l'affectation du spot
				if (pAffectations[IdxSpot] >= 0)
				{
					// on incrémente le nombre de spots et le GRP du plan
					NbSpots_Plan += (long)Spot.m_NbInsertion;
					GRP_Plan     += Spot.m_GRP * (double)Spot.m_NbInsertion;

					/////////////////////////////////////////////////////////////////////////////////////////////

					// on recherche la station du spot
					int IdxStation = -1; if (FALSE == m_MapStation.Lookup(Spot.m_NoStationJFC, IdxStation)) IdxStation = -1;

					// on vérifie la validité de la station
					if (IdxStation < 0 || IdxStation >= NbStations)
					{
						CString txt;
						txt.Format("Les probabilités de la station %d ne sont pas trouvées dans le panel.", Spot.m_NoStationJFC);
						::AfxMessageBox(txt);
						ASSERT(0);
						return (0.0);
					}

					/////////////////////////////////////////////////////////////////////////////////////////////

					// on teste si on déjà traité la station
					if (!pMAPStations[IdxStation])
					{
						// on boucle sur tous les jours nommés
						for (int IdxJrNomme = 0; IdxJrNomme < 7; IdxJrNomme += 1)
						{
							// on initialise le masque
							unsigned char Mask = 0x01;

							// on boucle sur tous les quarts d'heure
							for (int IdxQuartHeure = 0; IdxQuartHeure < 96; IdxQuartHeure += 1)
							{
								// on récupère les écoutes du support
								const CEcouteSupport* pEcoutes = m_FicProba->GetEcoutesQH(IdxStation, IdxJrNomme, IdxQuartHeure);

								// on boucle sur toutes les écoutes
								for (int IdxEcoute = 0; IdxEcoute < pEcoutes->m_NbEcoutes; IdxEcoute += 1)
								{
									// on récupère le numéro de l'individu et son écoute
									int NoIndividu = pEcoutes->m_pNoIndividus[IdxEcoute];
									int ValEcoute  = pEcoutes->m_pValEcoutes [IdxEcoute];

									// on teste le poids de l'individu
									if (m_CoeffEx[NoIndividu] > 0.0)
									{
										// on incrémente le patron d'écoute
										pPTREcoutes[IdxJrNomme][NoIndividu] |= Mask;
									}
								}

								// on permute le masque
								if (Mask == 0x01) Mask = 0x02; else Mask = 0x01;
							}
						}

						// on met à jour le map des stations
						pMAPStations[IdxStation] |= true;
					}

					/////////////////////////////////////////////////////////////////////////////////////////////

					// on calcule l'indice du quart d'heure
#ifdef OPTION_MAROC
					int JdxQuartHeure= ((((Spot.m_QHeure / 100) * 2) + ((Spot.m_QHeure % 100) / 30)) - 6) * 2;		// HeureDebut : 3h00 (300) de la journee
#else
					int JdxQuartHeure = ((((Spot.m_QHeure / 100) * 2) + ((Spot.m_QHeure % 100) / 30)) - 10) * 2;	// HeureDebut : 5h00 (500) de la journee
#endif


					// on calcule l'indice du jour nommé
					int JdxJrNomme = Spot.m_JourNomme;

					// on teste la validité du quart d'heure
					if (JdxQuartHeure < 0 || JdxQuartHeure > 94) {
						CString txt;
						txt.Format("Le quart d'heure %d est invalide.", Spot.m_QHeure);
						::AfxMessageBox(txt);
						ASSERT(0);
						return (0.0);
					}

					// on teste la validité du jour nommé
					if (JdxJrNomme < 0 || JdxJrNomme > 6) {
						CString txt;
						txt.Format("Le jour nommé %d est invalide.", Spot.m_JourNomme);
						::AfxMessageBox(txt);
						ASSERT(0);
						return (0.0);
					}

					// on initialise le masque
					unsigned char Mask = 0x01;

					// on initialise le quart d'heure
					int KdxQuartHeure = JdxQuartHeure;

					// on fait deux tours, pour les deux quarts d'heure
					for (int NoTour = 0; NoTour < 2; NoTour += 1)
					{
						// on récupère les écoutes du support
						const CEcouteSupport* pEcoutes = m_FicProba->GetEcoutesQH(IdxStation, JdxJrNomme, KdxQuartHeure);

						// on boucle sur toutes les écoutes
						for (int JdxEcoute = 0; JdxEcoute < pEcoutes->m_NbEcoutes; JdxEcoute += 1)
						{
							// on récupère le numéro de l'individu et la durée d'écoute
							int NoIndividu = pEcoutes->m_pNoIndividus[JdxEcoute];
							int ValEcoute  = pEcoutes->m_pValEcoutes [JdxEcoute];

							// on teste le poids de l'individu
							if (m_CoeffEx[NoIndividu] > 0.0)
							{
								// on incrémente les audiences de la semaines
								pPTRSemaines[pAffectations[IdxSpot]][NoIndividu] |= Mask;
							}
						}

						// on permute le masque
						if (Mask == 0x01) Mask = 0x02; else Mask = 0x01;

						// on incrémente le quart d'heure
						KdxQuartHeure += 1;
					}
				}
			}

			/////////////////////////////////////////////////////////////////////////////////////////////

			// on initialise les paramètres du modèle
			int     N  = NbActives;
			double  E  = 0.0;
			double  M  = 0.0;
			double  W  = 0.0;
			double* SW = new double[N]; for (int Sdx = 0; Sdx < N; Sdx += 1) SW[Sdx] = 0.0;

			// on boucle sur tous les individus
			for (int IdxIndividu = 0; IdxIndividu < m_NbIndiv; IdxIndividu += 1)
			{
				// on récupère le poids de l'individu
				double Poids = m_CoeffEx[IdxIndividu];

				// on teste le poids de l'individu
				if (Poids > 0.0)
				{
					// on incrémente l'effectif
					E += Poids;

					// on calcule le demi-poids de l'individu
					double DemiPoids = Poids / 2.0;

					/////////////////////////////////////////////////////////////////////////////////////////////

					// on incrémente les audiences
					if ((pPTREcoutes[0][IdxIndividu] & 0x01) > 0) M += DemiPoids;
					if ((pPTREcoutes[0][IdxIndividu] & 0x02) > 0) M += DemiPoids;
					if ((pPTREcoutes[1][IdxIndividu] & 0x01) > 0) M += DemiPoids;
					if ((pPTREcoutes[1][IdxIndividu] & 0x02) > 0) M += DemiPoids;
					if ((pPTREcoutes[2][IdxIndividu] & 0x01) > 0) M += DemiPoids;
					if ((pPTREcoutes[2][IdxIndividu] & 0x02) > 0) M += DemiPoids;
					if ((pPTREcoutes[3][IdxIndividu] & 0x01) > 0) M += DemiPoids;
					if ((pPTREcoutes[3][IdxIndividu] & 0x02) > 0) M += DemiPoids;
					if ((pPTREcoutes[4][IdxIndividu] & 0x01) > 0) M += DemiPoids;
					if ((pPTREcoutes[4][IdxIndividu] & 0x02) > 0) M += DemiPoids;
					if ((pPTREcoutes[5][IdxIndividu] & 0x01) > 0) M += DemiPoids;
					if ((pPTREcoutes[5][IdxIndividu] & 0x02) > 0) M += DemiPoids;
					if ((pPTREcoutes[6][IdxIndividu] & 0x01) > 0) M += DemiPoids;
					if ((pPTREcoutes[6][IdxIndividu] & 0x02) > 0) M += DemiPoids;

					/////////////////////////////////////////////////////////////////////////////////////////////

					// on initialise le masque des écoutes
					unsigned char MaskEcoute = 0x00;

					// on construit le masque des écoutes
					MaskEcoute |= pPTREcoutes[0][IdxIndividu];
					MaskEcoute |= pPTREcoutes[1][IdxIndividu];
					MaskEcoute |= pPTREcoutes[2][IdxIndividu];
					MaskEcoute |= pPTREcoutes[3][IdxIndividu];
					MaskEcoute |= pPTREcoutes[4][IdxIndividu];
					MaskEcoute |= pPTREcoutes[5][IdxIndividu];
					MaskEcoute |= pPTREcoutes[6][IdxIndividu];

					// on incrémente l'audience
					if ((MaskEcoute & 0x01) > 0) W += DemiPoids;
					if ((MaskEcoute & 0x02) > 0) W += DemiPoids;

					// on boucle sur toutes les semaines
					for (int IdxSemaine = 0; IdxSemaine < N; IdxSemaine += 1)
					{
						// on incrémente les audiences de la semaine
						if ((pPTRSemaines[IdxSemaine][IdxIndividu] & 0x01) > 0) SW[IdxSemaine] += DemiPoids;
						if ((pPTRSemaines[IdxSemaine][IdxIndividu] & 0x02) > 0) SW[IdxSemaine] += DemiPoids;
					}
				}
			}

			// on teste l'effectif de la cible
			if (E > 0.0)
			{
				// on calcule toutes les proportions des audiences
				M /= E;
				W /= E;

				// on calcule toutes les proportions des audiences de semaines
				for (int Qdx = 0; Qdx < N; Qdx += 1) SW[Qdx] /= E;

				// on calcule la couverture
				double Couv = this->CalculerCouverture(N, M, W, SW);

				// on calcule la distribution
				double* pDistribution = this->CalculerDistribution(NbSpots_Plan, Couv, GRP_Plan / ((double)NbSpots_Plan * 100.0));

				// on alloue la distribution de contacts
				m_Distribution.CreateNewDistribution(m_Param.m_NbClassesResultatDistribution + 1);
				m_Distribution.Initialise();

				// on boucle sur toutes les classes
				for (int c = 0; c <= NbSpots_Plan; c += 1)
				{
					// on initialise la classe de contacts
					if (c <= m_Param.m_NbClassesResultatDistribution)
						m_Distribution.SetValue(c, pDistribution[c] * 100.0);
					else
						m_Distribution.AddValue(m_Param.m_NbClassesResultatDistribution, pDistribution[c] * 100.0);
				}

				// on initialise la couverture et le GRP
				m_Couverture = Couv * 100.0;
				m_GRP        = GRP_Plan;

				// on libère la distribution
				delete [] pDistribution;

				// on positionne l'indicateur
				IsAbaqueValid = true;
			}

			/////////////////////////////////////////////////////////////////////////////////////////////

			// on libère les audiences
			delete [] SW;

			// on libère les audiences par semaines
			for (int Zdx = 0; Zdx < NbActives; Zdx += 1) delete [] pPTRSemaines[Zdx];
			delete [] pPTRSemaines;

			// on libère le patron d'écoutes
			delete [] pPTREcoutes[0]; delete [] pPTREcoutes[1];
			delete [] pPTREcoutes[2]; delete [] pPTREcoutes[3];
			delete [] pPTREcoutes[4]; delete [] pPTREcoutes[5];
			delete [] pPTREcoutes[6];
		}

		// on libère le map des stations
		delete [] pMAPStations;

		// on libère les affectations
		delete [] pAffectations;
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

#endif

double CTomyCIMRusse::GetAsymptote()
{
   return (0.0);
}

#ifdef OPTION_DUREEECOUTE
void CTomyCIMRusse::AddSpot(short Station, int JourDate, short JourNomme, short QHeure, double GRP, short NbInsertion)
{
   ASSERT(GRP >= 0);

   CElement E;

   E.m_NoStationJFC = Station;
   E.m_JourDate = JourDate;
   E.m_JourNomme = JourNomme;
   E.m_QHeure = (QHeure % 10000);
   E.m_OptionAncrage = (QHeure / 10000) > 0;
   E.m_GRP = GRP;
   E.m_NbInsertion = NbInsertion;

   m_Plan.Add(E);
}

void CTomyCIMRusse::AddEvenement(short Station, int JourDate, short JourNomme, short HoraireDebut, short Duree, double GRP)
{
}

void CTomyCIMRusse::UpdateSpot(short Station, int JourDate, short JourNomme, short QHeure, double GRP, short NbInsertion)
{
   ASSERT(GRP >= 0);

   CElement E;

   E.m_NoStationJFC = Station;
   E.m_JourDate = JourDate;
   E.m_JourNomme = JourNomme;
   E.m_QHeure = (QHeure % 10000);
   E.m_OptionAncrage = (QHeure / 10000) > 0;
   E.m_GRP = GRP;
   E.m_NbInsertion = NbInsertion;

   if (NbInsertion > 0) {
      for (int e = 0; e < m_Plan.GetSize(); e += 1) {
         if (m_Plan[e].IsEquivalent(&E)) {
            m_Plan[e].m_NbInsertion += NbInsertion;
            return;
         }
      }
      m_Plan.Add(E);
      return;
   }
   else {
      int nbtosuppress = -NbInsertion;
      for (int e = 0; e < m_Plan.GetSize(); e += 1) {
         if (m_Plan[e].IsEquivalent(&E)) {
            int NbEnMoins = nbtosuppress;
            if (NbEnMoins > m_Plan[e].m_NbInsertion)
               NbEnMoins = m_Plan[e].m_NbInsertion;
            nbtosuppress -= NbEnMoins;
            m_Plan[e].m_NbInsertion -= NbEnMoins;
            if (m_Plan[e].m_NbInsertion == 0) {
               m_Plan.RemoveAt(e);
            }
            if (nbtosuppress == 0)
               return;
         }
      }
   }
   AfxMessageBox("Erreur dans le calcul marginal: un spot à supprimer n'est pas retrouvé dans la liste des spots du calcul en cours!");
}
#else
void CTomyCIMRusse::AddSpot(short Station, short JourNomme, short QHeure, double GRP, short NbInsertion)
{
}

void CTomyCIMRusse::UpdateSpot(short Station, short JourNomme, short QHeure, double GRP, short NbInsertion)
{
}
#endif

int CTomyCIMRusse::GetNbSpot()
{
   return (m_Plan.GetSize());
}

long CTomyCIMRusse::GetNbInsertions()
{
   return (m_NbInsertions);
}

double CTomyCIMRusse::GetCouverture()
{
   return (m_Couverture);
}

double CTomyCIMRusse::GetCouvertureValideBeta()
{
   return (0.0);
}

double CTomyCIMRusse::GetCouvertureValideMinMax()
{
   return (0.0);
}

double CTomyCIMRusse::GetGRP()
{
   return (m_GRP);
}

int CTomyCIMRusse::GetNbClasseResultat()
{
   return (m_Param.m_NbClassesResultatDistribution);
}

double CTomyCIMRusse::GetExactementTouche(int categorie)
{
   return (m_Distribution.GetValue(categorie));
}

double CTomyCIMRusse::GetCourbeReponse(int nbfoistouche)
{
   return (100.0);
}

void CTomyCIMRusse::ActiveFiltreIndividusStrateConsomation(int NrStrate)
{
   ASSERT(FALSE);
}

double CTomyCIMRusse::LanceEval()
{
   return (LanceEvalDistrib());
}

void CTomyCIMRusse::SavePlanBeforeZoom()
{
   if (m_PlanWhileZoom == 0)
      m_PlanWhileZoom = new CTabElement;

   m_PlanWhileZoom->Copy(m_Plan);
}

void CTomyCIMRusse::RestorePlanAfterZoom()
{
   if (m_PlanWhileZoom != 0)
      m_Plan.Copy(*m_PlanWhileZoom);

   delete m_PlanWhileZoom;

   m_PlanWhileZoom = 0;
}

CTomyCIMRusse::~CTomyCIMRusse()
{
   delete m_PlanWhileZoom;

   this->FreeAllocIndiv();
   this->FreePanel();
}
