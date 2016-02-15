// DistributionCroiseeRedresse.h: interface for the CDistributionCroiseeRedresse class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DISTRIBUTIONCROISEEREDRESSE_H__31FDBFF1_71D0_4C71_AF41_C24ACA0CA3B8__INCLUDED_)
#define AFX_DISTRIBUTIONCROISEEREDRESSE_H__31FDBFF1_71D0_4C71_AF41_C24ACA0CA3B8__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "DistributionCroisee.h"

class CDistributionCroiseeEx: public CDistributionCroisee  
{
public:
	void CreateNewDistributionCroiseeTheorique(CDistribution & DistributionMarginale0,CDistribution & DistributionMarginale1);
	void RedresseDistributionMarginale0(CDistribution & distribMarginaleRedresse);
	void RedresseDistributionMarginale1(CDistribution & distribMarginaleRedresse);
	void RedresseDistributionMarginale(CDistribution & distribMarginaleRedresse0,CDistribution & distribMarginaleRedresse1);
	CDistributionCroiseeEx();
	CDistributionCroiseeEx(CDistributionCroisee & crossdistrib)
	{
		CreateNewDistribution(crossdistrib);
	};


	virtual ~CDistributionCroiseeEx();
	void CalculDistributionCroiseeRedressementRelatif(CDistributionCroiseeEx & DXTheoriqueInitiale,CDistributionCroiseeEx & DXTheoriqueFinale);
	void CalculDistributionCroiseeRedressementAbsolu(CDistributionCroiseeEx & DXTheoriqueInitiale,CDistributionCroiseeEx & DXTheoriqueFinale);
	void CalculDistrubitionMarginale(CDistribution & distribmarginale,const int Dimension);


};

#endif // !defined(AFX_DISTRIBUTIONCROISEEREDRESSE_H__31FDBFF1_71D0_4C71_AF41_C24ACA0CA3B8__INCLUDED_)
