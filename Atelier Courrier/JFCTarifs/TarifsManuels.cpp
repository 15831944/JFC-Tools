// TarifsManuels.cpp: implementation of the CTarifsManuels class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include ".\tarifsmanuels.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CTarifsManuels::CTarifsManuels(void)
{
}

CTarifsManuels::~CTarifsManuels(void)
{
	MoveLast();
	while( IsValid() )
	{
		delete GetItem();
		Remove(-1);
	}
}

JBool CTarifsManuels::GetTarifManuel( JUnt32 idSupport, JUnt32 idFormat, JDate dateIn, JUnt32 & tarif) const
{
	JBool bret = false;
	CTarifManuel * trf;
	JDate dateOut, date;
	for(MoveFirst(); IsValid(); MoveNext())
	{
		trf = GetItem();
		if (trf->GetIDSupport() == idSupport
			&& trf->GetIDFormat() == idFormat)
		{
			date = trf->GetDateC();
			//on prend la date de début la plus proche
			if ( date <= dateIn && dateOut < date )
			{
				//sauf si notre tarif n'est plus valide
				if ( trf->GetDateS().IsValid() && dateIn > trf->GetDateS() )
					continue;
				dateOut = date;
				tarif = trf->GetTarif();
			}
		}
		
	}
	if (dateOut.IsValid())
		bret = true;
	
	return bret;
}

JVoid CTarifsManuels::RemoveTarifManuel(CTarifManuel* tarif)
{
	MoveFirst();
	while ( IsValid() && GetItem() != tarif )
		MoveNext();

	if ( IsValid() )
	{
		 Remove(-1);
		 delete tarif;
	}
}

JBool CTarifsManuels::Load()
{
	SetCurrentDirectory(AfxGetApp()->GetProfileString("Data", "UserPath", 0));

	JStringEx sName = "/TarifManuel.1B";

	try
	{
		JArchive *pFile = JArchive::Create(sName.AsJCharPtr());

		LoadFileHeader		(pFile);
		LoadTarifsManuels	(pFile, m_FileHeader.GetVersion ());

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

JVoid CTarifsManuels::LoadFileHeader(JArchive *pFile)
{
	const JInt32 CURRENT_VERSION = 1;

	m_FileHeader.Recv(*pFile);
	
	// Check compagnie name
	if(m_FileHeader.GetCompagnie () != 0x0043464A)
		throw JInternalError::GetInstance(); //("File Header, compagnie field error");

	// Verify the magic number
	if(m_FileHeader.GetMagicNumber () != 0x6FD3481B)
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

JVoid CTarifsManuels::LoadTarifsManuels(JArchive *pFile, JInt32 Version)
{
	switch(Version)
	{
		case 1:
		{
			// Get number of TarifManuel
			JInt32 nCount;
			pFile->Recv(nCount);
			CTarifManuel* trfman;
			for( JInt32 i = 0; i < nCount; i++ )
			{
				trfman = new CTarifManuel();
				trfman->Recv(*pFile, Version);
				AddTail() = trfman;
			}
		}
		break;

		default:
			TRACE("CTarifsManuels   Invalid Version");
			throw JInternalError::GetInstance(); //("File Version error");
		break;
	}
}

JBool CTarifsManuels::Save()
{
	SetCurrentDirectory(AfxGetApp()->GetProfileString("Data", "UserPath", 0));

	JStringEx sName = "/TarifManuel.1B";

	try
	{
		JArchive *pFile = JArchive::Create(sName.AsJCharPtr(), true);

		SaveFileHeader		(pFile);
		SaveTarifsManuels 	(pFile, m_FileHeader.GetVersion ());

		pFile->Flush();
		delete pFile;
		JPathManager::UpdateFile( sName.AsJCharPtr() );


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

JVoid CTarifsManuels::SaveFileHeader(JArchive *pFile)
{
	const JInt32 CURRENT_VERSION = 1;

	// Check compagnie name
	m_FileHeader.SetCompagnie (0x0043464A);
	// Verify the magic number
	m_FileHeader.SetMagicNumber (0x6FD3481B);
	// No compression
	m_FileHeader.SetCompression (0);
	// No cryptage
	m_FileHeader.SetCryptage(0);
	// Must not be greater than my current version
	m_FileHeader.SetVersion(CURRENT_VERSION);

	m_FileHeader.Send(*pFile);

}

JVoid CTarifsManuels::SaveTarifsManuels(JArchive *pFile, JInt32 Version)
{
	switch(Version)
	{
		case 1:
		{
			// Set number of TarifsManuels
			pFile->Send( GetCount() );

			for( MoveFirst(); IsValid(); MoveNext())
				GetItem()->Send(*pFile, Version);
		}
		break;

		default:
			TRACE("CTarifsManuels   Invalid Version");
			throw JInternalError::GetInstance(); //("File Version error");
		break;
	}
}



