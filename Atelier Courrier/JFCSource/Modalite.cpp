// Modalite.cpp: implementation of the CModalite class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Modalite.h"
#include <stdlib.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CModalite::CModalite()
{
	m_ID = 0;
	m_Mask = 0xFFFF;
	m_Label = "";
	m_Type = 0;
}

CModalite::~CModalite()
{

}

JArchive& CModalite::Recv(JArchive& lhs, JInt32 Version)
{
	switch(Version)
	{
		case 1:
		{
			lhs.Recv(m_ID);
			lhs.Recv(m_Mask);
			lhs.Recv(m_Type);
			m_Label.Recv(lhs);
		}
		break;

		default:
			TRACE("CModalite Version error");
			throw JInternalError::GetInstance(); //("File Version error");
		break;
	}
	return lhs;
}

JBool	CModalite::ToItemString(JStringEx &String) const
{
	JChar t[20];
	_itoa_s(m_ID,t,20,10);
	String = "M ";
	String.Append(t);
	return true;
}
