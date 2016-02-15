// TomyCroiseeMediacabsat.h: interface for the CTomyCroiseeMediacabsat class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TOMYCROISEEMEDIACABSAT_H__D952F451_703B_4F94_A62A_70273CC2C107__INCLUDED_)
#define AFX_TOMYCROISEEMEDIACABSAT_H__D952F451_703B_4F94_A62A_70273CC2C107__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "TomyCroisee.h"
#include "TableConsomationTotalTV.h"

class CTomyCroiseeMediacabsat : public CTomyCroisee  
{
public:
	void ActiveFiltreIndividusStrateConsomation(double ProportionDebut,double ProportionFin);
	void InitialiseSpot();
	void InitialiseCible(int NrCible);
	void SetPopulationCouvRepresentative(int pop);
	void RedresseDistributionHertzien(CDistribution & DistribHertzien);
	void RedresseDistributionCabsat(CDistribution &DistribCabsat);
	void RedresseDistributionCroisee(CDistribution &DistribHertzien,CDistribution &DistribCabsat);
	void RedresseGRPThematique(double coeffThematique);
	void EtalonneGRPHertzien(double GRPHertzien);
	int GetPopulation(){return m_PopulationRepresentative;};
	int GetPopulationSourceCouverture(){return m_Population;};

	CTomyCroiseeMediacabsat();
	virtual ~CTomyCroiseeMediacabsat();


	void AddSpot(short Station, short JourNomme,short QHeure,double GRP,short NbInsertion);
	void AddSpotDansDimension(short Station, short JourNomme,short QHeure,double GRP,short NbInsertion,BYTE DimensionMedia);

	void UpdateSpot(short Station, short JourNomme,short QHeure,double GRP,short NbInsertion);
	void UpdateSpotDansDimension(short Station, short JourNomme,short QHeure,double GRP,short NbInsertion,BYTE DimensionMedia);

private:
	void RecalculGRPPlan();

	int m_PopulationRepresentative; // Ne pas confondre avec la population qui sert à calculer les quotas
	double m_GRPHertzien;
	double m_GRPThematique;
	CTableConsomationTotalTV m_TotalTV;
};

#endif // !defined(AFX_TOMYCROISEEMEDIACABSAT_H__D952F451_703B_4F94_A62A_70273CC2C107__INCLUDED_)
