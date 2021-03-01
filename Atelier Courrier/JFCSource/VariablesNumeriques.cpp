// VariablesNumeriques.cpp: implementation of the CVariablesNumeriques class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "VariablesNumeriques.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CVariablesNumeriques::CVariablesNumeriques()
{
	// File Name
	m_sName = "";
	// Source ID
	m_lSourceID = 0;
	// Terrain ID
	m_lTerrainID = 0;
}

CVariablesNumeriques::~CVariablesNumeriques()
{
}

JBool CVariablesNumeriques::Load(JStringEx &Name)
{

	m_sName = Name;
	m_sName.Append (".07");

	try
	{
		JArchive *pFile = JArchive::Create(m_sName.AsJCharPtr());

		LoadFileHeader		(pFile);
		LoadHeader			(pFile, m_FileHeader.GetVersion ());
		LoadVariables		(pFile, m_FileHeader.GetVersion ());

		delete pFile;

	}
	catch(JException *pE)
	{
		return false;
	}
	return true;
}

JVoid CVariablesNumeriques::LoadFileHeader(JArchive *pFile)
{
	const JInt32 CURRENT_VERSION = 1;

	m_FileHeader.Recv(*pFile);
	
	// Check compagnie name
	if(m_FileHeader.GetCompagnie () != 0x0043464A)
		throw JInternalError::GetInstance(); //("File Header, compagnie field error");

	// Verify the magic number
	if(m_FileHeader.GetMagicNumber () != 0x6FD35907)
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

JVoid CVariablesNumeriques::LoadHeader(JArchive *pFile, JInt32 Version)
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
			TRACE("CVariablesNumeriques   Invalid Version");
			throw JInternalError::GetInstance(); //("File Version error");
		break;
	}
}

JVoid CVariablesNumeriques::LoadVariables(JArchive *pFile, JInt32 Version)
{
	switch(Version)
	{
		case 1:
		{
			// Get number of individuals
			pFile->Recv(m_lNbIndividus);
			// Get number of variables
			JInt32 Count;
			pFile->Recv(Count);

			m_vVariables.SetCount(Count);

			for( m_vVariables.MoveFirst(); m_vVariables.IsValid(); m_vVariables.MoveNext())
			{
				PTR_VARIABLENUM pItem(new CVariableNum);

				pItem->Recv(*pFile, Version, m_lNbIndividus);

				m_vVariables.GetItem() = pItem;
			}
		}
		break;

		default:
			TRACE("CVariablesNumeriques   Invalid Version");
			throw JInternalError::GetInstance(); //("File Version error");
		break;
	}
}

const JFLT32VECTOR *	CVariablesNumeriques::GetNumValues(JInt32 Index)
{
	if(Index >= m_vVariables.GetCount())
		return 0;

	m_vVariables.MoveTo(Index);

	if(m_vVariables.IsValid())
		return m_vVariables.GetItem()->GetNumValues();

	return 0;
}

JInt32	CVariablesNumeriques::GetDivider(JInt32 Index)
{
	if(Index >= m_vVariables.GetCount())
		return 0;

	m_vVariables.MoveTo(Index);

	if(m_vVariables.IsValid())
		return m_vVariables.GetItem()->GetDivider();

	return 0;
}
