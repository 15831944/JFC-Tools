#if !defined(AFX_ATPResult_Grandeurs_H__35793E75_8DA7_4AA5_8DF2_341F557BE867__INCLUDED_)
#define AFX_ATPResult_Grandeurs_H__35793E75_8DA7_4AA5_8DF2_341F557BE867__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "JATPDocCreuset.h"

class CATPResult_Grandeurs  
{
public:
	// la fonction pour récupérer le nombre de grandeurs
	LONG GetNbGrandeurs();
	// Nombre de grandeur distri.fréquence
	LONG GetNbGrandeursDF();

	// la fonction pour définir le document
	virtual JVoid SetDocument(JATPDocCreuset* pATPDocCreuset);


public:
	// les fonctions pour tester les grandeurs
	BOOL CATPResult_Grandeurs::IsBudget      (LONG Index);
	BOOL CATPResult_Grandeurs::IsBudgetFormat(LONG Index);
	BOOL CATPResult_Grandeurs::IsNbIndividus (LONG Index);
	BOOL CATPResult_Grandeurs::IsNbContacts  (LONG Index);
	BOOL CATPResult_Grandeurs::IsNbCtsPlus   (LONG Index);
	BOOL CATPResult_Grandeurs::IsGRP         (LONG Index);
	BOOL CATPResult_Grandeurs::IsXGRP        (LONG Index);
	BOOL CATPResult_Grandeurs::IsCouverture  (LONG Index);
	BOOL CATPResult_Grandeurs::IsRepetition  (LONG Index);
	BOOL CATPResult_Grandeurs::IsRepetPlus  (LONG Index);
	BOOL CATPResult_Grandeurs::IsCouvMem     (LONG Index);
	BOOL CATPResult_Grandeurs::IsCoutGRP     (LONG Index);
	BOOL CATPResult_Grandeurs::IsCout1000Ind (LONG Index);
	BOOL CATPResult_Grandeurs::IsCout1000Cts (LONG Index);
	BOOL CATPResult_Grandeurs::IsCoutCouv (LONG Index);


private:
	JATPDocCreuset* m_pATPDocCreuset; // le document
	int m_nbFormat;
};

#endif // !defined(AFX_ATPResult_Grandeurs_H__35793E75_8DA7_4AA5_8DF2_341F557BE867__INCLUDED_)
