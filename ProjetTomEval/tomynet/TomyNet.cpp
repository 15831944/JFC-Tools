// TomyNet.cpp: implementation of the CTomyNet class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "TomyNet.h"
#include "math.h"

#ifdef MODE_BANDEAUX
#define NR_CIBLE_ENSEMBLE 0

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CTomyNet::CTomyNet()
{
	m_Distrib=NULL;
	m_NrInitialisation=0;
	m_Distrib=NULL;
	m_CibleEnsemble=NULL;
	m_Cible=NULL;
}

CTomyNet::~CTomyNet()
{
	m_Cible->UnloadCible(); 

	// Déchargement cible ensemble
	m_CibleEnsemble->UnloadCible();
	
	// Déchargement Panel Internet
	m_Panel.UnloadPanel(); 

	// Destruction distribution
	if(m_Distrib!=NULL)
	{
		for(int idxindiv=0;idxindiv<m_Panel.m_NbIndiv;idxindiv++)
		{
			delete[] m_Distrib[idxindiv];
		}
		delete m_Distrib;
	}

	// Destruction plan
	m_Plan.Initialise();

	// Destruction objets cible
	delete m_Cible;
	delete m_CibleEnsemble;
}

bool CTomyNet::InitialisePanel(LPCSTR CodePanel)
{
	if(m_NrInitialisation<1)
	{
		AfxMessageBox("Erreur d'utilisation de CTomyNet::InitialisePanel");
		return 0; //NOK
	}

	// Chargement du panel
	m_Panel.LoadPanel(m_CheminData,CodePanel);

	m_NrInitialisation=2;
	return 1; //OK
}

bool CTomyNet::InitialiseParametre(LPCSTR CheminData,int NbClasseMax)
{
	m_NrInitialisation=1;
	if(m_CheminData!="")
	{
		// Si le chemin des données change, il faut effacer tous les objets en mémoire
		// Déchargement cible
		m_Cible->UnloadCible(); 

		// Déchargement cible ensemble
		m_CibleEnsemble->UnloadCible();

		// Destruction distribution
		for(int idxindiv=0;idxindiv<m_Panel.m_NbIndiv;idxindiv++)
		{
			delete[] m_Distrib[idxindiv];
		}
		delete[] m_Distrib;
	
		// Déchargement Panel Internet
		m_Panel.UnloadPanel(); 

		// Destruction plan
		m_Plan.Initialise(); 
	}
	m_CheminData=CheminData;
	m_NbClasseMax=NbClasseMax;

	return 1;//OK
}

bool CTomyNet::InitialiseCible(int NumeroCible)
{
	if(m_NrInitialisation<2)
	{
		AfxMessageBox("Erreur d'utilisation de CTomyNet::InitialiseCible");
		return 0; //NOK
	}

	if(m_Cible==NULL)m_Cible = new CCible(m_Panel.m_NbIndiv);

	// Chargement de la cible
	if (!m_Cible->LoadCible(m_CheminData,m_Panel.m_CodePanel,NumeroCible))
	{
		m_Cible->UnloadCible();
		return 0; // NOK
	}

	m_NrInitialisation=3;
	return 1;// OK
}

bool CTomyNet::InitialiseCibleEnsemble()
{
	if(m_NrInitialisation<2)
	{
		AfxMessageBox("Erreur d'utilisation de CTomyNet::InitialiseCible");
		return 0; //NOK
	}
	if(m_CibleEnsemble==NULL)m_CibleEnsemble = new CCible(m_Panel.m_NbIndiv);

	// Chargement de la cible
	if (!m_CibleEnsemble->LoadCible(m_CheminData,m_Panel.m_CodePanel,NR_CIBLE_ENSEMBLE))
	{
		m_CibleEnsemble->UnloadCible();
		return 0; // NOK
	}

	return 1;// OK
}

bool CTomyNet::InitialiseNombreSemaine(int NbSemaine)
{
	m_Plan.m_NbSemaine=NbSemaine;
	return 1;// OK
}

