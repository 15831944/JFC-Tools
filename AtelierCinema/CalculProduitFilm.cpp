// CalculProduitFilm.cpp: implementation of the CCalculProduitFilm class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "cinema.h"
#include "CalculProduitFilm.h"
#include "CampagneFilm.h"
#include "CalculMemoDememo.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

extern CCinemaApp theApp;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////



CCalculProduitFilm::CCalculProduitFilm(CCampagneFilm * cmp/* pointeur sur la campagne*/)
{
	// D�finition pointeur de campagne
	m_pCampagne = cmp;

	// r�cup�ration de la table des films
	CTableProduitFilm * tabprod =  &m_pCampagne->m_TabProduitFilm;
	int nbfilm = tabprod->GetSize();

	// Initialisation resultat
	m_ResultatNbEntree.m_NbEntreeFilm  = new double[nbfilm+1];
	m_ResultatNbEntree.m_NbEntreeCible = new double*[nbfilm+1];
	m_ResultatNbEntree.m_Tarif  = new double[nbfilm+1];
	m_NbTickets=new double*[nbfilm+1];

	for(int idxfilm=0;idxfilm<=tabprod->GetSize() ;idxfilm++)
	{
		// Initialisation resultat
		m_ResultatNbEntree.m_NbEntreeCible[idxfilm] = new double[m_pCampagne->m_NbSemaine+1];
		m_NbTickets[idxfilm]=new double[m_pCampagne->m_NbSemaine+1];
		for(int idxsem=0;idxsem<=m_pCampagne->m_NbSemaine;idxsem++)
		{
			m_ResultatNbEntree.m_NbEntreeCible[idxfilm][idxsem]=0;
			m_NbTickets[idxfilm][idxsem]=0;
		}
		m_ResultatNbEntree.m_Tarif[idxfilm]=0;
		m_ResultatNbEntree.m_NbEntreeFilm[idxfilm]=0;
	}

	// Initialisation couverture
	m_CouvertureSemaine = new double*[m_pCampagne->m_NbSemaine];
	m_CouvertureCumul = new double*[m_pCampagne->m_NbSemaine];

	for(int idxsem=0;idxsem<m_pCampagne->m_NbSemaine;idxsem++)
	{
		m_CouvertureSemaine[idxsem] = new double[NBCLASSE+1];
		m_CouvertureCumul[idxsem] = new double[NBCLASSE+1];
	}



}

