////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// Class pour cr�er des items construits temporaires (entre autre servira pour les items quantitatifs num�ris�s automatiquement
// cr��s
#pragma once
#include "ItemsConstruitsBase.h"

class AFX_EXT_CLASS CItemsConstruits : public CItemsConstruitsBase
{
private:
	CItemsConstruits(void);
	~CItemsConstruits(void);

public:
	// R�cup�re instance des elmts automatis�s
	static CItemsConstruits *	GetInstance();
	static void Destroy();

private:
	/// The instance to return
	static CItemsConstruits *	m_pInstance;

protected:
	/// Loads the Items construit (via fichier extension .10)
	virtual JBool Load(const char *Name);
};