bool CTomyNet::InitialiseBandeaux()
{
	if(m_NrInitialisation<3)
	{
		AfxMessageBox("Erreur d'utilisation de CTomyNet::InitialiseBandeaux");
		return 0; // NOK
	}
	m_Plan.Initialise();
	m_NrInitialisation=4;
	return 1;// OK
}

// Attention, ce sont des numéros de sites utilisés dans le panel
// On doit passer en paramètre un numéro de site valide

// NOTE: Le jour est l'index dans cette fonction (0 à 6)
bool CTomyNet::AjouteBandeauxIndexJour(int NrSite, int Jour, int NombreAchete)
{
	ASSERT(Jour>=0 && Jour<7);
	// TODO: Bricolage fait en URGENCE; à refaire dès que JFC donne du temps....(jamais)
	int TabJour[]={1,2,4,8,16,32,64};
	return AjouteBandeauxMasqueJour( NrSite, TabJour[Jour],  NombreAchete);
}


// NOTE: Le jour est le masque (1,2,4,8,16,32 ou 64)
bool CTomyNet::AjouteBandeauxMasqueJour(int NrSite, int Jour, int NombreAchete)
{
	ASSERT(Jour==1 || Jour==2 || Jour==4 || Jour==8 || Jour==16 || Jour==32 || Jour==64 );
	if(m_NrInitialisation<3)
	{
		AfxMessageBox("Erreur d'utilisation de CTomyNet::AjouteBandeaux");
		return 0; //NOK
	}

	CSupportBandeaux Bdx;
	Bdx.m_NrSite=NrSite;

	Bdx.m_Jour=Jour;
	Bdx.m_NbAchete=NombreAchete;

	Bdx.m_SupportProba=m_Panel.RechercheSupportProba(Bdx);

	if (Bdx.m_SupportProba==NULL)
	{
		// Support non existant dans vague
		CString message;
		message.Format("Le support internet NrSite:%d BitJour:%d n'est pas trouvé dans le panel:%s",NrSite,Jour,m_Panel.m_CodePanel);
		AfxMessageBox(message); 
		return 0; // NOK
	}

	m_Plan.AddBandeaux(Bdx);

	m_NrInitialisation=4;
	return 1;// OK
}

double CTomyNet::CalculDebitCible()
{
	double Debit=0;
	for(int s=0;s<m_Plan.GetNbSupport();s++)
	{
		for (int i=0;i<m_Plan.m_ListeSupport[s].m_SupportProba->GetNbIndivTouche();i++)
		{
			int nr=m_Plan.m_ListeSupport[s].m_SupportProba->GetNrIndivTouche(i);
			double poids=m_Cible->m_CoeffEx[nr];
			Debit+=m_Plan.m_ListeSupport[s].m_SupportProba->GetNbContactProbabilise(i)*poids;
		}
	}
	return Debit;
}

// Donne debit cible du site et du jour
double CTomyNet::CalculDebitCible(int nrsite, int idxjour)
{
	//double Debit;

	InitialiseBandeaux();

	AjouteBandeauxIndexJour(nrsite,idxjour,0);

	return CalculDebitCible();
}
// Donne debit cible du site
double CTomyNet::CalculDebitCible(int nrsite)
{
	// Il faut initialiser le plan et ajouter tous les jour du site passé en paramètre
	// puis lancer le calcul :
	InitialiseBandeaux();

	// On cherche dans le panel quels sont les supports du site
	int nbsupport;
	nbsupport = m_Panel.m_SupportProba.GetSize();

	for(int idxsupport=0;idxsupport<nbsupport;idxsupport++)
	{
		CPanelInternet::CSupportProba * support = &m_Panel.m_SupportProba[idxsupport];
		if(support->m_NrSite==nrsite)
		{
			// Récupération du support bandeaux
			// Le calcul du débit ciblé ne tient pas compte du nombre de pages achetées.
			AjouteBandeauxMasqueJour(nrsite,support->m_Jour,0);
		}
		
	}
	return CalculDebitCible();
}

