// ABNumValue.cpp: implementation of the CABNumValue class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ABNumValue.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CABNumValue::CABNumValue()
{

}

CABNumValue::~CABNumValue()
{
	Reset();
}

// Resets to zero this object
JBool	CABNumValue::Reset()
{
	m_Value.Reset();
	SetOperator(IBloc::Nop);
	return true;
}

// Is this object Empty
JBool	CABNumValue::IsEmpty()
{
	if(m_Value.GetCount())
		return true;
	return false;
}

// Validate this object removing all empty sub objects
JBool	CABNumValue::Validate()
{
	if(m_Operator == IBloc::ALL || m_Value.GetCount())
		return true;
	return false;
}

// Validate this object removing all empty sub objects
JBool	CABNumValue::IsValid()
{
	if(m_Operator == IBloc::ALL || m_Value.GetCount())
		return true;
	return false;
}

// Converts this object to a string
JBool	CABNumValue::AsString(std::string & str) const
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
JBool	CABNumValue::FromString(const JChar ** pstr)
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

// Sets the values
JVoid CABNumValue::SetValues(JList<JFlt32> &Values)
{
	m_Value.Reset();

	for(Values.MoveFirst(); Values.IsValid(); Values.MoveNext())
	{
		m_Value.AddTail() = Values.GetItem();
	}
}

// Get the values 
JVoid CABNumValue::GetValues(JList<JFlt32>	&Values)
{
	Values.Reset();
	for(m_Value.MoveFirst(); m_Value.IsValid(); m_Value.MoveNext())
	{
		Values.AddTail() = m_Value.GetItem();
	}
}

// Get the values 
JVoid CABNumValue::GetValues(JVector<JFlt32>	&Values)
{
	Values.SetCount(m_Value.GetCount());
	for(Values.MoveFirst(), m_Value.MoveFirst(); m_Value.IsValid(); Values.MoveNext(), m_Value.MoveNext())
	{
		Values.GetItem() = m_Value.GetItem();
	}
}
