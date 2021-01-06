// VersionsTarifsAutos.cpp: implementation of the CVersionsTarifsAutos class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include ".\VersionsTarifsAutos.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CVersionsTarifsAutos::CVersionsTarifsAutos(void)
{
}

CVersionsTarifsAutos::~CVersionsTarifsAutos(void)
{
}

JBool CVersionsTarifsAutos::FindVersion( JUnt32 idSupport, JDate dateIn, JUnt32& version ) const
{
	JBool bret = false;
	CKeySupportDate key;
	key.m_IdSupport = idSupport;
	key.m_Date = dateIn;
	MoveTo(key, -1);
	if (IsValid())
	{
		version = GetItem();
		bret = true;
	}
	return bret;

}

JBool CVersionsTarifsAutos::Load(JStringEx &Name)
{

	m_sName = Name;
	m_sName.Append (".17");

	try
	{
		JArchive *pFile = JArchive::Create(m_sName.AsJCharPtr());

		LoadFileHeader		(pFile);
		LoadVersionsTarifsAutos	(pFile, m_FileHeader.GetVersion ());

		delete pFile;

	}
	catch(JException *pE)
	{
		pE->GetMessage();
		return false;
	}
	return true;
}

JVoid CVersionsTarifsAutos::LoadFileHeader(JArchive *pFile)
{
	const JInt32 CURRENT_VERSION = 1;

	m_FileHeader.Recv(*pFile);
	
	// Check compagnie name
	if(m_FileHeader.GetCompagnie () != 0x0043464A)
		throw JInternalError::GetInstance(); //("File Header, compagnie field error");

	// Verify the magic number
	if(m_FileHeader.GetMagicNumber () != 0x6FD34817)
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

JVoid CVersionsTarifsAutos::LoadVersionsTarifsAutos(JArchive *pFile, JInt32 Version)
{
	switch(Version)
	{
		case 1:
		{
			JInt32 nCount;
			//numero de synchronisation
			pFile->Recv(nCount);
			// Get number of Versions
			pFile->Recv(nCount);
			CKeySupportDate key;
			JUnt32 version;
			for( JInt32 i = 0; i < nCount; i++ )
			{
				key.Recv(*pFile, Version);
				pFile->Recv(version);
				MoveTo(key);
				if (IsValid())
					continue;//incohérence dans la base on jette l'entrée
				Add(key) = version;
			}
		}
		break;

		default:
			TRACE("CVersionsTarifsAutos   Invalid Version");
			throw JInternalError::GetInstance(); //("File Version error");
		break;
	}
}