CCalculProduitFilm::~CCalculProduitFilm()
{
	
	// r�cup�ration de la table des films
	CTableProduitFilm * tabprod =  &m_pCampagne->m_TabProduitFilm;

	// destruction r�sultats
	for(int idxfilm=0;idxfilm<tabprod->GetSize()+1;idxfilm++)
	{
		delete[] m_ResultatNbEntree.m_NbEntreeCible[idxfilm];
		delete [] m_NbTickets[idxfilm];
	}
	delete [] m_NbTickets;
	delete[] m_ResultatNbEntree.m_NbEntreeCible;
	delete[] m_ResultatNbEntree.m_NbEntreeFilm;

	delete[] m_ResultatNbEntree.m_Tarif;

	// destruction matrice de couverture
	for(int idxsem=0;idxsem<m_pCampagne->m_NbSemaine;idxsem++)
	{
		delete[] m_CouvertureSemaine[idxsem];
		delete[] m_CouvertureCumul[idxsem];
	}
	delete[] m_CouvertureSemaine;
	delete[] m_CouvertureCumul;

	
}
// Calcul du nombre d'entr�e total + tarif total de la campagne
CCalculProduitFilm::CResultatNbEntree *  CCalculProduitFilm::CalculNbEntre()
{

	// r�cup�ration de la table des films
	CTableProduitFilm * tabprod =  &m_pCampagne->m_TabProduitFilm;

	// Calcul du rapport F(c)/F(ensemble) pour le calcul du NbEntr�e s=1
	// F(c) = Somme(proba(i)*poids(i)) pour les individus i appartenant aux bacs probabilis�s correspondant � la s�lection
	// Grandeur d�finie par semaine (s=1)
	// Bouclage sur les individus de la cible

	// Bouclage par film (calcul du nombre d'entr�e par film, par semaine)
	for(int idxfilm=0;idxfilm<tabprod->GetSize() ;idxfilm++)
	{
		// Film actif
		if(tabprod->GetAt(idxfilm).m_Select)
		{

			// Le Produit
			CProduitFilm filmprod = tabprod->GetAt(idxfilm);

			// Tarif
			m_ResultatNbEntree.m_Tarif[idxfilm] = filmprod.m_Tarif;
			// total tarif films
			m_ResultatNbEntree.m_Tarif[tabprod->GetSize()] += filmprod.m_Tarif;


			// Liste des bacs du produit du film
			CByteArray listeBacActif;
	
			ConstruitListeBacsActifs(idxfilm,&listeBacActif);

			// fr�quentation cible
		/*	double Fc = CalculFrequentationCible(&listeBacActif);
			// fr�quentation cible ensemble
			double FEns = CalculFrequentationTotale(&listeBacActif);

  /*
			C'�tait pour calculer la fr�quentation de la sous cible
			FSC=Fc/FEns

  La fonction ci-dessous permet de tenir compte, d�s le calcul du nombre de tickets,
  de la sur fr�quentation, ce qui revient � calculer le nombre d'entr�es cibl�es
  sur chacune des sous cibles

			*/

			// Tableau contenant les proportions de sur fr�quentation seconde 
			// pour chaque sous cible
			CArray<double,double&>SFseconde;
			CalculSurFrequentationParSegmentCible(&filmprod,&SFseconde,&listeBacActif);	

			for (int i = 0;i<SFseconde.GetSize(); i++)
			{
				double coeff = SFseconde[i];
				coeff = coeff;
			}

			double NbEntreeCible,NbEntreeFilm;
			// bool
			// Bouclage par semaine
			for(int idxsem=0;idxsem<filmprod.m_NbSemaine;idxsem++)
			{
				// cas particulier : 1 ere semaine
				if(idxsem==0)
				{
					//NbEntreeCible=(double)((filmprod.m_EntreeCNC)*(Fc/FEns));
					
					NbEntreeFilm=(double)((filmprod.m_EntreeCNC));
					if( NbEntreeFilm>CalculFrequentationTotale(&listeBacActif))
					{
						NbEntreeFilm=CalculFrequentationTotale(&listeBacActif);
					}

					// bouclage sur les modalit�s
					NbEntreeCible=0;
					int NbModa = theApp.m_Cible.m_NbClasse[filmprod.m_CodeCritCoeff];
					for(int idxmod=0;idxmod<theApp.m_Cible.m_NbClasse[filmprod.m_CodeCritCoeff];idxmod++)
					{
						double NbEntreCibleSegment=(double)((filmprod.m_EntreeCNC)*SFseconde[idxmod]);
						// On s'assure que le nombre de tickets ne d�passe pas la population de la sous cible
						double popSousCible=(double)CalculPopulationSousCibleGeo(&listeBacActif,idxmod);	
						if(NbEntreCibleSegment>popSousCible)NbEntreCibleSegment=popSousCible;
						NbEntreeCible+=NbEntreCibleSegment;
					}



				}
				// sinon, prise en compte du pourcentage d'usure 
				// et de la semaine pr�c�dente
				else
				{

					double Tcu = filmprod.m_TabCoeffUsure[idxsem];
					NbEntreeCible *= 1-(filmprod.m_TabCoeffUsure[idxsem]/100.0);
					NbEntreeFilm *= 1-(filmprod.m_TabCoeffUsure[idxsem]/100.0);
				}

				int idxsemainecampagne = (tabprod->GetAt(idxfilm).m_Debut-m_pCampagne->m_DateDebut)/7+idxsem;

				// controle de la semaine du produit
				if((idxsemainecampagne<0)||(idxsemainecampagne>=m_pCampagne->m_NbSemaine))continue;


				// Entr�es cibl�es 
				// sur le film
				m_ResultatNbEntree.m_NbEntreeCible[idxfilm][idxsemainecampagne] = NbEntreeCible;

				// total film
				m_ResultatNbEntree.m_NbEntreeCible[idxfilm][m_pCampagne->m_NbSemaine] += NbEntreeCible;
				// total semaine
				m_ResultatNbEntree.m_NbEntreeCible[tabprod->GetSize()][idxsemainecampagne] += NbEntreeCible;
				// Total film semaine
				m_ResultatNbEntree.m_NbEntreeCible[tabprod->GetSize()][m_pCampagne->m_NbSemaine] += NbEntreeCible;

				// Tickets
				m_ResultatNbEntree.m_NbEntreeFilm[idxfilm] += NbEntreeFilm;
				m_ResultatNbEntree.m_NbEntreeFilm[tabprod->GetSize()]+= NbEntreeFilm;	
				m_NbTickets[idxfilm][idxsem]=NbEntreeFilm;
			}
		}
	}


	return &m_ResultatNbEntree;
}

