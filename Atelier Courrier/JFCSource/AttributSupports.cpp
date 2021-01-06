#include "stdafx.h"
#include ".\AttributSupports.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CAttributSupports::CAttributSupports(void)
{
	m_sName.Reset();
}

CAttributSupports::~CAttributSupports(void)
{
}

JBool CAttributSupports::Load(JStringEx &Name)
{

	m_sName = Name;
	m_sName.Append (".0B");

	try
	{
		JArchive *pFile = JArchive::Create(m_sName.AsJCharPtr());

		LoadFileHeader		(pFile);
		LoadHeader			(pFile, m_FileHeader.GetVersion ());
		LoadAttributSupports 		(pFile, m_FileHeader.GetVersion ());

		delete pFile;

	}
	catch(JException *pE)
	{
		return false;
	}
	return true;
}

JVoid CAttributSupports::LoadFileHeader(JArchive *pFile)
{
	const JInt32 CURRENT_VERSION = 1;

	m_FileHeader.Recv(*pFile);
	
	// Check compagnie name
	if(m_FileHeader.GetCompagnie () != 0x0043464A)
		throw JInternalError::GetInstance(); //("File Header, compagnie field error");

	// Verify the magic number
	if(m_FileHeader.GetMagicNumber () != 0x6FD3590B)
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

JVoid CAttributSupports::LoadHeader(JArchive *pFile, JInt32 Version)
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
			TRACE("CAttributSupports   Invalid Version");
			throw JInternalError::GetInstance(); //("File Version error");
		break;
	}
}

JVoid CAttributSupports::LoadAttributSupports(JArchive *pFile, JInt32 Version)
{
	switch(Version)
	{
		case 1:
		{

			// Nombre de dimensions de la configuration
			pFile->Recv(m_lNbDimensions);
			// Load the dimensions
			for(JInt32 x=0; x < m_lNbDimensions; x++)
			{
				JInt32 Id;
				pFile->Recv(Id);
				m_mDimensionID.Add(Id) = x;
			}

			// Get number of AttributSupports
			JInt32 Count;
			pFile->Recv(Count);

			for(JInt32 x=0; x < Count; x++)
			{
				// TRACE("Configuration %d\n",x);
				PTR_ATTRIBUTSUPPORT  pItem(new CAttributSupport);
				pItem->Recv(*pFile, Version, m_lNbDimensions);
				// Add support to map
				CDimSupportKey Key = pItem->GetIDs(); 
				m_mAttribSupport.Add(Key) = pItem.get();
				m_lAttribSupport.AddTail() = pItem; 
			}
		}
		break;

		default:
			TRACE("CAttributSupports   Invalid Version");
			throw JInternalError::GetInstance(); //("File Version error");
		break;
	}
}

const CAttributSupport * CAttributSupports::GetAttributSupportByIndex(JInt32 Index) const
{
	// Find the support
	m_lAttribSupport.MoveTo(Index);

	if(m_lAttribSupport.IsValid())
		return m_lAttribSupport.GetItem().get();

	return 0;
}

const CAttributSupport * CAttributSupports::GetAttributSupportByIds(const JINT32VECTOR & DimensionIds, const JINT32VECTOR & SupportIds) const
{
	if(DimensionIds.GetCount() != m_lNbDimensions || SupportIds.GetCount() != m_lNbDimensions)
		return 0;

	JVector<JInt32> Ids;
	Ids.SetCount(m_lNbDimensions);

	for(DimensionIds.MoveFirst(); DimensionIds.IsValid(); DimensionIds.MoveLast())
	{
		m_mDimensionID.MoveTo(DimensionIds.GetItem());
		if(m_mDimensionID.IsValid())
		{
			Ids.MoveTo(m_mDimensionID.GetItem());
			SupportIds.MoveTo(DimensionIds.GetIndex());
			Ids.GetItem() = SupportIds.GetItem();
		}
		else
			return 0;
	}

	CDimSupportKey Key = Ids;
	// Find the support
	m_mAttribSupport.MoveTo(Key);

	if(m_mAttribSupport.IsValid())
		return m_mAttribSupport.GetItem();

	return 0;
}
