#include "stdafx.h"
#include ".\JMoteurTerrainManager.h"
#include "JSRCPresse.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

JMoteurTerrainManager JMoteurTerrainManager::m_Instance;

JMoteurTerrainManager::JMoteurTerrainManager(void)
{
	m_CTerrains = CTerrainManager::GetInstance();
}

JMoteurTerrainManager::~JMoteurTerrainManager(void)
{
//on déréférence
	for(m_TblTerrains.MoveFirst(); m_TblTerrains.IsValid(); m_TblTerrains.MoveNext())
	{
		JMTFR03KEYTerrain kTerrain = m_TblTerrains.GetKey();
		//m_CTerrains->ReleaseTerrain(kTerrain.m_IdSource.AsJInt32(), kTerrain.m_IdTerrain.AsJInt32());
		m_CTerrains->ReleaseTerrain(GetTerrain(kTerrain.m_IdSource.AsJInt32(), kTerrain.m_IdTerrain.AsJInt32()));
	}
}

//Singleton
JMoteurTerrainManager * JMoteurTerrainManager::GetInstance()
{
	return & m_Instance;
}


CTerrain * JMoteurTerrainManager::GetTerrain(JInt32 idSource, JInt32 idTerrain)
{
	//on n'ajoute pas de ref
	return m_CTerrains->GetTerrainNoRef(idSource, idTerrain);
}
CTerrain * JMoteurTerrainManager::GetTerrain(CKeyTerrain key)
{
	//on n'ajoute pas de ref
	return  m_CTerrains->GetTerrainNoRef(key);
}


void JMoteurTerrainManager::AddTerrain(JInt32 idSource, JInt32 idTerrain)
{
	JMTFR03KEYTerrain kTerrain;
	kTerrain.m_IdSource = idSource;
	kTerrain.m_IdTerrain = idTerrain;
	if(kTerrain.IsValid())
	{
		m_TblTerrains.MoveTo(kTerrain);
		if (! m_TblTerrains.IsValid())
		{
			try
			{
				m_TblTerrains.Add(kTerrain);
				//on ajoute une ref
				m_CTerrains->GetTerrain(idSource, idTerrain);
			}
			catch (...)
			{
				AfxMessageBox("Problème chargement terrain, vérifier fichier <TERRAINS.TBL>.\nAttention suite à cela le programme ne marchera pas correctement.", MB_ICONERROR | MB_OK);

				// on propage l'exception
				throw;
			}
		}
	}
}