void CCalculProduitFilm::CalculPerformances()
{
	CalculNbEntre();
	CalculCouvertures();
	
	CTableProduitFilm * tabprod =  &m_pCampagne->m_TabProduitFilm;

	// calcul de la m�mo-d�m�mo
	// Initialisation des param�tres pour ce calcul 
	CCalculMemoDememo::CParametre parametre;
	parametre.m_BasePopulation = m_ResultatPermormances.m_Population;
	parametre.m_DureeBaseBeta = BASEFORMATREF;
	parametre.m_DureeMaximum  = BASEFORMATMAXI;
	parametre.m_DureeMoy = new double[m_pCampagne->m_NbSemaine];
	parametre.m_NbChanceSem = new int[m_pCampagne->m_NbSemaine];
	parametre.m_NbChanceCum = new int[m_pCampagne->m_NbSemaine];
	parametre.m_NbContactBarre =new double * [m_pCampagne->m_NbSemaine];
	parametre.m_NbContactPlaque =new double * [m_pCampagne->m_NbSemaine];

	for(int sem=0;sem<m_pCampagne->m_NbSemaine;sem++)
	{
		parametre.m_NbChanceSem[sem]=0;
		for(int c=NBCLASSE;c>=0;c--)
		{
			if(m_CouvertureSemaine[sem][c]>0)
			{
				parametre.m_NbChanceSem[sem]=c;
				break;
			}
		}

		parametre.m_DureeMoy[sem]=BASEFORMATREF;
		//parametre.m_NbChanceSem[sem]= tabprod->GetSize();
		parametre.m_NbChanceCum[sem]=min(tabprod->GetSize()*(sem+1),NBCLASSE);
		parametre.m_NbContactBarre [sem]=new double  [NBCLASSE];
		parametre.m_NbContactPlaque [sem]=new double  [NBCLASSE];
		for(c=0;c<NBCLASSE;c++)
		{
			parametre.m_NbContactBarre [sem][c]=m_CouvertureSemaine[sem][c+1]*m_ResultatPermormances.m_Population;
			parametre.m_NbContactPlaque [sem][c]=m_CouvertureCumul[sem][c+1]*m_ResultatPermormances.m_Population;
		}
	}
	parametre.m_NbClasse = NBCLASSE;
	parametre.m_NbSemaine = m_pCampagne->m_NbSemaine;
	parametre.m_NbSemaineCalcul =NBSEMAINECALCULMAX;
	parametre.m_OptionCalcul_DememoPendantMemo = m_pCampagne->CalculDememo!=0;
	parametre.m_SelectCalend = new bool[NBSEMAINECALCULMAX];

	for(sem=0;sem<NBSEMAINECALCULMAX;sem++)
	{
		// S�lection des semaines actives: d�finies dans la campagne
		parametre.m_SelectCalend[sem] = m_pCampagne->SelectCalend[sem]?true:false;
	}

	// Calcul de la courbe de r�ponse dans la campagne + init du tableau m_ValBeta
	m_pCampagne->CalculCourbeReponse();
	parametre.m_ValBeta = new double[NBCLASSE];
	for(int classe=0;classe<NBCLASSE;classe++)
	{
		parametre.m_ValBeta[classe] =  m_pCampagne->ValBeta[classe];
	}
	

	CCalculMemoDememo moteur;

	// Calcul 
	m_ResultatPermormances.m_pResultatMemoDememo = moteur.CalculCourbeMemoDememo(&parametre); 

	delete [] parametre.m_DureeMoy;
	delete [] parametre.m_NbChanceSem; 
	delete [] parametre.m_NbChanceCum ;
	delete [] parametre.m_ValBeta ;
	delete[] parametre.m_SelectCalend;

	for(sem=0;sem<m_pCampagne->m_NbSemaine;sem++)
	{
		delete [] parametre.m_NbContactBarre [sem];
		delete [] parametre.m_NbContactPlaque [sem];
	}
	delete [] parametre.m_NbContactBarre;
	delete [] parametre.m_NbContactPlaque;
}


