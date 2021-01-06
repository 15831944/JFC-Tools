#include "StdAfx.h"


#include ".\RepItemsAPI.h"

#include "JTypes.h"
#include "JLib.h"
#include "DlgItems.h"
#include "TerrainManager.h"

#include "JFCConfig.h"

using namespace std;

#include "RepItemsWrap.h"

class TerrainEntry
{
public:
	TerrainEntry()
	{
		IdSource = 0;
		IdTerrain = 0;
		IdPoids = 0;
		m_pTerrain = NULL;
		m_pPoidsPtr = NULL;
		LstItem.Reset();
	}

	int IdSource;
	int IdTerrain;
	int IdPoids;
	JList<const IItem *> LstItem;

private:
	CTerrain * m_pTerrain;
	JDBLVECTOR* m_pPoidsPtr;

public:
	void Reset()
	{
		IdPoids = 0;
		IdTerrain = 0;
		IdSource = 0;

		if (m_pTerrain)
		{
			CTerrainManager::GetInstance()->ReleaseTerrain(m_pTerrain);
			m_pTerrain = NULL;
		}
		// Free the list of item
		LstItem.Reset();

		if (m_pPoidsPtr)
		{
			// Do i need to free the vector????
			// To test
			//m_pPoidsPtr->Reset();
			//delete m_pPoidsPtr;
			m_pPoidsPtr = NULL;
		}
	}

	bool Load(int idSrc, int idTerr, int idPoids)
	{
		bool loaded = false;
		Reset();
		IdPoids = idPoids;
		IdTerrain = idTerr;
		IdSource = idSrc;

		CTerrainManager *mgr = CTerrainManager::GetInstance();
		m_pTerrain = mgr->GetTerrain(idSrc, idTerr);

		if (m_pTerrain)
		{
			m_pPoidsPtr = (JDBLVECTOR*)m_pTerrain->GetStatisticValues(
				m_pTerrain->GetDefaultEchantillonID(m_pTerrain->GetDefaultUnivers()),
				m_pTerrain->GetDefaultUnivers(),
				idPoids /*m_pTerrain->GetDefaultTypePoidsID()*/ );

			
			CItemsConstruits* pItems = CItemsConstruits::GetInstance();
			loaded = pItems->GetItemsIndependant(LstItem, m_pTerrain);

		}
		return loaded;
	}

	int GetIndivCount()
	{
		int count = 0;
		if (m_pPoidsPtr)
		{
			count = m_pPoidsPtr->GetCount();
		}
		return count;
	}

	int GetPoids(double* buffer, int size)
	{
		if (m_pPoidsPtr && m_pPoidsPtr->GetCount())
		{
			long i = 0;
			for (m_pPoidsPtr->MoveFirst() ; 
				m_pPoidsPtr->IsValid() && i < size; 
				m_pPoidsPtr->MoveNext())
			{

				double dVal = m_pPoidsPtr->GetItem();
				buffer[i] = dVal;
				i++;
			}
			return i - 1;
		}
		return 0;
	}

	int GetItemVector(int idItem, int* buffer, int size)
	{
		if(m_pTerrain)
		{
			CItemsConstruits* pItems = CItemsConstruits::GetInstance();
			CItemConstruit * pItem = pItems->GetItemConstruit(idItem);
			if(pItem)
			{

				CCalcItemParams Params;

				Params.SetTerrain(m_pTerrain);
				Params.SetEchantillonID(m_pTerrain->GetDefaultEchantillonID(m_pTerrain->GetDefaultUnivers()));
				Params.SetUniversID(m_pTerrain->GetDefaultUnivers());
				Params.SetTypePoidsID(IdPoids /*m_pTerrain->GetDefaultTypePoidsID()*/);

				CBitArray* bArrPopSeg = new CBitArray();
				JUnt32 contingent =  0xFFFF;
				m_pTerrain->GetDefaultPopSegment(*bArrPopSeg, contingent);
				Params.SetPopulationSegment(*bArrPopSeg);

				JVector<CSegment*> vectSeg;
				JList<JInt32> vectIdSeg;
				m_pTerrain->GetSegments(vectSeg);
				for(vectSeg.MoveFirst(); vectSeg.IsValid(); vectSeg.MoveNext())
					vectIdSeg.AddTail() = vectSeg.GetItem()->GetID();
				Params.SetSegmentsIDs(vectIdSeg);

				if(!pItem->CalculateEffectif(&Params))
					return 0;

				if (!pItem->Calculate(&Params))
					return 0;

				int resSize = Params.GetResults()->GetSize();
				int count = 0;
				for (int i = 0; i < resSize && i < size; i++)
				{
					buffer[i] = Params.GetResults()->GetAt(i);
					count++;
				}
				return count;
			}
		}
		return 0;
	}

};

