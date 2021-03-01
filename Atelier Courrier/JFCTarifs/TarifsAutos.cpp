// TarifsAutos.cpp: implementation of the CTarifsAutos class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include ".\TarifsAutos.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CTarifsAutos::CTarifsAutos(void)
{
}

CTarifsAutos::~CTarifsAutos(void)
{
}

JBool CTarifsAutos::Load(JStringEx &Name)
{

	m_sName = Name;
	m_sName.Append (".19");

	try
	{
		JArchive *pFile = JArchive::Create(m_sName.AsJCharPtr());

		LoadFileHeader		(pFile);
		LoadTarifsAutos	(pFile, m_FileHeader.GetVersion ());

		delete pFile;

	}
	catch(JException *pE)
	{
		pE->GetMessage();
		return false;
	}
	return true;
}

JVoid CTarifsAutos::LoadFileHeader(JArchive *pFile)
{
	const JInt32 CURRENT_VERSION = 1;

	m_FileHeader.Recv(*pFile);
	
	// Check compagnie name
	if(m_FileHeader.GetCompagnie () != 0x0043464A)
		throw JInternalError::GetInstance(); //("File Header, compagnie field error");

	// Verify the magic number
	if(m_FileHeader.GetMagicNumber () != 0x6FD34819)
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

JVoid CTarifsAutos::LoadTarifsAutos(JArchive *pFile, JInt32 Version)
{
	switch(Version)
	{
		case 1:
		{
			JInt32 nCount;
			//numero de synchronisation
			pFile->Recv(nCount);
			// Get number of TarifsAutos
			pFile->Recv(nCount);
			CKeyTarifAuto key;
			JUnt32 trf;
			for( JInt32 i = 0; i < nCount; i++ )
			{
				key.Recv(*pFile, Version);
				pFile->Recv(trf);
				MoveTo(key);
				if (IsValid())
					continue;//incohérence dans la base on jette l'entrée
				Add(key) = trf;
			}
		}
		break;

		default:
			TRACE("CTarifsAutos   Invalid Version");
			throw JInternalError::GetInstance(); //("File Version error");
		break;
	}
}

JBool CTarifsAutos::GetTarif( JUnt32 idSupport, JUnt32 idFormat, JUnt32 version, JUnt32 & tarif) const
{
	JBool bret = false;
	CKeyTarifAuto key;
	key.m_IdSupport = idSupport;
	key.m_Version = version;
	key.m_IdFormat = idFormat;

	MoveTo(key);
	if (IsValid())
	{
		tarif = GetItem();
		bret = true;
	}
	return bret;
}