void CCalculProduitFilm::CalculCouvertures()
{
	// changer le calculpopcible en calculpopregionale

	// Initialisation du r�sultat
	// bug: recalcul du budget brut � chaque fois
	m_ResultatPermormances.m_BudgetBrut=0;
	 m_ResultatPermormances.m_BudgetNet=m_pCampagne->m_BudgetNet; 

	// Calcul de la population cible: somme des coeffs d'extrapolation
	// MODIF FRED: Changement de la population en prend compte de la base de calcul
	m_ResultatPermormances.m_Population = GetPopulationCibleBase(m_pCampagne->BaseCalcul);//CalculPopCible();
	m_ResultatPermormances.m_NbContacts=0;
	m_ResultatPermormances.m_NbIndiv=0;

	// r�cup�ration de la table des films
	CTableProduitFilm * tabprod =  &m_pCampagne->m_TabProduitFilm;


	// Matrice NbIndivSemaine(f,s,nc) et (f,s,nc)
	double ***NbIndivSemaine;
	double ***NbIndivCumul;


	// cr�ation matrice 
	NbIndivSemaine = new double**[tabprod->GetSize()];
	NbIndivCumul = new double**[tabprod->GetSize()];

	for(int idxfilm=0;idxfilm<tabprod->GetSize() ;idxfilm++)
	{

		// Budget brut
		m_ResultatPermormances.m_BudgetBrut += tabprod->GetAt(idxfilm).m_Tarif;


		NbIndivSemaine[idxfilm] = new double*[m_pCampagne->m_NbSemaine];
		NbIndivCumul[idxfilm] = new double*[m_pCampagne->m_NbSemaine];

		for(int idxsem=0;idxsem<m_pCampagne->m_NbSemaine;idxsem++)
		{
			NbIndivSemaine[idxfilm][idxsem] = new double[NBCLASSE+1];
			NbIndivCumul[idxfilm][idxsem] = new double[NBCLASSE+1];

			NbIndivSemaine[idxfilm][idxsem][0]=m_ResultatPermormances.m_Population;
			NbIndivCumul[idxfilm][idxsem][0]=m_ResultatPermormances.m_Population;

			for(int idxclasse=1;idxclasse<NBCLASSE+1;idxclasse++)
			{
				NbIndivSemaine[idxfilm][idxsem][idxclasse]=0;
				NbIndivCumul[idxfilm][idxsem][idxclasse]=0;
			}
		}
	}

	

	// Calcul du nombre d'individus touch�s par semaine et par film
	// Bouclage sur les films
	for(idxfilm=0;idxfilm<tabprod->GetSize() ;idxfilm++)
	{
		// Film inactif
		if(tabprod->GetAt(idxfilm).m_Select==0)continue;

		// Le Produit
		CProduitFilm filmprod = tabprod->GetAt(idxfilm);


		// Liste des bacs du produit
		CByteArray m_ListeBacActif;
		ConstruitListeBacsActifs(idxfilm,&m_ListeBacActif);

		// Tableau contenant les proportions de sur fr�quentation seconde 
		// pour chaque sous cible
		CArray<double,double&>SFseconde;
		//CalculSurFrequentationParSousCible(&filmprod,&SFseconde,&m_ListeBacActif);	
		CalculSurFrequentationParSegmentCible(&filmprod,&SFseconde,&m_ListeBacActif);	

		for (int i = 0;i<SFseconde.GetSize(); i++)
				double coeff = SFseconde[i];

		// Bouclage sur les semaines
		for(int idxsem=0;idxsem<m_pCampagne->m_NbSemaine;idxsem++)
		{
			// On initialise les individus d�j� touch�
			if(idxsem>=1)
			{
				for(int classe=0;classe<=NBCLASSE;classe++)
				{
					NbIndivCumul[idxfilm][idxsem][classe] = NbIndivCumul[idxfilm][idxsem-1][classe];
				}
			}

			// bouclage sur les modalit�s
			for(int idxmod=0;idxmod<theApp.m_Cible.m_NbClasse[filmprod.m_CodeCritCoeff];idxmod++)
			{
				double Nb = m_NbTickets[idxfilm][idxsem]*SFseconde[idxmod];
				if(Nb<=0)continue;

				m_ResultatPermormances.m_NbContacts+=Nb;

				// nouveaux individus
				double newIndivHit = Nb*(100-filmprod.m_TabCoeffPluriFreq[idxmod])/100;
				// individus ayant d�j� vu le film
				double oldIndivHit = Nb*filmprod.m_TabCoeffPluriFreq[idxmod]/100;

				// classe1 : le film est vu une fois
				NbIndivSemaine[idxfilm][idxsem][1] +=Nb;

				// On en enl�ve autant dans la classe inf�rieure
				NbIndivSemaine[idxfilm][idxsem][0] -=Nb;

				// en semaine idxsem: le film a �t� vu au maximum idxsem+1 fois
				// Il faut donc r�partir le nombre d'individus ayant vu d�j� le film
				// entre les classes 2 � idxsem+1.
				if(idxsem>0)
				{
					double sommtouche1fois = m_ResultatPermormances.m_Population - NbIndivCumul[idxfilm][idxsem-1][0];

					
					if(sommtouche1fois<oldIndivHit)
					{
						newIndivHit+=oldIndivHit-sommtouche1fois;
						oldIndivHit=sommtouche1fois;
					}

					if((sommtouche1fois >0)&&(oldIndivHit>0))
					{
						for(int classe=NBCLASSE-1;classe>=1;classe--)
						{
							// calcul du ratio d'individus de la classe
							double ratio = (NbIndivCumul[idxfilm][idxsem-1][classe])/sommtouche1fois; 


							// Passage des individus dans la classe sup�rieure
							NbIndivCumul[idxfilm][idxsem][classe+1]+=oldIndivHit*ratio;

							// Retrait des individus dans la classe
							NbIndivCumul[idxfilm][idxsem][classe]-=oldIndivHit*ratio;

						}
					}
				}
				else
				{
					// En premi�re semaine, tous les nouveaux individus sont touch�s pour la premi�re fois
					newIndivHit+=oldIndivHit;
					oldIndivHit=0;
				}
				// Ajout des nouveaux individus
				NbIndivCumul[idxfilm][idxsem][1] +=newIndivHit;

				// Retrait des ces m�mes individus dans la classe des non touch�s
				NbIndivCumul[idxfilm][idxsem][0] -=newIndivHit;

				// Incr�mentation du nombre d'individus touch�s total
				m_ResultatPermormances.m_NbIndiv+=newIndivHit;
			}
		}
	}

	// Transformation des matrices en proportion
	// Les nb d'individus touch�s n fois deviennent des probabilit�s d'avoir vu le film n fois pour la cible active
	for(idxfilm=0;idxfilm<tabprod->GetSize() ;idxfilm++)
	{

		for(int idxsem=0;idxsem<m_pCampagne->m_NbSemaine;idxsem++)
		{
			for(int classe=0;classe<=NBCLASSE;classe++)
			{

				// division par la popcible
				NbIndivSemaine[idxfilm][idxsem][classe]/=m_ResultatPermormances.m_Population;
				NbIndivCumul[idxfilm][idxsem][classe]/=m_ResultatPermormances.m_Population;
				
			}
		}
	}

	// V�rification temporaire
	// Produit(1-P(f)): individu n'ayant vu aucun films
	double probaaucunfilm=1;

	for(idxfilm=0;idxfilm<tabprod->GetSize();idxfilm++)
	{
		probaaucunfilm*=NbIndivCumul[idxfilm][m_pCampagne->m_NbSemaine-1][0];
	}

	double couverture = (1-probaaucunfilm)*100;
	
	CalculProduitStatistique(NbIndivSemaine,m_CouvertureSemaine);
	CalculProduitStatistique(NbIndivCumul,m_CouvertureCumul);


	// destruction matrice 
	for(idxfilm=0;idxfilm<tabprod->GetSize();idxfilm++)
	{
		for(int idxsem=0;idxsem<m_pCampagne->m_NbSemaine;idxsem++)
		{
			delete[] NbIndivSemaine[idxfilm][idxsem]; 
			delete[] NbIndivCumul[idxfilm][idxsem];
		}
		delete[] NbIndivSemaine[idxfilm]; 
		delete[] NbIndivCumul[idxfilm];
	}
	delete[] NbIndivSemaine;
	delete[] NbIndivCumul;

}



