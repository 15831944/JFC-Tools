#include "stdafx.h"
#include ".\terrainmanager.h"
#include "JSRCTables.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CTerrainManager CTerrainManager::m_Instance;

CTerrainManager::CTerrainManager(void)
{
}

CTerrainManager::~CTerrainManager(void)
{
	//Par securité on vide les terrains chargés
	for(m_mapNbRef.MoveFirst(); m_mapNbRef.IsValid(); m_mapNbRef.MoveNext())
	{
		TRACE("CTerrainManager::Terrains restant : %d\n", m_mapNbRef.GetItem());
	}
		//delete m_mapTerrains.GetItem();
	//TRACE();
}

//Singleton
CTerrainManager * CTerrainManager::GetInstance()
{
	return & m_Instance;
}


CTerrain * CTerrainManager::GetTerrain(JInt32 idSource, JInt32 idTerrain, JBool AffichTerOk)
{
	CKeyTerrain key(idSource, idTerrain);
	return GetTerrain(key, AffichTerOk);
}
CTerrain * CTerrainManager::GetTerrain(const CKeyTerrain & key, JBool AffichTerOk)
{
	//On recherche le terrain
	m_mapTerrains.MoveTo(key);
	m_mapNbRef.MoveTo(key);
	if(m_mapTerrains.IsValid() && m_mapNbRef.IsValid())
	{
		//on incrémente le nb de ref
		m_mapNbRef.GetItem() ++ ;
		//et on donne le terrain
		PTR_TERRAIN & Ter = m_mapTerrains.GetItem();
		return Ter.get();
	}
	else
		return AddTerrain(key, AffichTerOk);//sinon on charge le terrain
}

CTerrain * CTerrainManager::GetTerrainNoRef(JInt32 idSource, JInt32 idTerrain)
{
	CKeyTerrain key(idSource, idTerrain);
	return GetTerrainNoRef(key);
}
CTerrain * CTerrainManager::GetTerrainNoRef(const CKeyTerrain & key)
{
	//On recherche le terrain
	m_mapTerrains.MoveTo(key);
	m_mapNbRef.MoveTo(key);
	if(m_mapTerrains.IsValid() && m_mapNbRef.IsValid())
	{
		//et on donne le terrain
		PTR_TERRAIN & Ter = m_mapTerrains.GetItem();
		return Ter.get();
	}
	else
		return NULL;
}


void CTerrainManager::ReleaseTerrain(CTerrain* terrain)
{
	if(terrain)
	{
		JUnt32 IdSource		= terrain->GetSourceID();
		JUnt32 IdTerrain	= terrain->GetTerrainID();
		if (IdSource != 0 && IdTerrain != 0)
		{
			CKeyTerrain key(terrain->GetSourceID(), terrain->GetTerrainID());
			ReleaseTerrain(key);
			terrain = NULL;
		}
	}
}

void CTerrainManager::ReleaseTerrain(JInt32 idSource, JInt32 idTerrain)
{
	CKeyTerrain key(idSource, idTerrain);
	ReleaseTerrain(key);
}
void CTerrainManager::ReleaseTerrain(CKeyTerrain key)
{
	//On recherche le terrain
	m_mapTerrains.MoveTo(key);
	m_mapNbRef.MoveTo(key);
	if(m_mapTerrains.IsValid() && m_mapNbRef.IsValid())
	{
		//on décrémente le nb de ref
		m_mapNbRef.GetItem() -- ;
		//S'il n'est plus reference on le retire
		if(m_mapNbRef.GetItem() == 0)
			RemoveTerrain(key);
	}
}

CTerrain * CTerrainManager::AddTerrain(CKeyTerrain key, JBool AffichTerOk)
{
	//Chargement du terrain
	const JSRCTables & srcTables = JSRCTables::GetInstance();
	JTerrain* Ter = (JTerrain*)srcTables.m_TBLTerrains.GetItem(key.m_IDSource, key.m_IDTerrain);
	if(Ter)
	{
		JStringEx Name;
		Name.Append(Ter->m_Chemin.AsJCharPtr());
		PTR_TERRAIN pTerrain = PTR_TERRAIN(new CTerrain());
		pTerrain->Load(Name, Ter->m_SeuilRedhibitoire, Ter->m_SeuilAvertissement, AffichTerOk);

		//on ajoute le terrain chargé et on incrémente son nb de ref
		m_mapTerrains.MoveTo(key);
		if (!m_mapTerrains.IsValid())
		{
			PTR_TERRAIN & Ter = m_mapTerrains.Add(key);
			Ter = pTerrain;
			m_mapNbRef.Add(key) = 1;
			return Ter.get();
		}
		return NULL;
	}
	if(key.m_IDSource == 0 && key.m_IDTerrain == 0)
	{
		//on ajoute le terrain dummy
		PTR_TERRAIN & Ter = m_mapTerrains.Add(key);
		Ter = PTR_TERRAIN(new CTerrain());
		m_mapNbRef.Add(key) = 1;
		return Ter.get();
	}
	return NULL;
}

void CTerrainManager::RemoveTerrain(CKeyTerrain key)
{
	//on est déja positionné, on efface le terrain et on le déréference
//	delete m_mapTerrains.GetItem();
	m_mapTerrains.Remove();
	m_mapNbRef.Remove();
}


