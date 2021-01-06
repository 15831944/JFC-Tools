// FacteursSupports.cpp: implementation of the CFacteursSupports class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "FacteursSupports.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CFacteursSupports::CFacteursSupports()
{
	// File Name
	m_sName = "";
	// Source ID
	m_lSourceID = 0;
	// Terrain ID
	m_lTerrainID = 0;
}

CFacteursSupports::~CFacteursSupports()
{
}

JBool CFacteursSupports::Load(JStringEx &Name)
{

	m_sName = Name;
	m_sName.Append (".39");

	m_lSourceID = m_lTerrainID = 0;

	try
	{
		if (JPathManager::IsFileExists(m_sName.AsJCharPtr()))
		{
			JArchive *pFile = JArchive::Create(m_sName.AsJCharPtr());

			LoadFileHeader		(pFile);
			LoadHeader			(pFile, m_FileHeader.GetVersion ());
			LoadValSupports		(pFile, m_FileHeader.GetVersion ());

			delete pFile;
		}
		else
			m_mValSupports.Reset();
	}
	catch(JException *pE)
	{
		return false;
	}
	return true;
}

JVoid CFacteursSupports::LoadFileHeader(JArchive *pFile)
{
	const JInt32 CURRENT_VERSION = 1;

	m_FileHeader.Recv(*pFile);
	
	// Check compagnie name
	if(m_FileHeader.GetCompagnie () != 0x0043464A)
		throw JInternalError::GetInstance(); //("File Header, compagnie field error");

	// Verify the magic number
	if(m_FileHeader.GetMagicNumber () != 0x6FD35939)
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

JVoid CFacteursSupports::LoadHeader(JArchive *pFile, JInt32 Version)
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
			TRACE("FacteursSupports Version error");
			throw JInternalError::GetInstance(); //("File Version error");
		break;
	}
}

JVoid CFacteursSupports::LoadValSupports(JArchive *pFile, JInt32 Version)
{
	switch(Version)
	{
		case 1:
		{
			m_mValSupports.Reset();
			JInt32 Count;
			pFile->Recv(Count);
			for(JInt32 x=0; x < Count; x++)
			{
				PTR_VALSUPPORT pItem( new CValSupport);

				pItem->Recv(*pFile, Version);

				// Add support to the set
				m_mValSupports.Add(pItem->GetID()) = pItem;
			}
		}
		break;

		default:
			TRACE("FacteursSupports Version error");
			throw JInternalError::GetInstance(); //("File Version error");
		break;
	}
}

JBool CFacteursSupports::GetRepartionStruct(JInt32 SupportId, JInt32 & Duree, JVector<JInt32>& vect, JInt32 & diviseur)
{
	m_mValSupports.MoveTo(SupportId);
	if (!m_mValSupports.IsValid())
		return false;

	m_mValSupports.GetItem()->GetRepartionStruct(Duree, vect, diviseur);
	return true;
}

