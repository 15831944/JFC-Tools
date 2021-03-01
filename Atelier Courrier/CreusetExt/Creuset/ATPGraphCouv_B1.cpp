// on inclut les définitions nécessaires
#include "stdafx.h"
#include "ATPGraphCouv_B1.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//////////////////
// le constructeur

CATPGraphCouv_B1::CATPGraphCouv_B1()
{
}
CATPGraphCouv_B1::~CATPGraphCouv_B1()
{
}

void CATPGraphCouv_B1::CalcValues(const JITPDATHypoFormat & DATHypoFormat, int idxHypo, int i, JFlt64 &A2, JFlt64 &A3, JFlt64 &A4)
{
	if(m_pATPDocCreuset
		&& m_pATPDocCreuset->IsValid()
		&& m_pATPDocCreuset->m_MoteurPresse.GetType() != CMoteurPresse::PROBA)
	{
		const JITPDATHypothese & DATHypothese = DATHypoFormat.GetDATHypoFormatActif();
		JFlt64 N3 = 0.0; 
		if (i > 0)
			N3 = DATHypothese.m_NbContactsJour.Item(i - 1);
		JFlt64 N4 = DATHypothese.m_NbContactsJour.Item(i);

		// on récupère les couvertures à 1+ de la veille
		if (i  > 0)   A2 = 100.0 * DATHypothese.m_DistributionJour.Item(i - 1).Item(1).AsJFlt64() / DATHypoFormat.m_Effectif;
		if (N3 > 0.0) A3 = 100.0 * DATHypothese.m_DistributionJour.Item(i - 1).Item(1).AsJFlt64() / DATHypoFormat.m_Effectif;
		if (N4 > 0.0) A4 = 100.0 * DATHypothese.m_DistributionJour.Item(i).Item(1).AsJFlt64() / DATHypoFormat.m_Effectif;
	}
}

