// Element.cpp: implementation of the CElement class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Element.h"
#include <stdlib.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CElement::CElement()
{
	m_ID = 0;
	m_Type = 0;
	m_Label = "";
	m_Mask = 0xFFFF;
}

CElement::~CElement()
{
}

JArchive& CElement::Recv(JArchive& lhs, JInt32 Version)
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
			TRACE("CElement Version error");
			throw JInternalError::GetInstance(); //("Element Version error");
		break;
	}
	return lhs;
}

JBool	CElement::ToItemString(JStringEx &String) const
{
	JChar t[20];
	_itoa_s(m_ID,t,20,10);
	String = "E ";
	String.Append(t);
	return true;
}
