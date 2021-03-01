//////////////////////////////////////////////////////////////////////////////
//
// ItemsConstruitsBase.cpp: implementation of the CItemsConstruitsBase class.
//
//////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include ".\itemsconstruitsbase.h"
#include "Convert.h"
#include "JMFCStream.h"
#include "ToXMLIVisitor.h"
#include "ToTextIVisitor.h"

#include "JFCConfig.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

// Get an instance of this class
// this adds one to the reference counter and
// thus must be released by the caller after use
//CItemsConstruitsBase * CItemsConstruitsBase::GetInstance()
//{
//	// on initialise l'indicateur
//	static JBool bLoaded = false;
//
//	if(!bLoaded)
//	{
//		if(!m_Instance.Load("ItemsConstruits"))
//			m_Instance.CreateEnsemble();
//
//		bLoaded = true;
//	}
//
//	m_RefCount++;
//
//	return & m_Instance;
//}
//
//// Decrements the reference count
//JBool	CItemsConstruitsBase::Release()
//{
//	if(!m_RefCount || --m_RefCount)
//		return false;
//
//	return true;
//}

CItemsConstruitsBase::CItemsConstruitsBase()
{
	CoInitialize(NULL);

	// File Name
	m_sName				= "";
	m_MaxItem			=          ITEM_BASE; //  1000000
	m_tMaxItem			=         -ITEM_BASE; // -1000000
	m_MaxPalettes		=      PALETTES_BASE; //  2000000
	m_tMaxPalettes		=     -PALETTES_BASE; // -2000000
	m_MaxQuestionInd	=   QUESTIONIND_BASE; //  3000000
	m_tMaxQuestionInd	=  -QUESTIONIND_BASE; // -3000000
	m_MaxCritereMod		=    CRITEREMOD_BASE; //  4000000
	m_tMaxCritereMod	=   -CRITEREMOD_BASE; // -4000000
	m_MaxCritereQuant	=  CRITEREQUANT_BASE; //  5000000
	m_tMaxCritereQuant	= -CRITEREQUANT_BASE; // -5000000
	m_MaxCritereScore	=  CRITERESCORE_BASE; //  6000000
	m_tMaxCritereScore	= -CRITERESCORE_BASE; // -6000000
	m_MaxItemPondere	=  ITEMPONDERE_BASE;  //  7000000
	m_tMaxItemPondere	= -ITEMPONDERE_BASE;  // -7000000

	m_bDirty			= false;
	m_bLoaded			= false;
}

CItemsConstruitsBase::~CItemsConstruitsBase()
{
	CoUninitialize(); 
}

JBool CItemsConstruitsBase::Load(const char *Name)
{
	m_FileName = Name;
	m_FileName.Append(".10");
	m_sName = "/";
	m_sName.Append (m_FileName);

	SetCurrentDirectory(GETPROFILESTRINGD(SECTION_DATA, KEY_USER_PATH, 0));

	CFileStatus Status;
	if(CFile::GetStatus(m_FileName.AsJCharPtr(),Status))
		m_ThisTime = Status.m_mtime ;
	else
		m_ThisTime=CTime::GetCurrentTime();

	JArchive * pFile = 0;

	try
	{
		pFile = JArchive::Create(m_sName.AsJCharPtr());

		LoadFileHeader		(pFile);
		LoadItems			(pFile, m_FileHeader.GetVersion());
		LoadItemsIndependant(pFile, m_FileHeader.GetVersion());
		LoadPalettess		(pFile, m_FileHeader.GetVersion());
		LoadQuestionInds	(pFile, m_FileHeader.GetVersion());
		LoadCritereMods		(pFile, m_FileHeader.GetVersion());
		LoadCritereQuants	(pFile, m_FileHeader.GetVersion());
		LoadCritereScores	(pFile, m_FileHeader.GetVersion());
		LoadItemsPonderes	(pFile, m_FileHeader.GetVersion());

		delete pFile;
	}
	catch(JException *pE)
	{
		delete pFile;
		SetCurrentDirectory(GETPROFILESTRINGD(SECTION_DATA, KEY_SOURCES_PATH, 0));
		return false;
	}

	m_bDirty	= false;
	m_bLoaded	= true;
	SetCurrentDirectory(GETPROFILESTRINGD(SECTION_DATA, KEY_SOURCES_PATH, 0));

	return true;
}

JBool CItemsConstruitsBase::Save()
{
	if(IsDirty())
	{
		SetCurrentDirectory(GETPROFILESTRINGD(SECTION_DATA, KEY_USER_PATH, 0));
		JArchive * pFile = 0;
		try
		{
			pFile = JArchive::Create(m_sName.AsJCharPtr(),true);

			SaveFileHeader		(pFile);
			SaveItems			(pFile);
			SaveItemsIndependant(pFile);
			SavePalettess		(pFile);
			SaveQuestionInds	(pFile);
			SaveCritereMods		(pFile);
			SaveCritereQuants	(pFile);
			SaveCritereScores	(pFile);
			SaveItemsPonderes	(pFile);

			pFile->Flush();
			delete pFile;

		}
		catch(JException *pE)
		{
			if(pFile)
				delete pFile;
			SetCurrentDirectory(GETPROFILESTRINGD(SECTION_DATA, KEY_SOURCES_PATH, 0));
			return false;
		}

		JPathManager::UpdateFile(m_sName.AsJCharPtr());
		SetCurrentDirectory(GETPROFILESTRINGD(SECTION_DATA, KEY_SOURCES_PATH, 0));
		m_bDirty = false;
	}
	return true;
}

// Load the file header
JVoid CItemsConstruitsBase::LoadFileHeader(JArchive *pFile)
{
	const JInt32 CURRENT_VERSION = 1;

	m_FileHeader.Recv(*pFile);

	// Check compagnie name
	if(m_FileHeader.GetCompagnie () != 0x0043464A)
		throw JInternalError::GetInstance(); //("CItemsConstruitsBase :: File Header, compagnie field error");

	// Verify the magic number
	if(m_FileHeader.GetMagicNumber () != 0x6FD35910)
		throw JInternalError::GetInstance(); //("CItemsConstruitsBase :: File Header, Magic Number field error");

	// No compression
	if(m_FileHeader.GetCompression () != 0)
		throw JInternalError::GetInstance(); //("CItemsConstruitsBase :: File Header, Compression field error");

	// No cryptage
	if(m_FileHeader.GetCryptage () != 0)
		throw JInternalError::GetInstance(); //("CItemsConstruitsBase :: File Header, Cryptage field error");

	// Must not be greater than my current version
	if(m_FileHeader.GetVersion () > CURRENT_VERSION)
		throw JInternalError::GetInstance(); //("CItemsConstruitsBase :: File Header, Version field error");
}

// Load the file header
JVoid CItemsConstruitsBase::SaveFileHeader(JArchive *pFile)
{
	const JInt32 CURRENT_VERSION = 1;

	// Set compagnie name
	m_FileHeader.SetCompagnie (0x0043464A);

	// Set the magic number
	m_FileHeader.SetMagicNumber (0x6FD35910);

	// Set No compression
	m_FileHeader.SetCompression (0);

	// Set No cryptage
	m_FileHeader.SetCryptage (0);

	// Set current version
	m_FileHeader.SetVersion (CURRENT_VERSION);

	m_FileHeader.Send(*pFile);
}

