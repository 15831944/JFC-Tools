#include "stdafx.h"
#include ".\AttributSupport.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CAttributSupport::CAttributSupport(void)
{
	m_Ids.Reset();
	m_NbAttributs = 0;
}

CAttributSupport::~CAttributSupport(void)
{
}

JArchive& CAttributSupport::Recv( JArchive& lhs, JInt32 Version, JInt32 NbDimensions)
{
	switch(Version)
	{
		case 1:
		{
			m_Ids.SetCount(NbDimensions);
			for(m_Ids.MoveFirst(); m_Ids.IsValid(); m_Ids.MoveNext())
				lhs.Recv(m_Ids.GetItem());

			lhs.Recv(m_NbAttributs);
			m_vAttributs.SetCount(m_NbAttributs);
			for(m_vAttributs.MoveFirst() ; m_vAttributs.IsValid(); m_vAttributs.MoveNext())
                lhs.Recv(m_vAttributs.GetItem());
		}
		break;

		default:
			TRACE("CAttributSupport   Invalid Version");
			throw JInternalError::GetInstance(); //("File Version error");
		break;
	}
	return lhs;
}