// Calcul de la fr�quentation d'un segment
double CCalculProduitFilm::CalculFrequentationSegment(CByteArray * listebac,int idxsegment)
{
	double Fseg=0;

	// S�lection de la sous cible du crit�re actif
	int nrcritereactif = m_pCampagne->m_TabProduitFilm.m_NrCritCoeff;

	BYTE * ModaliteCritereActif = theApp.m_Cible.GetNewVecteurCritere(nrcritereactif);
	
	// Calcul de Fsc
	// bouclage sur les bacs probabilis�s actifs
	for(int idxbacproba=0;idxbacproba<listebac->GetSize();idxbacproba++)
	{
		int nbindivsupp = theApp.m_NbIndivSupp[listebac->GetAt(idxbacproba)];
		// Prise en compte des individus du bac
		for(int idxindiv=0;idxindiv<nbindivsupp;idxindiv++)
		{
			DWORD nrindiv = theApp.m_NrIndivSupp[listebac->GetAt(idxbacproba)][idxindiv];
			if(ModaliteCritereActif[nrindiv]!=idxsegment)continue;
			Fseg+=(theApp.m_Proba2[listebac->GetAt(idxbacproba)][idxindiv])*(theApp.CoeffEx[nrindiv]);
		}
	}
	// destruction pointeur
	delete[] ModaliteCritereActif;

	return Fseg;
}







// Calcul de la fr�quentation sur la cible ensemble Fc = Somme(proba(i)*poids(i))
// i �tant un individu de la cible
double CCalculProduitFilm::CalculFrequentationTotale(CByteArray * listebac)
{
	double FEns=0;
	for(int idxbacproba=0;idxbacproba<listebac->GetSize();idxbacproba++)
	{
		int nbindivsupp = theApp.m_NbIndivSupp[listebac->GetAt(idxbacproba)];
		// Prise en compte des individus du bac
		for(int idxindiv=0;idxindiv<nbindivsupp;idxindiv++)
		{
			DWORD nrindiv = theApp.m_NrIndivSupp[listebac->GetAt(idxbacproba)][idxindiv];
			FEns+=(theApp.m_Proba2[listebac->GetAt(idxbacproba)][idxindiv])*(theApp.CoeffEx[nrindiv]);
		}
	}
	return FEns;
}

// Calcul de la fr�quentation de la cible active
double CCalculProduitFilm::CalculFrequentationCible (CByteArray * listebac)
{
	double Fc=0;

	// R�cup�ration du vecteur de cible
	VECTCIBLE Vecteur;
	Vecteur.Cible = (WORD)m_pCampagne->m_NumCible[0]; 
	theApp.m_Cible.GetVectCible(&Vecteur);

	// Calcul de Fc
	// bouclage sur les bacs probabilis�s actifs
	for(int idxbacproba=0;idxbacproba<listebac->GetSize();idxbacproba++)
	{
		int nbindivsupp = theApp.m_NbIndivSupp[listebac->GetAt(idxbacproba)];
		// Prise en compte des individus du bac
		for(int idxindiv=0;idxindiv<nbindivsupp;idxindiv++)
		{
			DWORD nrindiv = theApp.m_NrIndivSupp[listebac->GetAt(idxbacproba)][idxindiv];
			if(*(Vecteur.Vecteur+nrindiv))
			{
				Fc+=(theApp.m_Proba2[listebac->GetAt(idxbacproba)][idxindiv])*(*(Vecteur.Vecteur+nrindiv));
			}
		}
	}
	return Fc;
}

// Calcul de la fr�quentation de la sous cible active
// Seul les individus compris dans une modalit� du crit�re courant sont pris en compte
double CCalculProduitFilm::CalculFrequentationSouscible (CByteArray * listebac,CProduitFilm * prodfilm,int idxsouscible)
{	

	double Fsc=0;

	// r�cup�ration du coefficient d'extrapolation de la cible
	VECTCIBLE Vecteur;
	Vecteur.Cible = (WORD)m_pCampagne->m_NumCible[0]; 
	theApp.m_Cible.GetVectCible(&Vecteur);


	// S�lection de la sous cible du crit�re actif
	int nrcritereactif = m_pCampagne->m_TabProduitFilm.m_NrCritCoeff;
	BYTE * ModaliteCritereActif = theApp.m_Cible.GetNewVecteurCritere(nrcritereactif); // TODO
	
	// Calcul de Fsc
	// bouclage sur les bacs probabilis�s actifs
	for(int idxbacproba=0;idxbacproba<listebac->GetSize();idxbacproba++)
	{
		int nbindivsupp = theApp.m_NbIndivSupp[listebac->GetAt(idxbacproba)];
		// Prise en compte des individus du bac
		for(int idxindiv=0;idxindiv<nbindivsupp;idxindiv++)
		{
			DWORD nrindiv = theApp.m_NrIndivSupp[listebac->GetAt(idxbacproba)][idxindiv];
			if(ModaliteCritereActif[nrindiv]!=idxsouscible)continue;
			if(*(Vecteur.Vecteur+nrindiv))
			{
				Fsc+=(theApp.m_Proba2[listebac->GetAt(idxbacproba)][idxindiv])*(*(Vecteur.Vecteur+nrindiv));
			}
		}
	}
	// destruction pointeur
	delete[] ModaliteCritereActif;

	return Fsc;

}

