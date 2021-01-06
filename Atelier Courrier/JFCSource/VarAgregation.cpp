// VarAgregation.cpp: implementation of the CVarAgregation class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "VarAgregation.h"
#include <stdlib.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////


CVarAgregation::CVarAgregation()
{
	// Variable Number
	m_lVarNum = 0;
	// File Name
	m_sName = "";
	// Source ID
	m_lSourceID = 0;
	// Terrain ID
	m_lTerrainID = 0;
	// Number of individuals
	m_lNbIndividus = 0;
	// the divider
	m_lDiviseur = 0;
}

CVarAgregation::~CVarAgregation()
{
}

JBool CVarAgregation::Load(const JStringEx &Name, JInt32 NumVariable)
{
	m_lVarNum	= NumVariable;
	m_sName		= Name;
	m_sName.Append("_");

	JChar t[10];
	_itoa_s(m_lVarNum,t,10,10);
	m_sName.Append (t);
	m_sName.Append(".08");

	try
	{
		JArchive *pFile = JArchive::Create(m_sName.AsJCharPtr());

		LoadFileHeader		(pFile);
		LoadHeader			(pFile, m_FileHeader.GetVersion ());
		LoadVariable		(pFile, m_FileHeader.GetVersion ());

		delete pFile;

	}
	catch(JException *pE)
	{
		return false;
	}
	return true;
}

JVoid CVarAgregation::LoadFileHeader(JArchive *pFile)
{
	const JInt32 CURRENT_VERSION = 1;

	m_FileHeader.Recv(*pFile);
	
	// Check compagnie name
	if(m_FileHeader.GetCompagnie () != 0x0043464A)
		throw JInternalError::GetInstance(); //("File Header, compagnie field error");

	// Verify the magic number
	if(m_FileHeader.GetMagicNumber () != 0x6FD35908)
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

JVoid CVarAgregation::LoadHeader(JArchive *pFile, JInt32 Version)
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
			TRACE("CVarAgregation  Invalid Version");
			throw JInternalError::GetInstance(); //("File Version error");
		break;
	}
}

JVoid CVarAgregation::LoadVariable(JArchive *pFile, JInt32 Version)
{
	switch(Version)
	{
		case 1:
		{
			// Get number of individuals
			pFile->Recv(m_lNbIndividus);
			// Ge the nature of variable 
			pFile->Recv(m_Nature);
			// Nombre de dimensions de la configuration
			pFile->Recv(m_lNbDimensions);
			// Load the dimensions
			for(JInt32 x=0; x < m_lNbDimensions; x++)
			{
				JInt32 Id;
				pFile->Recv(Id);
				m_mDimensionID.Add(Id) = x;
			}

			// Get the divider
			pFile->Recv(m_lDiviseur);
			// Get number of support variables
			JInt32 Count;
			pFile->Recv(Count);

			for(JInt32 x=0; x < Count; x++)
			{
				// TRACE("Configuration %d\n",x);
				PTR_VARAGRSUPPORT pItem(new CVarAgrSupport(m_Nature));
				pItem->Recv(*pFile, Version, m_lNbIndividus, m_lNbDimensions);
				// Add support to map
				CDimSupportKey Key = pItem->GetIDs(); 
				m_mVarSupport.Add(Key) = pItem.get();
				m_lVarSupport.AddTail() = pItem; 
			}
		}
		break;

		default:
			TRACE("CVarAgregation  Invalid Version");
			throw JInternalError::GetInstance(); //("File Version error");
		break;
	}
}

const JFLT32VECTOR * CVarAgregation::GetNumValuesByIdx(JInt32 Index)
{
	// Find the support
	m_lVarSupport.MoveTo(Index);

	if(m_lVarSupport.IsValid())
		return m_lVarSupport.GetItem()->GetNumValues(m_lDiviseur);

	return 0;
}

const CBitArray * CVarAgregation::GetBinValuesByIdx(JInt32 Index)
{
	// Find the support
	m_lVarSupport.MoveTo(Index);

	if(m_lVarSupport.IsValid())
		return m_lVarSupport.GetItem()->GetBinValues();

	return 0;
}

const JFLT32VECTOR * CVarAgregation::GetNumValuesByIds(const JINT32VECTOR & DimensionIds, const JINT32VECTOR & SupportIds)
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
	m_mVarSupport.MoveTo(Key);

	if(m_mVarSupport.IsValid())
		return m_mVarSupport.GetItem()->GetNumValues(m_lDiviseur);

	return 0;
}

const CBitArray * CVarAgregation::GetBinValuesByIds(const JINT32VECTOR & DimensionIds, const JINT32VECTOR & SupportIds)
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
	m_mVarSupport.MoveTo(Key);

	if(m_mVarSupport.IsValid())
		return m_mVarSupport.GetItem()->GetBinValues();

	return 0;
}
