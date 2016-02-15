// TomyMediadata.h: interface for the CTomyMediadata class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TOMYMEDIADATA_H__E545E707_428D_4E77_8FBE_9942CFEFA199__INCLUDED_)
#define AFX_TOMYMEDIADATA_H__E545E707_428D_4E77_8FBE_9942CFEFA199__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "tomy.h"
#include "TableConsomationTotalTV.h"

class CTomyMediadata : public CTomy  
{
public:
	double CalculProportionPMGDansOffreElargie();
	void CalculPopulationInitialiseEtNonInitialise();
	void SetFiltreIndividuTous(bool flagRecalculDistributionExactementTouche=false);
	void SetFiltreIndividuNonInitialise(bool flagRecalculDistributionExactementTouche=false);
	void SetFiltreIndividuInitialise(bool flagRecalculDistributionExactementTouche=false);
	CTomyMediadata();
	virtual ~CTomyMediadata();
	void InitialisePanel(LPCSTR panel,LPCSTR etude);
	void FreePanel();
	double CalculGRP_IndividuInitialise(short Station,short JourNomme,short QHeure,double GRP);
	double GetLastGRPindividuInitialise(){return m_GRPInitialise;};

	double GetPopulationTous(){return (double)m_PopulationTous;};
	double GetPopulationInitialise(){return (double)m_PopulationInitialise;};
	double GetPopulationNonInitialise(){return (double)m_PopulationNonInitialise;};

	void ActiveFiltreIndividusStrateConsomation(int NrStrate);
	void InitialiseCible(int NrCible);
	void InitialiseCibleVentile(int NrCible,int NrCritere1 ,int NrModalite1,int NrCritere2,int NrModalite2);

private:
	void RecalculGRPPlan();

	double m_ProportionDernierGroupePMGsurOffreElargie;
	void FreeFiltre();
	void LoadCritereOffreElargie();
	double * m_FiltreIndividuInitialiseOffreElargie;
	double * m_FiltreIndividuNonInitialiseOffreElargie;
	int m_NrCritereOffreElargie;

	double m_GRPInitialise;
	double m_GRPNonInitialise;

	int m_PopulationInitialise;
	int m_PopulationNonInitialise;
	int m_PopulationTous;

	CTableConsomationTotalTV m_TotalTV;

	enum FiltreIndividu               // Declare enum type FiltreIndividu
	{
		IndividuTous,
		IndividuInitialise,
		IndividuNonInitialise
	} m_DernierFiltre;

	int m_DernierFiltreStrateConsomation;
};

#endif // !defined(AFX_TOMYMEDIADATA_H__E545E707_428D_4E77_8FBE_9942CFEFA199__INCLUDED_)
