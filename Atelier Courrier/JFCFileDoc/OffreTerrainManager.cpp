#include "stdafx.h"
#include ".\OffreTerrainManager.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

COffreTerrainManager COffreTerrainManager::m_Instance;

COffreTerrainManager::COffreTerrainManager(void)
{
	m_TerrainManager = CTerrainManager::GetInstance();
}

COffreTerrainManager::~COffreTerrainManager(void)
{
}

//Singleton
COffreTerrainManager * COffreTerrainManager::GetInstance()
{
	return & m_Instance;
}


CTerrain * COffreTerrainManager::GetTerrain(JInt32 idSource, JInt32 idTerrain)
{
	CKeyTerrain key;
	key.Set(idSource, idTerrain);
	return GetTerrain(key);
}
CTerrain * COffreTerrainManager::GetTerrain(CKeyTerrain key)
{
	//on n'ajoute pas de ref
	if (key.IsValid())
	{
		m_mapKeyTerrain.MoveTo(key);
		if(m_mapKeyTerrain.IsValid())
			return m_TerrainManager->GetTerrainNoRef(key);
		else
		{
			// Mettre les messages spécifiques terrain à ce niveau
			//
			// A FAIRE   
			// 
			// NON : CERTAINEMENT PAS mettre de d'affichages de messages ICI!!!
			m_mapKeyTerrain.Add(key) = key;
			//charge le terrain et incrémente la ref
			return m_TerrainManager->GetTerrain(key);
		}
	}
	else
		return NULL;
}


void COffreTerrainManager::Preload(const JATPOffre& offre)
{
	//on précharge les terrains s'il n'existent pas
	CKeyTerrain key;
	key.m_IDSource = offre.m_IdSourceAudience;
	for(offre.m_TBLIdTerrainsAudience.MoveFirst();
		offre.m_TBLIdTerrainsAudience.IsValid();
		offre.m_TBLIdTerrainsAudience.MoveNext() )
	{
		key.m_IDTerrain = offre.m_TBLIdTerrainsAudience.GetItem();
		m_mapKeyTerrain.MoveTo(key);
		if(key.IsValid() && !m_mapKeyTerrain.IsValid())
		{

			m_mapKeyTerrain.Add(key) = key;
			//charge le terrain et incrémente la ref
			m_TerrainManager->GetTerrain(key);
		}
	}
	key.m_IDSource = offre.m_IdSourceAudience;
	key.m_IDTerrain = offre.m_IdTerrainCiblage;
	m_mapKeyTerrain.MoveTo(key);
	if(key.IsValid() && !m_mapKeyTerrain.IsValid())
	{
		m_mapKeyTerrain.Add(key) = key;
		m_TerrainManager->GetTerrain(key);
	}
}

void COffreTerrainManager::Unload()
{
	//on libere les terrains inutilisés
	for(m_mapKeyTerrain.MoveFirst(); m_mapKeyTerrain.IsValid(); m_mapKeyTerrain.MoveNext())
		m_TerrainManager->ReleaseTerrain(GetTerrain(m_mapKeyTerrain.GetItem()));
	m_mapKeyTerrain.Reset();
}




