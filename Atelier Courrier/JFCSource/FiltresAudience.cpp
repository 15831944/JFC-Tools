#include "stdafx.h"
#include ".\filtresaudience.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

vLookupID::vLookupID(void)
{
}

vLookupID::vLookupID(const vLookupID &rhs)
{
	*this = rhs;
}

vLookupID::~vLookupID(void)
{
}

vLookupID & vLookupID::operator = (const vLookupID &rhs)
{
	m_vID1.SetCount(rhs.m_vID1.GetCount());
	for(m_vID1.MoveFirst(), rhs.m_vID1.MoveFirst(); m_vID1.IsValid(); m_vID1.MoveNext(), rhs.m_vID1.MoveNext())
		m_vID1.GetItem() = rhs.m_vID1.GetItem();

	m_ID2 = rhs.m_ID2;

	return *this;
}

vLookupID & vLookupID::Set(const JINT32VECTOR & vID1, JInt32 ID2)
{
	m_vID1.SetCount(vID1.GetCount());
	for(m_vID1.MoveFirst(), vID1.MoveFirst(); m_vID1.IsValid(); m_vID1.MoveNext(), vID1.MoveNext())
		m_vID1.GetItem() = vID1.GetItem();

	m_ID2 = ID2;

	return *this;
}

bool vLookupID::operator < (const vLookupID &rhs) const
{
	if(m_vID1.GetCount() < rhs.m_vID1.GetCount())
		return true;
	if(m_vID1.GetCount() == rhs.m_vID1.GetCount())
	{
		for(m_vID1.MoveFirst(), rhs.m_vID1.MoveFirst(); m_vID1.IsValid(); m_vID1.MoveNext(), rhs.m_vID1.MoveNext())
			if(m_vID1.GetItem() != rhs.m_vID1.GetItem())
				return (m_vID1.GetItem() < rhs.m_vID1.GetItem());

		return m_ID2 < rhs.m_ID2;
	}
	
	return false;
}

bool vLookupID::operator > (const vLookupID &rhs) const
{
	if(m_vID1.GetCount() > rhs.m_vID1.GetCount())
		return true;

	if(m_vID1.GetCount() == rhs.m_vID1.GetCount())
	{
		for(m_vID1.MoveFirst(), rhs.m_vID1.MoveFirst(); m_vID1.IsValid(); m_vID1.MoveNext(), rhs.m_vID1.MoveNext())
			if(m_vID1.GetItem() != rhs.m_vID1.GetItem())
				return (m_vID1.GetItem() > rhs.m_vID1.GetItem());

		return m_ID2 > rhs.m_ID2;
	}

	return false;
}


CFiltresAudience::CFiltresAudience(void)
{
}

CFiltresAudience::~CFiltresAudience(void)
{
}

JBool CFiltresAudience::Load(JStringEx &Name)
{

	m_sName = Name;
	m_sName.Append (".04");

	try
	{
		JArchive *pFile = JArchive::Create(m_sName.AsJCharPtr());

		LoadFileHeader		(pFile);
		LoadHeader			(pFile, m_FileHeader.GetVersion ());
		LoadFiltres  		(pFile, m_FileHeader.GetVersion ());
		delete pFile;

	}
	catch(JException *pE)
	{
		return false;
	}
	return true;
}

JVoid CFiltresAudience::LoadFileHeader(JArchive *pFile)
{
	const JInt32 CURRENT_VERSION = 1;

	m_FileHeader.Recv(*pFile);
	
	// Check compagnie name
	if(m_FileHeader.GetCompagnie () != 0x0043464A)
		throw JInternalError::GetInstance(); //("File Header, compagnie field error");

	// Verify the magic number
	if(m_FileHeader.GetMagicNumber () != 0x6FD35904)
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

JVoid CFiltresAudience::LoadHeader(JArchive *pFile, JInt32 Version)
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
			TRACE("CFiltresAudience   Invalid Version");
			throw JInternalError::GetInstance(); //("File Version error");
		break;
	}
}

JVoid CFiltresAudience::LoadFiltres (JArchive *pFile, JInt32 Version)
{
	switch(Version)
	{
		case 1:
		{
			// Get number of individuals
			pFile->Recv(m_lNbIndividus);
			// Get the number of dimensions primordial
			pFile->Recv(m_NbDimPrim);
			m_vRepereDimPrim.SetCount(m_NbDimPrim);
			for(m_vRepereDimPrim.MoveFirst(); m_vRepereDimPrim.IsValid(); m_vRepereDimPrim.MoveNext())
				pFile->Recv(m_vRepereDimPrim.GetItem());

			// Get number of Filters
			JInt32 Count;
			pFile->Recv(Count);

			m_vFiltres.SetCount(Count);

			for( m_vFiltres.MoveFirst(); m_vFiltres.IsValid(); m_vFiltres.MoveNext())
			{
				PTR_FILTREAUDIENCE pItem(new CFiltreAudience);

				pItem->Recv(*pFile, Version, m_lNbIndividus, m_NbDimPrim);

				const JINT32VECTOR & Supports = pItem->GetSupportID();
				vLookupID lData;
				lData.Set(Supports,pItem->GetFiltreID());
				m_mFiltres.MoveTo(lData);
				if(m_mFiltres.IsValid())
					break;

				m_mFiltres.Add(lData) = pItem.get();
				m_vFiltres.GetItem() = pItem;
			}
		}
		break;

		default:
			TRACE("CFiltresAudience   Invalid Version");
			throw JInternalError::GetInstance(); //("File Version error");
		break;
	}
}

JBool CFiltresAudience::GetFiltres(JVector<CFiltreAudience  *> & Filtres)
{
	Filtres.SetCount(m_vFiltres.GetCount());
	for(Filtres.MoveFirst(),m_vFiltres.MoveFirst(); m_vFiltres.IsValid(); Filtres.MoveNext(), m_vFiltres.MoveNext())
	{
		Filtres.GetItem() = m_vFiltres.GetItem().get();
	}
	return true;
}

// Returns the Audience filter of a given Support
const CFiltreAudience * CFiltresAudience::GetFilter(JINT32VECTOR & ReperePrim, JINT32VECTOR & SupportID, JInt32 FiltreID)
{
	vLookupID lData;
	if(SupportID.GetCount() != m_NbDimPrim)
		return 0;

	lData.m_vID1.SetCount(m_NbDimPrim);
	for(m_vRepereDimPrim.MoveFirst() ; m_vRepereDimPrim.IsValid() ; m_vRepereDimPrim.MoveNext())
	{
		for(ReperePrim.MoveFirst() ; ReperePrim.IsValid() ; ReperePrim.MoveNext())
		{
			if(m_vRepereDimPrim.GetItem() == ReperePrim.GetItem())
				break;
		}
		if(!ReperePrim.IsValid())
			return 0;

		lData.m_vID1.MoveTo(m_vRepereDimPrim.GetIndex());
		SupportID.MoveTo(ReperePrim.GetIndex());
		lData.m_vID1.GetItem() = SupportID.GetItem();
	}
	lData.m_ID2 = FiltreID;

	m_mFiltres.MoveTo(lData);
	if(m_mFiltres.IsValid())
		return m_mFiltres.GetItem();

	return 0;
}