// Donne le débit maximum: nb de pages max que l'on peut acheter pour un site et un jour donné
// Utilise la cible ensemble
double CTomyNet::CalculDebitMax(int nrsite,int jour)
{
	double Debit=0;

	InitialiseBandeaux();
	
	// On ajoute un seul bandeaux au plan
	if(!AjouteBandeauxIndexJour(nrsite,jour,0))return(0);

	// Boucle sur les supports
	for(int s=0;s<m_Plan.GetNbSupport();s++)
	{
		// Pour chaque individu du support
		for (int i=0;i<m_Plan.m_ListeSupport[s].m_SupportProba->GetNbIndivTouche();i++)
		{
			double poids=m_CibleEnsemble->m_CoeffEx[m_Plan.m_ListeSupport[s].m_SupportProba->GetNrIndivTouche(i)];
			Debit+=m_Plan.m_ListeSupport[s].m_SupportProba->GetNbContactProbabilise(i)*poids;
		}
	}
	return Debit;
}

// Donne le GRP cible du site
double CTomyNet::CalculGRPSupport(int nrsite)
{
	return 100.0*CalculDebitCible(nrsite)/m_Cible->m_Population;
}

// Donne le GRP cible par support 
double CTomyNet::CalculGRPSupport(int nrsite, int idxjour)
{
	return 100.0*CalculDebitCible(nrsite,idxjour)/m_Cible->m_Population;
}

// Donne le GRP max pour un Bandeau sur un jour
double CTomyNet::CalculGRPMax(int nrsite, int idxjour)
{

	return 100*CalculDebitMax(nrsite,idxjour)/m_CibleEnsemble->m_Population;
}

double CTomyNet::CalculPdV(int NrSupport)
{
	double NbBdx=0;
	for (int i=0;i<m_Plan.m_ListeSupport[NrSupport].m_SupportProba->GetNbIndivTouche();i++)
	{
		double poids=m_CibleEnsemble->m_CoeffEx[m_Plan.m_ListeSupport[NrSupport].m_SupportProba->GetNrIndivTouche(i)];
		NbBdx+=m_Plan.m_ListeSupport[NrSupport].m_SupportProba->GetNbContactProbabilise(i)*poids;
	}


	double PdV=m_Plan.m_ListeSupport[NrSupport].m_NbAchete/NbBdx;
	if(PdV>1.0)
	{
		PdV=1.0;
		TRACE("Surutilisation du support\n");
	}
	ASSERT(PdV>=0);
	return PdV;
}


double CTomyNet::CalculNbBandeauxCible()
{
	double NbBdx=0;
	for(int s=0;s<m_Plan.GetNbSupport();s++)
	{
		double PdV=CalculPdV(s);

		for (int i=0;i<m_Plan.m_ListeSupport[s].m_SupportProba->GetNbIndivTouche();i++)
		{
			double poids=m_Cible->m_CoeffEx[m_Plan.m_ListeSupport[s].m_SupportProba->GetNrIndivTouche(i)];
			NbBdx+=m_Plan.m_ListeSupport[s].m_SupportProba->GetNbContactProbabilise(i)*poids*PdV;
		}
	}
	
	return NbBdx*=m_Plan.m_NbSemaine;
}

double CTomyNet::CalculCouverture(bool flag_Caping)
{
	// TODO: prise en compte du caping...

	// Allocation
	double *Q = new double[m_Panel.m_NbIndiv];

	// Initialisation
	for(int i=0;i<m_Panel.m_NbIndiv;i++)
	{
		Q[i]=1.00;
	}

	
	// Calcul du vecteur Q[i]
	for(int s=0;s<m_Plan.GetNbSupport();s++)
	{
		double PdV=CalculPdV(s);

		for (int i=0;i<m_Plan.m_ListeSupport[s].m_SupportProba->GetNbIndivTouche();i++)
		{
			int NrIndiv=m_Plan.m_ListeSupport[s].m_SupportProba->GetNrIndivTouche(i);
			Q[NrIndiv]*=m_Plan.m_ListeSupport[s].m_SupportProba->GetProbabiliteTouche(i)*PdV;
		}
	}

	// Intégration 
	double couv=0;
	for(i=0;i<m_Cible->m_NbIndividuPanel;i++)
	{
		double poids=m_Cible->m_CoeffEx[i];
		if(poids==0)continue;
		couv+=(1-pow(Q[i],m_Plan.m_NbSemaine))*poids;
	}

	
	// Libération
	delete [] Q;
	
	return couv;
}

