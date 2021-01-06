// ValSupport.cpp: implementation of the CValSupport class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ValSupport.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CValSupport::CValSupport()
{
	m_lSupportID = 0;
	// Diviseur du taux Moyen
	m_lDiviseurTM = 0;
	// Taux Moyen de reprise en main
	m_lRepriseMainTM = 0;
	// Diviseur de la structure de répartition
	m_lDiviseurRepart = 0;
	// Durée de vie
	m_lDureeVie = 0;
}

CValSupport::~CValSupport()
{
}

JArchive& CValSupport::Recv( JArchive& lhs, JInt32 Version)
{
	switch(Version)
	{
		case 1:
		{
			lhs.Recv(m_lSupportID);
			lhs.Recv(m_lDiviseurTM);
			lhs.Recv(m_lRepriseMainTM);
			lhs.Recv(m_lDiviseurRepart);
			lhs.Recv(m_lDureeVie);

			m_vValues.SetCount(m_lDureeVie);

			for( m_vValues.MoveFirst(); m_vValues.IsValid() ; m_vValues.MoveNext())
			{
				lhs.Recv(m_vValues.GetItem());
			}
		}
		break;

		default:
			TRACE("CValSupport Invalid Version");
			throw JInternalError::GetInstance(); //("File Version error");
		break;
	}
	return lhs;
}

JVoid CValSupport::GetRepartionStruct(JInt32 & Duree, JVector<JInt32>& vect, JInt32 & diviseur)
{
	Duree = m_lDureeVie;
	diviseur = m_lDiviseurRepart;
	vect.SetCount(m_lDureeVie);

	for( m_vValues.MoveFirst(), vect.MoveFirst(); vect.IsValid(); m_vValues.MoveNext(), vect.MoveNext())
		vect.GetItem() = m_vValues.GetItem();
}
