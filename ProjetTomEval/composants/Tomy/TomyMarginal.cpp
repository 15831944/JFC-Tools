// TomyMarginal.cpp: implementation of the CTomyMarginal class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "TomyMarginal.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CTomyMarginal::CTomyMarginal()
: m_GRPavant(0)
{

}

CTomyMarginal::~CTomyMarginal()
{

}

double CTomyMarginal::GetApportGRP(void)
{
	return GetGRP()-m_GRPavant;
}
double CTomyMarginal::GetApportCouverture(void)
{
	return GetCouverture()-m_CouvertureAvant;
}

double CTomyMarginal::GetApportExactementTouche(int ClasseContact)
{
	return GetExactementTouche(ClasseContact)-m_DistribAvant.GetValue(ClasseContact);
}

void CTomyMarginal::PrepareCalculZoom(void)
{
	m_GRPavant=GetGRP();
	m_DistribAvant=GetDistributionExactementTouche();
	m_CouvertureAvant=GetCouverture();
	//SavePlanBeforeZoom();
}

void CTomyMarginal::TermineCalculZoom(void)
{
	//RestorePlanAfterZoom();
}

void CTomyMarginal::LanceEvalCalculMarginal(short Station,short JourNomme,short QHeure,double GRP,short NbInsertion/*=1*/)
{
	// PrepareCalculZoom();
	UpdateSpot(Station,JourNomme,QHeure,GRP,NbInsertion);
	LanceEval();
	UpdateSpot(Station,JourNomme,QHeure,GRP,-NbInsertion);
	//RestorePlanAfterZoom();
}
void CTomyMarginal::LanceEvalDistribCalculMarginal(short Station,short JourNomme,short QHeure,double GRP,short NbInsertion/*=1*/)
{
	//PrepareCalculZoom();
	UpdateSpot(Station,JourNomme,QHeure,GRP,NbInsertion);
	LanceEvalDistrib();
	UpdateSpot(Station,JourNomme,QHeure,GRP,-NbInsertion);
	//RestorePlanAfterZoom();
}




