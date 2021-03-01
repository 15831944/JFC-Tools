#if !defined(AFX_ATPResult_GrandeursHypo_H__35793E75_8DA7_4AA5_8DF2_341F557BE867__INCLUDED_)
#define AFX_ATPResult_GrandeursHypo_H__35793E75_8DA7_4AA5_8DF2_341F557BE867__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "JATPDocCreuset.h"

class CATPResult_GrandeursHypo  
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
	BOOL CATPResult_GrandeursHypo::IsBudget      (LONG Index);
	BOOL CATPResult_GrandeursHypo::IsNbIndividus (LONG Index);
	BOOL CATPResult_GrandeursHypo::IsNbContacts  (LONG Index);
	BOOL CATPResult_GrandeursHypo::IsNbCtsPlus   (LONG Index);
	BOOL CATPResult_GrandeursHypo::IsGRP         (LONG Index);
	BOOL CATPResult_GrandeursHypo::IsXGRP        (LONG Index);
	BOOL CATPResult_GrandeursHypo::IsCouverture  (LONG Index);
	BOOL CATPResult_GrandeursHypo::IsRepetition  (LONG Index);
	BOOL CATPResult_GrandeursHypo::IsRepetPlus  (LONG Index);
	BOOL CATPResult_GrandeursHypo::IsCouvMem     (LONG Index);
	BOOL CATPResult_GrandeursHypo::IsCoutGRP     (LONG Index);
	BOOL CATPResult_GrandeursHypo::IsCout1000Ind (LONG Index);
	BOOL CATPResult_GrandeursHypo::IsCout1000Cts (LONG Index);
	BOOL CATPResult_GrandeursHypo::IsCoutCouv (LONG Index);


private:
	JATPDocCreuset* m_pATPDocCreuset; // le document
};

#endif // !defined(AFX_ATPResult_GrandeursHypo_H__35793E75_8DA7_4AA5_8DF2_341F557BE867__INCLUDED_)
