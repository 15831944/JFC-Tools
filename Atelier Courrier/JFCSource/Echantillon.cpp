// Echantillon.cpp: implementation of the CEchantillon class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Echantillon.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CEchantillon::CEchantillon()
{
	m_ID = 0;
	m_DiviseurH = 1;
}

CEchantillon::~CEchantillon()
{
}

JArchive& CEchantillon::Recv(JArchive& lhs, JInt32 Version, JInt32 NbIndividuals)
{
	switch(Version)
	{
		case 1:
		{
			lhs.Recv(m_ID);
			lhs.Recv(m_bEchantillon);
			m_Individus.SetSize(NbIndividuals);
			m_Individus.Recv(lhs);

			// Diviseur d'homothétie
			lhs.Recv(m_DiviseurH);

			// Get number of Univers
			JInt32 Count;
			lhs.Recv(Count);

			m_vUniversID.SetCount(Count);
			m_vFacteur.SetCount(Count);

			for(m_vUniversID.MoveFirst(),m_vFacteur.MoveFirst(); m_vUniversID.IsValid(); m_vUniversID.MoveNext(), m_vFacteur.MoveNext())
			{
				lhs.Recv(m_vUniversID.GetItem());
				lhs.Recv(m_vFacteur.GetItem());
			}
		}
		break;

		default:
			TRACE("CEchantillon :: File Version error");
			throw JInternalError::GetInstance(); //("File Version error");
		break;
	}
	return lhs;
}

// Returns true if the echantillon has the univers
JBool	CEchantillon::HasUnivers(JInt32 UniversID) const
{
	for(m_vUniversID.MoveFirst(); m_vUniversID.IsValid(); m_vUniversID.MoveNext())
	{
		if(m_vUniversID.GetItem() == UniversID)
			return true;
	}
	return false;
}

// Returns the facteur for a given univers or 1.0 if not found
double	CEchantillon::GetFacteur(JInt32 UniversID) const
{
	for(m_vUniversID.MoveFirst(); m_vUniversID.IsValid(); m_vUniversID.MoveNext())
	{
		if(m_vUniversID.GetItem() == UniversID)
		{
			m_vFacteur.MoveTo(m_vUniversID.GetIndex());
			if(m_vFacteur.IsValid())
				return m_vFacteur.GetItem()/m_DiviseurH;
		}
	}
	return 1;
}
