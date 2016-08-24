// ResultatPerformance.h: interface for the CResultatPerformance class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RESULTATPERFORMANCE_H__311F78C0_0C58_45F4_967D_AAC348CE02E9__INCLUDED_)
#define AFX_RESULTATPERFORMANCE_H__311F78C0_0C58_45F4_967D_AAC348CE02E9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CResultatPerformance  
{
public:
	CResultatPerformance();
	virtual ~CResultatPerformance();

	// Budget
	double BudgetTot[NBHYPOTHESE];
	long BudgetNet[NBHYPOTHESE];

	//Cible
	double BasePopulation[NBCIBLECALCUL];
	int NbCas[NBCIBLECALCUL];

	// Contacts
	double	NbContactTot[NBCIBLECALCUL][NBHYPOTHESE];
	double GRP(int Cible,int Hypothese){return 100000*NbContactTot[Cible][Hypothese]/BasePopulation[Cible];};
	double CPMbrut(int Cible,int Hypothese){return BudgetTot[Hypothese]/NbContactTot[Cible][Hypothese];};
	double CPMnet(int Cible,int Hypothese){return BudgetNet[Hypothese]/NbContactTot[Cible][Hypothese];};

	// Couverture
	double NbIndivTouchesTot[NBCIBLECALCUL][NBHYPOTHESE]; // Nombre d'individus ayant été touché au moins une fois, en millier
	double Couverture(int Cible,int Hypothese){return 100000*NbIndivTouchesTot[Cible][Hypothese]/BasePopulation[Cible];};
	double Repetition(int Cible,int Hypothese){return NbContactTot[Cible][Hypothese]/NbIndivTouchesTot[Cible][Hypothese];};

	// Mémo-démémo
	double	NbIndivMemoTot[NBCIBLECALCUL][NBHYPOTHESE]; // Nombre d'individus  ayant mémorisé au moins une fois, en millier
	double MemoMoy[NBCIBLECALCUL][NBHYPOTHESE];
	double MemoNrSemaineX[NBCIBLECALCUL][NBHYPOTHESE];
	double PtMemoCampTot[NBCIBLECALCUL][NBHYPOTHESE]; // Points de mémo, pour les semaine "actives"
	double RatioMemo(int Cible,int Hypothese){return NbIndivMemoTot[Cible][Hypothese]/NbIndivTouchesTot[Cible][Hypothese];};
	











};

#endif // !defined(AFX_RESULTATPERFORMANCE_H__311F78C0_0C58_45F4_967D_AAC348CE02E9__INCLUDED_)
