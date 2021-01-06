/*
//	JFC Informatique et M�dia
//	2004
//
//	Auteur:
//	J�r�my DEVILERS
//
//
//	Classe:
//	COffreTerrainManager
//
//	Descriptif:
//	Permet la gestion intelligente du chargement des terrains dan la boite offre et l'application.
*/

#pragma once

#include "KeyTerrain.h"
#include "TerrainManager.h"
#include "JATPOffre.h"

//Singleton
class COffreTerrainManager
{
public:
	virtual ~COffreTerrainManager(void);

	//Singleton
	static COffreTerrainManager* GetInstance();

	void Preload(const JATPOffre& offre);

	//M�thode pour obtenir un terrain
	CTerrain * GetTerrain(JInt32 idSource, JInt32 idTerrain);
	CTerrain * GetTerrain(CKeyTerrain key);

	void Unload();


private:
	CTerrainManager* m_TerrainManager;
	JMap< CKeyTerrain , CKeyTerrain > m_mapKeyTerrain;


private:
	//Singleton
	COffreTerrainManager(void);
	static COffreTerrainManager	m_Instance;

};
