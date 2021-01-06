// Parutions.cpp: implementation of the CParutions class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include ".\Parutions.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CParutions::CParutions(void)
{
}

CParutions::~CParutions(void)
{
}

JBool CParutions::Load(JStringEx &Name)
{

	m_sName = Name;
	m_sName.Append (".18");

	try
	{
		JArchive *pFile = JArchive::Create(m_sName.AsJCharPtr());

		LoadFileHeader		(pFile);
		LoadParutions	(pFile, m_FileHeader.GetVersion ());

		delete pFile;

	}
	catch(JException *pE)
	{
		pE->GetMessage();
		return false;
	}
	return true;
}

JVoid CParutions::LoadFileHeader(JArchive *pFile)
{
	const JInt32 CURRENT_VERSION = 1;

	m_FileHeader.Recv(*pFile);
	
	// Check compagnie name
	if(m_FileHeader.GetCompagnie () != 0x0043464A)
		throw JInternalError::GetInstance(); //("File Header, compagnie field error");

	// Verify the magic number
	if(m_FileHeader.GetMagicNumber () != 0x6FD34818)
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

JVoid CParutions::LoadParutions(JArchive *pFile, JInt32 Version)
{
	switch(Version)
	{
		case 1:
		{
			JInt32 nCount;
			//numero de synchronisation
			pFile->Recv(nCount);
			// Get number of Parution
			pFile->Recv(nCount);
			CKeySupportDate key;
			CParution par;
			for( JInt32 i = 0; i < nCount; i++ )
			{
				key.Recv(*pFile, Version);
				par.Recv(*pFile, Version);
				MoveTo(key);
				if (IsValid())//il y a un pb ds la base
					continue;//incohérence dans la base on jette l'entrée
				Add(key) = par;
			}
		}
		break;

		default:
			TRACE("CParutions   Invalid Version");
			throw JInternalError::GetInstance(); //("File Version error");
		break;
	}
}

JBool CParutions::FindDateExact(JUnt32 idSupport, JDate dateIn, JDate &dateOut, JUnt32 &duree, JUnt32 &version, JInt32 sens) const
{
	JBool bret = false;
	dateOut.Reset();
	CKeySupportDate key;
	key.m_IdSupport = idSupport;
	key.m_Date = dateIn;
	MoveTo(key, sens);
	if (IsValid() )
	{
		key = GetKey();
		if( key.m_IdSupport == idSupport)
		{
			dateOut = key.m_Date;
			duree = GetItem().GetDuree();
			version = GetItem().GetVersion();
			bret = true;
		}
	}
	return bret;
}

JBool CParutions::FindNumero(JUnt32 idSupport, JDate dateIn, JUnt32& numero) const
{
	JBool bret = false;
	CKeySupportDate key;
	key.m_IdSupport = idSupport;
	key.m_Date = dateIn;
	MoveTo(key);
	if (IsValid() )
	{
		key = GetKey();
		if( key.m_IdSupport == idSupport)
		{
			numero = GetItem().GetNumPar();
			bret = true;
		}
	}
	return bret;
}