/*
//	JFC Informatique et M�dia
//	2004
//
//	Auteur:
//	J�r�my DEVILERS
//
//
//	Classe:
//	CTerrainManager
//
//	Descriptif:
//	Gestionnaire de terrain, permet de n'avoir qu'un instance d'un terrain en m�moire pour toute l'application
*/

#pragma once

#include "Terrain.h"
#include "KeyTerrain.h"


//Singleton
class AFX_EXT_CLASS CTerrainManager
{
public:
	virtual ~CTerrainManager(void);

	//Singleton
	static CTerrainManager* GetInstance();

	//M�thode pour obtenir un terrain
	CTerrain * GetTerrain(JInt32 idSource, JInt32 idTerrain, JBool AffichTerOk = true);
	CTerrain * GetTerrain(const CKeyTerrain & key, JBool AffichTerOk = true);
	
	//M�thode pour obtenir un terrain mais n'ajoute pas de reference
	CTerrain * GetTerrainNoRef(JInt32 idSource, JInt32 idTerrain);
	CTerrain * GetTerrainNoRef(const CKeyTerrain & key);

	//Quand on n'a plus besoin du terrain
	void ReleaseTerrain(CTerrain* terrain);

private:
	void ReleaseTerrain(JInt32 idSource, JInt32 idTerrain);
	void ReleaseTerrain(CKeyTerrain key);

	//Charge un nouveau terrain
	CTerrain * AddTerrain(CKeyTerrain key, JBool AffichTerOk = true);

	//Delete un terrain quand il n'est plus referenc�
	void RemoveTerrain(CKeyTerrain key);

	//Le map contenant les terrains
	JMap<CKeyTerrain, PTR_TERRAIN> m_mapTerrains;
	//Le map compteur de r�f�rence
	JMap<CKeyTerrain, JUnt32> m_mapNbRef;

private:
	//Singleton
	CTerrainManager(void);
	static CTerrainManager	m_Instance;
};