TerrainEntry CurrentEntry;

INTEROP_EXPORT void SetupIniFile(LPCTSTR iniFile)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	JFCConfig::CreateInstance(iniFile);

	JFCConfig::GetInstance();

	CString test1 = JFCConfig::GetInstance().GetValueString(SECTION_DATA,		KEY_SOURCES_PATH,	"?");
	CString test2 = JFCConfig::GetInstance().GetValueString(SECTION_DATA,		KEY_USER_PATH,	"?");

	//JFCConfig::GetInstance().WriteValueString(SECTION_DEFAULT, KEY_USER_PATH, iniFile);
	//JFCConfig::GetInstance().WriteValueInt(SECTION_OPTIONS_ANALYSEUR, KEY_RECT, 10);

}

INTEROP_EXPORT LPCTSTR GetSourcesPath()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	CString test = GETPROFILESTRINGD(SECTION_DATA, KEY_SOURCES_PATH, "");

	static CString buffer;
	buffer.Format(test);

	return buffer;
}

INTEROP_EXPORT LPCTSTR GetConfiguration(LPCTSTR keyVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	CString test = GETPROFILESTRINGD("Configuration", keyVal, "");

	static CString buffer;
	buffer.Format(test);

	return buffer;
}

INTEROP_EXPORT LPCTSTR GetIniValue(LPCTSTR sectionVal, LPCTSTR keyVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	CString test = GETPROFILESTRINGD(sectionVal, keyVal, "");

	static CString buffer;
	buffer.Format(test);

	return buffer;
}

INTEROP_EXPORT void ShowRepItem(int lang, int idSrc, int idTerr, int idPoids)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	CDlgItems * pRepItems = new CDlgItems(lang);
	pRepItems->SetShowMode(SHOWMODE_SIMPLE);
	pRepItems->SetPreloadTerrain(idSrc,idTerr,idPoids);
	pRepItems->DoModal();
	delete pRepItems;

}

INTEROP_EXPORT bool LoadTerrain(int idSrc, int idTerr, int idPoids)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	return CurrentEntry.Load(idSrc, idTerr, idPoids);
}

INTEROP_EXPORT void ResetTerrain()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	CurrentEntry.Reset();
}

INTEROP_EXPORT int GetIndivCount()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	
	return CurrentEntry.GetIndivCount();
}

INTEROP_EXPORT int GetPoids(double* buffer, int size)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	return CurrentEntry.GetPoids(buffer, size);
}

INTEROP_EXPORT void MoveFirstItem()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	CurrentEntry.LstItem.MoveFirst();
}

INTEROP_EXPORT bool IsValidItem()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	return CurrentEntry.LstItem.IsValid();

}

INTEROP_EXPORT void MoveNextItem()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	CurrentEntry.LstItem.MoveNext();

}

INTEROP_EXPORT void GetItem(Item* item)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	const IItem * ciItem = CurrentEntry.LstItem.GetItem();

	item->id = ciItem->GetID();
	item->label = (char*)ciItem->GetLabel().AsJCharPtr();

}

INTEROP_EXPORT int GetItemIndivVector(int idItem, int* buffer, int size)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	return CurrentEntry.GetItemVector(idItem, buffer, size);

}

//
//CString nString;
//
//INTEROP_EXPORT LPCTSTR TestString()
//{
//	AFX_MANAGE_STATE(AfxGetStaticModuleState());
//	nString = "This is a test";
// 
//	return (LPCTSTR)nString;
//}
//


