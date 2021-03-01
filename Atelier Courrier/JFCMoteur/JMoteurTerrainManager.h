/*
//	JFC Informatique et M�dia
//	2004
//
//	Auteur:
//	J�r�my DEVILERS
//
//
//	Classe:
//	JMoteurTerrainManager
//
//	Descriptif:
//	Permet de charger les terrains en m�moire de fa�on unique
//	Utilise CTerrainManager de JFCSource
*/

#pragma once

#include "KeyTerrain.h"
#include "TerrainManager.h"
#include ".\MoteurPresse\Include\JMTFR03.h"

//Singleton
class JMoteurTerrainManager
{
public:
	virtual ~JMoteurTerrainManager(void);

	//Singleton
	static JMoteurTerrainManager* GetInstance();

	//M�thode pour obtenir un terrain
	void AddTerrain(JInt32 idSource, JInt32 idTerrain);

	CTerrain * GetTerrain(JInt32 idSource, JInt32 idTerrain);
	CTerrain * GetTerrain(CKeyTerrain key);


public:
	JMTFR03TBLTerrains m_TblTerrains; // La table des terrains
private:
	CTerrainManager* m_CTerrains;


private:
	//Singleton
	JMoteurTerrainManager(void);
	static JMoteurTerrainManager	m_Instance;

};
