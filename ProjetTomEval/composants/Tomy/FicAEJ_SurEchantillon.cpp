// FicAEJ_SurEchantillon.cpp: implementation of the CFicAEJ_SurEchantillon class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "FicAEJ_SurEchantillon.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

//##ModelId=3D9AB7D60176
CFicAEJ_SurEchantillon::CFicAEJ_SurEchantillon()
{
	m_FiltreOffreElargie=NULL;
	m_NrIndivBase=NULL;
	m_NrIndivSurEchantillon=NULL;
	m_NbIndivBase=m_NbIndivSurEchantillon=0;
}

//##ModelId=3D9AB7D60177
CFicAEJ_SurEchantillon::~CFicAEJ_SurEchantillon()
{
	if(!m_FileLoaded)return;
	if(m_FiltreOffreElargie)delete [] m_FiltreOffreElargie;
	if(m_NrIndivBase)delete [] m_NrIndivBase;
	if(m_NrIndivSurEchantillon)delete [] m_NrIndivSurEchantillon;
}

#include "LiaisonChaineTeleFrance.h"
//##ModelId=3D9AB7D60179
bool CFicAEJ_SurEchantillon::BindPanelSurEchntillonOffreElargie(LPCSTR NomPanelFusion,CFicProba * FicProbaBase,CFicProba * FicProbaSurEchantillon,char * filtreOffreElargie)
{
	CWaitCursor wait;
	TRACE("Construction d'un panel avec un sur-�chantillon sur l'offre �largie...\n");


	m_FiltreOffreElargie=filtreOffreElargie;

	ASSERT(AfxCheckMemory());

	// M�morisation d'un nom unique pour le retrouver
	SetNomFichierUnique(NomPanelFusion);


	// +++++ Pr�paration des calculs... +++++
	CFicAEJ * pFicProbaBase=(CFicAEJ*)FicProbaBase;
	CFicAEJ * pFicProbaSurEchantillon=(CFicAEJ*)FicProbaSurEchantillon;

	// Calcul du nombre de station
	// on compte toutes les cha�nes hertzienne
	m_NbStation=pFicProbaBase->m_NbStation;

	// on ajoute toutes les cha�nes th�matique qui ne sont pas dans les hertzienne
	CLiaisonChaineTeleFrance linkChaine;
	for(int s=0;s<pFicProbaSurEchantillon->m_NbStation;s++)
	{
		// Si cette chaine existe dans les hertzienne, il ne faut pas la compter
		// donc on compte que celles que l'on ne retrouve pas
		if(linkChaine.GetNumeroMediadata(pFicProbaSurEchantillon->m_NrStation[s])==0)
			m_NbStation++;
	}


	// Calcul du nombre d'individus:
	// Le nombre d'individus vaut les individus qui ne sont pas
	// initialis�s offre �largie plus le sur-�chantillon
	m_NbIndiv=pFicProbaSurEchantillon->m_NbIndiv;
	//int NbIndivInitialise=0;
	for(int i=0;i<pFicProbaBase->m_NbIndiv;i++)
	{
		ASSERT(filtreOffreElargie[i]==0 || filtreOffreElargie[i]==1);
		if(filtreOffreElargie[i]==0)m_NbIndiv++;
		//else NbIndivInitialise++;
	}

	// Pr�paration de nouveau num�ro pour les individus du M�diadata
	m_NbIndivBase=pFicProbaBase->m_NbIndiv;
	m_NrIndivBase=new unsigned short[m_NbIndivBase];
	int nr=0;
	for(i=0;i<m_NbIndivBase;i++)
	{
		if(filtreOffreElargie[i]==0)
		{
			m_NrIndivBase[i]=nr;
			nr++;
		}
		else
		{
			m_NrIndivBase[i]=-1;
		}
	}

	// Pr�paration de nouveau num�ro pour les individus du M�diacabsat
	m_NbIndivSurEchantillon=pFicProbaSurEchantillon->m_NbIndiv;
	m_NrIndivSurEchantillon=new unsigned short[m_NbIndivSurEchantillon];
	// nr est d�j� initialis� dans la boucle ci-dessus
	for(i=0;i<m_NbIndivSurEchantillon;i++)
	{
		m_NrIndivSurEchantillon[i]=nr;
		nr++;
	}

	// Vue que nous ne disposons pas des taux d'�coute pour le m�diacabsat,
	// il est pour le moment impossible de travailler avec la methode par compptage.

	// Si cette information existe, il reste un probl�me � r�soudre car les vagues
	// ne sont certainement pas phase...le jour ne peut donc �tre bon !
	m_NbJour=-1;		// NE DOIT PAS ETRE UTILISE POUR LE COMPTAGE
	m_DateDebut=-1;	// NE DOIT PAS ETRE UTILISE POUR LE COMPTAGE

	// +++++ OK, on peut commencer � construire le panel +++++

	m_NrStation=new short [m_NbStation];
	m_NbEcranP=new short* [m_NbStation];
	m_NrEcranP=new short** [m_NbStation];
	m_ProbaQH=new CProbaSupport** [m_NbStation];


	// Pr�paration pour le calcul des ratio de consomation multi-support
	m_NbSupportMoyenParPanelParJour[0]=0;

	// On boucle sur le mediadata
	for(s=0;s<pFicProbaBase->m_NbStation;s++)
	{
		// On copie les probabilit�s des individus qui ne sont pas initialis�s offre �largie
		int nbjour=7;
		m_NrStation[s]=pFicProbaBase->m_NrStation[s];
		m_NbEcranP[s]=new short [nbjour];
		m_NrEcranP[s]=new short* [nbjour];
		m_ProbaQH[s]=new CProbaSupport* [nbjour];
		for(int j=0;j<nbjour;j++)
		{
			m_NbEcranP[s][j]=pFicProbaBase->m_NbEcranP[s][j];
			m_NrEcranP[s][j]=new short [m_NbEcranP[s][j]];
			m_ProbaQH[s][j]=new CProbaSupport [m_NbEcranP[s][j]];
			m_NbSupportMoyenParPanelParJour[0]+=m_NbEcranP[s][j];
			for(int e=0;e<m_NbEcranP[s][j];e++)
			{
				m_NrEcranP[s][j][e]=pFicProbaBase->m_NrEcranP[s][j][e];

				// Copie des probabilit� des individus qui ne sont pas offre �largie
				for(int c=0;c<pFicProbaBase->m_ProbaQH[s][j][e].m_NbCas;c++)
				{
					int i=pFicProbaBase->m_ProbaQH[s][j][e].m_NrIndividu[c];
					if(filtreOffreElargie[i]==0)
					{
						ASSERT(m_NrIndivBase[i]>=0);
						m_ProbaQH[s][j][e].AddNouveauCas(m_NrIndivBase[i],pFicProbaBase->m_ProbaQH[s][j][e].m_Proba[c]);
					}
				}
			}
		}
	}

	// Pr�paration pour le calcul des ratio de consomation multi-support
	// Pour comparer on parle de nb de support moyen par jour (unit� de d�coupage)
	m_NbSupportMoyenParPanelParJour[0]/=7*pFicProbaBase->m_NbStation;
	m_NbSupportMoyenParPanelParJour[1]=96;


	// on boucle maintenant sur le sur-�chantillon
	// l'index s2 est sur le sur �chantillon, s est sur le nouveau panel
	for(int s2=0;s2<pFicProbaSurEchantillon->m_NbStation;s2++)
	{
		// Est-ce une chaine th�matique ou hertzienne ?
		int nrchaine=linkChaine.GetNumeroMediadata(pFicProbaSurEchantillon->m_NrStation[s2]);
		if(nrchaine==0)
		{
			// Cha�ne CABSAT, c'est simple, il suffit d'allouer puis
			// de copier toutes les probabilit�s en changeant simplement le nr d'individu
			int nbjour=7;
			m_NrStation[s]=pFicProbaSurEchantillon->m_NrStation[s2];
			m_NbEcranP[s]=new short [nbjour];
			m_NrEcranP[s]=new short* [nbjour];
			m_ProbaQH[s]=new CProbaSupport* [nbjour];
			for(int j=0;j<nbjour;j++)
			{
				m_NbEcranP[s][j]=pFicProbaSurEchantillon->m_NbEcranP[s][j];
				m_NrEcranP[s][j]=new short [m_NbEcranP[s][j]];
				m_ProbaQH[s][j]=new CProbaSupport [m_NbEcranP[s][j]];
				for(int e=0;e<m_NbEcranP[s][j];e++)
				{
					m_NrEcranP[s][j][e]=pFicProbaSurEchantillon->m_NrEcranP[s2][j][e];

					// Copie des probabilit�s
					m_ProbaQH[s][j][e].SetNbCas(pFicProbaSurEchantillon->m_ProbaQH[s2][j][e].m_NbCas);
					for(int c=0;c<pFicProbaSurEchantillon->m_ProbaQH[s2][j][e].m_NbCas;c++)
					{
						m_ProbaQH[s][j][e].m_NrIndividu[c]=m_NrIndivSurEchantillon[pFicProbaSurEchantillon->m_ProbaQH[s2][j][e].m_NrIndividu[c]];
						m_ProbaQH[s][j][e].m_Proba[c]=pFicProbaSurEchantillon->m_ProbaQH[s2][j][e].m_Proba[c];
					}
				}
			}
			s++; // pour passer � la m�morisation suivante
		}
		else
		{
			// Chaine CABSAT ET HERTZIENNE; il faur enrichir les �crans avec les probabilites des
			// quarts d'heure qui de trouvent dans le sur-�chantillon

			// Premi�re �tape, on recherche la position de cette cha�ne.
			// NB il n'est pas utile de rechercher partout puisque le chaine que l'on cherche
			// EST obligatoirement dans les n premi�res
			for(int s3=0;s3<pFicProbaBase->m_NbStation;s3++)
			{
				if(m_NrStation[s3]==nrchaine)break;
			}
			if(s3>=pFicProbaBase->m_NbStation)
			{
				AfxMessageBox("Erreur dans la construction du sur-�chantillon:\nUne cha�ne CABSAT pr�sente en HERTZIEN ne retrouve pas les probas des individus de l'offre non �largie.");
				ASSERT(0);
			}
			else
			{
				// On ajoute les probabilit� � tous les �crans
				for(int j=0;j<7;j++)
				{
					for(int e=0;e<m_NbEcranP[s3][j];e++)
					{
						int ecran=m_NrEcranP[s3][j][e];

						// Recherche de l'�cran qui va bien:
						// NOTE on sait que les �crans sont des exQH base 3H00
						// donc pour aller plus vite on formule:
						int qh=(((ecran/100)-3)*4)+((ecran%100)/15);

						// AJOUT des probabilit�s
						for(int c=0;c<pFicProbaSurEchantillon->m_ProbaQH[s2][j][qh].m_NbCas;c++)
						{
							m_ProbaQH[s3][j][e].AddNouveauCas(
								m_NrIndivSurEchantillon[pFicProbaSurEchantillon->m_ProbaQH[s2][j][qh].m_NrIndividu[c]],
								pFicProbaSurEchantillon->m_ProbaQH[s2][j][qh].m_Proba[c]);
						}
					}
				}
			}
		}
	}

	ASSERT(AfxCheckMemory());
	TRACE("Construction du panel Termin�.\n");
	m_FileLoaded=1;
	return(1); // OK
}


