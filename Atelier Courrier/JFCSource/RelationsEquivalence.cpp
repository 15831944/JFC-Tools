// RelationsEquivalence.cpp: implementation of the CRelationsEquivalence class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "RelationsEquivalence.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CRelationsEquivalence::CRelationsEquivalence()
{
	// File Name
	m_sName = "";
	// Source ID
	m_lSourceID = 0;
	// Terrain ID
	m_lTerrainID = 0;
}

CRelationsEquivalence::~CRelationsEquivalence()
{
}

JBool CRelationsEquivalence::Load(JStringEx &Name)
{

	m_sName = Name;
	m_sName.Append (".06");

	try
	{
		JArchive *pFile = JArchive::Create(m_sName.AsJCharPtr());

		LoadFileHeader		(pFile);
		LoadHeader			(pFile, m_FileHeader.GetVersion ());
		LoadRelations		(pFile, m_FileHeader.GetVersion ());

		delete pFile;

	}
	catch(JException *pE)
	{
		return false;
	}
	return true;
}

JVoid CRelationsEquivalence::LoadFileHeader(JArchive *pFile)
{
	const JInt32 CURRENT_VERSION = 1;

	m_FileHeader.Recv(*pFile);
	
	// Check compagnie name
	if(m_FileHeader.GetCompagnie () != 0x0043464A)
		throw JInternalError::GetInstance(); //("File Header, compagnie field error");

	// Verify the magic number
	if(m_FileHeader.GetMagicNumber () != 0x6FD35906)
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

JVoid CRelationsEquivalence::LoadHeader(JArchive *pFile, JInt32 Version)
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
			TRACE("CRelationsEquivalence Invalid Version");
			throw JInternalError::GetInstance(); //("File Version error");
		break;
	}
}

JVoid CRelationsEquivalence::LoadRelations(JArchive *pFile, JInt32 Version)
{
	switch(Version)
	{
		case 1:
		{
			// Get number of Relations
			JInt32 Count;
			pFile->Recv(Count);

			JInt32 Rid;

			for(JInt32 x=0; x < Count; x++)
			{
				pFile->Recv(Rid);
				m_mRelations.Add(Rid).Recv(*pFile);
			}
		}
		break;

		default:
			TRACE("CRelationsEquivalence Invalid Version");
			throw JInternalError::GetInstance(); //("File Version error");
		break;
	}
}

// Returns true if the given repere has a relation equivalence
JBool	CRelationsEquivalence::HasEquivalence(JInt32 Repere) const
{
	m_mRelations.MoveTo(Repere);
	return m_mRelations.IsValid();
}

// Returns the relation equivalence for the given repere
const JChar *	CRelationsEquivalence::GetEquivalence(JInt32 Repere) const
{
	m_mRelations.MoveTo(Repere);
	if(m_mRelations.IsValid())
		return m_mRelations.GetItem().AsJCharPtr();

	return 0;
}