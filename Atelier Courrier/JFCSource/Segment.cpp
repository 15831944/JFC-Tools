#include "stdafx.h"
#include ".\segment.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CSegment::CSegment(void)
{
	m_ID	= 0;
	m_xMask = 0;
}

CSegment::~CSegment(void)
{
}

JArchive& CSegment::Recv( JArchive& lhs, JInt32 Version, JInt32 NbIndividuals)
{
	switch(Version)
	{
		case 1:
		{
			lhs.Recv(m_ID);
			lhs.Recv(m_xMask);
			m_Individus.SetSize (NbIndividuals);
			m_Individus.Recv(lhs);
		}
		break;

		default:
			TRACE("CSegment   Invalid Version");
			throw JInternalError::GetInstance(); //("File Version error");
		break;
	}
	return lhs;
}
