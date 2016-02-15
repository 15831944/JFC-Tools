// TomyConsolide.cpp: implementation of the CTomyConsolide class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "TomyConsolide.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CTomyConsolide::CTomyConsolide()
{

	m_Param=NULL;
}

CTomyConsolide::~CTomyConsolide()
{

}

void CTomyConsolide::InitialiseCible(int NrCible)
{
	for(int e=0;e<m_TableTomy.GetSize();e++)
		m_TableTomy[e]->InitialiseCible(NrCible);
}

void CTomyConsolide::InitialiseCibleVentile(int NrCible,int NrCritere1 ,int NrModalite1,int NrCritere2,int NrModalite2)
{
	for(int e=0;e<m_TableTomy.GetSize();e++)
		m_TableTomy[e]->InitialiseCibleVentile(NrCible,NrCritere1,NrModalite1,NrCritere2,NrModalite2);
}


void CTomyConsolide::SetParametres(CTomyParam * param)
{
	m_Param=param;

	for(int t=0;t<m_TableTomy.GetSize();t++)
	{
		m_TableTomy[t]->SetParametres(m_Param);
	}
}

int CTomyConsolide::GetNbCas()
{
	int result=0;
	for(int e=0;e<m_TableTomy.GetSize();e++)
		result+=m_TableTomy[e]->GetNbCas();
	return result;
}

int CTomyConsolide::GetPopulation()
{
	int result=0;
	for(int e=0;e<m_TableTomy.GetSize();e++)
		result+=m_TableTomy[e]->GetPopulation();
	return result;
}

double CTomyConsolide::GetCouverture()
{
	double sigma=0;
	double sigmapop=0;
	for(int e=0;e<m_TableTomy.GetSize();e++)
	{
		double pop=m_TableTomy[e]->GetPopulation();
		sigmapop+=pop;
		sigma+=m_TableTomy[e]->GetCouverture()*pop;
	}
	if(sigmapop>0)
		return(sigma/sigmapop);
	else return 0;
}

double CTomyConsolide::GetAsymptote()
{
	double sigma=0;
	double sigmapop=0;
	for(int e=0;e<m_TableTomy.GetSize();e++)
	{
		double pop=m_TableTomy[e]->GetPopulation();
		sigmapop+=pop;
		sigma+=m_TableTomy[e]->GetAsymptote()*pop;
	}
	if(sigmapop>0)
		return(sigma/sigmapop);
	else return 0;
}


void CTomyConsolide::InitialiseSpot()
{
	for(int e=0;e<m_TableTomy.GetSize();e++)
		m_TableTomy[e]->InitialiseSpot();

}

double CTomyConsolide::LanceEvalDistrib()
{
	for(int e=0;e<m_TableTomy.GetSize();e++)
		m_TableTomy[e]->LanceEvalDistrib();
	return(GetCouverture());
}


double CTomyConsolide::LanceEval()
{
	for(int e=0;e<m_TableTomy.GetSize();e++)
		m_TableTomy[e]->LanceEval();
	return(GetCouverture());
}


void CTomyConsolide::AddSpot(short Station,short JourNomme,short QHeure,double GRP,short NbInsertion/*=1*/)
{
	AfxMessageBox("Fc INTERDITE !utilser AddSpotDansJob");
	ASSERT(0);
}

void CTomyConsolide::UpdateSpot(short Station, short JourNomme,short QHeure,double GRP,short NbInsertion)
{
	AfxMessageBox("Fc INTERDITE ! utiliser UpdateSpotDansJob");
	ASSERT(0);
}

void CTomyConsolide::UpdateSpotDansJob(short NrJob,short Station, short JourNomme,short QHeure,double GRP,short NbInsertion)
{
	m_TableTomy[NrJob]->UpdateSpot(Station, JourNomme,QHeure,GRP,NbInsertion);
}
void CTomyConsolide::AddSpotDansJob(short NrJob,short Station,short JourNomme,short QHeure,double GRP,short NbInsertion/*=1*/)
{
	ASSERT(GRP>=0);
	m_TableTomy[NrJob]->AddSpot(Station,JourNomme,QHeure,GRP,NbInsertion);
}

