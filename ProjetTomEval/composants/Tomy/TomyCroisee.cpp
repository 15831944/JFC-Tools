// TomyCroisee.cpp: implementation of the CTomyCroisee class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "TomyCroisee.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CTomyCroisee::CTomyCroisee()
{
	m_MB=NULL;
	m_MBt=NULL;
	m_NbMB=NULL;
}

CTomyCroisee::~CTomyCroisee()
{
	FreeMatriceBinomiale();
}



inline void CTomyCroisee::InitialiseMatrice(double ** Matrice)
{
	for(int c=0;c<=NBCLASSEDEGAU;c++)
		for(int x=0;x<=NBCLASSEDEGAU;x++)
			Matrice[c][x]=+0.0;
}

inline void CTomyCroisee::InitialiseMatriceTemporaire()
{
	for(int c=0;c<=NBCLASSEDEGAU;c++)
		for(int x=0;x<=NBCLASSEDEGAU;x++)
			m_MBt[c][x]=+0.0;
}

// Calcul une évaluation en distribution croisée.
// Cette opération se substitue à la fonction de base: LanceEvalDistribution
// Retourne la couverture 1+
//##ModelId=3D9AB7D2018D
double CTomyCroisee::LanceEvalDistributionCroisee()
{
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

	// Allocation automatique de la matrice binomiale par individu
	if(m_MB==NULL)
	{
		AllocMatriceBinomiale();
	}
	
	// Initialisation du vecteur binomiale par individu
	for(int i=0;i<m_NbIndiv;i++)
	{
		m_MB[i][0][0]=+1.0;
		m_NbMB[i][0]=0;
		m_NbMB[i][1]=0;
		for (int t=0;t<m_NbClassesCalculDistribution;t++)
		{
			for (int c=0;c<m_NbClassesCalculDistribution;c++)
			{
				if((t!=0) || (c!=0))
				{
					m_MB[i][t][c]=+0.0;
				}
			}
		}
	}
	
	// Pour éviter les débordements dans les petits plans,
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
		for(int s=0;s<nbecran;s++){
			if(!TrouveProba(m_Plan[s]))continue;
			CalculMWventile(m_Plan[s]);
			CalculApportDansMatricesBinomiale(m_Plan[s]);
			if(m_Param.m_CallBack)m_Param.m_CallBack->OneSpotEvaluated();
		}
	}
	else {
		for(int s=0;s<nbecran;s++){
			if(!TrouveProba(m_Plan[s]))continue;
			CalculMediaWeight(m_Plan[s]);
			CalculApportDansMatricesBinomiale(m_Plan[s]);
			if(m_Param.m_CallBack)m_Param.m_CallBack->OneSpotEvaluated();
		}
	}

	// Pour le méthode par comptage, on met à jour la matrice binomiale de chaque individu
	if(m_Param.m_MethodeComptage==1)
	{	// METHODE PAR COMPTAGE
		for(i=0;i<m_NbIndiv;i++)
		{
			if(m_CoeffEx[i]<=0)continue;
			m_MB[i][0][0]=0.0;
			if(m_NbMB[i][0]>=m_Cmax)m_NbMB[i][0]=m_Cmax-1;
			if(m_NbMB[i][1]>=m_Cmax)m_NbMB[i][1]=m_Cmax-1;
			m_MB[i][m_NbMB[i][0]][m_NbMB[i][1]]=+1.0;
		}
	}

	// On met à jour le vecteur des non-touchés
	for(i=0;i<m_NbIndiv;i++)m_NT[i]=m_MB[i][0][0];
	
	
	// pour tester
	//CalculExactementToucheViaMatriceIndividuelle();

	// Intégration des individus
	CalculDistributionCroisee();
	m_Couverture=CalculExactementTouche();

	// Une petite vérification de la mémoire...
	ASSERT(_CrtCheckMemory());

	return (m_Couverture);
}


