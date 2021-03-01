#include "stdafx.h"
#include ".\filtreaudience.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CFiltreAudience::CFiltreAudience(void)
{
	// Support IDs
	m_vIdSupport.Reset();
	// Filter ID
	m_IdFilter	=	0;
	// Audience Filter
	m_Filter.SetSize(0);
}

CFiltreAudience::~CFiltreAudience(void)
{
}

JArchive& CFiltreAudience::Recv( JArchive& lhs, JInt32 Version, JInt32 NbIndividuals, JInt32 NbDimPrim)
{
	switch(Version)
	{
		case 1:
		{
			m_vIdSupport.SetCount(NbDimPrim);
			for(m_vIdSupport.MoveFirst(); m_vIdSupport.IsValid(); m_vIdSupport.MoveNext())
				lhs.Recv(m_vIdSupport.GetItem());
			lhs.Recv(m_IdFilter);
			m_Filter.SetSize (NbIndividuals);
			m_Filter.Recv(lhs);
		}
		break;

		default:
			TRACE("CFiltreAudience   Invalid Version");
			throw JInternalError::GetInstance(); //("File Version error");
		break;
	}
	return lhs;
}
