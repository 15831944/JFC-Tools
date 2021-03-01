// ReperesAbsolus.cpp: implementation of the CReperesAbsolus class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ReperesAbsolus.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CReperesAbsolus::CReperesAbsolus()
{
	// File Name
	m_sName = "";
	// Source ID
	m_lSourceID = 0;
	// Terrain ID
	m_lTerrainID = 0;

	m_pSecurity = 0;
}

CReperesAbsolus::~CReperesAbsolus()
{
}


JBool CReperesAbsolus::Load(JStringEx &Name, CSecurity *pSecurity)
{

	m_pSecurity = pSecurity; 
	m_sName = Name;
	m_sName.Append (".05");

	try
	{
		JArchive *pFile = JArchive::Create(m_sName.AsJCharPtr());

		LoadFileHeader		(pFile);
		LoadHeader			(pFile, m_FileHeader.GetVersion ());
		LoadReperes			(pFile, m_FileHeader.GetVersion ());

		delete pFile;

	}
	catch(JException *pE)
	{
		return false;
	}
	return true;
}

JVoid CReperesAbsolus::LoadFileHeader(JArchive *pFile)
{
	const JInt32 CURRENT_VERSION = 1;

	m_FileHeader.Recv(*pFile);
	
	// Check compagnie name
	if(m_FileHeader.GetCompagnie () != 0x0043464A)
		throw JInternalError::GetInstance(); //("File Header, compagnie field error");

	// Verify the magic number
	if(m_FileHeader.GetMagicNumber () != 0x6FD35905)
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

JVoid CReperesAbsolus::LoadHeader(JArchive *pFile, JInt32 Version)
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
			TRACE("CReperesAbsolus  Invalid Version");
			throw JInternalError::GetInstance(); //("File Version error");
		break;
	}
}

JVoid CReperesAbsolus::LoadReperes(JArchive *pFile, JInt32 Version)
{
	switch(Version)
	{
		case 1:
		{
			// Get number of items 
			JInt32 Count;
			pFile->Recv(Count);
			m_mItems.Reset ();
			m_mReperes.Reset();

			for(JInt32 x=0; x < Count; x++)
			{
				JInt32 Repere;
				pFile->Recv(Repere);

				if(m_pSecurity->IsValidItemNatif(x))
				{
					m_mItems.Add(x) = Repere;
					m_mReperes.MoveTo(Repere);
					if(!m_mReperes.IsValid())
						m_mReperes.Add(Repere) = x;
				}
			}
		}
		break;

		default:
			TRACE("CReperesAbsolus  Invalid Version");
			throw JInternalError::GetInstance(); //("File Version error");
		break;
	}
}

// Gets a repere absolut from a given Item Natif
// return 0 if not found
JInt32	CReperesAbsolus::GetRepere(JInt32 ItemNatif)
{
	m_mItems.MoveTo(ItemNatif);
	if(m_mItems.IsValid())
		return m_mItems.GetItem();
	else
		return 0;
}

// Gets an Item Natif from a given Repere Absolut
// return -1 if not found
JInt32	CReperesAbsolus::GetItemNatif(JInt32 Repere)
{
	if(Repere)
	{
		m_mReperes.MoveTo(Repere);
		if(m_mReperes.IsValid())
			return m_mReperes.GetItem();
	}
	return -1;
}