void CTomyCroisee::CalculApportDansMatricesBinomiale(CTomyCroisee::CElement & E)
{
	if(E.m_MW<=0)return;
	int i,NrIndiv;
	if(m_Param.m_MethodeComptage==0)
	{	// METHODE PROBABILISTE
		double CoeffDuplication=m_Param.GetCoeffDuplication();
		m_MBt=new double* [1+NBCLASSEDEGAU];
		for(int c=0;c<=NBCLASSEDEGAU;c++)
			m_MBt[c]=new double [1+NBCLASSEDEGAU];

		if(CoeffDuplication>0)
		{

			// Calcul avec les duplication. La duplication concerne les ecrans,
			// et consiste à calculer un très forte dupplication entre ces écrans
			// Exemple remarquable: Deux spots dans le même écran publicitaire
			// Il n'y a donc dans ce cas qu'un vecteur à jouer

			for(i=0;i<E.m_ProbaSupp->m_NbCas;i++)
			{
				NrIndiv=E.m_ProbaSupp->m_NrIndividu[i];
				if(m_CoeffEx[NrIndiv]<=0)continue;
				GetTrusquinVB(E.m_ProbaSupp->m_Proba[i],E.m_MW);
				AppliqueCorrectionHypotheseIndependance();
				CalculDuplicationVB(E.m_NbInsertion);

				CalculApportDansMatriceBinomiale(NrIndiv,E.m_Media);
			}
		}
		else // Calcul sans les duplications

			// Note ce calcul est effectué autant de fois qu'il y a de support, 
			// Cela signife que l'on considère qu'il n'existe aucune dupplication du même support
			// C'est uniquement l'hypothèse d'indépendance qui joue.
		{
			for(i=0;i<E.m_ProbaSupp->m_NbCas;i++)
			{
				NrIndiv=E.m_ProbaSupp->m_NrIndividu[i];
				if(m_CoeffEx[NrIndiv]<=0)continue;
				GetTrusquinVB(E.m_ProbaSupp->m_Proba[i],E.m_MW);
				AppliqueCorrectionHypotheseIndependance();
				// On boucle sur les spots comme si ils étaient indépendants
				for(int spot=0;spot<E.m_NbInsertion;spot++)
				{
					CalculApportDansMatriceBinomiale(NrIndiv,E.m_Media);
				}
			}
		}

		// Libération de la matrice temporaire
		for(c=0;c<=NBCLASSEDEGAU;c++)
			delete [] m_MBt[c];
		delete [] m_MBt;
		m_MBt=NULL;
	}
	else
	{	// METHODE PAR COMPTAGE

		//CETTE METHODE n'est pas au programme de la distribution croisée ...
ASSERT(0);


/*		double Seuil=1-m_Param.m_SeuilDeVision;
		for(i=0;i<E.m_ProbaSupp->m_NbCas;i++){
			NrIndiv=E.m_ProbaSupp->m_NrIndividu[i];
			if(m_CoeffEx[NrIndiv]<=0)continue;
			if(GetTrusquinNT(E.m_ProbaSupp->m_Proba[i],E.m_MW)<=Seuil)
			{
				// Pour des raisons de rapidité, on ne met pas à jour
				// le vcteur binomiale m_VB[NrIndiv], mais uniquement
				// le maxi, qui correspont au nombre de fois touchés
				// pour un individu donné.
				m_NbVB[NrIndiv]++;
			}
		}*/
	}
}

#ifdef OPTION_DUREEECOUTE
ERREUR !!!
#else
void CTomyCroisee::AddSpot(short Station, short JourNomme,short QHeure,double GRP,short NbInsertion,BYTE Media)
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
	E.m_Media=Media; // 0 ou 1
	ASSERT(E.m_Media==0 || E.m_Media==1);
	m_Plan.Add(E);
}
#endif


