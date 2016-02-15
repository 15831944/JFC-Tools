// TableConsomationTotalTV.cpp: implementation of the CTableConsomationTotalTV class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "TableConsomationTotalTV.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CTableConsomationTotalTV::CTableConsomationTotalTV()
{
	m_fLoaded=false;
	m_Consomation=NULL;
	m_NbIndiv=0;
	m_NrIndivTrie=NULL;
}

CTableConsomationTotalTV::~CTableConsomationTotalTV()
{
	if(m_fLoaded)
	{
		delete [] m_Consomation;
		delete [] m_NrIndivTrie;
		m_Consomation=0;
		m_NbIndiv=0;
		m_fLoaded=0;
		m_NrIndivTrie=NULL;
	}

}

int CTableConsomationTotalTV::LoadFile(LPCSTR Chemin,LPCSTR Etude,LPCSTR Panel)
{
	CString NomFic;
	NomFic.Format("%s\\%s\\%s.totalTV",Chemin,Etude,Panel);
	return(LoadFile(NomFic));
}

int CTableConsomationTotalTV::LoadFile(LPCSTR NomFic)
{
	if(NomFic==m_Nomfich)return(1);
	CStdioFile f;
	CJFCFileException e;
	CString txt;

	// Ouverture du fichier de convertion
	if( !f.Open(NomFic, CFile::modeRead | CFile::typeBinary, &e ) )
	{
		CString MsgErr;
		MsgErr.Format("Erreur à l'ouverture du fichier %s : ",NomFic);
		e.AfficheErreur(MsgErr);
		return(0);
	}

	TRACE("Lecture du fichier %s\n",NomFic);

	m_NbIndiv=(int)(f.GetLength())/sizeof(double);
	m_Consomation=new double [m_NbIndiv];
	f.Read(m_Consomation,sizeof(double)*m_NbIndiv);

	f.Close();
	m_fLoaded=true;
	m_Nomfich=NomFic;


#ifdef _DEBUG
	double sigma=0;
	for(int i=0;i<m_NbIndiv;i++)
	{
		sigma+=m_Consomation[i];
	}
	sigma/=m_NbIndiv;
	TRACE("Taux d'écoute moyen=%.2f%%\n",sigma);
#endif


	BindVecteurIndividuTrie();
	return (1); //OK
}

void CTableConsomationTotalTV::BindVecteurIndividuTrie()
{
	int i;
	if(m_NrIndivTrie) delete [] m_NrIndivTrie;
	m_NrIndivTrie= new int [m_NbIndiv];

	// Préparation des candidats
	CDWordArray TableCandidat;
	TableCandidat.SetSize(m_NbIndiv);
	for (i=0;i<m_NbIndiv;i++)TableCandidat[i]=i;

	// On les range dans un vecteur trié
	for(i=0;i<m_NbIndiv;i++)
	{
		int best=-1;
		double bestvalue =-1;

		for(int c=0;c<TableCandidat.GetSize();c++)
		{
			if(m_Consomation[TableCandidat[c]]>bestvalue)
			{
				best=c;
				bestvalue=m_Consomation[TableCandidat[c]];
			}
		}

		m_NrIndivTrie[i]=TableCandidat[best];
		TableCandidat.RemoveAt(best);
	}
}

void CTableConsomationTotalTV::CalculFiltreStrateConsoCible(long * CoeffEx,double * m_FiltreIndividus,int NrStrate,int NbRatioConsomation)
{
	int i;
	// Combien as-t-on d'individu dans cette cible ?
	double popcible=0;
	for(i=0;i<m_NbIndiv;i++)
	{
		popcible+=CoeffEx[i];
	}

	// Le groupe est les strates sont inversés
	int NrGroupe=NbRatioConsomation-NrStrate-1;

	// Il faut faire des groupe
	double poppargroupe=popcible/NbRatioConsomation;


	// Initialisation du vecteur résultat
	for(i=0;i<m_NbIndiv;i++)
		m_FiltreIndividus[i]=0;

	// On passe maintenant tous les individus, et on 
	double NbIndivTrouve=0;
	for(i=0;i<m_NbIndiv;i++)
	{
		int NrI=m_NrIndivTrie[i];
		if(CoeffEx[NrI]>0)
		{
			int groupe=(int)(NbIndivTrouve/poppargroupe);
			ASSERT(groupe<NbRatioConsomation);
			if(groupe==NrGroupe)
			{
				m_FiltreIndividus[NrI]=1.0;
			}
			NbIndivTrouve+=CoeffEx[NrI];
		}
	}

#ifdef _DEBUG
	double conso=0;
	double coeffex=0;
	int nbcas=0;
	for(i=0;i<m_NbIndiv;i++)
	{
		if(m_FiltreIndividus[i]>0)
		{
			conso+=m_Consomation[i]*CoeffEx[i];
			coeffex+=CoeffEx[i];
			nbcas++;
		}
	}
	conso/=coeffex;
	TRACE("Consomation moyenne du groupe %d (%d cas, %.0f individus extrapollé): %.2f%%\n",NrStrate,nbcas,coeffex,conso);
#endif
}


int CTableConsomationTotalTV::GetPlusGrosConsommateur(int rang)
{
	return m_NrIndivTrie[rang];

}

int CTableConsomationTotalTV::GetPlusPetitConsomateur(int rang)
{
	return m_NrIndivTrie[m_NbIndiv-rang-1];
}

double CTableConsomationTotalTV::GetTEI(int individu)
{
	return (m_Consomation[individu]);

}
