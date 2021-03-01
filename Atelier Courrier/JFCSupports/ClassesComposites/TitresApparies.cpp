#include "stdafx.h"
#include ".\TitresApparies.h"

CTitresApparies::CTitresApparies(void)
{
}

CTitresApparies::~CTitresApparies(void)
{
	for(MoveFirst(); IsValid(); MoveNext())
		delete GetItem();
}

JBool CTitresApparies::Load(JStringEx &Name)
{

	Reset();
	m_sName = Name;
	m_sName.Append (".1C");

	SetCurrentDirectory(AfxGetApp()->GetProfileString("Data", "UserPath", 0));

	try
	{
		JArchive *pFile = JArchive::Create(m_sName.AsJCharPtr());

		LoadFileHeader		(pFile);
		LoadHeader			(pFile, m_FileHeader.GetVersion ());
		LoadTitresApparies 		(pFile, m_FileHeader.GetVersion ());

		delete pFile;

	}
	catch(JException *pE)
	{
		pE->GetMessage();
		SetCurrentDirectory(AfxGetApp()->GetProfileString("Data", "SourcesPath", 0));
		return false;
	}
	SetCurrentDirectory(AfxGetApp()->GetProfileString("Data", "SourcesPath", 0));
	return true;
}

JVoid CTitresApparies::LoadFileHeader(JArchive *pFile)
{
	const JInt32 CURRENT_VERSION = 1;

	m_FileHeader.Recv(*pFile);
	
	// Check compagnie name
	if(m_FileHeader.GetCompagnie () != 0x0043464A)
		throw JInternalError::GetInstance(); //("File Header, compagnie field error");

	// Verify the magic number
	if(m_FileHeader.GetMagicNumber () != 0x6FD3591C)
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

JVoid CTitresApparies::LoadHeader(JArchive *pFile, JInt32 Version)
{
	JInt32 sourceID, terrainID;
	switch(Version)
	{
		case 1:
		{
			// Load Source and Terrain ID's
			pFile->Recv(sourceID);
			if (sourceID != m_lSourceID)
				throw JInternalError::GetInstance(); //("Header, Source Error");
			pFile->Recv(terrainID);
			if (terrainID != m_lTerrainID)
				throw JInternalError::GetInstance(); //("Header, Terrain Error");
		}
		break;

		default:
			TRACE("CTitresApparies   Invalid Version");
			throw JInternalError::GetInstance(); //("File Version error");
		break;
	}
}

JVoid CTitresApparies::LoadTitresApparies(JArchive *pFile, JInt32 Version)
{
	switch(Version)
	{
		case 1:
		{
			// Get number of TitresApparies
			JInt32 nCount;
			pFile->Recv(nCount);

			for( JInt32 i = 0; i < nCount; i++ )
			{
				CTitreApparie* pItem = new CTitreApparie();
				pItem->Recv(*pFile, Version);
	
				Add(pItem->GetID(), pItem, true);
			}
		}
		break;

		default:
			TRACE("CTitresApparies   Invalid Version");
			throw JInternalError::GetInstance(); //("File Version error");
		break;
	}
}

JBool CTitresApparies::Save()
{
	
	SetCurrentDirectory(AfxGetApp()->GetProfileString("Data", "UserPath", 0));

	try
	{
		JArchive *pFile = JArchive::Create(m_sName.AsJCharPtr(), true);

		SaveFileHeader		(pFile);
		SaveHeader			(pFile, m_FileHeader.GetVersion ());
		SaveTitresApparies 	(pFile, m_FileHeader.GetVersion ());

		pFile->Flush();
		delete pFile;
		JPathManager::UpdateFile( m_sName.AsJCharPtr() );

	}
	catch(JException *pE)
	{
		pE->GetMessage();
		SetCurrentDirectory(AfxGetApp()->GetProfileString("Data", "SourcesPath", 0));
		return false;
	}

	SetCurrentDirectory(AfxGetApp()->GetProfileString("Data", "SourcesPath", 0));
	return true;
}

JVoid CTitresApparies::SaveFileHeader(JArchive *pFile)
{
	const JInt32 CURRENT_VERSION = 1;

	// Check compagnie name
	m_FileHeader.SetCompagnie (0x0043464A);
	// Verify the magic number
	m_FileHeader.SetMagicNumber (0x6FD3591C);
	// No compression
	m_FileHeader.SetCompression (0);
	// No cryptage
	m_FileHeader.SetCryptage(0);
	// Must not be greater than my current version
	m_FileHeader.SetVersion(CURRENT_VERSION);

	m_FileHeader.Send(*pFile);

}

JVoid CTitresApparies::SaveHeader(JArchive *pFile, JInt32 Version)
{
	switch(Version)
	{
		case 1:
		{
			// Load Source and Terrain ID's
			pFile->Send(m_lSourceID);
			pFile->Send(m_lTerrainID);
		}
		break;

		default:
			TRACE("CTitresApparies   Invalid Version");
			throw JInternalError::GetInstance(); //("File Version error");
		break;
	}
}

JVoid CTitresApparies::SaveTitresApparies(JArchive *pFile, JInt32 Version)
{
	switch(Version)
	{
		case 1:
		{
			// Get number of TitresApparies
			pFile->Send( GetCount() );

			for( MoveFirst(); IsValid(); MoveNext())
				GetItem()->Send(*pFile, Version);
		}
		break;

		default:
			TRACE("CTitresApparies   Invalid Version");
			throw JInternalError::GetInstance(); //("File Version error");
		break;
	}
}

JVoid CTitresApparies::Add(JInt32 id, CTitreApparie* titreapp, JBool bMove)
{
	titreapp->SetSourceID(m_lSourceID);
	titreapp->SetTerrainID(m_lTerrainID);
	JMap<JInt32, CTitreApparie*>::Add(id, bMove) = titreapp;
}