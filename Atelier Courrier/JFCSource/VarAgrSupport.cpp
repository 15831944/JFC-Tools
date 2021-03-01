// VarAgregeSupport.cpp: implementation of the CVarAgregeSupport class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "VarAgrSupport.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CDimSupportKey::CDimSupportKey()
{
	m_Ids.Reset(); 
}

CDimSupportKey::CDimSupportKey(const CDimSupportKey &rhs)
{
	*this=rhs;
}

CDimSupportKey::CDimSupportKey(const JINT32VECTOR &rhs)
{
	*this=rhs;
}

CDimSupportKey & CDimSupportKey::operator=(const JINT32VECTOR &rhs)
{
	if(rhs.GetCount())
	{
		m_Ids.SetCount(rhs.GetCount());
		for(m_Ids.MoveFirst(),rhs.MoveFirst(); m_Ids.IsValid(); 
			m_Ids.MoveNext(),rhs.MoveNext())
		{
			m_Ids.GetItem() =  rhs.GetItem();
		}
	}
	else
		m_Ids.Reset();

	return *this;
}

CDimSupportKey & CDimSupportKey::operator=(const CDimSupportKey &rhs)
{
	*this = rhs.m_Ids;
	return *this;
}

CDimSupportKey::~CDimSupportKey(void)
{
}

bool CDimSupportKey::operator < (const CDimSupportKey &rhs) const
{
	if(m_Ids.GetCount() != rhs.m_Ids.GetCount())
		return false;

	for(m_Ids.MoveFirst(),rhs.m_Ids.MoveFirst(); m_Ids.IsValid(); m_Ids.MoveNext(),rhs.m_Ids.MoveNext())
	{
		if(m_Ids.GetItem() < rhs.m_Ids.GetItem())
			return true;
		if(m_Ids.GetItem() > rhs.m_Ids.GetItem())
			return false;
	}
	return false;
}

bool CDimSupportKey::operator > (const CDimSupportKey &rhs) const
{
	if(m_Ids.GetCount() != rhs.m_Ids.GetCount())
		return false;

	for(m_Ids.MoveFirst(),rhs.m_Ids.MoveFirst(); m_Ids.IsValid(); m_Ids.MoveNext(),rhs.m_Ids.MoveNext())
	{
		if(m_Ids.GetItem() > rhs.m_Ids.GetItem())
			return true;
		if(m_Ids.GetItem() < rhs.m_Ids.GetItem())
			return false;
	}
	return false;
}

/////////////////////////////////////////////////////////////////////////////////

CVarAgrSupport::CVarAgrSupport(JInt32 Nature)
{
	m_NbIndividus = 0;
	m_Nature = Nature;
	m_Ids.Reset();
	m_lNbIndivUtil = 0;
	m_vValues.Reset();
	m_vFValues.Reset(); 
}

CVarAgrSupport::~CVarAgrSupport()
{

}

JArchive& CVarAgrSupport::Recv( JArchive& lhs, JInt32 Version, JInt32 NbIndividuals, JInt32 NbDimensions)
{
	m_NbIndividus = NbIndividuals;

	switch(Version)
	{
		case 1:
		{
			m_Ids.SetCount(NbDimensions);
			for(m_Ids.MoveFirst(); m_Ids.IsValid(); m_Ids.MoveNext())
				lhs.Recv(m_Ids.GetItem());

			lhs.Recv(m_IdUniverse);
			lhs.Recv(m_lNbIndivUtil);
			// Values to be loaded
			switch(m_Nature)
			{
				case 1:
				{
					m_Individus.SetSize (m_NbIndividus);
					m_Individus.Recv(lhs);
					m_vValues.SetCount(m_lNbIndivUtil);
					for(m_vValues.MoveFirst(); m_vValues.IsValid(); m_vValues.MoveNext())
						lhs.Recv(m_vValues.GetItem());
				}
				break;

				case 2:
				{
					m_vIndividus.SetCount(m_lNbIndivUtil);
					for(m_vIndividus.MoveFirst(); m_vIndividus.IsValid(); m_vIndividus.MoveNext())
						lhs.Recv(m_vIndividus.GetItem());
					m_vValues.SetCount(m_lNbIndivUtil);
					for(m_vValues.MoveFirst(); m_vValues.IsValid(); m_vValues.MoveNext())
						lhs.Recv(m_vValues.GetItem());
				}
				break;

				case 3:
				{
					m_Individus.SetSize (m_NbIndividus);
					m_Individus.Recv(lhs);
				}
				break;
			}
		}
		break;

		default:
			TRACE("CVarAgrSupport  Invalid Version");
			throw JInternalError::GetInstance(); //("File Version error");
		break;
	}
	return lhs;
}

const CBitArray * CVarAgrSupport::GetBinValues()
{
	if(m_Nature != 3)
		return 0;
	return &m_Individus;
}

const JFLT32VECTOR * CVarAgrSupport::GetNumValues(JInt32 Diviseur)
{
	if(!m_vFValues.GetCount())
	{
		m_vFValues.SetCount(m_NbIndividus);
		m_vValues.MoveFirst();

		JInt32 x = 0;

		switch(m_Nature)
		{
			case 1:
			{
				for(m_vFValues.MoveFirst() ; m_vFValues.IsValid(); x++, m_vFValues.MoveNext())
				{
					if(m_Individus.GetAt(x))
					{
						m_vFValues.GetItem() = (JFlt32)((double)m_vValues.GetItem() / (double)Diviseur);
						m_vValues.MoveNext();
					}
					else
						m_vFValues.GetItem() = 0.0;
				}
			}
			break;

			case 2:
			{
				m_vIndividus.MoveFirst();
				m_vValues.MoveFirst();
				for(m_vFValues.MoveFirst() ; m_vFValues.IsValid(); ++x, m_vFValues.MoveNext())
				{
					if(m_vIndividus.IsValid() && m_vIndividus.GetItem() == x)
					{
						m_vFValues.GetItem() = (JFlt32)((double)m_vValues.GetItem() / (double)Diviseur);
						m_vValues.MoveNext();
						m_vIndividus.MoveNext();
					}
					else
						m_vFValues.GetItem() = 0.0;
				}
			}
			break;

			case 3:
			{
				for(m_vFValues.MoveFirst() ; m_vFValues.IsValid(); x++, m_vFValues.MoveNext())
				{
					if(m_Individus.GetAt(x))
						m_vFValues.GetItem() = 1.0;
					else
						m_vFValues.GetItem() = 0.0;
				}
			}
			break;
		}
	}
	return &m_vFValues;
}
