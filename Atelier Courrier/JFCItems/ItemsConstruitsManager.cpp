#include "stdafx.h"
#include "ItemsConstruitsManager.h"
#include "ItemsConstruits.h"
#include "ItemsConstruitsAuto.h"

// Init instance manager
CItemsConstruitsManager * CItemsConstruitsManager::m_pInstance = 0;

/////////////////////////////////////////////////////////////////////////////////////////////////////
// Constructeur
CItemsConstruitsManager::CItemsConstruitsManager(void)
{
	m_Mode = ModeUtilisateur;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
// Destructeur
CItemsConstruitsManager::~CItemsConstruitsManager(void)
{
}

//////////////////////////////////////////////////////////////////////////////////////////////////////
// R�cup�ration de l'instance pour les items construits selon le mode (utilisateur ou automatique)
CItemsConstruitsBase * CItemsConstruitsManager::GetItemsInstance()
{
	if (m_Mode == ModeUtilisateur)
		return CItemsConstruits::GetInstance();
	else if (m_Mode == ModeAutomatique)
		return CItemsConstruitsAuto::GetInstance();

	ASSERT(false);
	return 0;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////
// Release g�n�rique
//void CItemsConstruitsManager::Release()
//{
//	if (m_Mode == ModeUtilisateur)
//		return CItemsConstruits::Release();
//	else if (m_Mode == ModeAutomatique)
//		return CItemsConstruitsAuto::Release();
//
//	// GetInstance()->Release();
//
//	ASSERT(false);
//}

//////////////////////////////////////////////////////////////////////////////////////////////////////
// R�cup�ration de l'instance manager
CItemsConstruitsManager * CItemsConstruitsManager::GetInstance()
{
	if (m_pInstance == 0)
	{
		m_pInstance = new CItemsConstruitsManager;
	}
	return m_pInstance;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////
// Destruction de l'instance manager
void CItemsConstruitsManager::Destroy()
{
	// on d�truit le manager
	if (m_pInstance != 0)
		delete m_pInstance;
	m_pInstance = 0;

	// on d�truit les autres instances
	CItemsConstruits::Destroy();
	CItemsConstruitsAuto::Destroy();
}
