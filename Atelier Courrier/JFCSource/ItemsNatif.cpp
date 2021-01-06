// ItemsNatif.cpp: implementation of the CItemsNatif class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ItemsNatif.h"
#include <stdlib.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CItemsNatif::CItemsNatif()
{
	// File Name
	m_sName = "";
	// Source ID
	m_lSourceID = 0;
	// Terrain ID
	m_lTerrainID = 0;
}

CItemsNatif::~CItemsNatif()
{
}

JBool CItemsNatif::Load(JStringEx &Name, JInt32 NbIndividus, JInt32 Bloc)
{

	m_sName = Name;
	m_sName.Append("_");

	JChar t[10];
	_itoa_s(Bloc,t,10,10);
	m_sName.Append (t);
	m_sName.Append (".03");

	try
	{
		JArchive *pFile = JArchive::Create(m_sName.AsJCharPtr());

		LoadFileHeader		(pFile);
		LoadHeader			(pFile, m_FileHeader.GetVersion ());
		LoadItems			(pFile, m_FileHeader.GetVersion (), NbIndividus);

		delete pFile;

	}
	catch(JException *pE)
	{
		return false;
	}
	return true;
}

JVoid CItemsNatif::LoadFileHeader(JArchive *pFile)
{
	const JInt32 CURRENT_VERSION = 1;

	m_FileHeader.Recv(*pFile);
	
	// Check compagnie name
	if(m_FileHeader.GetCompagnie () != 0x0043464A)
		throw JInternalError::GetInstance(); //("File Header, compagnie field error");

	// Verify the magic number
	if(m_FileHeader.GetMagicNumber () != 0x6FD35903)
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

JVoid CItemsNatif::LoadHeader(JArchive *pFile, JInt32 Version)
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
			TRACE("CItemsNatif Version error");
			throw JInternalError::GetInstance(); //("File Version error");
		break;
	}
}

JVoid CItemsNatif::LoadItems(JArchive *pFile, JInt32 Version, JInt32 NbIndividus)
{
	switch(Version)
	{
		case 1:
		{
			// Get number of items natif
			JInt32 Count;

			pFile->Recv(Count);

			m_vItems.SetCount(Count);

			for(m_vItems.MoveFirst(); m_vItems.IsValid(); m_vItems.MoveNext())
			{
				m_vItems.GetItem().SetSize(NbIndividus); 
				m_vItems.GetItem().Recv(*pFile);
			}
		}
		break;

		default:
			TRACE("CItemsNatif Version error");
			throw JInternalError::GetInstance(); //("File Version error");
		break;
	}
}

const CBitArray * CItemsNatif::GetVectorIndividus(JInt32 Index) const
{
	if(Index >= m_vItems.GetCount())
		return 0;
	
	m_vItems.MoveTo(Index);
	return &m_vItems.GetItem();
}