double CTomyNet::CalculGRPPlan()
{
	return CalculNbBandeauxCible()*100/m_Cible->m_Population;
}

double ** CTomyNet::CalculDistributionIndividuelle(bool flag_Caping)
{
	// Le vecteur de distribution de chaque individu est alloué quand
	// -la cible a été chargé
	// -Panel Internet chargé
	// -Plan Initialisé et bandeaux ajoutés
	if(m_NrInitialisation!=4)
	{
		AfxMessageBox("Mauvaise initialisation de Tomynet avant utilisation de CalculDistributionIndividuelle");
		return NULL;
	}
	
	if(m_Distrib==NULL)
	{
		m_Distrib = new double *[m_Panel.m_NbIndiv];
		for(int indiv=0;indiv<m_Panel.m_NbIndiv;indiv++)
		{
			m_Distrib[indiv] = new double[m_NbClasseMax+1];
		}
	}
	// Initialisation de la distrib avec 1 dans la classe 0 pour tous les individus, sinon 0 pour les classes 1 à n
	for(int indiv=0;indiv<m_Panel.m_NbIndiv;indiv++)
	{
		for(int classe=0;classe<=m_NbClasseMax;classe++)
		{
			if(!classe)m_Distrib[indiv][classe]=1.0;
			else m_Distrib[indiv][classe]=0.0;
		}
	}
	

	for(int s=0;s<m_Plan.GetNbSupport();s++)
	{
		double PdV=CalculPdV(s);
//		TRACE("Part de voie =%4.1f%%\n",PdV*100);

		for (int i=0;i<m_Plan.m_ListeSupport[s].m_SupportProba->GetNbIndivTouche();i++)
		{
			int NrIndiv=m_Plan.m_ListeSupport[s].m_SupportProba->GetNrIndivTouche(i);
			

			double * VB;
			if(!flag_Caping) // SANS CAPING
			{
				VB = ConstruitVecteurB(m_Plan.m_ListeSupport[s].m_SupportProba->GetProbabiliteTouche(i), m_Plan.m_ListeSupport[s].m_SupportProba->GetNbFoisToucheMoyen(i), PdV);
			}
			else // AVEC CAPING
			{
				VB = ConstruitVecteurBCaping(m_Plan.m_ListeSupport[s].m_SupportProba->GetProbabiliteTouche(i), m_Plan.m_ListeSupport[s].m_SupportProba->GetNbFoisToucheMoyen(i), PdV);
			}
			
			// on calcul le produit binomial autant de fois qu'il y a de semaines
			for(int w=0;w<m_Plan.m_NbSemaine;w++)
			{
				CalculProduitVecteurB(m_Distrib[NrIndiv],VB);
			}

			delete VB;
		}
	}


#ifdef _DEBUG
	{
		double couverture=0;
		for(int indiv=0;indiv<m_Panel.m_NbIndiv;indiv++)
		{
			double sigma=0;
			for(int classe=0;classe<=m_NbClasseMax;classe++)
			{
				sigma+=m_Distrib[indiv][classe];
			}
			ASSERT(sigma>0.999999999999);
			ASSERT(sigma<1.000000000001);
			couverture+=(1-m_Distrib[indiv][0])*m_Cible->m_CoeffEx[indiv];
		}
		TRACE("Couverture ============> %.0f individus %4.2f%%\n",couverture,100*couverture/m_Cible->m_Population);
	}

#endif

	return m_Distrib;

}