// NOTE on passe par référence pour aller plus vite
inline void CTomyCroisee::CalculApportDansMatriceBinomiale(int & NrIndiv,BYTE & Media)
{
	ASSERT(Media==0 || Media==1);

	int NbMB0=m_NbMB[NrIndiv][0];
	int NbMB1=m_NbMB[NrIndiv][1];
	double ** MBi=m_MB[NrIndiv];
	InitialiseMatriceTemporaire();
	int maxi,a,b,bb,c,cc;
	if(Media==0)
	{
		maxi=NbMB0;
		// Le support apporte des contacts dans le premier Média
		for(a=0;a<=m_NbVBs;a++)for(b=0;b<=NbMB0;b++)for(c=0;c<=NbMB1;c++)
		{
			bb=a+b;
			if(bb>=m_Cmax)bb=m_Cmax-1;
			if(maxi<bb)maxi=bb;
			m_MBt[bb][c]+=(m_VBs[a])*(MBi[b][c]);
		}	
		m_NbMB[NrIndiv][0]=maxi;
	}
	else
	{
		// Le support apporte des contacts dans le second Média
		maxi=NbMB1;
		// Le support apporte des contacts dans le premier Média
		for(a=0;a<=m_NbVBs;a++)for(b=0;b<=NbMB0;b++)for(c=0;c<=NbMB1;c++)
		{
			cc=a+c;
			if(cc>=m_Cmax)cc=m_Cmax-1;
			if(maxi<cc)maxi=cc;
			m_MBt[b][cc]+=(m_VBs[a])*(MBi[b][c]);
		}	
		m_NbMB[NrIndiv][1]=maxi;
	}
	
	// On met à jour les vecteurs
	m_MB[NrIndiv]=m_MBt;
	m_MBt=MBi;


	// Un petit test pour vérifier que la matrice binomiale est juste
#ifdef _DEBUG
	double sigma=+0.0;
	for(b=0;b<=m_NbMB[NrIndiv][0];b++)
		for(c=0;c<=m_NbMB[NrIndiv][1];c++)
			sigma+=m_MB[NrIndiv][b][c];
	ASSERT(sigma>0.999999 && sigma<1.000001);
#endif

}

void CTomyCroisee::AllocMatriceBinomiale()
{
	m_MB=new double ** [m_NbIndiv];
	for(int i=0;i<m_NbIndiv;i++)
	{
		m_MB[i]=new double* [1+NBCLASSEDEGAU];
		for(int c=0;c<=NBCLASSEDEGAU;c++)
		{
			m_MB[i][c]=new double [1+NBCLASSEDEGAU];
		}
	}

	if(!m_VBs)
	{
		if(m_Param.GetCoeffDuplication()>0)
		{
			// Si le calcul des duplicaions est actif il est possible d'avoir
			// un Vecteur Binomiale très gros
			m_VBs=new double [m_Param.m_NbClassesResultatDistribution+1];
		}
		else 
		{
			// C'est que c'est limité au MédiaWeight maximum
			m_VBs=new double [MWMAX+1];
		}
	}

	m_NbMB=new int*  [m_NbIndiv];
	for(i=0;i<m_NbIndiv;i++)
		m_NbMB[i]=new int [2]; // Fusion bimédia=2 dimensions
}

void CTomyCroisee::FreeAllocIndiv()
{
	FreeMatriceBinomiale();
	CTomy::FreeAllocIndiv();
}

void CTomyCroisee::FreeMatriceBinomiale()
{
	if(m_MB==NULL)return;
	for(int i=0;i<m_NbIndiv;i++)
	{
		for(int c=0;c<=NBCLASSEDEGAU;c++)
		{
			delete [] m_MB[i][c];
		}
		delete [] m_MB[i];
		delete [] m_NbMB[i];
	}
	delete [] m_MB;
	delete [] m_VBs;
	delete [] m_NbMB;

	m_MB=NULL;
	m_VBs=NULL;
	m_NbMB=NULL;
}


//##ModelId=3D9AB7D20273
double CTomyCroisee::CalculExactementToucheViaMatriceIndividuelle()
{
	ASSERT(m_MB);
	ASSERT(m_Population);

	m_DistributionExactementTouche.CreateNewDistribution(m_NbClassesCalculDistribution);
	m_DistributionExactementTouche.Initialise();

	double coeff;
	for(int i=0;i<m_NbIndiv;i++)
	{
		if((coeff=m_CoeffEx[i])<=0)continue;
		if(m_flagFiltreIndividus)coeff*=m_FiltreIndividus[i];

	// Un petit test pour vérifier que la matrice binomiale est juste
#ifdef _DEBUG
		{
			double sigma=+0.0;
			for(int a=0;a<=m_NbMB[i][0];a++)
				for(int b=0;b<=m_NbMB[i][1];b++)
					sigma+=m_MB[i][a][b];
			ASSERT(sigma>0.999999 && sigma<1.000001);
		}
#endif

		for(int a=0;a<=m_NbMB[i][0];a++)
		for(int b=0;b<=m_NbMB[i][1];b++)
		{
			int c=a+b;
			if(c>=m_NbClassesCalculDistribution)
				c=m_NbClassesCalculDistribution-1;
			m_DistributionExactementTouche.AddValue(c,m_MB[i][a][b]*coeff);
		}
	}

	m_DistributionExactementTouche*=100.0/m_Population;

	/*tring txt;
	txt.Format("Population= %ld c%%=%.2f",m_Population,result);
	AfxMessageBox(txt);*/

	// On retourne la couverture 1+
		return(m_DistributionExactementTouche.CalculCouverture_nPlus(1));
}

