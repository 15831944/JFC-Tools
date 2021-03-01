
#pragma once

#include "Poids.h"

// Cible de la commande CVect_Poids

class CVect_Poids : public CCmdTarget
{
	DECLARE_DYNAMIC(CVect_Poids)

public:
	CVect_Poids();
	virtual ~CVect_Poids();

	virtual void OnFinalRelease();
	JDBLVECTOR * m_vect;
protected:
	DECLARE_MESSAGE_MAP()
	DECLARE_DISPATCH_MAP()
	DECLARE_INTERFACE_MAP()

	enum 
	{
		dispidVectPoidsIndivAsArray	= 3L,
		dispidPoidsIndiv			= 2,
		dispidCount					= 1
	};

public:
	LONG		GetCount(void);
	DOUBLE		GetPoidsIndiv(LONG idInd);
	HRESULT		VectPoidsIndivAsArray(VARIANT &vArray);
};


