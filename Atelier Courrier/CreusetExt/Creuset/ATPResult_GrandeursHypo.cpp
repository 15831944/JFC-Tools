// on inclut les définitions nécessaires
#include "stdafx.h"
#include "ATPResult_GrandeursHypo.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

JVoid CATPResult_GrandeursHypo::SetDocument(JATPDocCreuset* pATPDocCreuset)
{
	// on mémorise le document
	m_pATPDocCreuset = pATPDocCreuset;
}

////////////////////////////////////////////////////
// la fonction pour récupérer le nombre de grandeurs

LONG CATPResult_GrandeursHypo::GetNbGrandeurs()
{
	if(m_pATPDocCreuset->m_AFFCreuset.GetAffQuickPlan())
		return 6;

	if(m_pATPDocCreuset->m_MoteurPresse.GetType() == CMoteurPresse::PROBA)
		return 11;

	return 14;
}

// Nombre de grandeur distri.fréquence
LONG CATPResult_GrandeursHypo::GetNbGrandeursDF()
{
	return 21;
}

//////////////////////////////////////////
// les fonctions pour tester les grandeurs

BOOL CATPResult_GrandeursHypo::IsBudget      (LONG Index)
{
	if(m_pATPDocCreuset->m_AFFCreuset.GetAffQuickPlan())
		return ((Index == 0) ? TRUE : FALSE);
	if(m_pATPDocCreuset->m_MoteurPresse.GetType() == CMoteurPresse::PROBA)
		return ((Index == 0) ? TRUE : FALSE);
	return ((Index == 0) ? TRUE : FALSE);
}



BOOL CATPResult_GrandeursHypo::IsNbIndividus (LONG Index)
{
	if(m_pATPDocCreuset->m_AFFCreuset.GetAffQuickPlan())
		return ((Index == 1) ? TRUE : FALSE);
	if(m_pATPDocCreuset->m_MoteurPresse.GetType() == CMoteurPresse::PROBA)
		return ((Index == 1) ? TRUE : FALSE);
	return ((Index == 1) ? TRUE : FALSE);
}
BOOL CATPResult_GrandeursHypo::IsNbContacts  (LONG Index)
{
	if(m_pATPDocCreuset->m_AFFCreuset.GetAffQuickPlan())
		return  FALSE;
	if(m_pATPDocCreuset->m_MoteurPresse.GetType() == CMoteurPresse::PROBA)
		return ((Index == 2) ? TRUE : FALSE);
	return ((Index == 2) ? TRUE : FALSE);
}
BOOL CATPResult_GrandeursHypo::IsNbCtsPlus   (LONG Index)
{
	if(m_pATPDocCreuset->m_AFFCreuset.GetAffQuickPlan())
		return FALSE;
	if(m_pATPDocCreuset->m_MoteurPresse.GetType() == CMoteurPresse::PROBA)
		return FALSE;
	return ((Index == 3) ? TRUE : FALSE);
}
BOOL CATPResult_GrandeursHypo::IsGRP         (LONG Index)
{
	if(m_pATPDocCreuset->m_AFFCreuset.GetAffQuickPlan())
		return ((Index == 2) ? TRUE : FALSE);
	if(m_pATPDocCreuset->m_MoteurPresse.GetType() == CMoteurPresse::PROBA)
		return ((Index == 3) ? TRUE : FALSE);
	return ((Index == 4) ? TRUE : FALSE);
}
BOOL CATPResult_GrandeursHypo::IsXGRP        (LONG Index)
{
	if(m_pATPDocCreuset->m_AFFCreuset.GetAffQuickPlan())
		return FALSE;
	if(m_pATPDocCreuset->m_MoteurPresse.GetType() == CMoteurPresse::PROBA)
		return FALSE;
	return ((Index == 5) ? TRUE : FALSE);
}
BOOL CATPResult_GrandeursHypo::IsCouverture  (LONG Index)
{
	if(m_pATPDocCreuset->m_AFFCreuset.GetAffQuickPlan())
		return ((Index == 3) ? TRUE : FALSE);
	if(m_pATPDocCreuset->m_MoteurPresse.GetType() == CMoteurPresse::PROBA)
		return ((Index == 4) ? TRUE : FALSE);
	return ((Index == 6) ? TRUE : FALSE);
}
BOOL CATPResult_GrandeursHypo::IsRepetition  (LONG Index)
{
	if(m_pATPDocCreuset->m_AFFCreuset.GetAffQuickPlan())
		return ((Index == 4) ? TRUE : FALSE);
	if(m_pATPDocCreuset->m_MoteurPresse.GetType() == CMoteurPresse::PROBA)
		return ((Index == 5) ? TRUE : FALSE);
	return ((Index == 7) ? TRUE : FALSE);
}
BOOL CATPResult_GrandeursHypo::IsRepetPlus  (LONG Index)
{
	if(m_pATPDocCreuset->m_AFFCreuset.GetAffQuickPlan())
		return FALSE;
	if(m_pATPDocCreuset->m_MoteurPresse.GetType() == CMoteurPresse::PROBA)
		return FALSE;
	return ((Index == 8) ? TRUE : FALSE);
}
BOOL CATPResult_GrandeursHypo::IsCouvMem     (LONG Index)
{
	if(m_pATPDocCreuset->m_AFFCreuset.GetAffQuickPlan())
		return FALSE;
	if(m_pATPDocCreuset->m_MoteurPresse.GetType() == CMoteurPresse::PROBA)
		return ((Index == 6) ? TRUE : FALSE);
	return ((Index == 9) ? TRUE : FALSE);
}
BOOL CATPResult_GrandeursHypo::IsCoutGRP     (LONG Index)
{
	if(m_pATPDocCreuset->m_AFFCreuset.GetAffQuickPlan())
		return ((Index == 5) ? TRUE : FALSE);
	if(m_pATPDocCreuset->m_MoteurPresse.GetType() == CMoteurPresse::PROBA)
		return ((Index == 7) ? TRUE : FALSE);
	return ((Index == 10) ? TRUE : FALSE);
}
BOOL CATPResult_GrandeursHypo::IsCoutCouv (LONG Index)
{
	if(m_pATPDocCreuset->m_AFFCreuset.GetAffQuickPlan())
		return FALSE;
	if(m_pATPDocCreuset->m_MoteurPresse.GetType() == CMoteurPresse::PROBA)
		return ((Index == 8) ? TRUE : FALSE);
	return ((Index == 11) ? TRUE : FALSE);
}
BOOL CATPResult_GrandeursHypo::IsCout1000Ind (LONG Index)
{
	if(m_pATPDocCreuset->m_AFFCreuset.GetAffQuickPlan())
		return FALSE;
	if(m_pATPDocCreuset->m_MoteurPresse.GetType() == CMoteurPresse::PROBA)
		return ((Index == 9) ? TRUE : FALSE);
	return ((Index == 12) ? TRUE : FALSE);
}
BOOL CATPResult_GrandeursHypo::IsCout1000Cts (LONG Index)
{
	if(m_pATPDocCreuset->m_AFFCreuset.GetAffQuickPlan())
		return FALSE;
	if(m_pATPDocCreuset->m_MoteurPresse.GetType() == CMoteurPresse::PROBA)
		return ((Index == 10) ? TRUE : FALSE);
	return ((Index == 13) ? TRUE : FALSE);
}


