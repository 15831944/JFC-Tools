////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// Class pour créer des items construits temporaires (entre autre servira pour les items quantitatifs numérisés automatiquement
// créés
#pragma once
#include "ItemsConstruitsBase.h"

class AFX_EXT_CLASS CItemsConstruits : public CItemsConstruitsBase
{
private:
	CItemsConstruits(void);
	~CItemsConstruits(void);

public:
	// Récupère instance des elmts automatisés
	static CItemsConstruits *	GetInstance();
	static void Destroy();

private:
	/// The instance to return
	static CItemsConstruits *	m_pInstance;

protected:
	/// Loads the Items construit (via fichier extension .10)
	virtual JBool Load(const char *Name);
};