// Calcul de la population de la sous cible active sur un r�seau g�ographique donn�
// Seul les individus compris dans une modalit� du crit�re courant sont pris en compte
int CCalculProduitFilm::CalculPopulationSousCibleGeo (CByteArray * listebac,int idxsouscible)
{	

	int pop=0;

	// r�cup�ration du coefficient d'extrapolation de la cible
	VECTCIBLE Vecteur;
	Vecteur.Cible = (WORD)m_pCampagne->m_NumCible[0]; 
	theApp.m_Cible.GetVectCible(&Vecteur);


	// S�lection de la sous cible du crit�re actif
	int nrcritereactif = m_pCampagne->m_TabProduitFilm.m_NrCritCoeff;
	BYTE * ModaliteCritereActif = theApp.m_Cible.GetNewVecteurCritere(nrcritereactif); // TODO
	
	// Calcul de Fsc
	// bouclage sur les bacs probabilis�s actifs
	for(int idxbacproba=0;idxbacproba<listebac->GetSize();idxbacproba++)
	{
		int nbindivsupp = theApp.m_NbIndivSupp[listebac->GetAt(idxbacproba)];
		// Prise en compte des individus du bac
		for(int idxindiv=0;idxindiv<nbindivsupp;idxindiv++)
		{
			DWORD nrindiv = theApp.m_NrIndivSupp[listebac->GetAt(idxbacproba)][idxindiv];
			if(ModaliteCritereActif[nrindiv]!=idxsouscible)continue;
			pop+=(*(Vecteur.Vecteur+nrindiv));
		}
	}
	// destruction pointeur
	delete[] ModaliteCritereActif;

	return pop;

}


// Calcul de la population dsegment actif sur un r�seau g�ographique donn�
// Seul les individus compris dans une modalit� du crit�re courant sont pris en compte
int CCalculProduitFilm::CalculPopulationSegmentGeo (CByteArray * listebac,int idxsouscible)
{	

	int pop=0;

	// S�lection de la sous cible du crit�re actif
	int nrcritereactif = m_pCampagne->m_TabProduitFilm.m_NrCritCoeff;
	BYTE * ModaliteCritereActif = theApp.m_Cible.GetNewVecteurCritere(nrcritereactif); // TODO
	
	// Calcul de Fsc
	// bouclage sur les bacs probabilis�s actifs
	for(int idxbacproba=0;idxbacproba<listebac->GetSize();idxbacproba++)
	{
		int nbindivsupp = theApp.m_NbIndivSupp[listebac->GetAt(idxbacproba)];
		// Prise en compte des individus du bac
		for(int idxindiv=0;idxindiv<nbindivsupp;idxindiv++)
		{
			DWORD nrindiv = theApp.m_NrIndivSupp[listebac->GetAt(idxbacproba)][idxindiv];
			if(ModaliteCritereActif[nrindiv]!=idxsouscible)continue;
			pop+=theApp.CoeffEx[nrindiv];
		}
	}
	// destruction pointeur
	delete[] ModaliteCritereActif;

	return pop;

}

// Calcul de la proportion de sur fr�quentation seconde par sous cible
// donne la somme de SF' de chacune des sous cible
void CCalculProduitFilm::CalculSurFrequentationParSousCible(CProduitFilm * filmprod, CArray<double,double&> * SFseconde,CByteArray * listeBacActif)
{
	double sommeSFprim=0;
	double SommeCoeffSouscible = CalculSommeCoeffSouscible(filmprod);
	SFseconde->RemoveAll();

	// fr�quentation cible 
	double Fc = CalculFrequentationCible(listeBacActif);

	// Calcul de SF et SF prime
	// + somme de SF' de chacune des sous cible

	for(int idxmod=0;idxmod<theApp.m_Cible.m_NbClasse[filmprod->m_CodeCritCoeff];idxmod++)
	{
			// fr�quentation sous cible
			double Fsc;
		
			Fsc = CalculFrequentationSouscible(listeBacActif,filmprod,idxmod);

			double SF = Fsc/Fc;
			
			if(SommeCoeffSouscible>0)
				SF*=(filmprod->m_TabCoeffFreq[idxmod])/(SommeCoeffSouscible);
			SFseconde->Add(SF);
			sommeSFprim+=SF;
	}
	// Calcul de SF seconde
	for(idxmod=0;idxmod<theApp.m_Cible.m_NbClasse[filmprod->m_CodeCritCoeff];idxmod++)
	{
		double result = SFseconde->GetAt(idxmod)/sommeSFprim;
		SFseconde->SetAt(idxmod,result);
	}
}

