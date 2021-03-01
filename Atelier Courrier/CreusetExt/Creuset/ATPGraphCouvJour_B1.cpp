// on inclut les définitions nécessaires
#include "stdafx.h"
#include "JFCGraduation.h"
#include "ATPGraphCouvJour_B1.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//////////////////
// le constructeur

CATPGraphCouvJour_B1::CATPGraphCouvJour_B1()
{
}

void CATPGraphCouvJour_B1::CalcValues(const JITPDATHypoFormat & DATHypoFormat, int i, JFlt64 &Value)
{
	if(m_pATPDocCreuset
		&& m_pATPDocCreuset->IsValid()
		&& m_pATPDocCreuset->m_MoteurPresse.GetType() != CMoteurPresse::PROBA)
	{
		// on récupère l'effectif et le nombre de contacts
		JFlt64 Effectif   = DATHypoFormat.m_Effectif;
		if (Effectif > 0.0)
		{
			const JITPDATHypothese & DATHypothese = DATHypoFormat.GetDATHypoFormatActif();
			Value = 100.0 * DATHypothese.m_DistributionJour.Item(i).Item(1).AsJFlt64() / Effectif;
			JFlt64 couv1 = 0.0;
			if (i>0)
				couv1 = 100.0 * DATHypothese.m_DistributionJour.Item(i-1).Item(1).AsJFlt64() / Effectif;
			// on calcule la couv
			Value -= couv1;
		}
	}
}
// le destructeur

CATPGraphCouvJour_B1::~CATPGraphCouvJour_B1()
{
	// on ne fait rien
}

