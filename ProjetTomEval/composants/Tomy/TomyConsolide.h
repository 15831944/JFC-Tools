// TomyConsolide.h: interface for the CTomyConsolide class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TOMYCONSOLIDE_H__511EB26A_3BE9_4E29_A325_45705EECDBD2__INCLUDED_)
#define AFX_TOMYCONSOLIDE_H__511EB26A_3BE9_4E29_A325_45705EECDBD2__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//#include "..\COMPOSANTS\TOMY\tomy.h"
#include "tomy.h"

class CTomyConsolide : public InterfaceTomy  
{
public:
	int AddJob(CTomy * pTomy);
	void InitialiseJob();
	CTomyConsolide();
	virtual ~CTomyConsolide();

	// Implémentation de l'interface Tomy
	void SetParametres(CTomyParam * param);
	virtual	void InitialisePanel(LPCSTR panel,LPCSTR etude)=0;
	void InitialiseCible(int NrCible);
	void InitialiseCibleVentile(int NrCible,int NrCritere1 ,int NrModalite1,int NrCritere2=-1,int NrModalite2=-1);

	int GetNbCas();
	int GetPopulation();
	void InitialiseSpot();
	double LanceEvalDistrib();
	double GetAsymptote();
	void AddSpot(short Station,short JourNomme,short QHeure,double GRP,short NbInsertion=1);
	void AddSpotDansJob(short NrJob,short Station,short JourNomme,short QHeure,double GRP,short NbInsertion=1);
	void UpdateSpot(short Station, short JourNomme,short QHeure,double GRP,short NbInsertion);
	void UpdateSpotDansJob(short NrJob,short Station, short JourNomme,short QHeure,double GRP,short NbInsertion);

	double GetCouverture();
	double GetCouvertureValideBeta();
	int GetNbSpot();
	double GetCouvertureValideMinMax();
	double GetGRP();
	int GetNbClasseResultat();
	double GetExactementTouche(int categorie);
	double GetCourbeReponse(int nbfoistouche);
	void ActiveFiltreIndividusStrateConsomation(int NrStrate);
	double LanceEval();
	void SavePlanBeforeZoom();
	void RestorePlanAfterZoom();
	long GetNbInsertions();

	// -------------------------------------------------------------------------------

protected:
	CTomyParam * m_Param;
	CArray <CTomy*,CTomy*>m_TableTomy;
};

#endif // !defined(AFX_TOMYCONSOLIDE_H__511EB26A_3BE9_4E29_A325_45705EECDBD2__INCLUDED_)
