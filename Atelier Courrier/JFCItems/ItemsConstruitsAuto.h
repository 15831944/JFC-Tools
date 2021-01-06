////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// Class pour créer des items construits temporaires (entre autre servira pour les items quantitatifs numérisés automatiquement
// créés
#pragma once
#include "ItemsConstruitsBase.h"

class AFX_EXT_CLASS CItemsConstruitsAuto : public CItemsConstruitsBase
{
private:
	CItemsConstruitsAuto(void);
	~CItemsConstruitsAuto(void);

public:
	// Récupère instance des elmts automatisés
	static CItemsConstruitsAuto *	GetInstance();
	static void Destroy();
private:
	//// The instance to return
	static CItemsConstruitsAuto *	m_pInstance;

protected:
	/// Loads the Items construit
	virtual JBool Load(const char *Name);
};
