// on inclut les définitions nécessaires
#include "stdafx.h"
#include "ATPEchelle_Grandeurs.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

////////////////////////////////////////////////////
// la fonction pour récupérer le nombre de grandeurs

LONG CATPEchelle_Grandeurs::GetNbGrandeurs()
{ 
	/*
	if(m_pATPDocCreuset->m_MoteurPresse.GetType() == CMoteurPresse::PROBA)
		return 5;
	return 7;
	*/
	if(m_pATPDocCreuset->m_MoteurPresse.GetType() == CMoteurPresse::PROBA)
		return 6;
	return 8;

}

//////////////////////////////////////////
// les fonctions pour tester les grandeurs

BOOL CATPEchelle_Grandeurs::IsXGRP         (LONG Index)
{
	if(m_pATPDocCreuset->m_MoteurPresse.GetType() == CMoteurPresse::PROBA)
		return FALSE;
	return ((Index == 0) ? TRUE : FALSE);
}

BOOL CATPEchelle_Grandeurs::IsGRP  (LONG Index)
{
	if(m_pATPDocCreuset->m_MoteurPresse.GetType() == CMoteurPresse::PROBA)
		return ((Index == 0) ? TRUE : FALSE);
	return ((Index == 1) ? TRUE : FALSE);
}

BOOL CATPEchelle_Grandeurs::IsIndAffinite    (LONG Index)
{
	if(m_pATPDocCreuset->m_MoteurPresse.GetType() == CMoteurPresse::PROBA)
		return ((Index == 1) ? TRUE : FALSE);
	return ((Index == 2) ? TRUE : FALSE);
}

BOOL CATPEchelle_Grandeurs::IsPrcAffinite    (LONG Index)
{
	if(m_pATPDocCreuset->m_MoteurPresse.GetType() == CMoteurPresse::PROBA)
		return ((Index == 2) ? TRUE : FALSE);
	return ((Index == 3) ? TRUE : FALSE);
}

BOOL CATPEchelle_Grandeurs::IsCoutGRP     (LONG Index)
{
	if(m_pATPDocCreuset->m_MoteurPresse.GetType() == CMoteurPresse::PROBA)
		return ((Index == 3) ? TRUE : FALSE);
	return ((Index == 4) ? TRUE : FALSE);
}

BOOL CATPEchelle_Grandeurs::IsRepetition  (LONG Index)
{
	if(m_pATPDocCreuset->m_MoteurPresse.GetType() == CMoteurPresse::PROBA)
		return FALSE;
	return ((Index == 5) ? TRUE : FALSE);
}

BOOL CATPEchelle_Grandeurs::IsNbIndividus (LONG Index)
{
	//if(m_pATPDocCreuset->m_MoteurPresse.GetType() == CMoteurPresse::PROBA)
	//	return ((Index == 4) ? TRUE : FALSE);
	//return ((Index == 6) ? TRUE : FALSE);
	return FALSE;
}

BOOL CATPEchelle_Grandeurs::IsNbContacts  (LONG Index)
{
	if(m_pATPDocCreuset->m_MoteurPresse.GetType() == CMoteurPresse::PROBA)
		return ((Index == 4) ? TRUE : FALSE);
	return ((Index == 6) ? TRUE : FALSE);
}

BOOL CATPEchelle_Grandeurs::IsApport  (LONG Index)
{
	if(m_pATPDocCreuset->m_MoteurPresse.GetType() == CMoteurPresse::PROBA)
		return ((Index == 5) ? TRUE : FALSE);
	return ((Index == 7) ? TRUE : FALSE);
}

JVoid CATPEchelle_Grandeurs::SetDocument(JATPDocCreuset* pATPDocCreuset)
{
	// on mémorise le document
	m_pATPDocCreuset = pATPDocCreuset;
}
