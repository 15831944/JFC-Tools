// ItemsNatifManager.cpp: implementation of the CItemsNatifManager class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ItemsNatifManager.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CItemsNatifManager::CItemsNatifManager()
{

}

CItemsNatifManager::~CItemsNatifManager()
{
}

JBool CItemsNatifManager::Load(JStringEx &Name)
{

	m_sName = Name;
	JStringEx sName = Name;
	sName.Append (".02");

	try
	{
		JArchive *pFile = JArchive::Create(sName.AsJCharPtr());

		LoadFileHeader		(pFile);
		LoadHeader			(pFile, m_FileHeader.GetVersion ());
		LoadDef				(pFile, m_FileHeader.GetVersion ());

		delete pFile;

	}
	catch(JException *pE)
	{
		return false;
	}
	return true;
}

JVoid CItemsNatifManager::LoadFileHeader(JArchive *pFile)
{
	const JInt32 CURRENT_VERSION = 1;

	m_FileHeader.Recv(*pFile);
	
	// Check compagnie name
	if(m_FileHeader.GetCompagnie () != 0x0043464A)
		throw JInternalError::GetInstance(); //("File Header, compagnie field error");

	// Verify the magic number
	if(m_FileHeader.GetMagicNumber () != 0x6FD35902)
		throw JInternalError::GetInstance(); //("File Header, Magic Number field error");

	// No compression
	if(m_FileHeader.GetCompression () != 0)
		throw JInternalError::GetInstance(); //("File Header, Compression field error");

	// No cryptage
	if(m_FileHeader.GetCryptage () != 0)
		throw JInternalError::GetInstance(); //("File Header, Cryptage field error");

	// Must not be greater than my current version
	if(m_FileHeader.GetVersion () > CURRENT_VERSION)
		throw JInternalError::GetInstance(); //("File Header, Version error");
}

JVoid CItemsNatifManager::LoadHeader(JArchive *pFile, JInt32 Version)
{
	switch(Version)
	{
		case 1:
		{
			// Load Source and Terrain ID's
			pFile->Recv(m_lSourceID);
			pFile->Recv(m_lTerrainID);
		}
		break;

		default:
			TRACE("CItemsNatifManager Version error");
			throw JInternalError::GetInstance(); //("File Version error");
		break;
	}
}

JVoid CItemsNatifManager::LoadDef(JArchive *pFile, JInt32 Version)
{
	switch(Version)
	{
		case 1:
		{
			pFile->Recv(m_lNbIndividus);
			pFile->Recv(m_lNbItemsTot);
			pFile->Recv(m_lDecoupage);
		}
		break;

		default:
			TRACE("CItemsNatifManager Version error");
			throw JInternalError::GetInstance(); //("File Version error");
		break;
	}
}

const CBitArray * CItemsNatifManager::GetVectorIndividus(JInt32 IndexItem)
{
	if(IndexItem >= m_lNbItemsTot)
		return 0;

	JInt32 Bloc		= (IndexItem / m_lDecoupage) + 1;
	JInt32 Index	= IndexItem % m_lDecoupage;


	m_mItemsNatif.MoveTo(Bloc);

	if(!m_mItemsNatif.IsValid())
	{
		PTR_ITEMSNATIF pItem(new CItemsNatif);

		if(!pItem->Load (m_sName,m_lNbIndividus, Bloc))
			return 0;

		// Insert file into variable map
		m_mItemsNatif.Add(Bloc) = pItem;
		m_mItemsNatif.MoveTo(Bloc);
	}
	
	return m_mItemsNatif.GetItem()->GetVectorIndividus(Index);
}
