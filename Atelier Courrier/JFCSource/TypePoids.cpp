// TypePoids.cpp: implementation of the CTypePoids class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "TypePoids.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CTypePoids::CTypePoids()
{
	m_ID = 0;
	m_lDiviseur = 0;
}

CTypePoids::~CTypePoids()
{

}

JArchive& CTypePoids::Recv(JArchive& lhs, JInt32 Version)
{
	switch(Version)
	{
		case 1:
		{
			lhs.Recv(m_ID);
			lhs.Recv(m_lDiviseur);
		}
		break;

		default:
			TRACE("CTypePoids Invalid Version");
			throw JInternalError::GetInstance(); //("File Version error");
		break;
	}
	return lhs;
}


