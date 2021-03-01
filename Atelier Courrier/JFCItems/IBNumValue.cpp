// IBNumValue.cpp: implementation of the CIBNumValue class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "IBNumValue.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CIBNumValue::CIBNumValue()
{
}

CIBNumValue::~CIBNumValue()
{
	Reset();
}

// Resets to zero this object
JBool	CIBNumValue::Reset()
{
	m_Value.Reset();
	SetOperator(IBloc::Nop);
	return true;
}

// Is this object Empty
JBool	CIBNumValue::IsEmpty()
{
	if(m_Value.GetCount())
		return true;
	return false;
}

// Validate this object removing all empty sub objects
JBool	CIBNumValue::Validate()
{
	if(m_Operator == IBloc::ALL || m_Value.GetCount())
		return true;
	return false;
}

// Validate this object removing all empty sub objects
JBool	CIBNumValue::IsValid()
{
	if(m_Operator == IBloc::ALL || m_Value.GetCount())
		return true;
	return false;
}


// Converts this object to a string
JBool	CIBNumValue::AsString(std::string & str) const
{
	str.append(_T("V"));
	OperatorAsString(str, m_Operator);

	for(m_Value.MoveFirst(); m_Value.IsValid(); m_Value.MoveNext())
	{
		str.append(_T("%"));
		str.append(FltToString(m_Value.GetItem()));
	}

	return true;
}

// Builds this object from a string
JBool	CIBNumValue::FromString(const JChar ** pstr)
{
	JBool NewBloc = false;

	while(**pstr != ')')
	{

		switch(**pstr)
		{
			case '*': // Logical Not
			{
				SetOperator(IBloc::ALL);
				(*pstr)++;
			}
			break;

			case '%': // Value
			{
				++(*pstr);
				m_Value.AddTail() = ToFltValue(pstr);
			}
			break;

			default:
				return false;

		}
	}
	return true;
}

// Get the values 
JVoid CIBNumValue::GetValues(JList<JFlt32>	&Values)
{
	Values.Reset();
	for(m_Value.MoveFirst(); m_Value.IsValid(); m_Value.MoveNext())
	{
		Values.AddTail() = m_Value.GetItem();
	}
}

// Sets the values
JVoid CIBNumValue::SetValues(JList<JFlt32> &Values)
{
	m_Value.Reset();

	for(Values.MoveFirst(); Values.IsValid(); Values.MoveNext())
	{
		m_Value.AddTail() = Values.GetItem();
	}
}