void CTomyConsolide::InitialiseJob()
{
	m_TableTomy.RemoveAll();
}

int CTomyConsolide::AddJob(CTomy *pTomy)
{
	if(m_Param)
		pTomy->SetParametres(m_Param);
	return(m_TableTomy.Add(pTomy));
}


int CTomyConsolide::GetNbSpot()
{
	int nb=0;
	for(int e=0;e<m_TableTomy.GetSize();e++)
		nb+=m_TableTomy[e]->GetNbSpot();
	return nb;
}

double CTomyConsolide::GetCouvertureValideBeta()
{
	double sigma=0;
	double sigmapop=0;
	for(int e=0;e<m_TableTomy.GetSize();e++)
	{
		double pop=m_TableTomy[e]->GetPopulation();
		sigmapop+=pop;
		sigma+=m_TableTomy[e]->GetCouvertureValideBeta()*pop;
	}
	if(sigmapop>0)
		return(sigma/sigmapop);
	else return 0;
}

double CTomyConsolide::GetCouvertureValideMinMax()
{
	double sigma=0;
	double sigmapop=0;
	for(int e=0;e<m_TableTomy.GetSize();e++)
	{
		double pop=m_TableTomy[e]->GetPopulation();
		sigmapop+=pop;
		sigma+=m_TableTomy[e]->GetCouvertureValideMinMax()*pop;
	}
	if(sigmapop>0)
		return(sigma/sigmapop);
	else return 0;
}

double CTomyConsolide::GetGRP()
{
	double sigma=0;
	double sigmapop=0;
	for(int e=0;e<m_TableTomy.GetSize();e++)
	{
		double pop=m_TableTomy[e]->GetPopulation();
		sigmapop+=pop;
		sigma+=m_TableTomy[e]->GetGRP()*pop;
	}
	if(sigmapop>0)
		return(sigma/sigmapop);
	else return 0;
}

int CTomyConsolide::GetNbClasseResultat()
{
	if(m_TableTomy.GetSize()<=0)return 0;
	int max=m_TableTomy[0]->GetNbClasseResultat();
	for(int e=1;e<m_TableTomy.GetSize();e++)
	{
		if(m_TableTomy[e]->GetNbClasseResultat()>max)max=m_TableTomy[e]->GetNbClasseResultat();
	}
	return(max);
}

double CTomyConsolide::GetExactementTouche(int categorie)
{
		double sigma=0;
	double sigmapop=0;
	for(int e=0;e<m_TableTomy.GetSize();e++)
	{
		double pop=m_TableTomy[e]->GetPopulation();
		sigmapop+=pop;
		sigma+=m_TableTomy[e]->GetExactementTouche(categorie)*pop;
	}
	if(sigmapop>0)
		return(sigma/sigmapop);
	else return 0;
}

double CTomyConsolide::GetCourbeReponse(int nbfoistouche)
{
	ASSERT(m_TableTomy.GetSize()>=1);
	return(m_TableTomy[0]->GetCourbeReponse(nbfoistouche));
}

void CTomyConsolide::ActiveFiltreIndividusStrateConsomation(int NrStrate)
{	
	for(int e=0;e<m_TableTomy.GetSize();e++)
	{
		m_TableTomy[e]->ActiveFiltreIndividusStrateConsomation(NrStrate);
	}
}

void CTomyConsolide::SavePlanBeforeZoom()
{
	for(int e=0;e<m_TableTomy.GetSize();e++)
	{
		m_TableTomy[e]->SavePlanBeforeZoom();
	}
}

void CTomyConsolide::RestorePlanAfterZoom()
{
	for(int e=0;e<m_TableTomy.GetSize();e++)
	{
		m_TableTomy[e]->RestorePlanAfterZoom();
	}
}

long CTomyConsolide::GetNbInsertions()
{
	return 0;
}










