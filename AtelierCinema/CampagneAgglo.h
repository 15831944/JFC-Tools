// CampagneAgglo.h: interface for the CCampagneAgglo class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CAMPAGNEAGGLO_H__30D736F8_7059_4009_B6BD_AA03FD494B8D__INCLUDED_)
#define AFX_CAMPAGNEAGGLO_H__30D736F8_7059_4009_B6BD_AA03FD494B8D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Campagne.h"

class CCampagneAgglo : public CCampagne  
{
public:
	void CalculPerformanceAgglo(int Hypothese);
	void CalculPerformanceMigre(int Hypothese);

	CCampagneAgglo();
	virtual ~CCampagneAgglo();
	void PrepareVecteurIndividusAgglo(int cible);
	void LanceCalcul(int bDebut);


	CResultatPerformance m_ResultatAgglo;
	CResultatPerformance m_ResultatMigre;

};

#endif // !defined(AFX_CAMPAGNEAGGLO_H__30D736F8_7059_4009_B6BD_AA03FD494B8D__INCLUDED_)
