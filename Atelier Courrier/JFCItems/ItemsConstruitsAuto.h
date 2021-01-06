////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// Class pour cr�er des items construits temporaires (entre autre servira pour les items quantitatifs num�ris�s automatiquement
// cr��s
#pragma once
#include "ItemsConstruitsBase.h"

class AFX_EXT_CLASS CItemsConstruitsAuto : public CItemsConstruitsBase
{
private:
	CItemsConstruitsAuto(void);
	~CItemsConstruitsAuto(void);

public:
	// R�cup�re instance des elmts automatis�s
	static CItemsConstruitsAuto *	GetInstance();
	static void Destroy();
private:
	//// The instance to return
	static CItemsConstruitsAuto *	m_pInstance;

protected:
	/// Loads the Items construit
	virtual JBool Load(const char *Name);
};
