#include "stdafx.h"
#include ".\SecuritySupports.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CSecuritySupports::CSecuritySupports(void)
{
}

CSecuritySupports::~CSecuritySupports(void)
{
}

JBool CSecuritySupports::Load(JStringEx &Name)
{
	m_sName = Name;
	JStringEx sName = Name;
	sName.Append (".ZZ");

	m_lSourceID = m_lTerrainID = m_NbSupportForbiden = 0;

	try
	{
		if (JPathManager::IsFileExists(sName.AsJCharPtr()))
		{
			JArchive *pFile = JArchive::Create(sName.AsJCharPtr());

			LoadFileHeader		(pFile);
			LoadHeader			(pFile, m_FileHeader.GetVersion());
			LoadData			(pFile, m_FileHeader.GetVersion());

			delete pFile;
		}
		else
			m_IdSupportsForbiden.Reset();
	}
	catch(JException *pE)
	{
		return false;
	}
	return true;
}

JVoid CSecuritySupports::LoadFileHeader(JArchive *pFile)
{
	const JInt32 CURRENT_VERSION = 1;

	m_FileHeader.Recv(*pFile);
	
	// Check compagnie name
	if(m_FileHeader.GetCompagnie () != 0x0043464A)
		throw JInternalError::GetInstance(); //("File Header, compagnie field error");

	// Verify the magic number
	if(m_FileHeader.GetMagicNumber () != 0x6FD359FF)
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

JVoid CSecuritySupports::LoadHeader(JArchive *pFile, JInt32 Version)
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
			TRACE("CSecuritySupports Version error");
			throw JInternalError::GetInstance(); //("File Version error");
		break;
	}
}

JVoid CSecuritySupports::LoadData(JArchive *pFile, JInt32 Version)
{
	switch(Version)
	{
		case 1:
		{
			m_IdSupportsForbiden.Reset();
			// Get m_NbSupportForbiden
			pFile->Recv(m_NbSupportForbiden);

			JUnt32 id;
			for(int i=0; i< m_NbSupportForbiden; i++)
			{
				pFile->Recv(id);
				m_IdSupportsForbiden.Add(id) = id;
			}
		}
		break;

		default:
			TRACE("CSecuritySupports Version error");
			throw JInternalError::GetInstance(); //("File Version error");
		break;
	}
}

bool CSecuritySupports::IsSupportValid(JInt32 idSupport) const
{
	m_IdSupportsForbiden.MoveTo(idSupport);
	return !m_IdSupportsForbiden.IsValid();
}

