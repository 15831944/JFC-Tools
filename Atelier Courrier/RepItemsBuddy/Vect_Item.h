
#pragma once

#include "CalcItemParams.h"

// Cible de la commande CVect_Item

class CVect_Item : public CCmdTarget
{
	DECLARE_DYNAMIC(CVect_Item)

public:
	CVect_Item();
	virtual ~CVect_Item();

	virtual void OnFinalRelease();

protected:
	DECLARE_MESSAGE_MAP()
	DECLARE_DISPATCH_MAP()
	DECLARE_INTERFACE_MAP()

public:
	CCalcItemParams m_params;

public:
	LONG	GetCount(void);
	BOOL	GetIndiv(LONG idInd);
	LONG	GetIndivAsInt(LONG idInd);
	HRESULT VectIndivAsArray(VARIANT &vArray);

	enum 
	{
		dispidVectIndivAsArray	= 4L,
		dispidIndivAsInt		= 3,
		dispidIndiv				= 2,
		dispidCount				= 1
	};
};


