// on inclut les définitions nécessaires
#include "stdafx.h"
#include "ATPResult_Grandeurs.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

JVoid CATPResult_Grandeurs::SetDocument(JATPDocCreuset* pATPDocCreuset)
{
	// on mémorise le document
	m_pATPDocCreuset = pATPDocCreuset;
}

////////////////////////////////////////////////////
// la fonction pour récupérer le nombre de grandeurs

LONG CATPResult_Grandeurs::GetNbGrandeurs()
{
	//m_nbFormat = 0;
	//m_pATPDocCreuset->m_Offre.m_TBLIdFormats


	//for(m_pATPDocCreuset->m_MapIdFormatsSel.MoveFirst();
	//	m_pATPDocCreuset->m_MapIdFormatsSel.IsValid();
	//	m_pATPDocCreuset->m_MapIdFormatsSel.MoveNext())
	//{
	//	if(m_pATPDocCreuset->m_MapIdFormatsSel.GetItem() != 0)
	//		m_nbFormat++;
	//}
	m_nbFormat = m_pATPDocCreuset->m_MapIdFormatsSel.GetCount();
	if(m_nbFormat == 1)
		m_nbFormat = 0;

	if(m_pATPDocCreuset->m_MoteurPresse.GetType() == CMoteurPresse::PROBA)
		return 11 + m_nbFormat;
	return 14 + m_nbFormat;
}

// Nombre de grandeur distri.fréquence
LONG CATPResult_Grandeurs::GetNbGrandeursDF()
{
	return 21;
}

//////////////////////////////////////////
// les fonctions pour tester les grandeurs

BOOL CATPResult_Grandeurs::IsBudget      (LONG Index)
{
	if(m_pATPDocCreuset->m_MoteurPresse.GetType() == CMoteurPresse::PROBA)
		return ((Index == 0) ? TRUE : FALSE);
	return ((Index == 0) ? TRUE : FALSE);
}

BOOL CATPResult_Grandeurs::IsBudgetFormat(LONG Index)
{
	if(1 <= Index && Index <= m_nbFormat)
		return TRUE;

	return FALSE;
}

BOOL CATPResult_Grandeurs::IsNbIndividus (LONG Index)
{
	if(m_pATPDocCreuset->m_MoteurPresse.GetType() == CMoteurPresse::PROBA)
		return ((Index == 1 + m_nbFormat) ? TRUE : FALSE);
	return ((Index == 1 + m_nbFormat) ? TRUE : FALSE);
}
BOOL CATPResult_Grandeurs::IsNbContacts  (LONG Index)
{
	if(m_pATPDocCreuset->m_MoteurPresse.GetType() == CMoteurPresse::PROBA)
		return ((Index == 2 + m_nbFormat) ? TRUE : FALSE);
	return ((Index == 2 + m_nbFormat) ? TRUE : FALSE);
}
BOOL CATPResult_Grandeurs::IsNbCtsPlus   (LONG Index)
{
	if(m_pATPDocCreuset->m_MoteurPresse.GetType() == CMoteurPresse::PROBA)
		return FALSE;
	return ((Index == 3 + m_nbFormat) ? TRUE : FALSE);
}
BOOL CATPResult_Grandeurs::IsGRP         (LONG Index)
{
	if(m_pATPDocCreuset->m_MoteurPresse.GetType() == CMoteurPresse::PROBA)
		return ((Index == 3 + m_nbFormat) ? TRUE : FALSE);
	return ((Index == 4 + m_nbFormat) ? TRUE : FALSE);
}
BOOL CATPResult_Grandeurs::IsXGRP        (LONG Index)
{
	if(m_pATPDocCreuset->m_MoteurPresse.GetType() == CMoteurPresse::PROBA)
		return FALSE;
	return ((Index == 5 + m_nbFormat) ? TRUE : FALSE);
}
BOOL CATPResult_Grandeurs::IsCouverture  (LONG Index)
{
	if(m_pATPDocCreuset->m_MoteurPresse.GetType() == CMoteurPresse::PROBA)
		return ((Index == 4 + m_nbFormat) ? TRUE : FALSE);
	return ((Index == 6 + m_nbFormat) ? TRUE : FALSE);
}
BOOL CATPResult_Grandeurs::IsRepetition  (LONG Index)
{
	if(m_pATPDocCreuset->m_MoteurPresse.GetType() == CMoteurPresse::PROBA)
		return ((Index == 5 + m_nbFormat) ? TRUE : FALSE);
	return ((Index == 7 + m_nbFormat) ? TRUE : FALSE);
}
BOOL CATPResult_Grandeurs::IsRepetPlus  (LONG Index)
{
	if(m_pATPDocCreuset->m_MoteurPresse.GetType() == CMoteurPresse::PROBA)
		return FALSE;
	return ((Index == 8 + m_nbFormat) ? TRUE : FALSE);
}
BOOL CATPResult_Grandeurs::IsCouvMem     (LONG Index)
{
	if(m_pATPDocCreuset->m_MoteurPresse.GetType() == CMoteurPresse::PROBA)
		return ((Index == 6 + m_nbFormat) ? TRUE : FALSE);
	return ((Index == 9 + m_nbFormat) ? TRUE : FALSE);
}
BOOL CATPResult_Grandeurs::IsCoutGRP     (LONG Index)
{
	if(m_pATPDocCreuset->m_MoteurPresse.GetType() == CMoteurPresse::PROBA)
		return ((Index == 7 + m_nbFormat) ? TRUE : FALSE);
	return ((Index == 10 + m_nbFormat) ? TRUE : FALSE);
}
BOOL CATPResult_Grandeurs::IsCoutCouv (LONG Index)
{
	if(m_pATPDocCreuset->m_MoteurPresse.GetType() == CMoteurPresse::PROBA)
		return ((Index == 8 + m_nbFormat) ? TRUE : FALSE);
	return ((Index == 11 + m_nbFormat) ? TRUE : FALSE);
}
BOOL CATPResult_Grandeurs::IsCout1000Ind (LONG Index)
{
	if(m_pATPDocCreuset->m_MoteurPresse.GetType() == CMoteurPresse::PROBA)
		return ((Index == 9 + m_nbFormat) ? TRUE : FALSE);
	return ((Index == 12 + m_nbFormat) ? TRUE : FALSE);
}
BOOL CATPResult_Grandeurs::IsCout1000Cts (LONG Index)
{
	if(m_pATPDocCreuset->m_MoteurPresse.GetType() == CMoteurPresse::PROBA)
		return ((Index == 10 + m_nbFormat) ? TRUE : FALSE);
	return ((Index == 13 + m_nbFormat) ? TRUE : FALSE);
}


