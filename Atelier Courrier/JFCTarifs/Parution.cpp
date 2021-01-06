// Parution.cpp: implementation of the CParution class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Parution.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CParution::CParution(void)
{
}

CParution::~CParution(void)
{
}

JUnt32 CParution::GetVersion() const
{
	return m_Version;
}

JUnt32 CParution::GetNumPar() const
{
	return m_NumPar;
}

JUnt32 CParution::GetDuree() const
{
	return m_Duree;
}


JArchive& CParution::Recv(JArchive & lhs, JInt32 Version)
{
	switch(Version)
	{
	case 1:
		lhs.Recv( m_Version );
		lhs.Recv( m_NumPar );
		lhs.Recv( m_Duree );
		break;

	default:
		TRACE("CParution   Invalid Version");
		throw JInternalError::GetInstance(); //("File Version error");
	}
	return lhs;	
}