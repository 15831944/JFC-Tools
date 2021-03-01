#include "stdafx.h"
#include ".\segments.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CSegments::CSegments(void)
{
}

CSegments::~CSegments(void)
{
}


JBool CSegments::Load(JStringEx &Name)
{

	m_sName = Name;
	m_sName.Append (".0A");

	try
	{
		JArchive *pFile = JArchive::Create(m_sName.AsJCharPtr());

		LoadFileHeader		(pFile);
		LoadHeader			(pFile, m_FileHeader.GetVersion ());
		LoadSegments 		(pFile, m_FileHeader.GetVersion ());

		delete pFile;

	}
	catch(JException *pE)
	{
		pE->GetMessage();
		return false;
	}
	return true;
}

JVoid CSegments::LoadFileHeader(JArchive *pFile)
{
	const JInt32 CURRENT_VERSION = 1;

	m_FileHeader.Recv(*pFile);
	
	// Check compagnie name
	if(m_FileHeader.GetCompagnie () != 0x0043464A)
		throw JInternalError::GetInstance(); //("File Header, compagnie field error");

	// Verify the magic number
	if(m_FileHeader.GetMagicNumber () != 0x6FD3590A)
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

JVoid CSegments::LoadHeader(JArchive *pFile, JInt32 Version)
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
			TRACE("CSegments   Invalid Version");
			throw JInternalError::GetInstance(); //("File Version error");
		break;
	}
}

JVoid CSegments::LoadSegments(JArchive *pFile, JInt32 Version)
{
	switch(Version)
	{
		case 1:
		{
			// Get number of individuals
			pFile->Recv(m_lNbIndividus);
			// Get number of Segments
			JInt32 Count;
			pFile->Recv(Count);

			m_vSegments.SetCount(Count);

			for( m_vSegments.MoveFirst(); m_vSegments.IsValid(); m_vSegments.MoveNext())
			{
				PTR_SEGMENT pItem(new CSegment);

				pItem->Recv(*pFile, Version, m_lNbIndividus);

				m_vSegments.GetItem() = pItem;
			}
		}
		break;

		default:
			TRACE("CSegments   Invalid Version");
			throw JInternalError::GetInstance(); //("File Version error");
		break;
	}
}

JBool CSegments::GetSegments(JVector<CSegment *> & Segments)
{
	Segments.SetCount(m_vSegments.GetCount());
	for(Segments.MoveFirst(),m_vSegments.MoveFirst(); m_vSegments.IsValid(); Segments.MoveNext(), m_vSegments.MoveNext())
	{
		Segments.GetItem() = m_vSegments.GetItem().get();
	}
	return true;
}

// Gets the default population segment and contingent mask
JInt32 CSegments::GetDefaultPopSegment(CBitArray & PopSegment, JUnt32 & ContingentMask, bool AllSegment)
{
	if (!AllSegment)
	{
		m_vSegments.MoveFirst();
		if(m_vSegments.IsValid())
		{
			CSegment * pSegment = m_vSegments.GetItem().get();
			PopSegment = pSegment->GetPopSegment();
			ContingentMask = pSegment->GetContingentMask() ; 
			return pSegment->GetID() ;
		}

		PopSegment.SetAll(true);
		ContingentMask = 0xFFFF;
		return -1;
	}
	else
	{
		JUnt32 IdSeg = -1;

		// Cas spécifique multi-segments
		m_vSegments.MoveFirst();
		if(m_vSegments.IsValid())
		{
			// Comme avant
			CSegment * pSegment = m_vSegments.GetItem().get();
			PopSegment = pSegment->GetPopSegment();
			ContingentMask = pSegment->GetContingentMask(); 
			IdSeg = pSegment->GetID();

			if (AllSegment)
			{
				for (m_vSegments.MoveNext(); m_vSegments.IsValid(); m_vSegments.MoveNext())
				{
					CSegment * pSegment = m_vSegments.GetItem().get();
					PopSegment |= pSegment->GetPopSegment();
				}
			}
			return IdSeg;
		}

		PopSegment.SetAll(true);
		ContingentMask = 0xFFFF;
		return -1;
	}		
}

