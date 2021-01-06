// VariableNum.cpp: implementation of the CVariableNum class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "VariableNum.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CVariableNum::CVariableNum()
{
	m_lNbIndivUtil = 0;
	m_lDiviseur = 0;
}

CVariableNum::~CVariableNum()
{
}

JArchive& CVariableNum::Recv( JArchive& lhs, JInt32 Version, JInt32 NbIndividuals)
{
	switch(Version)
	{
		case 1:
		{
			lhs.Recv(m_lNbIndivUtil);
			lhs.Recv(m_lDiviseur);

			m_Repondants.SetSize(NbIndividuals);
//			m_Repondants.SetAll(true);
			m_Repondants.Recv(lhs);

			m_Individus.SetSize(NbIndividuals);
			m_Individus.Recv(lhs);

			m_vValues.SetCount(m_lNbIndivUtil);

			long NbVal = 0;
			for(m_vValues.MoveFirst(); m_vValues.IsValid(); m_vValues.MoveNext())
			{
				lhs.Recv(m_vValues.GetItem());
				NbVal++;
			}
			bool Pk = true;
		}
		break;

		default:
			TRACE("CVariableNum   Invalid Version");
			throw JInternalError::GetInstance(); //("File Version error");
		break;
	}
	return lhs;
}

const JFLT32VECTOR * CVariableNum::GetNumValues()
{
	if(!m_vFValues.GetCount())
	{
		m_vFValues.SetCount(m_Individus.GetSize());
		m_vValues.MoveFirst();

		JInt32 x = 0;
		for(m_vFValues.MoveFirst(); m_vFValues.IsValid(); x++, m_vFValues.MoveNext())
		{
			if(m_Repondants.GetAt(x))
			{
				if(m_Individus.GetAt(x))
				{
					m_vFValues.GetItem() = ((JFlt32)m_vValues.GetItem() / m_lDiviseur);
					m_vValues.MoveNext();
				}
				else
					m_vFValues.GetItem() = 0.0;
			}
			else
				m_vFValues.GetItem() = -1.0;
		}
	}
	return &m_vFValues;
}
