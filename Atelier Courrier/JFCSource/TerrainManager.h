/*
//	JFC Informatique et Média
//	2004
//
//	Auteur:
//	Jérémy DEVILERS
//
//
//	Classe:
//	CTerrainManager
//
//	Descriptif:
//	Gestionnaire de terrain, permet de n'avoir qu'un instance d'un terrain en mémoire pour toute l'application
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

	//Méthode pour obtenir un terrain
	CTerrain * GetTerrain(JInt32 idSource, JInt32 idTerrain, JBool AffichTerOk = true);
	CTerrain * GetTerrain(const CKeyTerrain & key, JBool AffichTerOk = true);
	
	//Méthode pour obtenir un terrain mais n'ajoute pas de reference
	CTerrain * GetTerrainNoRef(JInt32 idSource, JInt32 idTerrain);
	CTerrain * GetTerrainNoRef(const CKeyTerrain & key);

	//Quand on n'a plus besoin du terrain
	void ReleaseTerrain(CTerrain* terrain);

private:
	void ReleaseTerrain(JInt32 idSource, JInt32 idTerrain);
	void ReleaseTerrain(CKeyTerrain key);

	//Charge un nouveau terrain
	CTerrain * AddTerrain(CKeyTerrain key, JBool AffichTerOk = true);

	//Delete un terrain quand il n'est plus referencé
	void RemoveTerrain(CKeyTerrain key);

	//Le map contenant les terrains
	JMap<CKeyTerrain, PTR_TERRAIN> m_mapTerrains;
	//Le map compteur de référence
	JMap<CKeyTerrain, JUnt32> m_mapNbRef;

private:
	//Singleton
	CTerrainManager(void);
	static CTerrainManager	m_Instance;
};