double CTomyCroisee::CalculExactementTouche()
{
	return (CalculExactementToucheViaDistributionCroisee());
	// return (CalculExactementToucheViaMatriceIndividuelle());
}


double CTomyCroisee::CalculExactementToucheViaDistributionCroisee()
{
	m_DistributionExactementTouche.CreateNewDistribution(m_NbClassesCalculDistribution);
	m_DistributionExactementTouche.Initialise();
	m_Distribution0.CreateNewDistribution(m_NbClassesCalculDistribution);
	m_Distribution0.Initialise();
	m_Distribution1.CreateNewDistribution(m_NbClassesCalculDistribution);
	m_Distribution1.Initialise();


	for(int a=0;a<m_NbClassesCalculDistribution;a++)
	for(int b=0;b<m_NbClassesCalculDistribution;b++)
	{
		int c=a+b;
		if(c>=m_NbClassesCalculDistribution)
			c=m_NbClassesCalculDistribution-1;
		double v=m_DistributionCroisee.GetValue(a,b);
		m_DistributionExactementTouche.AddValue(c,v);
		m_Distribution0.AddValue(a,v);
		m_Distribution1.AddValue(b,v);
	}

#ifdef _DEBUG
//	ASSERT(m_DistributionExactementTouche.CalculCouverture_nPlus(0)==m_Distribution0.CalculCouverture_nPlus(0));
//	ASSERT(m_Distribution0.CalculCouverture_nPlus(0)==m_Distribution1.CalculCouverture_nPlus(0));
#endif

	// On retourne la couverture 1+
	m_Couverture=m_DistributionExactementTouche.CalculCouverture_nPlus(1);
	return(m_Couverture);
}



void CTomyCroisee::CalculDistributionCroisee()
{
	ASSERT(m_MB);
	ASSERT(m_Population);

	m_DistributionCroisee.CreateNewDistribution(m_NbClassesCalculDistribution);
	m_DistributionCroisee.Initialise();

	double coeff;
	for(int i=0;i<m_NbIndiv;i++)
	{
		if((coeff=m_CoeffEx[i])<=0)continue;
		if(m_flagFiltreIndividus)coeff*=m_FiltreIndividus[i];

	// Un petit test pour vérifier que la matrice binomiale est juste
#ifdef _DEBUG
		{
			double sigma=+0.0;
			for(int a=0;a<=m_NbMB[i][0];a++)
				for(int b=0;b<=m_NbMB[i][1];b++)
					sigma+=m_MB[i][a][b];
			ASSERT(sigma>0.999999 && sigma<1.000001);
		}
#endif

		for(int a=0;a<=m_NbMB[i][0];a++)
		for(int b=0;b<=m_NbMB[i][1];b++)
		{
			m_DistributionCroisee.AddValue(a,b,m_MB[i][a][b]*coeff);
		}
	}

	// Si il y a un filtre sur des individus, il faut recalculer
	// les non touchés
	if(m_flagFiltreIndividus)
	{
		double IndividuTouche=m_DistributionCroisee.CalculCouverture_nPlus(1);
		m_DistributionCroisee.SetValue(0,0,m_Population-IndividuTouche);
	}
	m_DistributionCroisee*=100.0/m_Population;

#ifdef _DEBUG
	double verif=m_DistributionCroisee.CalculCouverture_nPlus(0);
	ASSERT(verif>99.99999);
	ASSERT(verif<100.00001);
#endif

	// On retourne la couverture 1+
	// return(m_DistributionCroisee.CalculCouverture_nPlus(1));
}
