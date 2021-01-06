// IBQuant.cpp: implementation of the CIBQuant class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "IBQuant.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CIBQuant::CIBQuant()
{
}

CIBQuant::~CIBQuant()
{

}

// Resets to zero this object
JBool	CIBQuant::Reset()
{
	m_Value.Reset();
	SetOperator(IBloc::Nop);
	return true;
}

// Is this object Empty
JBool	CIBQuant::IsEmpty()
{
	if(m_Value.GetCount() == 2)
		return false;
	return true;
}

// Validate this object removing all empty sub objects
JBool	CIBQuant::Validate()
{
	if(m_Value.GetCount() == 2)
		return true;
	return false;
}

// Validate this object removing all empty sub objects
JBool	CIBQuant::IsValid()
{
	if(m_Value.GetCount() == 2)
		return true;
	return false;
}

// Converts this object to a string
JBool	CIBQuant::AsString(std::string & str) const
{
	str.append(_T("Q"));
	OperatorAsString(str, m_Operator);

	for(m_Value.MoveFirst(); m_Value.IsValid(); m_Value.MoveNext())
	{
		str.append(_T("%"));
		str.append(IntToString(m_Value.GetItem()));
	}

	return true;
}

// Builds this object from a string
JBool	CIBQuant::FromString(const JChar ** pstr)
{
	JBool NewBloc = false;

	while(**pstr != ')')
	{

		switch(**pstr)
		{
			case '%': // Value
			{
				++(*pstr);
				m_Value.AddTail() = ToIntValue(pstr);
			}
			break;

			default:
				return false;

		}
	}
	return true;
}

void CIBQuant::GetValues(JList<JInt32> & Values)
{
	Values.Reset();
	for(m_Value.MoveFirst(); m_Value.IsValid(); m_Value.MoveNext())
	{
		Values.AddTail() = m_Value.GetItem();
	}
}

// Sets the values
JVoid CIBQuant::SetValues(JList<JInt32> &Values)
{
	m_Value.Reset();

	for(Values.MoveFirst(); Values.IsValid(); Values.MoveNext())
	{
		m_Value.AddTail() = Values.GetItem();
	}
}

JVoid CIBQuant::AddValue(JInt32 Value)
{
	m_Value.AddTail() = Value;
}