// Calcul pour chaque individu � l'appartenance � une strate d'individu.
// Le nombre de strate est d�termin� par l'argument
//##ModelId=3D9AB7D60195
void CFicAEJ_SurEchantillon::CalculRatioConsomation(int NbRatio)
{

	CWaitCursor wait;
	m_NrRatioConso.SetSize(m_NbIndiv);

	// 1/ On boucle sur le �coutes (probabilit�s) pour �tablir la consomation
	// de chacun des individus dans le m�dia concern� :

	CArray<double,double&> Conso;
	Conso.SetSize(m_NbIndiv);
	for(int i=0;i<m_NbIndiv;i++)Conso[i]=0;
	int NbJour=7; // Jour probabilis�
	for(int s=0;s<m_NbStation;s++)
	{
		for(int j=0;j<NbJour;j++)
		{
			for(int e=0;e<m_NbEcranP[s][j];e++)
			{
				for(int i=0;i<m_ProbaQH[s][j][e].m_NbCas;i++)
				{
					Conso[m_ProbaQH[s][j][e].m_NrIndividu[i]]+=m_ProbaQH[s][j][e].m_Proba[i];
				}
			}
		}
	}


	// On calcul une consomation relative
	for(i=0;i<m_NbIndiv;i++)
		Conso[i]/=GetNbSupportParIndividu(i);



	// Maintenant on range les r�sultats pour constituer des strates de consomation
	// Il suffit de boucler autant de fois qu'il y a d'indiv
	for(i=0;i<m_NbIndiv;i++)
	{
		int index=-1;
		double mini=0;
		for(int b=0;b<m_NbIndiv;b++)
		{
			if(Conso[b]==-1)continue; // Individu d�j� marqu�
			if(index<0)
			{
				// On prend le premier
				index=b;
				mini=Conso[b];
			}
			else if(Conso[b]<mini)
			{
				// On le prend car il est pour le moment
				// le plus petit que l'on est trouv�
				index=b;
				mini=Conso[b];
			}
		}

		ASSERT(index>=0 && index<m_NbIndiv);
		m_NrRatioConso[index]=(i*NbRatio)/m_NbIndiv;
		ASSERT(m_NrRatioConso[index]>=0);
		ASSERT(m_NrRatioConso[index]<NbRatio);
		Conso[index]=-1;
	}
}

