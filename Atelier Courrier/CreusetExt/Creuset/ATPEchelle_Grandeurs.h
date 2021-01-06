// ATPEchelle_Grandeurs.h: interface for the CATPEchelle_Grandeurs class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ATPECHELLE_GRANDEURS_H__5EDCF7D6_FA04_4BFA_A5F7_9491BFCCED91__INCLUDED_)
#define AFX_ATPECHELLE_GRANDEURS_H__5EDCF7D6_FA04_4BFA_A5F7_9491BFCCED91__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "JATPDocCreuset.h"

class CATPEchelle_Grandeurs  
{
public:
	// la fonction pour récupérer le nombre de grandeurs
	LONG GetNbGrandeurs();

	// la fonction pour définir le document
	virtual JVoid SetDocument(JATPDocCreuset* pATPDocCreuset);

public:
	// les fonctions pour tester les grandeurs
	BOOL IsXGRP			(LONG Index);
	BOOL IsGRP			(LONG Index);
	BOOL IsIndAffinite  (LONG Index);
	BOOL IsPrcAffinite  (LONG Index);
	BOOL IsCoutGRP		(LONG Index);
	BOOL IsRepetition	(LONG Index);
	BOOL IsNbIndividus	(LONG Index);
	BOOL IsNbContacts	(LONG Index);
	BOOL IsApport		(LONG Index);

private:
	JATPDocCreuset* m_pATPDocCreuset; // le document

};

#endif // !defined(AFX_ATPECHELLE_GRANDEURS_H__5EDCF7D6_FA04_4BFA_A5F7_9491BFCCED91__INCLUDED_)
