#include "StdAfx.h"
#include "terraindata.h"
//#include "JSrcPresse.h"
#include ".\terraindata.h"
#include "TerrainManager.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CTerrainData::CTerrainData(void)
{
	m_SourceId			=	0;
	m_TerrainId			=	0;
	m_pTerrain			=	0;
	m_pParams			=	PTR_CALCITEMPARAMS(new CCalcItemParams()); 
	m_PopSegment.SetSize(0);
	m_ContingentMask	=	0;
	m_EchantillonID		=	0;
	m_TypePoidsID		=	0;
	m_UniversID			=	0;
}

CTerrainData::~CTerrainData(void)
{
	if(m_pTerrain)
		CTerrainManager::GetInstance()->ReleaseTerrain(m_pTerrain);
}

bool	CTerrainData::LoadTerrain(JInt32 SourceId, JInt32 TerrainId, const JList<UINT> & Segments, JUnt32 TypePoidsId)
{
	const JSRCTables & Sources = JSRCTables::GetInstance ();
	const JSource * Src  = Sources.m_TBLSources.GetItem(SourceId);
	const JTerrain * Ter = 0;

	// Check if we have a terrain Id
	if(TerrainId == 0)
	{
		// No terrain Id so get the latest Terrain for the given source
		for(Sources.m_TBLTerrains.MoveFirst(); Sources.m_TBLTerrains.IsValid(); Sources.m_TBLTerrains.MoveNext())
		{
			if(Sources.m_TBLTerrains.GetItem().m_IdSource != SourceId)
				continue;
			if(Sources.m_TBLTerrains.GetItem().m_IdTerrain > TerrainId)
				TerrainId = Sources.m_TBLTerrains.GetItem().m_IdTerrain;
		}
		Ter = Sources.m_TBLTerrains.GetItem(SourceId, TerrainId);
	}
	else
		Ter = Sources.m_TBLTerrains.GetItem(SourceId, TerrainId);

	if(!Src || !Ter)
		return false;

	m_SourceId = SourceId;
	m_TerrainId = TerrainId;
	m_pTerrain = CTerrainManager::GetInstance()->GetTerrain(SourceId,TerrainId);
	if(m_pTerrain)
	{
		m_UniversID			= m_pTerrain->GetDefaultUnivers();
		m_EchantillonID		= m_pTerrain->GetDefaultEchantillonID(m_UniversID);

		SetSegments(Segments);
		SetTypePoids(TypePoidsId);

		m_pParams->SetEchantillonID(m_EchantillonID);
		m_pParams->SetUniversID(m_UniversID);
		m_pParams->SetTerrain(m_pTerrain);
		return true;
	}

	return false;
}

bool	CTerrainData::SetSegments(const JList<UINT> & Segments)
{
	JVector<JInt32> IdSegments;
	if(Segments.GetCount())
	{
		m_PopSegment.SetSize(0);
		JList<JInt32> TmpSegments;
		JVector<CSegment *> TSegments;
		m_pTerrain->GetSegments(TSegments);
		bool bFirst = true;
		for(Segments.MoveFirst(); Segments.IsValid(); Segments.MoveNext())
		{
			for(TSegments.MoveFirst(); TSegments.IsValid(); TSegments.MoveNext())
			{
				if(Segments.GetItem() == TSegments.GetItem()->GetID())
				{
					if(bFirst)
						m_PopSegment = TSegments.GetItem()->GetPopSegment();
					else
						m_PopSegment |= TSegments.GetItem()->GetPopSegment();
					TmpSegments.AddTail()=(JInt32)Segments.GetItem();
					bFirst = false;
				}
			}
		}
		IdSegments.SetCount(TmpSegments.GetCount());
		IdSegments.MoveFirst();
		for (TmpSegments.MoveFirst();TmpSegments.IsValid();TmpSegments.MoveNext()) {
			IdSegments.GetItem() = TmpSegments.GetItem();
			IdSegments.MoveNext();
		}
	}
	else
		m_pTerrain->GetDefaultPopSegment(m_PopSegment,m_ContingentMask); 

	m_pParams->SetPopulationSegment(m_PopSegment);
	m_pParams->SetSegmentsIDs(IdSegments);
	return true;
}

bool	CTerrainData::SetTypePoids(JUnt32 TypePoidsId)
{
	m_TypePoidsID = TypePoidsId;
	m_pParams->SetTypePoidsID(m_TypePoidsID);
	return true;
}


// // Retrieves the terrain lable
CString CTerrainData::GetTerrainLabel(void)
{
	CString Str;

	const JSRCTables & Sources = JSRCTables::GetInstance ();
	const JTerrain * Ter = Sources.m_TBLTerrains.GetItem(m_SourceId, m_TerrainId);
	if(!Ter)
		return false;

	Str = Ter->m_Libelle.AsJCharPtr();  
	return Str;
}

// // Retrieves the terrain lable
CString CTerrainData::GetSourceLabel(void)
{
	CString Str;

	const JSRCTables & Sources = JSRCTables::GetInstance ();
	const JSource * Src  = Sources.m_TBLSources.GetItem(m_SourceId);
	if(!Src)
		return false;

	Str = Src->m_Libelle.AsJCharPtr();  
	return Str;
}
