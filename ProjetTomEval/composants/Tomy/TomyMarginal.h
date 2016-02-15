// TomyMarginal.h: interface for the CTomyMarginal class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TOMYMARGINAL_H__0284DEF6_644F_4168_BAD7_883EF98C12F6__INCLUDED_)
#define AFX_TOMYMARGINAL_H__0284DEF6_644F_4168_BAD7_883EF98C12F6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "tomy.h"

class CTomyMarginal : public CTomy  
{
public:
	CTomyMarginal();
	virtual ~CTomyMarginal();

	void LanceEvalCalculMarginal(short Station,short JourNomme,short QHeure,double GRP,short NbInsertion=1);
	void LanceEvalDistribCalculMarginal(short Station,short JourNomme,short QHeure,double GRP,short NbInsertion=1);
	
	double GetApportGRP(void);
	double GetApportCouverture(void);
	double GetApportExactementTouche(int ClasseContact);

	void PrepareCalculZoom(void);
private:
	void TermineCalculZoom(void);
	double m_GRPavant;	
	double m_CouvertureAvant;
	CDistribution m_DistribAvant;
};

#endif // !defined(AFX_TOMYMARGINAL_H__0284DEF6_644F_4168_BAD7_883EF98C12F6__INCLUDED_)
