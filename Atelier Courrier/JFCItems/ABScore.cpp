// ABScore.cpp: implementation of the CABScore class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ABScore.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CABScore::CABScore()
{

}


CABScore::~CABScore()
{
	Reset();
}

// Converts this object to a string
JBool	CABScore::AsString(std::string & str) const
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