// Calcul de la proportion de sur fr�quentation seconde par segment
void CCalculProduitFilm::CalculSurFrequentationParSegment(CProduitFilm * filmprod, CArray<double,double&> * SFseconde,CByteArray * listeBacActif)
{
	double sommeSFprim=0;
	double SommeCoeffSouscible = CalculSommeCoeffSouscible(filmprod);
	SFseconde->RemoveAll();

	// fr�quentation cible 
	double Fens = CalculFrequentationTotale(listeBacActif);

	// Calcul de SF et SF prime
	// + somme de SF' de chacune des sous cible

	for(int idxmod=0;idxmod<theApp.m_Cible.m_NbClasse[filmprod->m_CodeCritCoeff];idxmod++)
	{
			// fr�quentation sous cible
			double Fseg;
		
			Fseg = CalculFrequentationSegment(listeBacActif,idxmod);

			double SF = Fseg/Fens;
			
			if(SommeCoeffSouscible>0)
				SF*=(filmprod->m_TabCoeffFreq[idxmod])/(SommeCoeffSouscible);
			SFseconde->Add(SF);
			sommeSFprim+=SF;
	}
	// Calcul de SF seconde
	for(idxmod=0;idxmod<theApp.m_Cible.m_NbClasse[filmprod->m_CodeCritCoeff];idxmod++)
	{
		double result = SFseconde->GetAt(idxmod)/sommeSFprim;
		SFseconde->SetAt(idxmod,result);
	}
}


// Calcul de la proportion de sur fr�quentation seconde par segment cibl�
// donne la somme de SF' de chacune des sous cible
void CCalculProduitFilm::CalculSurFrequentationParSegmentCible(CProduitFilm * filmprod, CArray<double,double&> * SFseconde,CByteArray * listeBacActif)
{

	CalculSurFrequentationParSegment(filmprod,SFseconde,listeBacActif);

	for(int idxmod=0;idxmod<theApp.m_Cible.m_NbClasse[filmprod->m_CodeCritCoeff];idxmod++)
	{
			// population de la cible
			double PopCible=CalculPopulationSousCibleGeo(listeBacActif,idxmod);
			double PopSegment=CalculPopulationSegmentGeo(listeBacActif,idxmod);
			double quota;
			if(!PopSegment)quota=0;
			else quota = (double)PopCible/(double)PopSegment;
			(*SFseconde)[idxmod]*=quota;
	}
}




// Calcul de la somme des coefficients de sur fr�quentation 
double CCalculProduitFilm::CalculSommeCoeffSouscible(CProduitFilm * filmprod)
{
		double sigma=0;
		for(int idxmod=0;idxmod<theApp.m_Cible.m_NbClasse[filmprod->m_CodeCritCoeff];idxmod++)
		{
			double coeffFreq = filmprod->m_TabCoeffFreq[idxmod];
			sigma+=filmprod->m_TabCoeffFreq[idxmod]; 
		}	
		return sigma;
}

// Pr�pare la liste des bacs actifs par semaine campagne (par film)
void CCalculProduitFilm::ConstruitListeBacsActifs(int idxfilm, CByteArray * listebac)
{

	// r�cup�ration de la table des films
	CTableProduitFilm * tabprod =  &m_pCampagne->m_TabProduitFilm;

	CString codeproduit = tabprod->GetAt(idxfilm).m_CodeProduit;

	// r�cup�ration de la table des produits
	CTableProduit * tabProduit = theApp.m_TableCinema.GetTableProduit();

	// r�cup index produit
	int idxprod = tabProduit->FindIndexFromCode(codeproduit); 

	// r�cup�ration de la table des bacs
	CTableBAC * tabBac = theApp.m_TableCinema.GetTableBac();

	// r�cup�ration de la table des complexes
	CTableComplexe * tabComplexe = theApp.m_TableCinema.GetTableComplexe();

	// Utilisation d'un map pour d�terminer si l'index bac a �t� ajout�
	CMap<BYTE,BYTE,BYTE,BYTE> Map_Bac;

	// duree produit
	int duree=1; 
	tagProduit produit = tabProduit->GetAt(idxprod);
	// si le produit est fig�
	// VERIF FRED DUREE
	if(produit.m_flagProduitFige)
	{
		duree = produit.GetDuree();
	}
	for(int idxsem=0;idxsem<duree;idxsem++)
	{
		// boucle sur la composition en bac
		for(int idxbac=0;idxbac<produit.m_DescriptionIndexBac[idxsem].GetSize();idxbac++)
		{
			int idxbacproba = produit.m_DescriptionIndexBac[idxsem][idxbac];
			// controle si  l'index n'a pas d�j� �t� ajout�
			if(!Map_Bac.Lookup(idxbacproba,(unsigned char&)idxbacproba))
			{
				listebac->Add(idxbacproba);
				Map_Bac.SetAt(idxbacproba,idxbacproba);
			}
		}

		// boucle sur la composition en complexe
		for(int idxcomplexe=0;idxcomplexe<produit.m_DescriptionIndexComplexe[idxsem].GetSize();idxcomplexe++)
		{
			// r�cup�ration de l'index bac correspondant au complexe
			int idxcmp = produit.m_DescriptionIndexComplexe[idxsem][idxcomplexe];

			int idxbac = tabComplexe->GetAt(idxcmp).m_indexBAC;
			if(!Map_Bac.Lookup(idxbac,(unsigned char&)idxbac))
			{
				listebac->Add(idxbac);
				Map_Bac.SetAt(idxbac,idxbac);
			}
		}
		
	}
	Map_Bac.RemoveAll();
}

