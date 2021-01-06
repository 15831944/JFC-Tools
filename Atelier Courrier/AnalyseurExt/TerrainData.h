#pragma once

#include "CalcItemParams.h"
#include "Terrain.h"

class CTerrainData
{
public:
	CTerrainData(void);
	virtual ~CTerrainData(void);
	bool	LoadTerrain(JInt32 SourceId, JInt32 TerrainId,const JList<UINT> & Segments, JUnt32 TypePoidsId);
	bool	SetSegments(const JList<UINT> & Segments);
	bool	SetTypePoids(JUnt32 TypePoidsId);

	JInt32								m_SourceId;
	JInt32								m_TerrainId;
	CTerrain *							m_pTerrain;
	PTR_CALCITEMPARAMS					m_pParams;
	CBitArray							m_PopSegment;
	JUnt32								m_ContingentMask;
	JUnt32								m_EchantillonID;
	JUnt32								m_TypePoidsID;
	JUnt32								m_UniversID;
	// Retrieves the terrain lable
	CString GetTerrainLabel(void);
	// Retrieves the Source lable
	CString GetSourceLabel(void);
};

typedef std::auto_ptr<CTerrainData> PTR_TERRAINDATA;
