// RepereAbsolu.cpp: implementation of the CRepereAbsolu class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "RepereAbsolu.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CRepereAbsolu::CRepereAbsolu(JInt32 ItemNatif)
:m_lItemNatif(ItemNatif)
{
	m_lRepereID = 0;
}

CRepereAbsolu::~CRepereAbsolu()
{

}

JArchive&	CRepereAbsolu::Recv(JArchive& lhs, JInt32 Version)
{
	switch(Version)
	{
		case 1:
		{
			lhs.Recv(m_lRepereID);
		}
		break;

		default:
			TRACE("CRepereAbsolu  Invalid Version");
			throw JInternalError::GetInstance(); //("File Version error");
		break;
	}
	return lhs;
}