// Pr�pare la liste des bacs actifs pour toute la campagne
void CCalculProduitFilm::ConstruitListeBacsActifs(CByteArray * listebac)
{

	// r�cup�ration de la table des films
	CTableProduitFilm * tabprod =  &m_pCampagne->m_TabProduitFilm;

	// r�cup�ration de la table des bacs
	CTableBAC * tabBac = theApp.m_TableCinema.GetTableBac();

	// r�cup�ration de la table des complexes
	CTableComplexe * tabComplexe = theApp.m_TableCinema.GetTableComplexe();

	// r�cup�ration de la table des produits
	CTableProduit * tabProduit = theApp.m_TableCinema.GetTableProduit();

	// Utilisation d'un map pour d�terminer si l'index bac a �t� ajout�
	CMap<BYTE,BYTE,BYTE,BYTE> Map_Bac;

	// Boucle sur les films
	for(int idxfilm=0;idxfilm<tabprod->GetSize();idxfilm++)
	{
		CString codeproduit = tabprod->GetAt(idxfilm).m_CodeProduit;

		// r�cup index produit
		int idxprod = tabProduit->FindIndexFromCode(codeproduit); 

		// duree produit
		int duree=1; 
		tagProduit produit = tabProduit->GetAt(idxprod);
		// si le produit est fig�
		// VERIF FRED DUREE
		if(produit.m_flagProduitFige)
		{
			duree = produit.GetDuree();
		}
		for(int idxsem=0;idxsem<duree;idxsem++)
		{
			// boucle sur la composition en bac
			for(int idxbac=0;idxbac<produit.m_DescriptionIndexBac[idxsem].GetSize();idxbac++)
			{
				int idxbacproba = produit.m_DescriptionIndexBac[idxsem][idxbac];
				// controle si  l'index n'a pas d�j� �t� ajout�
				if(!Map_Bac.Lookup(idxbacproba,(unsigned char&)idxbacproba))
				{
					listebac->Add(idxbacproba);
					Map_Bac.SetAt(idxbacproba,idxbacproba);
				}
			}

			// boucle sur la composition en complexe
			for(int idxcomplexe=0;idxcomplexe<produit.m_DescriptionIndexComplexe[idxsem].GetSize();idxcomplexe++)
			{
				// r�cup�ration de l'index bac correspondant au complexe
				int idxcmp = produit.m_DescriptionIndexComplexe[idxsem][idxcomplexe];

				int idxbac = tabComplexe->GetAt(idxcmp).m_indexBAC;
				if(!Map_Bac.Lookup(idxbac,(unsigned char&)idxbac))
				{
					listebac->Add(idxbac);
					Map_Bac.SetAt(idxbac,idxbac);
				}
			}
			
		}
	}
	Map_Bac.RemoveAll();
}

// Modif Alain
// Calcul de la population de la cible
int CCalculProduitFilm::CalculPopCible(int IndexCible)
{

	// R�cup�ration du vecteur de cible
	VECTCIBLE Vecteur;
	Vecteur.Cible = (WORD)m_pCampagne->m_NumCible[IndexCible]; 
	theApp.m_Cible.GetVectCible(&Vecteur);
	return Vecteur.PopPhy; 
}


// Calcul le produit de deux vecteurs binomiaux
void CCalculProduitFilm::CalculProduit(double  *VBf,double * couverture)
{
	double * VBtempo = new double[NBCLASSE+1];
	// copy de couverture dans VBtempo
	for(int classe=0;classe<=NBCLASSE;classe++)
	{
		VBtempo[classe] = couverture[classe];
		couverture[classe]=0;
	}

	// On boucle sur toutes les cases
	for(int x=0;x<=NBCLASSE;x++)
	{
		for (int y=0;y<=NBCLASSE;y++)
		{
	        int z=x+y;

			if (z> NBCLASSE) z= NBCLASSE;
			couverture[z] +=VBtempo[x]*VBf[y];
		}
	}
	delete VBtempo;

}

 
// calcule le vecteur r�sultat: probabilit� d'avoir vu exactement 0...20 films.
void CCalculProduitFilm::CalculProduitStatistique(double*** proba,double** couverture)
{
	// Initialisation du produit par la probabilit� de voir le premier film
	for(int s=0 ;s<m_pCampagne->m_NbSemaine;s++)
	{
		// Intialisation du premier film
		for(int classe=0;classe<=NBCLASSE;classe++)
		{
			couverture[s][classe]=proba[0][s][classe];
		}

		for(int f=1 ;f<m_pCampagne->m_TabProduitFilm.GetSize();f++)
		{
			CalculProduit(proba[f][s],couverture[s]);
		}
	}

}



// retourne la population de la cible en ne consid�rant seulement les bacs r�gionaux ou nationaux
double CCalculProduitFilm::GetPopulationCibleBase(int base)
{
	// population
	double pop=0;

	// Base Regionale
	if(!base)
	{
		// Liste des bacs r�gionaux
		CByteArray listeBacActif;

		ConstruitListeBacsActifs(&listeBacActif);


		// bouclage sur les bacs probabilis�s actifs
		for(int idxbacproba=0;idxbacproba<listeBacActif.GetSize();idxbacproba++)
		{
			int nbindivsupp = theApp.m_NbIndivSupp[listeBacActif.GetAt(idxbacproba)];
			// Prise en compte des individus du bac
			for(int idxindiv=0;idxindiv<nbindivsupp;idxindiv++)
			{
				DWORD nrindiv = theApp.m_NrIndivSupp[listeBacActif.GetAt(idxbacproba)][idxindiv];
				pop+=theApp.CoeffEx[nrindiv];
			}
		}
	}
	// pop Base nationale = pop cible
	else
	{
		pop = CalculPopCible(0);
	}
	return pop;
}