void CTomyNet::CalculProduitVecteurB(double *couverture, double *VBf)
{
	double * VBtempo = new double[m_NbClasseMax+1];

	// Copie de couverture dans VBtempo
	for(int classe=0;classe<=m_NbClasseMax;classe++)
	{
		VBtempo[classe] = couverture[classe];
		couverture[classe]=0;
	}

	// On boucle sur toutes les cases
	for(int x=0;x<=m_NbClasseMax;x++)
	{
		for (int y=0;y<=m_NbClasseMax;y++)
		{
	        int z=x+y;

			if (z> m_NbClasseMax) z= m_NbClasseMax;
			couverture[z] +=VBtempo[x]*VBf[y];
		}
	}
	delete VBtempo;

}



// POUR TEST....
// construction d'un vecteur avec CAPING
double * CTomyNet::ConstruitVecteurBCaping(double probatouche, double nbcontactmoyen, double PdV)
{
	double * VB;

	// Initalisation vecteur
	VB = new double[m_NbClasseMax+1];
	for(int classe=0;classe<=m_NbClasseMax;classe++)
	{
		VB[classe]=0.0;
	}

	// Calcul des 2 classes qui vont être utilisée pour
	// ranger les probabilitées

	nbcontactmoyen*=PdV;

	classe = (int)(nbcontactmoyen);
	double Coeff;
	if(classe>=m_NbClasseMax)
	{
		// On limite au nombre de classe
		classe=m_NbClasseMax;

		// Et pas besoin de distribuer des contacts au dessus
		// car le vecteur binomial est limité en taille
		Coeff=1;
	}
	else
	{
		Coeff= 1-(nbcontactmoyen - classe);
		if(Coeff>1)
		{
			// Petit test pour éviter plantage
			Coeff=1;
			ASSERT(0);
		}
	}


	if(classe<m_NbClasseMax)
		VB[classe+1]= probatouche * (1-Coeff);
	if(classe>0)
	{
		VB[classe]= probatouche * Coeff;
		VB[0] = 1-(probatouche);
	}
	else
	{
		VB[0] = 1-(probatouche * (1-Coeff));
	}
	
	return VB; // !!! Le pointeur est à liberer plus tard...
}



double * CTomyNet::ConstruitVecteurB(double probatouche, double nbcontactmoyen, double PdV)
{
	double * VB;

	// Initalisation vecteur
	VB = new double[m_NbClasseMax+1];
	for(int classe=0;classe<=m_NbClasseMax;classe++)
	{
		VB[classe]=0.0;
	}

	// Calcul des 2 classes qui vont être utilisée pour
	// ranger les probabilitées
	classe = (int)(nbcontactmoyen);
	double Coeff;
	if(classe>=m_NbClasseMax)
	{
		// On limite au nombre de classe
		classe=m_NbClasseMax;

		// Et pas besoin de distribuer des contacts au dessus
		// car le vecteur binomial est limité en taille
		Coeff=1.0;
	}
	else
	{
		Coeff= 1-(nbcontactmoyen - classe);
		if(Coeff>1)
		{
			// Petit test pour éviter plantage
			Coeff=1;
			ASSERT(0);
		}
	}


	VB[classe]= probatouche * Coeff * PdV;
	if(classe<m_NbClasseMax)
		VB[classe+1]= probatouche * (1-Coeff) * PdV;
	VB[0] = 1-(probatouche * PdV);
	return VB;
}

// Permet de définir les GRP de la semaine type internet
void CTomyNet::InitGRP(CSemaineTypeInternet *semtype)
{
	// Boucle sur les supports
	for(int idxsite=0;idxsite<semtype->m_NbStation;idxsite++)
	{
		for(int idxjour=0;idxjour<7;idxjour++)
		{
			// Calcul du GRP
			// Le GRP est divisé par le nb de semaine du plan 
			semtype->m_GRP[idxsite][idxjour][m_Cible->m_NrCible] = CalculGRPSupport(semtype->m_NrStation[idxsite],idxjour);
			semtype->m_DebitMax[idxsite][idxjour] = CalculDebitMax(semtype->m_NrStation[idxsite],idxjour);
		}
	}
}
#endif