// Load Items
JVoid CItemsConstruitsBase::LoadItems(JArchive *pFile, JInt32 Version)
{
	switch(Version)
	{
	case 1:
		{
			pFile->Recv(m_MaxItem);
			JInt32 Count;
			pFile->Recv(Count);

			for( JInt32 x = 0; x < Count; x++)
			{
				PTR_ITEMCONSTRUIT pItem(new CItemConstruit(this,m_tMaxItem--));

				pItem->Recv (*pFile, Version);
				m_ItemsConstruits.Add(pItem->GetID()) = pItem;
			}
		}
		break;

	default:
		TRACE("CItemsConstruitsBase Version error");
		throw JInternalError::GetInstance(); //("CItemsConstruitsBase :: File Version error");
		break;
	}
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Save Items
JVoid CItemsConstruitsBase::SaveItems(JArchive *pFile) //const
{
	// Nombre d'items construits
	JUnt32 NbItemsConst = m_ItemsConstruits.GetCount(); 

	// Comptabilité des items existants enregistrés
	JInt32 Count = 0;
	for (m_ItemsConstruits.MoveFirst(); m_ItemsConstruits.IsValid();m_ItemsConstruits.MoveNext())
	{
		const PTR_ITEMCONSTRUIT & ItemConstruit = m_ItemsConstruits.GetItem();

		if (m_ItemsConstruits.GetItem()->IsItemConstruit())
		{
			// const CItemConstruit * pItemConst = m_ItemsConstruits.GetItem()->AsItemConstruit();
			CItemConstruit * pItemConst = m_ItemsConstruits.GetItem()->AsItemConstruit();
		
			// Test si item ou palette a virer, alors remettre le comp RefCount à 0
			// TODO JUIN 2014
			if ((pItemConst->GetIDSource() == 49 && pItemConst->GetIDTerrain() < 514) ||
				(pItemConst->GetIDSource() != 49 && pItemConst->GetID() != 1000000)
				)
			{
				if (pItemConst->GetOwnerID() >= 2000000)
				{
					m_Palettes.MoveTo(pItemConst->GetOwnerID());
					if (m_Palettes.IsValid())
						m_Palettes.GetItem()->ResetOwnerID();

				}

				pItemConst->RefCountAZero();

				m_ItemsIndependant.MoveTo(pItemConst->GetID());
				if (m_ItemsIndependant.IsValid())
					m_ItemsIndependant.Remove();

			}

			
		}
		else if (m_ItemsConstruits.GetItem()->IsItemPondere())
		{
			const CItemPondere * pItemConst = m_ItemsConstruits.GetItem()->AsItemPondere();
		
			/*
			if (pItemConst->GetIDSource() == 49 && pItemConst->GetIDTerrain() < 514)
			{
				pItemConst->RefCountAZero();
			}

			if (pItemConst->GetIDSource() != 49 && pItemConst->GetID() != 1000000)
			{
				pItemConst->RefCountAZero();
			}
			*/
		}
		if(m_ItemsConstruits.GetItem()->IsRegistered())
			Count++;
	}

	// Sauvegarde du maximum des items
	pFile->Send(m_MaxItem);

	// Sauvegarde du nombre d'items
	pFile->Send(Count);

	// Sauvegarde de chacun des items	
	for (m_ItemsConstruits.MoveFirst(); m_ItemsConstruits.IsValid();m_ItemsConstruits.MoveNext())
	{
		if(m_ItemsConstruits.GetItem()->IsRegistered())
			m_ItemsConstruits.GetItem()->Send(*pFile);
	}
}

// Load List of Items independant
JVoid CItemsConstruitsBase::LoadItemsIndependant(JArchive *pFile, JInt32 Version)
{
	switch(Version)
	{
	case 1:
		{
			JInt32 Count = 0;
			pFile->Recv(Count);

			for( JInt32 x = 0; x < Count; x++)
			{
				pFile->Recv(m_ItemsIndependant.AddTail());
			}
		}
		break;

	default:
		TRACE("CItemsConstruitsBase Version error");
		throw JInternalError::GetInstance(); //("CItemsConstruitsBase :: File Version error");
		break;
	}
}

// Save Items Independant
JVoid CItemsConstruitsBase::SaveItemsIndependant(JArchive *pFile) const
{
	JInt32 Count = 0;
	for (m_ItemsIndependant.MoveFirst(); m_ItemsIndependant.IsValid(); m_ItemsIndependant.MoveNext())
	{
		m_ItemsConstruits.MoveTo(m_ItemsIndependant.GetItem());
		if(m_ItemsConstruits.IsValid() && m_ItemsConstruits.GetItem()->IsRegistered())
		{
			Count++;
		}
	}
	pFile->Send(Count);

	for (m_ItemsIndependant.MoveFirst(); m_ItemsIndependant.IsValid(); m_ItemsIndependant.MoveNext())
	{
		m_ItemsConstruits.MoveTo(m_ItemsIndependant.GetItem());
		if(m_ItemsConstruits.IsValid() && m_ItemsConstruits.GetItem()->IsRegistered())
		{
			pFile->Send(m_ItemsIndependant.GetItem());
		}
	}

	/*JInt32 Count = m_ItemsIndependant.GetCount();
	pFile->Send(Count);

	for (m_ItemsIndependant.MoveFirst(); m_ItemsIndependant.IsValid();m_ItemsIndependant.MoveNext())
	{
		pFile->Send(m_ItemsIndependant.GetItem());
	}*/
}

// Load the palettes
JVoid CItemsConstruitsBase::LoadPalettess(JArchive *pFile, JInt32 Version)
{
	switch(Version)
	{
	case 1:
		{
			JInt32 Count = 0;

			pFile->Recv(m_MaxPalettes);
			pFile->Recv(Count);

			for( JInt32 x = 0; x < Count; x++)
			{
				PTR_PALETTES pItem(new CPalettes(this,m_tMaxPalettes--,false));

				pItem->Recv(*pFile, Version);
				m_Palettes.Add(pItem->GetID ()) = pItem;
			}
		}
		break;

	default:
		TRACE("CItemsConstruitsBase Version error");
		throw JInternalError::GetInstance(); //("CItemsConstruitsBase :: File Version error");
		break;
	}
}

// Save Palettess
JVoid CItemsConstruitsBase::SavePalettess(JArchive *pFile) const
{
	JInt32 Count = 0;
	
	for (m_Palettes.MoveFirst(); m_Palettes.IsValid();m_Palettes.MoveNext())
	{
		if(m_Palettes.GetItem()->IsRegistered())
			Count++;
	}

	pFile->Send(m_MaxPalettes);
	pFile->Send(Count);

	for (m_Palettes.MoveFirst(); m_Palettes.IsValid();m_Palettes.MoveNext())
	{
		if(m_Palettes.GetItem()->IsRegistered())
			m_Palettes.GetItem()->Send(*pFile);
	}
}

// Load the items pondérés
JVoid CItemsConstruitsBase::LoadItemsPonderes(JArchive *pFile, JInt32 Version)
{
	switch(Version)
	{
	case 1:
		{
			JInt32 Count = 0;

			pFile->Recv(m_MaxItemPondere);
			pFile->Recv(Count);

			for( JInt32 x = 0; x < Count; x++)
			{
				PTR_ITEMSPONDERES pItem(new CItemPondere(this,m_tMaxItemPondere--,false));

				pItem->Recv(*pFile, Version);
				m_ItemsPonderes.Add(pItem->GetID ()) = pItem;
			}
		}
		break;

	default:
		TRACE("CItemsConstruitsBase Version error");
		throw JInternalError::GetInstance(); //("CItemsConstruitsBase :: File Version error");
		break;
	}
}

// Save Items Ponderes
JVoid CItemsConstruitsBase::SaveItemsPonderes(JArchive *pFile) const
{
	// Comptabilise les items pondérés enregistrés
	JInt32 Count = 0;
	for (m_ItemsPonderes.MoveFirst(); m_ItemsPonderes.IsValid(); m_ItemsPonderes.MoveNext())
	{
		if(m_ItemsPonderes.GetItem()->IsRegistered())
			Count++;
	}

	pFile->Send(m_MaxItemPondere);
	pFile->Send(Count);

	for (m_ItemsPonderes.MoveFirst(); m_ItemsPonderes.IsValid(); m_ItemsPonderes.MoveNext())
	{
		if(m_ItemsPonderes.GetItem()->IsRegistered())
			m_ItemsPonderes.GetItem()->Send(*pFile);
	}
}

// Load Critere Induites
JVoid CItemsConstruitsBase::LoadQuestionInds(JArchive *pFile, JInt32 Version)
{
	switch(Version)
	{
	case 1:
		{
			JInt32 Count = 0;

			pFile->Recv(m_MaxQuestionInd);
			pFile->Recv(Count);

			for( JInt32 x = 0; x < Count; x++)
			{
				PTR_QUESTIONIND pItem(new CQuestionInduite(this,m_tMaxQuestionInd--,false));

				pItem->Recv(*pFile, Version);
				m_QuestionInds.Add(pItem->GetID ()) = pItem;
			}
		}
		break;

	default:
		TRACE("CItemsConstruitsBase Version error");
		throw JInternalError::GetInstance(); //("CItemsConstruitsBase :: File Version error");
		break;
	}
}

// Save QuestionInduites
JVoid CItemsConstruitsBase::SaveQuestionInds(JArchive *pFile) const
{
	JInt32 Count = 0;

	for (m_QuestionInds.MoveFirst(); m_QuestionInds.IsValid();m_QuestionInds.MoveNext())
	{
		if(m_QuestionInds.GetItem()->IsRegistered())
			Count++;
	}

	pFile->Send(m_MaxQuestionInd);
	pFile->Send(Count);

	for (m_QuestionInds.MoveFirst(); m_QuestionInds.IsValid();m_QuestionInds.MoveNext())
	{
		if(m_QuestionInds.GetItem()->IsRegistered())
			m_QuestionInds.GetItem()->Send(*pFile);
	}
}

// Load Critères modifiés
JVoid CItemsConstruitsBase::LoadCritereMods(JArchive *pFile, JInt32 Version)
{
	switch(Version)
	{
	case 1:
		{
			JInt32 Count = 0;

			pFile->Recv(m_MaxCritereMod);
			pFile->Recv(Count);

			for( JInt32 x = 0; x < Count; x++)
			{
				PTR_CRITEREMODIFIE pItem(new CCritereModifie(this,m_tMaxCritereMod--, false));

				pItem->Recv(*pFile, Version);
				m_CritereMods.Add(pItem->GetID ()) = pItem;
			}
		}
		break;

	default:
		TRACE("CItemsConstruitsBase Version error");
		throw JInternalError::GetInstance(); //("CItemsConstruitsBase :: File Version error");
		break;
	}
}

// Save Critere Modifiés
JVoid CItemsConstruitsBase::SaveCritereMods(JArchive *pFile) const
{
	JInt32 Count = 0;

	for (m_CritereMods.MoveFirst(); m_CritereMods.IsValid();m_CritereMods.MoveNext())
	{
		if(m_CritereMods.GetItem()->IsRegistered())
			Count++;
	}

	pFile->Send(m_MaxCritereMod);
	pFile->Send(Count);

	for (m_CritereMods.MoveFirst(); m_CritereMods.IsValid();m_CritereMods.MoveNext())
	{
		if(m_CritereMods.GetItem()->IsRegistered())
			m_CritereMods.GetItem()->Send(*pFile);
	}
}

// Load Critères quantiles
JVoid CItemsConstruitsBase::LoadCritereQuants(JArchive *pFile, JInt32 Version)
{
	switch(Version)
	{
	case 1:
		{
			JInt32 Count = 0;

			pFile->Recv(m_MaxCritereQuant);
			pFile->Recv(Count);

			for( JInt32 x = 0; x < Count; x++)
			{
				PTR_CRITEREQUANTILE pItem(new CCritereQuantile(this,m_tMaxCritereQuant--, false));

				pItem->Recv(*pFile, Version);
				m_CritereQuants.Add(pItem->GetID()) = pItem;
			}
		}
		break;

	default:
		TRACE("CItemsConstruitsBase Version error");
		throw JInternalError::GetInstance(); //("CItemsConstruitsBase :: File Version error");
		break;
	}
}

// Save Critere Quantiles
JVoid CItemsConstruitsBase::SaveCritereQuants(JArchive *pFile) const
{
	JInt32 Count = 0;

	for (m_CritereQuants.MoveFirst(); m_CritereQuants.IsValid();m_CritereQuants.MoveNext())
	{
		if(m_CritereQuants.GetItem()->IsRegistered())
			Count++;
	}

	pFile->Send(m_MaxCritereQuant);
	pFile->Send(Count);

	for (m_CritereQuants.MoveFirst(); m_CritereQuants.IsValid();m_CritereQuants.MoveNext())
	{
		if(m_CritereQuants.GetItem()->IsRegistered())
			m_CritereQuants.GetItem()->Send(*pFile);
	}
}

// Load the Critères de scoring
JVoid CItemsConstruitsBase::LoadCritereScores(JArchive *pFile, JInt32 Version)
{
	switch(Version)
	{
	case 1:
		{
			JInt32 Count = 0;

			pFile->Recv(m_MaxCritereScore);
			pFile->Recv(Count);

			for( JInt32 x = 0; x < Count; x++)
			{
				//				PTR_CRITERESCORE pItem(new CCritereScore(this,m_tMaxCritereScore--, false));

				//				pItem->Recv(*pFile, Version);

				//				m_CritereScores.Add(pItem->GetID()) = pItem;
			}
		}
		break;

	default:
		TRACE("CItemsConstruitsBase Version error");
		throw JInternalError::GetInstance(); //("CItemsConstruitsBase :: File Version error");
		break;
	}
}

// Save Critere Scoring
JVoid CItemsConstruitsBase::SaveCritereScores(JArchive *pFile) const
{
	JInt32 Count = 0;

	for (m_CritereScores.MoveFirst(); m_CritereScores.IsValid();m_CritereScores.MoveNext())
	{
		if(m_CritereScores.GetItem()->IsRegistered())
			Count++;
	}

	pFile->Send(m_MaxCritereScore);
	pFile->Send(Count);

	for (m_CritereScores.MoveFirst(); m_CritereScores.IsValid(); m_CritereScores.MoveNext())
	{
		if(m_CritereScores.GetItem()->IsRegistered())
			m_CritereScores.GetItem()->Send(*pFile);
	}
}

// Create new ItemsConstruit / this pointer must be deleted by the caller
CItemConstruit * CItemsConstruitsBase::GetNewItemConstruit(const CItemConstruit * rhs)
{
	CItemConstruit * pItem = new CItemConstruit(this, m_tMaxItem--);
	if(rhs)
		pItem->DeepCopy(rhs);
	return pItem;
};

// Get pointer an ItemConstruit
CItemConstruit *	CItemsConstruitsBase::GetItemConstruit(JInt32 ID) const
{
	m_ItemsConstruits.MoveTo(ID);

	if(m_ItemsConstruits.IsValid())
		return m_ItemsConstruits.GetItem()->AsItemConstruit();
	else
		return 0;
}

/*
// Debloquage item pour suppression
void CItemsConstruitsBase::DebloqueItem (JInt32 ID) const
{
	m_ItemsConstruits.MoveTo(ID);

	if(m_ItemsConstruits.IsValid())
	{
		m_ItemsConstruits.GetItem()->m_lReferenceCnt = 0;
	}
}
*/

// Register new item construit / returns a constant pointer to the registered version
CItemConstruit *	CItemsConstruitsBase::RegisterItemConstruit	(CItemConstruit * pItem, JMap<JInt32,const IItem *> &ImpMap, JInt32 OwnerID)
{
	pItem->Register(OwnerID,m_MaxItem++);
	if(OwnerID==ITEMSINDEPENDANT_ID || OwnerID==SYSTEM_ID)
		// Add ID to independant items list
		m_ItemsIndependant.AddTail() = pItem->GetID();

	m_ItemsConstruits.Add(pItem->GetID()) = PTR_ITEMCONSTRUIT(pItem); 
	SetDirty();

	return pItem;
}


// Register new item construit / returns a constant pointer to the registered version
const CItemConstruit *	CItemsConstruitsBase::RegisterItemConstruit	(const CItemConstruit * pItem, JInt32 OwnerID)
{
	// See if item is already registered
	if(pItem->IsRegistered())
	{
		m_ItemsConstruits.MoveTo(pItem->GetID());
		if(m_ItemsConstruits.IsValid())
		{
			// Get non const version
			m_ItemsConstruits.GetItem()->Register(OwnerID,0);
			// Set the data as dirty
			SetDirty();
		}
		return pItem;
	}

	if(!pItem->IsValid())
		return 0;

	PTR_ITEMCONSTRUIT pDest(new CItemConstruit(this,m_tMaxItem--));
	// Copy the original
	pDest->DeepCopy(pItem);

	pDest->Register(OwnerID,m_MaxItem++);
	if(OwnerID==ITEMSINDEPENDANT_ID || OwnerID==SYSTEM_ID)
		// Add ID to independant items list
		m_ItemsIndependant.AddTail() = pDest->GetID();

	CItemConstruit *pData = pDest.get();
	m_ItemsConstruits.Add(pDest->GetID()) = pDest; 
	SetDirty();

	return pData;
}

// UnRegister an item construit / returns true if unregistered
JBool	CItemsConstruitsBase::CanUnRegisterItemConstruit(JInt32 ID, JMap<JInt32, JLabel> * pUpwards, JInt32 OwnerID)
{
	// Find the Item construit
	m_ItemsConstruits.MoveTo(ID);
	if(!m_ItemsConstruits.IsValid())
		return false;

	// Can we unregister it
	JMap<JInt32, JInt32> MapItems;
	if(!m_ItemsConstruits.GetItem()->CanUnRegister(OwnerID, (pUpwards) ? &MapItems : 0))
	{
		if(pUpwards)
			GetUpwards(MapItems, ID, *pUpwards);
		return false;
	}


	// Yes, we can
	return true;
}

// UnRegister an item construit / returns true if unregistered
JBool	CItemsConstruitsBase::UnRegisterItemConstruit(JInt32 ID, JInt32 OwnerID)
{
	// Find the Item construit
	m_ItemsConstruits.MoveTo(ID);
	if(!m_ItemsConstruits.IsValid())
		return false;

	// Can we unregister it
	if(m_ItemsConstruits.GetItem()->CanUnRegister(OwnerID))
	{
		// Unregister the item construit
		m_ItemsConstruits.GetItem()->UnRegister(OwnerID);
		m_ItemsConstruits.MoveTo(ID);
		if(!m_ItemsConstruits.IsValid())
			return false;

		// Test if we were the last registered owner
		// If Items independant is owner remove the item from it's list
		if(!m_ItemsConstruits.GetItem()->IsRegistered() && OwnerID == ITEMSINDEPENDANT_ID)
		{
			for (m_ItemsIndependant.MoveFirst(); m_ItemsIndependant.IsValid();m_ItemsIndependant.MoveNext())
			{
				if(m_ItemsIndependant.GetItem()==m_ItemsConstruits.GetItem()->GetID())
				{
					// Removes the item from its list
					m_ItemsIndependant.Remove();
					break;
				}
			}
		}
		// Set the data as dirty
		SetDirty();
		return true;
	}
	return false;
}
// returns true if registered
JBool	CItemsConstruitsBase::IsRegisteredItemConstruit	(JInt32 ID)
{
	// Find the Item construit
	m_ItemsConstruits.MoveTo(ID);
	if(!m_ItemsConstruits.IsValid())
		return false;

	// Can we unregister it
	return m_ItemsConstruits.GetItem()->IsRegistered();
}

// Get constant pointer to a Palette
CPalettes *	CItemsConstruitsBase::GetPalette(JInt32 ID) const
{
	m_Palettes.MoveTo(ID);

	if(m_Palettes.IsValid())
		return m_Palettes.GetItem()->AsPalette();
	else
		return 0;
}

// Register new Palette / returns a constant pointer to the registered version
CPalettes *	CItemsConstruitsBase::RegisterPalette(CPalettes * pItem, JMap< JInt32, const IItem *> &ImpMap)
{
	// Set this as owner
	pItem->Register(PALETTE_ID,ImpMap,m_MaxPalettes++);
	m_Palettes.Add(pItem->GetID()) = PTR_PALETTES(pItem); 
	SetDirty();

	return pItem;
}

// Register new Palette / returns a constant pointer to the registered version
const CPalettes *	CItemsConstruitsBase::RegisterPalette(const CPalettes * pItem)
{
	// See if object is already registered
	if(pItem->IsRegistered())
		return pItem;

	PTR_PALETTES  pDest(new CPalettes(this,m_tMaxPalettes--, true));
	// Copy the original
	pDest->DeepCopy(pItem);

	// Set this as owner
	pDest->Register(PALETTE_ID,m_MaxPalettes++);

	CPalettes *pData = pDest.get();
	m_Palettes.Add(pDest->GetID()) = pDest; 
	SetDirty();

	return pData;
}

// Can UnRegister a Palette / returns true if the palette can be unregistered
JBool	CItemsConstruitsBase::CanUnRegisterPalette(JInt32 ID, JMap<JInt32, JLabel> * pUpwards)
{
	// Find the object
	m_Palettes.MoveTo(ID);
	if(!m_Palettes.IsValid())
		return false;

	// Can we unregister it
	JMap<JInt32, JInt32> MapItems;
	if(!m_Palettes.GetItem()->CanUnRegister(PALETTE_ID, (pUpwards) ? &MapItems : 0))
	{
		if(pUpwards)
			GetUpwards(MapItems, ID, *pUpwards);
		return false;
	}

	// Yes, we can
	return true;
}

// UnRegister a Palette / returns true if unregistered
JBool	CItemsConstruitsBase::UnRegisterPalette(JInt32 ID)
{
	// Find the object
	m_Palettes.MoveTo(ID);
	if(!m_Palettes.IsValid())
		return false;

	// Can we unregister it
	if(m_Palettes.GetItem()->CanUnRegister(PALETTE_ID))
	{
		// Unregister the object
		m_Palettes.GetItem()->UnRegister(PALETTE_ID);

		// Set the data as dirty
		SetDirty();
		return true;
	}
	return false;
}

// returns true if registered
JBool	CItemsConstruitsBase::IsRegisteredPalette(JInt32 ID)
{
	// Find the object
	m_Palettes.MoveTo(ID);
	if(!m_Palettes.IsValid())
		return false;
	return m_Palettes.GetItem()->IsRegistered();
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Get constant pointer to a Item Pondéré
CItemPondere *	CItemsConstruitsBase::GetItemPondere(JInt32 ID) const
{
	m_ItemsPonderes.MoveTo(ID);

	if(m_ItemsPonderes.IsValid())
		return m_ItemsPonderes.GetItem()->AsItemPondere();
	else
		return 0;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Register new Item Pondere / returns a constant pointer to the registered version
CItemPondere *	CItemsConstruitsBase::RegisterItemPondere(CItemPondere * pItem, JMap< JInt32, const IItem *> &ImpMap)
{
	// Set this as owner
	pItem->Register(ITEMSPONDERE_ID, ImpMap, m_MaxItemPondere++);
	m_ItemsPonderes.Add(pItem->GetID()) = PTR_ITEMSPONDERES(pItem); 
	SetDirty();

	return pItem;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Register new Item Pondéré / returns a constant pointer to the registered version
const CItemPondere *	CItemsConstruitsBase::RegisterItemPondere(const CItemPondere * pItem)
{
	// See if object is already registered
	if(pItem->IsRegistered())
		return pItem;

	PTR_ITEMSPONDERES  pDest(new CItemPondere(this, m_tMaxItemPondere--, true));
	// Copy the original
	pDest->DeepCopy(pItem);

	// Set this as owner
	pDest->Register(ITEMSPONDERE_ID, m_MaxItemPondere++);

	CItemPondere *pData = pDest.get();
	m_ItemsPonderes.Add(pDest->GetID()) = pDest; 
	SetDirty();

	return pData;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Can UnRegister a Item Pondéré / returns true if the item pondéré can be unregistered
JBool	CItemsConstruitsBase::CanUnRegisterItemPondere(JInt32 ID, JMap<JInt32, JLabel> * pUpwards)
{
	// Find the object
	m_ItemsPonderes.MoveTo(ID);
	if(!m_ItemsPonderes.IsValid())
		return false;

	// Can we unregister it
	JMap<JInt32, JInt32> MapItems;
	if(!m_ItemsPonderes.GetItem()->CanUnRegister(ITEMSPONDERE_ID, (pUpwards) ? &MapItems : 0))
	{
		if(pUpwards)
			GetUpwards(MapItems, ID, *pUpwards);
		return false;
	}

	// Yes, we can
	return true;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// UnRegister a Item Pondéré / returns true if unregistered
JBool	CItemsConstruitsBase::UnRegisterItemPondere(JInt32 ID)
{
	// Find the object
	m_ItemsPonderes.MoveTo(ID);
	if (!m_ItemsPonderes.IsValid())
		return false;

	// Can we unregister it
	if(m_ItemsPonderes.GetItem()->CanUnRegister(ITEMSPONDERE_ID))
	{
		// Unregister the object
		m_ItemsPonderes.GetItem()->UnRegister(ITEMSPONDERE_ID);

		// Vire l'item dans le map
		m_ItemsPonderes.Remove(); 

		// Set the data as dirty
		SetDirty();
		return true;
	}
	return false;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// returns true if item pondéré registered
JBool	CItemsConstruitsBase::IsRegisteredItemPondere(JInt32 ID)
{
	// Find the object
	m_ItemsPonderes.MoveTo(ID);
	if(!m_ItemsPonderes.IsValid())
		return false;
	return m_ItemsPonderes.GetItem()->IsRegistered();
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Get constant pointer to a QuestionInduite
CQuestionInduite *	CItemsConstruitsBase::GetQuestionInduite(JInt32 ID) const
{
	m_QuestionInds.MoveTo(ID);

	if(m_QuestionInds.IsValid())
		return m_QuestionInds.GetItem()->AsQuestionInduite();
	else
		return 0;
}

CQuestionInduite *	CItemsConstruitsBase::RegisterQuestionInduite(CQuestionInduite * pItem, JMap<JInt32, const IItem *> &ImpMap)
{
	// Set this as owner
	pItem->Register(QUESTIONIND_ID,ImpMap,m_MaxQuestionInd++);
	m_QuestionInds.Add(pItem->GetID()) = PTR_QUESTIONIND(  pItem); 
	SetDirty();

	return pItem;
}

// Register new QuestionInduite / returns a constant pointer to the registered version
const CQuestionInduite *	CItemsConstruitsBase::RegisterQuestionInduite(const CQuestionInduite * pItem)
{
	// See if object is already registered
	if(pItem->IsRegistered())
		return pItem;

	PTR_QUESTIONIND  pDest(new CQuestionInduite(this,m_tMaxQuestionInd--,true));
	// Copy the original
	pDest->DeepCopy(pItem);

	// Set this as owner
	pDest->Register(QUESTIONIND_ID,m_MaxQuestionInd++);
	CQuestionInduite * pData = pDest.get();
	m_QuestionInds.Add(pDest->GetID()) = pDest; 
	SetDirty();

	return pData;
}

// Can UnRegister a Critere Induite / returns true if the critere induite can be unregistered
JBool	CItemsConstruitsBase::CanUnRegisterQuestionInduite(JInt32 ID, JMap<JInt32, JLabel> * pUpwards)
{
	// Find the object
	m_QuestionInds.MoveTo(ID);
	if(!m_QuestionInds.IsValid())
		return false;

	// Can we unregister it
	JMap<JInt32, JInt32> MapItems;
	if(!m_QuestionInds.GetItem()->CanUnRegister(QUESTIONIND_ID, (pUpwards) ? &MapItems : 0))
	{
		if(pUpwards)
			GetUpwards(MapItems, ID, *pUpwards);
		return false;
	}

	// Yes, we can
	return true;
}

// UnRegister a QuestionInduite / returns true if unregistered
JBool	CItemsConstruitsBase::UnRegisterQuestionInduite(JInt32 ID)
{
	// Find the object
	m_QuestionInds.MoveTo(ID);
	if(!m_QuestionInds.IsValid())
		return false;

	// Can we unregister it
	if(m_QuestionInds.GetItem()->CanUnRegister(QUESTIONIND_ID))
	{
		// Unregister the object
		m_QuestionInds.GetItem()->UnRegister(QUESTIONIND_ID);
		// Set the data as dirty
		SetDirty();
		return true;
	}
	return false;
}

// returns true if registered
JBool	CItemsConstruitsBase::IsRegisteredQuestionInduite(JInt32 ID)
{
	// Find the object
	m_QuestionInds.MoveTo(ID);
	if(!m_QuestionInds.IsValid())
		return false;

	return m_QuestionInds.GetItem()->IsRegistered();
}


// Get constant pointer to a CritereModifie
CCritereModifie *	CItemsConstruitsBase::GetCritereModifie(JInt32 ID) const
{
	m_CritereMods.MoveTo(ID);

	if(m_CritereMods.IsValid())
		return m_CritereMods.GetItem()->AsCritereModifie();
	else
		return 0;
}

CCritereModifie *	CItemsConstruitsBase::RegisterCritereModifie(CCritereModifie * pItem, JMap<JInt32, const IItem *> &ImpMap)
{
	// Set this as owner
	pItem->Register(CRITEREMOD_ID,ImpMap,m_MaxCritereMod++);
	m_CritereMods.Add(pItem->GetID()) = PTR_CRITEREMODIFIE(  pItem); 
	SetDirty();

	return pItem;
}

// Register new CritereModifie / returns a constant pointer to the registered version
const CCritereModifie *	CItemsConstruitsBase::RegisterCritereModifie(const CCritereModifie * pItem)
{
	// See if object is already registered
	if(pItem->IsRegistered())
		return pItem;

	PTR_CRITEREMODIFIE  pDest(new CCritereModifie(this,m_tMaxCritereMod--, true));
	// Copy the original
	pDest->DeepCopy(pItem);

	// Set this as owner
	pDest->Register(CRITEREMOD_ID,m_MaxCritereMod++);

	CCritereModifie *pData = pDest.get();
	m_CritereMods.Add(pDest->GetID()) = pDest; 
	SetDirty();
	return pData;
}

// Can UnRegister a Critere Modifié / returns true if the critere modifié can be unregistered
JBool	CItemsConstruitsBase::CanUnRegisterCritereModifie(JInt32 ID, JMap<JInt32, JLabel> * pUpwards)
{
	// Find the object
	m_CritereMods.MoveTo(ID);
	if(!m_CritereMods.IsValid())
		return false;

	// Can we unregister it
	JMap<JInt32, JInt32> MapItems;
	if(!m_CritereMods.GetItem()->CanUnRegister(CRITEREMOD_ID, (pUpwards) ? &MapItems : 0))
	{
		if(pUpwards)
			GetUpwards(MapItems, ID, *pUpwards);
		return false;
	}

	// Yes, we can
	return true;
}

// UnRegister a CritereModifie / returns true if unregistered
JBool	CItemsConstruitsBase::UnRegisterCritereModifie(JInt32 ID)
{
	// Find the object
	m_CritereMods.MoveTo(ID);
	if(!m_CritereMods.IsValid())
		return false;

	// Can we unregister it
	if(m_CritereMods.GetItem()->CanUnRegister(CRITEREMOD_ID))
	{
		// Unregister the object
		m_CritereMods.GetItem()->UnRegister(CRITEREMOD_ID);

		// Set the data as dirty
		SetDirty();
		return true;
	}
	return false;
}

// returns true if registered
JBool	CItemsConstruitsBase::IsRegisteredCritereModifie(JInt32 ID)
{
	// Find the object
	m_CritereMods.MoveTo(ID);
	if(!m_CritereMods.IsValid())
		return false;
	return m_CritereMods.GetItem()->IsRegistered();
}

// Get constant pointer to a CritereQuant
CCritereQuantile *	CItemsConstruitsBase::GetCritereQuantile(JInt32 ID) const
{
	m_CritereQuants.MoveTo(ID);

	if(m_CritereQuants.IsValid())
		return m_CritereQuants.GetItem()->AsCritereQuantile();
	else
		return 0;
}

CCritereQuantile *	CItemsConstruitsBase::RegisterCritereQuantile(CCritereQuantile * pItem, JMap<JInt32, const IItem *> &ImpMap)
{
	// Set this as owner
	pItem->Register(CRITEREQUANT_ID,ImpMap,m_MaxCritereQuant ++);
	m_CritereQuants.Add(pItem->GetID()) = PTR_CRITEREQUANTILE(pItem); 
	SetDirty();

	return pItem;
}

// Register new CritereQuant / returns a constant pointer to the registered version
const CCritereQuantile *	CItemsConstruitsBase::RegisterCritereQuantile(const CCritereQuantile * pItem)
{
	// See if object is already registered
	if(pItem->IsRegistered())
		return pItem;

	PTR_CRITEREQUANTILE  pDest(new CCritereQuantile(this,m_tMaxCritereQuant--, true));
	// Copy the original
	pDest->DeepCopy(pItem);

	// Set this as owner
	pDest->Register(CRITEREQUANT_ID,m_MaxCritereQuant++);
	CCritereQuantile *pData = pDest.get();
	m_CritereQuants.Add(pDest->GetID()) = pDest; 
	SetDirty();
	return pData;
}

// Can UnRegister a Critere Quantile / returns true if the critere quantile can be unregistered
JBool	CItemsConstruitsBase::CanUnRegisterCritereQuantile(JInt32 ID, JMap<JInt32, JLabel> * pUpwards)
{
	// Find the object
	m_CritereQuants.MoveTo(ID);
	if(!m_CritereQuants.IsValid())
		return false;

	// Can we unregister it
	JMap<JInt32, JInt32> MapItems;
	if(!m_CritereQuants.GetItem()->CanUnRegister(CRITEREQUANT_ID, (pUpwards) ? &MapItems : 0))
	{
		if(pUpwards)
			GetUpwards(MapItems, ID, *pUpwards);
		return false;
	}

	// Yes, we can
	return true;
}

// UnRegister a CritereQuant / returns true if unregistered
JBool	CItemsConstruitsBase::UnRegisterCritereQuantile(JInt32 ID)
{
	// Find the object
	m_CritereQuants.MoveTo(ID);
	if(!m_CritereQuants.IsValid())
		return false;

	// Can we unregister it
	if(m_CritereQuants.GetItem()->CanUnRegister(CRITEREQUANT_ID))
	{
		// Unregister the object
		m_CritereQuants.GetItem()->UnRegister(CRITEREQUANT_ID);

		// Set the data as dirty
		SetDirty();
		return true;
	}
	return false;
}

// returns true if registered
JBool	CItemsConstruitsBase::IsRegisteredCritereQuantile(JInt32 ID)
{
	// Find the object
	m_CritereQuants.MoveTo(ID);
	if(!m_CritereQuants.IsValid())
		return false;

	return m_CritereQuants.GetItem()->IsRegistered();
}

// Get constant pointer to a CritereScore
CCritereScore *	CItemsConstruitsBase::GetCritereScore(JInt32 ID) const
{
	m_CritereScores.MoveTo(ID);

	if(m_CritereScores.IsValid())
		return m_CritereScores.GetItem()->AsCritereScore();
	else
		return 0;
}

// Register new CritereScore / returns a constant pointer to the registered version
const CCritereScore *	CItemsConstruitsBase::RegisterCritereScore(const CCritereScore * pItem)
{
	// See if object is already registered
	if(pItem->IsRegistered())
		return pItem;

	PTR_CRITERESCORE  pDest(new CCritereScore(this,m_tMaxCritereScore--, true));
	// Copy the original
	pDest->DeepCopy(pItem);

	// Set this as owner
	pDest->Register(CRITERESCORE_ID,m_MaxCritereScore++);
	CCritereScore *pData = pDest.get();
	m_CritereScores.Add(pDest->GetID()) = pDest; 
	SetDirty();

	return pData;
}

// Can UnRegister a Critere Score / returns true if the critere score can be unregistered
JBool	CItemsConstruitsBase::CanUnRegisterCritereScore(JInt32 ID, JMap<JInt32, JLabel> * pUpwards)
{
	// Find the object
	m_CritereScores.MoveTo(ID);
	if(!m_CritereScores.IsValid())
		return false;

	// Can we unregister it
	JMap<JInt32, JInt32> MapItems;
	if(!m_CritereScores.GetItem()->CanUnRegister(CRITERESCORE_ID, (pUpwards) ? &MapItems : 0))
	{
		if(pUpwards)
			GetUpwards(MapItems, ID, *pUpwards);
		return false;
	}

	// Yes, we can
	return true;
}

// UnRegister a CritereScore / returns true if unregistered
JBool	CItemsConstruitsBase::UnRegisterCritereScore(JInt32 ID)
{
	// Find the object
	m_CritereScores.MoveTo(ID);
	if(!m_CritereScores.IsValid())
		return false;

	// Can we unregister it
	if(m_CritereScores.GetItem()->CanUnRegister(CRITERESCORE_ID))
	{
		// Unregister the object
		m_CritereScores.GetItem()->UnRegister(CRITERESCORE_ID);

		// Set the data as dirty
		SetDirty();
		return true;
	}
	return false;
}

// returns true if registered
JBool	CItemsConstruitsBase::IsRegisteredCritereScore(JInt32 ID)
{
	// Find the object
	m_CritereScores.MoveTo(ID);
	if(!m_CritereScores.IsValid())
		return false;

	return m_CritereScores.GetItem()->IsRegistered();
}

// Get a vector of Items Independant as CItemConstruit
JBool	CItemsConstruitsBase::GetItemsIndependant(JList<const CItemConstruit *> & vItems, CTerrain *pTerrain) const
{
	for (m_ItemsIndependant.MoveFirst(); m_ItemsIndependant.IsValid(); m_ItemsIndependant.MoveNext())
	{
		m_ItemsConstruits.MoveTo(m_ItemsIndependant.GetItem());
		if(m_ItemsConstruits.IsValid())
		{
			CItemConstruit * pItem = m_ItemsConstruits.GetItem().get();
			if(!pTerrain || pItem->IsValid(pTerrain))
				vItems.AddTail() = pItem->AsItemConstruit();
		}
		/*else
			return false;*/
	}
	return true;
}

// Get a vector of Items Independant as IItems
JBool	CItemsConstruitsBase::GetItemsIndependant	(JList<const IItem *> & vItems, JList<CTerrain*> & lstTerrains) const
{
	for (m_ItemsIndependant.MoveFirst(); m_ItemsIndependant.IsValid(); m_ItemsIndependant.MoveNext())
	{
		m_ItemsConstruits.MoveTo(m_ItemsIndependant.GetItem());
		if(m_ItemsConstruits.IsValid())
		{
			CItemConstruit * pItem = m_ItemsConstruits.GetItem().get();
			bool bAdd = true;
			for(lstTerrains.MoveFirst(); lstTerrains.IsValid(); lstTerrains.MoveNext())
				if(! pItem->IsValid(lstTerrains.GetItem()))
				{
					bAdd = false;
					break;
				}
			if(bAdd)
				vItems.AddTail() = pItem->AsIItem();
		}
		/*else
			return false;*/
	}
	return true;

}

// Get a vector of Items Independant
JBool	CItemsConstruitsBase::GetItemsIndependant(JList<const IItem *> & vItems, CTerrain *pTerrain) const
{
	for (m_ItemsIndependant.MoveFirst(); m_ItemsIndependant.IsValid(); m_ItemsIndependant.MoveNext())
	{
		m_ItemsConstruits.MoveTo(m_ItemsIndependant.GetItem());
		if(m_ItemsConstruits.IsValid())
		{
			CItemConstruit * pItem = m_ItemsConstruits.GetItem().get();

			// Mars 2014
			// A REMETTRE JUIN 2014
			if (pItem->m_lSourceID == 49 && pItem->m_lTerrainID <= 514)
			{
				pItem->m_RebuildViaRepAbs = true;
			}
			//

			if(!pTerrain || pItem->IsValid(pTerrain))
				vItems.AddTail() = pItem->AsIItem();
		}
		/*else
			return false;*/
	}
	return true;
}

// Get a vector of Items Independant as IModalites
JBool	CItemsConstruitsBase::GetItemsIndependant(JList<const IModalite *> & vItems, CTerrain *pTerrain) const
{
	for (m_ItemsIndependant.MoveFirst(); m_ItemsIndependant.IsValid(); m_ItemsIndependant.MoveNext())
	{
		m_ItemsConstruits.MoveTo(m_ItemsIndependant.GetItem());
		if(m_ItemsConstruits.IsValid())
		{
			CItemConstruit * pItem = m_ItemsConstruits.GetItem().get();
			if(!pTerrain || pItem->IsValid(pTerrain))
				vItems.AddTail() = pItem->AsIModalite();
		}
		/*else
			return false;*/
	}
	return true;
}

// Get a vector of Palettes
JBool	CItemsConstruitsBase::GetPalettes(JList<const IQuestion *> &vItems, CTerrain *pTerrain) const
{
	for (m_Palettes.MoveFirst(); m_Palettes.IsValid(); m_Palettes.MoveNext())
	{
		CPalettes *pItem = m_Palettes.GetItem().get();
		if(pItem->IsRegistered())
			if(!pTerrain || pItem->IsValid(pTerrain))
				vItems.AddTail() = pItem->AsIQuestion();
	}
	return true;
}

/// Get a vector of Palettes
JBool	CItemsConstruitsBase::GetPalettes(JList<const IItem *>  &vItems, CTerrain *pTerrain) const
{
	for (m_Palettes.MoveFirst(); m_Palettes.IsValid(); m_Palettes.MoveNext())
	{
		CPalettes *pItem = m_Palettes.GetItem().get();
		if(pItem->IsRegistered())
			if(!pTerrain || pItem->IsValid(pTerrain))
				vItems.AddTail() = pItem->AsIItem();
	}
	return true;
}

/// Get a vector of Palettes
JBool	CItemsConstruitsBase::GetPalettes(JList<const IItem *>  &vItems, JList<CTerrain*> & lstTerrains) const
{
	for (m_Palettes.MoveFirst(); m_Palettes.IsValid(); m_Palettes.MoveNext())
	{
		CPalettes *pItem = m_Palettes.GetItem().get();
		if(pItem->IsRegistered())
		{
			bool bAdd = true;
			for(lstTerrains.MoveFirst(); lstTerrains.IsValid(); lstTerrains.MoveNext())
				if(!pItem->IsValid(lstTerrains.GetItem()))
					bAdd = false;
			if(bAdd)
				vItems.AddTail() = pItem->AsIItem();
		}
	}
	return true;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Get a vector of Items Ponderes
JBool CItemsConstruitsBase::GetItemsPonderes(JList<const IItem *>  &vItems, CTerrain *pTerrain) const
{
	for (m_ItemsPonderes.MoveFirst(); m_ItemsPonderes.IsValid(); m_ItemsPonderes.MoveNext())
	{
		CItemPondere *pItem = m_ItemsPonderes.GetItem().get();
		if(pItem->IsRegistered())
			if(!pTerrain || pItem->IsValid(pTerrain))
				vItems.AddTail() = pItem->AsIItem();
	}
	return true;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Get a vector of Items Ponderes
JBool CItemsConstruitsBase::GetItemsPonderes(JList<const IItem *>  &vItems, JList<CTerrain*> & lstTerrains) const
{
	for (m_ItemsPonderes.MoveFirst(); m_ItemsPonderes.IsValid(); m_ItemsPonderes.MoveNext())
	{
		CItemPondere *pItem = m_ItemsPonderes.GetItem().get();
		if(pItem->IsRegistered())
		{
			bool bAdd = true;
			for(lstTerrains.MoveFirst(); lstTerrains.IsValid(); lstTerrains.MoveNext())
				if(!pItem->IsValid(lstTerrains.GetItem()))
					bAdd = false;
			if(bAdd)
				vItems.AddTail() = pItem->AsIItem();
		}
	}
	return true;
}


// Get a vector of Criteres induite by BlocID
JBool	CItemsConstruitsBase::GetQuestionsInduiteByBlocID(JList<const IQuestion *> &vItems, JInt32 BlocID, CTerrain *pTerrain) const
{
	if(!BlocID)
	{
		for (m_QuestionInds.MoveFirst(); m_QuestionInds.IsValid(); m_QuestionInds.MoveNext())
		{
			CQuestionInduite * pItem = m_QuestionInds.GetItem().get();
			if(pItem->IsRegistered())
				if(!pTerrain || pItem->IsValid(pTerrain))
					vItems.AddTail() = pItem->AsIQuestion();
		}
	}
	else
	{
		for (m_QuestionInds.MoveFirst(); m_QuestionInds.IsValid(); m_QuestionInds.MoveNext())
		{
			CQuestionInduite * pItem = m_QuestionInds.GetItem().get();
			if(pItem->IsRegistered() && pItem->GetBlocID() == BlocID)
				if(!pTerrain || pItem->IsValid(pTerrain))
					vItems.AddTail() = pItem->AsIQuestion();
		}
	}
	return true;
}

JBool	CItemsConstruitsBase::GetQuestionsInduiteByBlocID(JList<const IQuestion *> &vItems, const CString & Filter, JInt32 BlocID, CTerrain *pTerrain, int iModality) const
{
	CString xFilter = Filter;
	Convert::ToSearchString(xFilter);

	if(!BlocID)
	{
		for (m_QuestionInds.MoveFirst(); m_QuestionInds.IsValid(); m_QuestionInds.MoveNext())
		{
			CQuestionInduite * pItem = m_QuestionInds.GetItem().get();
			if(pItem->IsRegistered())
			{
				if(!pTerrain || pItem->IsValid(pTerrain))
				{
					switch(iModality)
					{
						// Filter Question, Elements and Modalities
					case 1:
						{
							if(Convert::ToSearchString(pItem->GetLabel().AsJCharPtr()).Find(xFilter) != -1)
							{
								vItems.AddTail() = pItem->AsIQuestion();
								break;
							}
						}
						// Filter Elements and Modalities only
					case 3:
						{
							JVector<const IModalite *> Vector;
							pItem->GetIModalites(Vector);
							for(Vector.MoveFirst(); Vector.IsValid(); Vector.MoveNext())
								if(Convert::ToSearchString(Vector.GetItem()->GetLabel().AsJCharPtr()).Find(xFilter) != -1)
									break;

							if(Vector.IsValid())
								vItems.AddTail() = pItem->AsIQuestion();
						}
						break;
						// Filter Questions only
					case 2:
						{
							if(Convert::ToSearchString(pItem->GetLabel().AsJCharPtr()).Find(xFilter) != -1)
								vItems.AddTail() = pItem->AsIQuestion();
						}
						break;
						// No filter applied
					default:
						{
							vItems.AddTail() = pItem->AsIQuestion();
						}
						break;
					}
				}
			}
		}
	}
	else
	{
		for (m_QuestionInds.MoveFirst(); m_QuestionInds.IsValid(); m_QuestionInds.MoveNext())
		{
			CQuestionInduite * pItem = m_QuestionInds.GetItem().get();
			if(pItem->IsRegistered() && pItem->GetBlocID() == BlocID)
			{
				if(!pTerrain || pItem->IsValid(pTerrain))
				{
					switch(iModality)
					{
						// Filter Question, Elements and Modalities
					case 1:
						{
							if(Convert::ToSearchString(pItem->GetLabel().AsJCharPtr()).Find(xFilter) != -1)
							{
								vItems.AddTail() = pItem->AsIQuestion();
								break;
							}
						}
						// Filter Elements and Modalities only
					case 3:
						{
							JVector<const IModalite *> Vector;
							pItem->GetIModalites(Vector);
							for(Vector.MoveFirst(); Vector.IsValid(); Vector.MoveNext())
								if(Convert::ToSearchString(Vector.GetItem()->GetLabel().AsJCharPtr()).Find(xFilter) != -1)
									break;

							if(Vector.IsValid())
								vItems.AddTail() = pItem->AsIQuestion();
						}
						break;
						// Filter Questions only
					case 2:
						{
							if(Convert::ToSearchString(pItem->GetLabel().AsJCharPtr()).Find(xFilter) != -1)
								vItems.AddTail() = pItem->AsIQuestion();
						}
						break;
						// No filter applied
					default:
						{
							vItems.AddTail() = pItem->AsIQuestion();
						}
						break;
					}
				}
			}
		}
	}
	return true;
}

// Get a vector of Criteres Induite by BlocID
JBool	CItemsConstruitsBase::GetQuestionsInduiteByBlocID(JList<const IItem *> &vItems, JInt32 BlocID, CTerrain *pTerrain) const
{
	if(!BlocID)
	{
		for (m_QuestionInds.MoveFirst(); m_QuestionInds.IsValid(); m_QuestionInds.MoveNext())
		{
			CQuestionInduite * pItem = m_QuestionInds.GetItem().get();
			if(pItem->IsRegistered())
				if(!pTerrain || pItem->IsValid(pTerrain))
					vItems.AddTail() = pItem->AsIItem();
		}
	}
	else
	{
		for (m_QuestionInds.MoveFirst(); m_QuestionInds.IsValid(); m_QuestionInds.MoveNext())
		{
			CQuestionInduite * pItem = m_QuestionInds.GetItem().get();
			if(pItem->IsRegistered() && pItem->GetBlocID() == BlocID)
				if(!pTerrain || pItem->IsValid(pTerrain))
					vItems.AddTail() = pItem->AsIItem();
		}
	}
	return true;
}

// Get a vector of Criteres Modifie by BlocID 
JBool	CItemsConstruitsBase::GetCriteresModifieByBlocID(JList<const IQuestion *> &vItems, JInt32 BlocID, CTerrain *pTerrain) const
{
	if(!BlocID)
	{
		for (m_CritereMods.MoveFirst(); m_CritereMods.IsValid(); m_CritereMods.MoveNext())
		{
			CCritereModifie * pItem = m_CritereMods.GetItem().get();
			if(pItem->IsRegistered())
				if(!pTerrain || pItem->IsValid(pTerrain))
					vItems.AddTail() = pItem->AsIQuestion();
		}
	}
	else
	{
		for (m_CritereMods.MoveFirst(); m_CritereMods.IsValid(); m_CritereMods.MoveNext())
		{
			CCritereModifie * pItem = m_CritereMods.GetItem().get();
			if(pItem->IsRegistered() && pItem->GetBlocID() == BlocID)
				if(!pTerrain || pItem->IsValid(pTerrain))
					vItems.AddTail() = m_CritereMods.GetItem()->AsIQuestion();
		}
	}
	return true;
}

// Get a vector of Criteres Modifie by BlocID 
JBool	CItemsConstruitsBase::GetCriteresModifieByBlocID(JList<const IQuestion *> &vItems, const CString & Filter, JInt32 BlocID, CTerrain *pTerrain, int iModality) const
{
	CString xFilter = Filter;
	Convert::ToSearchString(xFilter);

	if(!BlocID)
	{
		for (m_CritereMods.MoveFirst(); m_CritereMods.IsValid(); m_CritereMods.MoveNext())
		{
			CCritereModifie * pItem = m_CritereMods.GetItem().get();
			if(pItem->IsRegistered())
			{
				if(!pTerrain || pItem->IsValid(pTerrain))
				{
					switch(iModality)
					{
						// Filter Question, Elements and Modalities
					case 1:
						{
							if(Convert::ToSearchString(pItem->GetLabel().AsJCharPtr()).Find(xFilter) != -1)
							{
								vItems.AddTail() = pItem->AsIQuestion();
								break;
							}
						}
						// Filter Elements and Modalities only
					case 3:
						{
							JVector<const IModalite *> Vector;
							pItem->GetIModalites(Vector);
							for(Vector.MoveFirst(); Vector.IsValid(); Vector.MoveNext())
								if(Convert::ToSearchString(Vector.GetItem()->GetLabel().AsJCharPtr()).Find(xFilter) != -1)
									break;

							if(Vector.IsValid())
								vItems.AddTail() = pItem->AsIQuestion();
						}
						break;
						// Filter Questions only
					case 2:
						{
							if(Convert::ToSearchString(pItem->GetLabel().AsJCharPtr()).Find(xFilter) != -1)
								vItems.AddTail() = pItem->AsIQuestion();
						}
						break;
						// No filter applied
					default:
						{
							vItems.AddTail() = pItem->AsIQuestion();
						}
						break;
					}
				}
			}
		}
	}
	else
	{
		for (m_CritereMods.MoveFirst(); m_CritereMods.IsValid(); m_CritereMods.MoveNext())
		{
			CCritereModifie * pItem = m_CritereMods.GetItem().get();
			if(pItem->IsRegistered() && pItem->GetBlocID() == BlocID)
			{
				if(!pTerrain || pItem->IsValid(pTerrain))
				{
					switch(iModality)
					{
						// Filter Question, Elements and Modalities
					case 1:
						{
							if(Convert::ToSearchString(pItem->GetLabel().AsJCharPtr()).Find(xFilter) != -1)
							{
								vItems.AddTail() = pItem->AsIQuestion();
								break;
							}
						}
						// Filter Elements and Modalities only
					case 3:
						{
							JVector<const IModalite *> Vector;
							pItem->GetIModalites(Vector);
							for(Vector.MoveFirst(); Vector.IsValid(); Vector.MoveNext())
								if(Convert::ToSearchString(Vector.GetItem()->GetLabel().AsJCharPtr()).Find(xFilter) != -1)
									break;

							if(Vector.IsValid())
								vItems.AddTail() = pItem->AsIQuestion();
						}
						break;
						// Filter Questions only
					case 2:
						{
							if(Convert::ToSearchString(pItem->GetLabel().AsJCharPtr()).Find(xFilter) != -1)
								vItems.AddTail() = pItem->AsIQuestion();
						}
						break;
						// No filter applied
					default:
						{
							vItems.AddTail() = pItem->AsIQuestion();
						}
						break;
					}
				}
			}
		}
	}
	return true;
}

// Get a vector of Criteres Modifie by BlocID 
JBool	CItemsConstruitsBase::GetCriteresModifieByBlocID(JList<const IItem *> &vItems, JInt32 BlocID, CTerrain *pTerrain) const
{
	if(!BlocID)
	{
		for (m_CritereMods.MoveFirst(); m_CritereMods.IsValid(); m_CritereMods.MoveNext())
		{
			CCritereModifie * pItem = m_CritereMods.GetItem().get();
			if(pItem->IsRegistered())
				if(!pTerrain || pItem->IsValid(pTerrain))
					vItems.AddTail() = pItem->AsIItem();
		}
	}
	else
	{
		for (m_CritereMods.MoveFirst(); m_CritereMods.IsValid(); m_CritereMods.MoveNext())
		{
			CCritereModifie * pItem = m_CritereMods.GetItem().get();
			if(pItem->IsRegistered() && pItem->GetBlocID() == BlocID)
				if(!pTerrain || pItem->IsValid(pTerrain))
					vItems.AddTail() = m_CritereMods.GetItem()->AsIItem();
		}
	}
	return true;
}

// Get a vector of Criteres Quantile by BlocID
JBool	CItemsConstruitsBase::GetCriteresQuantileByBlocID(JList<const IQuestion *> &vItems, JInt32 BlocID, CTerrain *pTerrain) const
{
	if(!BlocID)
	{
		for (m_CritereQuants.MoveFirst(); m_CritereQuants.IsValid(); m_CritereQuants.MoveNext())
		{
			CCritereQuantile * pItem = m_CritereQuants.GetItem().get();
			if(pItem->IsRegistered())
				if(!pTerrain || pItem->IsValid(pTerrain))
					vItems.AddTail() = pItem->AsIQuestion();
		}
	}
	else
	{
		for (m_CritereQuants.MoveFirst(); m_CritereQuants.IsValid(); m_CritereQuants.MoveNext())
		{
			CCritereQuantile * pItem = m_CritereQuants.GetItem().get();
			if(pItem->IsRegistered() && pItem->GetBlocID() == BlocID)
				if(!pTerrain || pItem->IsValid(pTerrain))
					vItems.AddTail() = pItem->AsIQuestion();
		}
	}
	return true;
}

// Get a vector of Criteres Quantile by BlocID
JBool	CItemsConstruitsBase::GetCriteresQuantileByBlocID(JList<const IQuestion *> &vItems, const CString & Filter, JInt32 BlocID, CTerrain *pTerrain, int iModality) const
{
	CString xFilter = Filter;
	Convert::ToSearchString(xFilter);

	if(!BlocID)
	{
		for (m_CritereQuants.MoveFirst(); m_CritereQuants.IsValid(); m_CritereQuants.MoveNext())
		{
			CCritereQuantile * pItem = m_CritereQuants.GetItem().get();
			if(pItem->IsRegistered())
			{
				if(!pTerrain || pItem->IsValid(pTerrain))
				{
					switch(iModality)
					{
						// Filter Question, Elements and Modalities
					case 1:
						{
							if(Convert::ToSearchString(pItem->GetLabel().AsJCharPtr()).Find(xFilter) != -1)
							{
								vItems.AddTail() = pItem->AsIQuestion();
								break;
							}
						}
						// Filter Elements and Modalities only
					case 3:
						{
							JVector<const IModalite *> Vector;
							pItem->GetIModalites(Vector);
							for(Vector.MoveFirst(); Vector.IsValid(); Vector.MoveNext())
								if(Convert::ToSearchString(Vector.GetItem()->GetLabel().AsJCharPtr()).Find(xFilter) != -1)
									break;

							if(Vector.IsValid())
								vItems.AddTail() = pItem->AsIQuestion();
						}
						break;
						// Filter Questions only
					case 2:
						{
							if(Convert::ToSearchString(pItem->GetLabel().AsJCharPtr()).Find(xFilter) != -1)
								vItems.AddTail() = pItem->AsIQuestion();
						}
						break;
						// No filter applied
					default:
						{
							vItems.AddTail() = pItem->AsIQuestion();
						}
						break;
					}
				}
			}
		}
	}
	else
	{
		for (m_CritereQuants.MoveFirst(); m_CritereQuants.IsValid(); m_CritereQuants.MoveNext())
		{
			CCritereQuantile * pItem = m_CritereQuants.GetItem().get();
			if(pItem->IsRegistered() && pItem->GetBlocID() == BlocID)
			{
				if(!pTerrain || pItem->IsValid(pTerrain))
				{
					switch(iModality)
					{
						// Filter Question, Elements and Modalities
					case 1:
						{
							if(Convert::ToSearchString(pItem->GetLabel().AsJCharPtr()).Find(xFilter) != -1)
							{
								vItems.AddTail() = pItem->AsIQuestion();
								break;
							}
						}
						// Filter Elements and Modalities only
					case 3:
						{
							JVector<const IModalite *> Vector;
							pItem->GetIModalites(Vector);
							for(Vector.MoveFirst(); Vector.IsValid(); Vector.MoveNext())
								if(Convert::ToSearchString(Vector.GetItem()->GetLabel().AsJCharPtr()).Find(xFilter) != -1)
									break;

							if(Vector.IsValid())
								vItems.AddTail() = pItem->AsIQuestion();
						}
						break;
						// Filter Questions only
					case 2:
						{
							if(Convert::ToSearchString(pItem->GetLabel().AsJCharPtr()).Find(xFilter) != -1)
								vItems.AddTail() = pItem->AsIQuestion();
						}
						break;
						// No filter applied
					default:
						{
							vItems.AddTail() = pItem->AsIQuestion();
						}
						break;
					}
				}
			}
		}
	}
	return true;
}

// Get a vector of Criteres Quantile by BlocID
JBool	CItemsConstruitsBase::GetCriteresQuantileByBlocID(JList<const IItem *> &vItems, JInt32 BlocID, CTerrain *pTerrain) const
{
	if(!BlocID)
	{
		for (m_CritereQuants.MoveFirst(); m_CritereQuants.IsValid(); m_CritereQuants.MoveNext())
		{
			CCritereQuantile * pItem = m_CritereQuants.GetItem().get();
			if(pItem->IsRegistered())
				if(!pTerrain || pItem->IsValid(pTerrain))
					vItems.AddTail() = pItem->AsIItem();
		}
	}
	else
	{
		for (m_CritereQuants.MoveFirst(); m_CritereQuants.IsValid(); m_CritereQuants.MoveNext())
		{
			CCritereQuantile * pItem = m_CritereQuants.GetItem().get();
			if(pItem->IsRegistered() && pItem->GetBlocID() == BlocID)
				if(!pTerrain || pItem->IsValid(pTerrain))
					vItems.AddTail() = pItem->AsIItem();
		}
	}
	return true;
}

// Get constant pointer to an ItemConstruit
const CItemConstruit *	CItemsConstruitsBase::GetItemIndependant(const JLabel & Label) const
{
	// Passe le label en majuscule pour tester les doublons
	CString LABEL		= Label.AsJCharPtr();
	LABEL.MakeUpper(); 

	// balaye tous les items indépendants
	for (m_ItemsIndependant.MoveFirst(); m_ItemsIndependant.IsValid(); m_ItemsIndependant.MoveNext())
	{
		m_ItemsConstruits.MoveTo(m_ItemsIndependant.GetItem());
		if(m_ItemsConstruits.IsValid())
		{
			// Récupére l'item courant
			const CItemConstruit * pItem = m_ItemsConstruits.GetItem().get();

			// Passe le label courant en majuscule pour tester les doublons
			CString LABELCUR	= pItem->GetLabel().AsJCharPtr();
			LABELCUR.MakeUpper(); 

			// Vérifie existence et non doublons
			if(pItem->IsRegistered() &&  LABEL == LABELCUR)
				return pItem;
		}
		else
			return 0;
	}
	return 0;
}

/// Get pointer to an ItemConstruit
const CItemConstruit *		CItemsConstruitsBase::GetItemConstruit(const JLabel & Label) const
{
	for (m_ItemsConstruits.MoveFirst(); m_ItemsConstruits.IsValid(); m_ItemsConstruits.MoveNext())
	{
		const CItemConstruit * pItem = m_ItemsConstruits.GetItem().get();
		if(pItem->IsRegistered() &&  Label == pItem->GetLabel())
			return pItem;
	}
	return 0;
}

// Get constant pointer to a Palette
const CPalettes *	CItemsConstruitsBase::GetPalette(const JLabel & Label) const
{
	// Passe le label en majuscule pour tester les doublons
	CString LABEL		= Label.AsJCharPtr();
	LABEL.MakeUpper(); 

	// Balaye toutes les palettes
	for (m_Palettes.MoveFirst(); m_Palettes.IsValid(); m_Palettes.MoveNext())
	{
		// Passe les labels n majuscule pour éviter les doublons
		CString LABELCUR	= m_Palettes.GetItem()->GetLabel().AsJCharPtr() ;
		LABELCUR.MakeUpper(); 

		if(m_Palettes.GetItem()->IsRegistered() &&  LABEL == LABELCUR)
			return m_Palettes.GetItem()->AsPalette();
	}
	return 0;
}

/////////////////////////////////////////////////////////////////////////////////////////////////
// Renvoi le nombre d'items pondérés existants
const JUnt32 CItemsConstruitsBase::GetNbItemsPonderes()
{
	return (m_ItemsPonderes.GetCount());
}

/// Get constant pointer to a Ponderate item
const CItemPondere *		CItemsConstruitsBase::GetItemPondere		(const JLabel & Label) const
{
	// Passe le label en majuscule pour tester les doublons
	CString LABEL		= Label.AsJCharPtr();
	LABEL.MakeUpper(); 

	// Balaye tous les items pondérés
	for (m_ItemsPonderes.MoveFirst(); m_ItemsPonderes.IsValid(); m_ItemsPonderes.MoveNext())
	{
		// Passe les labels n majuscule pour éviter les doublons
		CString LABELCUR	= m_ItemsPonderes.GetItem()->GetLabel().AsJCharPtr() ;
		LABELCUR.MakeUpper(); 

		if(m_ItemsPonderes.GetItem()->IsRegistered() &&  LABEL == LABELCUR)
			return m_ItemsPonderes.GetItem()->AsItemPondere();
	}
	return 0;
}

// Get constant pointer to a Critere Induite
const CQuestionInduite *	CItemsConstruitsBase::GetQuestionInduite(const JLabel & Label) const
{
	// Passe le label en majuscule pour tester les doublons
	CString LABEL		= Label.AsJCharPtr();
	LABEL.MakeUpper(); 

	// Balaye toutes les questions induites
	for (m_QuestionInds.MoveFirst(); m_QuestionInds.IsValid(); m_QuestionInds.MoveNext())
	{
		// Passe les labels n majuscule pour éviter les doublons
		CString LABELCUR	= m_QuestionInds.GetItem()->GetLabel().AsJCharPtr();
		LABELCUR.MakeUpper(); 

		if(m_QuestionInds.GetItem()->IsRegistered() && LABEL == LABELCUR)
			return m_QuestionInds.GetItem()->AsQuestionInduite();
	}
	return 0;
}

// Get constant pointer to a Critere Modifié
const CCritereModifie *	CItemsConstruitsBase::GetCritereModifie(const JLabel & Label) const
{
	// Passe le label en majuscule à identifier pour tester les doublons
	CString LABEL		= Label.AsJCharPtr();
	LABEL.MakeUpper(); 

	// Balaye tous les critères modifiés
	for (m_CritereMods.MoveFirst(); m_CritereMods.IsValid(); m_CritereMods.MoveNext())
	{
		// Passe le label courant en majuscule pour tester les doublons
		CString LABELCUR	= m_CritereMods.GetItem()->GetLabel().AsJCharPtr();
		LABELCUR.MakeUpper(); 

		if(m_CritereMods.GetItem()->IsRegistered() &&  LABEL == LABELCUR)
			return m_CritereMods.GetItem()->AsCritereModifie();
	}
	return 0;
}

// Get constant pointer to a Critere Quantile
const CCritereQuantile *	CItemsConstruitsBase::GetCritereQuantile(const JLabel & Label) const
{
	// Passe le label en majuscule pour tester les doublons
	CString LABEL		= Label.AsJCharPtr();
	LABEL.MakeUpper(); 

	// Balaye tosu les quantiles
	for (m_CritereQuants.MoveFirst(); m_CritereQuants.IsValid(); m_CritereQuants.MoveNext())
	{
		// Passe le label en majuscule pour tester les doublons
		CString LABELCUR		= m_CritereQuants.GetItem()->GetLabel().AsJCharPtr();
		LABELCUR.MakeUpper(); 

		if(m_CritereQuants.GetItem()->IsRegistered() &&  LABEL == LABELCUR)
			return m_CritereQuants.GetItem()->AsCritereQuantile();
	}
	return 0;
}

// Get constant pointer to a Critere Score
const CCritereScore *	CItemsConstruitsBase::GetCritereScore(const JLabel & Label) const
{
	for (m_CritereScores.MoveFirst(); m_CritereScores.IsValid(); m_CritereScores.MoveNext())
	{
		if(m_CritereScores.GetItem()->IsRegistered() &&  Label == m_CritereScores.GetItem()->GetLabel())
			return m_CritereScores.GetItem()->AsCritereScore();
	}
	return 0;
}

// Get constant pointer to a IQuestion
IQuestion *	CItemsConstruitsBase::GetQuestionByID(JInt32 ID) const
{
	// If item construit or other without the IQuestion interface return null
	if(ID <= m_MaxItem)
		return 0;

	// Return the palette
	if(ID >= PALETTES_BASE && ID <= m_MaxPalettes)
		return GetPalette(ID); 

	// Return the QuestionInduite
	if(ID >= QUESTIONIND_BASE && ID <=m_MaxQuestionInd)
		return GetQuestionInduite(ID);

	// return the critere modifie
	if(ID >= CRITEREMOD_BASE && ID <= m_MaxCritereMod)
		return GetCritereModifie(ID);

	// return the critere quantile
	if(ID >= CRITEREQUANT_BASE && ID <= m_MaxCritereQuant)
		return GetCritereQuantile(ID);

	// return the critere score
	if(ID >= CRITERESCORE_BASE && ID <= m_MaxCritereScore)
		return GetCritereScore(ID);

	// return the item pondéré
	if(ID >= ITEMPONDERE_BASE && ID <= m_MaxItemPondere)
		return 0;

	return 0;
}

/// Gets a pointer to an IItem Object with the given Id
IItem * CItemsConstruitsBase::GetIItemByID(JInt32 ID) const
{
	// Return the Item Construit
	if(ID >= ITEM_BASE && ID <= m_MaxItem)
		return GetItemConstruit(ID);

	// Return the palette
	if(ID >= PALETTES_BASE && ID <= m_MaxPalettes)
		return GetPalette(ID); 

	// Return the QuestionInduite
	if(ID >= QUESTIONIND_BASE && ID <=m_MaxQuestionInd)
		return GetQuestionInduite(ID);

	// return the critere modifie
	if(ID >= CRITEREMOD_BASE && ID <= m_MaxCritereMod)
		return GetCritereModifie(ID);

	// return the critere quantile
	if(ID >= CRITEREQUANT_BASE && ID <= m_MaxCritereQuant)
		return GetCritereQuantile(ID);

	// return the critere score
	if(ID >= CRITERESCORE_BASE && ID <= m_MaxCritereScore)
		return GetCritereScore(ID);

	// return the item pondéré
	if(ID >= ITEMPONDERE_BASE && ID <= m_MaxItemPondere)
		return GetItemPondere(ID);

	return 0;
}

JVoid CItemsConstruitsBase::CreateEnsemble()
{
	CItemConstruit * pItem = GetNewItemConstruit();
	pItem->m_IBString = "&();";
	pItem->m_ABString = "&(S&(Y|(r1)));";
	pItem->m_Label = _GETSTRING(IDS_ENSEMBLE);
	pItem->m_lSourceID = 0;
	pItem->m_lTerrainID = 0;
	RegisterItemConstruit(pItem,SYSTEM_ID);
	delete pItem;
}


// Export a list of Items / Palettes / Classes / Quantiles etc...
JBool CItemsConstruitsBase::Export(JList<JInt32> & IdList, CWnd* pParent)
{
	CFileDialog Dlg(
		FALSE,
		_T(".ric"),
		NULL,
		OFN_NOCHANGEDIR | OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
		_T("Fichiers Patrimoine (*.ric)|*.ric|Tous les fichiers (*.*)|*.*||"), pParent);

	if(Dlg.DoModal ()!=IDOK)
		return false;

	CStdioFile ExportFile;
	CFileException Error = 0;
	if(!ExportFile.Open(Dlg.GetPathName(),CFile::modeCreate|CFile::modeWrite|CFile::typeText,&Error))
		return false;

	JMap<JInt32,JBool> ExpMap;
	return Export(ExportFile, IdList, ExpMap);
}

JBool CItemsConstruitsBase::Export(CStdioFile & ExportFile, JList<JInt32> & IdList, JMap<JInt32,JBool> & ExpMap)
{

	// Map output objects to make sure we dont rewrite them
	for(IdList.MoveFirst(); IdList.IsValid (); IdList.MoveNext())
	{
		JInt32 ItemId = IdList.GetItem();
		if(ItemId == ITEM_BASE)
			continue;
		ExpMap.MoveTo(ItemId);
		if(!ExpMap.IsValid())
		{
			IItem * pItem = GetIItemByID(ItemId);
			if(pItem)
				pItem->Export(ExportFile, ExpMap);
		}
	}
	ExportFile.WriteString(_T("END_EXPORT\n")); 
	return true;
}

JBool CItemsConstruitsBase::Export(CStdioFile & ExportFile, const IItem *pItem, JMap<JInt32,JBool> & ExpMap)
{
	// Map output objects to make sure we dont rewrite them
	if(pItem->GetID() != ITEM_BASE)
	{
		ExpMap.MoveTo(pItem->GetID());
		if(!ExpMap.IsValid())
			pItem->Export(ExportFile, ExpMap);
	}
	ExportFile.WriteString(_T("END_EXPORT\n")); 
	return true;
}

// Export à partir d'un export Archive
JBool CItemsConstruitsBase::Export(CArchive & ExportArchive, JList<JInt32> & IdList, JMap<JInt32,JBool> & ExpMap)
{
	// Map output objects to make sure we dont rewrite them
	for(IdList.MoveFirst(); IdList.IsValid (); IdList.MoveNext())
	{
		JInt32 ItemId = IdList.GetItem();
		if(ItemId == ITEM_BASE)
			continue;
		ExpMap.MoveTo(ItemId);
		if(!ExpMap.IsValid())
		{
			IItem * pItem = GetIItemByID(ItemId);
			if(pItem)
				pItem->Export(ExportArchive, ExpMap);
		}
	}

	// Chaine Fin export
	JMFCStream* pstream = NULL;
	pstream = JMFCStream::Create(ExportArchive);

	// CString Txt = "END_EXPORT\n";
	JLabel TxtFin;
	TxtFin = "END_EXPORT";
	TxtFin.Send(*pstream);

	// Export OK
	delete pstream;
	return true;
}

JBool CItemsConstruitsBase::Export(CArchive & ExportArchive, const IItem *pItem, JMap<JInt32,JBool> & ExpMap)
{
	// Map output objects to make sure we dont rewrite them
	if(pItem->GetID() != ITEM_BASE)
	{
		ExpMap.MoveTo(pItem->GetID());
		if(!ExpMap.IsValid())
			pItem->Export(ExportArchive, ExpMap);
	}

	// Chaine Fin export
	JMFCStream* pstream = NULL;
	pstream = JMFCStream::Create(ExportArchive);
	JLabel TxtFin;
	TxtFin = "END_EXPORT";
	TxtFin.Send(*pstream);

	// Export OK
	delete pstream;
	return true;
}

// Import a file of exported items / Palettes / Classes / Quantiles ...
JBool CItemsConstruitsBase::Import(CWnd* pParent)
{
	CFileDialog Dlg(
		TRUE,
		_T(".ric"),
		NULL,
		OFN_NOCHANGEDIR | OFN_FILEMUSTEXIST | OFN_HIDEREADONLY,
		_T("Fichiers Patrimoine (*.ric)|*.ric|Tous les fichiers (*.*)|*.*||"), pParent);

	if(Dlg.DoModal ()!=IDOK)
		return false;

	CStdioFile ImportFile;
	CFileException Error = 0;
	if(!ImportFile.Open (Dlg.GetPathName(),CFile::modeRead | CFile::typeText,&Error))
		return false;

	// Map input objects to be able to renumber them later
	JMap<JInt32,const IItem *> ImpMap;

	return Import(ImportFile,ImpMap);
}

JBool CItemsConstruitsBase::Import(CStdioFile & ImportFile, JMap<JInt32,const IItem *> &ImpMap)
{
	JInt32 Id		= 0;
	CString Type	= "";

	while(ImportFile.ReadString(Type))
	{
		IItem *pItem	= 0;
		Id				= 0;

		// Import finished
		if(Type.Find("END_EXPORT")!=-1)
			break;

		// Import an item
		if(Type.Find("ITEM")!=-1 || Type.Find("TEMPITEM")!=-1)
		{
			// Get new item
			CItemConstruit * pCItem = GetNewItemConstruit();
			if(pCItem)
			{
				// Import the item
				if(Type.Find("TEMPITEM")!=-1)
				{
					if(pCItem->Import(ImportFile,ImpMap,true))
						continue;
				}
				else
				{
					if(pCItem->Import(ImportFile,ImpMap))
						continue;
				}
				delete pCItem;
			}
			return false;
		}
		else if(Type.Find("PALETTE")!=-1)
		{
			pItem = (IItem *)GetNewPalette(); 
			if(pItem)
			{
				// Import the item
				if(pItem->Import(ImportFile,ImpMap))
					continue;

				delete pItem;
			}
			return false;
		}
		else if(Type.Find("CLASS")!=-1)
		{
			pItem = (IItem *)GetNewCritereModifie(); 
			if(pItem)
			{
				// Import the item
				if(pItem->Import(ImportFile,ImpMap))
					continue;

				delete pItem;
			}
			return false;
		}
		else if(Type.Find("QUANTILE")!=-1)
		{
			pItem = (IItem *)GetNewCritereQuantile();
			if(pItem)
			{
				// Import the item
				if(pItem->Import(ImportFile,ImpMap))
					continue;

				delete pItem;
			}
			return false;
		}
		else if(Type.Find("INDUITE")!=-1)
		{
			pItem = (IItem *)GetNewQuestionInduite();
			if(pItem)
			{
				// Import the item
				if(pItem->Import(ImportFile,ImpMap))
					continue;

				delete pItem;
			}
			return false;
		}
		else
			return false;
	}
	return true;
}

/////////////////////////////////////////////////////////////////////////////
// Lecture Label
// CString CItemsConstruitsBase::ReadStreamLabel(JMFCStream &stream, JLabel &Type)
CString CItemsConstruitsBase::ReadStreamLabel(JStream &stream, JLabel &Type)
{
	// Lecture label
	Type.Recv(stream);

	// Renvoi la chaine de caractère
	return (Type.AsJCharPtr());  
}

JBool CItemsConstruitsBase::Import(CArchive & ImportArchive, JMap<JInt32,const IItem *> &ImpMap)
{
	JInt32 Id		= 0;
	JLabel Type		= "";

	// Chaine Fin export
	JMFCStream* pstream = NULL;
	pstream = JMFCStream::Create(ImportArchive);

	// Boucle sur la totalité de l'export
	// Type.Recv(*pstream);
	while(ReadStreamLabel(*pstream,Type)!= "END_EXPORT")
	{
		//Texte trouvé
		CString TxtType = Type.AsJCharPtr();

		IItem *pItem	= 0;
		Id				= 0;

		// Import finished
		if(TxtType.Find("END_EXPORT")!=-1)
			break;

		// Import an item
		if(TxtType.Find("ITEM")!=-1 || TxtType.Find("TEMPITEM")!=-1)
		{
			// Get new item
			CItemConstruit * pCItem = GetNewItemConstruit();
			if(pCItem)
			{
				// Import the item
				if(TxtType.Find("TEMPITEM")!=-1)
				{
					if(pCItem->Import(ImportArchive,ImpMap,true))
						continue;
				}
				else
				{
					if(pCItem->Import(ImportArchive,ImpMap))
						continue;
				}
				delete pCItem;
			}
			delete pstream;
			return false;
		}
		else if(TxtType.Find("PALETTE")!=-1)
		{
			pItem = (IItem *)GetNewPalette(); 
			if(pItem)
			{
				// Import the item
				if(pItem->Import(ImportArchive,ImpMap))
					continue;

				delete pItem;
			}
			delete pstream;
			return false;
		}
		else if(TxtType.Find("CLASS")!=-1)
		{
			pItem = (IItem *)GetNewCritereModifie(); 
			if(pItem)
			{
				// Import the item
				if(pItem->Import(ImportArchive,ImpMap))
					continue;

				delete pItem;
			}
			delete pstream;
			return false;
		}
		else if(TxtType.Find("QUANTILE")!=-1)
		{
			pItem = (IItem *)GetNewCritereQuantile();
			if(pItem)
			{
				// Import the item
				if(pItem->Import(ImportArchive,ImpMap))
					continue;

				delete pItem;
			}
			delete pstream;
			return false;
		}
		else if(TxtType.Find("INDUITE")!=-1)
		{
			pItem = (IItem *)GetNewQuestionInduite();
			if(pItem)
			{
				// Import the item
				if(pItem->Import(ImportArchive,ImpMap))
					continue;

				delete pItem;
			}
			return false;
		}
		else
			return false;

		// Passe à la chaine suivante
		// Type.Recv(*pstream);

	} // End while

	delete pstream;
	return true;
}

/// Check if new item already exists / returns the Id of the existing version
JInt32		CItemsConstruitsBase::CheckExists(CItemConstruit * pItem)
{
	// Try finding the same Id
	const CItemConstruit * pFound = GetItemConstruit(pItem->GetID());
	// If the Same Id check the configuration
	if(pFound && pFound->IsSimilaire(pItem))
		return pFound->GetID();

	pFound = GetItemConstruit(pItem->GetLabel());
	// If the Same Id check the configuration
	if(pFound && pFound->IsSimilaire(pItem))
		return pFound->GetID();

	for(m_ItemsConstruits.MoveFirst(); m_ItemsConstruits.IsValid(); m_ItemsConstruits.MoveNext())
		if(m_ItemsConstruits.GetItem()->IsSimilaire(pItem))
			return m_ItemsConstruits.GetItem()->GetID();

	return 0;
}

/// Check if new palette already exists / returns the Id of the existing version
JInt32		CItemsConstruitsBase::CheckExists(CPalettes * pItem)
{
	// Try finding the same Id
	const CPalettes * pFound = GetPalette(pItem->GetID());
	// If the Same Id check the configuration
	if(pFound && pFound->IsSimilaire(pItem))
		return pFound->GetID();

	pFound = GetPalette(pItem->GetLabel());
	// If the Same Id check the configuration
	if(pFound && pFound->IsSimilaire(pItem))
		return pFound->GetID();

	for(m_Palettes.MoveFirst(); m_Palettes.IsValid(); m_Palettes.MoveNext())
		if(m_Palettes.GetItem()->IsSimilaire(pItem))
			return m_Palettes.GetItem()->GetID();

	return 0;
}

////////////////////////////////////////////////////////////////////////////////////////
/// Check if new item pondéré already exists / returns the Id of the existing version
JInt32 CItemsConstruitsBase::CheckExists(CItemPondere * pItem)
{
	// Try finding the same Id
	const CItemPondere * pFound = GetItemPondere(pItem->GetID());

	// If the Same Id check the configuration
	if(pFound && pFound->IsSimilaire(pItem))
		return pFound->GetID();

	pFound = GetItemPondere(pItem->GetLabel());
	// If the Same Id check the configuration
	if(pFound && pFound->IsSimilaire(pItem))
		return pFound->GetID();

	// Boucle sur les items pondérés
	for(m_ItemsPonderes.MoveFirst(); m_ItemsPonderes.IsValid(); m_ItemsPonderes.MoveNext())
		if(m_ItemsPonderes.GetItem()->IsSimilaire(pItem))
			return m_ItemsPonderes.GetItem()->GetID();

	return 0;
}

/// Check if new Critere Induite already exists / returns the Id of the existing version
JInt32		CItemsConstruitsBase::CheckExists(CQuestionInduite * pItem)
{
	// Try finding the same Id
	const CQuestionInduite * pFound = GetQuestionInduite(pItem->GetID());
	// If the Same Id check the configuration
	if(pFound && pFound->IsSimilaire(pItem))
		return pFound->GetID();

	pFound = GetQuestionInduite(pItem->GetLabel());
	// If the Same Id check the configuration
	if(pFound && pFound->IsSimilaire(pItem))
		return pFound->GetID();

	for(m_QuestionInds.MoveFirst(); m_QuestionInds.IsValid(); m_QuestionInds.MoveNext())
		if(m_QuestionInds.GetItem()->IsSimilaire(pItem))
			return m_QuestionInds.GetItem()->GetID();

	return 0;
}

/// Check if new Critere Modifié already exists / returns the Id of the existing version
JInt32		CItemsConstruitsBase::CheckExists(CCritereModifie * pItem)
{
	// Try finding the same Id
	const CCritereModifie * pFound = GetCritereModifie(pItem->GetID());
	// If the Same Id check the configuration
	if(pFound && pFound->IsSimilaire(pItem))
		return pFound->GetID();

	pFound = GetCritereModifie(pItem->GetLabel());
	// If the Same Id check the configuration
	if(pFound && pFound->IsSimilaire(pItem))
		return pFound->GetID();

	for(m_CritereMods.MoveFirst(); m_CritereMods.IsValid(); m_CritereMods.MoveNext())
		if(m_CritereMods.GetItem()->IsSimilaire(pItem))
			return m_CritereMods.GetItem()->GetID();

	return 0;
}

/// Check if new Critere Quantile already exists / returns the Id of the existing version
JInt32		CItemsConstruitsBase::CheckExists(CCritereQuantile * pItem)
{
	// Try finding the same Id
	const CCritereQuantile * pFound = GetCritereQuantile(pItem->GetID());
	// If the Same Id check the configuration
	if(pFound && pFound->IsSimilaire(pItem))
		return pFound->GetID();

	pFound = GetCritereQuantile(pItem->GetLabel());
	// If the Same Id check the configuration
	if(pFound && pFound->IsSimilaire(pItem))
		return pFound->GetID();

	for(m_CritereQuants.MoveFirst(); m_CritereQuants.IsValid(); m_CritereQuants.MoveNext())
		if(m_CritereQuants.GetItem()->IsSimilaire(pItem))
			return m_CritereQuants.GetItem()->GetID();

	return 0;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Get Upwards (retrouve tous les élmts utilisant les items de MapItems)
JVoid	CItemsConstruitsBase::GetUpwards(const JMap<JInt32, JInt32> & MapItems, JInt32 ID, JMap<JInt32, JLabel> & MapUpwards)
{
	int CptItem = 0;

	// on cherche tous les items
	for (MapItems.MoveFirst(); MapItems.IsValid(); MapItems.MoveNext())
	{
		// dans tous les items indépendants
		for (m_ItemsIndependant.MoveFirst(); m_ItemsIndependant.IsValid(); m_ItemsIndependant.MoveNext())
		{
			m_ItemsConstruits.MoveTo(m_ItemsIndependant.GetItem());

			if(m_ItemsConstruits.IsValid())
			{
				// Libelle libellé en cours
				CString Libelle = m_ItemsConstruits.GetItem()->GetLabel().AsJCharPtr();

				CptItem++;

				// Ident du libellé en cours
				JUnt32 IdItemConstruit = m_ItemsConstruits.GetItem()->GetID();

				if(m_ItemsConstruits.GetItem()->GetID() != ID && m_ItemsConstruits.GetItem()->IsRegistered() && m_ItemsConstruits.GetItem()->HasItem(MapItems.GetKey()))
				{
					// MapUpwards.MoveTo(m_ItemsConstruits.GetItem()->GetID());
					MapUpwards.MoveTo(IdItemConstruit);
					if(!MapUpwards.IsValid())
					{
						// Ajout libellé du composant
						MapUpwards.Add(IdItemConstruit) = Libelle;
					}
				}
			}

			
			/*
			if(m_ItemsConstruits.IsValid())
			{
				if(m_ItemsConstruits.GetItem()->GetID() != ID && m_ItemsConstruits.GetItem()->IsRegistered() && m_ItemsConstruits.GetItem()->HasItem(MapItems.GetKey()))
				{
					// MapUpwards.MoveTo(m_ItemsConstruits.GetItem()->GetID());
					MapUpwards.MoveTo(IdItemConstruit);
					if(!MapUpwards.IsValid())
					{
						// Ajout libellé du composant
						MapUpwards.Add(IdItemConstruit) = Libelle;
					}
				}
			}
			*/
		}
		// dans toutes les palettes
		for (m_Palettes.MoveFirst(); m_Palettes.IsValid(); m_Palettes.MoveNext())
		{
			if(m_Palettes.GetItem()->GetID() != ID && m_Palettes.GetItem()->IsRegistered() && m_Palettes.GetItem()->HasItem(MapItems.GetKey()))
			{
				MapUpwards.MoveTo(m_Palettes.GetItem()->GetID());
				if(!MapUpwards.IsValid())
					MapUpwards.Add(m_Palettes.GetItem()->GetID()) = m_Palettes.GetItem()->GetLabel();
			}
		}
		// dans toutes les questions induites
		for (m_QuestionInds.MoveFirst(); m_QuestionInds.IsValid(); m_QuestionInds.MoveNext())
		{
			if(m_QuestionInds.GetItem()->GetID() != ID && m_QuestionInds.GetItem()->IsRegistered() && m_QuestionInds.GetItem()->HasItem(MapItems.GetKey()))
			{
				MapUpwards.MoveTo(m_QuestionInds.GetItem()->GetID());
				if(!MapUpwards.IsValid())
					MapUpwards.Add(m_QuestionInds.GetItem()->GetID()) = m_QuestionInds.GetItem()->GetLabel();
			}
		}
		// dans tous les critères modifiés
		for (m_CritereMods.MoveFirst(); m_CritereMods.IsValid(); m_CritereMods.MoveNext())
		{
			if(m_CritereMods.GetItem()->GetID() != ID && m_CritereMods.GetItem()->IsRegistered() && m_CritereMods.GetItem()->HasItem(MapItems.GetKey()))
			{
				MapUpwards.MoveTo(m_CritereMods.GetItem()->GetID());
				if(!MapUpwards.IsValid())
					MapUpwards.Add(m_CritereMods.GetItem()->GetID()) = m_CritereMods.GetItem()->GetLabel();
			}
		}
		// dans tous les critères quantiles
		for (m_CritereQuants.MoveFirst(); m_CritereQuants.IsValid(); m_CritereQuants.MoveNext())
		{
			if(m_CritereQuants.GetItem()->GetID() != ID && m_CritereQuants.GetItem()->IsRegistered() && m_CritereQuants.GetItem()->HasItem(MapItems.GetKey()))
			{
				MapUpwards.MoveTo(m_CritereQuants.GetItem()->GetID());
				if(!MapUpwards.IsValid())
					MapUpwards.Add(m_CritereQuants.GetItem()->GetID()) = m_CritereQuants.GetItem()->GetLabel();
			}
		}
		// dans tous les critères scrores
		for (m_CritereScores.MoveFirst(); m_CritereScores.IsValid(); m_CritereScores.MoveNext())
		{
			if(m_CritereScores.GetItem()->GetID() != ID && m_CritereScores.GetItem()->IsRegistered() && m_CritereScores.GetItem()->HasItem(MapItems.GetKey()))
			{
				MapUpwards.MoveTo(m_CritereScores.GetItem()->GetID());
				if(!MapUpwards.IsValid())
					MapUpwards.Add(m_CritereScores.GetItem()->GetID()) = m_CritereScores.GetItem()->GetLabel();
			}
		}
	}
}

///////////////////////////////////////////////////////////////////////////////////////
/// Get Downwards (retrouve tous les élmts composant l'item ident ID)
CString CItemsConstruitsBase::GetCompoItem(JInt32 ID, CTerrain *pTerrain)
{
	// Libellé composition item
	CString LibCompo = "";

	// Récupère l'item à décortiquer
	m_ItemsConstruits.MoveTo(ID);

	if (m_ItemsConstruits.IsValid())
	{
		// Elmt item contruit
		const CItemConstruit *pItemConstruit = GetItemConstruit(ID);

		// Recherche contruction de cet item
		CIBuilder IBuilder;
		pItemConstruit->GetItemBuilder(pTerrain,IBuilder, true);   // Modif ALAIN Juillet 2014 (on force reconstruction avec repéres absolus)

		// Format texte
		CToTextIVisitor	ToTextIV;
		CString Descript;
		ToTextIV.ToDescription(&IBuilder, this, pTerrain, &Descript);

		// Format xml
		CToXMLIVisitor	ToXMLIV;
		CString TxtXML;
		ToXMLIV.ToXML(&IBuilder, this, pTerrain, &TxtXML);
		LibCompo = Descript;
	}

	// Retourne la composition de l'item
	return LibCompo;
}
