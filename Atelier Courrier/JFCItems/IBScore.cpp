// IBScore.cpp: implementation of the CIBScore class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "IBScore.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CIBScore::CIBScore()
{

}

CIBScore::~CIBScore()
{

}

// Converts this object to a string
JBool	CIBScore::AsString(std::string & str) const
{
	str.append(_T("R"));
	OperatorAsString(str, m_Operator);

	for(m_Value.MoveFirst(); m_Value.IsValid(); m_Value.MoveNext())
	{
		str.append(_T("%"));
		str.append(IntToString(m_Value.GetItem()));
	}

	return true;
}
