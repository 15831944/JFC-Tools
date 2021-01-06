// Poids.cpp: implementation of the CPoids class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Poids.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CPoids::CPoids()
{
	// File Name
	m_sName = "";
	// Source ID
	m_lSourceID = 0;
	// Terrain ID
	m_lTerrainID = 0;
	// Number of individuals
	m_lNbIndividus = 0;
}

CPoids::~CPoids()
{
}


JBool CPoids::Load(JStringEx &Name)
{

	m_sName = Name;
	m_sName.Append (".09");

	try
	{
		JArchive *pFile = JArchive::Create(m_sName.AsJCharPtr());

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

JVoid CPoids::LoadFileHeader(JArchive *pFile)
{
	const JInt32 CURRENT_VERSION = 1;

	m_FileHeader.Recv(*pFile);
	
	// Check compagnie name
	if(m_FileHeader.GetCompagnie () != 0x0043464A)
		throw JInternalError::GetInstance(); //("File Header, compagnie field error");

	// Verify the magic number
	if(m_FileHeader.GetMagicNumber () != 0x6FD35909)
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

JVoid CPoids::LoadHeader(JArchive *pFile, JInt32 Version)
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
			TRACE("CPoids Version error");
			throw JInternalError::GetInstance(); //("File Version error");
		break;
	}
}

JVoid CPoids::LoadData(JArchive *pFile, JInt32 Version)
{
	switch(Version)
	{
		case 1:
		{
			// Get number of items 
			JInt32 Count;

			// Get the number of individus
			pFile->Recv(m_lNbIndividus);
			// Get the number of univers and load them
			pFile->Recv(Count);
			m_vUniversID.SetCount(Count); 
			for(m_vUniversID.MoveFirst(); m_vUniversID.IsValid(); m_vUniversID.MoveNext())
			{
				JInt32 & Id = m_vUniversID.GetItem();
				pFile->Recv(Id);
				PTR_CBITARRAY pData(new CBitArray);
				pData->SetSize(m_lNbIndividus);
				pData->Recv(*pFile);
				m_mpUniversIndiv.Add(Id) = pData;
			}

			// Get the number of type poids and load them
			pFile->Recv(Count);
			m_vTypePoids.SetCount(Count);
			for(m_vTypePoids.MoveFirst(); m_vTypePoids.IsValid(); m_vTypePoids.MoveNext())
			{
				m_vTypePoids.GetItem().Recv(*pFile,Version);
			}

			// Load the individual poids
			for(m_vUniversID.MoveFirst(); m_vUniversID.IsValid(); m_vUniversID.MoveNext())
			{
				LookupID lData;
				// Set the Univers ID
				lData.m_ID1 = m_vUniversID.GetItem();
				for(m_vTypePoids.MoveFirst(); m_vTypePoids.IsValid(); m_vTypePoids.MoveNext())
				{
					// Set the Poids ID
					lData.m_ID2 = m_vTypePoids.GetItem().GetID();
					// Load the poids
					PTR_JINT32VECTOR & vData = m_mvlPoids.Add(lData);
					vData = PTR_JINT32VECTOR(new JINT32VECTOR);
					vData->SetCount(m_lNbIndividus);
					for(vData->MoveFirst(); vData->IsValid(); vData->MoveNext())
						pFile->Recv(vData->GetItem());
				}
			}

			// Get the number of Echantillons
			pFile->Recv(Count);

			for(JInt32 x=0; x < Count; x++)
			{
				PTR_ECHANTILLON pItem(new CEchantillon);

				pItem->Recv(*pFile, Version, m_lNbIndividus);

				m_mEchantillons.Add(pItem->GetID()) = pItem;
			}
		}
		break;

		default:
			TRACE("CPoids Version error");
			throw JInternalError::GetInstance(); //("File Version error");
		break;
	}
}

/// Returns a bit array of the individus belonging to the univers
const CBitArray	* CPoids::GetUniversIndivs(JInt32 Univers) const
{
	m_mpUniversIndiv.MoveTo(Univers);
	if(m_mpUniversIndiv.IsValid())
		return m_mpUniversIndiv.GetItem().get();
	return 0;
}

// Returns true if this terrain has a given Echantillion and Statistic Unit
JBool	CPoids::HasStatisticValues(JInt32 EchantillonID, JInt32 UniversID, JInt32 TypePoidsID) const
{
	m_mEchantillons.MoveTo(EchantillonID);
	if(m_mEchantillons.IsValid())
	{
		return m_mEchantillons.GetItem()->HasUnivers(UniversID);
	}
	return false;
}

// Returns the Statistical unit vector for a given Echantillion and Statistic Unit
const JDBLVECTOR *	CPoids::GetStatisticValues(JInt32 EchantillonID, JInt32 UniversID, JInt32 TypePoidsID)
{
	m_mEchantillons.MoveTo(EchantillonID);
	if(m_mEchantillons.IsValid())
	{
		if(m_mEchantillons.GetItem()->HasUnivers(UniversID))
		{
			LookupID lData;
			lData.m_ID1		= UniversID;
			lData.m_ID2		= TypePoidsID;
			m_mvfPoids.MoveTo(lData);
			if(m_mvfPoids.IsValid())
				return m_mvfPoids.GetItem().get();
			else
			{
				m_mvlPoids.MoveTo(lData);
				if(m_mvlPoids.IsValid())
				{
					JInt32 Divider = GetDivider(TypePoidsID);
					// Load the poids
					const PTR_JINT32VECTOR & vlData = m_mvlPoids.GetItem();
					PTR_JDBLVECTOR & vfData = m_mvfPoids.Add(lData);
					vfData = PTR_JDBLVECTOR(new JDBLVECTOR);
					vfData->SetCount(vlData->GetCount());
					// Calculate the poids
					for(vlData->MoveFirst(),vfData->MoveFirst(); vlData->IsValid(); vlData->MoveNext(), vfData->MoveNext())
					{
						// JInt32 Val = vlData->GetItem();
						vfData->GetItem() = (double)((double)vlData->GetItem()/(double)Divider);
						// double Val2 = vfData->GetItem();
					}
                    return vfData.get();					
				}
			}
		}
	}
	return 0;
}

// Returns the individual mask for the given Echantillon
const CBitArray * CPoids::GetEchantillonFilter(JInt32 EchantillonID) const
{
	m_mEchantillons.MoveTo(EchantillonID);
	if(m_mEchantillons.IsValid())
	{
		return m_mEchantillons.GetItem()->GetIndividus();
	}
	return 0;
}

// Returns vector of valid univers
const JINT32VECTOR *	CPoids::GetUnivers(JInt32 EchantillonID) const
{
	m_mEchantillons.MoveTo(EchantillonID);
	if(m_mEchantillons.IsValid())
	{
		return m_mEchantillons.GetItem()->GetUnivers();
	}
	return 0;
}

//// Returns the facteurs d'homothétie
//const JINT32VECTOR *	CPoids::GetFacteurs(JInt32 EchantillonID) const
//{
//	m_mEchantillons.MoveTo(EchantillonID);
//	if(m_mEchantillons.IsValid())
//	{
//		return m_mEchantillons.GetItem()->GetFacteurs();
//	}
//	return 0;
//}

// Returns the facteur for a given univers or 1.0 if not found
double	CPoids::GetFacteur(JInt32 EchantillonID, JInt32 UniversID) const
{
	m_mEchantillons.MoveTo(EchantillonID);
	if(m_mEchantillons.IsValid())
	{
		return m_mEchantillons.GetItem()->GetFacteur(UniversID);
	}
	return 0;
}

	// Find Divider
JInt32 CPoids::GetDivider(JInt32 TypePoidsID) const
{
	for(m_vTypePoids.MoveFirst(); m_vTypePoids.IsValid(); m_vTypePoids.MoveNext())
	{
		if(m_vTypePoids.GetItem().GetID() == TypePoidsID)
			return m_vTypePoids.GetItem().GetDivider();
	}
	return 1;
}

// Returns vector of poids
JBool CPoids::GetTypesPoids(JVector<JInt32>& vect) const
{
	vect.SetCount ( m_vTypePoids.GetCount() );
	vect.MoveFirst();
	for(m_vTypePoids.MoveFirst(); m_vTypePoids.IsValid(); m_vTypePoids.MoveNext(), vect.MoveNext() )
	{
		vect.GetItem() = m_vTypePoids.GetItem().GetID();
	}
	return true;
}

///	Returns true if poids ID exists
JBool	CPoids::HasTypePoids(JInt32 poidsID) const
{
	for(m_vTypePoids.MoveFirst(); m_vTypePoids.IsValid(); m_vTypePoids.MoveNext())
	{
		if(poidsID == m_vTypePoids.GetItem().GetID())
			return true;
	}
	return false;
}

// Returns the default univers
JInt32	CPoids::GetDefaultUnivers() const
{
	m_vUniversID.MoveFirst();
	if(m_vUniversID.IsValid())
		return m_vUniversID.GetItem();
	else
		return 0;
}

/// Returns the default type Poids ID
JInt32	CPoids::GetDefaultTypePoidsID() const
{
	m_vTypePoids.MoveFirst();
	if(m_vTypePoids.IsValid())
		return m_vTypePoids.GetItem().GetID();
	return 0;
}

/// Returns the default Echantillon ID for a given univers
JInt32	CPoids::GetDefaultEchantillonID(JInt32 UniversID) const
{
	for(m_mEchantillons.MoveFirst(); m_mEchantillons.IsValid();  m_mEchantillons.MoveNext())
	{
		if(m_mEchantillons.GetItem()->HasUnivers(UniversID))
			return m_mEchantillons.GetKey();
	}
	return 0;
}