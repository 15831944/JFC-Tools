#include "stdafx.h"
#include ".\security.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CSecurity::CSecurity(void)
{
}

CSecurity::~CSecurity(void)
{
}

JBool CSecurity::Load(JStringEx &Name)
{

	m_sName = Name;
	JStringEx sName = Name;
	sName.Append (".0E");

	try
	{
		JArchive *pFile = JArchive::Create(sName.AsJCharPtr());

		LoadFileHeader		(pFile);
		LoadHeader			(pFile, m_FileHeader.GetVersion ());
		LoadData			(pFile, m_FileHeader.GetVersion ());

		delete pFile;

	}
	catch(JException *pE)
	{
		return false;
	}
	return true;
}

JVoid CSecurity::LoadFileHeader(JArchive *pFile)
{
	const JInt32 CURRENT_VERSION = 1;

	m_FileHeader.Recv(*pFile);
	
	// Check compagnie name
	if(m_FileHeader.GetCompagnie () != 0x0043464A)
		throw JInternalError::GetInstance(); //("File Header, compagnie field error");

	// Verify the magic number
	if(m_FileHeader.GetMagicNumber () != 0x6FD3590E)
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

JVoid CSecurity::LoadHeader(JArchive *pFile, JInt32 Version)
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
			TRACE("CSecurity Version error");
			throw JInternalError::GetInstance(); //("File Version error");
		break;
	}
}

JVoid CSecurity::LoadData(JArchive *pFile, JInt32 Version)
{
	switch(Version)
	{
		case 1:
		{
			m_MapValidIN.Reset();
			// Get Items Natif
			pFile->Recv(m_NbItemsNatif);
			// Get Number of segments
			pFile->Recv(m_NbSegments);
			for (JInt32 idxSegment = 0; idxSegment < m_NbSegments; idxSegment += 1)
			{
				JInt32 IdSegment; pFile->Recv(IdSegment);
				CBitArray & ItemNatif = m_MapValidIN.Add(IdSegment);
				ItemNatif.SetSize(m_NbItemsNatif);
				ItemNatif.Recv(*pFile); 
			}
		}
		break;
		default:
			TRACE("CSecurity Version error");
			throw JInternalError::GetInstance(); //("File Version error");
		break;
	}
}

bool CSecurity::IsValidItemNatif(JInt32 ItemNatif) const
{
	// ancien code qui plante
	//for (m_MapValidIN.MoveFirst(); m_MapValidIN.IsValid(); m_MapValidIN.MoveNext())
	//	if (m_MapValidIN.GetItem().GetAt(ItemNatif)) return (true);
	//return (false);

	// nouveau code Aurélien avec test de la taille du CBitArray avant de faire GetAt
	for (m_MapValidIN.MoveFirst(); m_MapValidIN.IsValid(); m_MapValidIN.MoveNext())
	{
		if (ItemNatif < m_MapValidIN.GetItem().GetSize())
			if (m_MapValidIN.GetItem().GetAt(ItemNatif))
				return (true);
	}
	return (false);
}

bool CSecurity::IsValidItemNatif(JInt32 ItemNatif, const JVector<JInt32> & IdSegments) const
{
	if (IdSegments.GetCount() == 0) return (false);
	for (IdSegments.MoveFirst();IdSegments.IsValid();IdSegments.MoveNext())
	{
		m_MapValidIN.MoveTo(IdSegments.GetItem()); if (!m_MapValidIN.IsValid()) return (false);
		if (!m_MapValidIN.GetItem().GetAt(ItemNatif)) return (false);
	}
	return (true);
}